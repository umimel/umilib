#ifndef COUNT_SPANNING_TREE_HPP
#define COUNT_SPANNING_TREE_HPP

/*depend on*/
#include "../header.hpp"
#include "../matrix/matrix.hpp"
#include "template.hpp"

/*start*/
template<typename T>
T count_spanning_tree(unweighted_graph G){
    int n = (int)G.size();
    if(n==1) return T(1);

    matrix<T> L(n);
    for(int v=0; v<n; v++){
        L[v][v] = T((int)G[v].size());
        for(int to : G[v]) L[v][to]-=T(1);
    }

    matrix<T> L11(n-1);
    for(int i=0; i<n-1; i++)for(int j=0; j<n-1; j++) L11[i][j]=L[i+1][j+1];
    return L11.determinant();
}
#endif
