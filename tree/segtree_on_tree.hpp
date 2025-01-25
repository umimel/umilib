#ifndef SEGTREEONTREE_HPP
#define SEGTREEONTREE_HPP

/*depends on*/
#include "../header.hpp"
#include "../graph/template.hpp"

/*start*/
template<typename W, W (*op)(W, W), W (*ie)()>
struct segtree_on_tree{
    struct segtree{
        int n;
        vector<W> dat;

        segtree(){};
        segtree(int n_){
            n = 1;
            while(n < n_) n*=2;
            dat.resize(2*n, ie());
        }

        void add(int k, W a){
            k += n-1;
            dat[k] += a;
            while(k > 0){
                k = (k-1)/2;
                dat[k] = op(dat[2*k+1], dat[2*k+2]);
            }
        }

        void set(int k, W a){
            k += n-1;
            dat[k] = a;
            while(k > 0){
                k = (k-1)/2;
                dat[k] = op(dat[2*k+1], dat[2*k+2]);
            }
        }

        // sum of [a, b)
        W prod(int a, int b){return prod_sub(a, b, 0, 0, n);}

        W prod_sub(int a, int b, int k, int l, int r){
            if(r <= a || b <= l){
                return ie();
            }else if(a <= l && r <= b){
                return dat[k];
            }else{
                W vl = prod_sub(a, b, 2*k+1, l, (l+r)/2);
                W vr = prod_sub(a, b, 2*k+2, (l+r)/2, r);
                return op(vl, vr);
            }
        }
    } seg;

    int n;
    vector<int> par;
    vector<int> pos;
    vector<int> out;
    vector<int> depth;
    vector<int> siz;
    vector<int> path_top;

    segtree_on_tree(tree<W> &T, int root = 0){
        n = (int)T.size();
        par.resize(n, 0);
        pos.resize(n, 0);
        out.resize(n, 0);
        depth.resize(n, 0);
        siz.resize(n, 1);
        path_top.resize(n, -1);

        init(T, root);
    }

    void init(tree<W> &T, int root){
        // calculate par, siz, depth
        function<void(int, int)> dfs1 = [&](int v, int p){
            par[v] = p;
            for(edge<W> e : T[v]) if(e.to!=p){
                depth[e.to] = depth[v]+1;
                dfs1(e.to, v);
                siz[v] += siz[e.to];
            }
        }; dfs1(root, -1);

        // heavy-light decomposition
        int cnt = 0;
        function<void(int, int, int)> dfs2 = [&](int v, int p, int a){
            pos[v] = cnt++;
            path_top[v] = a;

            int M = 0;
            int ch = -1;
            for(edge<W> e : T[v]) if(e.to!=p){
                if(siz[e.to] > M){
                    M = siz[e.to];
                    ch = e.to;
                }
            }
            if(ch!=-1) dfs2(ch, v, a);

            for(edge<W> e : T[v]) if(e.to!=p && e.to!=ch) dfs2(e.to, v, e.to);
        }; dfs2(root, -1, root);

        
        function<int(int, int)> dfs3 = [&](int v, int p){
            int out_pos = 0;
            for(edge<W> e : T[v]) if(e.to != p){
                out_pos = max(out_pos, dfs3(e.to, v));
            }
            out_pos = max(out_pos, pos[v]);
            out[v] = out_pos;
            return out_pos;
        }; dfs3(root, -1);

        seg = segtree(n);
    }

    vector<pair<int, int>> path_dec(int u, int v){
        vector<pair<int, int>> ret;
        while(path_top[u] != path_top[v]){
            if(depth[path_top[u]] <= depth[path_top[v]]){
                ret.push_back({pos[path_top[v]], pos[v]});
                v = par[path_top[v]];
            }else{
                ret.push_back({pos[path_top[u]], pos[u]});
                u = par[path_top[u]];
            }
        }
        ret.push_back({min(pos[u], pos[v]), max(pos[u], pos[v])});
        return ret;
    }

    void vertex_add(int v, W x){seg.add(pos[v], x);}
    void vertex_set(int v, W x){seg.set(pos[v], x);}

    /*
        input : vertex v
        output : prod of subtree rooted as v on rooted tree
        query time : O(log n)
    */
    W subtree_prod(int v){
        return seg.prod(pos[v], out[v]+1);
    }

    /*
        input : vertex u, v
        output : prod of u-v path on rooted tree
        query time : O(log^2 n)
    */
    W path_prod(int u, int v){
        vector<pair<int, int>> dec = path_dec(u, v);
        W ans = ie();
        for(auto [l, r] : dec) ans = op(ans, seg.prod(l, r+1));

        return ans;
    }

    /*
        input : vertex u, v
        output : prod of the vertices are not included in u-v path on rooted tree
        query time : O(log^2 n)
    */
    W non_path_prod(int u, int v){
        vector<pair<int, int>> dec = path_dec(u, v);
        W ans = ie();
        int x = 0;
        for(auto [l, r] : dec){
            if(x<=l){
                ans = op(ans, seg.prod(x, l));
            }
            x = r+1;
        }

        return ans;
    }
};
#endif