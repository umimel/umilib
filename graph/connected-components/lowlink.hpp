#ifndef LOWLINK_HPP
#define LOWLINK_HPP

/*depend on*/
#include "../header.hpp"
#include "../graph-template.hpp"

/*start*/
struct lowlink{
    int n;
    vector<bool> used;
    vector<int> ord, low;
    vector<int> aps;
    vector<bool> vec_aps;
    vector<pair<int, int>> bridges;

    lowlink(){}
    lowlink(unweighted_graph &G){
        n = (int)G.size();
        used.resize(n, false);
        ord.resize(n, 0);
        low.resize(n, 0);
        int k = 0;
        for(int v=0; v<n; v++){
            if(!used[v]) k = dfs(v, k, -1, G);
        }
        vec_aps.resize(n, false);
        for(int a : aps) vec_aps[a] = true;
    }

    int dfs(int v, int k, int p, unweighted_graph &G){
        used[v] = true;
        ord[v] = k++;
        low[v] = ord[v];
        bool is_aps = false;
        int cnt = 0;
        for(int to : G[v]){
            if(!used[to]){
                cnt++;
                k = dfs(to, k, v, G);
                low[v] = min(low[v], low[to]);
                if(p!=-1 && ord[v] <= low[to]) is_aps = true;
                if(ord[v] < low[v]) bridges.push_back({min(v, to), max(v, to)});
            }else{
                low[v] = min(low[v], ord[to]);
            }
        }
        if(p==-1 && cnt>=2) is_aps = true;
        if(is_aps) aps.push_back(v);
        return k;
    }

    bool is_articulation(int v){return vec_aps[v];}
};
#endif
