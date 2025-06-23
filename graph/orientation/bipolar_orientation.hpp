#ifndef BIPOLAR_HPP
#define BIPOLAR_HPP

/*depends on*/
#include "../../header.hpp"
#include "template.hpp"

/*start*/
/**
 * Problem : Bipoler Orientation
 * Input : undirected graph G, source s and sink t
 * Return Value : 
 *      - direction ((u, v)->false, (v, u)->true) for each edge (u, v)
 *      - If there is not orientaion, return empty set
 * Time Complexity : O(m + n)
 * Space Complexity : O(m + n)
 * Constraint : 2<=n, 1<=m, s!=t
 */
template<typename S>
vector<bool> bipolar_orientation(graph<S> G, int s, int t){
    int n = G.get_vnum();
    int m = G.get_enum();

    vector<bool> orient(m, false);

    vector<int> par(n), ord(n, -1), low(n);
    vector<bool> sign(n, false);
    vector<int> preord; // preorder
    vector<int> pre(n, -1), nxt(n, -1); // 双方向連結リスト

    int cur = 1;
    function<void(int)> dfs = [&](int v){
        ord[v] = cur++;
        low[v] = v;
        for(auto e : G[v]){
            if(ord[e.to]==-1){
                preord.push_back(e.to);
                dfs(e.to);
                par[e.to] = v;
                if(ord[low[e.to]]<ord[low[v]]) low[v] = low[e.to];
            }else if(ord[e.to]<ord[low[v]]){
                low[v] = e.to;
            }
        }
    };
    
    ord[s] = 0;
    dfs(t);

    if(cur<n) return {};
    for(int v=0; v<n; v++) if(v!=s&&v!=t){
        if(ord[par[v]]<=ord[low[v]]) return {};
    }

    nxt[s] = t, pre[t] = s;
    for(int v : preord){
        if(!sign[low[v]]){
            pre[v] = pre[par[v]];
            nxt[v] = par[v];
            nxt[pre[v]] = v;
            pre[par[v]] = v;
            sign[par[v]] = true;
        }else{
            pre[v] = par[v];
            nxt[v] = nxt[par[v]];
            nxt[par[v]] = v;
            pre[nxt[v]] = v;
            sign[par[v]] = false;
        }
    }

    vector<int> numbering(n, 0);
    cur = s;
    for(int i=0; i<n; i++){
        numbering[cur] = i;
        cur = nxt[cur];
    }

    for(int i=0; i<m; i++){
        auto e = G.get_edge(i);
        orient[i] = (numbering[e.from] > numbering[e.to]);
    }

    return orient;
}
#endif
