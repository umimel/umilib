#ifndef SHORTEST_PATH_HPP
#define SHORTEST_PATH_HPP

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

template<typename T>
vector<T> bellman_ford(graph<T> &G, int s){
    int n = G.size();
    const T TINF = numeric_limits<T>::max()/3;
    edges<T> es = G.get_edge_set();
    vector<T> dist(n, TINF);
    vector<bool> flag(n, false);

    dist[s] = 0;
    for(int i=0; i<n; i++) for(auto e : es){
        if(dist[e.from] != TINF && dist[e.from] + e.cost < dist[e.to]){
            dist[e.to] = dist[e.from] + e.cost;
        }
    }

    for(int i=0; i<n; i++) for(auto e : es){
        if(dist[e.from] != TINF && dist[e.from] + e.cost < dist[e.to]){
            dist[e.to] = dist[e.from] + e.cost;
            flag[e.to] = true;
        }
    }

    for(int i=0; i<n; i++) for(auto e : es){
        flag[e.to] = flag[e.to] | flag[e.from];
    }

    for(int v=0; v<n; v++) if(flag[v]) dist[v] = -TINF;
    return dist;
}

template<typename T>
vector<vector<T>> warshall_floyd(graph<T> &G){
    int n = G.size();
    const T TINF = numeric_limits<T>::max()/3;
    vector<vector<T>> dist(n, vector<T>(n, TINF));

    for(int v=0; v<n; v++) dist[v][v] = 0;
    for(int v=0; v<n; v++){
        for(auto e : G[v]){
            dist[v][e.to] = min(dist[v][e.to], e.cost);
        }
    }

    for(int k=0; k<n; k++){
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                if(dist[i][k] < TINF && dist[k][j] < TINF){
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    return dist;
}

#endif
