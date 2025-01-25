#ifndef LA_HPP
#define LA_HPP

/*depend on*/
#include "../header.hpp"
#include "../graph/graph-template.hpp"

/*start*/
template<typename W>
struct level_ancestor{
    int root = 0;
    int n;
    int log_n = 1;
    vector<vector<int>> par;
    vector<int> depth;

    level_ancestor(tree<W> &T, int root=0) : root(root){
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

    int query(int u, int d){
        if(n <= d) return -1;

        int lva = u;
        int k = 0;
        while(d){
            if(k==log_n){
                lva = -1;
                break;
            }
            if(d&1) lva = par[k][lva];
            if(lva==-1) break;
            d/=2;
            k++;
        }

        return lva;
    }
};
#endif