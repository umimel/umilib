#ifndef ComparabilityDistOracle_HPP
#define ComparabilityDistOracle_HPP

/*depends on*/
#include "../../header.hpp"

/*start*/
template<typename T>
class comparability_distoracle{
private:
    int n;
    vector<T> lmin, rmin, w;
    vector<pair<int, int>> itv;
    const T inf = numeric_limits<T>::max()/4;

public:
    comparability_distoracle(int n_, vector<pair<int, int>> &itv_, vector<T> &w_){
        n = n_;
        itv.resize(n);
        w.resize(n);
        lmin.resize(2*n+2, inf);
        rmin.resize(2*n+2, inf);
        
        for(int i=0; i<n; i++){
            auto [l, r] = itv_[i];
            w[i] = w_[i];
            itv[i] = {l, r};
            lmin[r] = min(lmin[r], w[i]);
            rmin[l] = min(rmin[l], w[i]);
        }
        
        for(int i=1; i<=2*n+1; i++) lmin[i] = min(lmin[i], lmin[i-1]);
        for(int i=2*n; i>=0; i--) rmin[i] = min(rmin[i], rmin[i+1]);
    }

    T query(int s, int t){
        if(itv[t].first < itv[s].first) swap(s, t);
        else if(itv[s].first == itv[t].first && itv[s].second < itv[t].second) swap(s, t);

        /** 1 hop */
        if(itv[s].second < itv[t].first) return w[s] + w[t];

        /** 2 hops */
        if(itv[t].second <= itv[s].second){
            T ans = inf;
            ans = min(ans, w[s] + min(lmin[itv[s].first-1], rmin[itv[s].second+1]) + w[t]);
            return (ans == inf ? -1 : ans);
        }

        /** 3 hops */
        T ans = inf;
        ans = min(ans, w[s] + min(lmin[itv[s].first-1], rmin[itv[t].second+1]) + w[t]);
        ans = min(ans, w[s] + rmin[itv[s].second+1] + lmin[itv[t].first-1] + w[t]);
        return (ans == inf ? -1 : ans);
    }
};
#endif