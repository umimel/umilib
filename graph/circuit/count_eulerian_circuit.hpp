#ifndef COUNT_EULEAN_CIRCUIT_HPP
#define COUNT_EULEAN_CIRCUIT_HPP

/*depend on*/
#include "../header.hpp"
#include "../matrix/matrix.hpp"
#include "../datastructure/union_find.hpp"
#include "../graph-template.hpp"
#include "count_directed_spanning_tree.hpp"
#include "../math/combination.hpp"

/*start*/
template<typename T>
T count_eulerian_circuit(weighted_graph<int> &G){
    // n is the number of vertices in weighted graph G
    int n = (int)G.size();

    // incoming degree:=ind, outgoing degree:=outd
    vector<int> ind(n, 0), outd(n, 0);
    for(int v=0; v<n; v++)for(edge<int> e : G[v]){
        ind[e.to]+=e.cost;
        outd[v]+=e.cost;
    }
    
    // there does not exist eulrian circuit if there exist some vertex v s.t. ind[v]!=outd[v]
    for(int v=0; v<n; v++) if(ind[v]!=outd[v]) return (T)0;

    // iv : non isolated vertices, iv_id[v] : label of vertex v
    vector<int> iv, iv_id(n, -1);
    for(int v=0; v<n; v++) if(ind[v] > 0){
        iv_id[v] = (int)iv.size();
        iv.push_back(v);
    }
    int n2 = (int)iv.size();

    if(n2 == 0) return (T)(1);

    // make connected graph to r
    weighted_graph<T> G2(n2);
    for(int v=0; v<n; v++) if(iv_id[v]!=-1){
        for(edge<int> e : G[v]) if(iv_id[e.to]!=-1){
            G2[iv_id[v]].push_back(edge<T>(iv_id[e.to], e.cost));
        }
    }

    // judge connectity
    union_find uf(n2);
    for(int v=0; v<n2; v++) for(edge<T> e : G2[v]) uf.unite(v, e.to);
    if(uf.size(0)!=n2) return T(0);

    // count directed spanning tree of G2
    T cnt = count_directed_spanning_tree(G2, 0);

    combination<T> comb;
    for(int v : iv) cnt *= comb.factorial((int)ind[v]-1);

    return cnt;
}
#endif
