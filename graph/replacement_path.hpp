#ifndef REPLACEMENT_PATH_HPP
#define REPLACEMENT_PATH_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"
#include "shortest_path_tree.hpp"

/*start*/
struct replacement_path {

    // Malick-Mittal-Gupta: for undirected graphs with non-negative edge weights.
    // Returns ans[e.id] for all edges: replacement s-t distance for path edges,
    // dist(s,t) for non-path edges, TINF if no replacement exists.
    // Complexity: O((n + m) log n)
    template<typename T>
    static vector<T> malick_mittal_gupta(graph<T> &G, int s, int t) {
        const T TINF = numeric_limits<T>::max() / 3;
        shortest_path_tree<T> dijk_s(G, s), dijk_t(G, t);
        int n = G.size();
        int m = G.edge_size();
        vector<T> dist_s = dijk_s.get_dist();
        vector<T> dist_t = dijk_t.get_dist();
        vector<int> path = dijk_s.get_vpath(t);
        int p = (int)path.size();
        path.push_back(n); // sentinel

        vector<vector<int>> ch(n);
        for (int v = 0; v < n; v++)
            if (dijk_s.get_vpar(v) != -1)
                ch[dijk_s.get_vpar(v)].push_back(v);

        vector<int> label(n, -1);
        function<void(int, int)> labeling = [&](int v, int l) {
            label[v] = l;
            for (int to : ch[v]) labeling(to, l);
        };
        for (int i = 0; i < p; i++) {
            label[path[i]] = i;
            for (int to : ch[path[i]])
                if (to != path[i + 1])
                    labeling(to, i);
        }

        vector<bool> used(m, false);
        for (int i = 1; i < p; i++) used[dijk_s.get_epar(path[i]).id] = true;

        vector<vector<int>> sevt(p), eevt(p);
        for (int v = 0; v < n; v++)
            for (auto e : G[v])
                if (!used[e.id] && label[v] >= 0 && label[e.to] >= 0 && label[v] < label[e.to]) {
                    sevt[label[v]].push_back(e.id);
                    eevt[label[e.to]].push_back(e.id);
                }

        vector<T> ans(m, dijk_s.get_dist(t));
        set<pair<T, int>> eset;
        for (int i = 1; i < p; i++) {
            auto f = dijk_s.get_epar(path[i]);
            ans[f.id] = TINF;

            for (int id : sevt[i - 1]) {
                auto e = G.get_edge(id);
                int x = e.from, y = e.to;
                if (label[x] > label[y]) swap(x, y);
                eset.insert({dist_s[x] + e.cost + dist_t[y], id});
            }

            if (!eset.empty()) ans[f.id] = min(ans[f.id], eset.begin()->first);

            for (int id : eevt[i]) {
                auto e = G.get_edge(id);
                int x = e.from, y = e.to;
                if (label[x] > label[y]) swap(x, y);
                eset.erase({dist_s[x] + e.cost + dist_t[y], id});
            }
        }

        return ans;
    }

    // Roditty-Zwick: for unweighted directed graphs (edge weights ignored, each hop = 1).
    // Returns ans[e.id] for all edges: replacement s-t hop-distance for path edges,
    // dist(s,t) for non-path edges, INF if no replacement exists.
    // Randomized complexity: O(m * sqrt(n) * log(n)) expected.
    template<typename T>
    static vector<T> roditty_zwick(graph<T> &G, int s, int t) {
        int n = G.size();
        int m = G.edge_size();
        const int INF = numeric_limits<int>::max() / 2;
        int log_n = 0, sqrt_n = 1;
        { int sn = n; while (sn) sn >>= 1, log_n++; }
        while (sqrt_n * sqrt_n < n) sqrt_n++;

        // BFS from s
        vector<int> vpar(n, -1), epar(n, -1), sdist(n, INF);
        {
            queue<int> que;
            que.push(s); sdist[s] = 0;
            while (!que.empty()) {
                int v = que.front(); que.pop();
                for (auto e : G[v]) if (sdist[e.to] == INF) {
                    sdist[e.to] = sdist[v] + 1;
                    vpar[e.to] = v;
                    epar[e.to] = e.id;
                    que.push(e.to);
                }
            }
        }

        vector<int> vpath, epath;
        vector<T> ans(m, (T)sdist[t]);
        if (sdist[t] == INF) return ans;

        {
            int now = t;
            while (now != -1) {
                vpath.push_back(now);
                if (now != s) {
                    epath.push_back(epar[now]);
                    ans[epar[now]] = (T)INF;
                }
                now = vpar[now];
            }
            reverse(vpath.begin(), vpath.end());
            reverse(epath.begin(), epath.end());
        }

        int p = (int)vpath.size();

        // H: non-path edges (directed), RH: reverse of H
        graph<int> H(n, true), RH(n, true);
        for (int v = 0; v < n; v++)
            for (auto e : G[v])
                if (ans[e.id] != (T)INF) {
                    H.add_edge(v, e.to);
                    RH.add_edge(e.to, v);
                }

        // Short detours: multi-source BFS with sqrt_n-spaced seeds for each offset i
        int bsz = 2 * n + sqrt_n + 2;
        for (int i = 0; i < sqrt_n; i++) {
            vector<int> dist(n, -1);
            vector<vector<int>> vec(bsz);
            for (int j = i; j < p; j += sqrt_n) {
                int d = sqrt_n * (j / sqrt_n) + i;
                dist[vpath[j]] = d;
                vec[d].push_back(vpath[j]);
            }
            for (int d = 0; d < bsz; d++)
                for (auto v : vec[d])
                    for (auto e : H[v])
                        if (dist[e.to] == -1) {
                            dist[e.to] = dist[v] + 1;
                            if (dist[e.to] < bsz)
                                vec[dist[e.to]].push_back(e.to);
                        }
            for (int j = i + 1; j < p; j++) {
                if (j % sqrt_n == i || dist[vpath[j]] == -1) continue;
                int lo = sqrt_n * ((j - i) / sqrt_n) + i;
                int hi = lo + sqrt_n;
                if (lo <= dist[vpath[j]] && dist[vpath[j]] < hi)
                    for (int k = lo; k < j; k++)
                        ans[epath[k]] = min(ans[epath[k]], (T)(dist[vpath[j]] + (p - 1 - j)));
            }
        }

        // Long detours: randomized sampling of non-path vertices
        auto bfs2 = [&](graph<int> &g, int r, vector<int> &d) {
            queue<int> que;
            que.push(r); d[r] = 0;
            while (!que.empty()) {
                int v = que.front(); que.pop();
                for (auto e : g[v]) if (d[e.to] == -1) {
                    d[e.to] = d[v] + 1;
                    que.push(e.to);
                }
            }
        };

        vector<bool> on_path(n, false);
        for (int i = 0; i < p; i++) on_path[vpath[i]] = true;
        vector<int> rest;
        for (int v = 0; v < n; v++) if (!on_path[v]) rest.push_back(v);

        for (int loop = 0; loop < sqrt_n * log_n && !rest.empty(); loop++) {
            int idx = rng() % rest.size();
            int r = rest[idx];
            swap(rest[idx], rest.back());
            rest.pop_back();

            vector<int> dist(n, -1), rdist(n, -1);
            bfs2(H, r, dist);
            bfs2(RH, r, rdist);

            vector<int> rmin(p + 1, INF);
            for (int i = p - 1; i >= 0; i--) {
                rmin[i] = rmin[i + 1];
                if (dist[vpath[i]] != -1)
                    rmin[i] = min(rmin[i + 1], dist[vpath[i]] + (p - 1 - i));
            }

            int mn = INF;
            for (int i = 0; i < p - 1; i++) {
                if (rdist[vpath[i]] != -1)
                    mn = min(mn, i + rdist[vpath[i]]);
                if (mn < INF && rmin[i + 1] < INF)
                    ans[epath[i]] = min(ans[epath[i]], (T)(mn + rmin[i + 1]));
            }
        }

        return ans;
    }
};
#endif
