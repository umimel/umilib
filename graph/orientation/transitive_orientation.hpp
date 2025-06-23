#ifndef TRANSITIVE_HPP
#define TRANSITIVE_HPP

/*depends on*/
#include "../../header.hpp"
#include "../graph-template.hpp"

/*start*/
/**
 * Problem : Bipoler Orientation
 * Input : undirected graph G, source s and sink t
 * Return Value : 
 *      - direction ((u, v)->false, (v, u)->true) for each edge (u, v)
 *      - If there is not orientaion, return empty set
 * Time Complexity : O(m + n)
 * Space Complexity : O(m + n)
 * Constraint : 2<=n, 1<=m, s!=t
 */
template<typename S>
vector<bool> transitive_orientation(graph<S> &G){

    /* check the graph class of G */
    assert(!G.get_dir());

    /* define notation */
    int n = G.get_vnum(), m = G.get_enum();
    vector<bool> orinet(m, false), used(m, false);

    /* define variable for TRD-Test */
    vector<vector<int>> nout(n);
    vector<bool> v_check(n, false), nout_check(n, false);

    function<bool(edges<S>)> TRD_Test = [&](edges<S> es){
        vector<int> vs;
        for(auto e : es){
            n_out_list[e.from].push_back(e.to);
            if(!v_check[e.from]) vs.push_back(e.from), v_check[e.from]=true;
            if(!v_check[e.to]) vs.push_back(e.to), v_check[e.to]=true;
        }

        for(auto u : vs){
            for(auto v : nout[u]) nout_check[v] = true;
            bool ok = true;
            for(auto v : nout[u]){
                for(auto to : nout[v]){
                    if(!nout_check[to]){
                        ok = false;
                        break;
                    }
                }
                if(!ok) break;
            }
            for(auto v : nout[u]) nout_check[v] = true;
            if(!ok) return 
        }

        for(auto v : vs) v_check[v] = false, nout[v].clear();

        return true;
    };

    function<void(int)> gamma_rule = [&](){

    };

    edges<S> es = G.get_edges();
}
#endif
