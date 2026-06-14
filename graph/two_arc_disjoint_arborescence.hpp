#ifndef TWO_ARC_DISJOINT_ARBORESCENCE_HPP
#define TWO_ARC_DISJOINT_ARBORESCENCE_HPP

/*depends on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
struct two_arc_disjoint_arborescence_result {
    bool exists = false;
    array<vector<int>, 2> arborescences;
};

/**
 * Input:
 *   A directed graph G and a root r.
 * Return value:
 *   Two arc-disjoint spanning out-arborescences rooted at r, represented by
 *   edge ids. If they do not exist, exists is false.
 * Complexity:
 *   O((n + m) log(n + m)) time and O(n + m) space.
 */
template<typename T>
two_arc_disjoint_arborescence_result
two_arc_disjoint_arborescence(const graph<T>& G, int r) {
    assert(G.get_dir());
    int n = G.size(), m = G.edge_size(), vn = n+m;
    assert(0 <= r && r < n);

    two_arc_disjoint_arborescence_result result;
    if(n == 1) {
        result.exists = true;
        return result;
    }

    vector<int> from(m), to(m), out_head(n, -1), out_next(m);
    vector<int> in_head(n, -1), in_next(m);
    for(int e=0; e<m; e++) {
        auto a = G.get_edge(e);
        from[e] = a.from;
        to[e] = a.to;
        out_next[e] = out_head[from[e]];
        out_head[from[e]] = e;
        in_next[e] = in_head[to[e]];
        in_head[to[e]] = e;
    }

    // Split every arc e=(u,v) into u -> (n+e) -> v.
    vector<int> order(vn, -1), vertex, parent;
    vector<int> iter(vn, -1), st;
    for(int v=0; v<n; v++) iter[v] = out_head[v];
    for(int e=0; e<m; e++) iter[n+e] = 0;

    order[r] = 0;
    vertex.push_back(r);
    parent.push_back(-1);
    st.push_back(r);
    while(!st.empty()) {
        int v = st.back(), nxt = -1;
        if(v < n) {
            int e = iter[v];
            if(e != -1) {
                iter[v] = out_next[e];
                nxt = n+e;
            }
        } else if(iter[v] == 0) {
            iter[v] = -1;
            nxt = to[v-n];
        }

        if(nxt == -1) {
            st.pop_back();
        } else if(order[nxt] == -1) {
            order[nxt] = vertex.size();
            vertex.push_back(nxt);
            parent.push_back(order[v]);
            st.push_back(nxt);
        }
    }

    if((int)vertex.size() != vn) return result;

    int k = vertex.size();
    vector<int> semi(k), idom(k, -1), relative(k, -1);
    vector<int> ancestor(k, -1), label(k), last(k, -1);
    vector<int> bucket_head(k, -1), bucket_next(k), path;
    iota(semi.begin(), semi.end(), 0);
    iota(label.begin(), label.end(), 0);
    path.reserve(k);

    auto eval = [&](int v) {
        if(ancestor[v] == -1) return label[v];
        path.clear();
        int x = v;
        while(ancestor[x] != -1 && ancestor[ancestor[x]] != -1) {
            path.push_back(x);
            x = ancestor[x];
        }
        for(int i=(int)path.size()-1; i>=0; i--) {
            int y = path[i], a = ancestor[y];
            if(semi[label[a]] < semi[label[y]]) label[y] = label[a];
            ancestor[y] = ancestor[a];
        }
        return label[v];
    };

    auto use_predecessor = [&](int pred_vertex, int v) {
        int p = order[pred_vertex];
        if(p == -1) return;
        int x = eval(p);
        if(semi[x] < semi[v]) {
            semi[v] = semi[x];
            last[v] = pred_vertex;
        }
    };

    for(int i=k-1; i>=1; i--) {
        int v = vertex[i];
        last[i] = vertex[parent[i]];
        if(v < n) {
            for(int e=in_head[v]; e!=-1; e=in_next[e]) use_predecessor(n+e, i);
        } else {
            use_predecessor(from[v-n], i);
        }

        bucket_next[i] = bucket_head[semi[i]];
        bucket_head[semi[i]] = i;
        ancestor[i] = parent[i];

        int p = parent[i];
        for(int x=bucket_head[p]; x!=-1; x=bucket_next[x]) {
            int y = eval(x);
            relative[x] = y;
            idom[x] = (semi[y] == semi[x] ? p : y);
        }
        bucket_head[p] = -1;
    }
    for(int i=1; i<k; i++) {
        if(idom[i] != semi[i]) idom[i] = idom[idom[i]];
    }

    // Georgiadis--Tarjan: construct two independent spanning trees from
    // DFS parents, last arcs of semidominator paths, and relative dominators.
    vector<char> blue(k, false);
    vector<int> blue_parent(k, -1), red_parent(k, -1);
    for(int i=1; i<k; i++) {
        blue[i] = (semi[i] == idom[i]) || !blue[relative[i]];
        if(blue[i]) {
            blue_parent[i] = last[i];
            red_parent[i] = vertex[parent[i]];
        } else {
            blue_parent[i] = vertex[parent[i]];
            red_parent[i] = last[i];
        }
    }

    for(int v=0; v<n; v++) if(v != r) {
        int i = order[v];
        int a = blue_parent[i], b = red_parent[i];
        if(a < n || b < n || a == b) return result;
        result.arborescences[0].push_back(a-n);
        result.arborescences[1].push_back(b-n);
    }
    result.exists = true;
    return result;
}

#endif
