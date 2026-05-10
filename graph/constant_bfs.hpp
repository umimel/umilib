#ifndef CONSTANT_BFS_HPP
#define CONSTANT_BFS_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename T>
vector<T> constant_bfs(graph<T> &G, int s, T W){
    int n = G.size();
    vector<T> dist(n, -1);
    vector<vector<int>> cand(n*W+1);

    dist[s] = 0;
    cand[0].push_back(s);
    for(int d=0; d<=n*W; d++) for(int v : cand[d]){
        if(dist[v] < d) continue;
        for(auto e : G[v]) if(dist[e.to] == -1 || dist[v] + e.cost < dist[e.to]){
            dist[e.to] = dist[v] + e.cost;
            cand[dist[e.to]].push_back(e.to);
        }
    }

    return dist;
}
#endif
