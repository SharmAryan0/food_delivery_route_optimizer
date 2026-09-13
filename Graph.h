#ifndef Graph_H
#define Graph_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <limits>

using namespace std;

// Graph represents the city map. Every restaurant / customer / delivery hub
// location is a node. Edge weight = travel time in minutes.
class Graph{
private:
    map<string, vector<pair<string,int>>> adj;

public:
    void addLocation(const string& name);
    void addRoad(const string& a, const string& b, int weight);
    bool hasLocation(const string& name) const;

    // Dijkstra from src -> distance to every reachable node
    map<string,int> dijkstra(const string& src) const;

    // Dijkstra from src -> dest, also reconstructs the path taken
    // returns {path (list of location names), total distance}. Distance is -1 if unreachable.
    pair<vector<string>,int> shortestPath(const string& src, const string& dest) const;

    // convenience: just the distance between two nodes
    int distance(const string& src, const string& dest) const;

    void displayLocations() const;
};

#endif
