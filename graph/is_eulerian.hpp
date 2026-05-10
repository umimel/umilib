#ifndef IS_EULERIAN_HPP
#define IS_EULERIAN_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename S>
bool is_eulerian(graph<S> &G){
    int n = G.size();
    int m = G.edge_size();
    if(m == 0) return true;

    if(G.get_dir()){
        vector<int> indeg(n, 0), outdeg(n, 0);
        vector<vector<int>> rev(n);
        int s = -1;

        for(int v=0; v<n; v++){
            for(auto e : G[v]){
                outdeg[v]++;
                indeg[e.to]++;
                rev[e.to].push_back(v);
                s = v;
            }
        }

        for(int v=0; v<n; v++) if(indeg[v] != outdeg[v]) return false;
        for(int v=0; v<n; v++) if(indeg[v] + outdeg[v] > 0){
            s = v;
            break;
        }

        auto reach = [&](bool reverse_graph){
            vector<bool> used(n, false);
            queue<int> que;
            used[s] = true;
            que.push(s);
            while(!que.empty()){
                int v = que.front();
                que.pop();
                if(reverse_graph){
                    for(int to : rev[v]) if(!used[to]){
                        used[to] = true;
                        que.push(to);
                    }
                }else{
                    for(auto e : G[v]) if(!used[e.to]){
                        used[e.to] = true;
                        que.push(e.to);
                    }
                }
            }
            for(int v=0; v<n; v++) if(indeg[v] + outdeg[v] > 0 && !used[v]) return false;
            return true;
        };

        return reach(false) && reach(true);
    }

    vector<int> deg(n, 0);
    for(auto e : G.get_edge_set()){
        deg[e.from]++;
        deg[e.to]++;
    }
    for(int v=0; v<n; v++) if(deg[v] % 2 != 0) return false;

    int s = -1;
    for(int v=0; v<n; v++) if(deg[v] > 0){
        s = v;
        break;
    }
    if(s == -1) return true;

    vector<bool> used(n, false);
    queue<int> que;
    used[s] = true;
    que.push(s);
    while(!que.empty()){
        int v = que.front();
        que.pop();
        for(auto e : G[v]) if(!used[e.to]){
            used[e.to] = true;
            que.push(e.to);
        }
    }

    for(int v=0; v<n; v++) if(deg[v] > 0 && !used[v]) return false;
    return true;
}
#endif
