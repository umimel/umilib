#ifndef MV_HPP
#define MV_HPP

/*depend on*/
#include "../../header.hpp"

/*start*/
template<typename T>
struct micali_vazirani{
    struct mv_node {
        vector<int> preds;
        vector<int> pred_to;
        vector<int> hanging_bridges;
        int min_level, max_level, odd_level, even_level;
        int match, bud;
        int above, below;
        int ddfs_green, ddfs_red;
        bool deleted, visited;
        int number_preds;
        
        mv_node() : min_level(-1), max_level(-1), odd_level(-1), even_level(-1),
                    match(-1), bud(-1), above(-1), below(-1), 
                    ddfs_green(-1), ddfs_red(-1), deleted(false), visited(false), number_preds(0) {}
        
        bool outer() const { return even_level != -1; }
        bool inner() const { return odd_level != -1; }
    };

    struct mv_bridge {
        int n1, n2;
        int level;
        bool used;
        
        mv_bridge(int n1, int n2, int level) : n1(n1), n2(n2), level(level), used(false) {}
    };

    enum ddfs_result { DDFS_EMPTY, DDFS_PATH, DDFS_PETAL };

    int n;
    vector<mv_node> nodes;
    vector<vector<int>> adj;
    vector<vector<mv_bridge>> bridges;
    vector<vector<int>> levels;
    
    vector<int> green_stack, red_stack;
    vector<int> path_found;
    ddfs_result last_ddfs;
    
    int matchnum, bridgenum, todonum;
    int current_level;

    micali_vazirani(int n, vector<pair<int, int>> edges) : n(n) {
        nodes.resize(n);
        adj.resize(n);
        levels.resize(n + 1);
        bridges.resize(n + 1);
        
        matchnum = bridgenum = todonum = 0;
        current_level = 0;
        
        for(auto& e : edges) {
            adj[e.first].push_back(e.second);
            adj[e.second].push_back(e.first);
        }
        
        max_match();
    }

private:
    void max_match() {
        while(true) {
            if(!max_match_phase()) break;
        }
    }
    
    bool max_match_phase() {
        for(int i = 0; i < n; i++) {
            nodes[i].min_level = nodes[i].max_level = -1;
            nodes[i].odd_level = nodes[i].even_level = -1;
            nodes[i].preds.clear();
            nodes[i].pred_to.clear();
            nodes[i].hanging_bridges.clear();
            nodes[i].visited = false;
            nodes[i].number_preds = 0;
            nodes[i].ddfs_green = nodes[i].ddfs_red = -1;
            nodes[i].above = nodes[i].below = -1;
        }
        
        for(int i = 0; i <= current_level; i++) {
            levels[i].clear();
            bridges[i].clear();
        }
        
        bridgenum = todonum = 0;
        current_level = 0;
        
        for(int i = 0; i < n; i++) {
            if(nodes[i].match == -1 && !nodes[i].deleted) {
                nodes[i].even_level = 0;
                levels[0].push_back(i);
            }
        }
        
        return run_phases();
    }
    
    bool run_phases() {
        for(int i = 0; i <= n; i++) {
            if(!min_phase(i)) return false;
            if(!max_phase(i)) return false;
        }
        return false;
    }
    
    bool min_phase(int level) {
        if(level >= levels.size() || levels[level].empty()) return true;
        
        for(int u : levels[level]) {
            if(nodes[u].deleted) continue;
            
            for(int v : adj[u]) {
                if(nodes[v].deleted) continue;
                
                if(nodes[u].outer() && nodes[v].match != u) {
                    step_to(u, v, level);
                }
            }
        }
        
        return true;
    }
    
    bool max_phase(int level) {
        if(level >= bridges.size() || bridges[level].empty()) return true;
        
        for(auto& bridge : bridges[level]) {
            if(bridge.used) continue;
            
            ddfs_result result = ddfs(bridge.n1, bridge.n2);
            if(result == DDFS_PATH) {
                find_path(bridge.n1, bridge.n2);
                augment_path();
                remove_path();
                return false;
            }
        }
        
        return true;
    }
    
    ddfs_result ddfs(int n1, int n2) {
        green_stack.clear();
        red_stack.clear();
        
        green_stack.push_back(n1);
        red_stack.push_back(n2);
        
        nodes[n1].ddfs_green = 0;
        nodes[n2].ddfs_red = 0;
        
        while(!green_stack.empty() || !red_stack.empty()) {
            if(!green_stack.empty()) {
                if(step_into(green_stack, red_stack, true)) {
                    return DDFS_PATH;
                }
            }
            
            if(!red_stack.empty()) {
                if(step_into(red_stack, green_stack, false)) {
                    return DDFS_PATH;
                }
            }
        }
        
        return DDFS_EMPTY;
    }
    
    bool step_into(vector<int>& stack1, vector<int>& stack2, bool is_green) {
        if(stack1.empty()) return false;
        
        int u = stack1.back();
        
        for(int v : nodes[u].preds) {
            if(nodes[v].deleted) continue;
            
            int bud_v = bud_star(v);
            
            if(find(stack2.begin(), stack2.end(), bud_v) != stack2.end()) {
                return true;
            }
            
            if((is_green && nodes[bud_v].ddfs_green == -1) || 
               (!is_green && nodes[bud_v].ddfs_red == -1)) {
                stack1.push_back(bud_v);
                if(is_green) nodes[bud_v].ddfs_green = stack1.size() - 1;
                else nodes[bud_v].ddfs_red = stack1.size() - 1;
                return false;
            }
        }
        
        stack1.pop_back();
        return false;
    }
    
    void find_path(int n1, int n2) {
        path_found.clear();
        walk_down_path(n1, n2);
    }
    
    void walk_down_path(int start, int end) {
        path_found.push_back(start);
        
        int cur = start;
        while(cur != end) {
            bool found_next = false;
            for(int next : nodes[cur].preds) {
                if(nodes[next].deleted) continue;
                
                int bud_next = bud_star(next);
                if(bud_next == end || 
                   find(red_stack.begin(), red_stack.end(), bud_next) != red_stack.end()) {
                    path_found.push_back(next);
                    cur = next;
                    found_next = true;
                    break;
                }
            }
            
            if(!found_next) break;
        }
        
        if(cur != end) {
            path_found.push_back(end);
        }
    }
    
    void augment_path() {
        for(int i = 0; i < path_found.size() - 1; i += 2) {
            int u = path_found[i];
            int v = path_found[i + 1];
            nodes[u].match = v;
            nodes[v].match = u;
        }
        matchnum += path_found.size() / 2;
    }
    
    void remove_path() {
        for(int node : path_found) {
            nodes[node].deleted = true;
        }
    }
    
    int bud_star(int v) {
        return nodes[v].bud != -1 ? nodes[v].bud : v;
    }
    
    int tenacity(int u, int v) {
        if(nodes[u].match == v) {
            return nodes[u].odd_level + nodes[v].odd_level + 1;
        } else {
            return nodes[u].even_level + nodes[v].even_level + 1;
        }
    }
    
    void step_to(int u, int v, int level) {
        if(nodes[v].even_level == -1 && nodes[v].odd_level == -1) {
            if(nodes[v].match == -1) {
                bridges[level].push_back(mv_bridge(u, v, level));
                bridgenum++;
            } else {
                nodes[v].odd_level = level + 1;
                nodes[nodes[v].match].even_level = level + 2;
                if(level + 2 < levels.size()) {
                    levels[level + 2].push_back(nodes[v].match);
                }
                current_level = max(current_level, level + 2);
                
                nodes[v].preds.push_back(u);
                nodes[nodes[v].match].preds.push_back(v);
            }
        } else if(nodes[v].outer()) {
            bridges[level].push_back(mv_bridge(u, v, level));
            bridgenum++;
        }
    }

public:
    int max_matching_size() {
        return matchnum;
    }
    
    vector<pair<int, int>> max_matching() {
        vector<pair<int, int>> matching;
        for(int i = 0; i < n; i++) {
            if(nodes[i].match != -1 && i < nodes[i].match) {
                matching.push_back({i, nodes[i].match});
            }
        }
        return matching;
    }
};
#endif
