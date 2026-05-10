#ifndef IS_BICONNECTED_HPP
#define IS_BICONNECTED_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"
#include "is_connected.hpp"

/*start*/
template<typename S>
bool is_biconnected(graph<S> &G){
    assert(!G.get_dir());
    int n = G.size();
    if(n <= 2) return is_connected(G);
    if(!is_connected(G)) return false;

    vector<int> ord(n, -1), low(n, 0);
    int timer = 0;
    bool ok = true;

    auto dfs = [&](auto self, int v, int pe) -> void {
        ord[v] = low[v] = timer++;
        int child = 0;

        for(auto e : G[v]){
            if(e.id == pe) continue;
            if(ord[e.to] == -1){
                child++;
                self(self, e.to, e.id);
                low[v] = min(low[v], low[e.to]);
                if(pe != -1 && ord[v] <= low[e.to]) ok = false;
            }else{
                low[v] = min(low[v], ord[e.to]);
            }
        }

        if(pe == -1 && child >= 2) ok = false;
    };

    dfs(dfs, 0, -1);
    return ok;
}
#endif
