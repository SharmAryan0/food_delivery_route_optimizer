#include "RestaurantManager.h"

using namespace std;

void RestaurantManager :: addRestaurant(const Restaurant& r){
    restaurants.push_back(r);
}

void RestaurantManager :: displayRestaurant() const { 

    if(restaurants.empty()){
        cout << " No Restaurant Available " << endl;
    }

    else{
        cout << " -------------RESTAURANTS------------- " << endl;
        for(const auto & r : restaurants){
            r.display();
            cout << " ------------------------------------- " << endl;
        }
    }
}

void RestaurantManager :: searchRestaurant(int id) const {

    for(size_t i=0;i<restaurants.size();i++){
        if(restaurants[i].getId() == id){
            restaurants[i].display();
            return;
        }
    }
    cout << "------Restaurant Not Found------" << endl;
}

void RestaurantManager :: removeRestaurant(int id){

    for(size_t i=0;i<restaurants.size();i++){
        if(restaurants[i].getId() == id){
            restaurants.erase(restaurants.begin() + i);
            cout << "------Restaurant Removed Successfully------" << endl;
            return;
        }
    }
    cout << "------Restaurant Not Found------" << endl;
}

Restaurant RestaurantManager :: getRestaurant(int id){

    for(auto it : restaurants){
        if(it.getId() == id) {
            return it;
        }
    }
    cout << "Restaurant Not Found!" << endl;
    return Restaurant();
}

vector<Restaurant> RestaurantManager :: getAllRestaurants() const{
    return restaurants;
}

bool RestaurantManager :: rateRestaurant(int id, double stars){
    for(auto& r : restaurants){
        if(r.getId() == id){
            r.addRating(stars);
            return true;
        }
    }
    return false;
}

void RestaurantManager :: restoreAllRestaurants(const vector<Restaurant>& snapshot){
    restaurants = snapshot;
}
