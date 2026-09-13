#ifndef RestaurantManager_H
#define RestaurantManager_H

#include <iostream>
#include <vector>
#include "Restaurant.h"

using namespace std;

class RestaurantManager{
private:
    vector<Restaurant> restaurants;
public:
    void addRestaurant(const Restaurant& r);
    void displayRestaurant() const;
    void searchRestaurant(int id) const;
    void removeRestaurant(int id);
    Restaurant getRestaurant(int id);
    vector<Restaurant> getAllRestaurants() const;

    // Folds a customer's 1-5 star rating into the given restaurant's running
    // average. Returns false if the restaurant doesn't exist.
    bool rateRestaurant(int id, double stars);

    // Overwrites the whole restaurant list in one shot - used to restore a
    // prior snapshot when the user undoes an action that changed a rating.
    void restoreAllRestaurants(const vector<Restaurant>& snapshot);
};

#endif
