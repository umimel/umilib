#ifndef BELLMANFORD_HPP
#define BELLMANFORD_HPP

/*depends on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
vector<long long> bellmanford(graph<long long> &G, int src){
    int n = G.size();
    Edges<long long> E = G.get_edge_set();

    vector<long long> dist(n, LINF);
    dist[src] = 0;
    for(int i=0; i<n; i++){
        for(edge<long long> e : E){
            if(dist[e.from]!=LINF && dist[e.from]+e.cost<dist[e.to]){
                dist[e.to] = dist[e.from] + e.cost;
            }
        }
    }

    vector<bool> flg(n, false);
    for(int i=0; i<n; i++){
        for(edge<long long> e : E){
            if(dist[e.from]!=LINF && dist[e.from]+e.cost<dist[e.to]){
                dist[e.to] = dist[e.from] + e.cost;
                flg[e.to] = true;
            }
        }
    }

    for(int i=0; i<n; i++){
        for(edge<long long> e : E){
            if(flg[e.from]) flg[e.to]=true;
        }
    }

    for(int v=0; v<n; v++) if(flg[v]) dist[v] = -LINF;
    return dist;
}
#endif
