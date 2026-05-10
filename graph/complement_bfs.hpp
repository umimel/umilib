#ifndef COMPLEMENT_BFS_HPP
#define COMPLEMENT_BFS_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename T>
vector<T> complement_bfs(graph<T> &G, int s){
    int n = G.size();
    map<pair<int, int>, bool> mp;
    for(int v=0; v<n; v++) for(auto e : G[v]) mp[{v, e.to}] = true;

    vector<T> dist(n, -1);
    vector<int> unvisited;
    for(int v=0; v<n; v++) if(v != s) unvisited.push_back(v);
    queue<int> visited;
    visited.push(s);
    dist[s] = 0;

    while(!visited.empty()){
        int v = visited.front();
        visited.pop();

        vector<int> nxt;
        for(int to : unvisited){
            if(!mp[{v, to}]){
                visited.push(to);
                dist[to] = dist[v]+1;
            }else{
                nxt.pb(to);
            }
        }

        unvisited = nxt;
    }

    return dist;
}
#endif
