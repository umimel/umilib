#ifndef MINCOSTFLOW_HPP
#define MINCOSTFLOW_HPP

/*depend on*/
#include "../../header.hpp"
#include "../graph-template.hpp"

/*start*/
template<typename T>
struct mincostflow{
    residual_graph<T> G;
    const T TINF = std::numeric_limits<T>::max() / 2;
    int n;

    mincostflow(residual_graph<T> &G_){
        n = (int)G_.size();
        G.resize(n);

        for(int from=0; from<n; from++){
            for(redge<T> e : G_[from]){
                G[from].push_back(redge<T>(e.to, e.cap, e.cost, (int)G[e.to].size()));
                G[e.to].pb(redge<T>(from, 0, -e.cost, (int)G[from].size()-1));
            }
        }
    }

    T flow(int s, int t, T f){
        residual_graph<T> H(n);
        vector<T> h(n, 0); //ポテンシャル
        vector<T> dist(n, 0); //最短距離
        vector<int> prevv(n, 0); // 直前の頂点
        vector<int> preve(n, 0); // 直前の辺

        for(int from=0; from<n; from++){
            for(redge<T> e : G[from]){
                H[from].push_back(e);
            }
        }

        T res = 0;
        while(f > 0){
            //ダイクストラ法を用いてhを更新
            priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> PQ;
            for(int i=0; i<n; i++) dist[i] = TINF;
            dist[s] = 0;
            PQ.push({0, s});
            while(!PQ.empty()){
                pair<T, int> p = PQ.top();
                PQ.pop();
                int v = p.se;
                if(dist[v] < p.fi) continue;

                for(int i=0; i<(int)H[v].size(); i++){
                    redge<T> &e = H[v][i];
                    if(e.cap > 0 && dist[e.to] > dist[v] + e.cost + h[v] - h[e.to]){
                        dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
                        prevv[e.to] = v;
                        preve[e.to] = i;
                        PQ.push({dist[e.to], e.to});
                    }
                }
            }
            if(dist[t] == TINF){
                //これ以上流せない
                return -1;
            }
            for(int v=0; v<n; v++) h[v] += dist[v];

            // s-t間最短経路に沿って目一杯流す
            T d = f;
            for(int v=t; v!=s; v=prevv[v]){
                d = min(d, H[prevv[v]][preve[v]].cap);
            }
            f -= d;
            res += d*h[t];
            for(int v=t; v!=s; v=prevv[v]){
                redge<T> &e = H[prevv[v]][preve[v]];
                e.cap -= d;
                H[v][e.rev].cap += d;
            }
        }

        return res;
    }
};
#endif