#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

/*depend on*/
#include "../header.hpp"
#include "../graph-template.hpp"

/*start*/
struct functional_graph {
private:
    int n;
    int LOG;
    vector<int> nxt;
    vector<vector<int>> rev;

    // indeg peeling
    vector<int> indeg;
    vector<int> is_cycle;

    // cycle information
    vector<int> comp;        // component(cycle) id
    vector<int> cycle_id;    // same as comp, kept for readability
    vector<int> cycle_pos;   // position in its cycle, -1 if not on cycle
    vector<int> cycle_size_; // size of each cycle

    // tree information
    vector<int> dist;        // distance to cycle
    vector<vector<int>> up;  // reverse-tree parent lifting
                           // up[0][v] = nxt[v] for non-cycle node, cycle node -> itself

    void build() {
        rev.assign(n, {});
        indeg.assign(n, 0);
        for (int v = 0; v < n; v++) {
            rev[nxt[v]].push_back(v);
            indeg[nxt[v]]++;
        }

        // 1) peel non-cycle vertices
        queue<int> q;
        for (int v = 0; v < n; v++) {
            if (indeg[v] == 0) q.push(v);
        }

        vector<int> removed(n, 0);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            removed[v] = 1;
            int to = nxt[v];
            indeg[to]--;
            if (indeg[to] == 0) q.push(to);
        }

        is_cycle.assign(n, 0);
        for (int v = 0; v < n; v++) {
            if (!removed[v]) is_cycle[v] = 1;
        }

        comp.assign(n, -1);
        cycle_id.assign(n, -1);
        cycle_pos.assign(n, -1);
        dist.assign(n, -1);

        // 2) enumerate cycles
        int cid = 0;
        for (int v = 0; v < n; v++) {
            if (!is_cycle[v] || comp[v] != -1) continue;

            vector<int> cyc;
            int cur = v;
            do {
                comp[cur] = cid;
                cycle_id[cur] = cid;
                cycle_pos[cur] = (int)cyc.size();
                dist[cur] = 0;
                cyc.push_back(cur);
                cur = nxt[cur];
            } while (cur != v);

            cycle_size_.push_back((int)cyc.size());
            cid++;
        }

        // 3) BFS from cycles into trees
        queue<int> bfs;
        for (int v = 0; v < n; v++) {
            if (is_cycle[v]) bfs.push(v);
        }

        while (!bfs.empty()) {
            int v = bfs.front();
            bfs.pop();
            for (int u : rev[v]) {
                if (comp[u] != -1) continue; // already assigned
                comp[u] = comp[v];
                cycle_id[u] = cycle_id[v];
                dist[u] = dist[v] + 1;
                bfs.push(u);
            }
        }

        // 4) lifting
        LOG = 1;
        while ((1LL << LOG) <= max(1, n)) LOG++;
        up.assign(LOG, vector<int>(n));

        for (int v = 0; v < n; v++) {
            if (is_cycle[v]) up[0][v] = v;
            else up[0][v] = nxt[v];
        }
        for (int k = 1; k < LOG; k++) {
            for (int v = 0; v < n; v++) {
                up[k][v] = up[k - 1][up[k - 1][v]];
            }
        }
    }

    int climb_to_cycle(int v, int step) const {
        for (int k = 0; k < LOG; k++) {
            if (step >> k & 1) v = up[k][v];
        }
        return v;
    }

public:
    functional_graph(const vector<int>& _nxt) : n((int)_nxt.size()), nxt(_nxt) {
        build();
    }

    // get loop size that v belongs in group
    int loop_size(int v) const {
        return cycle_size_[cycle_id[v]];
    }

    // get id with k-hop from v
    int k_hop(int v, long long k) const {
        if (k <= dist[v]) {
            for (int b = 0; b < LOG; b++) {
                if (k >> b & 1LL) v = up[b][v];
            }
            return v;
        }

        // first move to cycle
        k -= dist[v];
        v = climb_to_cycle(v, dist[v]);

        int cid = cycle_id[v];
        int sz = cycle_size_[cid];
        int pos = cycle_pos[v];

        k %= sz;
        while (k--) v = nxt[v];
        return v;
    }

    // optional utilities
    bool on_cycle(int v) const {
        return is_cycle[v];
    }

    int dist_to_cycle(int v) const {
        return dist[v];
    }

    int component_id(int v) const {
        return comp[v];
    }
};
#endif