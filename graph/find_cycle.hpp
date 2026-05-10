#ifndef FIND_CYCLE_HPP
#define FIND_CYCLE_HPP

/*depends on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename S>
edges<S> find_cycle(graph<S> &G){
    int n = G.size();
    vector<int> used(n, 0);
    edges<S> cyc;

    function<bool(int, int)> dfs = [&](int v, int e_id){
        for(auto e : G[v]) if(e.id != e_id){
            if(used[e.to]==1){
                cyc.push_back(e);
                return true;
            }else if(used[e.to]==0){
                used[e.to] = used[v];
                if(dfs(e.to, e.id)){
                    cyc.push_back(e);
                    return true;
                }
            }
        }

        used[v] = 2;
        return false;
    };

    for(int v=0; v<n; v++) if(used[v]==0){
        used[v] = 1;
        if(dfs(v, -1)) break;
    }

    if(cyc.empty()) return cyc;
    while(cyc.back().from != cyc[0].to) cyc.pop_back();
    reverse(cyc.begin(), cyc.end());

    return cyc;
}
#endif
