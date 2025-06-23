#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

/*depend on*/
#include "../../header.hpp"
#include "../template.hpp"

/*start*/
template<typename T>
struct dijkstra{
private:
    const T TINF = numeric_limits<T>::max()/3;
    int n, s;
    graph<T> G;
    vector<T> dist;
    vector<int> vpar;
    edges<T> epar;

public:
    dijkstra(const graph<T>& G, int s) : G(G), s(s){
        // initilization
        n = G.get_vnum();
        dist.resize(n, TINF);
        vpar.resize(n, -1);
        epar.resize(n, -1);

        // running Dijkstra algorithm
        run();
    }

    void run(){
        dist[s] = 0;
        priority_queue<pair<T, int>, vector<pair<T, int>>, greater<>> que;
        que.push({0, s});
        while(!que.empty()){
            auto [d, v] = que.top();
            que.pop();
            if(dist[v] < d) continue;

            for(const auto& e : G[v]){
                if(dist[v] + e.cost < dist[e.to]){
                    dist[e.to] = dist[v] + e.cost;
                    vpar[e.to] = v;
                    epar[e.to] = e;
                    que.push({dist[e.to], e.to});
                }
            }
        }
    }

    T get_dist(int t){
        return dist[t];
    }

    vector<T> get_dist(){
        return dist;
    }

    vector<int> get_vpar(){
        return vpar;
    }

    int get_vpar(int v){
        return vpar[v];
    }

    edges<T> get_epar(){
        return epar;
    }

    edge<T> get_epar(int v){
        return epar[v];
    }

    vector<int> get_vpath(int t){
        vector<int> vpath;
        int cur = t;
        while(cur != -1){
            vpath.push_back(cur);
            cur = vpar[cur];
        }
        reverse(vpath.begin(), vpath.end());

        return vpath;
    }

    edges<T> get_epath(int t){
        edges<T> epath;
        int cur = t;
        while(cur != s){
            epath.push_back(epar[cur]);
            cur = vpar[cur];
        }
        reverse(epath.begin(), epath.end());

        return epath;
    }

    graph<T> get_shotest_path_tree(){
        graph<T> spt(n, false);
        for(int v=0; v<n; v++) if(v != s){
            int p = vpar[v];
            auto e = G.get_edge(epar[v]);
            spt.add_edge(vpar[v], v, e.cost);
        }

        return spt;
    }
};
#endif
