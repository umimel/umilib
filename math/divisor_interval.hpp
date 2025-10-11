#ifndef DIV_ITV_HPP
#define DIV_ITV_HPP

/*depend on*/
#include "../header.hpp"

/*start*/
/**
 * calculate the y's interval such that floor(n/y) = x. (1 <= x <= n)
 */
pair<long, long> divisor_interval(long long n, long long x){
    assert(1 <= x && x <= n);
    assert(0 <= n);
    long long L, R;
    {
        long long lo = 0, hi = n+1, mid;
        while(hi-lo>1){
            mid = (lo + hi) / 2;
            if (n/mid < x) hi = mid;
            else lo = mid;
        }
        R = lo; // R is the first where floor(n/b) < x
    }
    {
        long long lo = 0, hi = n, mid;
        while(hi-lo>1){
            mid = (lo + hi) / 2;
            if (n/mid <= x) hi = mid;
            else lo = mid;
        }
        L = hi; // L is the first where floor(n/b) <= x
    }
    return {L, R};
}
#endif