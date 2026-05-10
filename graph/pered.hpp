#ifndef PERED_HPP
#define PERED_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename T>
vector<vector<pair<int, T>>> pered(graph<T> &G, int k){
    int n = G.size();
    const T TINF = numeric_limits<T>::max()/3;
    priority_queue<tuple<T, int, int>, vector<tuple<T, int, int>>, greater<>> que;
    vector<vector<pair<int, T>>> neibors(n);
    vector<unordered_map<int, T>> mp(n);
    for(int v=0; v<n; v++){
        que.push({0, v, v});
        mp[v][v] = 0;
    }
    while(!que.empty()){
        auto [d, v, s] = que.top();
        que.pop();
        if((int)neibors[v].size()==k) continue;
        if(mp[v].find(s)!=mp[v].end()) if(mp[v][s] < d) continue;
        neibors[v].push_back({s, d});

        for(auto e : G[v]){
            if((int)neibors[e.to].size()==k) continue;
            if(mp[e.to].find(s)==mp[e.to].end()) mp[e.to][s] = TINF;
            if(d + e.cost < mp[e.to][s]){
                mp[e.to][s] = d + e.cost;
                que.push({d+e.cost, e.to, s});
            }
        }
    }

    return neibors;
}
#endif
