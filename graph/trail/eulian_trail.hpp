#ifndef EULERIAN_TRAIL_HPP
#define EULERIAN_TRAIL_HPP

/*depends on*/
#include "../header.hpp"
#include "../template.hpp"

/*start*/
template<typename T>
tuple<bool, vector<int>, vector<int>> eulian_trail(graph<T> g){
    int n = g.n;
    int m = g.m;
    if(m==0) return {true, {0}, {}};

    int s = -1, t = -1;
    function<bool()> check_existence = [&](){
        if(g.directed){
            vector<int> deg_diff(n, 0);
            for(auto e : g.es){
                deg_diff[e.to]--;
                deg_diff[e.from]++;
            }

            for(int v=0; v<n; v++){
                if(deg_diff[v] == 0) continue;
                if(deg_diff[v]==-1 && t==-1){
                    t = v;
                }else if(deg_diff[v]==1 && s==-1){
                    s = v;
                }else{
                    return false;
                }
            }

            if((s==-1&&t!=-1)||(s!=-1&&t==-1)) return false;
            if(s==-1 && t==-1){
                for(int v=0; v<n; v++) if(!g[v].empty()){
                    s = t = v;
                    break;
                }
            }
        }else{
            vector<int> deg(n, 0);
            for(auto e : g.es){
                deg[e.to]++;
                deg[e.from]++;
            }

            for(int v=0; v<n; v++){
                if(deg[v]%2 == 0) continue;
                if(s==-1){
                    s = v;
                }else if(t==-1){
                    t = v;
                }else{
                    return false;
                }
            }

            if(s!=-1&&t==-1) return false;
            if(s==-1 && t==-1){
                for(int v=0; v<n; v++) if(!g[v].empty()){
                    s = t = v;
                    break;
                }
            }
        }

        return true;
    };

    if(!check_existence()) return {false, {}, {}};

    vector<int> vtrail, etrail;
    function<void(int, int)> dfs = [&](int v, int e_id){
        while(!g[v].empty()){
            auto e = g[v].back();
            g[v].pop_back();
            if(!g.directed){
                for(int i=0; i<(int)g[e.to].size(); i++){
                    if(g[e.to][i].id == e.id){
                        g[e.to].erase(g[e.to].begin() + i);
                        break;
                    }
                }
            }
            dfs(e.to, e.id);
        }
        vtrail.push_back(v);
        etrail.push_back(e_id);
    }; dfs(s, -1);

    for(int v=0; v<n; v++) if(!g[v].empty()) return {false, {}, {}};

    reverse(vtrail.begin(), vtrail.end());
    etrail.pop_back();
    reverse(etrail.begin(), etrail.end());

    return {true, vtrail, etrail};
}
#endif
