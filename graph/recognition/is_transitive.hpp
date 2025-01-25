#ifndef IS_TRANSITIVE_HPP
#define IS_TRANSITIVE_HPP

/*depend on*/
#include "../../header.hpp"
#include "../graph-template.hpp"

/*start*/
bool is_transitive(graph<int> &G){
    int n = (int)G.get_vnum();

    vector<bool> conn(n, false);
    for(int v=0; v<n; v++){
        for(auto e : G[v]) conn[e.to] = true;
        for(auto e : G[v]) for(auto f : G[e.to]) if(!conn[f.to]) return false;
        for(auto e : G[v]) conn[e.to] = false;
    }

    return true;
}
#endif