#ifndef DOMINATOR_TREE_HPP
#define DOMINATOR_TREE_HPP

/*depends on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
vector<int> dominator_tree(graph<int>& G, int s){
    assert(G.get_dir());

    int n = (int)G.size();
    graph<int> R(n, true);
    for(int v=0; v<n; v++) for(auto e : G[v]) R.add_edge(e.to, v);
    vector<int> ord(n, -1), rev, par(n, -1);

    auto dfs = [&](auto self, int v) -> void{
        ord[v] = (int)rev.size();
        rev.emplace_back(v);
        for(auto e : G[v]) if(ord[e.to] == -1){
            par[e.to] = v;
            self(self, e.to);
        }
    }; dfs(dfs, s);

    int N = (int)rev.size();
    vector<int> semi(N), idom(N, -1), uf(N), best(N);
    vector<vector<int>> bucket(N);
    for(int i=0; i<N; i++) semi[i] = uf[i] = best[i] = i;

    auto find = [&](auto self, int v) -> int{
        if(uf[v] == v) return v;
        int r = self(self, uf[v]);
        if(semi[best[uf[v]]] < semi[best[v]]) best[v] = best[uf[v]];
        return uf[v] = r;
    };

    auto eval = [&](int v) -> int{
        find(find, v);
        return best[v];
    };

    auto link = [&](int p, int v){
        uf[v] = p;
    };

    for(int i=N-1; i>=1; i--){
        int v = rev[i];

        // calculate semi-dominator
        for(auto e : R[v]){
            if(ord[e.to] == -1) continue;
            int a = ord[e.to];
            int x = eval(a);
            semi[i] = min(semi[i], semi[x]);
        }

        bucket[semi[i]].emplace_back(i);

        int p = ord[par[v]];
        link(p, i);

        // process vertices that semi-dominated by p
        for (int x : bucket[p]) {
            int y = eval(x);

            if (semi[y] == semi[x]) {
                idom[x] = p;
            } else {
                idom[x] = y;
            }
        }

        bucket[p].clear();
    }

    // immediate dominator を確定させる
    for (int i=1; i<N; i++) {
        if (idom[i] != semi[i]) {
            idom[i] = idom[idom[i]];
        }
    }

    vector<int> res(n, -1);
    res[s] = s;
    for(int i=1; i<N; i++) res[rev[i]] = rev[idom[i]];

    return res;
}
#endif
