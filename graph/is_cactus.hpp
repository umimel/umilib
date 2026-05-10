#ifndef IS_CACTUS_HPP
#define IS_CACTUS_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<bool EDGE=true, typename S>
bool is_cactus(graph<S> &G){
    assert(!G.get_dir());

    int n = G.size();
    int m = G.edge_size();

    for(auto e : G.get_edge_set()){
        if(e.from == e.to) return false;
    }

    if constexpr(EDGE){
        vector<int> ord(n, -1), low(n, 0);
        vector<int> st;
        int timer = 0;
        bool ok = true;

        auto check_component = [&](int last_id) -> void {
            vector<int> edge_ids;

            while(true){
                int id = st.back();
                st.pop_back();
                edge_ids.push_back(id);
                if(id == last_id) break;
            }

            vector<int> vs;
            for(int id : edge_ids){
                auto e = G.get_edge(id);
                vs.push_back(e.from);
                vs.push_back(e.to);
            }

            sort(vs.begin(), vs.end());
            vs.erase(unique(vs.begin(), vs.end()), vs.end());

            int vertex_num = (int)vs.size();
            int edge_num = (int)edge_ids.size();

            if(edge_num != 1 && edge_num != vertex_num) ok = false;
        };

        auto dfs = [&](auto self, int v, int pe) -> void {
            ord[v] = low[v] = timer++;

            for(auto e : G[v]){
                if(e.id == pe) continue;

                if(ord[e.to] == -1){
                    st.push_back(e.id);
                    self(self, e.to, e.id);
                    low[v] = min(low[v], low[e.to]);

                    if(ord[v] <= low[e.to]) check_component(e.id);
                }else if(ord[e.to] < ord[v]){
                    st.push_back(e.id);
                    low[v] = min(low[v], ord[e.to]);
                }
            }
        };

        for(int v=0; v<n; v++){
            if(ord[v] == -1) dfs(dfs, v, -1);
        }

        return ok;
    }else{
        vector<int> ord(n, -1), low(n, 0);
        vector<bool> is_bridge(m, false);
        int timer = 0;

        auto dfs = [&](auto self, int v, int pe) -> void {
            ord[v] = low[v] = timer++;

            for(auto e : G[v]){
                if(e.id == pe) continue;

                if(ord[e.to] == -1){
                    self(self, e.to, e.id);
                    low[v] = min(low[v], low[e.to]);
                    if(ord[v] < low[e.to]) is_bridge[e.id] = true;
                }else{
                    low[v] = min(low[v], ord[e.to]);
                }
            }
        };

        for(int v=0; v<n; v++){
            if(ord[v] == -1) dfs(dfs, v, -1);
        }

        vector<int> comp(n, -1);
        int comp_num = 0;

        auto paint = [&](auto self, int v) -> void {
            comp[v] = comp_num;

            for(auto e : G[v]){
                if(is_bridge[e.id]) continue;
                if(comp[e.to] == -1) self(self, e.to);
            }
        };

        for(int v=0; v<n; v++){
            if(comp[v] != -1) continue;
            paint(paint, v);
            comp_num++;
        }

        vector<int> vertex_num(comp_num, 0), edge_num(comp_num, 0);

        for(int v=0; v<n; v++) vertex_num[comp[v]]++;

        for(auto e : G.get_edge_set()){
            if(comp[e.from] == comp[e.to]) edge_num[comp[e.from]]++;
        }

        for(int i=0; i<comp_num; i++){
            if(vertex_num[i] < edge_num[i]) return false;
        }

        return true;
    }
}

template<typename S>
bool is_cacutas(graph<S> &G){
    return is_cactus(G);
}

#endif
