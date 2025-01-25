#ifndef POWMOD_HPP
#define POWMOD_HPP

/*depend on*/
#include "../header.hpp"

/*start*/
template<typename T>
struct combination{
    vector<T> fac, ifac;

    combination(size_t n=0) : fac(1, 1), ifac(1, 1){
        make_table(n);
    }

    void make_table(size_t n){
        if(fac.size()>n) return;
        size_t now = fac.size();
        n = max(n, now*2);
        fac.resize(n+1);
        ifac.resize(n+1);
        for(size_t i=now; i<=n; i++) fac[i] = fac[i-1]*i;
        ifac[n]=T(1)/fac[n];
        for(size_t i=n; i-->now; ) ifac[i] = ifac[i+1]*(i+1);
    }

    T factorial(size_t n){
        make_table(n);
        return fac[n];
    }

    T invfac(size_t n){
        make_table(n);
        return ifac[n];
    }

    T P(size_t n, size_t k){
        if(n < k) return 0;
        make_table(n);
        return fac[n]*ifac[n-k];
    }

    T C(size_t n, size_t k){
        if(n < k) return 0;
        make_table(n);
        return fac[n]*ifac[n-k]*ifac[k];
    }

    T H(size_t n, size_t k){
        if(n==0) return k==0?1:0;
        return C(n-1+k, k);
    }
};
#endif