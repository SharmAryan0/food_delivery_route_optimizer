#ifndef OrderManager_H
#define OrderManager_H

#include <iostream>
#include <vector>
#include <string>
#include "Order.h"
#include "Graph.h"
#include "DeliveryPartner.h"
#include "RestaurantManager.h"
#include "CustomerManager.h"
#include "Restaurant.h"

using namespace std;

struct Batch{
    int batchId;
    vector<int> orderIds;
    vector<string> route;      // ordered sequence of stops (pickup/drop locations)
    vector<int> dropOrderId;   // parallel to route: orderId dropped at this stop, -1 if pickup/hub stop
    int totalDistance;         // minutes for the whole route
    int partnerId;             // -1 until assigned
    int startTime;             // simulated minute the batch route begins

    Batch(){ batchId = 0; totalDistance = 0; partnerId = -1; startTime = 0; }
};

struct StateSnapshot{
    vector<Order> orders;
    vector<Batch> batches;
    vector<DeliveryPartner> partners;
    vector<Restaurant> restaurants; 
    int nextOrderId;
    int nextBatchId;
    int clock;
    string lastAction; 
};

class OrderManager{
private:
    vector<Order> orders;
    vector<Batch> batches;
    vector<DeliveryPartner> partners;
    vector<StateSnapshot> undoStack;

    int nextOrderId;
    int nextBatchId;
    int clock; 

    Graph& graph;
    RestaurantManager& restaurantManager;
    CustomerManager& customerManager;

    const string HUB = "Sindhi Camp";
    const int DETOUR_THRESHOLD = 15; 
    const int MAX_BATCH_SIZE = 3;    
    const size_t MAX_UNDO_DEPTH = 30;

    Order* findOrder(int orderId);
    int findBatchIndexForOrder(int orderId) const;
    void recomputeBatchDistance(Batch& b) const;

    void pushUndoSnapshot(const string& actionDescription);

public:
    OrderManager(Graph& g, RestaurantManager& rm, CustomerManager& cm);

    // Seeds the roster of delivery partners this manager will dispatch.
    void registerPartners(const vector<DeliveryPartner>& initialPartners);

    int placeOrder(int customerId, int restaurantId, vector<int> foodIds);

    // THE UNIQUE FEATURE: groups pending orders into efficient multi-stop
    // routes using a savings-based greedy heuristic (inspired by the
    // Clarke-Wright savings algorithm used in real vehicle routing).
    void runSmartBatching();

    // Assigns the cheapest available delivery partner (min-heap by free time)
    // to each unassigned batch and computes a real ETA per order via Dijkstra.
    void assignPartners();

    // Cancels an order that hasn't been dispatched to a partner yet, cleanly
    // unwinding it from any batch it was merged into and re-costing the
    // remaining route. Orders already ASSIGNED or DELIVERED can't be cancelled.
    bool cancelOrder(int orderId);

    // Lets a customer leave a 1-5 star rating on a DELIVERED order; the
    // rating folds into that restaurant's running-average rating.
    bool rateOrder(int orderId, int stars);

    // Moves the simulation clock forward by `minutes`; any ASSIGNED order
    // whose ETA has now passed automatically flips to DELIVERED.
    void advanceClock(int minutes);

    // Reverts the most recent state-changing action (Memento/undo stack).
    bool undo();

    void trackOrder(int orderId) const;
    void displayAllOrders() const;
    void displayBatches() const;
    void displayPartners() const;

    vector<DeliveryPartner> getAllPartners() const;
    int getClock() const;
};

#endif
