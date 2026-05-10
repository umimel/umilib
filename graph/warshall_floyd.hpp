#ifndef WARSHALL_FLOYD_HPP
#define WARSHALL_FLOYD_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename T>
vector<vector<T>> warshall_floyd(graph<T> &G){
    int n = G.size();
    const T TINF = numeric_limits<T>::max()/3;
    vector<vector<T>> dist(n, vector<T>(n, TINF));
    
    for(int v=0; v<n; v++) dist[v][v] = 0;
    for(int v=0; v<n; v++) for(auto e : G[v]) dist[v][e.to] = min(dist[v][e.to], e.cost);
    for(int k=0; k<n; k++) for(int i=0; i<n; i++) for(int j=0; j<n; j++) if(dist[i][k] < TINF && dist[k][j] < TINF) dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

    return dist;
}
#endif
