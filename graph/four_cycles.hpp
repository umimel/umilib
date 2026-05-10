#ifndef FOUR_CYCLES_HPP
#define FOUR_CYCLES_HPP

/*depends on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename S>
vector<array<edge<S>, 4>> enumerate_4cycles(graph<S> &G){
    int n = G.size();
    assert(!G.get_dir());

    vector<vector<int>> eid(n, vector<int>(n, -1));
    for(auto e : G.get_edge_set()){
        eid[e.from][e.to] = e.id;
        eid[e.to][e.from] = e.id;
    }

    vector<array<edge<S>, 4>> ans;
    for(int a=0; a<n; a++){
        for(auto e1 : G[a]){
            int b = e1.to;
            if(b == a) continue;
            for(auto e2 : G[b]){
                int c = e2.to;
                if(c == a || c == b) continue;
                for(auto e3 : G[c]){
                    int d = e3.to;
                    if(d == a || d == b || d == c) continue;
                    if(eid[d][a] == -1) continue;
                    if(a > b || a > c || a > d) continue;
                    if(b > d) continue;

                    auto e4 = G.get_edge(eid[d][a]);
                    if(e4.from != d) e4.reverse();
                    ans.push_back({e1, e2, e3, e4});
                }
            }
        }
    }

    return ans;
}

template<typename S>
long long count_4cycles(graph<S> &G){
    return (long long)enumerate_4cycles(G).size();
}

template<typename S>
array<edge<S>, 4> find_4cycle(graph<S> &G){
    int n = G.size();
    assert(!G.get_dir());

    vector<vector<int>> eid(n, vector<int>(n, -1));
    for(auto e : G.get_edge_set()){
        eid[e.from][e.to] = e.id;
        eid[e.to][e.from] = e.id;
    }

    for(int a=0; a<n; a++){
        for(auto e1 : G[a]){
            int b = e1.to;
            if(b == a) continue;
            for(auto e2 : G[b]){
                int c = e2.to;
                if(c == a || c == b) continue;
                for(auto e3 : G[c]){
                    int d = e3.to;
                    if(d == a || d == b || d == c) continue;
                    if(eid[d][a] == -1) continue;
                    if(a > b || a > c || a > d) continue;
                    if(b > d) continue;

                    auto e4 = G.get_edge(eid[d][a]);
                    if(e4.from != d) e4.reverse();
                    return {e1, e2, e3, e4};
                }
            }
        }
    }

    return {};
}

template<typename S>
array<edge<S>, 4> find_min_4cycle(graph<S> &G){
    auto cycles = enumerate_4cycles(G);
    if(cycles.empty()) return {};

    auto cost = [](const array<edge<S>, 4> &cyc){
        return cyc[0].cost + cyc[1].cost + cyc[2].cost + cyc[3].cost;
    };

    auto ans = cycles[0];
    for(auto cyc : cycles) if(cost(cyc) < cost(ans)) ans = cyc;
    return ans;
}

template<typename S>
array<edge<S>, 4> find_max_4cycle(graph<S> &G){
    auto cycles = enumerate_4cycles(G);
    if(cycles.empty()) return {};

    auto cost = [](const array<edge<S>, 4> &cyc){
        return cyc[0].cost + cyc[1].cost + cyc[2].cost + cyc[3].cost;
    };

    auto ans = cycles[0];
    for(auto cyc : cycles) if(cost(ans) < cost(cyc)) ans = cyc;
    return ans;
}
#endif
