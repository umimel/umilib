#ifndef STATIC_RECTSUM_HPP
#define STATIC_RECTSUM_HPP

/*depend on*/
#include "../header.hpp"

/*start*/
template<typename T>
class undo_li_chao_tree{
private:
 
    struct line{
        T a, b;
    };
 
    int n = 1;
    vector<line> dat;
    const T TINF = std::numeric_limits<T>::max() / (T)2;
    line none = (line){0, -TINF};
    vector<vector<pair<int, line>>> his;
 
    T y(line f, int x){
        return f.a*x + f.b;
    }
 
    void add_line_sub(line f, int k, int l, int r){
        int m = (l + r)>>1;
        if(make_pair(dat[k].a, dat[k].b)==make_pair(none.a, none.b)){
            his.back().push_back({k, dat[k]});
            dat[k] = f;
            return;
        }
 
        bool left = (y(f, l) > y(dat[k], l));
        bool mid = (y(f, m) > y(dat[k], m));
        bool right = (y(f, r-1) > y(dat[k], r-1));
 
        if(left && right){
            his.back().push_back({k, dat[k]});
            dat[k] = f;
            return;
        }
 
        if(!left && !right) return;
 
        if(mid){
            his.back().push_back({k, dat[k]});
            swap(dat[k], f);
        }
 
        if(left != mid) add_line_sub(f, 2*k+1, l, m);
        else add_line_sub(f, 2*k+2, m, r);
 
        return;
    }
 
public:
 
    undo_li_chao_tree(int n_){
        while(n < n_) n*=2;
        dat.resize(2*n+1, none);
    }
 
    void add_line(T a, T b){
        his.push_back({});
        add_line_sub((line){a, b}, 0, 0, n);
 
        return;
    }
 
    T query(int x){
        int k = x+n-1;
        T ans = y(dat[k], x);
        while(k){
            k=(k-1)/2;
            ans = max(ans, y(dat[k], x));
        }
 
        return ans;
    }
 
    void undo(){
        if(his.empty()) return;
        for(auto [k, f] : his.back()) dat[k] = f;
        his.pop_back();
 
        return;
    }
};
#endif