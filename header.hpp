#ifndef HEADER_HPP
#define HEADER_HPP

/*start*/
#include<bits/stdc++.h>
using namespace std;
using ll = long long;
using pll = pair<ll, ll>;
#define drep(i, cc, n) for (ll i = (cc); i <= (n); ++i)
#define rep(i, n) drep(i, 0, n - 1)
#define all(a) (a).begin(), (a).end()
#define pb push_back
#define fi first
#define se second
mt19937_64 rng(chrono::system_clock::now().time_since_epoch().count());
const ll MOD1000000007 = 1000000007;
const ll MOD998244353 = 998244353;
const ll MOD[3] = {999727999, 1070777777, 1000000007};
const ll LINF = 1LL << 60LL;
const int IINF = (1 << 30) - 1;
#endif