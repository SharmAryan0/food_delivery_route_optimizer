#include "MenuItem.h"

MenuItem :: MenuItem(){
    foodId = 0;
    restaurantId = 0;
    foodName = "";
    price = 0.0;
}

MenuItem :: MenuItem(int foodId,int restaurantId,string foodName,double price){
    this->foodId = foodId;
    this->restaurantId = restaurantId;
    this->foodName = foodName;
    this->price = price;
}

int MenuItem :: getFoodId () {
    return foodId;
}

int MenuItem :: getRestaurantId() {
    return restaurantId;
}

string MenuItem :: getFoodName() {
    return foodName;
}

double MenuItem :: getPrice() {
    return price;
}

void MenuItem :: display () {
    cout << foodId << "   "
         << foodName
         << "   Rs." << price << endl;
}