#ifndef WPT_HPP
#define WPT_HPP

/*depends on*/
#include "../header.hpp"
#include <atcoder/all>
using namespace atcoder;

/*start*/
vector<bool> wildcard_pattern_matching(string T, string P){
    int n = (int)T.size();
    int m = (int)P.size();
    reverse(P.begin(), P.end());

    unordered_map<char, long long> mp;
    long long now = 1;
    mp['*'] = 0; // * : wildcard
    for(int i=0; i<n; i++) if(mp.find(T[i])==mp.end()) mp[T[i]] = now++;
    for(int j=0; j<m; j++) if(mp.find(P[j])==mp.end()) mp[P[j]] = now++;

    vector<vector<long long>> NT(4, vector<long long>(n, 1)), NP(4, vector<long long>(m, 1));
    for(int i=1; i<=3; i++) for(int j=0; j<n; j++) NT[i][j] = NT[i-1][j] * mp[T[j]];
    for(int i=1; i<=3; i++) for(int j=0; j<m; j++) NP[i][j] = NP[i-1][j] * mp[P[j]];

    vector<vector<long long>> C(3);
    C[0] = convolution(NT[3], NP[1]);
    C[1] = convolution(NT[2], NP[2]);
    C[2] = convolution(NT[1], NP[3]);

    vector<bool> match(n-m+1, false);
    for(int i=m-1; i<n; i++){
        long long wcmp = C[0][i] - 2*C[1][i] + C[2][i];
        if(wcmp == 0LL) match[i-m+1] = true;
    }

    return match;
}
#endif