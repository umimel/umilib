#ifndef REPLACEMET_PATH_HPP
#define REPLACEMET_PATH_HPP

/*depend on*/
#include "../../header.hpp"
#include "../graph-template.hpp"
#include "dijkstra.hpp"

/*start*/
struct replacement_path{
public:

    template<typename T>
    static vector<T> malick_mittal_gupta(graph<T> &G, int s, int t){

        // declear variable
        const T TINF = numeric_limits<T>::max()/2;
        dijkstra<T> dijk_s(G, s), dijk_t(G, t);
        int n = G.get_vnum();
        int m = G.get_enum();
        vector<T> dist_s = dijk_s.get_dist();
        vector<T> dist_t = dijk_t.get_dist();
        vector<int> path = dijk_s.get_vpath(t);
        int p = (int)path.size();
        path.push_back(n); // sentinel
        vector<vector<int>> ch(n);
        for(int v=0; v<n; v++) if(v != s) ch[dijk_s.get_vpar(v)].push_back(v);
        vector<int> label(n, -1);


        function<void(int, int)> labeling = [&](int v, int l){
            label[v] = l;
            for(int to : ch[v]) dfs(to, l);
        };
        
        for(int i=0; i<p; i++){
            label[path[i]] = i;
            for(int to : ch[path[i]]) if(to != path[i+1]){
                dfs(to, path[i], i);
            }
        }

        vector<vector<int>> sevt(p), eevt(p);
        for(int v=0; v<n; v++) for(auto e : G[v]) if(dijk_s.get_epar(e.to).id != e.id && label[v] < label[e.to]){
            sevt[label[v]].push_back(e.id);
            tevt[label[v]].push_back(e.id);
        }
        

        set<pair<T, int>> eset;
        vector<int> dist(m, sdist[t]);
        for(int i=1; i<p; i++){
            int u = path[i-1], v = path[i], e_id = dijk_s.get_epar(v);
        
            // start event with label = i-1
            for(int id : sevt[i-1]){
                auto e = G.get_edge(id);
                int x = e.from, y = e.to;
                if(label[x] > label[y]) swap(x, y);
                eset.insert({sdist[x]+e.cost+tdist[y], id});
            }

            // calc ans
            if(eset.empty()) dist[e_id] = TINF;
            else dist[e_id] = (*eset.begin()).first;

            // end event with label = i
            for(int id : evt[i]){
                auto e = G.get_edge(id);
                int x = e.from, y = e.to;
                if(label[x] > label[y]) swap(x, y);
                eset.erase({sdist[x]+e.cost+tdist[y], id});
            }
        }

        return dist;
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
};

template<typename T>
vector<T> strp(graph<T> g, int s, int t){
    int n = g.n;
    int m = g.m;
    const T TINF = numeric_limits<T>::max()/2;

    vector<T> sdist(n, TINF), tdist(n, TINF);
    vector<int> vpar(n, -1), epar(n, -1);
    {
        priority_queue<pair<T, int>, vector<pair<T, int>>, greater<>> pq;
        sdist[s] = 0;
        pq.push({});
        while(!pq.empty()){
            auto [tmp, v] = pq.top();
            pq.pop();
            if(sdist[v] < tmp) continue;

            for(edge<T> e : g[v]){
                if(sdist[v]+e.cost < sdist[e.to]){
                    sdist[e.to] = sdist[v]+e.cost;
                    vpar[e.to] = v;
                    epar[e.to] = e.id;
                    pq.push({sdist[e.to], e.to});
                }
            }
        }

        tdist[t] = 0;
        pq.push({});
        while(!pq.empty()){
            auto [tmp, v] = pq.top();
            pq.pop();
            if(tdist[v] < tmp) continue;

            for(edge<T> e : g[v]){
                if(tdist[v]+e.cost < tdist[e.to]){
                    tdist[e.to] = tdist[v]+e.cost;
                    pq.push({tdist[e.to], e.to});
                }
            }
        }
    }

    vector<int> path;
    int now = t;
    while(now != -1){
        path.push_back(now);
        now = vpar[now];
    }

    vector<bool> is_stpath(n, false);
    is_stpath[s] = true;
    vector<bool> is_stpath_edge(m, false);
    for(int i=1; i<(int)path.size(); i++){
        is_stpath[path[i]] = true;
        is_stpath[epar[path[i]]] = true;
    }

    vector<vector<int>> ch(n);
    for(int v=0; v<n; v++) ch[par[v]].push_back(v);

    vector<int> label(n, -1);
    function<void(int, int)> dfs = [&](int v, int p, int l){
        label[v] = l;
        for(int to : ch[v]) dfs(to, v, l);
    };
    
    for(int i=0; i<(int)path.size(); i++){
        label[path[i]] = i;
        for(int to : ch[path[i]]) if(!is_stpath[to]){
            dfs(to, path[i], i);
        }
    }

    vector<int> add, del;
    for(int i=0; i<m; i++) if(!is_stpath_edge[i]){
        auto e = g.es[i];
        int l0 = label[e.from], l1 = label[e.to];
        if(l0 > l1) swap(l0, l1);
        add.push_back(i);
        del.push_back(i);
    }

    set<pair<T, i>> eset;
    vector<int> dist(m, sdist[t]);
    for(int i=1; i<(int)path.size(); i++){
        int u = path[i-1], v = path[i], e_id = epar[v];
    
        // add label = i-1
        for(int id : add[i-1]){
            auto e = g.es[id];
            int x = e.from, y = e.to;
            if(label[x] > label[y]) swap(x, y);
            eset.insert({sdist[x]+e.cost+tdist[y], i});
        }

        // calc ans
        if(eset.empty()) dist[e_id] = TINF;
        else dist[e_id] = (*eset.begin()).fi;

        // delete label = i
        for(int id : del[i]){
            auto e = g.es[id];
            int x = e.from, y = e.to;
            if(label[x] > label[y]) swap(x, y);
            eset.erase({sdist[x]+e.cost+tdist[y], i});
        }
    }

    return dist;
}
#endif
