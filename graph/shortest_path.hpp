#ifndef SHORTEST_PATH_HPP
#define SHORTEST_PATH_HPP

/*depend on*/
#include "shortest_path_tree.hpp"
#include "bfs.hpp"
#include "binary_bfs.hpp"
#include "constant_bfs.hpp"
#include "complement_bfs.hpp"
#include "bellman_ford.hpp"
#include "warshall_floyd.hpp"
#include "pered.hpp"
#include "malick_mittal_gupta.hpp"
#include "roditty_zwick.hpp"
#include "yen.hpp"

/*start*/
class shortest_path{
public:
    template<typename T>
    static vector<T> bfs(graph<T> &G, int s){
        return ::bfs(G, s);
    }

    template<typename T>
    static vector<T> binary_bfs(graph<T> &G, int s){
        return ::binary_bfs(G, s);
    }

    template<typename T>
    static vector<T> constant_bfs(graph<T> &G, int s, T W){
        return ::constant_bfs(G, s, W);
    }

    template<typename T>
    static vector<T> complement_bfs(graph<T> &G, int s){
        return ::complement_bfs(G, s);
    }
    
    template<typename T>
    static vector<T> bellman_ford(graph<T> &G, int s){
        return ::bellman_ford(G, s);
    }

    template<typename T>
    static vector<vector<T>> warshall_floyd(graph<T> &G){
        return ::warshall_floyd(G);
    }

    template<typename T>
    static vector<vector<pair<int, T>>> pered(graph<T> &G, int k){
        return ::pered(G, k);
    }

    template<typename T>
    static vector<T> malick_mittal_gupta(graph<T> &G, int s, int t){
        return ::malick_mittal_gupta(G, s, t);
    }

    template<typename T>
    static vector<T> roditty_zwick(graph<T> &G, int s, int t){
        return ::roditty_zwick(G, s, t);
    }

    template<typename T>
    static vector<T> yen(graph<T> &G, int s, int t, int k){
        return ::yen(G, s, t, k);
    }
};
#endif
