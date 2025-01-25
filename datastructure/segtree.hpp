#ifndef SEGTREE_HPP
#define SEGTREE_HPP

#include "../header.hpp"

/*start*/
template<typename T, T (*op)(T, T), T (*e)()>
struct segtree{
    int n;
    vector<T> dat;

    segtree(int n_){
        n = 1;
        while(n < n_) n*=2;
        dat.resize(2*n, e());
    }

    void update(int k, T x){
        k += n-1;
        dat[k] = x;
        while(k > 0){
            k = (k-1)/2;
            dat[k] = op(dat[2*k+1], dat[2*k+2]);
        }
    }

    // the prod element of [a, b)
    T query(int a, int b){return query_sub(a, b, 0, 0, n);}

    T query_sub(int a, int b, int k, int l, int r){
        if(r <= a || b <= l){
            return e();
        }else if(a <= l && r <= b){
            return dat[k];
        }else{
            T vl = query_sub(a, b, 2*k+1, l, (l+r)/2);
            T vr = query_sub(a, b, 2*k+2, (l+r)/2, r);
            return op(vl, vr);
        }
    }
};
#endif