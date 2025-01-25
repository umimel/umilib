#ifndef GRAPHTEMPLATE_HPP
#define GRAPHTEMPLATE_HPP

/*depends on*/
#include<bits/stdc++.h>
using namespace std;

/*start*/
template<typename T> 
struct edge{
    int from;
    int to;
    T cost;
    int id;

    edge(){}
    edge(int to, T cost=1) : from(-1), to(to), cost(cost){}
    edge(int from, int to, T cost) : from(from), to(to), cost(cost) {}
    edge(int from, int to, T cost, int id) : from(from), to(to), cost(cost), id(id){}

    void reverse(){swap(from, to);}
};

template<typename T>
struct edges : std::vector<edge<T>>{
    void sort(){
        std::sort(
            (*this).begin(),
            (*this).end(), 
            [](const edge<T>& a, const edge<T>& b){
                return a.cost < b.cost;
            }
        );
    }
};

template<typename T = bool>
struct graph : std::vector<edges<T>>{
private:
    int n = 0;
    int m = 0;
    edges<T> es;
    bool dir;

public:
    graph(int n, bool dir) : n(n), dir(dir){
        (*this).resize(n);
    }

    void add_edge(int from, int to, T cost=1){
        if(dir){
            es.push_back(edge<T>(from, to, cost, m));
            (*this)[from].push_back(edge<T>(from, to, cost, m++));
        }else{
            if(from > to) swap(from, to);
            es.push_back(edge<T>(from, to, cost, m));
            (*this)[from].push_back(edge<T>(from, to, cost, m));
            (*this)[to].push_back(edge<T>(to, from, cost, m++));
        }
    }

    int get_vnum(){
        return n;
    }

    int get_enum(){
        return m;
    }

    bool get_dir(){
        return dir;
    }

    edge<T> get_edge(int i){
        return es[i];
    }

    edges<T> get_edge_set(){
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

template<typename T> using Edges = vector<edge<T>>;
template<typename T> using weighted_graph = vector<Edges<T>>;
template<typename T> using tree = vector<Edges<T>>;
using unweighted_graph = vector<vector<int>>;
template<typename T> using residual_graph = vector<vector<redge<T>>>;
#endif