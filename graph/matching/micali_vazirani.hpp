#ifndef MV_HPP
#define MV_HPP

/*depend on*/
#include "../header.hpp"

/*start*/
template<typename T>
struct micali_vazirani{
    struct disjoint_set_union{
        vector<int> link;
        vector<int> directParent;
        vector<int> size;
        vector<int> groupRoot;

        void reset(int n) {
            link = vector<int>(n);
            size = vector<int>(n,1);
            iota(link.begin(), link.end(), 0);
            groupRoot = link;
            directParent = vector<int>(n,-1);
        }
        
        int find(int a) {
            return link[a] = (a == link[a] ? a : find(link[a]));
        }

        int operator[](const int& a) {
            return groupRoot[find(a)];
        }

        void linkTo(int a, int b) {
            assert(directParent[a] == -1);
            assert(directParent[b] == -1);
            directParent[a] = b;
            a = find(a);
            b = find(b);
            int gr = groupRoot[b];
            assert(a != b);
            
            if(size[a] > size[b])
                swap(a,b);
            link[b] = a;
            size[a] += size[b];
            groupRoot[a] = gr;
        }
    };

    enum edge_type{not_scanned, prop, bridge};
    struct edge{
        int to;
        int other;
        edge_type type;

        edge(int to, int other, edge_type type=not_scanned) : to(to), other(other), type(type){}
    };

    int n;
    vector<vector<edge>> G;
    vector<int> mate;

    vector<vector<int>> predecessors;
    vector<int> ddfsPredecessorsPtr;
    vector<int> removed;
    vector<int> evenlvl, oddlvl;
    disjoint_set_union bud;

    int minlvl(int u){
        return min(evenlvl[u], oddlvl[u]);
    }

    int tenacity(pair<int, int> e) {
        if(mate[e.first] == e.second){
            return oddlvl[e.first]+oddlvl[e.second]+1;
        }else{
            return evenlvl[e.first]+evenlvl[e.second]+1;
        }
    }

    int ddfs_move(vector<int>& stack1, const int color1, vector<int>& stack2, const int color2, vector<int>& support){
        int u = stack1.back();
        for(; ddfsPredecessorsPtr[u]<predecessors[u].size(); ddfsPredecessorsPtr[u]++) {
            int a = predecessors[u][ddfsPredecessorsPtr[u]];
            int v = bud[a];
            assert(removed[a] == removed[v]);
            if(removed[a]) continue;
            if(color[v] == 0) {
                stack1.push_back(v);
                support.push_back(v);
                childsInDDFSTree[u].push_back({a, v});
                color[v] = color1;
                return -1;
            }else if(v == stack2.back()){
                childsInDDFSTree[u].push_back({a,v});
            }
        }
        stack1.pop_back();

        if(stack1.size() == 0) {
            if(stack2.size() == 1) { //found bottleneck
                color[stack2.back()] = 0;
                return stack2.back();
            }
            //change colors
            assert(color[stack2.back()] == color2);
            stack1.push_back(stack2.back());
            color[stack1.back()] = color1;
            stack2.pop_back();
        }
        return -1;
    }
};
#endif