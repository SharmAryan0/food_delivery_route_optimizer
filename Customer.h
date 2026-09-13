#ifndef Customer_H
#define Customer_H

#include <iostream>
#include <string>

using namespace std;

class Customer{

private:
    int id;
    string name;
    string location;
public:
    Customer();

    Customer(int id,string name,string location);

    int getId() const;

    string getName() const;

    string getLocation() const;

    void display() const;

};

# endif