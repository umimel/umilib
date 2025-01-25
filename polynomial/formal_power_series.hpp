#ifndef FPS_HPP
#define FPS_HPP

/*depend on*/
#include "../header.hpp"
#include "../../atcoder/convolution"

/*start*/
enum Mode {
	FAST = 1,
	NAIVE = -1,
};

template <class T, Mode mode = FAST>
struct formal_power_series : std::vector<T> {
	using std::vector<T>::vector;
	using std::vector<T>::size;
	using std::vector<T>::resize;
	using F = formal_power_series;

	F &operator+=(const F &g) {
		for(int i = 0; i < int(std::min((*this).size(), g.size())); i++) (*this)[i] += g[i];
		return *this;
	}

	F &operator+=(const T &t) {
		assert(int((*this).size()));
		(*this)[0] += t;
		return *this;
	}

	F &operator-=(const F &g) {
		for(int i = 0; i < int(std::min((*this).size(), g.size())); i++) (*this)[i] -= g[i];
		return *this;
	}

	F &operator-=(const T &t) {
		assert(int((*this).size()));
		(*this)[0] -= t;
		return *this;
	}

	F &operator*=(const T &t) {
		for(int i = 0; i < int((*this).size()); ++i) (*this)[i] *= t;
		return *this;
	}

	F &operator/=(const T &t) {
		T div = t.inv();
		for(int i = 0; i < int((*this).size()); ++i) (*this)[i] *= div;
		return *this;
	}

	F &operator>>=(const int sz) const {
		assert(sz >= 0);
		int n = (*this).size();
		(*this).erase((*this).begin(), (*this).begin() + std::min(sz, n));
		(*this).resize(n);
		return *this;
	}

	F &operator<<=(const int sz) const {
		assert(sz >= 0);
		int n = (*this).size();
		(*this).insert((*this).begin(), (*this).begin() + sz, 0);
		(*this).resize(n);
		return *this;
	}

	F &operator%=(const F &g) { return *this -= *this / g * g; }

	F &operator=(const std::vector<T> &v) {
		int n = (*this).size();
		for(int i = 0; i < n; ++i) (*this)[i] = v[i];
		return *this;
	}

	F operator-() const {
		F ret = *this;
		return ret * -1;
	}

	F &operator*=(const F &g) {
		if(mode == FAST) {
			int n = (*this).size();
			auto tmp = atcoder::convolution(*this, g);
			for(int i = 0; i < n; ++i) (*this)[i] = tmp[i];
			return *this;
		} else {
			int n = (*this).size(), m = g.size();
			for(int i = n - 1; i >= 0; --i) {
				(*this)[i] *= g[0];
				for(int j = 1; j < std::min(i + 1, m); j++)
					(*this)[i] += (*this)[i - j] * g[j];
			}
			return *this;
		}
	}

	F &operator/=(const F &g) {
		if(mode == FAST) {
			int n = (*this).size();
			(*this) = atcoder::convolution(*this, g.inv());
			return *this;
		} else {
			assert(g[0] != T(0));
			T ig0 = g[0].inv();
			int n = (*this).size(), m = g.size();
			for(int i = 0; i < n; ++i) {
				for(int j = 1; j < std::min(i + 1, m); ++j)
					(*this)[i] -= (*this)[i - j] * g[j];
				(*this)[i] *= ig0;
			}
			return *this;
		}
	}

	F operator+(const F &g) const { return F(*this) += g; }

	F operator+(const T &t) const { return F(*this) += t; }

	F operator-(const F &g) const { return F(*this) -= g; }

	F operator-(const T &t) const { return F(*this) -= t; }

	F operator*(const F &g) const { return F(*this) *= g; }

	F operator*(const T &t) const { return F(*this) *= t; }

	F operator/(const F &g) const { return F(*this) /= g; }

	F operator/(const T &t) const { return F(*this) /= t; }

	F operator%(const F &g) const { return F(*this) %= g; }

	T eval(const T &t) const {
		int n = (*this).size();
		T res = 0, tmp = 1;
		for(int i = 0; i < n; ++i) res += (*this)[i] * tmp, tmp *= t;
		return res;
	}

	F inv(int deg = -1) const {
		int n = (*this).size();
		assert(mode == FAST and n and (*this)[0] != 0);
		if(deg == -1) deg = n;
		assert(deg > 0);
		F res{(*this)[0].inv()};
		while(int(res.size()) < deg) {
			int m = res.size();
			F f((*this).begin(), (*this).begin() + std::min(n, m * 2)), r(res);
			f.resize(m * 2), atcoder::internal::butterfly(f);
			r.resize(m * 2), atcoder::internal::butterfly(r);
			for(int i = 0; i < m * 2; ++i) f[i] *= r[i];
			atcoder::internal::butterfly_inv(f);
			f.erase(f.begin(), f.begin() + m);
			f.resize(m * 2), atcoder::internal::butterfly(f);
			for(int i = 0; i < m * 2; ++i) f[i] *= r[i];
			atcoder::internal::butterfly_inv(f);
			T iz = T(m * 2).inv();
			iz *= -iz;
			for(int i = 0; i < m; ++i) f[i] *= iz;
			res.insert(res.end(), f.begin(), f.begin() + m);
		}
		res.resize(deg);
		return res;
	}

	F &diff_inplace() {
		int n = (*this).size();
		for(int i = 1; i < n; ++i) (*this)[i - 1] = (*this)[i] * i;
		(*this)[n - 1] = 0;
		return *this;
	}
	F diff() const { F(*this).diff_inplace(); }

	F &integral_inplace() {
		int n = (*this).size(), mod = T::mod();
		std::vector<T> inv(n);
		{
			inv[1] = 1;
			for(int i = 2; i < n; ++i)
				inv[i] = T(mod) - inv[mod % i] * (mod / i);
		}
		for(int i = n - 2; i >= 0; --i) (*this)[i + 1] = (*this)[i] * inv[i + 1];
		(*this)[0] = 0;
		return *this;
	}
	F integral() const { return F(*this).integral_inplace(); }

	F &log_inplace() {
		int n = (*this).size();
		assert(n and (*this)[0] == 1);
		F f_inv = (*this).inv();
		(*this).diff_inplace();
		(*this) *= f_inv;
		(*this).integral_inplace();
		return *this;
	}
	F log() const { return F(*this).log_inplace(); }

	F &deriv_inplace() {
		int n = (*this).size();
		assert(n);
		for(int i = 2; i < n; ++i) (*this)[i] *= i;
		(*this).erase((*this).begin());
		(*this).push_back(0);
		return *this;
	}
	F deriv() const { return F(*this).deriv_inplace(); }

	F &exp_inplace() {
		int n = (*this).size();
		assert(n and (*this)[0] == 0);
		F g{1};
		(*this)[0] = 1;
		F h_drv((*this).deriv());
		for(int m = 1; m < n; m *= 2) {
			F f((*this).begin(), (*this).begin() + m);
			f.resize(2 * m), atcoder::internal::butterfly(f);
			auto mult_f = [&](F &p) {
				p.resize(2 * m);
				atcoder::internal::butterfly(p);
				for(int i = 0; i < 2 * m; ++i) p[i] *= f[i];
				atcoder::internal::butterfly_inv(p);
				p /= 2 * m;
			};
			if(m > 1) {
				F g_(g);
				g_.resize(2 * m), atcoder::internal::butterfly(g_);
				for(int i = 0; i < 2 * m; ++i) g_[i] *= g_[i] * f[i];
				atcoder::internal::butterfly_inv(g_);
				T iz = T(-2 * m).inv();
				g_ *= iz;
				g.insert(g.end(), g_.begin() + m / 2, g_.begin() + m);
			}
			F t((*this).begin(), (*this).begin() + m);
			t.deriv_inplace();
			{
				F r{h_drv.begin(), h_drv.begin() + m - 1};
				mult_f(r);
				for(int i = 0; i < m; ++i) t[i] -= r[i] + r[m + i];
			}
			t.insert(t.begin(), t.back());
			t.pop_back();
			t *= g;
			F v((*this).begin() + m, (*this).begin() + std::min(n, 2 * m));
			v.resize(m);
			t.insert(t.begin(), m - 1, 0);
			t.push_back(0);
			t.integral_inplace();
			for(int i = 0; i < m; ++i) v[i] -= t[m + i];
			mult_f(v);
			for(int i = 0; i < std::min(n - m, m); ++i)
				(*this)[m + i] = v[i];
		}
		return *this;
	}
	F exp() const { return F(*this).exp_inplace(); }

	F &pow_inplace(long long k) {
		int n = (*this).size(), l = 0;
		assert(k >= 0);
		if(!k) {
			for(int i = 0; i < n; ++i) (*this)[i] = !i;
			return *this;
		}
		while(l < n and (*this)[l] == 0) ++l;
		if(l > (n - 1) / k or l == n) return *this = F(n);
		T c = (*this)[l];
		(*this).erase((*this).begin(), (*this).begin() + l);
		(*this) /= c;
		(*this).log_inplace();
		(*this).resize(n - l * k);
		(*this) *= k;
		(*this).exp_inplace();
		(*this) *= c.pow(k);
		(*this).insert((*this).begin(), l * k, 0);
		return *this;
	}
	F pow(const long long k) const { return F(*this).pow_inplace(); }
};
#endif