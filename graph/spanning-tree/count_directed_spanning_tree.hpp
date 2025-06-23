#ifndef COUNT_DIRECTED_SPANNING_TREE_HPP
#define COUNT_DIRECTED_SPANNING_TREE_HPP

/*depend on*/
#include "../header.hpp"
#include "../matrix/matrix.hpp"
#include "template.hpp"

/*start*/
template<typename T>
T count_directed_spanning_tree(weighted_graph<T> &G, int r){
    // weight represents the number of multiple edges.
    int n = (int)G.size();
    if(n <= 1) return T(1);

    matrix<T> L(n-1, n-1);
    for(int s=0; s<n; s++){
        int from = s;
        if(r < from) from--;

        for(edge<T> e : G[s]){
            int to = e.to;
            if(to == s) continue; // ingnore self loop
            if(to == r) continue;
            if(r < to) to--;

            if(s != r && e.to != r) L[from][to] -= e.cost;
            L[to][to] += e.cost;
        }
    }
    
    return L.determinant();
}
#endif
