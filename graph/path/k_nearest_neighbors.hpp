#ifndef K_NEAREST_NEIGHBORS_HPP
#define K_NEAREST_NEIGHBORS_HPP

/*depend on*/
#include "../header.hpp"
#include "template.hpp"

/*start*/
template<typename T>
vector<vector<int>> k_nearest_neighbors(weighted_graph<T> &G, int k){
    int n = (int)G.size();
    priority_queue<tuple<T, int, int>, vector<tuple<T, int, int>>, greater<tuple<T, int, int>>> PQ;
    vector<vector<int>> neibors(n);
    vector<unordered_map<int, bool>> mp(n);
    for(int v=0; v<n; v++) PQ.push({0, v, v});
    while(!PQ.empty()){
        auto [d, v, s] = PQ.top();
        PQ.pop();
        if(mp[v][s]) continue;
        mp[v][s] = true;
        neibors[v].push_back(s);

        for(edge<T> e : G[v]){
            if(mp[e.to][s]) continue;
            if((int)neibors[e.to].size()==k) continue;
            PQ.push({d+e.cost, e.to, s});
        }
    }

    return neibors;
};
#endif