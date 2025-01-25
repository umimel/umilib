#ifndef RAGE_EDGE_GRAPH_HPP
#define RAGE_EDGE_GRAPH_HPP

/*depend on*/
#include "../header.hpp"
#include "template.hpp"

/*start*/
template<typename T>
struct range_edge_graph{
    int n;
    weighted_graph<T> G;
    const T TINF = numeric_limits<T>::max()/2;

    range_edge_graph(int n) : n(n){
        G.resize(3*n);
        for(int i=1; i<n; i++){
            int cl = 2*i, cr = 2*i+1;
            add_edge(i, cl, 0);
            add_edge(i, cr, 0);
            add_edge(2*n+cl, 2*n+i, 0);
            add_edge(2*n+cr, 2*n+i, 0);
        }
    }

    void add_edge(int u, int v, T w){
        if(3*n <= u) u -= 2*n;
        G[u].push_back(edge<T>(v, w));
    }

    // from [l1, r1) to [l2, r2)
    void add_edge(int l1, int r1, int l2, int r2, T w){
        int idx = (int)G.size();
        for(l1+=n, r1+=n; l1<r1; l1/=2, r1/=2){
            if(l1 & 1) add_edge(2*n+l1, idx, 0), l1++;
            if(r1 & 1) --r1, add_edge(2*n+r1, idx, 0);
        }
        vector<edge<T>> node;
        for(l2+=n, r2+=n; l2<r2; l2/=2, r2/=2){
            if(l2 & 1) node.push_back(edge<T>(l2++, w));
            if(r2 & 1) node.push_back(edge<T>(--r2, w));
        }
        G.push_back(node);
    }

    vector<T> dijkstra(int s){
        s += n;
        vector<T> dist((int)G.size(), TINF);
        dist[s] = 0;
        priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> pq;
        pq.push({0, s});
        while(!pq.empty()){
            auto [tmp, v] = pq.top();
            pq.pop();
            if(dist[v] < tmp) continue;
            for(edge<T> e : G[v]){
                if(dist[v] + e.cost < dist[e.to]){
                    dist[e.to] = dist[v] + e.cost;
                    pq.push({dist[e.to], e.to});
                }
            }
        }
        vector<T> res(dist.begin()+n, dist.begin()+2*n);
        for(int i=0; i<n; i++)if(res[i]==TINF) res[i] = -1;

        return res;
    }
};
#endif