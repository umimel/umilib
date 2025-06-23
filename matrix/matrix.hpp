#ifndef matrix_HPP
#define matrix_HPP

#include<bits/stdc++.h>
#include "../header.hpp"

/*start*/
template<typename T>
struct matrix{
    vector<vector<T>> A;

    matrix(){}
    matrix(size_t n, size_t m) : A(n, vector<T>(m, 0)){}
    matrix(size_t n) : A(n, vector<T>(n, 0)){};

    size_t height() const{return (A.size());}
    size_t width() const{return (A[0].size());}

    inline const vector<T> &operator[](int k) const{return (A.at(k));}

    inline vector<T> &operator[](int k){return (A.at(k));}

    static matrix I(size_t n){
        matrix mat(n);
        for(int i=0; i<n; i++) mat[i][i] = 1;
        return (mat);
    }

    matrix &operator+=(const matrix &B){
        size_t n = height(), m = width();
        assert(n == B.height() && m == B.width());
        for(int i=0; i<n; i++)for(int j=0; j<m; j++) (*this)[i][j] += B[i][j];
        return (*this);
    }

    matrix &operator-=(const matrix &B){
        size_t n = height(), m = width();
        assert(n == B.height() && m == B.width());
        for(int i=0; i<n; i++)for(int j=0; j<m; j++) (*this)[i][j] -= B[i][j];
        return (*this);
    }

    matrix &operator*=(const matrix &B){
        size_t n = height(), m = B.width(), p = width();
        assert(p == B.height());
        vector<vector<T>> C(n, vector<T>(m, 0));
        for(int i = 0; i < n; i++)
            for(int k = 0; k < p; k++)
                for(int j = 0; j < m; j++)
                    C[i][j] = (C[i][j] + (*this)[i][k] * B[k][j]);
        A.swap(C);
        return (*this);
    }

    matrix &operator^=(long long k){
        matrix B = matrix::I(height());
        while(k > 0) {
            if(k & 1) B *= *this;
            *this *= *this;
            k >>= 1LL;
        }
        A.swap(B.A);
        return (*this);
    }

    matrix operator+(const matrix &B) const{
        return (matrix(*this) += B);
    }

    matrix operator-(const matrix &B) const{
        return (matrix(*this) -= B);
    }

    matrix operator*(const matrix &B) const{
        return (matrix(*this) *= B);
    }

    matrix operator^(const long long k) const{
        return (matrix(*this) ^= k);
    }

    friend ostream &operator<<(ostream &os, matrix &p){
        size_t n = p.height(), m = p.width();
        for(int i=0; i<n; i++){
            os << "[";
            for(int j=0; j<m; j++){
                os << p[i][j] << (j + 1 == m ? "]\n" : ",");
            }
        }
        return (os);
    }


    T determinant(){
        matrix B(*this);
        assert(width() == height());
        T ret = 1;
        for(int i=0; i<width(); i++) {
            int idx = -1;
            for(int j=i; j<width(); j++) {
                if(B[j][i] != 0) idx = j;
            }
            if(idx == -1) return (0);
            if(i != idx) {
                ret *= -1;
                swap(B[i], B[idx]);
            }
            ret *= B[i][i];
            T vv = B[i][i];
            for(int j=0; j<width(); j++) {
                B[i][j] /= vv;
            }
            for(int j=i+1; j<width(); j++) {
                T a = B[j][i];
                for(int k=0; k<width(); k++) {
                    B[j][k] -= B[i][k] * a;
                }
            }
        }
        return (ret);
    }
};
#endif
