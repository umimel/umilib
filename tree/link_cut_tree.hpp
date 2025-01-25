#ifndef LC_TREE_HPP
#define LC_TREE_HPP

/*depends on*/
#include "../header.hpp"

/*start*/
template<typename S, S (*op)(S, S), S (*o)()>
class link_cut_tree{
private:
    struct node{
        S val; // 頂点の値
        S acc; // 部分木内のvalの総和
        bool rev; // 部分木が反転されているか
        node *p; // 親へのポインタ
        node *l, *r;

        node(S val = o()) : val(val), acc(val), rev(false), p(nullptr), l(nullptr), r(nullptr){}

        // 自身が根かどうか
        bool is_root(){
            return !p || (p->l!=this && p->r!=this);
        }

        // 自身の情報を子の情報を元にして更新
        void push_up(){
            acc = op(l ? l->acc : o(), op(val, r ? r->acc : o()));
        }

        void push_down(){
            if(rev){
                rev = false;
                swap(l, r);

                if(l) l->rev = !l->rev;
                if(r) r->rev = !r->rev;
            }

            push_up();
        }

        // 右回転
        void rotr(){
            node *my_p = p, *my_pp = my_p->p;

            if(my_p->l = r) r->p = my_p;
            my_p->push_up();

            r = my_p; my_p->p = this;
            push_up();

            if(p = my_pp){
                if(my_pp->l == my_p) my_pp->l = this;
                if(my_pp->r == my_p) my_pp->r = this;
            }
        }

        // 左回転
        void rotl(){
            node *my_p = p, *my_pp = my_p->p;

            if(my_p->r = l) l->p = my_p;
            my_p->push_up();

            l = my_p; my_p->p = this;
            push_up();

            if(p = my_pp){
                if(my_pp->l == my_p) my_pp->l = this;
                if(my_pp->r == my_p) my_pp->r = this;
            }
        }

        // 自身を heavy path 内の根に移動
        void splay(){
            push_down();

            while(!is_root()){
                node *my_p = p;

                if(my_p->is_root()){
                    my_p->push_down();
                    push_down();

                    if(my_p->l == this) rotr();
                    else rotl();
                }else{
                    node *my_pp = my_p->p;
                    my_pp->push_down();
                    my_p->push_down();
                    push_down();

                    if(my_pp->l == my_p){
                        // zig-zig step
                        if(my_p->l == this) my_p->rotr(), rotr();
                        // zig-zag step
                        else rotl(), rotr();
                    }else{
                        // zig-zig step
                        if(my_p->r == this) my_p->rotl(), rotl();
                        // zig-zag step
                        else rotr(), rotl();
                    }
                }
            }
        }
    };

    int n;
    vector<node> vs;

    void expose(node *v){
        auto v0(v);

        // rt := 作成途中の heavy path の根
        node *rt = nullptr;

        while(v){
            v->splay();

            v->r = rt;
            rt = v;
            v->push_up();

            v = v->p;
        }

        v0->splay();
    }

    void cut(node *v){
        expose(v);

        node *l = v->l;

        v->l = nullptr;
        v->p = nullptr;

        v->push_up();
    }

    void link(node *v, node *vp){
        expose(v);
        expose(vp);

        v->p = vp;
        vp->r = v;

        vp->push_up();
    }

    void evert(node *v){
        expose(v);
        v->rev = !v->rev;
    }

public:
    link_cut_tree(int n) : n(n), vs(n) {}

    link_cut_tree(const vector<S>& a) : n((int)a.size()), vs(n){
        for(int i=0; i<n; i++) vs[i].val = vs[i].acc = a[i];
    }

    void set_root(int rt){
        evert(&vs[rt]);
    }

    void cut(int v){
        cut(&vs[v]);
    }

    void cut(int u, int v){
        if(u == v) return;
        expose(&vs[u]), expose(&vs[v]);
        if(!vs[u].p) return;

        if(vs[v].l == &vs[u]){
            node *l = vs[v].l;
            vs[v].l = nullptr;
            l->p = nullptr;
            vs[v].push_up();
        }else{
            expose(&vs[u]);
            node *l = vs[u].l;
            vs[u].l = nullptr;
            l->p = nullptr;
            vs[u].push_up();
        }
    }

    // 頂点 u と根 v に対し, 親から子への辺 u->tを張る
    void link(int u, int v){
        set_root(v);
        link(&vs[v], &vs[u]);
    }

    // 根から頂点 v までの頂点まで総積を返す
    S prod(int v){
        expose(&vs[v]);
        return vs[v].acc;
    }

    // 頂点 u から頂点 v までの頂点まで総積を返す
    S path_prod(int u, int v){
        set_root(u);
        expose(&vs[v]);
        return vs[v].acc;
    }

    void set(int v, S x){
        expose(&vs[v]);
        vs[v].val = x;
        vs[v].push_up();
    }

    void add(int v, S x){
        expose(&vs[v]);
        vs[v].val += x;
        vs[v].push_up();
    }
};
#endif