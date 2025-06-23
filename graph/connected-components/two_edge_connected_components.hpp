#ifndef TWO_ECC_HPP
#define TWO_ECC_HPP

/*depend on*/
#include "../header.hpp"
#include "../graph-template.hpp"

/*start*/
struct two_edge_connected_components{
    int n, m;
    vector<vector<pair<int, int>>> G;
    unweighted_graph H;
    vector<bool> isbridge;
    vector<int> groupList;

    void init(Edges<int> &E){
        m = (int)E.size();
        G.resize(n);
        isbridge.resize(m, false);
        groupList.resize(n, -1);

        for(int i=0; i<m; i++){
            G[E[i].from].push_back({E[i].to, i});
            G[E[i].to].push_back({E[i].from, i});
        }
    }

    void findbridge(){
        vector<int> epar(n, -1);
        vector<vector<int>> ch(n); 
        vector<bool> visited(n, false);
        vector<int> imos(m, 0);
        vector<int> depth(n, 0);
        
        function<void(int, int)> dfs0 = [&](int v, int r){
            visited[v] = true;
            for(pair<int, int> tmp : G[v]){
                int to = tmp.first;
                int id = tmp.second;
                if(visited[to]){
                    if(depth[v]>depth[to] && epar[v]!=id){
                        imos[epar[v]]++;
                        imos[id]++;
                        if(to!=r) imos[epar[to]]--;
                    }
                }else{
                    depth[to] = depth[v]+1;
                    ch[v].push_back(to);
                    epar[to] = id;
                    dfs0(to, r);
                }
            }
        };

        function<void(int, int)> dfs1 = [&](int v, int r){
            for(int to : ch[v]){
                dfs1(to, r);
                if(v!=r) imos[epar[v]]+=imos[epar[to]];
            }
        };
        
        for(int v=0; v<n; v++) if(!visited[v]){
            dfs0(v, v);
            dfs1(v, v);
        }

        for(int i=0; i<m; i++) isbridge[i] = (imos[i]==0);
    }

    void construction_ecc(){
        int cnt = 0;
        function<void(int)> dfs = [&](int v){
            groupList[v] = cnt;
            for(pair<int, int> e : G[v]){
                if(groupList[e.first]==-1 && !isbridge[e.second]) dfs(e.first);
            }
        };

        for(int v=0; v<n; v++) if(groupList[v]==-1){
            dfs(v);
            cnt++;
        }
    }

    two_edge_connected_components(int n, Edges<int> &E) : n(n){
        init(E);
        findbridge();
        construction_ecc();
    }

    int get_groupList(int v){
        return groupList[v];
    }
};
#endif
