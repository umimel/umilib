#ifndef LI_CHAO_TREE_HPP
#define LI_CHAO_TREE_HPP

/*depend on*/
#include "../header.hpp"

/*start*/
template<typename T>
struct static_li_chao_tree{
    struct line{T a, b;};

    int n;
    vector<line> dat;
    vector<T> x;
    const T TINF = std::numeric_limits<T>::max() / 2;
    line none = (line){0, TINF};

    static_li_chao_tree(vector<T> x_) : x(x_){
        n = 1;
        while(n < (int)x.size()) n*=2;
        dat.resize(2*n+1, none);
        while((int)x.size()<n) x.push_back(x.back()+1);
    }

    T f(line coef, T x){return coef.a*x + coef.b;}

    void add_line_sub(line coef, T k, T l, T r){
        T m = (l + r)/2;
        if(make_pair(dat[k].a, dat[k].b)==make_pair(none.a, none.b)){
            dat[k] = coef;
            return;
        }

        T lx = x[l], mx = x[m], rx = x[r-1];
        bool left = (f(coef, lx) < f(dat[k], lx));
        bool mid = (f(coef, mx) < f(dat[k], mx));
        bool right = (f(coef, rx) < f(dat[k], rx));

        if(left && right){
            dat[k] = coef;
            return;
        }

        if(!left && !right){return;}

        if(mid) swap(dat[k], coef);

        if(left != mid){
            add_line_sub(coef, 2*k+1, l, m);
        }else{
            add_line_sub(coef, 2*k+2, m, r);
        }
    }

    void add_line(T a, T b){
        add_line_sub((line){a, b}, 0, 0, n);
        return;
    }

    T query(T k){
        k = lower_bound(x.begin(), x.end(), k)-x.begin();
        T kx = x[k];
        k += n-1;
        T s = TINF;
        if(make_pair(dat[k].a, dat[k].b)!=make_pair(none.a, none.b)){
            s = f(dat[k], kx);
        }
        while(k>0){
            k=(k-1)/2;
            if(make_pair(dat[k].a, dat[k].b)!=make_pair(none.a, none.b)){
                s = min(s, f(dat[k], kx));
            }
        }
        return s;
    }
};
#endif