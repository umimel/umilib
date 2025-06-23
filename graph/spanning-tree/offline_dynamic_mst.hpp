#ifndef OFFLINE_DYMST_HPP
#define OFFLINE_DYMST_HPP

/*depends on*/
#include "../../header.hpp"
#include "../template.hpp"

/*start*/
template<typename S>
class dynamic_mst{
private:
    struct qedge{
        int l, r;
        int u, v;
        S cost;
        bool operator<(const qedge& e) {
            return cost < e.cost;
        }
    };
    struct union_find{
        vector<int> par;
        vector<int> siz;

        void reset(int n){
            fill(siz.begin(), siz.begin()+n, 1);
            iota(par.begin(), par.begin()+n, 0);
        }
        
        union_find(int n) : par(n), siz(n){
            reset(n);
        }
        
        int root(int x){
            if (par[x] == x) return x;
            return par[x] = root(par[x]);
        }
    
        void unite(int x, int y){
            int rx = root(x);
            int ry = root(y);
            if (rx == ry) return;
            if (siz[rx] < siz[ry]) swap(rx, ry);
            siz[rx] += siz[ry];
            par[ry] = rx;
        }
    
        bool same(int x, int y){
            int rx = root(x);
            int ry = root(y);
            return rx == ry;
        }
    
        int size(int x){
            return siz[root(x)];
        }
    };

    int n=0, m=0, q=0;
    vector<qedge> ives; // edges + time interval that they are active.
    edges<S> ses; // edge sets of input graph 
    vector<int> touch; // last time this edge was touched
    union_find uf1, uf2;
    vector<int> id;
    vector<S> ans;
    const S sinf = std::numeric_limits<S>::max() / 2;

public:

    dynamic_mst(graph<S> &G) : uf1(G.get_vnum()), uf2(G.get_vnum()){
        n = G.get_vnum();
        m = G.get_enum();
        ses = G.get_edge_set();
        touch.resize(m);
        id.resize(n);
    }

    void update(int i, int x){
        // update edge weight of edge i to x
        // if you want to delete the edge, just set it to infinity
        q++;
        auto& e = ses[i];
        ives.push_back({touch[i], q, e.from, e.to, e.cost});
        touch[i] = q;
        e.cost = x;
    }

    void solve(int l, int r, vector<qedge> es, int sn, S cost = 0) {
        // remove edges that don't belong to this interval
        es.erase(stable_partition(es.begin(), es.end(), [&](const qedge& e){return !(e.r<=l || r<=e.l);}), es.end());
        uf1.reset(sn), uf2.reset(sn);

        // compressing connected components
        for(auto& e : es) if(l<e.l || e.r<r) { // active edges
            uf1.unite(e.u, e.v);
        }
        
        for(auto& e : es) if(e.l<=l && r<=e.r) { // fully overlapping edges
            if(!uf1.same(e.u, e.v)) {
                uf1.unite(e.u, e.v);
                cost += e.cost;
                uf2.unite(e.u, e.v);
            }
        }

        if(l+1 == r) { // base case, we found the MST.
            ans[l] = cost;
            return;
        }

        int cnt = 0; // relabel all connected components to 0...cnt-1
        for(int v=0; v<sn; v++) if(uf2.root(v) == v) id[v] = cnt++;
        uf1.reset(cnt);
        for(auto& e : es) { // relabeling and marking useless edges
            e.u = id[uf2.root(e.u)], e.v = id[uf2.root(e.v)];
            if(e.l<=l && r<=e.r) {
                // mark useless edge, will get deleted in next step
                if(uf1.same(e.u, e.v)) e.l = sinf/n, e.r = -sinf/n;
                else uf1.unite(e.u, e.v);
            }
        }

        int m = (l+r)/2;
        solve(l, m, es, cnt, cost);
        solve(m, r, es, cnt, cost);
    }

    vector<S> run(){
        q++;
        for(int i=0; i<m; i++){
            auto& e = ses[i];
            ives.push_back({touch[i], q, e.from, e.to, e.cost});
        }
        
        sort(ives.begin(), ives.end()); // (q+m) log(q+m) time
        ans.resize(q);
        solve(0, q, ives, n); // (q+m) log(q) alpha(n) time

        return ans;
    }
};
#endif
