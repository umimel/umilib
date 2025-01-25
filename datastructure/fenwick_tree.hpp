#ifndef FENWICH_TREE_HPP
#define FENWICH_TREE_HPP

/*depends on*/
#include "../header.hpp"

/*start*/
template<typename T>
struct fenwick_tree{
    int n;
    vector<T> dat;

    // constractor
    fenwick_tree(){}
    
    fenwick_tree(int n) : n(n){
        dat.resize(n+1, 0);
    }

    /*
        - return sum in range [0, i]
        - query time : O(log n)
    */
    T sum(int i){
        T s = 0;
        while(i > 0){
            s += dat[i];
            i -= i & -i;
        }
        return s;
    }

    /*
        - return sum in range [l, r)
        - query time : O(log n)
    */
    T sum(int l, int r){
        return sum(r) - sum(l);
    }

    /*
        - add x to dat[i+1]
        - processing time : O(log n)
    */
   void add(int i, T x){
        i++;
        while(i <= n){
            dat[i] += x;
            i += i & -i;
        }
    }
};
#endif