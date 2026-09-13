#include "Graph.h"
#include <queue>
#include <algorithm>
#include <climits>

void Graph :: addLocation(const string& name){
    if(adj.find(name) == adj.end()){
        adj[name] = vector<pair<string,int>>();
    }
}

void Graph :: addRoad(const string& a, const string& b, int weight){
    addLocation(a);
    addLocation(b);
    adj[a].push_back({b, weight});
    adj[b].push_back({a, weight}); // roads are bidirectional
}

bool Graph :: hasLocation(const string& name) const{
    return adj.find(name) != adj.end();
}

map<string,int> Graph :: dijkstra(const string& src) const{

    map<string,int> dist;
    for(const auto& node : adj){
        dist[node.first] = INT_MAX;
    }

    if(adj.find(src) == adj.end()) return dist;

    dist[src] = 0;

    // min-heap of (distance, node)
    priority_queue<pair<int,string>, vector<pair<int,string>>, greater<pair<int,string>>> pq;
    pq.push({0, src});

    while(!pq.empty()){
        auto top = pq.top();
        pq.pop();
        int d = top.first;
        string u = top.second;

        if(d > dist[u]) continue;

        for(const auto& edge : adj.at(u)){
            string v = edge.first;
            int w = edge.second;
            if(dist[u] != INT_MAX && dist[u] + w < dist[v]){
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}

pair<vector<string>,int> Graph :: shortestPath(const string& src, const string& dest) const{

    map<string,int> dist;
    map<string,string> prev;

    for(const auto& node : adj){
        dist[node.first] = INT_MAX;
    }

    if(adj.find(src) == adj.end() || adj.find(dest) == adj.end()){
        return {vector<string>(), -1};
    }

    dist[src] = 0;
    priority_queue<pair<int,string>, vector<pair<int,string>>, greater<pair<int,string>>> pq;
    pq.push({0, src});

    while(!pq.empty()){
        auto top = pq.top();
        pq.pop();
        int d = top.first;
        string u = top.second;

        if(d > dist[u]) continue;
        if(u == dest) break;

        for(const auto& edge : adj.at(u)){
            string v = edge.first;
            int w = edge.second;
            if(dist[u] != INT_MAX && dist[u] + w < dist[v]){
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    if(dist[dest] == INT_MAX){
        return {vector<string>(), -1};
    }

    vector<string> path;
    string cur = dest;
    path.push_back(cur);
    while(cur != src){
        cur = prev[cur];
        path.push_back(cur);
    }
    reverse(path.begin(), path.end());

    return {path, dist[dest]};
}

int Graph :: distance(const string& src, const string& dest) const{
    if(src == dest) return 0;
    auto result = shortestPath(src, dest);
    return result.second;
}

void Graph :: displayLocations() const{
    cout << "\n------ Known Locations ------\n";
    for(const auto& node : adj){
        cout << " - " << node.first << endl;
    }
}
