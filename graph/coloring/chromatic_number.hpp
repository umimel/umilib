#ifndef CHROMATIC_NUMBER_HPP
#define CHROMATIC_NUMBER_HPP

/*depend on*/
#include "../header.hpp"
#include "../convolution/bitwise_or_convolution.hpp"
#include "../graph-template.hpp"

/*start*/
int chromatic_number(unweighted_graph &G){
    int n = (int)G.size();
    vector<vector<bool>> A(n, vector<bool>(n, false)); //隣接行列
    for(int v=0; v<n; v++)for(int to : G[v]) A[v][to] = true;

    vector<bool> connected(1<<n, false);
    for(int bit=0; bit<(1<<n); bit++){
        connected[bit] = false;
        for(int u=0; u<n-1; u++)for(int v=u+1; v<n; v++){
            if(A[u][v]&&((bit>>u)&1)&&((bit>>v)&1)) connected[bit]=true;
        }
    }

    vector<vector<int>> dp(n+1, vector<int>(1<<n, 0));
    //初期化
    dp[0][0] = 1;
    for(int bit=0; bit<(1<<n); bit++) if(!connected[bit]) dp[1][bit] = 1;

    //遷移
    for(int k=2; k<=n; k++){
        for(int k1=0; k1<=k/2; k1++){
            int k2 = k-k1;
            vector<int> c = bitwise_or_convolution<int>(dp[k1], dp[k2]);
            for(int bit=0; bit<(1<<n); bit++)if(dp[k][bit]==0&&c[bit]>0){
                dp[k][bit] = 1;
            }
        }
    }

    for(int k=0; k<=n; k++) if(dp[k][(1<<n)-1]) return k;
    assert(false);
    return -1;
}
#endif
