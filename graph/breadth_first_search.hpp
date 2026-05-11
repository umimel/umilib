#ifndef BREADTH_FIRST_SEARCH_HPP
#define BREADTH_FIRST_SEARCH_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename T>
vector<T> bfs(graph<T> &G, int s){
    int n = G.size();
    vector<T> dist(n, -1);

    dist[s] = 0;
    queue<int> que;
    que.push(s);

    while(!que.empty()){
        int v = que.front();
        que.pop();
        for(auto e : G[v]) if(dist[e.to] == -1){
            dist[e.to] = dist[v] + 1;
            que.push(e.to);
        }
    }

    return dist;
}

template<typename T>
vector<T> binary_bfs(graph<T> &G, int s){
    int n = G.size();
    vector<T> dist(n, -1);

    dist[s] = 0;
    deque<int> deq;
    deq.push_front(s);

    while(!deq.empty()){
        int v = deq.front();
        deq.pop_front();
        for(auto e : G[v]) if(dist[e.to] == -1){
            dist[e.to] = dist[v] + e.cost;
            if(e.cost) deq.push_back(e.to);
            else deq.push_front(e.to);
        }
    }

    return dist;
}

template<typename T>
vector<T> constant_bfs(graph<T> &G, int s, T W){
    int n = G.size();
    vector<T> dist(n, -1);
    vector<vector<int>> cand(n*W+1);

    dist[s] = 0;
    cand[0].push_back(s);

    for(int d=0; d<=n*W; d++){
        for(int i=0; i<(int)cand[d].size(); i++){
            int v = cand[d][i];
            if(dist[v] < d) continue;
            for(auto e : G[v]){
                if(dist[e.to] == -1 || dist[v] + e.cost < dist[e.to]){
                    dist[e.to] = dist[v] + e.cost;
                    cand[dist[e.to]].push_back(e.to);
                }
            }
        }
    }

    return dist;
}

template<typename T>
vector<T> complement_bfs(graph<T> &G, int s){
    int n = G.size();
    map<pair<int, int>, bool> mp;
    for(int v=0; v<n; v++){
        for(auto e : G[v]) mp[{v, e.to}] = true;
    }

    vector<T> dist(n, -1);
    vector<int> unvisited;
    for(int v=0; v<n; v++) if(v != s) unvisited.push_back(v);

    queue<int> visited;
    visited.push(s);
    dist[s] = 0;

    while(!visited.empty()){
        int v = visited.front();
        visited.pop();

        vector<int> nxt;
        for(int to : unvisited){
            if(!mp[{v, to}]){
                visited.push(to);
                dist[to] = dist[v] + 1;
            }else{
                nxt.push_back(to);
            }
        }

        unvisited = nxt;
    }

    return dist;
}

#endif
