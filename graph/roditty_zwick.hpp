#ifndef RODITTY_ZWICK_HPP
#define RODITTY_ZWICK_HPP

/*depend on*/
#include "../header.hpp"
#include "graph.hpp"

/*start*/
template<typename T>
vector<T> roditty_zwick(graph<T> &G, int s, int t){
    int n = G.size();
    int m = G.edge_size();
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
#endif
