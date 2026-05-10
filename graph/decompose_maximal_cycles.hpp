#ifndef DECOMPOSE_MAXIMAL_CYCLES_HPP
#define DECOMPOSE_MAXIMAL_CYCLES_HPP

/*depends on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename S>
vector<vector<int>> decompose_maximal_cycles(graph<S> &G){
    int n = G.size();
    vector<bool> visited(n, false);
    vector<vector<int>> cycles;
    vector<int> idx(n, -1);
    vector<int> vst;

    for(int r=0; r<n; r++){
        vst.push_back(r);
        visited[r] = true;
        while(!vst.empty()){
            int v = vst.back();
            if((int)G[v].size() <= ++idx[v]){
                visited[v] = false;
                vst.pop_back();
                continue;
            }
            auto e = G[v][idx[v]];
            if(visited[e.to]){
                vector<int> cyc;
                cyc.push_back(e.id);
                while(v != e.to){
                    vst.pop_back();
                    visited[v] = false;
                    v = vst.back();
                    cyc.push_back(G[v][idx[v]].id);
                }
                reverse(cyc.begin(), cyc.end());
                cycles.push_back(cyc);
            }else{
                visited[e.to] = true;
                vst.push_back(e.to);
            }
        }
        visited[r] = false;
    }

    return cycles;
}
#endif
