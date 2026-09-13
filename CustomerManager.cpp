#include "CustomerManager.h"

void CustomerManager :: addCustomer (const Customer& c) {

    customers.push_back(c);
    cout << "--------Customer Added Successfully--------" << endl;

}

void CustomerManager :: displayCustomers() const {

    if(customers.empty()){
        cout << "--------No Customer Available--------" << endl;
    }

    else{
        cout << "\n------ Customers ------\n";
        for(const auto& customer : customers){
            customer.display();
            cout << "-----------------------\n";
        }
    }
}

void CustomerManager :: searchCustomer(int id) const{
    for(size_t i=0;i<customers.size();i++){
        if(customers[i].getId() == id){
            customers[i].display();
            return;
        }
    }
    cout << "------Customer Not Found------" << endl;
}

void CustomerManager :: removeCustomer(int id) {
    for(size_t i=0;i<customers.size();i++){
        if(customers[i].getId() == id){
            customers.erase(customers.begin() + i);
            cout << "------Customer Removed Successfully------" << endl;
            return;
        }
    }
    cout << "------Customer Not Found------" << endl;
}

vector<Customer> CustomerManager::getAllCustomers() const{
    return customers;
}

Customer CustomerManager :: getCustomer(int id){
    for(auto it : customers){
        if(it.getId() == id){
            return it;
        }
    }
    cout << "Customer Not Found!" << endl;
    return Customer();
}
