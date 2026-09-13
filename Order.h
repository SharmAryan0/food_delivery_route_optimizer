#ifndef Order_H
#define Order_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum OrderStatus { PLACED, BATCHED, ASSIGNED, DELIVERED, CANCELLED };

class Order{
private:
    int orderId;
    int customerId;
    int restaurantId;
    vector<int> foodIds;
    int placedTime;      // simulated minute the order was placed
    int batchId;         // -1 until grouped into a batch
    int partnerId;        // -1 until a delivery partner is assigned
    int eta;              // estimated minute of delivery, -1 if unknown
    int rating;            // 1-5 stars once the customer rates it, -1 if unrated
    OrderStatus status;

public:
    Order();
    Order(int orderId, int customerId, int restaurantId, vector<int> foodIds, int placedTime);

    int getOrderId() const;
    int getCustomerId() const;
    int getRestaurantId() const;
    vector<int> getFoodIds() const;
    int getPlacedTime() const;
    int getBatchId() const;
    int getPartnerId() const;
    int getEta() const;
    int getRating() const;
    OrderStatus getStatus() const;

    void setBatchId(int id);
    void setPartnerId(int id);
    void setEta(int t);
    void setRating(int stars);
    void setStatus(OrderStatus s);

    void display() const;
    string statusToString() const;
};

#endif
