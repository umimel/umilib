#ifndef OR_CNV_HPP
#define OR_CNV_HPP

/*depend on*/
#include "../header.hpp"

/*start*/
template<typename T>
vector<T> bitwise_or_convolution(vector<T> a, vector<T> b){
    assert((int)a.size() == (int)b.size());
    int n = (int)a.size();

    for(int i=1; i<n; i<<=1){
        for(int j=0; j<n; j++){
            if((j & i) == 0){
                a[j | i] += a[j];
                b[j | i] += b[j];
            }
        }
    }

    vector<T> c(n);
    for(int i=0; i<n; i++) c[i] = a[i]*b[i];

    for(int i=1; i<n; i<<=1){
        for(int j=0; j<n; j++){
            if((j & i) == 0){
                c[j | i] -= c[j];
            }
        }
    }

    return c;
}
#endif