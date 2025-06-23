#ifndef CCC_HPP
#define CCC_HPP

/*depend on*/
#include "../../header.hpp"
#include "../template.hpp"

/*start*/
template<typename T>
struct complement_connected_components{
private:
    int n;
    vector<vector<int>> cc; // connected-components

public:
    complement_connected_components(graph<T> &G){
        assert(!G.get_dir());

        int n = G.get_vnum();
        set<pair<int, int>> st;
        for(int v=0; v<n; v++) for(auto e : G[v]) st.insert({v, e.to});

        vector<bool> check(n, false);
        vector<int> unvisited;
        for(int v=0; v<n; v++) unvisited.push_back(v);
        queue<int> visited;
        
        for(int r=0; r<n; r++) if(!check[r]){
            cc.push_back({});
            visited.push(r);
            check[r] = true;
            while(!visited.empty()){
                int u = visited.front();
                visited.pop();
                cc.back().push_back(u);

                vector<int> nxt;
                for(int v : unvisited){
                    if(v==r) continue;
                    if(st.find({u, v}) == st.end()){
                        visited.push(v);
                        check[v] = true;
                    }else{
                        nxt.push_back(v);
                    }
                }

                unvisited = nxt;
            }
        }
    }

    vector<vector<int>> get_connected_components(){
        return cc;
    }
};
#endif
