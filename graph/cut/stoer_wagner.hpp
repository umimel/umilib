#ifndef GLOBAL_MIN_CUT_HPP
#define GLOBAL_MIN_CUT_HPP

/*depend on*/
#include "../../header.hpp"
#include "../graph-template.hpp"

/*start*/
template<typename T>
T stoer_wagner(graph<T> &_G){
    int n = _G.get_vnum();
    vector<map<int, T>> G(n);
    for(int v=0; v<n; v++) for(auto &e : _G[v]) G[v][e.to] += e.cost;

    T gmc = numeric_limits<T>::max();
    priority_queue<pair<T, int>> que;
    for(int siz=n; siz>=2; siz--){
        vector<T> sum(n, 0);
        for(auto [to, cost] : G[0]){
            sum[to] = cost;
            que.push({cost, to});
        }

        vector<int> ord;
        vector<bool> used(n, false);
        used[0] = true;
        ord.push_back(0);
        while(!que.empty()){
            auto [s, v] = que.top();
            que.pop();
            if(used[v]) continue;
            used[v] = true;
            ord.push_back(v);

            for(auto [to, cost] : G[v]) if(!used[to]){
                sum[to] += cost;
                que.push({sum[to], to});
            }
        }

        T csum = 0;
        for(auto [to, cost] : G[ord[siz-1]]) csum += cost;
        gmc = min(gmc, csum);

        int u = ord[siz-2], v = ord[siz-1];
        if(u > v) swap(u, v);
        // merge v to u
        G[u].erase(v);
        for(auto [to, cost] : G[v]) if(to != u){
            G[u][to] += cost;
            G[to][u] += cost;
            G[to].erase(v);
        }
    }

    return gmc;
}
#endif