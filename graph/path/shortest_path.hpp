#ifndef SHORTEST_PATH_HPP
#define SHORTEST_PATH_HPP

/*depend on*/
#include "../../header.hpp"
#include "../graph-template.hpp"

/*start*/
class shortest_path{
private:

// restoreable dijkstra
template<typename T>
    struct rdijkstra{
    private:
        const T TINF = numeric_limits<T>::max()/3;
        int n, s;
        graph<T> G;
        vector<T> dist;
        vector<int> vpar;
        edges<T> epar;

    public:
        rdijkstra(graph<T> G, int s) : G(G), s(s){
            // initilization
            n = G.get_vnum();
            dist.resize(n, TINF);
            vpar.resize(n, -1);
            epar.resize(n, -1);

            // running Dijkstra algorithm
            run();
        }

        void run(){
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
    template<typename T>
    static vector<T> bfs(graph<T> &G, int s){
        int n = G.get_vnum();
        vector<T> dist(n, -1);

        dist[s] = 0;
        queue<int> que;
        que.push(s);
        while(!que.empty()){
            int v = que.front();
            que.pop();
            for(auto e : G[v]) if(dist[e.to]==-1){
                dist[e.to] = dist[v] + 1;
                que.push(e.to);
            }
        }

        return dist;
    }

    template<typename T>
    static vector<T> binary_bfs(graph<T> &G, int s){
        int n = G.get_vnum();
        vector<T> dist(n, -1);

        dist[s] = 0;
        deque<int> deq;
        deq.push_front(s);
        while(!deq.empty()){
            int v = deq.front();
            deq.pop_front();
            for(auto e : G[v]) if(dist[e.to]==-1){
                dist[e.to] = dist[v] + e.cost;
                if(e.cost) deq.push_back(e.to);
                else deq.push_front(e.to);
            }
        }

        return dist;
    }

    template<typename T>
    static vector<T> constant_bfs(graph<T> &G, int s, T W){
        int n = G.get_vnum();
        vector<T> dist(n, -1);
        vector<vector<int>> cand(n*W+1);

        dist[s] = 0;
        cand[0].push_back(s);
        for(int d=0; d<=n*W; d++) for(int v : cand[d]){
            if(dist[v]!=-1) continue;
            for(auto e : G[v]) if(dist[v] + dist[e.to] < dist[e.ot]){
                dist[e.to] = dist[v] + e.cost;
                cand[dist[e.to]].push_back(e.to);
            }
        }

        return dist;
    }

    template<typename T>
    static vector<T> complement_bfs(graph<T> &G, int s){
        int n = G.get_vnum();
        map<pair<int, int>, bool> mp;
        for(int v=0; v<n; v++) for(auto e : G[v]) mp[{v, e.to}] = true;

        vector<T> dist(n, -1);
        vector<int> unvisited;
        for(int v=0; v<n; v++) if(v != s) unvisited.push_back(v);
        queue<int> visited;
        visited.push(s);
        dist[s] = 0;

        while(!visited.empty()){
            int v = visited.front();
            visited.pop();

            vector<int> nxt;
            for(int to : unvisited){
                if(!mp[{v, to}]){
                    visited.push(to);
                    dist[to] = dist[v]+1;
                }else{
                    nxt.pb(to);
                }
            }

            unvisited = nxt;
        }

        return dist;
    }
    
    template<typename T>
    static vector<T> bellman_ford(graph<T> &G, int s){
        int n = G.get_vnum();
        bool dir = G.get_dir();
        const T TINF = numeric_limits<T>::max()/3;
        edges<T> es = G.get_edge_set();
        vector<T> dist(n, TINF);
        vector<bool> flag(n, false);

        dist[s] = 0;
        for(int i=0; i<n; i++) for(auto e : es){
            if(dist[e.from]!=TINF && dist[e.from]+e.cost<dist[e.to]) dist[e.to] = dist[e.from] + e.cost;
            if(!dir && dist[e.to]!=TINF && dist[e.to]+e.cost<dist[e.from]) dist[e.from] = dist[e.to] + e.cost;
        }

        for(int i=0; i<n; i++) for(auto e : es){
            if(dist[e.from]!=TINF && dist[e.from]+e.cost<dist[e.to]) dist[e.to] = dist[e.from] + e.cost, flag[e.to]=true;
            if(!dir && dist[e.to]!=TINF && dist[e.to]+e.cost<dist[e.from]) dist[e.from] = dist[e.to] + e.cost, flag[e.from]=true;
        }

        for(int i=0; i<n; i++) for(auto e : es){
            flag[e.to] = flag[e.to] | flag[e.from];
            if(!dir) flag[e.from] = flag[e.from] | flag[e.to];
        }
        for(int v=0; v<n; v++) if(flag[v]) dist[v] = -TINF;

        return dist;
    }

    template<typename T>
    static vector<vector<T>> warshall_floyd(graph<T> &G){
        int n = G.get_vnum();
        const T TINF = numeric_limits<T>::max()/3;
        vector<vector<T>> dist(n, vector<T>(n, TINF));
        
        for(int v=0; v<n; v++) dist[v][v] = 0;
        for(int v=0; v<n; v++) for(auto e : G[v]) dist[v][e.to] = min(dist[v][e.to], e.cost);
        for(int k=0; k<n; k++) for(int i=0; i<n; i++) for(int j=0; j<n; j++) if(dist[i][k] < TINF && dist[k][j] < TINF) dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

        return dist;
    }

    template<typename T>
    static vector<vector<pair<int, T>>> pered(graph<T> &G, int k){
        int n = G.get_vnum();
        const T TINF = numeric_limits<T>::max()/3;
        priority_queue<tuple<T, int, int>, vector<tuple<T, int, int>>, greater<>> que;
        vector<vector<pair<int, T>>> neibors(n);
        vector<unordered_map<int, T>> mp(n);
        for(int v=0; v<n; v++){
            que.push({0, v, v});
            mp[v][v] = 0;
        }
        while(!que.empty()){
            auto [d, v, s] = que.top();
            que.pop();
            if((int)neibors[v].size()==k) continue;
            if(mp[v].find(s)!=mp[v].end()) if(mp[v][s] < d) continue;
            neibors[v].push_back({s, d});

            for(auto e : G[v]){
                if((int)neibors[e.to].size()==k) continue;
                if(mp[e.to].find(s)==mp[e.to].end()) mp[e.to][s] = TINF;
                if(d + e.cost < mp[e.to][s]){
                    mp[e.to][s] = d + e.cost;
                    que.push({d+e.cost, e.to, s});
                }
            }
        }

        return neibors;
    }

    template<typename T>
    static vector<T> malick_mittal_gupta(graph<T> &G, int s, int t){
        // declear variable
        const T TINF = numeric_limits<T>::max()/3;
        rdijkstra<T> dijk_s(G, s), dijk_t(G, t);
        int n = G.get_vnum();
        int m = G.get_enum();
        vector<T> dist_s = dijk_s.get_dist();
        vector<T> dist_t = dijk_t.get_dist();
        vector<int> path = dijk_s.get_vpath(t);
        int p = (int)path.size();
        path.push_back(n);
        vector<vector<int>> ch(n);
        for(int v=0; v<n; v++) if(dijk_s.get_vpar(v) != -1) ch[dijk_s.get_vpar(v)].push_back(v);
        vector<int> label(n, -1);

        function<void(int, int)> labeling = [&](int v, int l){
            label[v] = l;
            for(int to : ch[v]) labeling(to, l);
        };
        
        for(int i=0; i<p; i++){
            label[path[i]] = i;
            for(int to : ch[path[i]]) if(to != path[i+1]){
                labeling(to, i);
            }
        }

        vector<bool> used(m, false);
        for(int i=1; i<p; i++) used[dijk_s.get_epar(path[i]).id] = true;

        vector<vector<int>> sevt(p), eevt(p);
        for(int v=0; v<n; v++) for(auto e : G[v]) if(!used[e.id] && label[v] < label[e.to]){
            sevt[label[v]].push_back(e.id);
            eevt[label[e.to]].push_back(e.id);
        }
        

        vector<T> ans(m, dijk_s.get_dist(t));
        set<pair<T, int>> eset;
        for(int i=1; i<p; i++){
            auto v = path[i];
            auto f = dijk_s.get_epar(v);
            ans[f.id] = TINF;
        
            // start event with label = i-1
            for(int id : sevt[i-1]){
                auto e = G.get_edge(id);
                int x = e.from, y = e.to;
                if(label[x] > label[y]) swap(x, y);
                eset.insert({dist_s[x]+e.cost+dist_t[y], id});
            }

            // calc ans
            if(!eset.empty()) ans[f.id] = min(ans[f.id], (*eset.begin()).first);

            // end event with label = i
            for(int id : eevt[i]){
                auto e = G.get_edge(id);
                int x = e.from, y = e.to;
                if(label[x] > label[y]) swap(x, y);
                eset.erase({dist_s[x]+e.cost+dist_t[y], id});
            }
        }

        return ans;
    }

    template<typename T>
    static vector<T> roditty_zwick(graph<T> &G, int s, int t){
        int n = G.get_vnum();
        int m = G.get_enum();
        const T TINF = numeric_limits<T>::max()/2;
        int log_n = 0, sqrt_n = 0;
        int sn = n;
        while(sn) sn/=2, log_n++;
        while(sqrt_n*sqrt_n<n) sqrt_n++;

        vector<int> vpar(n, -1), epar(n, -1), sdist(n, IINF);
        auto bfs1 = [&](int s){
            queue<int> que;
            que.push(s);
            sdist[s] = 0;
            while(!que.empty()){
                int v = que.front();
                que.pop();
                for(auto e : G[v]) if(sdist[e.to]==IINF){
                    sdist[e.to] = sdist[v] + 1;
                    vpar[e.to] = v;
                    epar[e.to] = e.id;
                    que.push(e.to);
                }
            }
        }; bfs1(s);

        vector<int> vpath, epath;
        vector<int> ans(m, sdist[n-1]);

        if(sdist[n-1]==IINF) return ans;

        int now = t;
        while(now != -1){
            vpath.push_back(now);
            if(now != 0){
                epath.push_back(epar[now]);
                ans[epar[now]] = IINF;
            }
            now = vpar[now];
        }
        reverse(vpath.begin(), vpath.end());
        reverse(epath.begin(), epath.end());

        int p = (int)vpath.size();
        graph<int> H(n, true), RH(n, true);
        for(int v=0; v<n; v++) for(auto e : G[v]) if(ans[e.id] != IINF){
            H.add_edge(v, e.to);
            RH.add_edge(e.to, v);
        }

        // find all short detour of length < sqrt_n
        for(int i=0; i<sqrt_n; i++){
            vector<int> dist(n, -1);
            vector<vector<int>> vec(2*n);
            for(int j=i; j<p; j+=sqrt_n){
                dist[vpath[j]] = sqrt_n*(j/sqrt_n) + i;
                assert(dist[vpath[j]]<n);
                vec[dist[vpath[j]]].push_back(vpath[j]);
            }

            for(int j=0; j<2*n; j++){
                for(auto v : vec[j]) for(auto e : H[v]) if(dist[e.to]==-1){
                    dist[e.to] = dist[v] + 1;
                    vec[dist[e.to]].push_back(e.to);
                }
            }

            for(int j=i+1; j<p; j++) if(j%sqrt_n!=i){
                int lo = sqrt_n*((j-i)/sqrt_n) + i;
                int hi = lo + sqrt_n;
                int r = lo;
                if(dist[vpath[j]]==-1) continue;
                if(lo <= dist[vpath[j]] && dist[vpath[j]] < hi){
                    for(int k=r; k<j; k++){
                        ans[epath[k]] = min(ans[epath[k]], dist[vpath[j]]+(p-1-j));
                    }
                }
            }
        }

        auto bfs2 = [&](graph<int> &g, int r, vector<int> &dist){
            queue<int> que;
            que.push(r);
            dist[r] = 0;
            while(!que.empty()){
                int v = que.front();
                que.pop();
                for(auto e : g[v]) if(dist[e.to]==-1){
                    dist[e.to] = dist[v] + 1;
                    que.push(e.to);
                }
            }
        };

        // find long detours (randomized)
        vector<bool> check_path_vertex(n, false);
        for(int i=0; i<p; i++) check_path_vertex[vpath[i]] = true;
        vector<int> rest;
        for(int v=0; v<n; v++) if(!check_path_vertex[v]) rest.push_back(v);
        for(int loop=0; loop<sqrt_n*log_n; loop++){
            if((int)rest.size()==0) break;
            int idx = rng()%(int)rest.size();
            int r = rest[idx];
            rest.erase(rest.begin()+idx);

            vector<int> dist(n, -1), rdist(n, -1);
            bfs2(H, r, dist);
            bfs2(RH, r, rdist);

            vector<int> rmin(p+1, IINF);
            for(int i=p-1; i>=0; i--){
                rmin[i] = rmin[i+1];
                if(dist[vpath[i]]!=-1){
                    rmin[i] = min(rmin[i+1], dist[vpath[i]]+(p-1-i));
                }
            }
            
            int mn = IINF;
            for(int i=0; i<p-1; i++){
                if(rdist[i]!=-1){
                    mn = min(mn, i + rdist[vpath[i]]);
                }
                ans[epath[i]] = min(ans[epath[i]], mn + rmin[i+1]);
            }
        }

        return ans;
    }

    template<typename T>
    static vector<T> yen(graph<T> &G, int s, int t, int k){
        
    }

    template<typename T>
    static vector<T> dijkstra(graph<T> &G, int s){
        int n = G.get_vnum();
        const T TINF = numeric_limits<T>::max()/3;
        vector<T> dist(n, TINF);

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
                    que.push({dist[e.to], e.to});
                }
            }
        }

        return dist;
    }
};
#endif