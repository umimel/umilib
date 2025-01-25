#ifndef FLOORKTHROOT_HPP
#define FLOORKTHROOT_HPP

/*depend on*/
#include "../header.hpp"

/*start*/
unsigned long long floor_kth_root(unsigned long long a, int k){
    const __int128_t MAX = 1e20;
    __int128_t lo = 0, hi = MAX, mid;
    __int128_t b = (__int128_t)a;
    while(hi-lo>1){
        mid = (lo + hi)/2;

        bool flg = true;
        __int128_t v = 1;
        for(int i=0; i<k; i++){
            if((MAX+mid-1)/mid <= v){
                flg = false;
                break;
            }
            v *= mid;
        }

        if(!flg){
            hi = mid;
        }else{
            if(v <= b) lo = mid;
            else hi = mid;
        }
    }

    return (unsigned long long)lo;
}
#endif