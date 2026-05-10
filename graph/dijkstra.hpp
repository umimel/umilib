#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename T>
vector<T> dijkstra(graph<T> &G, int s){
    int n = G.size();
    const T TINF = numeric_limits<T>::max()/3;
    vector<T> dist(n, TINF);

    dist[s] = 0;
    priority_queue<pair<T, int>, vector<pair<T, int>>, greater<>> que;
    que.push({0, s});
    while(!que.empty()){
        auto [d, v] = que.top();
        que.pop();
        if(dist[v] < d) continue;

        for(auto e : G[v]){
            if(dist[v] + e.cost < dist[e.to]){
                dist[e.to] = dist[v] + e.cost;
                que.push({dist[e.to], e.to});
            }
        }
    }

    return dist;
}
#endif
