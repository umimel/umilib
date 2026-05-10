#ifndef RUNLENGTH_HPP
#define RUNLENGTH_HPP

/*depend on*/
#include "../header.hpp"

/*start*/
template<typename T>
vector<pair<T, int>> run_length_compression(vector<T>& A){
    if(A.empty()) return {};
    vector<pair<T, int>> rlc = {{A[0], 1}};
    for(int i=1; i<(int)A.size(); i++){
        if(A[i-1] == A[i]) rlc.back().second++;
        else rlc.emplace_back(A[i], 1);
    }
    return rlc;
}

vector<pair<char, int>> run_length_compression(string& S){
    if(S.empty()) return {};
    vector<pair<char, int>> rlc = {{S[0], 1}};
    for(int i=1; i<(int)S.size(); i++){
        if(S[i-1] == S[i]) rlc.back().second++;
        else rlc.emplace_back(S[i], 1);
    }
    return rlc;
}
#endif