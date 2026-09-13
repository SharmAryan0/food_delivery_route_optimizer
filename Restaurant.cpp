#include "Restaurant.h"

Restaurant::Restaurant(){
    id = 0;
    name = "";
    location = "";
    rating = 0.0;
    ratingCount = 0;
}

Restaurant :: Restaurant(int id,string name,string location,double rating){

    this->id = id;
    this->name = name;
    this->location = location;
    this->rating = rating;
    this->ratingCount = 1;

}

void Restaurant :: display() const{

    cout << "ID : " << id << endl;
    cout << "Name : " << name << endl;
    cout << "Location : " << location << endl;
    cout << "Rating : " << rating << " (" << ratingCount << " rating" << (ratingCount == 1 ? "" : "s") << ")" << endl;

}

int Restaurant :: getId() const{
    return id;
}

string Restaurant :: getName() const{
    return name;
}

string Restaurant :: getLocation() const{
    return location;
}

double Restaurant:: getRating() const{ 
    return rating;
}

int Restaurant :: getRatingCount() const{
    return ratingCount;
}

void Restaurant :: addRating(double stars){
    rating = (rating * ratingCount + stars) / (ratingCount + 1);
    ratingCount++;
}