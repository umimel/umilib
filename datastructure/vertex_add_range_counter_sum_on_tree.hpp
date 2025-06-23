#ifndef AAAA_HPP
#define AAAA_HPP

/*depends on*/
#include "../header.hpp"
#include "fenwick_tree.hpp"
#include "../graph/graph-template.hpp"
#include "../tree/lowest_common_ancestor.hpp"

/*start*/
template<typename W>
struct vertex_add_range_counter_sum_on_tree{
    int n;
    vector<fenwick_tree<W>> dsum;
    lowest_common_ancestor<W> lca;
    vector<vector<int>> dep;
    vector<W> dat;
    vector<int> siz;
    vector<bool> centroid;
    vector<int> par;

    vertex_add_range_counter_sum_on_tree(tree<W> &T){
        // initialize variable
        n = (int)T.size();
        dsum.resize(n);
        dat.resize(n, 0);
        siz(n, 0);
        centroid(n, false);
        par.resize(n, -1);
        lca = lowest_common_ancestor<W>(T, 0);

        // build data structure
        centroid_dec(T, 0, -1);
    }

    W get_dist(int u, int v){
        return lca.depth[u] + lca.depth[v] - 2*lca.depth[lca.query(u, v)];
    }

    int compute_subtree_size(tree<W> &T, int v, int p){

        int c = 1;

        for(edge<int> e : T[v]){
            if(e.to==p || centroid[e.to]) continue;
            c += compute_subtree_size(T, e.to, v);
        }

        siz[v] = c;

        return c;
    }

    pair<int, int> search_centroid(tree<W> &T, int v, int p, int t){

        pair<int, int> res = {IINF, -1};
        int s = 1, m = 0;

        for(edge<int> e : T[v]){
            if(e.to==p || centroid[e.to]) continue;
            res = min(res, search_centroid(T, e.to, v, t));
            m = max(m, siz[e.to]);
            s += siz[e.to];
        }

        m = max(m, t-s);
        res = min(res, {m, v});

        return res;
    }

    // return max depth & store d to dep[v]
    int compute_dep(tree<W> &T, int v, int p, int d){
        dep[v].push_back(d);

        int res = d;
        for(edge<int> e : T[v]){
            if(e.to==p || centroid[e.to]) continue;
            res = max(res, compute_dist(T, e.to, v, d+1));
        }

        return res;
    }

    void centroid_dec(tree<W> &T, int v, int r){
        // search centroid s
        compute_subtree_size(T, v, -1);
        int s = search_centroid(T, v, -1, subtree_size[v]).second;
        centroid[s] = true;

        // set par[s]=r
        par[s] = r;

        // recusion phase
        for(edge<int> e : T[s]){
            if(centroid[e.to]) continue;
            centroid_dec(T, e.to, s);
        }
        
        // compute depth rooted as s in recusion
        int max_dep = 0;
        dep[s].pb(0);
        for(edge<W> e : T[s]) if(!centroid[e.to]){
            max_dep = max(max_dep, compute_dep(T, e.to, s, 1));
        }

        dsum[s] = fenwick_tree<W>(max_dep+1);

        centroid[s] = false;
    }

    void add(int v, W x){
        int rec = 0;
        int now = v;
        while(now != -1){
            dsum[now].add(dep[v][rec++], x);
            now = par[now];
        }
    }

    // [0, i]
    void query(int v, int i){
        W ans = dsum[v].sum(i);
        int now = v;
        while(par[now] != -1){
            i -= 
        }
    }
};
#endif
