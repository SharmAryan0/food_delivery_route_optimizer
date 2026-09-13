#include "OrderManager.h"
#include <queue>
#include <algorithm>
#include <limits>
#include <climits>

OrderManager :: OrderManager(Graph& g, RestaurantManager& rm, CustomerManager& cm)
    : graph(g), restaurantManager(rm), customerManager(cm){
    nextOrderId = 1;
    nextBatchId = 1;
    clock = 0;
}

void OrderManager :: registerPartners(const vector<DeliveryPartner>& initialPartners){
    partners = initialPartners;
}

Order* OrderManager :: findOrder(int orderId){
    for(auto& o : orders){
        if(o.getOrderId() == orderId) return &o;
    }
    return nullptr;
}

int OrderManager :: findBatchIndexForOrder(int orderId) const{
    for(int i = 0; i < (int)batches.size(); i++){
        for(int oid : batches[i].orderIds){
            if(oid == orderId) return i;
        }
    }
    return -1;
}

// Re-sums a batch's total travel time leg by leg. Used any time the route
// is mutated (e.g. an order is cancelled out of it) instead of trying to
// patch the running total incrementally, which is easy to get subtly wrong.
void OrderManager :: recomputeBatchDistance(Batch& b) const{
    int total = 0;
    for(size_t i = 1; i < b.route.size(); i++){
        int leg = graph.distance(b.route[i-1], b.route[i]);
        if(leg > 0) total += leg;
    }
    b.totalDistance = total;
}

// ------------------------------------------------------------------
// Undo support (Memento pattern): every mutating public method calls this
// first to push a full copy of current state onto a stack. undo() just
// pops the most recent one back in.
// ------------------------------------------------------------------
void OrderManager :: pushUndoSnapshot(const string& actionDescription){
    StateSnapshot snap;
    snap.orders = orders;
    snap.batches = batches;
    snap.partners = partners;
    snap.restaurants = restaurantManager.getAllRestaurants();
    snap.nextOrderId = nextOrderId;
    snap.nextBatchId = nextBatchId;
    snap.clock = clock;
    snap.lastAction = actionDescription;

    undoStack.push_back(snap);
    if(undoStack.size() > MAX_UNDO_DEPTH){
        undoStack.erase(undoStack.begin()); // drop the oldest entry
    }
}

bool OrderManager :: undo(){
    if(undoStack.empty()){
        cout << "--------Nothing To Undo--------" << endl;
        return false;
    }

    StateSnapshot snap = undoStack.back();
    undoStack.pop_back();

    orders = snap.orders;
    batches = snap.batches;
    partners = snap.partners;
    restaurantManager.restoreAllRestaurants(snap.restaurants);
    nextOrderId = snap.nextOrderId;
    nextBatchId = snap.nextBatchId;
    clock = snap.clock;

    cout << "--------Undone: " << snap.lastAction << "--------" << endl;
    return true;
}

int OrderManager :: placeOrder(int customerId, int restaurantId, vector<int> foodIds){
    pushUndoSnapshot("Place Order");

    clock += 2; // small simulated gap between orders arriving
    Order o(nextOrderId, customerId, restaurantId, foodIds, clock);
    orders.push_back(o);
    cout << "--------Order #" << nextOrderId << " Placed Successfully (t=" << clock << "min)--------" << endl;
    return nextOrderId++;
}

// ------------------------------------------------------------------
// THE UNIQUE FEATURE: Smart Order Batching & Route Optimization
void OrderManager :: runSmartBatching(){

    vector<Order*> pending;
    for(auto& o : orders){
        if(o.getStatus() == PLACED) pending.push_back(&o);
    }
    sort(pending.begin(), pending.end(), [](Order* a, Order* b){
        return a->getPlacedTime() < b->getPlacedTime();
    });

    if(pending.empty()){
        cout << "--------No Pending Orders To Batch--------" << endl;
        return;
    }

    pushUndoSnapshot("Run Smart Batching");

    for(Order* o : pending){

        Restaurant r = restaurantManager.getRestaurant(o->getRestaurantId());
        Customer c = customerManager.getCustomer(o->getCustomerId());
        string R = r.getLocation();
        string C = c.getLocation();

        if(!graph.hasLocation(R) || !graph.hasLocation(C)){
            cout << "Skipping Order #" << o->getOrderId()
                 << " - location not on the map (use displayLocations to see valid names)\n";
            continue;
        }

        int baselineCost = graph.distance(HUB, R) + graph.distance(R, C);

        int bestBatchIdx = -1;
        int bestDetour = INT_MAX;

        // Look for the open batch (not yet assigned a partner) where
        // adding this order causes the smallest detour.
        for(int i = 0; i < (int)batches.size(); i++){
            Batch& b = batches[i];
            if(b.partnerId != -1) continue;                 // already dispatched
            if((int)b.orderIds.size() >= MAX_BATCH_SIZE) continue;

            string lastStop = b.route.back();
            int detour = graph.distance(lastStop, R);        // extra travel to reach the new pickup
            if(detour < 0) continue;                          // unreachable

            if(detour <= DETOUR_THRESHOLD && detour < bestDetour){
                bestDetour = detour;
                bestBatchIdx = i;
            }
        }

        if(bestBatchIdx != -1){
            // merge into the existing batch
            Batch& b = batches[bestBatchIdx];
            string lastStop = b.route.back();
            int addRC = graph.distance(R, C);

            b.route.push_back(R);   b.dropOrderId.push_back(-1);
            b.route.push_back(C);   b.dropOrderId.push_back(o->getOrderId());
            b.totalDistance += bestDetour + addRC;
            b.orderIds.push_back(o->getOrderId());

            o->setBatchId(b.batchId);

            int saving = baselineCost - (bestDetour + addRC);
            cout << "Order #" << o->getOrderId() << " merged into Batch #" << b.batchId
                 << " (detour " << bestDetour << "min, saves ~" << saving << "min vs solo delivery)\n";
        } else {
            // start a brand-new batch
            Batch b;
            b.batchId = nextBatchId++;
            b.route = {HUB, R, C};
            b.dropOrderId = {-1, -1, o->getOrderId()};
            b.totalDistance = baselineCost;
            b.startTime = o->getPlacedTime();
            b.orderIds.push_back(o->getOrderId());
            batches.push_back(b);

            o->setBatchId(b.batchId);
            cout << "Order #" << o->getOrderId() << " started new Batch #" << b.batchId << endl;
        }
    }
}

// ------------------------------------------------------------------
// Assigns the earliest-available delivery partner (min-heap keyed by
// free time) to each pending batch, then walks the optimized route to
// give every individual order an accurate ETA via Dijkstra distances.
// ------------------------------------------------------------------
void OrderManager :: assignPartners(){

    if(partners.empty()){
        cout << "--------No Delivery Partners Registered--------" << endl;
        return;
    }

    vector<int> pendingBatchIdx;
    for(int i = 0; i < (int)batches.size(); i++){
        if(batches[i].partnerId == -1 && !batches[i].orderIds.empty()) pendingBatchIdx.push_back(i);
    }

    if(pendingBatchIdx.empty()){
        cout << "--------No Batches Ready For Assignment--------" << endl;
        return;
    }

    pushUndoSnapshot("Assign Delivery Partners");

    // min-heap of (availableAt, partnerIndex)
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> freePartners;
    for(int i = 0; i < (int)partners.size(); i++){
        freePartners.push({partners[i].getAvailableAt(), i});
    }

    for(int idx : pendingBatchIdx){
        if(freePartners.empty()){
            cout << "No free delivery partners left for Batch #" << batches[idx].batchId << endl;
            continue;
        }

        auto top = freePartners.top();
        freePartners.pop();
        int availableAt = top.first;
        int partnerIdx = top.second;

        Batch& b = batches[idx];
        DeliveryPartner& p = partners[partnerIdx];

        int travelToStart = graph.distance(p.getCurrentLocation(), b.route.front());
        if(travelToStart < 0) travelToStart = 0; // fallback if hub unreachable from partner's spot

        int clockTime = max(availableAt, b.startTime) + travelToStart;

        // walk the route stop by stop, accumulating time, stamping ETAs
        for(size_t i = 1; i < b.route.size(); i++){
            int legDist = graph.distance(b.route[i-1], b.route[i]);
            if(legDist < 0) legDist = 0;
            clockTime += legDist;

            int orderIdHere = b.dropOrderId[i];
            if(orderIdHere != -1){
                Order* o = findOrder(orderIdHere);
                if(o){
                    o->setEta(clockTime);
                    o->setPartnerId(p.getId());
                }
            }
        }

        b.partnerId = p.getId();
        p.setAvailableAt(clockTime);
        p.setCurrentLocation(b.route.back());

        cout << "Batch #" << b.batchId << " (" << b.orderIds.size() << " order(s)) assigned to "
             << p.getName() << " [Partner ID " << p.getId() << "] | route: ";
        for(size_t i = 0; i < b.route.size(); i++){
            cout << b.route[i];
            if(i + 1 < b.route.size()) cout << " -> ";
        }
        cout << " | finishes at t=" << clockTime << "min\n";
    }
}

// ------------------------------------------------------------------
// Cancellation: only orders that haven't been dispatched to a partner yet
// can be cancelled (once a partner is en route, real delivery apps don't
// let you cancel either). If the order was merged into a batch, the pickup
// + drop stops it contributed to that batch's route are surgically removed
// and the batch's distance is recomputed from scratch.
// ------------------------------------------------------------------
bool OrderManager :: cancelOrder(int orderId){
    Order* o = findOrder(orderId);
    if(!o){
        cout << "------Order Not Found------" << endl;
        return false;
    }

    if(o->getStatus() == DELIVERED){
        cout << "------Cannot Cancel - Order Already Delivered------" << endl;
        return false;
    }
    if(o->getStatus() == ASSIGNED){
        cout << "------Cannot Cancel - A Delivery Partner Is Already En Route------" << endl;
        return false;
    }
    if(o->getStatus() == CANCELLED){
        cout << "------Order Is Already Cancelled------" << endl;
        return false;
    }

    pushUndoSnapshot("Cancel Order #" + to_string(orderId));

    int batchIdx = findBatchIndexForOrder(orderId);
    if(batchIdx != -1){
        Batch& b = batches[batchIdx];

        // Every order contributes exactly one consecutive [pickup, drop]
        // pair to the route; find the drop stop (tagged with this orderId)
        // and remove it along with the pickup stop right before it.
        for(int i = (int)b.route.size() - 1; i >= 1; i--){
            if(b.dropOrderId[i] == orderId){
                b.route.erase(b.route.begin() + i);
                b.dropOrderId.erase(b.dropOrderId.begin() + i);
                b.route.erase(b.route.begin() + (i - 1));
                b.dropOrderId.erase(b.dropOrderId.begin() + (i - 1));
                break;
            }
        }

        b.orderIds.erase(remove(b.orderIds.begin(), b.orderIds.end(), orderId), b.orderIds.end());

        if(b.orderIds.empty()){
            // nothing left in this batch at all - drop it entirely
            batches.erase(batches.begin() + batchIdx);
        } else {
            recomputeBatchDistance(b);
        }
    }

    o->setBatchId(-1);
    o->setStatus(CANCELLED);
    cout << "------Order #" << orderId << " Cancelled Successfully------" << endl;
    return true;
}

// ------------------------------------------------------------------
// Rating: only valid once an order has actually been delivered, and only
// once per order. Feeds straight into the restaurant's running average.
// ------------------------------------------------------------------
bool OrderManager :: rateOrder(int orderId, int stars){
    Order* o = findOrder(orderId);
    if(!o){
        cout << "------Order Not Found------" << endl;
        return false;
    }
    if(o->getStatus() != DELIVERED){
        cout << "------Order Hasn't Been Delivered Yet - Nothing To Rate------" << endl;
        return false;
    }
    if(o->getRating() != -1){
        cout << "------Order Already Rated (" << o->getRating() << "/5)------" << endl;
        return false;
    }
    if(stars < 1 || stars > 5){
        cout << "------Rating Must Be Between 1 And 5------" << endl;
        return false;
    }

    pushUndoSnapshot("Rate Order #" + to_string(orderId));

    o->setRating(stars);
    restaurantManager.rateRestaurant(o->getRestaurantId(), stars);
    cout << "--------Thanks! Recorded " << stars << "/5 For Order #" << orderId << "--------" << endl;
    return true;
}

// ------------------------------------------------------------------
// Advances the simulation clock. Any order that is ASSIGNED and whose ETA
// has now passed is automatically marked DELIVERED - this is what makes
// the delivery partners' availableAt times (stamped back in assignPartners)
// actually mean something across the run of the program.
// ------------------------------------------------------------------
void OrderManager :: advanceClock(int minutes){
    if(minutes <= 0){
        cout << "------Enter A Positive Number Of Minutes------" << endl;
        return;
    }

    pushUndoSnapshot("Advance Clock By " + to_string(minutes) + "min");

    clock += minutes;
    cout << "--------Simulation Clock Advanced To t=" << clock << "min--------" << endl;

    int deliveredCount = 0;
    for(auto& o : orders){
        if(o.getStatus() == ASSIGNED && o.getEta() != -1 && o.getEta() <= clock){
            o.setStatus(DELIVERED);
            cout << "  -> Order #" << o.getOrderId() << " has now arrived (ETA was t=" << o.getEta() << "min)\n";
            deliveredCount++;
        }
    }
    if(deliveredCount == 0){
        cout << "  (no orders reached their ETA yet)\n";
    }
}

void OrderManager :: trackOrder(int orderId) const{
    for(const auto& o : orders){
        if(o.getOrderId() == orderId){
            o.display();
            return;
        }
    }
    cout << "------Order Not Found------" << endl;
}

void OrderManager :: displayAllOrders() const{
    if(orders.empty()){
        cout << "--------No Orders Yet--------" << endl;
        return;
    }
    cout << "\n------ All Orders ------\n";
    for(const auto& o : orders){
        o.display();
    }
}

void OrderManager :: displayBatches() const{
    if(batches.empty()){
        cout << "--------No Batches Created Yet--------" << endl;
        return;
    }
    cout << "\n------ Batches ------\n";
    for(const auto& b : batches){
        cout << "Batch #" << b.batchId << " | Orders: ";
        for(int oid : b.orderIds) cout << oid << " ";
        cout << "| Total Distance: " << b.totalDistance << "min | Partner: "
             << (b.partnerId == -1 ? "unassigned" : to_string(b.partnerId)) << " | Route: ";
        for(size_t i = 0; i < b.route.size(); i++){
            cout << b.route[i];
            if(i + 1 < b.route.size()) cout << " -> ";
        }
        cout << endl;
    }
}

void OrderManager :: displayPartners() const{
    if(partners.empty()){
        cout << "--------No Delivery Partners Registered--------" << endl;
        return;
    }
    cout << "\n------ Delivery Partners ------\n";
    for(const auto& p : partners) p.display();
}

vector<DeliveryPartner> OrderManager :: getAllPartners() const{
    return partners;
}

int OrderManager :: getClock() const{
    return clock;
}
