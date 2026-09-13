#ifndef Restaurant_H
#define Restaurant_H

#include <iostream>
#include <string>

using namespace std;

class Restaurant{
private:
    int id;
    string name;
    string location;
    double rating;
    int ratingCount;   // how many rating samples have gone into 'rating' so far

public:
    Restaurant();     // default constructor

    Restaurant(int id,string name,string location,double rating); //parameterized constructor

    void display() const;

    int getId() const;

    string getName() const;

    string getLocation() const;

    double getRating() const;

    int getRatingCount() const;

    // Folds a new customer rating (1-5) into the running average.
    void addRating(double stars);
};

# endif

