#ifndef MINCOSTCYCLE_HPP
#define MINCOSTCYCLE_HPP

/*depends on*/
#include "../../header.hpp"
#include "../graph-template.hpp"

/*start*/
template<typename T>
tuple<T, vector<int>, vector<int>> mincostcycle(graph<T> g, int s){
    struct dat{
        T dist;
        int pv;
        int pe;
    };

    int n = g.n;
    const T TINF = numeric_limits<T>::max()/2;
    bool is_directed = g.directed;

    // return shortest path tree vec, vec[v] = {dist(r, v), parent of r, edge id of (parent of r, r)};
    function<vector<dat>()> dijkstra = [&](){
        vector<dat> vec(n, {TINF, -1, -1});
        vec[s].dist = 0;
        priority_queue<pair<T, int>, vector<pair<T, int>>, greater<>> pq;
        pq.push({0, s});
        while(!pq.empty()){
            auto [tmp, v] = pq.top();
            pq.pop();
            if(vec[v].dist < tmp) continue;

            for(edge<T> e : g[v]){
                if(vec[v].dist+e.cost < vec[e.to].dist){
                    vec[e.to].dist = vec[v].dist + e.cost;
                    vec[e.to].pv = v;
                    vec[e.to].pe = e.id;
                    pq.push({vec[e.to].dist, e.to});
                }
            }
        }

        return vec;
    };

    // g is a directed graph
    if(is_directed){
        vector<dat> vec = dijkstra();
        T cost = TINF;
        int vlast = -1;
        int elast = -1;
        for(int v=0; v<n; v++) for(edge<T> e : g[v]) if(e.to == s){
            if(vec[v].dist + e.cost < cost){
                cost = vec[v].dist + e.cost;
                vlast = v;
                elast = e.id;
            }
        }

        if(cost == TINF) return {T(-1), {}, {}};

        vector<int> vcycle;
        vector<int> ecycle;
        ecycle.push_back(elast);
        while(vlast != -1){
            vcycle.push_back(vlast);
            ecycle.push_back(vec[vlast].pe);
            vlast = vec[vlast].pv;
        }
        ecycle.pop_back();
        reverse(all(vcycle));
        reverse(all(ecycle));

        return {cost, vcycle, ecycle};
    }else{ // g is an undirected graph
        vector<dat> vec = dijkstra();
        graph<bool> spt(n, false);
        for(int v=0; v<n; v++) if(v!=s && vec[v].dist != TINF){
            spt.add_edge(v, vec[v].pv);
        }

        vector<int> label(n, -1);
        label[s] = s;
        function<void(int, int, int)> dfs = [&](int v, int p, int l){
            label[v] = l;
            for(edge<bool> e : spt[v]) if(e.to != p) dfs(e.to, v, l);
        };

        for(edge<bool> e : spt[s]) dfs(e.to, s, e.to);

        T cost = TINF;
        int x = -1, y = -1, min_e = -1;
        for(int v=0; v<n; v++) if(v != s) for(edge<T> e : g[v]){
            if(vec[v].pv != e.to && label[v] != label[e.to]){
                if(vec[v].dist + vec[e.to].dist + e.cost < cost){
                    cost = vec[v].dist + vec[e.to].dist + e.cost;
                    x = v;
                    y = e.to;
                    min_e = e.id;
                }
            }
        }

        if(cost == TINF) return {T(-1), {}, {}};

        vector<int> vcycle, ecycle;
        ecycle.push_back(min_e);
        while(x != -1){
            vcycle.push_back(x);
            ecycle.push_back(vec[x].pe);
            x = vec[x].pv;
        }
        ecycle.pop_back();
        reverse(all(vcycle));
        reverse(all(ecycle));

        while(y != s){
            vcycle.push_back(y);
            ecycle.push_back(vec[y].pe);
            y = vec[y].pv;
        }

        return {cost, vcycle, ecycle};
    }
}

template<typename T>
tuple<T, vector<int>, vector<int>> mincostcycle(graph<T> g){
    int n = g.n;
    const T TINF = numeric_limits<T>::max()/2;

    T cost = TINF;
    vector<int> vcycle;
    vector<int> ecycle;

    for(int r=0; r<n; r++){
        auto [r_cost, r_vcycle, r_ecycle] = mincostcycle<T>(g, r);
        if(r_cost != -1 && r_cost < cost){
            cost = r_cost;
            vcycle = r_vcycle;
            ecycle = r_ecycle;
        }
    }

    if(cost == TINF) return {T(-1), {}, {}};
    else return {cost, vcycle, ecycle};
}
#endif