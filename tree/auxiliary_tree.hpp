#ifndef AUXILIARY_TREE_HPP
#define AUXILIARY_TREE_HPP

/*depends on*/
#include "../header.hpp"
#include "../graph/graph-template.hpp"
#include "lowest_common_ancestor.hpp"

/*start*/
template<typename W>
struct auxiliary_tree{
    int n;
    vector<int> in;
    int root;
    lowest_common_ancestor<W> lca;

    auxiliary_tree(tree<W> &T, int root = 0) : root(root){
        n = (int)T.size();
        in.resize(n);
        euler_tour(T);
        lca = lowest_common_ancestor<W>(T, root);
    }

    void euler_tour(tree<W> &T){
        int now = 0;
        function<void(int, int)> dfs = [&](int v, int p){
            in[v] = now++;
            for(edge<W> e : T[v]) if(e.to != p) dfs(e.to, v);
        }; dfs(root, -1);
    }

    int build(int k, vector<int> &vs, tree<W> &AT){
        sort(vs.begin(), vs.end(), [&](int i, int j){return in[i] < in[j];});
        stack<int> stk;
        stk.push(vs[0]);
        AT[vs[0]] = {};

        for(int i=0; i<k-1; i++){
            int r = lca.query(vs[i], vs[i+1]);

            if(r != vs[i]){
                int last = stk.top();
                stk.pop();
                while(!stk.empty()&&lca.depth[r]<lca.depth[stk.top()]){
                    AT[stk.top()].push_back(edge<W>(last));
                    last = stk.top();
                    stk.pop();
                }

                if(stk.empty()||stk.top()!=r){
                    stk.push(r);
                    vs.push_back(r);
                    AT[r] = {edge<W>(last)};
                }else{
                    AT[r].push_back(edge<W>(last));
                }
            }

            stk.push(vs[i+1]);
            AT[vs[i+1]] = {};
        }

        int r = stk.top();
        while(!stk.empty()){
            stk.pop();
            if(!stk.empty()){
                AT[stk.top()].push_back(edge<W>(r));
                r = stk.top();
            }
        }

        function<void(int, int)> dfs = [&](int v, int p){
            for(edge<int> e : AT[v]) if(e.to != p){
                AT[e.to].push_back(edge<int>(v));
                dfs(e.to, v);
            }
        }; dfs(r, -1);

        return r;
    }
};
#endif
