#ifndef ROOTEDISOMORPHISM_HPP
#define ROOTEDISOMORPHISM_HPP

/*depend on*/
#include "../header.hpp"
#include "../graph/graph-template.hpp"

/*start*/
template<typename W>
vector<int> rooted_isomorphoism(tree<W> T, int root=0){
    int n = (int)T.size();
    vector<vector<ll>> R(n+1, vector<ll>(3, 0)), hash(n, vector<ll>(3, 1));
    vector<int> height(n, 1);

    for(int k=0; k<3; k++) for(int h=1; h<=n; h++) R[h][k] = rng()%MOD[k];
    function<void(int, int)> dfs = [&](int v, int p){
        for(edge<W> e : T[v]) if(e.to != p){
            dfs(e.to, v);
            height[v] = max(height[v], height[e.to]+1);
        }
        for(int k=0; k<3; k++) for(edge<W> e : T[v])
            hash[v][k] = hash[v][k] * (R[height[v]][k]+hash[e.to][k]) % MOD[k];
    }; dfs(root, -1);

    map<vector<ll>, int> mp;
    int kind = 0;
    vector<int> ans(n, 0);
    for(int i=0; i<n; i++){
        if(mp.find(hash[i])==mp.end()){
            ans[i]=kind;
            mp[hash[i]]=kind++;
        }else{
            ans[i]=mp[hash[i]];
        }
    }

    return ans;
}
#endif
