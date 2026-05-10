#ifndef BELLMAN_FORD_HPP
#define BELLMAN_FORD_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename T>
vector<T> bellman_ford(graph<T> &G, int s){
    int n = G.size();
    bool dir = G.get_dir();
    assert(dir);
    const T TINF = numeric_limits<T>::max()/3;
    edges<T> es = G.get_edge_set();
    vector<T> dist(n, TINF);
    vector<bool> flag(n, false);

    dist[s] = 0;
    for(int i=0; i<n; i++) for(auto e : es){
        if(dist[e.from]!=TINF && dist[e.from]+e.cost<dist[e.to]) dist[e.to] = dist[e.from] + e.cost;
    }

    for(int i=0; i<n; i++) for(auto e : es){
        if(dist[e.from]!=TINF && dist[e.from]+e.cost<dist[e.to]) dist[e.to] = dist[e.from] + e.cost, flag[e.to]=true;
    }

    for(int i=0; i<n; i++) for(auto e : es){
        flag[e.to] = flag[e.to] | flag[e.from];
    }
    for(int v=0; v<n; v++) if(flag[v]) dist[v] = -TINF;

    return dist;
}
#endif
