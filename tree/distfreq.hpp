#ifndef DISTFREQ_HPP
#define DISTFREQ_HPP

/*depend on*/
#include "../header.hpp"
#include "../graph/template.hpp"
#include "../../atcoder/convolution"

/*start*/
vector<long long> distfreq(tree<int> &T){
    int n = (int)T.size();
    vector<bool> centroid(n, false);
    vector<int> subtree_size(n, 0);
    vector<long long> ans(n, 0);

    function<int(int, int)> compute_subtree_size = [&](int v, int p){
        int c = 1;
        for(edge<int> e : T[v]){
            if(e.to==p || centroid[e.to]) continue;
            c += compute_subtree_size(e.to, v);
        }
        subtree_size[v] = c;
        return c;
    };

    function<pair<int, int>(int, int, int)> search_centroid = [&](int v, int p, int t){
        pair<int, int> res = {IINF, -1};
        int s = 1, m = 0;
        for(edge<int> e : T[v]){
            if(e.to==p || centroid[e.to]) continue;
            res = min(res, search_centroid(e.to, v, t));
            m = max(m, subtree_size[e.to]);
            s += subtree_size[e.to];
        }
        m = max(m, t-s);
        res = min(res, {m, v});
        return res;
    };

    function<void(int, int, int, vector<int>&)> compute_distance = [&](int v, int par, int d, vector<int> &dist){
        dist.push_back(d);
        for(edge<int> e : T[v]){
            if(e.to==par || centroid[e.to]) continue;
            compute_distance(e.to, v, d+1, dist);
        }
    };

    function<void(int v)> cent_dec = [&](int v){
        // search centroid s
        compute_subtree_size(v, -1);
        int s = search_centroid(v, -1, subtree_size[v]).second;
        centroid[s] = true;

        for(edge<int> e : T[s]){
            if(centroid[e.to]) continue;
            cent_dec(e.to);
        }

        
        vector<vector<ll>> freq;
        vector<pair<int, int>> max_dist;
        for(edge<int> e : T[s]){
            if(centroid[e.to]) continue;
            vector<int> dist;
            compute_distance(e.to, s, 1, dist);
            sort(all(dist));
            vector<ll> f;
            for(int d : dist){
                while((int)f.size()<d+1) f.push_back(0);
                f[d]++;
            }
            freq.push_back(f);
            max_dist.push_back({(int)f.size(), (int)freq.size()-1});
        }

        sort(all(max_dist));
        vector<ll> sum;
        sum.push_back(0);

        for(auto [siz, i] : max_dist){
            vector<ll> g = atcoder::convolution(freq[i], sum);
            for(int j=0; j<(int)g.size(); j++) ans[j]+=g[j];
            for(int j=0; j<(int)freq[i].size(); j++){
                if((int)sum.size()<j+1) sum.push_back(freq[i][j]);
                else sum[j]+=freq[i][j];
            }
        }

        for(int i=0; i<(int)sum.size(); i++) ans[i] += sum[i];

        centroid[s] = false;
    };

    cent_dec(0);

    return ans;
}
#endif