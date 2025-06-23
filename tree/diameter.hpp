#ifndef DIAMETER_HPP
#define DIAMETER_HPP

/*depend on*/
#include "../header.hpp"
#include "../graph/graph-template.hpp"

/*start*/
template<typename S>
struct diameter{
    int n;
    int s=0, t=0;
    vector<int> path;
    S diam = 0;

    diameter(graph<S> &T){
        n = (int)T.size();

        vector<S> dist1(n, 0);
        function<void(int, int)> dfs1 = [&](int v, int p){
            for(auto e : T[v]) if(e.to!=p){
                dist1[e.to] = dist1[v] + e.cost;
                dfs1(e.to, v);
            }
        };
        dfs1(0, -1);
        S sdist = 0;
        for(int v=0; v<n; v++){
            if(sdist < dist1[v]){
                s = v;
                sdist = dist1[v];
            }
        }

        vector<S> dist2(n, 0);
        function<void(int, int)> dfs2 = [&](int v, int p){
            for(auto e : T[v]) if(e.to!=p){
                dist2[e.to] = dist2[v] + e.cost;
                dfs2(e.to, v);
            }
        };
        dfs2(s, -1);
        S tdist = 0;
        for(int v=0; v<n; v++){
            if(tdist < dist2[v]){
                t = v;
                tdist = dist2[v];
            }
        }

        diam = tdist;

        function<bool(int, int)> dfs3 = [&](int v, int p){
            if(v == t){
                path.pb(v);
                return true;
            }

            bool flag = false;
            for(auto e : T[v]) if(e.to!=p){
                flag = flag | dfs3(e.to, v);
            }

            if(flag) path.pb(v);
            return flag;
        };

        dfs3(s, -1);
    }

    pair<int, int> get_endpoints(){return {s, t};}
    vector<int> get_path(){return path;}
    S get_distance(){return diam;}
};
#endif
