#ifndef FIND_MIN_CYCLE_HPP
#define FIND_MIN_CYCLE_HPP

/*depends on*/
#include "../header.hpp"
#include "graph.hpp"
#include "shortest_path_tree.hpp"

/*start*/
template<typename S>
edges<S> min_weight_cycle(graph<S> &G, int s){
    int n = G.size();
    const S SINF = numeric_limits<S>::max()/3;
    bool dir = G.get_dir();
    shortest_path_tree<S> dijk(G, s);
    auto dist = dijk.get_dist();
    edges<S> cyc;

    if(dir){
        S cost = SINF;
        edge<S> emin; 
        for(int v=0; v<n; v++) for(auto e : G[v]) if(e.to == s){
            if(dist[v] + e.cost < cost){
                cost = dist[v] + e.cost;
                emin = e;
            }
        }

        if(cost == SINF) return {};
        cyc = dijk.get_epath(emin.from);
        cyc.push_back(emin);
    }

    if(!dir){
        vector<vector<int>> ch(n);
        for(int v=0; v<n; v++) if(v != s && dijk.get_vpar(v)!=-1){
            ch[dijk.get_vpar(v)].push_back(v);
        }
        
        vector<int> label(n, -1);
        label[s] = s;
        function<void(int, int)> labeling = [&](int v, int l){
            label[v] = l;
            for(int to : ch[v]) labeling(to, l);
        };
        for(int to : ch[s]) labeling(to, to);

        S cost = SINF;
        edge<S> emin;
        for(int v=0; v<n; v++) if(v != s) for(auto e : G[v]){
            if(e.id != dijk.get_epar(v).id && label[v] != label[e.to] && dist[v] + dist[e.to] + e.cost < cost){
                cost = dist[v] + dist[e.to] + e.cost;
                emin = e;
            }   
        }

        if(cost == SINF) return {};

        cyc = dijk.get_epath(emin.from);
        cyc.push_back(emin);
        auto epath = dijk.get_epath(emin.to);
        reverse(epath.begin(), epath.end());
        for(auto e : epath){
            e.reverse();
            cyc.push_back(e);
        }
    }

    return cyc;
}

template<typename S>
edges<S> min_weight_cycle(graph<S> &G){
    int n = G.size();
    const S SINF = numeric_limits<S>::max()/2;
    S cost = SINF;
    edges<S> min_cyc;
    
    for(int s=0; s<n; s++){
        auto cyc = min_weight_cycle(G, s);
        if(cyc.empty()) continue;
        S sum = 0;
        for(auto e : cyc) sum += e.cost;
        if(sum < cost){
            cost = sum;
            min_cyc = cyc;
        }
    }

    return min_cyc;
}
#endif
