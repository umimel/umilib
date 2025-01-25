#ifndef Z_ALGO_HPP
#define Z_ALGO_HPP

#include<bits/stdc++.h>
#include "../header.hpp"

/*start*/
vector<int> z_algorithm(string s){
    int n = (int)s.size();
    vector<int> z(n);
    z[0] = n;
    int i=1, j=0;
    while(i < n){
        while(i+j<n && s[j]==s[i+j]) j++;
        z[i] = j;

        if(j == 0){
            i++;
            continue;
        }

        int k=1;
        while(k < j && k+z[k]<j){
            z[i+k] = z[k];
            k++;
        }
        i += k;
        j -= k;
    }

    return z;
}

template<typename T>
vector<int> z_algorithm(vector<T> s){
    int n = (int)s.size();
    vector<int> z(n);
    z[0] = n;
    int i=1, j=0;
    while(i < n){
        while(i+j<n && s[j]==s[i+j]) j++;
        z[i] = j;

        if(j == 0){
            i++;
            continue;
        }

        int k=1;
        while(k < j && k+z[k]<j){
            z[i+k] = z[k];
            k++;
        }
        i += k;
        j -= k;
    }

    return z;
}
#endif