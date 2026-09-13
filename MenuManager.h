#ifndef MenuManager_H
#define MenuManager_H

#include <iostream>
#include <vector>
#include "MenuItem.h"

using namespace std;

class MenuManager{
private:
    vector<MenuItem> menu;
public:
    void addFood(MenuItem item);
    void displayRestaurantMenu(int restaurantId);
    MenuItem getFood(int foodId);
    vector<MenuItem> getMenuForRestaurant(int restaurantId);
};

#endif
