#ifndef INVMOD_HPP
#define INVMOD_HPP

/*depend on*/
#include "../header.hpp"
#include "euler_phi.hpp"
#include "pow_mod.hpp"

/*start*/
long long inv_mod(long long x, long long mod){
    long long g = __gcd(x, mod);
    if(g!=1) return -1;
    long long y = euler_phi(mod);
    return pow_mod(x, y-1, mod);
}
#endif