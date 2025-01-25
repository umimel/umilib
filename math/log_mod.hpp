#ifndef LOGMOD_HPP
#define LOGMOD_HPP

/*depend on*/
#include "../header.hpp"
#include "pow_mod.hpp"
#include "euler_phi.hpp"
#include "inv_mod.hpp"

/*description*/
/*
    Descrete Logarithm
    
*/

/*start*/
long long log_mod(long long x, long long y, long long mod){
    if(mod==1) return 0LL;

    long long g = __gcd(x, mod);
    if(y%g!=0) return -1;
    x /= g;
    y /= g;
    mod /= g;

    long long sq = 0;
    while(sq*sq < mod) sq++;

    unordered_map<long long, long long> mp;
    long long v = 1;
    cout << sq << endl;
    for(long long b=1; b<=sq; b++){
        v = v*x%mod;
        cout << y*v%mod << " ";
        mp[y*v%mod] = b;
    }
    cout << endl;

    long long pow_x = pow_mod(x, sq, mod);
    v = 1;
    for(long long a=1; a<=sq+1; a++){
        v = v*pow_x%mod;
        cout << v << " ";
        if(mp.find(v)!=mp.end()) return (a*sq-mp[v]);
    }
    cout << endl;

    return -1;
}
#endif