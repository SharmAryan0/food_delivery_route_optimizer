#include "DeliveryPartner.h"

DeliveryPartner :: DeliveryPartner(){
    id = 0;
    name = "";
    currentLocation = "";
    availableAt = 0;
}

DeliveryPartner :: DeliveryPartner(int id, string name, string currentLocation){
    this->id = id;
    this->name = name;
    this->currentLocation = currentLocation;
    this->availableAt = 0;
}

int DeliveryPartner :: getId() const{
    return id;
}

string DeliveryPartner :: getName() const{
    return name;
}

string DeliveryPartner :: getCurrentLocation() const{
    return currentLocation;
}

int DeliveryPartner :: getAvailableAt() const{
    return availableAt;
}

void DeliveryPartner :: setCurrentLocation(const string& location){
    currentLocation = location;
}

void DeliveryPartner :: setAvailableAt(int time){
    availableAt = time;
}

void DeliveryPartner :: display() const{
    cout << "Partner ID : " << id << " | Name : " << name
         << " | Location : " << currentLocation
         << " | Free at t=" << availableAt << "min" << endl;
}
