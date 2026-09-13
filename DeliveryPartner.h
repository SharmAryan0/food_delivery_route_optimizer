#ifndef DeliveryPartner_H
#define DeliveryPartner_H

#include <iostream>
#include <string>

using namespace std;

class DeliveryPartner{
private:
    int id;
    string name;
    string currentLocation;
    int availableAt; // simulated clock time (minutes) at which the partner becomes free

public:
    DeliveryPartner();
    DeliveryPartner(int id, string name, string currentLocation);

    int getId() const;
    string getName() const;
    string getCurrentLocation() const;
    int getAvailableAt() const;

    void setCurrentLocation(const string& location);
    void setAvailableAt(int time);

    void display() const;
};

#endif
