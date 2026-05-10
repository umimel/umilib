#ifndef UNICYCLIC_GRAPH_HPP
#define UNICYCLIC_GRAPH_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
struct unicyclic_graph{
private:
    int n;
    int LOG;
    vector<vector<int>> G;

    vector<int> par;
    vector<vector<int>> up;

    vector<int> is_cycle;
    vector<int> comp;
    vector<int> cycle_id;
    vector<int> cycle_pos;
    vector<int> cycle_size_;
    vector<int> dist;
    vector<vector<int>> cycles;

    void build(){
        vector<int> deg(n, 0);
        queue<int> q;
        vector<int> removed(n, 0);

        for(int v=0; v<n; v++){
            deg[v] = (int)G[v].size();
            if(deg[v] <= 1) q.push(v);
        }

        while(!q.empty()){
            int v = q.front();
            q.pop();
            if(removed[v]) continue;
            removed[v] = 1;

            for(int to : G[v]){
                if(removed[to]) continue;
                deg[to]--;
                if(deg[to] == 1) q.push(to);
            }
        }

        is_cycle.assign(n, 0);
        for(int v=0; v<n; v++){
            if(!removed[v]) is_cycle[v] = 1;
        }

        par.assign(n, -1);
        comp.assign(n, -1);
        cycle_id.assign(n, -1);
        cycle_pos.assign(n, -1);
        dist.assign(n, -1);

        int cid = 0;
        for(int s=0; s<n; s++){
            if(!is_cycle[s] || comp[s] != -1) continue;

            vector<int> cyc;
            int v = s, p = -1;

            while(true){
                comp[v] = cid;
                cycle_id[v] = cid;
                cycle_pos[v] = (int)cyc.size();
                dist[v] = 0;
                par[v] = v;
                cyc.push_back(v);

                int to = -1;
                for(int u : G[v]){
                    if(is_cycle[u] && u != p){
                        to = u;
                        break;
                    }
                }
                if(to == -1){
                    for(int u : G[v]){
                        if(is_cycle[u]){
                            to = u;
                            break;
                        }
                    }
                }

                assert(to != -1);
                if(to == s) break;

                p = v;
                v = to;
                assert(comp[v] == -1);
            }

            cycles.push_back(cyc);
            cycle_size_.push_back((int)cyc.size());
            cid++;
        }

        queue<int> bfs;
        for(int v=0; v<n; v++){
            if(is_cycle[v]) bfs.push(v);
        }

        while(!bfs.empty()){
            int v = bfs.front();
            bfs.pop();

            for(int to : G[v]){
                if(is_cycle[to] || comp[to] != -1) continue;

                comp[to] = comp[v];
                cycle_id[to] = cycle_id[v];
                dist[to] = dist[v] + 1;
                par[to] = v;
                bfs.push(to);
            }
        }

        for(int v=0; v<n; v++) assert(comp[v] != -1);

        LOG = 1;
        while((1LL << LOG) <= max(1, n)) LOG++;
        up.assign(LOG, vector<int>(n));

        for(int v=0; v<n; v++){
            if(is_cycle[v]) up[0][v] = v;
            else up[0][v] = par[v];
        }
        for(int k=1; k<LOG; k++){
            for(int v=0; v<n; v++){
                up[k][v] = up[k-1][up[k-1][v]];
            }
        }
    }

    int climb_to_cycle(int v, int step) const{
        for(int k=0; k<LOG; k++){
            if(step >> k & 1) v = up[k][v];
        }
        return v;
    }

public:
    unicyclic_graph(const vector<vector<int>> &graph){
        n = (int)graph.size();
        G = graph;
        build();
    }

    template<typename T>
    unicyclic_graph(const graph<T> &graph){
        assert(!graph.get_dir());
        n = graph.size();
        G.assign(n, {});
        for(auto e : graph.get_edge_set()){
            G[e.from].push_back(e.to);
            G[e.to].push_back(e.from);
        }
        build();
    }

    int loop_size(int v) const{
        return cycle_size_[cycle_id[v]];
    }

    int k_hop(int v, long long k) const{
        if(k <= dist[v]){
            for(int b=0; b<LOG; b++){
                if(k >> b & 1LL) v = up[b][v];
            }
            return v;
        }

        k -= dist[v];
        v = climb_to_cycle(v, dist[v]);

        int cid = cycle_id[v];
        int sz = cycle_size_[cid];
        int pos = cycle_pos[v];

        return cycles[cid][(pos + k) % sz];
    }

    bool on_cycle(int v) const{
        return is_cycle[v];
    }

    int dist_to_cycle(int v) const{
        return dist[v];
    }

    int component_id(int v) const{
        return comp[v];
    }
};

#endif
