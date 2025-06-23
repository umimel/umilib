#ifndef CYCLE_HPP
#define CYCLE_HPP

/*depends on*/
#include "../../header.hpp"
#include "../template.hpp"

/*start*/
struct cycle{
private:

    template<typename T>
    struct dijkstra{
    private:
        const T TINF = numeric_limits<T>::max()/2;
        int n, s;
        graph<T> G;
        vector<T> dist;
        vector<int> vpar;
        edges<T> epar;

    public:
        dijkstra(graph<T> G, int s) : G(G), s(s){
            // initilization
            n = G.get_vnum();
            dist.resize(n, TINF);
            vpar.resize(n, -1);
            epar.resize(n);

            // running Dijkstra algorithm
            run(G, s);
        }

        void run(graph<T> &G, int s){
            dist[s] = 0;
            priority_queue<pair<T, int>, vector<pair<T, int>>, greater<>> que;
            que.push({0, s});
            while(!que.empty()){
                auto [d, v] = que.top();
                que.pop();
                if(dist[v] < d) continue;

                for(auto e : G[v]){
                    if(dist[v] + e.cost < dist[e.to]){
                        dist[e.to] = dist[v] + e.cost;
                        vpar[e.to] = v;
                        epar[e.to] = e;
                        que.push({dist[e.to], e.to});
                    }
                }
            }
        }

        T get_dist(int t){
            return dist[t];
        }

        vector<T> get_dist(){
            return dist;
        }

        vector<int> get_vpar(){
            return vpar;
        }

        int get_vpar(int v){
            return vpar[v];
        }

        edges<T> get_epar(){
            return epar;
        }

        edge<T> get_epar(int v){
            return epar[v];
        }

        vector<int> get_vpath(int t){
            vector<int> vpath;
            int cur = t;
            while(cur != -1){
                vpath.push_back(cur);
                cur = vpar[cur];
            }
            reverse(vpath.begin(), vpath.end());

            return vpath;
        }

        edges<T> get_epath(int t){
            edges<T> epath;
            int cur = t;
            while(cur != s){
                epath.push_back(epar[cur]);
                cur = vpar[cur];
            }
            reverse(epath.begin(), epath.end());

            return epath;
        }

        graph<T> get_shotest_path_tree(){
            graph<T> spt(n, false);
            for(int v=0; v<n; v++) if(v != s){
                int p = vpar[v];
                auto e = G.get_edge(epar[v]);
                spt[vpar[v]].add_edge(vpar[v], v, e.cost);
            }

            return spt;
        }
    };

public:
    template<typename S>
    static edges<S> find_cycle(graph<S> &G){
        int n = G.get_vnum();
        vector<int> used(n, 0);
        edges<S> cyc;

        function<bool(int, int)> dfs = [&](int v, int e_id){
            for(auto e : G[v]) if(e.id != e_id){
                if(used[e.to]==1){
                    cyc.push_back(e);
                    return true;
                }else if(used[e.to]==0){
                    used[e.to] = used[v];
                    if(dfs(e.to, e.id)){
                        cyc.push_back(e);
                        return true;
                    }
                }
            }

            used[v] = 2;
            return false;
        };

        for(int v=0; v<n; v++) if(used[v]==0){
            used[v] = 1;
            if(dfs(v, -1)) break;
        }

        if(cyc.empty()) return cyc;
        while(cyc.back().from != cyc[0].to) cyc.pop_back();
        reverse(cyc.begin(), cyc.end());

        return cyc;
    }

    template<typename T>
    static edges<T> find_odd_cycle(graph<T> &G, int s){

    }

    template<typename T>
    static edges<T> find_even_cycle(graph<T> &G, int s){
        
    }

    template<typename S>
    static edges<S> find_min_cycle(graph<S> &G, int s){
        int n = G.get_vnum();
        const S SINF = numeric_limits<S>::max()/3;
        bool dir = G.get_dir();
        dijkstra<S> dijk(G, s);
        auto dist = dijk.get_dist();
        edges<S> cyc;

        // find minimum cost cycle on directed graph
        if(dir){
            S cost = SINF;
            edge<S> emin; 
            for(int v=0; v<n; v++) for(auto e : G[v]) if(e.to == s){
                if(dist[v] + e.cost < cost){
                    cost = dist[v] + e.cost;
                    emin = e;
                }
            }

            if(cost == SINF) return {};
            cyc = dijk.get_epath(emin.from);
            cyc.push_back(emin);
        }

        // find minimum cost cycle on undirected graph
        if(!dir){
            vector<vector<int>> ch(n);
            for(int v=0; v<n; v++) if(v != s && dijk.get_vpar(v)!=-1){
                ch[dijk.get_vpar(v)].push_back(v);
            }
            
            vector<int> label(n, -1);
            label[s] = s;
            function<void(int, int)> labeling = [&](int v, int l){
                label[v] = l;
                for(int to : ch[v]) labeling(to, l);
            };
            for(int to : ch[s]) labeling(to, to);

            S cost = SINF;
            edge<S> emin;
            for(int v=0; v<n; v++) if(v != s) for(auto e : G[v]){
                if(e.id != dijk.get_epar(v).id && label[v] != label[e.to] && dist[v] + dist[e.to] + e.cost < cost){
                    cost = dist[v] + dist[e.to] + e.cost;
                    emin = e;
                }   
            }

            if(cost == SINF) return {};

            cyc = dijk.get_epath(emin.from);
            cyc.push_back(emin);
            auto epath = dijk.get_epath(emin.to);
            reverse(epath.begin(), epath.end());
            for(auto e : epath){
                e.reverse();
                cyc.push_back(e);
            }
        }

        return cyc;
    }

    template<typename S>
    static edges<S> find_min_cycle(graph<S> &G){
        int n = G.get_vnum();
        const S SINF = numeric_limits<S>::max()/2;
        S cost = SINF;
        edges<S> min_cyc;
        
        for(int s=0; s<n; s++){
            auto cyc = find_min_cycle(G, s);
            if(cyc.empty()) continue;
            S sum = 0;
            for(auto e : cyc) sum += e.cost;
            if(sum < cost){
                cost = sum;
                min_cyc = cyc;
            }
        }

        return min_cyc;
    }

    template<typename S>
    static edges<S> find_min_mean_cycle(graph<S> &G){

    }

    template<typename S>
    static vector<edges<S>> enumerate_3cycles(graph<S> &G){
        int n = G.get_vnum();
        int m = G.get_enum();
        bool dir = G.get_dir();
        vector<edges<S>> ans;

        if(!dir){
            auto comp = [&](int i, int j){
                if(G[i].size() != G[j].size()) return G[i].size() < G[j].size();
                return i < j;
            };

            graph<S> H(n, true);
            for(int i=0; i<m; i++){
                auto e = G.get_edge(i);
                if(comp(e.from, e.to)) H.add_edge(e.from, e.to);
                else H.add_edge(e.to, e.from);
            }
            
            vector<int> used(n, -1);
            for(int v=0; v<n; v++){
                for(auto e3 : H[v]) used[e3.to] = e3.id;
                for(auto e1 : H[v]) for(auto e2 : H[e1.to]){
                    if(used[e2.to] != -1){
                        ans.push_back({});
                        ans.back().push_back(e1);
                        ans.back().push_back(e2);
                        auto e3 = H.get_edge(used[e2.to]);
                        e3.reverse();
                        ans.back().push_back(e3);
                    }
                }
                for(auto &e3 : G[v]) used[e3.to] = -1;
            }
        }

        return ans;
    }

    template<typename S>
    static int count_3cycles(graph<S> &G){
        int n = G.get_vnum();
        int m = G.get_enum();
        bool dir = G.get_dir();
        int cnt = 0;

        if(!dir){
            auto comp = [&](int i, int j){
                if(G[i].size() != G[j].size()) return G[i].size() < G[j].size();
                return i < j;
            };

            graph<S> H(n, true);
            for(int i=0; i<m; i++){
                auto e = G.get_edge(i);
                if(comp(e.from, e.to)) H.add_edge(e.from, e.to);
                else H.add_edge(e.to, e.from);
            }
            
            vector<int> used(n, -1);
            for(int v=0; v<n; v++){
                for(auto e3 : H[v]) used[e3.to] = e3.id;
                for(auto e1 : H[v]) for(auto e2 : H[e1.to]){
                    if(used[e2.to] != -1) cnt++;
                }
                for(auto &e3 : G[v]) used[e3.to] = -1;
            }
        }

        return cnt;
    }

    template<typename S>
    static edges<S> find_min_3cycle(graph<S> &G){

    }

    template<typename S>
    static vector<S> count_4cycles(graph<S> &G){
        /** declear variable */
        int n = G.get_vnum();
        int m = G.get_enum();
        int b = (int)ceil(sqrt(m));
        vector<S> cnt(n, 0);
        vector<int> vec(n, -1);
        vector<S> ans(m, 0);

        // process low degree
        for(int i=0; i<n; i++) if((int)G[i].size() < b){
            for(auto e : G[i]) if((int)G[e.to].size() < b){
                for(auto f : G[e.to]) if((int)G[f.to].size() < b && f.to != i){
                    cnt[f.to] += e.cost * f.cost;
                }
            }
            for(auto e : G[i]) if((int)G[e.to].size() < b){
                for(auto f : G[e.to]) if((int)G[f.to].size() < b && f.to != i){
                    ans[e.id] += (cnt[f.to] - e.cost * f.cost) * f.cost;
                }
            }
            for(auto e : G[i]) if((int)G[e.to].size() < b){
                for(auto f : G[e.to]) if((int)G[f.to].size() < b){
                    cnt[f.to] = 0;
                }
            }
        }
        for(int i=0; i<m; i++) ans[i] /= 2LL;

        // process high degree
        for(int i=0; i<n; i++) if(b <= (int)G[i].size()) {

            for(auto e : G[i]){
                vec[e.to] = e.id;
                cnt[e.to] = e.cost;
            }

            for(int j=0; j<n; j++) if(j != i){
                if(b <= (int)G[j].size() && j < i) continue;
                ll sum = 0;
                for(auto e : G[j]){
                    if(b <= (int)G[e.to].size() && e.to < i) continue;
                    sum += cnt[e.to] * e.cost;
                }

                for(auto e : G[j]){
                    if(b <= (int)G[e.to].size() && e.to < i) continue;
                    if(cnt[e.to] == 0) continue;
                    ans[e.id] += cnt[e.to] * (sum - cnt[e.to]*e.cost);
                    ans[vec[e.to]] += e.cost * (sum - cnt[e.to]*e.cost);
                }
            }

            for(auto e : G[i]) cnt[e.to] = 0;
        }

        return ans;
    }

    template<typename S>
    static edges<S> find_min_4cycle(graph<S> &G){

    }

    template<typename S>
    static edges<S> find_min_ncycle(graph<S> &G){
        int n = G.get_vnum();
        const S SINF = numeric_limits<S>::max()/2;
        vector<vector<S>> dist(1<<n, vector<int>(n, SINF));
        vector<vector<edge<S>>> pre_e(1<<n, vector<edge<S>>(n));
        vector<vector<S>> adj(n, vector<S>(n, SINF));
        vector<vector<edge<S>>> adj_e(n, vector<edge<S>>(n));
        for(int v=0; v<n; v++) for(auto e : G[v]){
            if(e.cost < adj[v][e.to]){
                adj[v][e.to] = e.cost;
                adj_e[v][e.to] = e;
            }
        }

        // initialization
        dist[1][0] = 0;

        for(int bit=2; bit<(1<<n); bit++){
            for(int i=0; i<n; i++) if((bit>>i)&1){
                int bit2 = bit ^ (1<<i);
                for(int j=0; j<n; j++) if((bit2>>j)&1){
                    if(dist[bit2][j] + adj[j][i] < dist[bit][i]){
                        dist[bit][i] = dist[bit2][j] + adj[j][i];
                        pre_e[bit][i] = adj_e[j][i];
                    }
                }
            }
        }

        S cost = SINF;
        edge<S> emin;
        for(int v=1; v<n; v++){
            if(dist[(1<<n)-1][v]+adj[v][0] < cost){
                cost = dist[(1<<n)-1][v]+adj[v][0];
                emin = adj_e[v][0];
            }
        }

        if(cost == SINF) return {};

        edges<S> cyc;
        int state = (1<<n)-1;
        while(true){
            cyc.push_back(emin);
            if(emin.from==0) break;
            emin = pre_e[state][emin.from];
            state = state ^ (1<<emin.to);
        }

        return cyc;
    }

    template<typename S>
    static vector<edges<S>> enumerate_all_cycles(graph<S> &G){

    }

    template<typename S>
    static vector<edges<S>> enumerate_kbest_min_cycles(graph<S> &G, int s, int k){

    }

    template<typename S>
    static vector<edges<S>> find_kth_min_cycle(graph<S> &G, int s, int k){

    }

    template<typename S>
    static vector<edges<S>> enumerate_kbest_min_cycles(graph<S> &G, int k){
        // TODO: Implement k-best minimum cycles enumeration
    }

    template<typename S>
    static vector<edges<S>> find_kth_min_cycle(graph<S> &G, int k){
        // TODO: Implement k-th minimum cycle finding
    }

    template<typename S>
    static vector<vector<int>> decompose_maximal_cycles(graph<S> &G){
        int n = G.get_vnum(); // the number of vertices
        vector<bool> visited(n, false); // visited flag
        vector<vector<int>> cycles; // the set of cycles
        vector<int> idx(n, -1); // the index of the edge
        vector<int> vst; // stack maintaining the vertices

        for(int r=0; r<n; r++){
            vst.push_back(r);
            visited[r] = true;
            while(!vst.empty()){
                int v = vst.back();
                if((int)G[v].size() <= ++idx[v]){
                    visited[v] = false;
                    vst.pop_back();
                    continue;
                }
                auto e = G[v][idx[v]];
                if(visited[e.to]){
                    vector<int> cyc;
                    cyc.push_back(e.id);
                    while(v != e.to){
                        vst.pop_back();
                        visited[v] = false;
                        v = vst.back();
                        cyc.push_back(G[v][idx[v]].id);
                    }
                    reverse(cyc.begin(), cyc.end());
                    cycles.push_back(cyc);
                }else{
                    visited[e.to] = true;
                    vst.push_back(e.to);
                }
            }
            visited[r] = false;
        }

        return cycles;
    }
};
#endif
