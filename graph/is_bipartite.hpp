#ifndef IS_BIPARTITE_HPP
#define IS_BIPARTITE_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename S>
bool is_bipartite(graph<S> &G){
    int n = G.size();
    vector<int> color(n, -1);

    for(int s=0; s<n; s++) if(color[s] == -1){
        color[s] = 0;
        queue<int> que;
        que.push(s);

        while(!que.empty()){
            int v = que.front();
            que.pop();
            for(auto e : G[v]){
                if(color[e.to] == -1){
                    color[e.to] = 1-color[v];
                    que.push(e.to);
                }else if(color[e.to] == color[v]){
                    return false;
                }
            }
        }
    }

    return true;
}
#endif
