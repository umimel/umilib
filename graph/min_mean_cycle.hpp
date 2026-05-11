#ifndef MIN_MEAN_CYCLE_HPP
#define MIN_MEAN_CYCLE_HPP

/*depends on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
// Karp's algorithm: finds the cycle with minimum mean edge weight.
// Runs Bellman-Ford for exactly n steps with a virtual source (dp[0][v]=0 for all v),
// applies Karp's theorem to locate the optimal vertex, then reconstructs the cycle.
// Returns edges forming the minimum mean cycle, or empty if no cycle exists.
template<typename S>
edges<S> min_mean_cycle(graph<S> &g) {
    int n = g.size();
    if (n == 0) return {};

    const double INF = 1e18;

    // dp[k][v]: min cost to reach v in exactly k edges
    vector<vector<double>> dp(n + 1, vector<double>(n, INF));
    // par[k][v]: {predecessor, edge_id} achieving dp[k][v]
    vector<vector<pair<int,int>>> par(n + 1, vector<pair<int,int>>(n, {-1,-1}));

    for (int v = 0; v < n; v++) dp[0][v] = 0.0;

    for (int k = 1; k <= n; k++) {
        for (int u = 0; u < n; u++) {
            if (dp[k-1][u] >= INF) continue;
            for (auto &e : g[u]) {
                double nd = dp[k-1][u] + (double)e.cost;
                if (nd < dp[k][e.to]) {
                    dp[k][e.to] = nd;
                    par[k][e.to] = {u, e.id};
                }
            }
        }
    }

    // Karp's theorem: mu* = min_v  max_{0<=k<n} (dp[n][v] - dp[k][v]) / (n-k)
    double mu_star = INF;
    int best_v = -1;

    for (int v = 0; v < n; v++) {
        if (dp[n][v] >= INF) continue;
        double mu_v = -INF;
        for (int k = 0; k < n; k++) {
            if (dp[k][v] >= INF) continue;
            double cand = (dp[n][v] - dp[k][v]) / (n - k);
            if (cand > mu_v) mu_v = cand;
        }
        if (mu_v < mu_star) {
            mu_star = mu_v;
            best_v = v;
        }
    }

    if (best_v == -1) return {};

    // Trace back n steps from best_v along parent pointers.
    // verts has n+1 entries; by pigeonhole (n+1 values in [0,n-1]) there is always a repeat.
    vector<int> verts(n + 1), eids(n + 1, -1);
    verts[n] = best_v;
    for (int k = n; k >= 1; k--) {
        auto [from, eid] = par[k][verts[k]];
        verts[k-1] = from;
        eids[k] = eid; // edge from verts[k-1] to verts[k]
    }

    // Scan backwards to find the rightmost cycle (the one involving best_v).
    unordered_map<int,int> seen;
    for (int k = n; k >= 0; k--) {
        if (seen.count(verts[k])) {
            int cs = k, ce = seen[verts[k]];
            edges<S> result;
            for (int i = cs + 1; i <= ce; i++) {
                result.push_back(g.get_edge(eids[i]));
            }
            return result;
        }
        seen[verts[k]] = k;
    }

    return {};
}
#endif
