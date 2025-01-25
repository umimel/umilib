#ifndef HUNGARIAN_HPP
#define HUNGARIAN_HPP

/*depend on*/
#include "../../header.hpp"
#include "../../matrix/matrix.hpp"

/*start*/
template<typename S>
pair<S, vector<int>> hungarian(matrix<S> &A){
    const S sinf = numeric_limits<S>::max();
    const int n = (int)A.height();
    const int m = (int)A.width();
    vector<int> P(m), way(m);
    vector<S> U(n, 0), V(m, 0), minV(m, 0);
    vector<bool> used(m, false);

    for(int i=1; i<n; i++){
        P[0] = i;
        for(int j=0; j<m; j++) minV[j] = sinf;
        for(int j=0; j<m; j++) used[j] = false;
        int j0 = 0;
        while(P[j0] != 0){
            int i0 = P[j0], j1 = 0;
            used[j0] = true;
            S delta = sinf;
            for(int j=1; j<m; j++){
                if(used[j]) continue;
                S curr = A[i0][j] - U[i0] - V[j];
                if(curr < minV[j]) minV[j] = curr, way[j] = j0;
                if(minV[j] < delta) delta = minV[j], j1 = j;
            }
            for(int j=0; j<m; j++){
                if(used[j]) U[P[j]] += delta, V[j] -= delta;
                else minV[j] -= delta;
            }
            j0 = j1;
        }
        do{
            P[j0] = P[way[j0]];
            j0 = way[j0];
        }while(j0 != 0);
    }
    return {-V[0], P};
}
#endif