#ifndef RECOGNITION_HPP
#define RECOGNITION_HPP

/*depend on*/
#include "../../header.hpp"
#include "../template.hpp"

/*start*/
class recognition{

    template<typename S>
    const bool is_tree(graph<S> &G){
        int n = G.get_vnum();
        int m = G.get_enum();
        if(m != n-1) return false;
        for(int v=0; v<n; v++) if(G[v].empty()) return false;
        return true;
    }

    template<typename S>
    const bool is_path(graph<S> &G){
        int n = G.get_vnum();
        int m = G.get_enum();
        bool dir = G.get_dir();
        if(m != n-1) return false;

        if(dir) for(int v=0; v<n; v++) if((int)G[v].size()>1) return false;
        else for(int v=0; v<n; v++) if((int)G[v].size()>2 || (int)G[v].size()==0) return false;

        return true;
    }

    template<typename S>
    const bool is_bipartite(graph<S> &G){
        int n = G.get_vnum();
        vector<int> c(n, -1);
        bool res = true;
        function<void(int)> dfs = [&](int v){
            for(auto e : G[v]){
                if(c[e.to] == -1){
                    c[e.to] = 1-c[v];
                    dfs(e.to);
                }else if(c[e.to] == c[v]){
                    res = false;
                }
            }
        };
        for(int v=0; v<n; v++) if(c[v]==-1){
            c[v] = 0;
            dfs(v);
        }

        return res;
    }
};
#endif
