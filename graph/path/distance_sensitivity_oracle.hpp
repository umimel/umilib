#ifndef DISTANCE_SENSITIVITY_ORACLE_HPP
#define DISTANCE_SENSITIVITY_ORACLE_HPP

/*depend on*/
#include "../../header.hpp"

/*start*/
/**
 * @brief 1-Vertex Fault-Tolerant Distance Sensitivity Oracle
 * @date 2025-01-19
 * @author umimel
 * @ref https://people.csail.mit.edu/karger/Papers/optimalDistanceOracle.pdf
 * @note construction time : O(mn \log^2 n)
 * @note size : O(n^2 log n)
 * @note query time : O(log n)
 */
template<typename S>
class distance_sensitivity_oracle{
private:
    struct edge{
        int to;
        S cost;
    };

    struct node{
        vector<int> ch;
        int in = -1;
        int out = -1;
        int par = -1;
        int dep = 0;
        int hv = -1;
        int mv = -1; // max label vertex from r to this node
        vector<int> nv; // mlv[i] = most nearest vertex with label >= i from r to this node
    };

    struct sparse_table{
        vector<int> log_table;
        vector<vector<pair<S, int>>> table;

        sparse_table(){}
        sparse_table(vector<pair<S, int>> vec){
            int n = (int)vec.size();
            log_table.resize(n+1, 0);
            for(int i=2; i<=n; i++) log_table[i] = log_table[i>>1]+1;
            int m = log_table[n];
            table.resize(n, vector<pair<S, int>>(m+1, {-1, -1}));
            for(int i=0; i<n; i++) table[i][0] = vec[i];
            for(int j=1; j<=m; j++){
                for(int i=0; i<n; i++){
                    if(i+(1<<(j-1))<n) table[i][j] = max(table[i][j-1], table[i+(1<<(j-1))][j-1]);
                    else table[i][j] = table[i][j-1];
                }
            }
        }

        // prod of [l, r)
        pair<S, int> prod(int l, int r){
            assert(l<=r);
            if(l==r) return {-1, -1};
            int k = log_table[r-l];
            return max(table[l][k], table[r-(1<<k)][k]);
        }
    };

    S inf = numeric_limits<S>::max()/3;
    using graph = vector<vector<edge>>;
    using tree = vector<node>;

    int n, log_n = 0;
    graph G, revG;
    vector<tree> T, revT;
    vector<vector<S>> dist, rdist;
    vector<vector<sparse_table>> st;
    /** {d, v} in D[c][t] : d = dist_{G - v}(c, t) */
    vector<vector<vector<pair<S, int>>>> D, revD;
    vector<int> label;

    vector<S> dijkstra(graph &G, int s){
        vector<S> dist(n, inf);
        priority_queue<pair<S, int>, vector<pair<S, int>>, greater<>> que;
        dist[s] = 0;
        que.push({0, s});
        while(!que.empty()){
            auto [d, v] = que.top(); que.pop();
            if(dist[v] < d) continue;

            for(auto e : G[v]) if(dist[v] + e.cost < dist[e.to]){
                dist[e.to] = dist[v] + e.cost;
                que.push({dist[e.to], e.to});
            }
        }

        return dist;
    }

    tree construct_minimum_lexicographic_shortest_path_tree(graph &G, vector<S> &dist, int s){
        tree T(n);
        vector<bool> used(n, false);
        function<void(int)> dfs = [&](int v){
            used[v] = true;
            for(auto e : G[v]) if(dist[v] + e.cost == dist[e.to] && !used[e.to]){
                T[e.to].par = v;
                T[v].ch.push_back(e.to);
                dfs(e.to);
            }
        }; dfs(s);
        return T;
    }

    tree construct_revT(vector<tree> &T, int r){
        vector<bool> used(n, false);
        used[r] = true;
        tree revT(n);
        for(int v=0; v<n; v++){ // this part is O(n^3)
            int par = T[v][r].par;
            int now = r;
            while(par != -1){
                used[par] = true;
                revT[par].par = now;
                revT[now].ch.push_back(par);
                now = par;
                par = T[v][now].par;
            }
        }

        for(int v=0; v<n; v++){
            sort(revT[v].ch.begin(), revT[v].ch.end());
            revT[v].ch.erase(unique(revT[v].ch.begin(), revT[v].ch.end()), revT[v].ch.end());
        }

        return revT;
    }

    void update_dep(tree &T, int r){
        queue<int> que;
        que.push(r);
        while(!que.empty()){
            int v= que.front(); que.pop();
            for(int to : T[v].ch){
                T[to].dep = T[v].dep + 1;
                que.push(to);
            }
        }
    }

    void update_hv(tree &T, int r){
        vector<int> path;
        int now = 0;
        function<void(int)> dfs = [&](int v){
            path.push_back(v);
            T[v].in = now++;
            T[v].hv = path[T[v].dep/2];
            for(int to : T[v].ch) dfs(to);
            path.pop_back();
            T[v].out = now++;
        }; dfs(r);
    }

    void update_mv(tree &T, int r, vector<int> &label){
        queue<int> que;
        que.push(r);
        while(!que.empty()){
            int v = que.front(); que.pop();
            for(int to : T[v].ch){
                if(T[v].mv == -1 || label[T[v].mv] <= label[v]) T[to].mv = v;
                else T[to].mv = T[v].mv;
                update_mv(T, to, label);
            }
        }
    }

    void update_nv(tree &T, int r, vector<int> &label){
        queue<int> que;
        que.push(r);
        while(!que.empty()){
            int v = que.front(); que.pop();
            for(int to : T[v].ch){
                T[to].nv = T[v].nv;
                for(int k=0; k<=label[v]; k++) T[to].nv[k] = v;
                update_nv(T, to, label);
            }
        }
    }

    vector<int> get_subtree_vertices(tree &T, int v){
        vector<int> res;
        queue<int> que; que.push(v);
        while(!que.empty()){
            int v = que.front(); que.pop();
            for(int to : T[v].ch){
                res.push_back(to);
                que.push(to);
            }
        }

        return res;
    }

    void pick_center(vector<tree> &T, vector<int> &label){
        int n = (int)T.size();
        vector<vector<int>> cnt(n, vector<int>(n, 0));

        for(int k=1; k<=log_n; k++){
            // init
            for(int s=0; s<n; s++)for(int v=0; v<n; v++) cnt[s][v] = 0;
            function<void(int, int)> dfs = [&](int s, int v){
                if(T[s][v].dep == (1<<(k-1))){
                    cnt[s][v] = 1;
                    return;
                }
                for(int to : T[s][v].ch){
                    dfs(s, to);
                    cnt[s][v] += cnt[s][to];
                }
            };
            for(int s=0; s<n; s++) dfs(s, s);

            while(true){
                int mxv = -1, mxcnt = 0;
                for(int v=0; v<n; v++){
                    int sum = 0; for(int s=0; s<n; s++) sum += cnt[s][v];
                    if(mxcnt < sum){
                        mxcnt = sum;
                        mxv = v;
                    }
                }
                if(mxv == -1) break;
                label[mxv] = k;
                for(int s=0; s<n; s++){
                    int now = mxv;
                    int c = cnt[s][mxv];
                    while(now != -1){
                        cnt[s][now] -= c;
                        now = T[s][now].par; 
                    }
                }
            }
        }
    }

    void cover_ball(vector<tree> &T, vector<vector<vector<pair<S, int>>>> &D, graph &G, graph &revG, vector<vector<S>> &dist){
        for(int s=0; s<n; s++){
            vector<S> d(n, inf);
            for(int v=0; v<n; v++) d[v] = dist[s][v];
            queue<int> que; for(int to : T[s][s].ch) que.push(to);
            while(!que.empty()){
                int v = que.front(); que.pop();
                vector<int> vs = get_subtree_vertices(T[s], v);
                for(int t : vs) d[t] = inf;
                d[v] = inf;
                priority_queue<pair<S, int>, vector<pair<S, int>>, greater<>> pque;
                for(int t : vs) for(auto e : revG[t]){
                    if(d[e.to]+e.cost < d[t]){
                        d[t] = d[e.to] + e.cost;
                        pque.push({d[t], t});
                    }
                }
                while(!pque.empty()){
                    auto [tmp, t] = pque.top(); pque.pop();
                    if(d[t] < tmp) continue;
                    for(auto e : G[t]){
                        if(d[t]+e.cost < d[e.to]){
                            d[e.to] = d[t] + e.cost;
                            pque.push({d[e.to], e.to});
                        }
                    }
                }
                
                for(int t : vs) D[s][t].push_back({d[t], v});
                for(int t : vs) d[t] = dist[s][t];
                d[v] = dist[s][v];
                if(label[s] < label[v]) continue;
                for(int to : T[s][v].ch) que.push(to);
            }
        }
    }

    S find_bot(int s, int t, int cs, int ct, int l, int r){
        if(T[l][r].dep <= 1){
            S res = 0;
            res = max(res, min(dist[s][cs] + D[cs][t][T[cs][l].dep-1].first, revD[ct][s][revT[ct][l].dep-1].first + dist[ct][t]));
            res = max(res, min(dist[s][cs] + D[cs][t][T[cs][r].dep-1].first, revD[ct][s][revT[ct][r].dep-1].first + dist[ct][t]));

            return res;
        }

        int m = T[l][r].hv;
        auto [tmp, v] = st[l][r].prod(T[l][m].dep-1, T[l][r].dep-1);
        S L = dist[s][cs] + D[cs][t][T[cs][v].dep-1].first;
        S R = revD[ct][s][revT[ct][v].dep-1].first + dist[ct][t];

        if(L <= R) return max(min(L, R), find_bot(s, t, cs, ct, l, m));
        else return find_bot(s, t, cs, ct, m, r);
    }

public:

    distance_sensitivity_oracle(int n) : n(n){
        G.resize(n); revG.resize(n);
        dist.resize(n); rdist.resize(n);
        T.resize(n), revT.resize(n);
        D.resize(n, vector<vector<pair<S, int>>>(n)), revD.resize(n, vector<vector<pair<S, int>>>(n));
        st.resize(n, vector<sparse_table>(n));
        label.resize(n, 0);
        while(n){
            n >>= 1;
            log_n++;
        }
    }

    void add_edge(int from, int to, S cost){
        G[from].push_back({to, cost});
        revG[to].push_back({from, cost});
    }

    void build(){
        /** construct the minimum lexcographic shortest path tree on G */
        for(int s=0; s<n; s++){
            dist[s] = dijkstra(G, s);
            T[s] = construct_minimum_lexicographic_shortest_path_tree(G, dist[s], s);
        }
        for(int s=0; s<n; s++){
            rdist[s].resize(n);
            for(int v=0; v<n; v++) rdist[s][v] = dist[v][s];
        }

        // construct revT
        for(int s=0; s<n; s++) revT[s] = construct_revT(T, s);

        // update tree depth
        for(int r=0; r<n; r++) update_dep(T[r], r), update_hv(T[r], r);
        for(int r=0; r<n; r++) update_dep(revT[r], r), update_hv(revT[r], r);

        /** pick center */
        pick_center(T, label);


        /** cover with center */
        cover_ball(T, D, G, revG, dist);
        cover_ball(revT, revD, revG, G, rdist);

        // construct sparse table
        for(int c=0; c<n; c++)for(int t=0; t<n; t++) st[c][t] = sparse_table(D[c][t]);

        // update tree info
        for(int r=0; r<n; r++){
            for(int v=0; v<n; v++) T[r][v].nv = vector<int>(log_n+1, -1), revT[r][v].nv = vector<int>(log_n+1, -1);
            update_mv(T[r], r, label);
            update_nv(T[r], r, label);
            update_mv(revT[r], r, label);
            update_nv(revT[r], r, label);
        }
    }

    /**
     * @param s : source vertex
     * @param t : target vertex
     * @param v : failed vertex
     * 
     * @return the distance from s to t avoiding v on G
     * 
     * @note query time : O(\log n)
     */
    S query(int s, int t, int v){
        assert(s!=v && t!=v);
        if(s == t) return 0;
        if(T[s][t].dep == 0) return -1;

        // v does not exist in the shortest path from s to t
        if(T[s][t].in < T[s][v].in || T[s][v].out < T[s][t].out) return dist[s][t];

        // specify cs and ct
        int cs = T[s][v].mv, ct = revT[t][v].mv;
        if(label[cs] < label[ct]) ct = revT[t][v].nv[label[cs]];
        else cs = T[s][v].nv[label[ct]];

        S res = min(find_bot(s, t, cs, ct, cs, ct), min(dist[s][cs] + D[cs][t][T[cs][v].dep-1].first, revD[ct][s][revT[ct][v].dep-1].first + dist[ct][t]));

        if(inf <= res) return -1;
        else return res;
    }
};
#endif
