#ifndef CENTRALITY_HPP
#define CENTRALITY_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"
#include "shortest_path.hpp"

/*start*/
template<typename T>
vector<T> eccentricity(graph<T> &G){
    int n = G.size();
    const T TINF = numeric_limits<T>::max()/3;
    auto dist = warshall_floyd(G);
    vector<T> ecc(n, 0);

    for(int v=0; v<n; v++){
        for(int u=0; u<n; u++){
            if(dist[v][u] >= TINF) ecc[v] = TINF;
            else if(ecc[v] < dist[v][u]) ecc[v] = dist[v][u];
        }
    }

    return ecc;
}

template<typename T>
T radius(graph<T> &G){
    auto ecc = eccentricity(G);
    return *min_element(ecc.begin(), ecc.end());
}

template<typename T>
int center(graph<T> &G){
    auto ecc = eccentricity(G);
    return (int)(min_element(ecc.begin(), ecc.end()) - ecc.begin());
}

template<typename T>
vector<int> centers(graph<T> &G){
    auto ecc = eccentricity(G);
    T rad = *min_element(ecc.begin(), ecc.end());
    vector<int> ans;

    for(int v=0; v<(int)ecc.size(); v++){
        if(ecc[v] == rad) ans.push_back(v);
    }

    return ans;
}

template<typename T>
T diameter(graph<T> &G){
    int n = G.size();
    const T TINF = numeric_limits<T>::max()/3;
    auto dist = warshall_floyd(G);
    T diam = 0;

    for(int s=0; s<n; s++){
        for(int t=0; t<n; t++){
            if(dist[s][t] >= TINF) return TINF;
            diam = max(diam, dist[s][t]);
        }
    }

    return diam;
}

template<typename T>
vector<T> farness(graph<T> &G){
    int n = G.size();
    const T TINF = numeric_limits<T>::max()/3;
    auto dist = warshall_floyd(G);
    vector<T> sum(n, 0);

    for(int v=0; v<n; v++){
        for(int u=0; u<n; u++){
            if(dist[v][u] >= TINF){
                sum[v] = TINF;
                break;
            }
            sum[v] += dist[v][u];
        }
    }

    return sum;
}

template<typename T>
T median(graph<T> &G){
    auto sum = farness(G);
    return *min_element(sum.begin(), sum.end());
}

template<typename T>
int median_vertex(graph<T> &G){
    auto sum = farness(G);
    return (int)(min_element(sum.begin(), sum.end()) - sum.begin());
}

template<typename T>
vector<int> median_vertices(graph<T> &G){
    auto sum = farness(G);
    T med = *min_element(sum.begin(), sum.end());
    vector<int> ans;

    for(int v=0; v<(int)sum.size(); v++){
        if(sum[v] == med) ans.push_back(v);
    }

    return ans;
}

template<typename T>
bool positive_betweenness_centrality(graph<T> &G, int x){
    int n = G.size();
    const T TINF = numeric_limits<T>::max()/3;
    auto dist = warshall_floyd(G);

    for(int s=0; s<n; s++){
        if(s == x) continue;
        for(int t=0; t<n; t++){
            if(t == x || s == t) continue;
            if(dist[s][x] >= TINF || dist[x][t] >= TINF || dist[s][t] >= TINF) continue;
            if(dist[s][x] + dist[x][t] == dist[s][t]) return true;
        }
    }

    return false;
}

template<typename T>
vector<long double> betweenness_centrality(graph<T> &G){
    int n = G.size();
    const T TINF = numeric_limits<T>::max()/3;
    vector<long double> bc(n, 0);

    for(auto e : G.get_edge_set()) assert(T(0) <= e.cost);

    for(int s=0; s<n; s++){
        vector<vector<int>> pred(n);
        vector<long double> sigma(n, 0), delta(n, 0);
        vector<T> dist(n, TINF);
        vector<int> order;
        priority_queue<pair<T, int>, vector<pair<T, int>>, greater<>> que;

        sigma[s] = 1;
        dist[s] = 0;
        que.push({0, s});

        while(!que.empty()){
            auto [d, v] = que.top();
            que.pop();
            if(dist[v] < d) continue;
            order.push_back(v);

            for(auto e : G[v]){
                T nd = dist[v] + e.cost;
                if(nd < dist[e.to]){
                    dist[e.to] = nd;
                    sigma[e.to] = sigma[v];
                    pred[e.to].clear();
                    pred[e.to].push_back(v);
                    que.push({nd, e.to});
                }else if(nd == dist[e.to]){
                    sigma[e.to] += sigma[v];
                    pred[e.to].push_back(v);
                }
            }
        }

        reverse(order.begin(), order.end());
        for(int w : order){
            if(sigma[w] == 0) continue;
            for(int v : pred[w]){
                delta[v] += sigma[v]/sigma[w]*(1 + delta[w]);
            }
            if(w != s) bc[w] += delta[w];
        }
    }

    return bc;
}
#endif
