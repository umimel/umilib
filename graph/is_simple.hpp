#ifndef IS_SIMPLE_HPP
#define IS_SIMPLE_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename S>
bool is_simple(graph<S> &G){
    set<pair<int, int>> st;

    for(auto e : G.get_edge_set()){
        int u = e.from, v = e.to;
        if(u == v) return false;
        if(!G.get_dir() && u > v) swap(u, v);
        if(st.find({u, v}) != st.end()) return false;
        st.insert({u, v});
    }

    return true;
}
#endif
