#include <iostream>
#include <vector>
#include <sstream>
#include <limits>
#include "Restaurant.h"
#include "RestaurantManager.h"
#include "MenuItem.h"
#include "MenuManager.h"
#include "Customer.h"
#include "CustomerManager.h"
#include "Graph.h"
#include "DeliveryPartner.h"
#include "Order.h"
#include "OrderManager.h"

using namespace std;

void seedCityGraph(Graph& g){
    g.addLocation("Sindhi Camp");

    // central / old city cluster
    g.addRoad("Sindhi Camp", "C-Scheme", 5);
    g.addRoad("Sindhi Camp", "Civil Lines", 6);
    g.addRoad("Sindhi Camp", "Bani Park", 7);
    g.addRoad("Sindhi Camp", "Johari Bazaar", 8);
    g.addRoad("C-Scheme", "Civil Lines", 4);
    g.addRoad("C-Scheme", "Raja Park", 9);
    g.addRoad("Raja Park", "Johari Bazaar", 6);
    g.addRoad("Civil Lines", "Bani Park", 5);

    // north
    g.addRoad("Civil Lines", "Vidhyadhar Nagar", 14);

    // west cluster
    g.addRoad("Sindhi Camp", "Sodala", 6);
    g.addRoad("Sodala", "C-Scheme", 8);
    g.addRoad("Sodala", "Vaishali Nagar", 6);
    g.addRoad("Vaishali Nagar", "Ajmer Road", 4);
    g.addRoad("Vaishali Nagar", "Mansarovar", 9);
    g.addRoad("Mansarovar", "Ajmer Road", 7);

    // south cluster
    g.addRoad("C-Scheme", "Malviya Nagar", 14);
    g.addRoad("Malviya Nagar", "Mansarovar", 10);
    g.addRoad("Malviya Nagar", "Tonk Road", 8);
    g.addRoad("Tonk Road", "Jagatpura", 9);
    g.addRoad("Tonk Road", "Sanganer", 12);
    g.addRoad("Jagatpura", "Sanganer", 8);
}

void seedDemoData(RestaurantManager& rm, MenuManager& mm, CustomerManager& cm){
    // real, well-known Jaipur restaurants, placed at their actual localities
    rm.addRestaurant(Restaurant(1, "LMB (Laxmi Mishthan Bhandar)", "Johari Bazaar", 4.4));
    rm.addRestaurant(Restaurant(2, "Rawat Mishthan Bhandar", "Sindhi Camp", 4.3));
    rm.addRestaurant(Restaurant(3, "Copper Chimney", "C-Scheme", 4.3));
    rm.addRestaurant(Restaurant(4, "Kanha Restaurant", "Malviya Nagar", 4.5));
    rm.addRestaurant(Restaurant(5, "Handi Restaurant", "Tonk Road", 4.4));
    rm.addRestaurant(Restaurant(6, "Tapri Central", "C-Scheme", 4.1));
    rm.addRestaurant(Restaurant(7, "Peacock Rooftop Restaurant", "Ajmer Road", 4.2));
    rm.addRestaurant(Restaurant(8, "Chokhi Dhani", "Tonk Road", 4.5));
    rm.addRestaurant(Restaurant(9, "Niro's", "C-Scheme", 4.2));
    rm.addRestaurant(Restaurant(10, "Natraj Dining Hall", "C-Scheme", 4.3));

    mm.addFood(MenuItem(101, 1, "Dal Baati Churma", 220));
    mm.addFood(MenuItem(102, 1, "Pyaaz Kachori", 40));
    mm.addFood(MenuItem(201, 2, "Mawa Kachori", 60));
    mm.addFood(MenuItem(202, 2, "Pyaaz Kachori", 35));
    mm.addFood(MenuItem(301, 3, "Laal Maas", 380));
    mm.addFood(MenuItem(302, 3, "Butter Chicken", 340));
    mm.addFood(MenuItem(401, 4, "Dal Baati Churma", 230));
    mm.addFood(MenuItem(402, 4, "Ker Sangri", 210));
    mm.addFood(MenuItem(501, 5, "Laal Maas", 400));
    mm.addFood(MenuItem(502, 5, "Safed Maas", 390));
    mm.addFood(MenuItem(601, 6, "Kulhad Chai", 40));
    mm.addFood(MenuItem(602, 6, "Masala Maggi", 90));
    mm.addFood(MenuItem(701, 7, "Continental Platter", 450));
    mm.addFood(MenuItem(702, 7, "Paneer Tikka", 260));
    mm.addFood(MenuItem(801, 8, "Rajasthani Thali", 550));
    mm.addFood(MenuItem(802, 8, "Bajre Ki Roti", 60));
    mm.addFood(MenuItem(901, 9, "Rogan Josh", 350));
    mm.addFood(MenuItem(902, 9, "Butter Naan", 40));
    mm.addFood(MenuItem(1001, 10, "Rajasthani Thali", 300));
    mm.addFood(MenuItem(1002, 10, "Gatte Ki Sabzi", 180));

    cm.addCustomer(Customer(1001, "Aarav", "Raja Park"));
    cm.addCustomer(Customer(1002, "Priya", "Mansarovar"));
    cm.addCustomer(Customer(1003, "Rohan", "Jagatpura"));
    cm.addCustomer(Customer(1004, "Sneha", "Vaishali Nagar"));
    cm.addCustomer(Customer(1005, "Kabir", "Civil Lines"));
    cm.addCustomer(Customer(1006, "Ananya", "Sanganer"));
    cm.addCustomer(Customer(1007, "Diya", "Ajmer Road"));
    cm.addCustomer(Customer(1008, "Arjun", "Bani Park"));
    cm.addCustomer(Customer(1009, "Ishita", "Vidhyadhar Nagar"));
    cm.addCustomer(Customer(1010, "Karan", "Sodala"));
    cm.addCustomer(Customer(1011, "Meera", "Johari Bazaar"));
    cm.addCustomer(Customer(1012, "Yash", "C-Scheme"));
    cm.addCustomer(Customer(1013, "Riya", "Tonk Road"));
    cm.addCustomer(Customer(1014, "Dev", "Sindhi Camp"));
    cm.addCustomer(Customer(1015, "Naina", "Malviya Nagar"));
}

vector<DeliveryPartner> seedPartners(){
    vector<DeliveryPartner> partners;
    partners.push_back(DeliveryPartner(1, "Vikram", "Sindhi Camp"));
    partners.push_back(DeliveryPartner(2, "Meena", "Sindhi Camp"));
    partners.push_back(DeliveryPartner(3, "Rajesh", "Malviya Nagar"));
    partners.push_back(DeliveryPartner(4, "Suresh", "Vaishali Nagar"));
    partners.push_back(DeliveryPartner(5, "Pooja", "C-Scheme"));
    return partners;
}

int readInt(const string& prompt){
    int val;
    cout << prompt;
    while(!(cin >> val)){
        cout << "Please enter a valid number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return val;
}

string readLine(const string& prompt){
    string val;
    cout << prompt;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, val);
    return val;
}

void printMenu(){
    cout << "\n================ FOOD DELIVERY OPTIMIZER ================\n";
    cout << " 1. View Restaurants\n";
    cout << " 2. View Menu For A Restaurant\n";
    cout << " 3. View Customers\n";
    cout << " 4. View City Map (valid location names)\n";
    cout << " 5. Place An Order\n";
    cout << " 6. Run Smart Batching  (the optimizer's core feature)\n";
    cout << " 7. Assign Delivery Partners To Batches\n";
    cout << " 8. View All Batches & Routes\n";
    cout << " 9. Track An Order\n";
    cout << "10. View All Orders\n";
    cout << "11. View Delivery Partners\n";
    cout << "12. Cancel An Order\n";
    cout << "13. Rate A Delivered Order\n";
    cout << "14. Advance Simulation Clock (lets partners finish deliveries)\n";
    cout << "15. Undo Last Action\n";
    cout << " 0. Exit\n";
    cout << "===========================================================\n";
}

int main(){

    Graph cityGraph;
    seedCityGraph(cityGraph);

    RestaurantManager restaurantManager;
    MenuManager menuManager;
    CustomerManager customerManager;
    seedDemoData(restaurantManager, menuManager, customerManager);

    OrderManager orderManager(cityGraph, restaurantManager, customerManager);
    orderManager.registerPartners(seedPartners());

    int choice;
    do{
        printMenu();
        choice = readInt("Enter your choice: ");

        switch(choice){

            case 1:
                restaurantManager.displayRestaurant();
                break;

            case 2: {
                int rid = readInt("Enter Restaurant ID: ");
                menuManager.displayRestaurantMenu(rid);
                break;
            }

            case 3:
                customerManager.displayCustomers();
                break;

            case 4:
                cityGraph.displayLocations();
                break;

            case 5: {
                int cid = readInt("Enter Customer ID: ");
                int rid = readInt("Enter Restaurant ID: ");
                int foodId = readInt("Enter Food Item ID to order: ");
                vector<int> foodIds = { foodId };
                orderManager.placeOrder(cid, rid, foodIds);
                break;
            }

            case 6:
                orderManager.runSmartBatching();
                break;

            case 7:
                orderManager.assignPartners();
                break;

            case 8:
                orderManager.displayBatches();
                break;

            case 9: {
                int oid = readInt("Enter Order ID: ");
                orderManager.trackOrder(oid);
                break;
            }

            case 10:
                orderManager.displayAllOrders();
                break;

            case 11:
                orderManager.displayPartners();
                break;

            case 12: {
                int oid = readInt("Enter Order ID To Cancel: ");
                orderManager.cancelOrder(oid);
                break;
            }

            case 13: {
                int oid = readInt("Enter Order ID To Rate: ");
                int stars = readInt("Enter Rating (1-5): ");
                orderManager.rateOrder(oid, stars);
                break;
            }

            case 14: {
                int minutes = readInt("Advance Clock By How Many Minutes: ");
                orderManager.advanceClock(minutes);
                break;
            }

            case 15:
                orderManager.undo();
                break;

            case 0:
                cout << "Thank you for using Food Delivery Optimizer!\n";
                break;

            default:
                cout << "Invalid choice, please try again.\n";
        }

    } while(choice != 0);

    return 0;
}
