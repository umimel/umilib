#ifndef IS_DAG_HPP
#define IS_DAG_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename S>
bool is_dag(graph<S> &G){
    assert(G.get_dir());
    int n = G.size();
    vector<int> indeg(n, 0);
    for(int v=0; v<n; v++) for(auto e : G[v]) indeg[e.to]++;

    queue<int> que;
    for(int v=0; v<n; v++) if(indeg[v] == 0) que.push(v);

    int cnt = 0;
    while(!que.empty()){
        int v = que.front();
        que.pop();
        cnt++;
        for(auto e : G[v]){
            indeg[e.to]--;
            if(indeg[e.to] == 0) que.push(e.to);
        }
    }

    return cnt == n;
}
#endif
