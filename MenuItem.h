#ifndef MenuItem_H
#define MenuItem_H

#include<iostream>
#include<string>

using namespace std;

class MenuItem{
private:
    int foodId;
    int restaurantId;
    string foodName;
    double price;

public:
    
    MenuItem();
    MenuItem(int foodId,int restaurantId,string foodName,double price);

    int getFoodId();

    int getRestaurantId();

    string getFoodName();

    double getPrice();

    void display();

};



#endif