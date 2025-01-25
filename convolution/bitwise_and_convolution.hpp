#ifndef AND_CNV_HPP
#define AND_CNV_HPP

/*depend on*/
#include "../header.hpp"

/*start*/
template<typename T>
vector<T> bitwise_and_convolution(vector<T> a, vector<T> b){
    assert((int)a.size() == (int)b.size());
    int n = (int)a.size();

    for(int i=1; i<n; i<<=1){
        for(int j=0; j<n; j++){
            if((j & i) == 0){
                a[j] += a[j | i];
                b[j] += b[j | i];
            }
        }
    }

    vector<T> c(n);
    for(int i=0; i<n; i++) c[i] = a[i]*b[i];

    for(int i=1; i<n; i<<=1){
        for(int j=0; j<n; j++){
            if((j & i) == 0){
                c[j] -= c[j | i];
            }
        }
    }

    return c;
}
#endif