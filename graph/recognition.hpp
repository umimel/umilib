#ifndef RECOGNITION_HPP
#define RECOGNITION_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename S>
bool is_connected(graph<S> &G){
    int n = G.size();
    if(n == 0) return true;

    vector<bool> used(n, false);
    queue<int> que;
    used[0] = true;
    que.push(0);

    while(!que.empty()){
        int v = que.front();
        que.pop();
        for(auto e : G[v]) if(!used[e.to]){
            used[e.to] = true;
            que.push(e.to);
        }
    }

    for(int v=0; v<n; v++) if(!used[v]) return false;
    return true;
}

template<typename S>
bool is_simple(graph<S> &G){
    set<pair<int, int>> st;

    for(auto e : G.get_edge_set()){
        int u = e.from, v = e.to;
        if(u == v) return false;
        if(!G.get_dir() && u > v) swap(u, v);
        if(st.find({u, v}) != st.end()) return false;
        st.insert({u, v});
    }

    return true;
}

template<typename S>
bool is_tree(graph<S> &G){
    assert(!G.get_dir());
    int n = G.size();
    int m = G.edge_size();
    return m == n-1 && is_connected(G);
}

template<typename S>
bool is_forest(graph<S> &G){
    assert(!G.get_dir());
    int n = G.size();

    vector<int> par(n), siz(n, 1);
    iota(par.begin(), par.end(), 0);

    auto root = [&](auto self, int x) -> int {
        if(par[x] == x) return x;
        return par[x] = self(self, par[x]);
    };

    auto unite = [&](int x, int y) -> bool {
        x = root(root, x);
        y = root(root, y);
        if(x == y) return false;
        if(siz[x] < siz[y]) swap(x, y);
        par[y] = x;
        siz[x] += siz[y];
        return true;
    };

    for(auto e : G.get_edge_set()){
        if(!unite(e.from, e.to)) return false;
    }
    return true;
}

template<typename S>
bool is_path(graph<S> &G){
    assert(!G.get_dir());
    int n = G.size();
    int m = G.edge_size();
    if(n == 0) return true;
    if(n == 1) return m == 0;
    if(m != n-1 || !is_connected(G)) return false;

    int leaf = 0;
    for(int v=0; v<n; v++){
        int deg = (int)G[v].size();
        if(deg == 1) leaf++;
        else if(deg != 2) return false;
    }

    return leaf == 2;
}

template<typename S>
bool is_biconnected(graph<S> &G){
    assert(!G.get_dir());
    int n = G.size();
    if(n <= 2) return is_connected(G);
    if(!is_connected(G)) return false;

    vector<int> ord(n, -1), low(n, 0);
    int timer = 0;
    bool ok = true;

    auto dfs = [&](auto self, int v, int pe) -> void {
        ord[v] = low[v] = timer++;
        int child = 0;

        for(auto e : G[v]){
            if(e.id == pe) continue;
            if(ord[e.to] == -1){
                child++;
                self(self, e.to, e.id);
                low[v] = min(low[v], low[e.to]);
                if(pe != -1 && ord[v] <= low[e.to]) ok = false;
            }else{
                low[v] = min(low[v], ord[e.to]);
            }
        }

        if(pe == -1 && child >= 2) ok = false;
    };

    dfs(dfs, 0, -1);
    return ok;
}

template<typename S>
bool is_bipartite(graph<S> &G){
    int n = G.size();
    vector<int> color(n, -1);

    for(int s=0; s<n; s++) if(color[s] == -1){
        color[s] = 0;
        queue<int> que;
        que.push(s);

        while(!que.empty()){
            int v = que.front();
            que.pop();
            for(auto e : G[v]){
                if(color[e.to] == -1){
                    color[e.to] = 1-color[v];
                    que.push(e.to);
                }else if(color[e.to] == color[v]){
                    return false;
                }
            }
        }
    }

    return true;
}

template<typename S>
bool is_dag(graph<S> &G){
    assert(G.get_dir());
    int n = G.size();
    vector<int> indeg(n, 0);
    for(int v=0; v<n; v++) for(auto e : G[v]) indeg[e.to]++;

    queue<int> que;
    for(int v=0; v<n; v++) if(indeg[v] == 0) que.push(v);

    int cnt = 0;
    while(!que.empty()){
        int v = que.front();
        que.pop();
        cnt++;
        for(auto e : G[v]){
            indeg[e.to]--;
            if(indeg[e.to] == 0) que.push(e.to);
        }
    }

    return cnt == n;
}

template<typename S>
bool is_eulerian(graph<S> &G){
    int n = G.size();
    int m = G.edge_size();
    if(m == 0) return true;

    if(G.get_dir()){
        vector<int> indeg(n, 0), outdeg(n, 0);
        vector<vector<int>> rev(n);
        int s = -1;

        for(int v=0; v<n; v++){
            for(auto e : G[v]){
                outdeg[v]++;
                indeg[e.to]++;
                rev[e.to].push_back(v);
                s = v;
            }
        }

        for(int v=0; v<n; v++) if(indeg[v] != outdeg[v]) return false;
        for(int v=0; v<n; v++) if(indeg[v] + outdeg[v] > 0){
            s = v;
            break;
        }

        auto reach = [&](bool reverse_graph){
            vector<bool> used(n, false);
            queue<int> que;
            used[s] = true;
            que.push(s);
            while(!que.empty()){
                int v = que.front();
                que.pop();
                if(reverse_graph){
                    for(int to : rev[v]) if(!used[to]){
                        used[to] = true;
                        que.push(to);
                    }
                }else{
                    for(auto e : G[v]) if(!used[e.to]){
                        used[e.to] = true;
                        que.push(e.to);
                    }
                }
            }
            for(int v=0; v<n; v++) if(indeg[v] + outdeg[v] > 0 && !used[v]) return false;
            return true;
        };

        return reach(false) && reach(true);
    }

    vector<int> deg(n, 0);
    for(auto e : G.get_edge_set()){
        deg[e.from]++;
        deg[e.to]++;
    }
    for(int v=0; v<n; v++) if(deg[v] % 2 != 0) return false;

    int s = -1;
    for(int v=0; v<n; v++) if(deg[v] > 0){
        s = v;
        break;
    }
    if(s == -1) return true;

    vector<bool> used(n, false);
    queue<int> que;
    used[s] = true;
    que.push(s);
    while(!que.empty()){
        int v = que.front();
        que.pop();
        for(auto e : G[v]) if(!used[e.to]){
            used[e.to] = true;
            que.push(e.to);
        }
    }

    for(int v=0; v<n; v++) if(deg[v] > 0 && !used[v]) return false;
    return true;
}

template<typename S>
bool is_complete(graph<S> &G){
    int n = G.size();
    long long m = G.edge_size();
    if(!is_simple(G)) return false;

    if(G.get_dir()){
        if(m != 1LL*n*(n-1)) return false;
        for(int v=0; v<n; v++) if((int)G[v].size() != n-1) return false;
        return true;
    }

    if(m != 1LL*n*(n-1)/2) return false;
    for(int v=0; v<n; v++) if((int)G[v].size() != n-1) return false;
    return true;
}

template<typename S>
bool is_regular(graph<S> &G){
    int n = G.size();
    if(n == 0) return true;

    int deg = (int)G[0].size();
    for(int v=1; v<n; v++) if((int)G[v].size() != deg) return false;
    return true;
}

bool is_transitive(graph<int> &G){
    int n = (int)G.size();

    vector<bool> conn(n, false);
    for(int v=0; v<n; v++){
        for(auto e : G[v]) conn[e.to] = true;
        for(auto e : G[v]) for(auto f : G[e.to]) if(!conn[f.to]) return false;
        for(auto e : G[v]) conn[e.to] = false;
    }

    return true;
}

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
