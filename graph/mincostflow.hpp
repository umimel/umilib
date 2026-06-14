#ifndef MINCOSTFLOW_HPP
#define MINCOSTFLOW_HPP

#include <bits/stdc++.h>
#include "../header.hpp"
using namespace std;

/*start*/
template <class Cap, class Cost>
struct mcf_graph {
public:
    mcf_graph() {}
    explicit mcf_graph(int n) : _n(n) {}

    int add_edge(int from, int to, Cap cap, Cost cost) {
        assert(0 <= from && from < _n);
        assert(0 <= to && to < _n);
        assert(0 <= cap);
        assert(0 <= cost);
        int m = int(_edges.size());
        _edges.push_back({from, to, cap, 0, cost});
        return m;
    }

    struct edge {
        int from, to;
        Cap cap, flow;
        Cost cost;
    };

    edge get_edge(int i) {
        assert(0 <= i && i < (int)_edges.size());
        return _edges[i];
    }
    vector<edge> edges() { return _edges; }

    pair<Cap, Cost> flow(int s, int t) {
        return flow(s, t, numeric_limits<Cap>::max());
    }
    pair<Cap, Cost> flow(int s, int t, Cap flow_limit) {
        return slope(s, t, flow_limit).back();
    }
    vector<pair<Cap, Cost>> slope(int s, int t) {
        return slope(s, t, numeric_limits<Cap>::max());
    }
    vector<pair<Cap, Cost>> slope(int s, int t, Cap flow_limit) {
        assert(0 <= s && s < _n);
        assert(0 <= t && t < _n);
        assert(s != t);

        int m = int(_edges.size());
        vector<int> edge_idx(m);

        vector<vector<_edge>> g(_n);
        {
            vector<int> redge_idx(m);
            for (int i = 0; i < m; i++) {
                auto& e = _edges[i];
                edge_idx[i] = (int)g[e.from].size();
                redge_idx[i] = (int)g[e.to].size();
                g[e.from].push_back({e.to, -1, e.cap - e.flow, e.cost});
                g[e.to].push_back({e.from, -1, e.flow, -e.cost});
            }
            for (int i = 0; i < m; i++) {
                auto& e = _edges[i];
                g[e.from][edge_idx[i]].rev = redge_idx[i];
                g[e.to][redge_idx[i]].rev = edge_idx[i];
            }
        }

        auto result = _slope(g, s, t, flow_limit);

        for (int i = 0; i < m; i++) {
            _edges[i].flow = _edges[i].cap - g[_edges[i].from][edge_idx[i]].cap;
        }

        return result;
    }

private:
    int _n;
    vector<edge> _edges;

    struct _edge {
        int to, rev;
        Cap cap;
        Cost cost;
    };

    vector<pair<Cap, Cost>> _slope(vector<vector<_edge>>& g, int s, int t, Cap flow_limit) {
        // dual[v]: Johnson potential; dist[v]: reduced-cost shortest distance
        vector<pair<Cost, Cost>> dual_dist(_n);
        vector<int> prev_e(_n);  // prev_e[v] = index of reverse edge in g[v] on shortest path
        vector<bool> vis(_n);
        struct Q {
            Cost key;
            int to;
            bool operator<(Q r) const { return key > r.key; }
        };
        vector<int> que_min;  // zero-cost edges (dist == current dist)
        vector<Q> que;

        auto dual_ref = [&]() -> bool {
            for (int i = 0; i < _n; i++) dual_dist[i].second = numeric_limits<Cost>::max();
            fill(vis.begin(), vis.end(), false);
            que_min.clear();
            que.clear();

            size_t heap_r = 0;
            dual_dist[s].second = 0;
            que_min.push_back(s);
            while (!que_min.empty() || !que.empty()) {
                int v;
                if (!que_min.empty()) {
                    v = que_min.back();
                    que_min.pop_back();
                } else {
                    while (heap_r < que.size()) {
                        heap_r++;
                        push_heap(que.begin(), que.begin() + heap_r);
                    }
                    v = que.front().to;
                    pop_heap(que.begin(), que.end());
                    que.pop_back();
                    heap_r--;
                }
                if (vis[v]) continue;
                vis[v] = true;
                if (v == t) break;
                Cost dual_v = dual_dist[v].first, dist_v = dual_dist[v].second;
                for (auto& e : g[v]) {
                    if (!e.cap) continue;
                    Cost cost = e.cost - dual_dist[e.to].first + dual_v;
                    if (dual_dist[e.to].second - dist_v > cost) {
                        Cost dist_to = dist_v + cost;
                        dual_dist[e.to].second = dist_to;
                        prev_e[e.to] = e.rev;
                        if (dist_to == dist_v) {
                            que_min.push_back(e.to);
                        } else {
                            que.push_back({dist_to, e.to});
                        }
                    }
                }
            }
            if (!vis[t]) return false;

            for (int v = 0; v < _n; v++) {
                if (!vis[v]) continue;
                dual_dist[v].first -= dual_dist[t].second - dual_dist[v].second;
            }
            return true;
        };

        Cap flow = 0;
        Cost cost = 0, prev_cost_per_flow = -1;
        vector<pair<Cap, Cost>> result = {{Cap(0), Cost(0)}};
        while (flow < flow_limit) {
            if (!dual_ref()) break;
            Cap c = flow_limit - flow;
            for (int v = t; v != s; ) {
                // g[v][prev_e[v]] is the reverse edge from v to previous vertex u
                int u = g[v][prev_e[v]].to;
                c = min(c, g[u][g[v][prev_e[v]].rev].cap);
                v = u;
            }
            for (int v = t; v != s; ) {
                auto& rev_e = g[v][prev_e[v]];
                int u = rev_e.to;
                rev_e.cap += c;
                g[u][rev_e.rev].cap -= c;
                v = u;
            }
            Cost d = -dual_dist[s].first;
            flow += c;
            cost += c * d;
            if (prev_cost_per_flow == d) result.pop_back();
            result.push_back({flow, cost});
            prev_cost_per_flow = d;
        }
        return result;
    }
};
#endif
