#ifndef IS_FOREST_HPP
#define IS_FOREST_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename S>
bool is_forest(graph<S> &G){
    assert(!G.get_dir());
    int n = G.size();

    vector<int> par(n), siz(n, 1);
    iota(par.begin(), par.end(), 0);

    auto root = [&](auto self, int x) -> int {
        if(par[x] == x) return x;
        return par[x] = self(self, par[x]);
    };

    auto unite = [&](int x, int y) -> bool {
        x = root(root, x);
        y = root(root, y);
        if(x == y) return false;
        if(siz[x] < siz[y]) swap(x, y);
        par[y] = x;
        siz[x] += siz[y];
        return true;
    };

    for(auto e : G.get_edge_set()){
        if(!unite(e.from, e.to)) return false;
    }
    return true;
}
#endif
