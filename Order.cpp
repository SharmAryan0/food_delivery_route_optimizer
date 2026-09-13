#include "Order.h"

Order :: Order(){
    orderId = 0;
    customerId = 0;
    restaurantId = 0;
    placedTime = 0;
    batchId = -1;
    partnerId = -1;
    eta = -1;
    rating = -1;
    status = PLACED;
}

Order :: Order(int orderId, int customerId, int restaurantId, vector<int> foodIds, int placedTime){
    this->orderId = orderId;
    this->customerId = customerId;
    this->restaurantId = restaurantId;
    this->foodIds = foodIds;
    this->placedTime = placedTime;
    this->batchId = -1;
    this->partnerId = -1;
    this->eta = -1;
    this->rating = -1;
    this->status = PLACED;
}

int Order :: getOrderId() const{
    return orderId;
}

int Order :: getCustomerId() const{
    return customerId;
}

int Order :: getRestaurantId() const{
    return restaurantId;
}

vector<int> Order :: getFoodIds() const{
    return foodIds;
}

int Order :: getPlacedTime() const{
    return placedTime;
}

int Order :: getBatchId() const{
    return batchId;
}

int Order :: getPartnerId() const{
    return partnerId;
}

int Order :: getEta() const{
    return eta;
}

int Order :: getRating() const{
    return rating;
}

OrderStatus Order :: getStatus() const{
    return status;
}

void Order :: setBatchId(int id){
    batchId = id;
    if(status == PLACED) status = BATCHED;
}

void Order :: setPartnerId(int id){
    partnerId = id;
    if(status == BATCHED) status = ASSIGNED;
}

void Order :: setEta(int t){
    eta = t;
}

void Order :: setRating(int stars){
    rating = stars;
}

void Order :: setStatus(OrderStatus s){
    status = s;
}

string Order :: statusToString() const{
    switch(status){
        case PLACED:    return "PLACED";
        case BATCHED:   return "BATCHED";
        case ASSIGNED:  return "ASSIGNED";
        case DELIVERED: return "DELIVERED";
        case CANCELLED: return "CANCELLED";
    }
    return "UNKNOWN";
}

void Order :: display() const{
    cout << "Order #" << orderId
         << " | Customer ID: " << customerId
         << " | Restaurant ID: " << restaurantId
         << " | Food Items: ";
    for(size_t i = 0; i < foodIds.size(); i++){
        cout << foodIds[i];
        if(i + 1 < foodIds.size()) cout << ", ";
    }
    cout << " | Placed at t=" << placedTime << "min"
         << " | Batch: " << (batchId == -1 ? "unbatched" : to_string(batchId))
         << " | Partner: " << (partnerId == -1 ? "unassigned" : to_string(partnerId))
         << " | ETA: " << (eta == -1 ? "N/A" : to_string(eta) + "min")
         << " | Status: " << statusToString()
         << " | Rating: " << (rating == -1 ? "not rated" : to_string(rating) + "/5")
         << endl;
}
