#ifndef EPPSTEIN_HPP
#define EPPSTEIN_HPP

/*depend on*/
#include "../header.hpp"
#include "../template.hpp"

/*start*/
template<typename T>
struct heap{
    struct node{
        node* ch[2]={0,0};
        int s;
        T val;
        int from,to;
        node(T val,int from,int to):s(1),val(val),from(from),to(to){}
    };
    using np=node*;
    np root=0;
    heap(np t=0):root(t){}
    np meld(np a,np b){
        if(!b)return a?new node(*a):0;
        if(!a)return b?new node(*b):0;
        a=new node(*a);b=new node(*b);
        if(a->val>b->val)swap(a,b);
        a->ch[1]=meld(a->ch[1],b);
        if(!a->ch[0]||a->ch[0]->s<a->ch[1]->s)swap(a->ch[0],a->ch[1]);
        a->s=(a->ch[1]?a->ch[1]->s:0)+1;
        return a;
    }
    heap meld(heap b){
        return heap(meld(root,b.root));
    }
    heap insert(T x,int from,int to){
        return heap(meld(root,new node(x,from,to)));
    }
    heap pop(){
        return heap(meld(root->ch[0],root->ch[1]));
    }
    T top(){
        return root?root->val:T(-1);
    }
    bool empty(){
        return !root;
    }
};

vector<ll> eppstein(weighted_graph<ll> &graph, int s, int t, int k){
    int n = (int)graph.size();
    vector<vector<tuple<int, ll, int>>> G(n), RG(n);
    int m = 0;
    for(int v=0; v<n; v++) for(edge<ll> e : graph[v]) G[v].push_back({e.to, e.cost, m++});
    for(int v=0; v<n; v++) for(tuple<int, ll, int> e : G[v]){
        int to = get<0>(e), id = get<2>(e);
        ll cost = get<1>(e);
        RG[to].push_back({v, cost, id});
    }

    
    //calculate shortest path tree rooted as t on RG
    vector<ll> dist(n, LINF);
    vector<int> par(n, -1);
    vector<vector<int>> ch(n);
    vector<int> idx(n, -1);
    {
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
        pq.push({0LL, t});
        dist[t] = 0LL;
        while(!pq.empty()){
            int v = pq.top().second;
            ll tmp_dist  = pq.top().first;
            pq.pop();
            if(dist[v] < tmp_dist) continue;
            for(tuple<int, ll, int> e : RG[v]){
                int to = get<0>(e), id = get<2>(e);
                ll cost = get<1>(e);
                if(dist[v] + cost < dist[to]){
                    idx[to] = id;
                    dist[to] = dist[v] + cost;
                    par[to] = v;
                    pq.push({dist[to], to});
                }
            }
        }

        for(int v=0; v<n; v++) if(par[v]!=-1) ch[par[v]].push_back(v);
    }

    //calculate heap for each vertex
    vector<heap<ll>> H(n);
    for(int v=0; v<n; v++) H[v] = heap<ll>();
    {
        queue<int> q;
        q.push(t);
        while(!q.empty()){
            int v = q.front();
            q.pop();
            if(par[v]!=-1) H[v] = H[v].meld(H[par[v]]);
            for(tuple<int, ll, int> e : G[v]){
                int to = get<0>(e), id = get<2>(e);
                ll cost = get<1>(e);
                if(id!=idx[v]) H[v] = H[v].insert(cost-dist[v]+dist[to], v, to);
            }
            for(int c : ch[v]) q.push(c);
        }
    }
    
    //calculate k-shortest walk
    vector<ll> ans;
    {
        auto comp=[](auto a, auto b){return get<0>(a)>get<0>(b);};
        priority_queue<tuple<ll, heap<ll>::np>, vector<tuple<ll, heap<ll>::np>>, decltype(comp)> pq(comp);
        heap<ll> r;
        r = r.insert(dist[s], -1, s);
        pq.push({dist[s], r.root});
        while(!pq.empty()){
            ll d = get<0>(pq.top());
            auto v = get<1>(pq.top());
            pq.pop();
            if(d>=LINF) break;
            ans.push_back(d);
            if((int)ans.size()==k) break;

            if(v->ch[0]) pq.push({d+(v->ch[0]->val)-(v->val), v->ch[0]});
            if(v->ch[1]) pq.push({d+(v->ch[1]->val)-(v->val), v->ch[1]});
            if(H[v->to].root){
                pq.push({d + ((H[v->to].root)->val), H[v->to].root});
            }
        }
    }

    return ans;
}
#endif
