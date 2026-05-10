#ifndef FIND_MIN_NCYCLE_HPP
#define FIND_MIN_NCYCLE_HPP

/*depends on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename S>
edges<S> find_min_ncycle(graph<S> &G){
    int n = G.size();
    const S SINF = numeric_limits<S>::max()/2;
    vector<vector<S>> dist(1<<n, vector<int>(n, SINF));
    vector<vector<edge<S>>> pre_e(1<<n, vector<edge<S>>(n));
    vector<vector<S>> adj(n, vector<S>(n, SINF));
    vector<vector<edge<S>>> adj_e(n, vector<edge<S>>(n));
    for(int v=0; v<n; v++) for(auto e : G[v]){
        if(e.cost < adj[v][e.to]){
            adj[v][e.to] = e.cost;
            adj_e[v][e.to] = e;
        }
    }

    dist[1][0] = 0;

    for(int bit=2; bit<(1<<n); bit++){
        for(int i=0; i<n; i++) if((bit>>i)&1){
            int bit2 = bit ^ (1<<i);
            for(int j=0; j<n; j++) if((bit2>>j)&1){
                if(dist[bit2][j] + adj[j][i] < dist[bit][i]){
                    dist[bit][i] = dist[bit2][j] + adj[j][i];
                    pre_e[bit][i] = adj_e[j][i];
                }
            }
        }
    }

    S cost = SINF;
    edge<S> emin;
    for(int v=1; v<n; v++){
        if(dist[(1<<n)-1][v]+adj[v][0] < cost){
            cost = dist[(1<<n)-1][v]+adj[v][0];
            emin = adj_e[v][0];
        }
    }

    if(cost == SINF) return {};

    edges<S> cyc;
    int state = (1<<n)-1;
    while(true){
        cyc.push_back(emin);
        if(emin.from==0) break;
        emin = pre_e[state][emin.from];
        state = state ^ (1<<emin.to);
    }

    return cyc;
}
#endif
