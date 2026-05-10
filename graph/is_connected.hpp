#ifndef IS_CONNECTED_HPP
#define IS_CONNECTED_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename S>
bool is_connected(graph<S> &G){
    int n = G.size();
    if(n == 0) return true;

    vector<bool> used(n, false);
    queue<int> que;
    used[0] = true;
    que.push(0);

    while(!que.empty()){
        int v = que.front();
        que.pop();
        for(auto e : G[v]) if(!used[e.to]){
            used[e.to] = true;
            que.push(e.to);
        }
    }

    for(int v=0; v<n; v++) if(!used[v]) return false;
    return true;
}
#endif
