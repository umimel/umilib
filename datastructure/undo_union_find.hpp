#ifndef UNDO_UNION_FIND_HPP
#define UNDO_UNION_FIND_HPP

#include "../header.hpp"

/*start*/
struct undo_union_find{
    vector<int> par;
    vector<int> siz;
    stack<pair<int, int>> st;
    
    undo_union_find(int n) : par(n), siz(n, 1){
        for(int i=0; i<n; i++) par[i] = i;
    }
    
    int root(int x){
        if (par[x] == x) return x;
        return root(par[x]);
    }
 
    void unite(int x, int y){
        int rx = root(x);
        int ry = root(y);
        if (siz[rx] < siz[ry]) swap(rx, ry);
        st.push({rx, ry});
        if (rx == ry) return;
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

    void undo(){
        int rx = st.top().first;
        int ry = st.top().second;
        st.pop();
        if(rx!=ry){
            siz[rx] -= siz[ry];
            par[ry] = ry;
        }
    }
};
#endif