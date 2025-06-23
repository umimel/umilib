#ifndef MIS_HPP
#define MIS_HPP

/*depend on*/
#include "../header.hpp"
#include "../graph-template.hpp"

/*start*/
vector<int> maximum_independent_set(unweighted_graph G){
    int n = (int)G.size();

    //半分に分けて, MISかどうかを判定
    int n1 = n/2;
    int n2 = n-n1;
    vector<vector<int>> G1(n1), G2(n2), G3(n);
    for(int u=0; u<n1; u++){
        for(int v : G[u]){
            if(v < n1) G1[u].pb(v);
            else G3[u].pb(v);
        }
    }
    for(int u=n1; u<n; u++){
        for(int v : G[u]){
            if(n1 <= v) G2[u-n1].pb(v);
            else G3[u].pb(v);
        }
    }

    vector<bool> ok1((1<<n1), true);
    //初期化
    for(int u=0; u<n1; u++){
        for(int v : G1[u]){
            ok1[(1<<u) | (1<<v)] = false;
        }
    }
    //遷移
    rep(bit, (1<<n1)){
        for(int i=0; i<n1; i++){
            ok1[bit | (1<<i)] = (ok1[bit | (1<<i)] & ok1[bit]);
        }
    }

    vector<pair<int, int>> dp((1<<n1), {-1, -1});
    //初期化
    rep(bit, (1<<n1)) if(ok1[bit]){
        int cnt = 0;
        rep(i, n1) if((bit>>i) & 1) cnt++;
        dp[bit] = {cnt, bit};
    }
    //遷移
    rep(bit, (1<<n1)) rep(i, n1) dp[bit] = max(dp[bit], dp[bit & ((1<<n1)-1-(1<<i))]);

    vector<bool> ok2((1<<n2), true);
    //初期化
    for(int u=0; u<n2; u++){
        for(int v : G2[u]){
            ok2[(1<<u) | (1<<(v-n1))] = false;
        }
    }
    //遷移
    rep(bit, (1<<n2)) rep(i, n2) ok2[bit | (1<<i)] = (ok2[bit | (1<<i)] & ok2[bit]);

    vector<int> dp2(n2, (1<<n1)-1);
    for(int u=n1; u<n; u++){
        for(int v : G3[u]) dp2[u-n1] &= ~(1<<v);
    }

    vector<int> ans;
    rep(bit, (1<<n2)){
        if(!ok2[bit]) continue;
        int b = (1<<n1)-1;
        int cnt = 0;
        rep(i, n2){
            if((bit>>i) & 1){
                cnt++;
                b &= dp2[i];
            }
        }
        
        if(dp[b].first == -1) continue;
        if((ll)ans.size()<dp[b].first + cnt){
            ans.clear();
            for(int v=0; v<n1; v++) if((dp[b].second>>v)&1) ans.pb(v);
            for(int v=n1; v<n; v++) if((bit>>(v-n1))&1) ans.pb(v);
        }
    }

    return ans;
}
#endif
