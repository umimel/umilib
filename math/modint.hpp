#ifndef MODINT_HPP
#define MODINT_HPP

#include "../header.hpp"

/*start*/
template<long long mod>
class modint{
    long long x;
public:
    modint(long long x=0) : x((x%mod+mod)%mod) {}
    modint operator-() const { 
      return modint(-x);
    }
    bool operator==(const modint& a){
        if(x == a) return true;
        else return false;
    }
    bool operator==(long long a){
        if(x == a) return true;
        else return false;
    }
    bool operator!=(const modint& a){
        if(x != a) return true;
        else return false;
    }
    bool operator!=(long long a){
        if(x != a) return true;
        else return false;
    }
    modint& operator+=(const modint& a) {
        if ((x += a.x) >= mod) x -= mod;
        return *this;
    }
    modint& operator-=(const modint& a) {
        if ((x += mod-a.x) >= mod) x -= mod;
        return *this;
    }
    modint& operator*=(const  modint& a) {
        (x *= a.x) %= mod;
        return *this;
    }
    modint operator+(const modint& a) const {
        modint res(*this);
        return res+=a;
    }
    modint operator-(const modint& a) const {
        modint res(*this);
        return res-=a;
    }
    modint operator*(const modint& a) const {
        modint res(*this);
        return res*=a;
    }
    modint pow(long long t) const {
        if (!t) return 1;
        modint a = pow(t>>1);
        a *= a;
        if (t&1) a *= *this;
        return a;
    }
    // for prime mod
    modint inv() const {
        return pow(mod-2);
    }
    modint& operator/=(const modint& a) {
        return (*this) *= a.inv();
    }
    modint operator/(const modint& a) const {
        modint res(*this);
        return res/=a;
    }

    friend std::istream& operator>>(std::istream& is, modint& m) noexcept {
        is >> m.x;
        m.x %= mod;
        if (m.x < 0) m.x += mod;
        return is;
    }

    friend ostream& operator<<(ostream& os, const modint& m){
        os << m.x;
        return os;
    }
};
#endif