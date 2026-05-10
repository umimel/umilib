#ifndef BFS_HPP
#define BFS_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename T>
vector<T> bfs(graph<T> &G, int s){
    int n = G.size();
    vector<T> dist(n, -1);

    dist[s] = 0;
    queue<int> que;
    que.push(s);
    while(!que.empty()){
        int v = que.front();
        que.pop();
        for(auto e : G[v]) if(dist[e.to]==-1){
            dist[e.to] = dist[v] + 1;
            que.push(e.to);
        }
    }

    return dist;
}
#endif
