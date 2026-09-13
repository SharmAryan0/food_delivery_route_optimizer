#include "MenuManager.h"

void MenuManager :: addFood(MenuItem item){
    menu.push_back(item);
}

void MenuManager :: displayRestaurantMenu(int restaurantId){

    cout << "\n========== MENU ==========\n";
    for(auto it : menu){
        if(it.getRestaurantId() == restaurantId){
            it.display();
        }
    }
}

MenuItem MenuManager :: getFood(int foodId){

    for(auto it : menu){
        if(it.getFoodId() == foodId){
            return it;
        }
    }
    cout << "Food Item Not Found\n";
    return MenuItem();
    
}

vector<MenuItem> MenuManager :: getMenuForRestaurant(int restaurantId){
    vector<MenuItem> result;
    for(auto it : menu){
        if(it.getRestaurantId() == restaurantId){
            result.push_back(it);
        }
    }
    return result;
}