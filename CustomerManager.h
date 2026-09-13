#ifndef CustomerManager_H
#define CustomerManager_H

#include <iostream>
#include <vector>
#include "Customer.h"

using namespace std;

class CustomerManager{
private:
    vector<Customer> customers;
public:
    void addCustomer(const Customer& c);
    void displayCustomers() const;
    void searchCustomer(int id) const;
    void removeCustomer(int id);
    vector<Customer> getAllCustomers() const;
    Customer getCustomer(int id);
};

#endif
