#ifndef PERSISTENT_SEGTREE_HPP
#define PERSISTENT_SEGTREE_HPP

/*depends on*/
#include "../header.hpp"

/*start*/
template <typename T, typename F, int NODES = 20000000>
struct persistent_segtree {
    using ll = long long;

    struct node {
        T data;
        node *l, *r;
        node() {}
        node(const T &_data) : data(_data), l(nullptr), r(nullptr) {}
    };

    node *pool;
    int pid;
    ll n;
    const F f;
    const T id;
    node *nil;
    vector<node *> roots;

    persistent_segtree(const vector<T> &v, const F &f_, const T &id_)
        : pid(0), f(f_), id(id_), nil(nullptr) {
        pool = new node[NODES];
        nil = my_new(id);
        nil->l = nil->r = nil;
        roots.reserve(262144);
        roots.push_back(build(v));
    }

    persistent_segtree(const ll &n_, const F &f_, const T &id_)
        : pid(0), n(n_), f(f_), id(id_), nil(nullptr) {
        pool = new node[NODES];
        nil = my_new(id);
        nil->l = nil->r = nil;
        roots.reserve(262144);
        roots.push_back(nil);
    }

    node *my_new(const T &data) {
        pool[pid].data = data;
        pool[pid].l = pool[pid].r = nil;
        return &(pool[pid++]);
    }

    node *merge(node *l, node *r) {
        pool[pid].data = f(l->data, r->data);
        pool[pid].l = l;
        pool[pid].r = r;
        return &(pool[pid++]);
    }

    node *build(const vector<T> &v) {
        n = (ll)v.size();
        return build(0, (ll)v.size(), v);
    }

    node *build(ll l, ll r, const vector<T> &v) {
        if (l + 1 == r) return my_new(v[l]);
        ll m = (l + r) >> 1;
        return merge(build(l, m, v), build(m, r, v));
    }

 private:
    node *update_sub(ll a, const T &x, node *v, ll l, ll r) {
        if (l + 1 == r) return my_new(x);
        ll m = (l + r) >> 1;
        if (a < m) return merge(update_sub(a, x, v->l, l, m), v->r);
        return merge(v->l, update_sub(a, x, v->r, m, r));
    }

    node *add_sub(ll a, const T &x, node *v, ll l, ll r) {
        if (l + 1 == r) return my_new(f(x, v->data));
        ll m = (l + r) >> 1;
        if (a < m) return merge(add_sub(a, x, v->l, l, m), v->r);
        return merge(v->l, add_sub(a, x, v->r, m, r));
    }

    T query_sub(ll a, ll b, node *v, ll l, ll r) {
        if (v == nil) return id;
        if (r <= a or b <= l) return id;
        if (a <= l and r <= b) return v->data;
        ll m = (l + r) >> 1;
        return f(query_sub(a, b, v->l, l, m), query_sub(a, b, v->r, m, r));
    }

 public:
    node *update(node *v, ll k, const T &x) {
        node *root = update_sub(k, x, v, 0, n);
        roots.push_back(root);
        return root;
    }

    node *update(int t, ll k, const T &x) {
        node *root = update_sub(k, x, roots[t], 0, n);
        roots.push_back(root);
        return root;
    }

    node *update(ll k, const T &x) {
        node *root = update_sub(k, x, roots.back(), 0, n);
        roots.push_back(root);
        return root;
    }

    node *add(node *v, ll k, const T &x) {
        node *root = add_sub(k, x, v, 0, n);
        roots.push_back(root);
        return root;
    }

    node *add(int t, ll k, const T &x) {
        node *root = add_sub(k, x, roots[t], 0, n);
        roots.push_back(root);
        return root;
    }

    node *add(ll k, const T &x) {
        node *root = add_sub(k, x, roots.back(), 0, n);
        roots.push_back(root);
        return root;
    }

    T query(node *v, ll a, ll b) { return query_sub(a, b, v, 0, n); }
    T query(int t, ll a, ll b) { return query_sub(a, b, roots[t], 0, n); }
    T query(ll a, ll b) { return query_sub(a, b, roots.back(), 0, n); }

    node *new_tree() { return nil; }
};
#endif
