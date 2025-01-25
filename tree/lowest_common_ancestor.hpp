#ifndef LCA_HPP
#define LCA_HPP

/*depend on*/
#include "../header.hpp"
#include "../graph/graph-template.hpp"

/*start*/
template<typename W>
struct lowest_common_ancestor{
    int root = 0;
    int n;
    int log_n = 1;
    vector<vector<int>> par;
    vector<int> depth;

    lowest_common_ancestor(){}
    lowest_common_ancestor(tree<W> &T, int root=0) : root(root){
        n = (int)T.size();
        while((1 << log_n) < n) log_n++;
        par.resize(log_n, vector<int>(n, -1));
        depth.resize(n, 0);
        init(T);
    }

    void init(tree<W> &T){
        function<void(int, int)> dfs = [&](int v, int p){
            par[0][v] = p;
            for(edge<W> e : T[v]) if(e.to!=p){
                depth[e.to] = depth[v]+1;
                dfs(e.to, v);
            }
        };

        dfs(root, -1);
        for(int k=0; k+1<log_n; k++){
            for(int v=0; v<n; v++){
                if(par[k][v]<0) par[k+1][v] = -1;
                else par[k+1][v] = par[k][par[k][v]];
            }
        }
    }

    int query(int u, int v){
        if(depth[u] < depth[v]) swap(u, v);
        for(int k=0; k<log_n; k++) if((depth[u]-depth[v]) >> k & 1) u = par[k][u];

        if(u == v) return u;
        for(int k=log_n-1; k>=0; k--){
            if(par[k][u] != par[k][v]){
                u = par[k][u];
                v = par[k][v];
            }
        }

        return par[0][u];
    }
};
#endif