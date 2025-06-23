#ifndef CSSSP_HPP
#define CSSSP_HPP

/*depend on*/
#include "../../header.hpp"
#include "../template.hpp"

/*start*/
vector<int> complement_sssp(unweighted_graph G){
    int n = (int)G.size();
    map<pair<int, int>, bool> mp;
    for(int u=0; u<n; u++) for(int v : G[u]) mp[{u, v}] = true;

    vector<int> dist(n, -1);
    vector<int> unvisited;
    for(int i=1; i<n; i++) unvisited.push_back(i);
    queue<int> visited;
    visited.push(0);
    dist[0] = 0;

    while(!visited.empty()){
        int u = visited.front();
        visited.pop();

        vector<int> nxt;
        for(int v : unvisited){
            if(!mp[{u, v}]){
                visited.push(v);
                dist[v] = dist[u]+1;
            }else{
                nxt.pb(v);
            }
        }

        unvisited = nxt;
    }

    return dist;
}

#endif
