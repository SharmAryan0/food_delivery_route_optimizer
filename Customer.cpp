#include "Customer.h"

Customer :: Customer(){

    id = 0;
    name = "";
    location = "";

}

Customer :: Customer(int id,string name,string location){

    this->id = id;
    this->name = name;
    this->location = location;

}

int Customer :: getId() const{
    return id;
}

string Customer :: getName() const{
    return name;
}

string Customer :: getLocation() const{
    return location;
}

void Customer :: display() const{

    cout << "Customer ID : " << id << endl;
    cout << "Customer Name : " << name << endl;
    cout << "Location : " << location << endl;
    
}