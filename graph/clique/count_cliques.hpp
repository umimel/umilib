#ifndef COUNT_CLIQUES_HPP
#define COUNT_CLIQUES_HPP

/*depend on*/
#include "../header.hpp"
#include "template.hpp"

/*start*/
template<typename T>
T count_cliques(unweighted_graph &G, vector<T> x){
    int n = (int)G.size();
    int m = 0;
    for(int v=0; v<n; v++) m += (int)G[v].size();

    int sq = 0;
    while(sq*sq<m) sq++;

    vector<bitset<100>> A(n);
    vector<int> deg(n, 0);
    for(int v=0; v<n; v++) for(int to : G[v]){
        A[v][to]=1;
        deg[v]++;
    }

    T ans = 0;
    for(int v=0; v<n; v++) if(deg[v]<sq){
        vector<int> neighbors;
        for(int i=0; i<n; i++) if(A[v][i]) neighbors.push_back(i);
        ans += x[v];

        int siz = (int)neighbors.size();
        for(int bit=1; bit<(1<<siz); bit++){
            A[v].reset();
            A[v][v] = 1;
            for(int i=0; i<siz; i++) if((bit>>i)&1){
                A[v][neighbors[i]] = 1;
            }

            T sa = x[v];
            for(int i=0; i<siz; i++) if((bit>>i)&1){
                A[neighbors[i]][neighbors[i]] = 1;
                if((A[neighbors[i]]&A[v])==A[v]) sa *= x[neighbors[i]];
                else{
                    sa = 0;
                    A[neighbors[i]][neighbors[i]] = 0;
                    break;
                }
                A[neighbors[i]][neighbors[i]] = 0;
            }

            ans += sa;
        }

        for(int to : neighbors) A[to][v] = 0;
    }

    vector<int> rest;
    for(int v=0; v<n; v++)if(deg[v]>=sq) rest.push_back(v);
    int siz = (int)rest.size();
    bitset<100> bs;
    for(int bit=1; bit<(1<<siz); bit++){
        bs.reset();
        for(int i=0; i<siz; i++)if((bit>>i)&1) bs[rest[i]]=1;
        T sa = 1;
        for(int i=0; i<siz; i++)if((bit>>i)&1){
            A[rest[i]][rest[i]]=1;
            if((A[rest[i]]&bs)==bs) sa *= x[rest[i]];
            else{
                sa = 0;
                A[rest[i]][rest[i]]=0;
                break;
            }
            A[rest[i]][rest[i]]=0;
        }

        ans += sa;
    }

    return ans;
}
#endif