#ifndef BIPATITE_MATCHING_HPP
#define BIPATITE_MATCHING_HPP

/*depend on*/
#include "../header.hpp"
#include "../template.hpp"

/*start*/
struct bipartite_matching{
    int value = 0;
    Edges<bool> P;

    bipartite_matching(int L, int R, Edges<bool> E){
        //source:L+R, sing:L+R+1, vertices set:[0, L-1], [L, L+R-1]
        residual_graph<bool> G(L+R+2);
        int s = L+R, t = L+R+1;
        for(int l=0; l<L; l++){
            G[s].push_back(redge<bool>(l, true, false, (int)G[l].size()));
            G[l].push_back(redge<bool>(s, false, false, (int)G[s].size()-1));
        }
        for(edge<bool> e : E){
            G[e.from].push_back(redge<bool>(L+e.to, true, false, (int)G[L+e.to].size()));
            G[L+e.to].push_back(redge<bool>(e.from, false, false, (int)G[e.from].size()-1));
        }
        for(int r=L; r<L+R; r++){
            G[r].push_back(redge<bool>(t, true, false, (int)G[t].size()));
            G[t].push_back(redge<bool>(r, false, false, (int)G[r].size()-1));
        }
        dinic(G, L, R);
    }

    void dinic(residual_graph<bool> &G, int L, int R){
        int n = (int)G.size();
        int s = L+R, t = L+R+1;
        vector<int> level(n, 0);
        vector<int> iter(n, 0);

        function<void()> bfs = [&](){
            for(int v=0; v<n; v++) level[v]=-1;

            queue<int> Q;
            level[s] = 0;
            Q.push(s);
            while(!Q.empty()){
                int v = Q.front();
                Q.pop();

                for(redge<bool> e : G[v]){
                    if(e.cap && level[e.to]==-1){
                        level[e.to] = level[v] + 1;
                        Q.push(e.to);
                    }
                }
            }
        };

        function<bool(int, bool)> dfs = [&](int v, bool f){
            if(v == t) return f;

            for(int &i=iter[v]; i<(int)G[v].size(); i++){
                redge<bool> &e = G[v][i];
                if(e.cap && level[v]<level[e.to]){
                    if(dfs(e.to, f)){
                        e.cap = false;
                        G[e.to][e.rev].cap = true;
                        return true;
                    }
                }
            }

            return false;
        };

        for(;;){
            bfs();
            if(level[t] < 0) break;
            for(int v=0; v<n; v++) iter[v] = 0;
            bool f;
            while((f = dfs(s, true))) value += f;
        }

        for(int l=0; l<L; l++){
            for(redge<bool> e : G[l]){
                if(e.to!=s && !e.cap){
                    P.push_back(edge<bool>(l, e.to-L, false));
                }
            }
        }
    }

    int max_value(){return value;}
    Edges<bool> max_pair(){return P;}
};
#endif
