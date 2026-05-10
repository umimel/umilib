#ifndef IS_REGULAR_HPP
#define IS_REGULAR_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename S>
bool is_regular(graph<S> &G){
    int n = G.size();
    if(n == 0) return true;

    int deg = (int)G[0].size();
    for(int v=1; v<n; v++) if((int)G[v].size() != deg) return false;
    return true;
}
#endif
