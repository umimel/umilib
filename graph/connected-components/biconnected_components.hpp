#ifndef BICONNETEDCOMPONENTS_HPP
#define BICONNETEDCOMPONENTS_HPP

/*depend on*/
#include "../../header.hpp"
#include "../template.hpp"
#include "lowlink.hpp"

/*start*/
struct biconnected_components{
    unweighted_graph G;
    lowlink L;
    int n;
    vector<bool> used;
    vector<vector<pair<int, int>>> bc;
    stack<pair<int, int>> st;
    vector<int> diff;

    biconnected_components(unweighted_graph G) : G(G){
        n = (int)G.size();
        used.resize(n, false);
        L = lowlink(G);
        for(int v=0; v<n; v++) if(!used[v]) dfs(v, -1, G);
        diff.resize(n, -1);
        for(int v=0; v<n; v++){
            if((int)G[v].size()==0) diff[v]=-1;
            else if(!L.is_articulation(v)) diff[v] = 0;
        }
        
        for(vector<pair<int, int>> comp : bc){
            set<int> st;
            for(auto [u, v] : comp){
                if(L.is_articulation(u)){
                    if(st.find(u)==st.end()){
                        diff[u]++;
                    }
                    st.insert(u);
                }
                if(L.is_articulation(v)){
                    if(st.find(v)==st.end()){
                        diff[v]++;
                    }
                    st.insert(v);
                }
            }
        }
    }

    void dfs(int v, int p, unweighted_graph &G){
        used[v] = true;
        for(int to : G[v]){
            if(to==p) continue;
            if(!used[to] || L.ord[to]<L.ord[v]){
                st.push({min(v, to), max(v, to)});
            }
            if(!used[to]){
                dfs(to, v, G);
                if(L.low[to]>=L.ord[v]){
                    bc.push_back({});
                    for(;;){
                        pair<int, int> e = st.top();
                        bc.back().push_back(e);
                        st.pop();
                        if(e.first==min(v, to)&&e.second==max(v, to)){
                            break;
                        }
                    }
                }
            }
        }
    }

    // 二重頂点連結成分分解
    vector<vector<int>> get_bc(){
        vector<vector<int>> res;

        for(vector<pair<int, int>> comp : bc){
            vector<int> vset;
            for(auto [u, v] : comp){
                vset.push_back(u);
                vset.push_back(v);
            }
            sort(all(vset));
            vset.erase(unique(all(vset)), vset.end());
            res.push_back(vset);
        }

        for(int v=0; v<n; v++)if((int)G[v].size()==0) res.push_back({v});

        return res;
    }

    // 頂点vを削除した際に, 連結成分の変化数
    int get_diff(int v){return diff[v];}
};
#endif