#ifndef SCC_HPP
#define SCC_HPP

/*depend on*/
#include "../header.hpp"
#include "template.hpp"

/*start*/
struct strongly_connected_components{
    int n=0;
    int n2=0;
    unweighted_graph dag;
    vector<int> group_list;
    vector<int> group_size;

    strongly_connected_components(unweighted_graph &G){
        n = (int)G.size();
        group_list.resize(n, -1);
        init(G);
    }

    void init(unweighted_graph &G){
        vector<bool> checked(n, false);
        vector<int> order_list(n, -1);
        int cnt = 0;
        unweighted_graph RG(n);
        for(int v=0; v<n; v++) for(int to : G[v]) RG[to].pb(v);

        function<void(int)> dfs1 = [&](int v){
            checked[v] = true;
            for(int to : G[v]){
                if(checked[to]) continue;
                dfs1(to);
            }
            order_list[cnt++] = v;
            return;
        };

        for(int v=0; v<n; v++) if(!checked[v]) dfs1(v);

        int group = 0;
        function<void(int)> dfs2 = [&](int v){
            group_list[v] = group;
            for(int to : RG[v]){
                if(group_list[to]!=-1) continue;
                dfs2(to);
            }
            return;
        };

        for(int i=n-1; i>=0; i--){
            if(order_list[i]==-1) break;
            if(group_list[order_list[i]] != -1) continue;
            dfs2(order_list[i]);
            group++;
        }
 
        n2 = group;
        group_size.resize(n2, 0);
        for(int v=0; v<n; v++) group_size[group_list[v]]++;

        unweighted_graph dag_tmp(n2);
        for(int v=0; v<n; v++) for(int to : G[v]){
            if(group_list[v] == group_list[to]) continue;
            dag_tmp[group_list[v]].push_back(group_list[to]);
        }

        for(int v=0; v<n2; v++){
            sort(all(dag_tmp[v]));
            dag_tmp[v].erase(unique(all(dag_tmp[v])), dag_tmp[v].end());
        }

        dag.resize(n2);
        for(int v=0; v<n2; v++){
            for(int to : dag_tmp[v]) dag[v].push_back(to);
        }
    }
    
    //各頂点番号は, トポロジカルソート済み
    unweighted_graph get_dag(){return dag;}
    
    int get_group(int v){return group_list[v];}
    int get_size(int group){return group_size[group];}
};
#endif