#ifndef XOR_CNV_HPP
#define XOR_CNV_HPP

/*depend on*/
#include "../header.hpp"

/*start*/
template <typename T>
void fwt(vector<T>& f) {
    int n = f.size();
    for (int i = 1; i < n; i <<= 1) {
        for (int j = 0; j < n; j++) {
            if ((j & i) == 0) {
                T x = f[j], y = f[j | i];
                f[j] = x + y, f[j | i] = x - y;
            }
        }
    }
}
template <typename T>
void ifwt(vector<T>& f) {
    int n = f.size();
    for (int i = 1; i < n; i <<= 1) {
        for (int j = 0; j < n; j++) {
            if ((j & i) == 0) {
                T x = f[j], y = f[j | i];
                f[j] = (x + y) / T(2), f[j | i] = (x - y) / T(2);
            }
        }
    }
}

template<typename T>
vector<T> bitwise_xor_convolution(vector<T> a, vector<T> b){
    int n = (int)a.size();

    for(int i=0; i<n; i++) a[i]*=T(2), b[i]*=T(2);

    fwt(a);
    fwt(b);

    vector<T> c(n);
    for(int i=0; i<n; i++) c[i] = a[i]*b[i]/T(4);

    ifwt(c);

    return c;
}
#endif