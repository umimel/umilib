#ifndef SPARSE_TABLE_HPP
#define SPARSE_TABLE_HPP

/*depends on*/
#include "../header.hpp"

/*start*/
template<typename T, T (*op)(T, T), T (*e)()>
struct sparse_table{
    int n, m;
    vector<int> log_table;
    vector<vector<T>> table;

    sparse_table(vector<T> vec){
        n = (int)vec.size();
        log_table.resize(n+1, 0);
        for(int i=2; i<=n; i++) log_table[i] = log_table[i>>1]+1;
        m = log_table[n];
        table.resize(n, vector<T>(m+1, e()));
        for(int i=0; i<n; i++) table[i][0] = vec[i];
        for(int j=1; j<=m; j++){
            for(int i=0; i<n; i++){
                if(i+(1<<(j-1))<n) table[i][j] = op(table[i][j-1], table[i+(1<<(j-1))][j-1]);
                else table[i][j] = table[i][j-1];
            }
        }
    }

    // prod of [l, r)
    T prod(int l, int r){
        assert(l<=r);
        if(l==r) return e();
        int k = log_table[r-l];
        return op(table[l][k], table[r-(1<<k)][k]);
    }
};
#endif