#ifndef IS_COMPLETE_HPP
#define IS_COMPLETE_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"
#include "is_simple.hpp"

/*start*/
template<typename S>
bool is_complete(graph<S> &G){
    int n = G.size();
    long long m = G.edge_size();
    if(!is_simple(G)) return false;

    if(G.get_dir()){
        if(m != 1LL*n*(n-1)) return false;
        for(int v=0; v<n; v++) if((int)G[v].size() != n-1) return false;
        return true;
    }

    if(m != 1LL*n*(n-1)/2) return false;
    for(int v=0; v<n; v++) if((int)G[v].size() != n-1) return false;
    return true;
}
#endif
