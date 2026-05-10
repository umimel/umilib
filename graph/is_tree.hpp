#ifndef IS_TREE_HPP
#define IS_TREE_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"
#include "is_connected.hpp"

/*start*/
template<typename S>
bool is_tree(graph<S> &G){
    assert(!G.get_dir());
    int n = G.size();
    int m = G.edge_size();
    return m == n-1 && is_connected(G);
}
#endif
