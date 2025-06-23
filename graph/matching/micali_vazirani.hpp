#ifndef MV_HPP
#define MV_HPP

/*depend on*/
#include "../../header.hpp"

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
    vector<int> color;
    vector<vector<pair<int, int>>> childsInDDFSTree;

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

    micali_vazirani(int n, vector<pair<int, int>> edges) : n(n) {
        G.resize(n);
        mate.resize(n, -1);
        predecessors.resize(n);
        ddfsPredecessorsPtr.resize(n);
        removed.resize(n);
        evenlvl.resize(n);
        oddlvl.resize(n);
        color.resize(n);
        childsInDDFSTree.resize(n);
        
        for(auto& e : edges) {
            G[e.first].push_back(edge(e.second, e.first));
            G[e.second].push_back(edge(e.first, e.second));
        }
        
        compute_maximum_matching();
    }

private:
    void compute_maximum_matching() {
        fill(mate.begin(), mate.end(), -1);
        
        while(true) {
            if(!build_bfs_tree()) break;
            find_augmenting_paths();
        }
    }
    
    bool build_bfs_tree() {
        fill(evenlvl.begin(), evenlvl.end(), -1);
        fill(oddlvl.begin(), oddlvl.end(), -1);
        fill(removed.begin(), removed.end(), 0);
        fill(color.begin(), color.end(), 0);
        
        for(int i = 0; i < n; i++) {
            predecessors[i].clear();
            ddfsPredecessorsPtr[i] = 0;
            childsInDDFSTree[i].clear();
        }
        
        bud.reset(n);
        
        queue<int> q;
        for(int i = 0; i < n; i++) {
            if(mate[i] == -1) {
                evenlvl[i] = 0;
                q.push(i);
            }
        }
        
        bool found_augmenting_path = false;
        
        while(!q.empty() && !found_augmenting_path) {
            int u = q.front();
            q.pop();
            
            for(auto& e : G[u]) {
                int v = e.to;
                
                if(evenlvl[u] != -1) {
                    if(mate[u] == v) continue;
                    
                    if(evenlvl[v] == -1 && oddlvl[v] == -1) {
                        if(mate[v] == -1) {
                            found_augmenting_path = true;
                        } else {
                            oddlvl[v] = evenlvl[u] + 1;
                            evenlvl[mate[v]] = oddlvl[v] + 1;
                            q.push(mate[v]);
                        }
                    }
                }
            }
        }
        
        return found_augmenting_path;
    }
    
    void find_augmenting_paths() {
        for(int i = 0; i < n; i++) {
            if(mate[i] == -1 && evenlvl[i] == 0) {
                vector<int> path;
                if(find_augmenting_path_from(i, path)) {
                    apply_augmenting_path(path);
                }
            }
        }
    }
    
    bool find_augmenting_path_from(int start, vector<int>& path) {
        path.clear();
        path.push_back(start);
        
        function<bool(int, vector<int>&)> dfs = [&](int u, vector<int>& current_path) -> bool {
            for(auto& e : G[u]) {
                int v = e.to;
                if(mate[u] == v) continue;
                
                if(mate[v] == -1 && evenlvl[v] == -1) {
                    current_path.push_back(v);
                    return true;
                }
                
                if(oddlvl[v] == evenlvl[u] + 1 && mate[v] != -1) {
                    current_path.push_back(v);
                    current_path.push_back(mate[v]);
                    if(dfs(mate[v], current_path)) return true;
                    current_path.pop_back();
                    current_path.pop_back();
                }
            }
            return false;
        };
        
        return dfs(start, path);
    }
    
    void apply_augmenting_path(const vector<int>& path) {
        for(int i = 0; i < path.size() - 1; i += 2) {
            mate[path[i]] = path[i + 1];
            mate[path[i + 1]] = path[i];
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

public:
    int max_matching_size() {
        int count = 0;
        for(int i = 0; i < n; i++) {
            if(mate[i] != -1 && i < mate[i]) count++;
        }
        return count;
    }
    
    vector<pair<int, int>> max_matching() {
        vector<pair<int, int>> matching;
        for(int i = 0; i < n; i++) {
            if(mate[i] != -1 && i < mate[i]) {
                matching.push_back({i, mate[i]});
            }
        }
        return matching;
    }
};
#endif
