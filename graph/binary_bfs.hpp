#ifndef BINARY_BFS_HPP
#define BINARY_BFS_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename T>
vector<T> binary_bfs(graph<T> &G, int s){
    int n = G.size();
    vector<T> dist(n, -1);

    dist[s] = 0;
    deque<int> deq;
    deq.push_front(s);
    while(!deq.empty()){
        int v = deq.front();
        deq.pop_front();
        for(auto e : G[v]) if(dist[e.to]==-1){
            dist[e.to] = dist[v] + e.cost;
            if(e.cost) deq.push_back(e.to);
            else deq.push_front(e.to);
        }
    }

    return dist;
}
#endif
