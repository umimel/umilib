#ifndef MALICK_MITTAL_GUPTA_HPP
#define MALICK_MITTAL_GUPTA_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"
#include "shortest_path_tree.hpp"

/*start*/
template<typename T>
vector<T> malick_mittal_gupta(graph<T> &G, int s, int t){
    const T TINF = numeric_limits<T>::max()/3;
    shortest_path_tree<T> dijk_s(G, s), dijk_t(G, t);
    int n = G.size();
    int m = G.edge_size();
    vector<T> dist_s = dijk_s.get_dist();
    vector<T> dist_t = dijk_t.get_dist();
    vector<int> path = dijk_s.get_vpath(t);
    int p = (int)path.size();
    path.push_back(n);
    vector<vector<int>> ch(n);
    for(int v=0; v<n; v++) if(dijk_s.get_vpar(v) != -1) ch[dijk_s.get_vpar(v)].push_back(v);
    vector<int> label(n, -1);

    function<void(int, int)> labeling = [&](int v, int l){
        label[v] = l;
        for(int to : ch[v]) labeling(to, l);
    };
    
    for(int i=0; i<p; i++){
        label[path[i]] = i;
        for(int to : ch[path[i]]) if(to != path[i+1]){
            labeling(to, i);
        }
    }

    vector<bool> used(m, false);
    for(int i=1; i<p; i++) used[dijk_s.get_epar(path[i]).id] = true;

    vector<vector<int>> sevt(p), eevt(p);
    for(int v=0; v<n; v++) for(auto e : G[v]) if(!used[e.id] && label[v] < label[e.to]){
        sevt[label[v]].push_back(e.id);
        eevt[label[e.to]].push_back(e.id);
    }
    
    vector<T> ans(m, dijk_s.get_dist(t));
    set<pair<T, int>> eset;
    for(int i=1; i<p; i++){
        auto v = path[i];
        auto f = dijk_s.get_epar(v);
        ans[f.id] = TINF;
    
        for(int id : sevt[i-1]){
            auto e = G.get_edge(id);
            int x = e.from, y = e.to;
            if(label[x] > label[y]) swap(x, y);
            eset.insert({dist_s[x]+e.cost+dist_t[y], id});
        }

        if(!eset.empty()) ans[f.id] = min(ans[f.id], (*eset.begin()).first);

        for(int id : eevt[i]){
            auto e = G.get_edge(id);
            int x = e.from, y = e.to;
            if(label[x] > label[y]) swap(x, y);
            eset.erase({dist_s[x]+e.cost+dist_t[y], id});
        }
    }

    return ans;
}
#endif
