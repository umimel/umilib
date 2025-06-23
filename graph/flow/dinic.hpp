#ifndef DINIC_HPP
#define DINIC_HPP

#include<bits/stdc++.h>
#include "../../header.hpp"
#include "template.hpp"
using namespace std;

/*start*/
template<typename T>
struct dinic{
    int n;
    residual_graph<T> graph;

    dinic(residual_graph<T> &graph_){
        n = (int)graph_.size();
        graph.resize(n);

        for(int from=0; from<n; from++){
            for(redge<T> e : graph_[from]){
                graph[from].push_back(redge<T>(e.to, e.cap, e.cost, (int)graph[e.to].size()));
                graph[e.to].push_back(redge<T>(from, 0, e.cost, (int)graph[from].size()-1));
            }
        }
    }

    T max_flow(int s, int t){
        residual_graph<T> rgraph(n);
        vector<int> level(n);
        vector<int> iter(n);
        for(int from=0; from<n; from++) for(redge<T> e : graph[from]) rgraph[from].push_back(e);

        function<void()> bfs = [&](){
            for(int v=0; v<n; v++) level[v]=-1;

            queue<int> Q;
            level[s] = 0;
            Q.push(s);
            while(!Q.empty()){
                int v = Q.front();
                Q.pop();

                for(redge<T> e : rgraph[v]){
                    if(e.cap > 0 && level[e.to] < 0){
                        level[e.to] = level[v] + 1;
                        Q.push(e.to);
                    }
                }
            }
        };

        function<T(int, T)> dfs = [&](int v, T f){
            if(v == t) return f;
            for(int &i=iter[v]; i<(int)rgraph[v].size(); i++){
                redge<T> &e = rgraph[v][i];
                if(e.cap > 0 && level[v] < level[e.to]){
                    T d = dfs(e.to, min(f, e.cap));
                    if(d > 0){
                        e.cap -= d;
                        rgraph[e.to][e.rev].cap += d;
                        return d;
                    }
                }
            }

            return (T)0;
        };

        T flow = 0;
        for(;;){
            bfs();
            if(level[t] < 0) return flow;
            for(int v=0; v<n; v++) iter[v] = 0;
            T f;
            while((f = dfs(s, LINF)) > 0) flow += f;
        }
    }
};
#endif
