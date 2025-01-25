#ifndef MDST_HPP
#define MDST_HPP

/*depend on*/
#include "../../header.hpp"
#include "../graph-template.hpp"
#include "../path/shortest_path.hpp"

/*start*/
template<typename S>
pair<S, edges<S>> minimum_diameter_spanning_tree(graph<S> &G){
    int n = G.get_vnum();
    int m = G.get_enum();
    const S SINF = numeric_limits<S>::max()/2;
    if(n == 1) return {0, {}};

    vector<vector<S>> dist(n);
    for(int s=0; s<n; s++) dist[s] = shortest_path::dijkstra(G, s);

    edge<S> min_e;
    S diam = SINF, t = -1;
    for(int i=0; i<m; i++){
        auto e = G.get_edge(i);
        vector<pair<S, S>> l(n);
        for(int v=0; v<n; v++) l[v] = {(S)2*dist[e.from][v], (S)2*(e.cost+dist[e.to][v])};
        sort(l.begin(), l.end(), greater<pair<S, S>>());

        S tx = 0, ty = l[0].first;
        int idx = 0;
        for(int j=1; j<n; j++){
            if(l[idx].second < l[j].second){
                S x = (l[idx].second - l[j].first)/2;
                S y = -x + l[idx].second;
                if(y < ty && 0 <= x && x <= (S)2*e.cost){
                    tx = x;
                    ty = y;
                }
                idx = j;
            }
        }
        if(l[idx].second-(S)2*e.cost < ty){
            tx = (S)2*e.cost;
            ty = l[idx].second-(S)2*e.cost;
        }

        if(ty < diam){
            diam = ty;
            t = tx;
            min_e = e;
        }
    }

    vector<S> d(n, SINF);
    vector<int> par(n, -1);
    priority_queue<pair<S, int>, vector<pair<S, int>>, greater<>> que;
    d[min_e.from] = t; que.push({d[min_e.from], min_e.from});
    d[min_e.to] = (S)2*min_e.cost - t; que.push({d[min_e.to], min_e.to});
    while(!que.empty()){
        auto [tmp, v] = que.top();
        que.pop();
        if(d[v] < tmp) continue;

        for(auto e : G[v]) if(d[v] + (S)2*e.cost < d[e.to]){
            d[e.to] = d[v] + (S)2*e.cost;
            par[e.to] = e.id;
            que.push({d[e.to], e.to});
        }
    }

    edges<S> es;
    for(int v=0; v<n; v++) if(par[v]!=-1) es.push_back(G.get_edge(par[v]));
    es.push_back(min_e);

    return {diam, es};
}
#endif