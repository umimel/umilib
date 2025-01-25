#ifndef MINPLS_CNV_HPP
#define MINPLS_CNV_HPP

/*depend on*/
#include "../header.hpp"

/*start*/
template<typename T, typename Compare = less<T>>
vector<pair<int, T>> monotone_minima(int h, int w, const function<T(int, int)> &f, const Compare &comp = Compare()){
    vector<pair<int, T>> dp(h);

    function<void(int, int, int, int)> dfs = [&](int top, int btm, int left, int right){
        if(top > btm) return;
        int line = (top + btm)/2;
        T min_val;
        int min_j = -1;
        for(int j=left; j<=right; j++){
            T cst = f(line, j);
            if(min_j==-1 || comp(cst, min_val)){
                min_val = cst;
                min_j = j;
            }
        }
        dp[line] = make_pair(min_j, min_val);
        dfs(top, line-1, left, min_j);
        dfs(line+1, btm, min_j, right);
    };

    dfs(0, h-1, 0, w-1);
    return dp;
}

// bが上に凸
vector<pair<int, ll>> min_plus_convolution(const vector<ll> &a, const vector<ll> &b){
    const int n = (int)a.size(), m = (int)b.size();
    function<ll(int, int)> f = [&](int i, int j){
        if(i < j || i-j >=m) return LINF;
        return a[j]+b[i-j];
    };

    return monotone_minima<ll>(n+m-1, n, f);
}
#endif