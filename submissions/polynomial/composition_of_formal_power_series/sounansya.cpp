#ifndef FPS_998244353_HPP
#define FPS_998244353_HPP
#include <bits/stdc++.h>
#if defined(__ARM_NEON)
#include <arm_neon.h>
#elif defined(__x86_64__) || defined(__i386__)
#include <immintrin.h>
#endif

#include <atcoder/convolution>
#include <atcoder/modint>

template <class T = atcoder::modint998244353>
struct FormalPowerSeries : std::vector<T> {
    static_assert(std::is_same_v<T, atcoder::modint998244353>,
                  "FormalPowerSeries supports only mod 998244353");
    using base = std::vector<T>;
    using F = FormalPowerSeries;
    using base::base;

    FormalPowerSeries() = default;
    FormalPowerSeries(base v) : base(std::move(v)) {}
    F& operator=(base v) {
        base::operator=(std::move(v));
        return *this;
    }
    F& operator=(std::initializer_list<T> v) {
        base::operator=(v);
        return *this;
    }

    static F multiply(const F& a, const F& b) {
        if (a.empty() || b.empty()) return {};
        return atcoder::convolution(static_cast<const base&>(a), static_cast<const base&>(b));
    }

    F operator-() const {
        F r = *this;
        for (T& x : r) x = -x;
        return r;
    }
    F& operator+=(const F& g) {
        if (this->size() < g.size()) this->resize(g.size());
        for (size_t i = 0; i < g.size(); ++i) (*this)[i] += g[i];
        return *this;
    }
    F& operator-=(const F& g) {
        if (this->size() < g.size()) this->resize(g.size());
        for (size_t i = 0; i < g.size(); ++i) (*this)[i] -= g[i];
        return *this;
    }
    F& operator+=(T x) {
        if (this->empty()) this->resize(1);
        (*this)[0] += x;
        return *this;
    }
    F& operator-=(T x) { return *this += -x; }
    F& operator*=(const F& g) { return *this = multiply(*this, g); }
    F& operator*=(T x) {
        for (T& a : *this) a *= x;
        return *this;
    }
    F& operator/=(const F& g) { return *this = divmod(g).first; }
    F& operator%=(const F& g) { return *this = divmod(g).second; }
    F& operator/=(T x) { return *this *= x.inv(); }

    F& operator<<=(int d) {
        assert(d >= 0);
        const int n = (int)this->size();
        if (d >= n) return *this = F(n);
        this->insert(this->begin(), d, T(0));
        this->resize(n);
        return *this;
    }
    F& operator>>=(int d) {
        assert(d >= 0);
        const int n = (int)this->size();
        if (d >= n) return *this = F(n);
        this->erase(this->begin(), this->begin() + d);
        this->resize(n);
        return *this;
    }

    F operator+(const F& g) const { return F(*this) += g; }
    F operator-(const F& g) const { return F(*this) -= g; }
    F operator*(const F& g) const { return multiply(*this, g); }
    F operator/(const F& g) const { return divmod(g).first; }
    F operator%(const F& g) const { return divmod(g).second; }
    F operator+(T x) const { return F(*this) += x; }
    F operator-(T x) const { return F(*this) -= x; }
    F operator*(T x) const { return F(*this) *= x; }
    F operator/(T x) const { return F(*this) /= x; }
    F operator<<(int d) const { return F(*this) <<= d; }
    F operator>>(int d) const { return F(*this) >>= d; }

    F pre(int n) const {
        assert(n >= 0);
        return F(this->begin(), this->begin() + std::min<int>(n, this->size()));
    }

    F div_series(const F& g, int deg = -1) const {
        if (deg < 0) deg = (int)this->size();
        if (deg == 0) return {};
        F r = multiply(*this, g.inv(deg)).pre(deg);
        r.resize(deg);
        return r;
    }

    F inv(int deg = -1) const {
        if (deg < 0) deg = (int)this->size();
        assert(deg >= 0);
        if (deg == 0) return {};
        assert(!this->empty() && (*this)[0] != T(0));
        F g{(*this)[0].inv()};
        auto terms = sparse_terms(deg, 32);
        if (terms.size() <= 32) {
            g.resize(deg);
            for (int i = 1; i < deg; ++i) {
                for (auto [j, x] : terms) {
                    if (j > i) break;
                    g[i] -= x * g[i - j];
                }
                g[i] *= g[0];
            }
            return g;
        }
        g.resize(std::min(deg, 32));
        for (int i = 1; i < (int)g.size(); ++i)
            g[i] =
                -g[0] * dot(this->data() + 1, g.data() + i - 1, std::min<int>(i, this->size() - 1));
        while ((int)g.size() < deg) {
            const int m = (int)g.size(), n = 2 * m;
            if (deg - m <= 32 && m >= 32) {
                g.resize(deg);
                for (int i = m; i < deg; ++i) {
                    const int count = std::min<int>(i, this->size() - 1);
                    g[i] = -g[0] * dot(this->data() + 1, g.data() + i - 1, count);
                }
                break;
            }
            base f = this->pre(n), r = g;
            f.resize(n);
            r.resize(n);
            atcoder::internal::butterfly(f);
            atcoder::internal::butterfly(r);
            for (int i = 0; i < n; ++i) f[i] *= r[i];
            atcoder::internal::butterfly_inv(f);
            std::fill(f.begin(), f.begin() + m, T(0));
            atcoder::internal::butterfly(f);
            for (int i = 0; i < n; ++i) f[i] *= r[i];
            atcoder::internal::butterfly_inv(f);
            const T inv = T::raw(T::mod() - (T::mod() - 1) / n);
            const T scale = -inv * inv;
            g.resize(n);
            for (int i = m; i < n; ++i) g[i] = f[i] * scale;
        }
        return g.pre(deg);
    }

    F sqrt(int deg = -1) const {
        if (deg < 0) deg = (int)this->size();
        assert(deg >= 0);
        F f = pre(deg);
        f.trim();
        int first = 0;
        while (first < (int)f.size() && f[first] == T(0)) ++first;
        if (first == (int)f.size()) return F(deg);
        if (first & 1) return {};
        const T constant = mod_sqrt(f[first]);
        if (constant == T(0)) return {};
        if (first + 1 == (int)f.size()) {
            F result(deg);
            result[first / 2] = constant;
            return result;
        }
        const int shift = first / 2, target = deg - shift;
        f.erase(f.begin(), f.begin() + first);
        f.resize(target);
        const T half = T(2).inv(), scale = half / constant;
        F g(std::min(target, 32));
        g[0] = constant;
        for (int i = 1; i < (int)g.size(); ++i) {
            T sum = 0;
            for (int j = 1; j < i; ++j) sum += g[j] * g[i - j];
            g[i] = (f[i] - sum) * scale;
        }
        F h;
        base cache;
        if ((int)g.size() < target) {
            h = g.pre(16).inv(16);
            cache = h;
            cache.resize(32);
            atcoder::internal::butterfly(cache);
        }
        while ((int)g.size() < target) {
            const int m = (int)g.size(), n = 2 * m;
            if (target - m <= 32) {
                g.resize(target);
                for (int i = m; i < target; ++i)
                    g[i] = (f[i] - dot(g.data() + 1, g.data() + i - 1, i - 1)) * scale;
                break;
            }
            assert(n <= (1 << 23));
            const T in = T::raw(T::mod() - (T::mod() - 1) / n);
            base a = g, error(n);
            a.resize(n);
            atcoder::internal::butterfly(a);
            inverse_extend(h, a, cache);
            cache = h;
            cache.resize(n);
            atcoder::internal::butterfly(cache);
            for (int i = 0; i < n; ++i) error[i] = a[i] * a[i];
            atcoder::internal::butterfly_inv(error);
            std::fill(error.begin(), error.begin() + m, T(0));
            for (int i = m; i < n; ++i) error[i] = (i < target ? f[i] : T(0)) - error[i] * in;
            atcoder::internal::butterfly(error);
            for (int i = 0; i < n; ++i) error[i] *= cache[i] * half;
            atcoder::internal::butterfly_inv(error);
            g.resize(std::min(n, target));
            for (int i = m; i < (int)g.size(); ++i) g[i] = error[i] * in;
        }
        g.resize(target);
        g.insert(g.begin(), shift, T(0));
        return g;
    }

    static T bostan_mori(F p, F q, long long k) {
        assert(k >= 0 && !q.empty() && q[0] != T(0));
        p.trim();
        q.trim();
        if (p.empty()) return 0;
        auto coefficient = [](const base& p, const base& q, int k) {
            F inverse(q.begin(), q.begin() + std::min<int>(k + 1, q.size()));
            inverse = inverse.inv(k + 1);
            return dot(p.data(), inverse.data() + k, std::min<int>(k + 1, p.size()));
        };
        if (k < 32) return coefficient(p, q, k);
        T answer = 0;
        if (p.size() >= q.size()) {
            auto [quotient, remainder] = p.divmod(q);
            if (k < (long long)quotient.size()) answer = quotient[k];
            p = std::move(remainder);
        }
        if (p.empty()) return answer;
        if (q.size() == 2) return answer + p[0] / q[0] * (-q[1] / q[0]).pow(k);
        const int d = (int)q.size() - 1;
        if (k < d) return answer + coefficient(p, q, k);
        int n = 1;
        while (n < d) n <<= 1;
        assert(n <= (1 << 22));
        const T half = T(2).inv(), inv = T(n).inv();
        const T step = T(3).pow((T::mod() - 1) / (2 * n));
        const auto& roots = inverse_ntt_roots(2 * n);
        base powers(n), a = p, b = q;
        T power = inv, lead = q.back();
        for (T& x : powers) x = power, power *= step;
        a.resize(2 * n);
        b.resize(2 * n);
        atcoder::internal::butterfly(a);
        atcoder::internal::butterfly(b);
        auto doubling = [&](base& v, T leading) {
            base odd = v;
            atcoder::internal::butterfly_inv(odd);
            odd[0] -= T(2 * n) * leading;
            for (int i = 0; i < n; ++i) odd[i] *= powers[i];
            atcoder::internal::butterfly(odd);
            v.insert(v.end(), odd.begin(), odd.end());
        };
        while (k) {
            for (int i = 0; i < n; ++i) {
                T u = a[2 * i] * b[2 * i + 1];
                T v = a[2 * i + 1] * b[2 * i];
                a[i] = (k & 1 ? (u - v) * roots[2 * i] : u + v) * half;
                b[i] = b[2 * i] * b[2 * i + 1];
            }
            a.resize(n);
            b.resize(n);
            lead = (d & 1 ? -lead : lead) * lead;
            k >>= 1;
            if (k < d) break;
            doubling(a, T(0));
            doubling(b, d == n ? lead : T(0));
        }
        if (k) {
            intt(a);
            intt(b);
            if (d == n) b[0] -= lead;
            return answer + coefficient(a, b, k);
        }
        T numerator = 0, denominator = 0;
        for (int i = 0; i < n; ++i) numerator += a[i], denominator += b[i];
        if (d == n) denominator -= T(n) * lead;
        return answer + numerator / denominator;
    }

    static T linear_recurrence(const base& initial, const base& coefficients, long long k) {
        assert(k >= 0 && initial.size() == coefficients.size());
        const int d = (int)initial.size();
        if (k < d) return initial[k];
        if (d == 0) return 0;
        F q(d + 1);
        q[0] = 1;
        for (int i = 0; i < d; ++i) q[i + 1] = -coefficients[i];
        q.trim();
        if (q.size() == 1) return 0;
        F p = multiply(F(initial), q).pre(d);
        return bostan_mori(std::move(p), std::move(q), k);
    }

    F compose(const F& inner, int deg = -1) const {
        if (deg < 0) deg = (int)this->size();
        assert(deg >= 0 && (inner.empty() || inner[0] == T(0)));
        if (deg == 0) return {};
        F f = pre(deg), g = inner.pre(deg);
        f.trim();
        g.trim();
        if (f.size() <= 1 || g.size() <= 1) {
            F result(deg);
            if (!f.empty()) result[0] = f[0];
            return result;
        }
        int first = 1;
        while (g[first] == T(0)) ++first;
        if (first + 1 == (int)g.size()) {
            F result(deg);
            T power = 1;
            for (int i = 0; i < (int)f.size() && (long long)i * first < deg; ++i)
                result[i * first] = f[i] * power, power *= g[first];
            return result;
        }
        if (deg <= 32 || f.size() <= 8) {
            F result;
            for (int i = (int)f.size() - 1; i >= 0; --i) {
                result = multiply(result, g).pre(deg);
                result += f[i];
            }
            result.resize(deg);
            return result;
        }
        int size = 1;
        while (size < deg) size <<= 1;
        assert(size <= (1 << 22));
        CompositionNTT transform(size);
        std::vector<unsigned> p(2 * size), q(size);
        for (int i = 0; i < (int)f.size(); ++i) p[i] = transform.in(f[i].val());
        for (int i = 0; i < (int)g.size(); ++i) q[i] = transform.in(g[i].val());
        unsigned a = transform.one, b = transform.one;
        transform.compose(p.data(), q.data(), 1, size, a, b);
        F result(deg);
        for (int i = 0; i < deg; ++i) result[i] = T::raw(transform.out(p[i]));
        return result;
    }

    T eval(T x) const {
        T ans = 0;
        for (auto it = this->rbegin(); it != this->rend(); ++it) ans = ans * x + *it;
        return ans;
    }

    std::vector<T> eval(const std::vector<T>& xs) const {
        if (xs.empty()) return {};
        if (this->size() <= 32 || xs.size() <= 32) {
            std::vector<T> result;
            result.reserve(xs.size());
            for (T x : xs) result.push_back(eval(x));
            return result;
        }
        constexpr int block = 8;
        int length = block;
        while (length < (int)std::max(this->size(), xs.size())) length <<= 1;
        assert(length <= (1 << 23));
        const auto& roots = ntt_roots(length);
        base values(length);
        std::copy(this->begin(), this->end(), values.begin());
        atcoder::internal::butterfly(values);
        std::vector<T> points, factor, result(xs.size());
        std::vector<int> index;
        auto locate = [&](auto&& self, T x) -> int {
            if (x == T(1)) return 0;
            int i = 2 * self(self, x * x);
            return i + (x != roots[i]);
        };
        const int lg = __builtin_ctz((unsigned)length);
        for (int i = 0; i < (int)xs.size(); ++i) {
            T power = xs[i];
            for (int j = 0; j < lg; ++j) power *= power;
            if (power == T(1))
                result[i] = values[locate(locate, xs[i])];
            else {
                points.push_back(xs[i]);
                factor.push_back(T(1) - power);
                index.push_back(i);
            }
        }
        if (points.empty()) return result;
        int size = length / block;
        std::vector<std::vector<T>> frequency;
        auto tree = product_tree(points, size, block, true, &frequency);
        base denominator(length);
        if ((int)frequency[2].size() == length && (int)frequency[3].size() == length) {
            for (int i = 0; i < length; ++i) denominator[i] = frequency[2][i] * frequency[3][i];
        } else {
            std::copy_n(tree[1].begin(), std::min<int>(tree[1].size(), length),
                        denominator.begin());
            if ((int)tree[1].size() > length) denominator[0] += tree[1][length];
            atcoder::internal::butterfly(denominator);
        }
        base prefix(length);
        T product = 1;
        for (int i = 0; i < length; ++i) prefix[i] = product, product *= denominator[i];
        assert(product != T(0));
        product = product.inv();
        F root(length);
        for (int i = 0; i < length; ++i) root[i] = values[i] * roots[i];
        for (int i = 2; i < length; i <<= 1) std::reverse(root.begin() + i, root.begin() + 2 * i);
        for (int i = length - 1; i >= 0; --i) {
            root[i] *= prefix[i] * product;
            product *= denominator[i];
        }
        atcoder::internal::butterfly_inv(static_cast<base&>(root));
        root *= T(length).inv();
        base evaluated = evaluate_tree(points, tree, frequency, size, block, std::move(root));
        for (int i = 0; i < (int)points.size(); ++i) result[index[i]] = evaluated[i] * factor[i];
        return result;
    }
    std::vector<T> multipoint_eval(const std::vector<T>& xs) const { return eval(xs); }

    static F interpolate(const std::vector<T>& xs, const std::vector<T>& ys) {
        assert(xs.size() == ys.size());
        const int n = (int)xs.size();
        if (n == 0) return {};
        int size = 1;
        std::vector<base> frequency;
        auto tree = product_tree(xs, size, 1, false, &frequency);
        F root = tree[1].diff().pre(n);
        root.resize(size);
        std::reverse(root.begin(), root.end());
        auto flip = [&] {
            for (int i = 1; i < 2 * size; ++i) {
                std::reverse(tree[i].begin(), tree[i].end());
                base& a = frequency[i];
                if (a.empty()) continue;
                const int length = (int)a.size(), d = (int)tree[i].size() - 1;
                for (int j = 2; j < length; j <<= 1) std::reverse(a.begin() + j, a.begin() + 2 * j);
                if (d == length / 2) {
                    for (int j = length / 2; j < length; ++j) a[j] = -a[j];
                } else {
                    const auto& roots = ntt_roots(length);
                    base phase(length, T(1));
                    for (int half = 1; half < length; half <<= 1) {
                        const T step = roots[half].pow(d);
                        for (int j = 0; j < half; ++j) phase[half + j] = phase[j] * step;
                    }
                    for (int j = 0; j < length; ++j) a[j] *= phase[j];
                }
            }
        };
        flip();
        F inverse = tree[1].inv(size);
        inverse.trim();
        root.trim();
        root = multiply(root, inverse).pre(size);
        root.resize(size);
        base den = evaluate_tree(xs, tree, frequency, size, 1, std::move(root));
        flip();
        base prefix(n);
        T product = 1;
        for (int i = 0; i < n; ++i) prefix[i] = product, product *= den[i];
        assert(product != T(0));
        product = product.inv();
        std::vector<F> value(2 * size);
        for (int i = n - 1; i >= 0; --i) {
            value[size + i] = {ys[i] * prefix[i] * product};
            product *= den[i];
        }
        for (int i = size - 1; i > 0; --i) {
            if (value[2 * i + 1].empty()) {
                value[i] = std::move(value[2 * i]);
                continue;
            }
            const int length = (int)frequency[2 * i].size();
            if (length && (int)frequency[2 * i + 1].size() == length) {
                base a = std::move(value[2 * i]), b = std::move(value[2 * i + 1]);
                a.resize(length);
                b.resize(length);
                atcoder::internal::butterfly(a);
                atcoder::internal::butterfly(b);
                for (int j = 0; j < length; ++j)
                    a[j] = a[j] * frequency[2 * i + 1][j] + b[j] * frequency[2 * i][j];
                intt(a);
                a.resize(tree[i].size() - 1);
                value[i] = std::move(a);
            } else {
                value[i] = value[2 * i] * tree[2 * i + 1] + value[2 * i + 1] * tree[2 * i];
            }
        }
        return std::move(value[1]);
    }

    void onemul(int d, T c) {
        assert(d >= 0);
        if (d == 0) {
            *this *= T(1) + c;
            return;
        }
        for (int i = (int)this->size() - 1; i >= d; --i) (*this)[i] += (*this)[i - d] * c;
    }
    void onediv(int d, T c) {
        assert(d >= 0);
        if (d == 0) {
            *this /= T(1) + c;
            return;
        }
        for (int i = d; i < (int)this->size(); ++i) (*this)[i] -= (*this)[i - d] * c;
    }

    F diff() const {
        const int n = (int)this->size();
        F r(n);
        for (int i = 1; i < n; ++i) r[i - 1] = (*this)[i] * i;
        return r;
    }
    F integral() const {
        const int n = (int)this->size();
        assert(n < T::mod());
        F r(n);
        const auto& inv = inverses(n);
        for (int i = 1; i < n; ++i) r[i] = (*this)[i - 1] * inv[i];
        return r;
    }

    F log(int deg = -1) const {
        if (deg < 0) deg = (int)this->size();
        assert(deg >= 0);
        if (deg == 0) return {};
        assert(!this->empty() && (*this)[0] == T(1));
        F derivative(std::min<int>(deg - 1, this->size() - 1));
        for (int i = 0; i < (int)derivative.size(); ++i) derivative[i] = T(i + 1) * (*this)[i + 1];
        F r = multiply(derivative, inv(deg)).pre(deg - 1);
        r.resize(deg);
        return r.integral();
    }

    F exp(int deg = -1) const {
        if (deg < 0) deg = (int)this->size();
        assert(deg >= 0);
        if (deg == 0) return {};
        assert(this->empty() || (*this)[0] == T(0));
        const auto& iv = inverses(deg);
        auto terms = sparse_terms(deg, 64);
        if (terms.size() <= 64) {
            for (auto& [j, x] : terms) x *= T(j);
            F g(deg);
            g[0] = 1;
            for (int i = 1; i < deg; ++i) {
                for (auto [j, x] : terms) {
                    if (j > i) break;
                    g[i] += x * g[i - j];
                }
                g[i] *= iv[i];
            }
            return g;
        }
        const int initial = std::min(deg, 32);
        F g(initial);
        g[0] = 1;
        for (int i = 1; i < initial; ++i) {
            T sum = 0;
            for (int j = 1; j <= i && j < (int)this->size(); ++j)
                sum += T(j) * (*this)[j] * g[i - j];
            g[i] = sum * iv[i];
        }
        if (initial == deg) return g;
        F h = g.pre(16).inv(16);
        base cache = h;
        cache.resize(32);
        atcoder::internal::butterfly(cache);
        while ((int)g.size() < deg) {
            const int m = (int)g.size(), n = 2 * m;
            if (deg - m <= 32) {
                base derivative(std::min<int>(deg, this->size()));
                for (int i = 1; i < (int)derivative.size(); ++i) derivative[i] = T(i) * (*this)[i];
                derivative.resize(std::max<int>(1, derivative.size()));
                g.resize(deg);
                for (int i = m; i < deg; ++i) {
                    const int count = std::min<int>(i, derivative.size() - 1);
                    g[i] = dot(derivative.data() + 1, g.data() + i - 1, count) * iv[i];
                }
                break;
            }
            assert(n <= (1 << 23));
            const T im = T::raw(T::mod() - (T::mod() - 1) / m);
            const T in = T::raw(T::mod() - (T::mod() - 1) / n);
            base a = g, b(m), error(n);
            a.resize(n);
            atcoder::internal::butterfly(a);
            inverse_extend(h, a, cache);
            cache = h;
            cache.resize(n);
            atcoder::internal::butterfly(cache);
            std::fill(b.begin(), b.end(), T(0));
            for (int i = 1; i < m && i < (int)this->size(); ++i) b[i - 1] = T(i) * (*this)[i];
            atcoder::internal::butterfly(b);
            for (int i = 0; i < m; ++i) b[i] *= a[i];
            atcoder::internal::butterfly_inv(b);
            error[m - 1] = b[m - 1] * im;
            for (int i = 0; i < m - 1; ++i) error[m + i] = b[i] * im - T(i + 1) * g[i + 1];
            atcoder::internal::butterfly(error);
            for (int i = 0; i < n; ++i) error[i] *= cache[i];
            atcoder::internal::butterfly_inv(error);
            for (int i = std::min(n, deg) - 1; i >= m; --i)
                error[i] = (i < (int)this->size() ? (*this)[i] : T(0)) + error[i - 1] * in * iv[i];
            std::fill(error.begin(), error.begin() + m, T(0));
            std::fill(error.begin() + std::min(n, deg), error.end(), T(0));
            atcoder::internal::butterfly(error);
            for (int i = 0; i < n; ++i) error[i] *= a[i];
            atcoder::internal::butterfly_inv(error);
            g.resize(std::min(n, deg));
            for (int i = m; i < (int)g.size(); ++i) g[i] = error[i] * in;
        }
        return g;
    }

    F pow(long long k) const {
        assert(k >= 0);
        const int n = (int)this->size();
        if (n == 0) return {};
        if (k == 0) {
            F r(n);
            r[0] = 1;
            return r;
        }
        if (k == 1) return *this;
        if (k <= 4) {
            F square = multiply(*this, *this).pre(n);
            return k == 2 ? square : multiply(square, k == 3 ? *this : square).pre(n);
        }
        int first = 0;
        while (first < n && (*this)[first] == T(0)) ++first;
        if (first == n || first > (n - 1) / k) return F(n);
        const int shift = first * k;
        const int len = n - shift;
        const T lead = (*this)[first];
        F f(this->begin() + first, this->end());
        f.resize(len);
        f /= lead;
        f = f.log(len);
        f *= T(k);
        f = f.exp(len);
        f *= lead.pow(k);
        F r(n);
        for (int i = 0; i < len; ++i) r[shift + i] = f[i];
        return r;
    }

    F shift(T c) {
        const int n = (int)this->size();
        if (n == 0) return *this;
        assert(n < T::mod());
        const auto& inv = inverses(n);
        F a(n), b(n);
        T fact = 1, invfact = 1, cpow = 1;
        for (int i = 0; i < n; ++i) {
            a[n - 1 - i] = (*this)[i] * fact;
            b[i] = cpow * invfact;
            fact *= i + 1;
            invfact *= inv[i + 1];
            cpow *= c;
        }
        F prod = multiply(a, b);
        invfact = 1;
        for (int i = 0; i < n; ++i) {
            (*this)[i] = prod[n - 1 - i] * invfact;
            invfact *= inv[i + 1];
        }
        return *this;
    }

    std::pair<F, F> divmod(const F& divisor) const {
        F a = *this;
        a.trim();
        int m = (int)divisor.size();
        while (m && divisor[m - 1] == T(0)) --m;
        assert(m);
        if ((int)a.size() < m) return {{}, std::move(a)};
        const int qn = (int)a.size() - m + 1;
        if (m <= 2 || (size_t)m * qn <= 4096) {
            F q(qn);
            const T inv = divisor[m - 1].inv();
            for (int i = qn - 1; i >= 0; --i) {
                q[i] = a[i + m - 1] * inv;
                for (int j = 0; j < m; ++j) a[i + j] -= q[i] * divisor[j];
            }
            a.resize(m - 1);
            a.trim();
            q.trim();
            return {std::move(q), std::move(a)};
        }
        F ra = a, rb(divisor.begin(), divisor.begin() + m);
        std::reverse(ra.begin(), ra.end());
        std::reverse(rb.begin(), rb.end());
        F q = multiply(ra.pre(qn), rb.inv(qn)).pre(qn);
        std::reverse(q.begin(), q.end());
        std::reverse(rb.begin(), rb.end());
        a -= multiply(rb, q);
        a.resize(m - 1);
        a.trim();
        q.trim();
        return {std::move(q), std::move(a)};
    }

   private:
    std::vector<std::pair<int, T>> sparse_terms(int deg, int limit) const {
        std::vector<std::pair<int, T>> terms;
        for (int i = 1; i < std::min<int>(deg, this->size()) && (int)terms.size() <= limit; ++i)
            if ((*this)[i] != T(0)) terms.emplace_back(i, (*this)[i]);
        return terms;
    }
    static base evaluate_tree(const base& points, const std::vector<F>& tree,
                              const std::vector<base>& frequency, int size, int block, F root) {
        base result(points.size());
        auto dfs = [&](auto&& self, int node, int l, int r, F cur) -> void {
            if (l * block >= (int)points.size()) return;
            const int length = (r - l) * block, half = length / 2;
            if (r - l == 1 || length <= 8) {
                base cofactor(length);
                for (int i = l * block; i < std::min<int>(r * block, points.size()); ++i) {
                    cofactor[0] = 1;
                    for (int k = 1; k < length; ++k)
                        cofactor[k] = (k < (int)tree[node].size() ? tree[node][k] : T(0)) +
                                      points[i] * cofactor[k - 1];
                    for (int k = 0; k < length; ++k) result[i] += cur[length - 1 - k] * cofactor[k];
                }
                return;
            }
            const int mid = (l + r) / 2;
            if (mid * block >= (int)points.size()) {
                self(self, 2 * node, l, mid, F(cur.begin() + half, cur.end()));
                return;
            }
            F left(half), right(half);
            if (!frequency[2 * node].empty()) {
                atcoder::internal::butterfly(static_cast<base&>(cur));
                base a = frequency[2 * node], b = frequency[2 * node + 1];
                for (int i = 0; i < length; ++i) a[i] *= cur[i], b[i] *= cur[i];
                intt(a);
                intt(b);
                std::copy_n(b.begin() + half, half, left.begin());
                std::copy_n(a.begin() + half, half, right.begin());
            } else {
                F a = multiply(cur, tree[2 * node + 1]), b = multiply(cur, tree[2 * node]);
                std::copy_n(a.begin() + half, half, left.begin());
                std::copy_n(b.begin() + half, half, right.begin());
            }
            self(self, 2 * node, l, mid, std::move(left));
            self(self, 2 * node + 1, mid, r, std::move(right));
        };
        dfs(dfs, 1, 0, size, std::move(root));
        return result;
    }
    static void inverse_extend(F& h, const base& a, const base& cache) {
        const int m = (int)cache.size();
        const T im = T::raw(T::mod() - (T::mod() - 1) / m);
        base b(m);
        for (int i = 0; i < m; ++i) b[i] = a[i] * cache[i];
        atcoder::internal::butterfly_inv(b);
        std::fill(b.begin(), b.begin() + m / 2, T(0));
        atcoder::internal::butterfly(b);
        for (int i = 0; i < m; ++i) b[i] *= cache[i];
        atcoder::internal::butterfly_inv(b);
        h.resize(m);
        for (int i = m / 2; i < m; ++i) h[i] = -b[i] * im * im;
    }
    static T dot(const T* a, const T* b, int n) {
        unsigned result = 0;
        for (int i = 0; i < n; i += 16) {
            unsigned long long sum = 0;
            for (int j = i; j < std::min(n, i + 16); ++j)
                sum += (unsigned long long)a[j].val() * b[-j].val();
            result += sum % T::mod();
            if (result >= T::mod()) result -= T::mod();
        }
        return T::raw(result);
    }

    struct CompositionNTT {
        using U = unsigned;
        using V = std::vector<U>;
        static constexpr U mod = 998244353, twice = 2 * mod, one = 301989884;
        V roots, inverse_roots, twists, inverse_twists;
        V root_quotients, inverse_root_quotients;
        static U reduce(unsigned long long x) {
            return (x + (unsigned long long)(U(x) * 998244351u) * mod) >> 32;
        }
        static U mul(U a, U b) { return reduce((unsigned long long)a * b); }
        static U add(U a, U b) {
            U c = a + b;
            return c < twice ? c : c - twice;
        }
        static U sub(U a, U b) {
            U c = a - b;
            return c < twice ? c : c + twice;
        }
        static U in(U a) { return mul(a, 932051910); }
        static U out(U a) {
            a = reduce(a);
            return a < mod ? a : a - mod;
        }
        static U quotient(U r) { return ((unsigned long long)r << 32) / mod; }
        static U fixed_mul(U a, U r, U q) {
            return a * r - U(((unsigned long long)a * q) >> 32) * mod;
        }
        CompositionNTT(int size)
            : roots(size, one),
              inverse_roots(size, one),
              twists(size, one),
              inverse_twists(size, one),
              root_quotients(size),
              inverse_root_quotients(size) {
            for (int h = 1; h < size; h <<= 1) {
                U r = in(T(3).pow((mod - 1) / (4 * h)).val());
                U ir = in(T(3).pow(mod - 1 - (mod - 1) / (4 * h)).val());
                U t = in(T(3).pow((mod - 1) / (2 * h)).val());
                U it = in(T(3).pow(mod - 1 - (mod - 1) / (2 * h)).val());
                U a = in(mod - (mod - 1) / h), b = a;
                for (int i = 0; i < h; ++i) {
                    roots[h + i] = mul(roots[i], r);
                    inverse_roots[h + i] = mul(inverse_roots[i], ir);
                    twists[h + i] = a;
                    inverse_twists[h + i] = b;
                    a = mul(a, t);
                    b = mul(b, it);
                }
            }
            for (int i = 0; i < size; ++i) {
                roots[i] = out(roots[i]);
                inverse_roots[i] = out(inverse_roots[i]);
                root_quotients[i] = quotient(roots[i]);
                inverse_root_quotients[i] = quotient(inverse_roots[i]);
                twists[i] = out(twists[i]);
                inverse_twists[i] = out(inverse_twists[i]);
            }
        }

#if defined(__ARM_NEON)
        using I = uint32x4_t;
        static I mul4(I a, I b) {
            uint64x2_t x = vmull_u32(vget_low_u32(a), vget_low_u32(b));
            uint64x2_t y = vmull_u32(vget_high_u32(a), vget_high_u32(b));
            auto reduce2 = [](uint64x2_t z) {
                uint32x2_t q = vmul_u32(vmovn_u64(z), vdup_n_u32(998244351));
                return vshrn_n_u64(vaddq_u64(z, vmull_u32(q, vdup_n_u32(mod))), 32);
            };
            return vcombine_u32(reduce2(x), reduce2(y));
        }
        static I add4(I a, I b) {
            I c = vaddq_u32(a, b);
            return vminq_u32(c, vsubq_u32(c, vdupq_n_u32(twice)));
        }
        static I sub4(I a, I b) {
            I c = vsubq_u32(a, b);
            return vminq_u32(c, vaddq_u32(c, vdupq_n_u32(twice)));
        }
        static I fixed_mul4(I a, I r, I q) {
            uint64x2_t x = vmull_u32(vget_low_u32(a), vget_low_u32(q));
            uint64x2_t y = vmull_u32(vget_high_u32(a), vget_high_u32(q));
            I t = vcombine_u32(vshrn_n_u64(x, 32), vshrn_n_u64(y, 32));
            return vmlsq_u32(vmulq_u32(a, r), t, vdupq_n_u32(mod));
        }
        template <bool inverse>
        static void fixed_butterfly4(I& x, I& y, I r, I q, bool unit = false) {
            if constexpr (!inverse)
                if (!unit) y = fixed_mul4(y, r, q);
            I z = sub4(x, y);
            x = add4(x, y);
            y = inverse && !unit ? fixed_mul4(z, r, q) : z;
        }
#endif
#if defined(__x86_64__) || defined(__i386__)
        static bool vectorized() {
            static const bool available = __builtin_cpu_supports("avx2");
            return available;
        }
        using I8 = __m256i;
        __attribute__((target("avx2"))) static I8 load8(const U* a) {
            return _mm256_loadu_si256(reinterpret_cast<const I8*>(a));
        }
        __attribute__((target("avx2"))) static void store8(U* a, I8 x) {
            _mm256_storeu_si256(reinterpret_cast<I8*>(a), x);
        }
        __attribute__((target("avx2"))) static I8 mul8(I8 a, I8 b) {
            I8 x = _mm256_mul_epu32(a, b);
            I8 y = _mm256_mul_epu32(_mm256_srli_epi64(a, 32), _mm256_srli_epi64(b, 32));
            I8 r = _mm256_set1_epi32(998244351), p = _mm256_set1_epi32(mod);
            x = _mm256_add_epi64(x, _mm256_mul_epu32(_mm256_mul_epu32(x, r), p));
            y = _mm256_add_epi64(y, _mm256_mul_epu32(_mm256_mul_epu32(y, r), p));
            return _mm256_blend_epi32(_mm256_srli_epi64(x, 32), y, 0xaa);
        }
        __attribute__((target("avx2"))) static I8 fixed_mul8(I8 a, I8 r, I8 q) {
            I8 x = _mm256_srli_epi64(_mm256_mul_epu32(a, q), 32);
            I8 y = _mm256_mul_epu32(_mm256_srli_epi64(a, 32), q);
            I8 t = _mm256_blend_epi32(x, y, 0xaa);
            return _mm256_sub_epi32(_mm256_mullo_epi32(a, r),
                                    _mm256_mullo_epi32(t, _mm256_set1_epi32(mod)));
        }
        __attribute__((target("avx2"))) static I8 fixed_even8(I8 a, I8 r, I8 q) {
            I8 t = _mm256_srli_epi64(_mm256_mul_epu32(a, q), 32);
            return _mm256_sub_epi64(_mm256_mul_epu32(a, r),
                                    _mm256_mul_epu32(t, _mm256_set1_epi32(mod)));
        }
        __attribute__((target("avx2"))) static I8 add8(I8 a, I8 b) {
            I8 x = _mm256_add_epi32(a, b);
            return _mm256_min_epu32(x, _mm256_sub_epi32(x, _mm256_set1_epi32(twice)));
        }
        __attribute__((target("avx2"))) static I8 sub8(I8 a, I8 b) {
            I8 x = _mm256_sub_epi32(a, b);
            return _mm256_min_epu32(x, _mm256_add_epi32(x, _mm256_set1_epi32(twice)));
        }

        template <int stride>
        __attribute__((target("avx2"))) static I8 rates8(const V& values, int k) {
            if constexpr (stride == 1)
                return _mm256_cvtepu32_epi64(
                    _mm_loadu_si128(reinterpret_cast<const __m128i*>(values.data() + 4 * k)));
            if constexpr (stride == 2)
                return _mm256_permute4x64_epi64(
                    _mm256_cvtepu32_epi64(
                        _mm_loadl_epi64(reinterpret_cast<const __m128i*>(values.data() + 2 * k))),
                    0x50);
            if constexpr (stride == 4) return _mm256_set1_epi32(values[k]);
        }
        template <int stride, bool inverse>
        __attribute__((target("avx2"))) I8 small8(I8 z, int k) const {
            if constexpr (stride == 4) {
                if (k == 0) {
                    I8 x = _mm256_permute2x128_si256(z, z, 0x00),
                       y = _mm256_permute2x128_si256(z, z, 0x11);
                    return _mm256_blend_epi32(add8(x, y), sub8(x, y), 0xf0);
                }
            }
            const V& w = inverse ? inverse_roots : roots;
            const V& quot = inverse ? inverse_root_quotients : root_quotients;
            I8 r = rates8<stride>(w, k), q = rates8<stride>(quot, k);
            if constexpr (inverse) {
                if constexpr (stride == 2) z = _mm256_shuffle_epi32(z, 0xd8);
                if constexpr (stride == 4)
                    z = _mm256_permutevar8x32_epi32(z, _mm256_setr_epi32(0, 4, 1, 5, 2, 6, 3, 7));
                I8 y = _mm256_srli_epi64(z, 32), x = add8(z, y);
                I8 d = fixed_even8(sub8(z, y), r, q);
                z = _mm256_blend_epi32(x, _mm256_slli_epi64(d, 32), 0xaa);
            } else {
                if constexpr (stride == 1) z = _mm256_shuffle_epi32(z, 0xb1);
                if constexpr (stride == 2) z = _mm256_shuffle_epi32(z, 0x72);
                if constexpr (stride == 4)
                    z = _mm256_permutevar8x32_epi32(z, _mm256_setr_epi32(4, 0, 5, 1, 6, 2, 7, 3));
                I8 y = fixed_even8(z, r, q);
                I8 x = _mm256_blend_epi32(_mm256_srli_epi64(z, 32), z, 0xaa);
                y = _mm256_blend_epi32(
                    y, _mm256_sub_epi32(_mm256_set1_epi32(twice), _mm256_slli_epi64(y, 32)), 0xaa);
                z = add8(x, y);
            }
            if constexpr (stride == 2) z = _mm256_shuffle_epi32(z, 0xd8);
            if constexpr (stride == 4)
                z = _mm256_permutevar8x32_epi32(z, _mm256_setr_epi32(0, 2, 4, 6, 1, 3, 5, 7));
            return z;
        }
        template <bool inverse>
        __attribute__((target("avx2"))) void fft8(U* a, int length, int limit = 0) const {
            if (!limit) limit = length;
            if constexpr (!inverse)
                for (int h = limit / 2; h >= 8; h >>= 1) stage8<false>(a, h, length);
            for (int j = 0; j < length; j += 8) {
                I8 z = load8(a + j);
                if constexpr (inverse)
                    z = small8<4, true>(small8<2, true>(small8<1, true>(z, j / 8), j / 8), j / 8);
                else
                    z = small8<1, false>(small8<2, false>(small8<4, false>(z, j / 8), j / 8),
                                         j / 8);
                store8(a + j, z);
            }
            if constexpr (inverse)
                for (int h = 8; h < limit; h <<= 1) stage8<true>(a, h, length);
        }
        template <bool inverse>
        __attribute__((target("avx2"))) void stage8(U* a, int stride, int length) const {
            const V& w = inverse ? inverse_roots : roots;
            const V& quot = inverse ? inverse_root_quotients : root_quotients;
            if (stride < 8) {
                for (int j = 0; j < length; j += 8) {
                    I8 z = load8(a + j);
                    if (stride == 1)
                        z = small8<1, inverse>(z, j / 8);
                    else if (stride == 2)
                        z = small8<2, inverse>(z, j / 8);
                    else
                        z = small8<4, inverse>(z, j / 8);
                    store8(a + j, z);
                }
            } else {
                for (int j = 0, k = 0; j < length; j += 2 * stride, ++k) {
                    I8 r = _mm256_set1_epi32(w[k]), q = _mm256_set1_epi32(quot[k]);
                    for (int i = 0; i < stride; i += 8) {
                        I8 x = load8(a + j + i), y = load8(a + j + stride + i);
                        if constexpr (!inverse)
                            if (k) y = fixed_mul8(y, r, q);
                        I8 z = sub8(x, y);
                        x = add8(x, y);
                        if constexpr (inverse)
                            if (k) z = fixed_mul8(z, r, q);
                        store8(a + j + i, x);
                        store8(a + j + stride + i, z);
                    }
                }
            }
        }

        __attribute__((target("avx2"))) static void fold8(U* a, int columns) {
            for (int i = 0; i < columns; i += 8) {
                I8 x = add8(load8(a + i), load8(a + columns + i));
                store8(a + i, x);
                store8(a + columns + i, x);
            }
        }
        __attribute__((target("avx2"))) void project4_8(U* a, int rows) const {
            for (int j = 0; j < rows; ++j) {
                I8 x = small8<2, true>(small8<1, true>(load8(a + 8 * j), 0), 0);
                x = add8(x, _mm256_permute2x128_si256(x, x, 1));
                x = small8<1, false>(small8<2, false>(x, 0), 0);
                store8(a + 8 * j, x);
            }
        }
        __attribute__((target("avx2"))) static void load_pairs(const U* a, I8& x, I8& y) {
            I8 index = _mm256_setr_epi32(0, 2, 4, 6, 1, 3, 5, 7);
            I8 u = _mm256_permutevar8x32_epi32(load8(a), index),
               v = _mm256_permutevar8x32_epi32(load8(a + 8), index);
            x = _mm256_permute2x128_si256(u, v, 0x20);
            y = _mm256_permute2x128_si256(u, v, 0x31);
        }
        __attribute__((target("avx2"))) static void store_pairs(U* a, I8 x, I8 y) {
            I8 index = _mm256_setr_epi32(0, 4, 1, 5, 2, 6, 3, 7);
            store8(a, _mm256_permutevar8x32_epi32(_mm256_permute2x128_si256(x, y, 0x20), index));
            store8(a + 8,
                   _mm256_permutevar8x32_epi32(_mm256_permute2x128_si256(x, y, 0x31), index));
        }
        __attribute__((target("avx2"))) static void scale8(U* a, int length, U r) {
            I8 v = _mm256_set1_epi32(r), q = _mm256_set1_epi32(quotient(r));
            for (int i = 0; i < length; i += 8) store8(a + i, fixed_mul8(load8(a + i), v, q));
        }
        __attribute__((target("avx2"))) static void pair_product8(U* a, const U* b, int length) {
            for (int i = 0; i < length; i += 8) {
                I8 x, y;
                load_pairs(b + 2 * i, x, y);
                store8(a + i, mul8(x, y));
            }
        }
        __attribute__((target("avx2"))) static void pair_multiply8(U* a, const U* b, int length) {
            for (int i = 0; i < length; i += 8) {
                I8 x, y;
                load_pairs(a + 2 * i, x, y);
                I8 r = load8(b + i);
                store_pairs(a + 2 * i, mul8(y, r), mul8(x, r));
            }
        }
        __attribute__((target("avx2"))) static void difference8(U* a, const U* b, const U* c,
                                                                int length) {
            for (int i = 0; i < length; i += 8) store8(a + i, sub8(load8(b + i), load8(c + i)));
        }
        __attribute__((target("avx2"))) static void project1_8(U* a, int rows) {
            for (int j = 0; j < rows; j += 8) {
                I8 x, y;
                load_pairs(a + 2 * j, x, y);
                x = add8(x, y);
                store_pairs(a + 2 * j, x, x);
            }
        }
        __attribute__((target("avx2"))) void project2_8(U* a, int rows) const {
            const I8 index = _mm256_setr_epi32(0, 4, 1, 5, 2, 6, 3, 7);
            for (int j = 0; j < rows; j += 8) {
                U* p = a + 4 * j;
                I8 u = load8(p), v = load8(p + 8), w = load8(p + 16), z = load8(p + 24);
                I8 uv0 = _mm256_unpacklo_epi32(u, v), uv1 = _mm256_unpackhi_epi32(u, v);
                I8 wz0 = _mm256_unpacklo_epi32(w, z), wz1 = _mm256_unpackhi_epi32(w, z);
                I8 a0 = _mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(uv0, wz0), index);
                I8 a1 = _mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(uv0, wz0), index);
                I8 a2 = _mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(uv1, wz1), index);
                I8 a3 = _mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(uv1, wz1), index);
                I8 x = add8(add8(a0, a1), add8(a2, a3));
                I8 y =
                    add8(sub8(a0, a1), fixed_mul8(sub8(a2, a3), _mm256_set1_epi32(inverse_roots[1]),
                                                  _mm256_set1_epi32(inverse_root_quotients[1])));
                a0 = add8(x, y);
                a1 = sub8(x, y);
                y = fixed_mul8(y, _mm256_set1_epi32(roots[1]),
                               _mm256_set1_epi32(root_quotients[1]));
                a2 = add8(x, y);
                a3 = sub8(x, y);
                uv0 = _mm256_unpacklo_epi32(a0, a1);
                uv1 = _mm256_unpackhi_epi32(a0, a1);
                wz0 = _mm256_unpacklo_epi32(a2, a3);
                wz1 = _mm256_unpackhi_epi32(a2, a3);
                u = _mm256_unpacklo_epi64(uv0, wz0);
                v = _mm256_unpackhi_epi64(uv0, wz0);
                w = _mm256_unpacklo_epi64(uv1, wz1);
                z = _mm256_unpackhi_epi64(uv1, wz1);
                store8(p, _mm256_permute2x128_si256(u, v, 0x20));
                store8(p + 8, _mm256_permute2x128_si256(w, z, 0x20));
                store8(p + 16, _mm256_permute2x128_si256(u, v, 0x31));
                store8(p + 24, _mm256_permute2x128_si256(w, z, 0x31));
            }
        }
#endif

#if defined(__ARM_NEON)
        template <int stride, bool inverse>
        void small4(I& a, I& b, int k) const {
            const V& w = inverse ? inverse_roots : roots;
            const V& quot = inverse ? inverse_root_quotients : root_quotients;
            if constexpr (stride == 4) {
                if (k == 0) {
                    I x = sub4(a, b);
                    a = add4(a, b);
                    b = x;
                } else
                    fixed_butterfly4<inverse>(a, b, vdupq_n_u32(w[k]), vdupq_n_u32(quot[k]));
            }
            if constexpr (stride == 2) {
                I x = vcombine_u32(vget_low_u32(a), vget_low_u32(b));
                I y = vcombine_u32(vget_high_u32(a), vget_high_u32(b));
                fixed_butterfly4<inverse>(
                    x, y, vcombine_u32(vdup_n_u32(w[2 * k]), vdup_n_u32(w[2 * k + 1])),
                    vcombine_u32(vdup_n_u32(quot[2 * k]), vdup_n_u32(quot[2 * k + 1])));
                a = vcombine_u32(vget_low_u32(x), vget_low_u32(y));
                b = vcombine_u32(vget_high_u32(x), vget_high_u32(y));
            }
            if constexpr (stride == 1) {
                uint32x4x2_t z = vuzpq_u32(a, b);
                fixed_butterfly4<inverse>(z.val[0], z.val[1], vld1q_u32(w.data() + 4 * k),
                                          vld1q_u32(quot.data() + 4 * k));
                z = vzipq_u32(z.val[0], z.val[1]);
                a = z.val[0];
                b = z.val[1];
            }
        }
        template <bool inverse>
        void fft4(U* a, int length, int limit = 0) const {
            if (!limit) limit = length;
            if constexpr (!inverse) {
                int h = limit / 2;
                for (; h >= 16; h >>= 2) stage_pair<false>(a, h / 2, length);
                if (h == 8) stage<false>(a, h, length);
            }
            for (int j = 0; j < length; j += 8) {
                I x = vld1q_u32(a + j), y = vld1q_u32(a + j + 4);
                if constexpr (inverse) {
                    small4<1, true>(x, y, j / 8);
                    small4<2, true>(x, y, j / 8);
                    small4<4, true>(x, y, j / 8);
                } else {
                    small4<4, false>(x, y, j / 8);
                    small4<2, false>(x, y, j / 8);
                    small4<1, false>(x, y, j / 8);
                }
                vst1q_u32(a + j, x);
                vst1q_u32(a + j + 4, y);
            }
            if constexpr (inverse) {
                int h = 8;
                for (; 4 * h <= limit; h <<= 2) stage_pair<true>(a, h, length);
                if (h < limit) stage<true>(a, h, length);
            }
        }
#endif
        template <bool inverse>
        void stage(U* a, int stride, int length) const {
            const V& w = inverse ? inverse_roots : roots;
            const V& quot = inverse ? inverse_root_quotients : root_quotients;
#if defined(__x86_64__) || defined(__i386__)
            if (length >= 8 && vectorized()) {
                stage8<inverse>(a, stride, length);
                return;
            }
#endif
            for (int j = 0, k = 0; j < length; j += 2 * stride, ++k) {
                U r = w[k], q = quot[k];
                int i = 0;
#if defined(__ARM_NEON)
                for (; i + 4 <= stride; i += 4) {
                    I x = vld1q_u32(a + j + i), y = vld1q_u32(a + j + stride + i);
                    if constexpr (!inverse)
                        if (k) y = fixed_mul4(y, vdupq_n_u32(r), vdupq_n_u32(q));
                    I z = sub4(x, y);
                    x = add4(x, y);
                    if constexpr (inverse)
                        if (k) z = fixed_mul4(z, vdupq_n_u32(r), vdupq_n_u32(q));
                    y = z;
                    vst1q_u32(a + j + i, x);
                    vst1q_u32(a + j + stride + i, y);
                }
#endif
                for (; i < stride; ++i) {
                    U x = a[j + i], y = a[j + stride + i];
                    if constexpr (!inverse)
                        if (k) y = fixed_mul(y, r, q);
                    a[j + i] = add(x, y);
                    a[j + stride + i] = inverse && k ? fixed_mul(sub(x, y), r, q) : sub(x, y);
                }
            }
        }

        template <bool inverse>
        void stage_pair(U* a, int stride, int length) const {
#if defined(__x86_64__) || defined(__i386__)
            if (stride >= 8 && vectorized()) {
                if constexpr (inverse)
                    stage8<true>(a, stride, length), stage8<true>(a, 2 * stride, length);
                else
                    stage8<false>(a, 2 * stride, length), stage8<false>(a, stride, length);
                return;
            }
#endif
#if defined(__ARM_NEON)
            if (stride >= 4) {
                const V& w = inverse ? inverse_roots : roots;
                const V& q = inverse ? inverse_root_quotients : root_quotients;
                for (int j = 0, k = 0; j < length; j += 4 * stride, ++k) {
                    I r = vdupq_n_u32(w[k]), rq = vdupq_n_u32(q[k]);
                    I r0 = vdupq_n_u32(w[2 * k]), q0 = vdupq_n_u32(q[2 * k]);
                    I r1 = vdupq_n_u32(w[2 * k + 1]), q1 = vdupq_n_u32(q[2 * k + 1]);
                    for (int i = 0; i < stride; i += 4) {
                        I x0 = vld1q_u32(a + j + i), x1 = vld1q_u32(a + j + stride + i);
                        I x2 = vld1q_u32(a + j + 2 * stride + i),
                          x3 = vld1q_u32(a + j + 3 * stride + i);
                        if constexpr (inverse) {
                            fixed_butterfly4<true>(x0, x1, r0, q0, k == 0);
                            fixed_butterfly4<true>(x2, x3, r1, q1);
                            fixed_butterfly4<true>(x0, x2, r, rq, k == 0);
                            fixed_butterfly4<true>(x1, x3, r, rq, k == 0);
                        } else {
                            fixed_butterfly4<false>(x0, x2, r, rq, k == 0);
                            fixed_butterfly4<false>(x1, x3, r, rq, k == 0);
                            fixed_butterfly4<false>(x0, x1, r0, q0, k == 0);
                            fixed_butterfly4<false>(x2, x3, r1, q1);
                        }
                        vst1q_u32(a + j + i, x0);
                        vst1q_u32(a + j + stride + i, x1);
                        vst1q_u32(a + j + 2 * stride + i, x2);
                        vst1q_u32(a + j + 3 * stride + i, x3);
                    }
                }
                return;
            }
#endif
            if constexpr (inverse)
                stage<true>(a, stride, length), stage<true>(a, 2 * stride, length);
            else
                stage<false>(a, 2 * stride, length), stage<false>(a, stride, length);
        }
        template <bool inverse>
        void fft(U* a, int length, int limit = 0) const {
            if (!limit) limit = length;
#if defined(__ARM_NEON)
            if (limit >= 8) {
                fft4<inverse>(a, length, limit);
                return;
            }
#elif defined(__x86_64__) || defined(__i386__)
            if (limit >= 8 && vectorized()) {
                fft8<inverse>(a, length, limit);
                return;
            }
#endif
            if constexpr (inverse) {
                for (int h = 1; h < limit; h <<= 1) stage<true>(a, h, length);
            } else {
                for (int h = limit / 2; h; h >>= 1) stage<false>(a, h, length);
            }
        }
        void scale(U* a, int length, U r) const {
#if defined(__x86_64__) || defined(__i386__)
            if (length >= 8 && vectorized()) {
                scale8(a, length, r);
                return;
            }
#endif
            int i = 0;
            U q = quotient(r);
#if defined(__ARM_NEON)
            for (; i + 4 <= length; i += 4)
                vst1q_u32(a + i, fixed_mul4(vld1q_u32(a + i), vdupq_n_u32(r), vdupq_n_u32(q)));
#endif
            for (; i < length; ++i) a[i] = fixed_mul(a[i], r, q);
        }

        static void pair_product(U* a, const U* b, int length) {
#if defined(__x86_64__) || defined(__i386__)
            if (length >= 8 && vectorized()) {
                pair_product8(a, b, length);
                return;
            }
#endif
            int i = 0;
#if defined(__ARM_NEON)
            for (; i + 4 <= length; i += 4) {
                uint32x4x2_t v = vld2q_u32(b + 2 * i);
                vst1q_u32(a + i, mul4(v.val[0], v.val[1]));
            }
#endif
            for (; i < length; ++i) a[i] = mul(b[2 * i], b[2 * i + 1]);
        }
        static void pair_multiply(U* a, const U* b, int length) {
#if defined(__x86_64__) || defined(__i386__)
            if (length >= 8 && vectorized()) {
                pair_multiply8(a, b, length);
                return;
            }
#endif
            int i = 0;
#if defined(__ARM_NEON)
            for (; i + 4 <= length; i += 4) {
                uint32x4x2_t v = vld2q_u32(a + 2 * i);
                I x = mul4(v.val[1], vld1q_u32(b + i));
                v.val[1] = mul4(v.val[0], vld1q_u32(b + i));
                v.val[0] = x;
                vst2q_u32(a + 2 * i, v);
            }
#endif
            for (; i < length; ++i) {
                U x = a[2 * i];
                a[2 * i] = mul(b[i], a[2 * i + 1]);
                a[2 * i + 1] = mul(b[i], x);
            }
        }
        static void difference(U* a, const U* b, const U* c, int length) {
#if defined(__x86_64__) || defined(__i386__)
            if (length >= 8 && vectorized()) {
                difference8(a, b, c, length);
                return;
            }
#endif
            int i = 0;
#if defined(__ARM_NEON)
            for (; i + 4 <= length; i += 4)
                vst1q_u32(a + i, sub4(vld1q_u32(b + i), vld1q_u32(c + i)));
#endif
            for (; i < length; ++i) a[i] = sub(b[i], c[i]);
        }
        static void fold(U* a, int columns) {
#if defined(__x86_64__) || defined(__i386__)
            if (columns >= 8 && vectorized()) {
                fold8(a, columns);
                return;
            }
#endif
            int i = 0;
#if defined(__ARM_NEON)
            for (; i + 4 <= columns; i += 4) {
                I x = add4(vld1q_u32(a + i), vld1q_u32(a + columns + i));
                vst1q_u32(a + i, x);
                vst1q_u32(a + columns + i, x);
            }
#endif
            for (; i < columns; ++i) a[i] = a[columns + i] = add(a[i], a[columns + i]);
        }
        void project(U* a, int rows, int columns) const {
            if (columns == 1) {
#if defined(__x86_64__) || defined(__i386__)
                if (rows >= 8 && vectorized()) {
                    project1_8(a, rows);
                    return;
                }
#endif
                int j = 0;
#if defined(__ARM_NEON)
                for (; j + 4 <= rows; j += 4) {
                    uint32x4x2_t v = vld2q_u32(a + 2 * j);
                    v.val[0] = v.val[1] = add4(v.val[0], v.val[1]);
                    vst2q_u32(a + 2 * j, v);
                }
#endif
                for (; j < rows; ++j) a[2 * j] = a[2 * j + 1] = add(a[2 * j], a[2 * j + 1]);
                return;
            }
            if (columns == 2) {
#if defined(__x86_64__) || defined(__i386__)
                if (rows >= 8 && vectorized()) {
                    project2_8(a, rows);
                    return;
                }
#endif
                int j = 0;
#if defined(__ARM_NEON)
                for (; j + 4 <= rows; j += 4) {
                    uint32x4x4_t v = vld4q_u32(a + 4 * j);
                    I x = add4(add4(v.val[0], v.val[1]), add4(v.val[2], v.val[3]));
                    I y = add4(sub4(v.val[0], v.val[1]),
                               fixed_mul4(sub4(v.val[2], v.val[3]), vdupq_n_u32(inverse_roots[1]),
                                          vdupq_n_u32(inverse_root_quotients[1])));
                    v.val[0] = add4(x, y);
                    v.val[1] = sub4(x, y);
                    y = fixed_mul4(y, vdupq_n_u32(roots[1]), vdupq_n_u32(root_quotients[1]));
                    v.val[2] = add4(x, y);
                    v.val[3] = sub4(x, y);
                    vst4q_u32(a + 4 * j, v);
                }
#endif
                for (; j < rows; ++j) {
                    U* r = a + 4 * j;
                    U x = add(add(r[0], r[1]), add(r[2], r[3]));
                    U y = add(sub(r[0], r[1]), fixed_mul(sub(r[2], r[3]), inverse_roots[1],
                                                         inverse_root_quotients[1]));
                    r[0] = add(x, y);
                    r[1] = sub(x, y);
                    y = fixed_mul(y, roots[1], root_quotients[1]);
                    r[2] = add(x, y);
                    r[3] = sub(x, y);
                }
                return;
            }
            if (columns == 4) {
#if defined(__x86_64__) || defined(__i386__)
                if (vectorized()) {
                    project4_8(a, rows);
                    return;
                }
#endif
#if defined(__ARM_NEON)
                for (int j = 0; j < rows; ++j) {
                    I x = vld1q_u32(a + 8 * j), y = vld1q_u32(a + 8 * j + 4);
                    small4<1, true>(x, y, 0);
                    small4<2, true>(x, y, 0);
                    x = y = add4(x, y);
                    small4<2, false>(x, y, 0);
                    small4<1, false>(x, y, 0);
                    vst1q_u32(a + 8 * j, x);
                    vst1q_u32(a + 8 * j + 4, y);
                }
                return;
#endif
            }
            for (int j = 0; j < rows; ++j) {
                U* row = a + 2 * columns * j;
                fft<true>(row, 2 * columns, columns);
                fold(row, columns);
                fft<false>(row, 2 * columns, columns);
            }
        }
        void double_y(U* a, int rows, int columns, bool inverse) const {
            const int size = rows * columns;
            int h = 2 * columns;
            for (; h <= size / 2; h <<= 2) stage_pair<true>(a, h, 2 * size);
            if (h <= size) stage<true>(a, h, 2 * size);
            for (int j = 0; j < rows; ++j)
                scale(a + 2 * columns * j, 2 * columns,
                      (inverse ? inverse_twists : twists)[rows + j]);
        }
        void compose(U* p, const U* q, int rows, int columns, U& p_one, U& q_one) const {
            const int size = rows * columns;
            if (columns == 1) {
                fft<false>(p, rows);
                for (int i = rows - 1; i >= 0; --i) p[2 * i] = p[2 * i + 1] = p[i];
                return;
            }
            V v(4 * size);
            if (rows == 1) {
                for (int i = 0; i < columns; ++i) v[i] = sub(0, q[i]);
                fft<false>(v.data(), 2 * columns);
                for (int i = 0; i < 2 * columns; ++i) {
                    U x = v[i];
                    v[i] = add(x, one);
                    v[i + 2 * columns] = sub(x, one);
                }
            } else {
                pair_product(v.data(), q, 2 * size);
                q_one = mul(q_one, q_one);
                project(v.data(), rows, columns);
                std::copy_n(v.data(), 2 * size, v.data() + 2 * size);
                U* upper = v.data() + 2 * size;
                double_y(upper, rows, columns, false);
                q_one = mul(q_one, in(2 * columns));
                U correction = add(q_one, q_one);
                for (int i = 0; i < 2 * columns; ++i) upper[i] = sub(upper[i], correction);
                int h = size;
                for (; h / 2 > columns; h >>= 2) stage_pair<false>(upper, h / 2, 2 * size);
                if (h > columns) stage<false>(upper, h, 2 * size);
            }
            U saved = q_one;
            compose(p, v.data(), 2 * rows, columns / 2, p_one, q_one);
            pair_multiply(v.data(), p, 2 * size);
            p_one = mul(p_one, saved);
            if (rows == 1) {
                fft<true>(v.data(), 2 * columns);
                fft<true>(v.data() + 2 * columns, 2 * columns);
                stage<true>(v.data(), 2 * columns, 4 * columns);
                U factor = in(T(out(mul(p_one, in(4 * size)))).inv().val());
                for (int i = 0; i < columns; ++i) p[i] = mul(v[2 * columns + i], factor);
            } else {
                U* upper = v.data() + 2 * size;
                double_y(upper, rows, columns, true);
                int h = size;
                for (; h / 2 > columns; h >>= 2) stage_pair<false>(upper, h / 2, 2 * size);
                if (h > columns) stage<false>(upper, h, 2 * size);
                p_one = add(p_one, p_one);
                difference(p, v.data(), upper, 2 * size);
                project(p, rows, columns);
                p_one = mul(p_one, in(2 * columns));
            }
        }
    };
    static void intt(base& a) {
        atcoder::internal::butterfly_inv(a);
        const T inv = T::raw(T::mod() - (T::mod() - 1) / (int)a.size());
        for (T& x : a) x *= inv;
    }
    static T mod_sqrt(T a) {
        if (a == T(0) || a.pow((T::mod() - 1) / 2) != T(1)) return 0;
        T r = a.pow(60), t = a.pow(119), c = T(3).pow(119);
        int m = 23;
        while (t != T(1)) {
            int i = 0;
            for (T u = t; u != T(1); u *= u) ++i;
            T b = c.pow(1 << (m - i - 1));
            r *= b;
            c = b * b;
            t *= c;
            m = i;
        }
        return r;
    }
    static const base& inverse_ntt_roots(int n) {
        static base roots{1};
        while ((int)roots.size() < n) {
            const int m = (int)roots.size();
            const T step = T(3).pow((T::mod() - 1) / (2 * m)).inv();
            roots.resize(2 * m);
            for (int i = 0; i < m; ++i) roots[m + i] = roots[i] * step;
        }
        return roots;
    }
    static const base& ntt_roots(int n) {
        static base roots{1};
        while ((int)roots.size() < n) {
            const int m = (int)roots.size();
            const T step = T(3).pow((T::mod() - 1) / (2 * m));
            roots.resize(2 * m);
            for (int i = 0; i < m; ++i) roots[m + i] = roots[i] * step;
        }
        return roots;
    }
    static std::vector<F> product_tree(const std::vector<T>& xs, int& size, int block,
                                       bool reciprocal,
                                       std::vector<std::vector<T>>* frequency = nullptr) {
        const int groups = ((int)xs.size() + block - 1) / block;
        while (size < groups) size <<= 1;
        std::vector<F> tree(2 * size);
        if (frequency) frequency->resize(2 * size);
        for (int i = 0; i < size; ++i) {
            if (i >= groups) {
                tree[size + i] = {1};
                continue;
            }
            F p{1};
            for (int j = i * block; j < std::min<int>((i + 1) * block, xs.size()); ++j) {
                const int d = (int)p.size() - 1;
                if (reciprocal) {
                    p.push_back(0);
                    for (int k = d + 1; k > 0; --k) p[k] -= xs[j] * p[k - 1];
                } else {
                    p.push_back(p.back());
                    for (int k = d; k > 0; --k) p[k] = p[k - 1] - xs[j] * p[k];
                    p[0] *= -xs[j];
                }
            }
            tree[size + i] = std::move(p);
        }
        for (int i = size - 1; i > 0; --i) {
            if (tree[2 * i + 1].size() == 1 && tree[2 * i + 1][0] == T(1)) {
                tree[i] = tree[2 * i];
                continue;
            }
            int depth = 31 - __builtin_clz((unsigned)i);
            int length = (size >> depth) * block;
            if (frequency && length >= 32) {
                auto& a = (*frequency)[2 * i];
                auto& b = (*frequency)[2 * i + 1];
                auto transform = [&](base& a, const F& p) {
                    if (a.empty()) {
                        a.resize(length);
                        std::copy(p.begin(), p.end(), a.begin());
                        atcoder::internal::butterfly(a);
                    } else {
                        const int half = length / 2;
                        base odd(half);
                        std::copy_n(p.begin(), std::min<int>(half, p.size()), odd.begin());
                        if ((int)p.size() > half) odd[0] -= p[half];
                        T power = 1, step = ntt_roots(length)[length / 2];
                        for (T& x : odd) x *= power, power *= step;
                        atcoder::internal::butterfly(odd);
                        a.insert(a.end(), odd.begin(), odd.end());
                    }
                };
                transform(a, tree[2 * i]);
                transform(b, tree[2 * i + 1]);
                std::vector<T> c(length);
                for (int j = 0; j < length; ++j) c[j] = a[j] * b[j];
                if (i > 1) (*frequency)[i] = c;
                atcoder::internal::butterfly_inv(c);
                const int n = (int)(tree[2 * i].size() + tree[2 * i + 1].size() - 1);
                tree[i].resize(n);
                const T inv = T::raw(T::mod() - (T::mod() - 1) / length);
                for (int j = 0; j < std::min(n, length); ++j) tree[i][j] = c[j] * inv;
                if (n == length + 1) {
                    const T lead = tree[2 * i].back() * tree[2 * i + 1].back();
                    tree[i][0] -= lead;
                    tree[i][length] = lead;
                }
            } else {
                tree[i] = multiply(tree[2 * i], tree[2 * i + 1]);
            }
        }
        return tree;
    }
    void trim() {
        while (!this->empty() && this->back() == T(0)) this->pop_back();
    }
    static const std::vector<T>& inverses(int n) {
        assert(n < T::mod());
        static std::vector<T> inv{0, 1};
        while ((int)inv.size() <= n) {
            const int i = (int)inv.size();
            inv.push_back(-inv[T::mod() % i] * (T::mod() / i));
        }
        return inv;
    }
};

using fps = FormalPowerSeries<atcoder::modint998244353>;

#endif
#ifndef FPS_YOSUPO_IO_HPP
#define FPS_YOSUPO_IO_HPP
#include <charconv>

struct FastIO {
    static constexpr int capacity = 1 << 16;
    char input[capacity], output[capacity];
    int pos = 0, length = 0, used = 0;
    ~FastIO() { flush(); }
    int next() {
        if (pos == length) {
            length = std::fread(input, 1, capacity, stdin);
            pos = 0;
            if (!length) return EOF;
        }
        return static_cast<unsigned char>(input[pos++]);
    }
    long long read() {
        long long x = 0;
        int c;
        do c = next(); while (c <= ' ' && c != EOF);
        while ('0' <= c && c <= '9') x = 10 * x + c - '0', c = next();
        return x;
    }
    fps poly(int n) {
        fps f(n);
        for (auto& x : f) x = atcoder::modint998244353::raw(read());
        return f;
    }
    fps sparse(int n, int k) {
        fps f(n);
        for (int j = 0; j < k; ++j) {
            int i = read();
            f[i] = atcoder::modint998244353::raw(read());
        }
        return f;
    }
    void flush() {
        if (used) std::fwrite(output, 1, used, stdout);
        used = 0;
    }
    void write(long long x, char end = '\n') {
        if (used + 32 > capacity) flush();
        used = std::to_chars(output + used, output + capacity, x).ptr - output;
        output[used++] = end;
    }
    template <class V>
    void print(const V& a) {
        if (a.empty()) {
            if (used == capacity) flush();
            output[used++] = '\n';
        }
        for (int i = 0; i < (int)a.size(); ++i)
            write(a[i].val(), i + 1 == (int)a.size() ? '\n' : ' ');
    }
};
#endif

int main() {
    FastIO io;
    int n = io.read();
    auto f = io.poly(n);
    auto g = io.poly(n);
    io.print(f.compose(g, n));
}
