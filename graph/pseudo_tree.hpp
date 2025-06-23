#ifndef PSEUDO_TREE_HPP
#define PSEUDO_TREE_HPP

/*depend on*/
#include "../header.hpp"
#include "template.hpp"

/*start*/
template<typename T>
struct pseudo_tree{
    int n;
    vector<int> root; //root[閉路上の頂点]=自身の頂点, root[閉路外の頂点]=根頂点
    vector<int> par; //par[閉路上の頂点]=-1, par[閉路外の頂点]=親頂点

    pseudo_tree(weighted_graph<T> &graph){
        n = (int)graph.size();
        root.resize(n, 0); for(int v=0; v<n; v++) root[v]=v;
        par.resize(n, -1);
        init(graph);
    }

    void init(weighted_graph<T> &graph){
        //閉路上の頂点を決定
        vector<int> deg(n, 0);
        for(int v=0; v<n; v++) deg[v] = (int)graph[v].size();
        queue<int> Q;
        for(int v=0; v<n; v++) if(deg[v]==1) Q.push(v);

        while(!Q.empty()){
            int v = Q.front();
            Q.pop();
            root[v] = -1;
            for(edge<T> e : graph[v]){
                deg[e.to]--;
                if(deg[e.to]==1) Q.push(e.to);
            }
        }

        function<void(int, int)> dfs = [&](int v, int p){
            root[v] = root[p];
            par[v] = p;
            for(edge<T> e : graph[v]) if(e.to != p) dfs(e.to, v);
        };

        for(ll r=0; r<n; r++){
            if(root[r]!=r) continue;
            for(edge<T> e : graph[r]){
                if(root[e.to]==e.to) continue;
                dfs(e.to, r);
            }
        }
    }

    vector<int> get_cycle(){
        
    }

    int get_root(int v){return root[v];}
    int get_par(int v){return par[v];}
};
#endif
