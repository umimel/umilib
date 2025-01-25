#ifndef UNION_FIND_HPP
#define UNION_FIND_HPP

#include "../header.hpp"

/*start*/
struct union_find{
    vector<int> par;
    vector<int> siz;
    
    union_find(int n) : par(n), siz(n, 1){
        for(int i=0; i<n; i++) par[i] = i;
    }
    
    int root(int x){
        if (par[x] == x) return x;
        return par[x] = root(par[x]);
    }
 
    void unite(int x, int y){
        int rx = root(x);
        int ry = root(y);
        if (rx == ry) return;
        if (siz[rx] < siz[ry]) swap(rx, ry);
        siz[rx] += siz[ry];
        par[ry] = rx;
    }
 
    bool same(int x, int y){
        int rx = root(x);
        int ry = root(y);
        return rx == ry;
    }
 
    int size(int x){
        return siz[root(x)];
    }
};
#endif