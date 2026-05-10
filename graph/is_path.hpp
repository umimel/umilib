#ifndef IS_PATH_HPP
#define IS_PATH_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"
#include "is_connected.hpp"

/*start*/
template<typename S>
bool is_path(graph<S> &G){
    assert(!G.get_dir());
    int n = G.size();
    int m = G.edge_size();
    if(n == 0) return true;
    if(n == 1) return m == 0;
    if(m != n-1 || !is_connected(G)) return false;

    int leaf = 0;
    for(int v=0; v<n; v++){
        int deg = (int)G[v].size();
        if(deg == 1) leaf++;
        else if(deg != 2) return false;
    }

    return leaf == 2;
}
#endif
