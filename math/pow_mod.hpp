#ifndef POWMOD_HPP
#define POWMOD_HPP

/*depend on*/
#include "../header.hpp"

/*start*/
long long pow_mod(long long x, long long n, long long mod){
    long long ret = 1;
    while(n > 0){
        if(n & 1) ret = (ret*x)%mod;
        x = x*x%mod;
        n >>= 1;
    }
    return ret;
}
#endif