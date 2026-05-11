#ifndef GRAPH_HPP
#define GRAPH_HPP

/*depends on*/
#include<bits/stdc++.h>
using namespace std;

/*start*/
template<typename T> 
struct edge{
    int from = -1;
    int to = -1;
    T cost = T(1);
    int id = -1;

    edge() = default;
    edge(int to, T cost=T(1)) : to(to), cost(cost){}
    edge(int from, int to, T cost, int id=-1) : from(from), to(to), cost(cost), id(id){}

    void reverse(){swap(from, to);}
    edge rev() const { return edge(to, from, cost, id); }
};

template<typename T>
struct edges : std::vector<edge<T>>{
    using std::vector<edge<T>>::vector;

    void sort_by_cost(){
        std::sort(
            (*this).begin(),
            (*this).end(), 
            [](const edge<T>& a, const edge<T>& b){
                return a.cost < b.cost;
            }
        );
    }

    void rsort_by_cost(){
        std::sort(
            (*this).begin(),
            (*this).end(), 
            [](const edge<T>& a, const edge<T>& b){
                return a.cost > b.cost;
            }
        );
    }

    void sort(){
        sort_by_cost();
    }
};

template<typename T = bool>
struct graph{
private:
    int n = 0;
    int m = 0;
    vector<edges<T>> adj;
    edges<T> es;
    bool dir = false;

public:
    graph() = default;
    graph(int n, bool dir=false) : n(n), adj(n), dir(dir){}

    int add_vertex(){
        adj.push_back(edges<T>());
        return n++;
    }

    int add_edge(int from, int to, T cost=T(1)){
        int id = m++;
        es.push_back(edge<T>(from, to, cost, id));
        if(dir){
            adj[from].push_back(edge<T>(from, to, cost, id));
        }else{
            adj[from].push_back(edge<T>(from, to, cost, id));
            adj[to].push_back(edge<T>(to, from, cost, id));
        }
        return id;
    }

    int size() const{
        return n;
    }

    bool empty() const{
        return n == 0;
    }

    edges<T>& operator[](int v){
        return adj[v];
    }

    const edges<T>& operator[](int v) const{
        return adj[v];
    }

    auto begin(){ return adj.begin(); }
    auto end(){ return adj.end(); }
    auto begin() const{ return adj.begin(); }
    auto end() const{ return adj.end(); }

    int edge_size() const{
        return m;
    }

    bool get_dir() const{
        return dir;
    }

    edge<T> get_edge(int i) const{
        return es[i];
    }

    const edges<T>& get_edge_set() const{
        return es;
    }

    const edges<T>& get_edges() const{
        return es;
    }
};

template<typename T>
struct redge{
    int from, to;
    T cap, cost;
    int rev;
    
    redge(int to, T cap, T cost=(T)(1)) : from(-1), to(to), cap(cap), cost(cost){}
    redge(int to, T cap, T cost, int rev) : from(-1), to(to), cap(cap), cost(cost), rev(rev){}
};

template<typename T> using tree = vector<edges<T>>;
template<typename T> using residual_graph = vector<vector<redge<T>>>;
#endif
