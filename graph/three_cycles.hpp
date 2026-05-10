#ifndef THREE_CYCLES_HPP
#define THREE_CYCLES_HPP

/*depends on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename S>
long long count_3cycles(graph<S> &G){
    int n = G.size();
    int m = G.edge_size();
    assert(!G.get_dir());

    auto comp = [&](int i, int j){
        if(G[i].size() != G[j].size()) return G[i].size() < G[j].size();
        return i < j;
    };

    graph<S> H(n, true);
    for(int i=0; i<m; i++){
        auto e = G.get_edge(i);
        if(comp(e.from, e.to)) H.add_edge(e.from, e.to, e.cost);
        else H.add_edge(e.to, e.from, e.cost);
    }
    
    long long cnt = 0;
    vector<int> used(n, -1);
    for(int v=0; v<n; v++){
        for(auto e3 : H[v]) used[e3.to] = e3.id;
        for(auto e1 : H[v]) for(auto e2 : H[e1.to]){
            if(used[e2.to] != -1) cnt++;
        }
        for(auto e3 : H[v]) used[e3.to] = -1;
    }

    return cnt;
}

template<typename S>
vector<array<edge<S>, 3>> enumerate_3cycles(graph<S> &G){
    int n = G.size();
    int m = G.edge_size();
    assert(!G.get_dir());

    auto comp = [&](int i, int j){
        if(G[i].size() != G[j].size()) return G[i].size() < G[j].size();
        return i < j;
    };

    graph<S> H(n, true);
    for(int i=0; i<m; i++){
        auto e = G.get_edge(i);
        if(comp(e.from, e.to)) H.add_edge(e.from, e.to, e.cost);
        else H.add_edge(e.to, e.from, e.cost);
    }
    
    vector<array<edge<S>, 3>> ans;
    vector<int> used(n, -1);
    for(int v=0; v<n; v++){
        for(auto e3 : H[v]) used[e3.to] = e3.id;
        for(auto e1 : H[v]) for(auto e2 : H[e1.to]){
            if(used[e2.to] != -1){
                auto e3 = H.get_edge(used[e2.to]);
                e3.reverse();
                ans.push_back({e1, e2, e3});
            }
        }
        for(auto e3 : H[v]) used[e3.to] = -1;
    }

    return ans;
}

template<typename S>
array<edge<S>, 3> find_3cycle(graph<S> &G){
    int n = G.size();
    int m = G.edge_size();
    assert(!G.get_dir());

    auto comp = [&](int i, int j){
        if(G[i].size() != G[j].size()) return G[i].size() < G[j].size();
        return i < j;
    };

    graph<S> H(n, true);
    for(int i=0; i<m; i++){
        auto e = G.get_edge(i);
        if(comp(e.from, e.to)) H.add_edge(e.from, e.to, e.cost);
        else H.add_edge(e.to, e.from, e.cost);
    }
    
    vector<int> used(n, -1);
    for(int v=0; v<n; v++){
        for(auto e3 : H[v]) used[e3.to] = e3.id;
        for(auto e1 : H[v]) for(auto e2 : H[e1.to]){
            if(used[e2.to] != -1){
                auto e3 = H.get_edge(used[e2.to]);
                e3.reverse();
                return {e1, e2, e3};
            }
        }
        for(auto e3 : H[v]) used[e3.to] = -1;
    }

    return {};
}

template<typename S>
array<edge<S>, 3> find_min_3cycle(graph<S> &G){
    auto cycles = enumerate_3cycles(G);
    if(cycles.empty()) return {};

    auto cost = [](const array<edge<S>, 3> &cyc){
        return cyc[0].cost + cyc[1].cost + cyc[2].cost;
    };

    auto ans = cycles[0];
    for(auto cyc : cycles) if(cost(cyc) < cost(ans)) ans = cyc;
    return ans;
}

template<typename S>
array<edge<S>, 3> find_max_3cycle(graph<S> &G){
    auto cycles = enumerate_3cycles(G);
    if(cycles.empty()) return {};

    auto cost = [](const array<edge<S>, 3> &cyc){
        return cyc[0].cost + cyc[1].cost + cyc[2].cost;
    };

    auto ans = cycles[0];
    for(auto cyc : cycles) if(cost(ans) < cost(cyc)) ans = cyc;
    return ans;
}
#endif
