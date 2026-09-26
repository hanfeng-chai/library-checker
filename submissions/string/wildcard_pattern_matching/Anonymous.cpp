#line 1 "cp-algorithms-aux/verify/poly/wildcard.test.cpp"
// @brief Wildcard Pattern Matching
#define PROBLEM "https://judge.yosupo.jp/problem/wildcard_pattern_matching"
#pragma GCC optimize("Ofast,unroll-loops")
#define CP_ALGO_CHECKPOINT
#line 1 "cp-algorithms-aux/cp-algo/math/cvector.hpp"


#line 1 "cp-algorithms-aux/cp-algo/util/complex.hpp"


#include <iostream>
#include <cmath>
namespace cp_algo {
    // Custom implementation, since std::complex is UB on non-floating types
    template<typename T>
    struct complex {
        using value_type = T;
        T x, y;
        constexpr complex(): x(), y() {}
        constexpr complex(T x): x(x), y() {}
        constexpr complex(T x, T y): x(x), y(y) {}
        complex& operator *= (T t) {x *= t; y *= t; return *this;}
        complex& operator /= (T t) {x /= t; y /= t; return *this;}
        complex operator * (T t) const {return complex(*this) *= t;}
        complex operator / (T t) const {return complex(*this) /= t;}
        complex& operator += (complex t) {x += t.x; y += t.y; return *this;}
        complex& operator -= (complex t) {x -= t.x; y -= t.y; return *this;}
        complex operator * (complex t) const {return {x * t.x - y * t.y, x * t.y + y * t.x};}
        complex operator / (complex t) const {return *this * t.conj() / t.norm();}
        complex operator + (complex t) const {return complex(*this) += t;}
        complex operator - (complex t) const {return complex(*this) -= t;}
        complex& operator *= (complex t) {return *this = *this * t;}
        complex& operator /= (complex t) {return *this = *this / t;}
        complex operator - () const {return {-x, -y};}
        complex conj() const {return {x, -y};}
        T norm() const {return x * x + y * y;}
        T abs() const {return std::sqrt(norm());}
        T real() const {return x;}
        T imag() const {return y;}
        T& real() {return x;}
        T& imag() {return y;}
        static constexpr complex polar(T r, T theta) {return {r * cos(theta), r * sin(theta)};}
        auto operator <=> (complex const& t) const = default;
    };
    template<typename T>
    complex<T> operator * (auto x, complex<T> y) {return y *= x;}
    template<typename T> complex<T> conj(complex<T> x) {return x.conj();}
    template<typename T> T norm(complex<T> x) {return x.norm();}
    template<typename T> T abs(complex<T> x) {return x.abs();}
    template<typename T> T& real(complex<T> &x) {return x.real();}
    template<typename T> T& imag(complex<T> &x) {return x.imag();}
    template<typename T> T real(complex<T> const& x) {return x.real();}
    template<typename T> T imag(complex<T> const& x) {return x.imag();}
    template<typename T>
    constexpr complex<T> polar(T r, T theta) {
        return complex<T>::polar(r, theta);
    }
    template<typename T>
    std::ostream& operator << (std::ostream &out, complex<T> x) {
        return out << x.real() << ' ' << x.imag();
    }
}

#line 1 "cp-algorithms-aux/cp-algo/util/checkpoint.hpp"


#line 4 "cp-algorithms-aux/cp-algo/util/checkpoint.hpp"
#include <chrono>
#include <string>
namespace cp_algo {
    template<bool final = false>
    void checkpoint([[maybe_unused]] std::string const& msg = "") {
#ifdef CP_ALGO_CHECKPOINT
        static double last = 0;
        double now = (double)clock() / CLOCKS_PER_SEC;
        double delta = now - last;
        last = now;
        if(msg.size()) {
            std::cerr << msg << ": " << (final ? now : delta) * 1000 << " ms\n";
        }
#endif
    }
}

#line 1 "cp-algorithms-aux/cp-algo/util/big_alloc.hpp"



#include <cstddef>
#line 6 "cp-algorithms-aux/cp-algo/util/big_alloc.hpp"

// Single macro to detect POSIX platforms (Linux, Unix, macOS)
#if defined(__linux__) || defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#  define CP_ALGO_USE_MMAP 1
#  include <sys/mman.h>
#else
#  define CP_ALGO_USE_MMAP 0
#endif

namespace cp_algo {
    template <typename T>
    class big_alloc: public std::allocator<T> {
        public:
        using value_type = T;
        using base = std::allocator<T>;

        big_alloc() noexcept = default;

        template <typename U>
        big_alloc(const big_alloc<U>&) noexcept {}

#if CP_ALGO_USE_MMAP
        [[nodiscard]] T* allocate(std::size_t n) {
            if(n * sizeof(T) < 1024 * 1024) {
                return base::allocate(n);
            }
            n *= sizeof(T);
            void* raw = mmap(nullptr, n,
                            PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS,
                            -1, 0);
            madvise(raw, n, MADV_HUGEPAGE);
            madvise(raw, n, MADV_POPULATE_WRITE);
            return static_cast<T*>(raw);
        }
#endif

#if CP_ALGO_USE_MMAP
        void deallocate(T* p, std::size_t n) noexcept {
            if(n * sizeof(T) < 1024 * 1024) {
                return base::deallocate(p, n);
            }
            if(p) {
                munmap(p, n * sizeof(T));
            }
        }
#endif
    };
}

#line 6 "cp-algorithms-aux/cp-algo/math/cvector.hpp"
#include <experimental/simd>
#include <ranges>

namespace stdx = std::experimental;
namespace cp_algo::math::fft {
    using ftype = double;
    static constexpr size_t flen = 4;
    static constexpr size_t bytes = flen * sizeof(ftype);
    using point = complex<ftype>;
    using vftype [[gnu::vector_size(bytes)]] = ftype;
    using vpoint = complex<vftype>;
    static constexpr vftype vz = {};
    static constexpr vpoint vi = {vz, vz + 1};

    struct cvector {
        std::vector<vpoint, big_alloc<vpoint>> r;
        cvector(size_t n) {
            n = std::max(flen, std::bit_ceil(n));
            r.resize(n / flen);
            checkpoint("cvector create");
        }

        vpoint& at(size_t k) {return r[k / flen];}
        vpoint at(size_t k) const {return r[k / flen];}
        template<class pt = point>
        void set(size_t k, pt t) {
            if constexpr(std::is_same_v<pt, point>) {
                real(r[k / flen])[k % flen] = real(t);
                imag(r[k / flen])[k % flen] = imag(t);
            } else {
                at(k) = t;
            }
        }
        template<class pt = point>
        pt get(size_t k) const {
            if constexpr(std::is_same_v<pt, point>) {
                return {real(r[k / flen])[k % flen], imag(r[k / flen])[k % flen]};
            } else {
                return at(k);
            }
        }

        size_t size() const {
            return flen * r.size();
        }
        static size_t eval_arg(size_t n) {
            if(n < pre_evals) {
                return eval_args[n];
            } else {
                return eval_arg(n / 2) | (n & 1) << (std::bit_width(n) - 1);
            }
        }
        static auto root(size_t n, size_t k) {
            if(n < pre_roots) {
                return roots[n + k];
            } else if (k % 2 == 0) {
                return root(n / 2, k / 2);
            } else {
                return polar(1., std::numbers::pi / (ftype)n * (ftype)k);
            }
        }
        static point eval_point(size_t n) {
            if(n % 2) {
                return -eval_point(n - 1);
            } else if(n % 4) {
                return eval_point(n - 2) * point(0, 1);
            } else if(n / 4 < pre_evals) {
                return evalp[n / 4];
            } else {
                return root(std::bit_floor(n), eval_arg(n));
            }
        }
        static void exec_on_roots(size_t n, size_t m, auto &&callback) {
            point cur = {1, 0};
            point arg = root(n, 1);
            for(size_t i = 0; i < m; i++) {
                callback(i, cur);
                if(i % 64 == 63) {
                    cur = root(n / 64, i / 64 + 1);
                } else {
                    cur *= arg;
                }
            }
        }
        template<int step>
        static void exec_on_evals(size_t n, auto &&callback) {
            point factor = root(2 * step * n, 1);
            for(size_t i = 0; i < n; i++) {
                callback(i, factor * eval_point(step * i));
            }
        }
        static auto dot_block(size_t k, cvector const& A, cvector const& B) {
            auto rt = eval_point(k / flen / 2);
            if(k / flen % 2) {
                rt = -rt;
            }
            auto [Ax, Ay] = A.at(k);
            auto Bv = B.at(k);
            vpoint res = vz;
            for (size_t i = 0; i < flen; i++) {
                res += vpoint(vz + Ax[i], vz + Ay[i]) * Bv;
                real(Bv) = __builtin_shufflevector(real(Bv), real(Bv), 3, 0, 1, 2);
                imag(Bv) = __builtin_shufflevector(imag(Bv), imag(Bv), 3, 0, 1, 2);
                auto x = real(Bv)[0], y = imag(Bv)[0];
                real(Bv)[0] = x * real(rt) - y * imag(rt);
                imag(Bv)[0] = x * imag(rt) + y * real(rt);
            }
            return res;
        }

        void dot(cvector const& t) {
            size_t n = this->size();
            exec_on_evals<1>(n / flen, [&](size_t k, point rt) {
                k *= flen;
                auto [Ax, Ay] = at(k);
                auto Bv = t.at(k);
                vpoint res = vz;
                for (size_t i = 0; i < flen; i++) {
                    res += vpoint(vz + Ax[i], vz + Ay[i]) * Bv;
                    real(Bv) = __builtin_shufflevector(real(Bv), real(Bv), 3, 0, 1, 2);
                    imag(Bv) = __builtin_shufflevector(imag(Bv), imag(Bv), 3, 0, 1, 2);
                    auto x = real(Bv)[0], y = imag(Bv)[0];
                    real(Bv)[0] = x * real(rt) - y * imag(rt);
                    imag(Bv)[0] = x * imag(rt) + y * real(rt);
                }
                set(k, res);
            });
            checkpoint("dot");
        }

        void ifft() {
            size_t n = size();
            for(size_t i = flen; i <= n / 2; i *= 2) {
                if (4 * i <= n) { // radix-4
                    exec_on_evals<4>(n / (4 * i), [&](size_t k, point rt) {
                        k *= 4 * i;
                        vpoint v1 = {vz + real(rt), vz - imag(rt)};
                        vpoint v2 = v1 * v1;
                        vpoint v3 = v1 * v2;
                        for(size_t j = k; j < k + i; j += flen) {
                            auto A = at(j);
                            auto B = at(j + i);
                            auto C = at(j + 2 * i);
                            auto D = at(j + 3 * i);
                            at(j) = (A + B + C + D);
                            at(j + 2 * i) = (A + B - C - D) * v2;
                            at(j +     i) = (A - B - vi * (C - D)) * v1;
                            at(j + 3 * i) = (A - B + vi * (C - D)) * v3;
                        }
                    });
                    i *= 2;
                } else { // radix-2 fallback
                    exec_on_evals<2>(n / (2 * i), [&](size_t k, point rt) {
                        k *= 2 * i;
                        vpoint cvrt = {vz + real(rt), vz - imag(rt)};
                        for(size_t j = k; j < k + i; j += flen) {
                            auto B = at(j) - at(j + i);
                            at(j) += at(j + i);
                            at(j + i) = B * cvrt;
                        }
                    });
                }
            }
            checkpoint("ifft");
            for(size_t k = 0; k < n; k += flen) {
                set(k, get<vpoint>(k) /= vz + (ftype)(n / flen));
            }
        }
        void fft() {
            size_t n = size();
            for(size_t i = n / 2; i >= flen; i /= 2) {
                if (i / 2 >= flen) { // radix-4
                    i /= 2;
                    exec_on_evals<4>(n / (4 * i), [&](size_t k, point rt) {
                        k *= 4 * i;
                        vpoint v1 = {vz + real(rt), vz + imag(rt)};
                        vpoint v2 = v1 * v1;
                        vpoint v3 = v1 * v2;
                        for(size_t j = k; j < k + i; j += flen) {
                            auto A = at(j);
                            auto B = at(j + i) * v1;
                            auto C = at(j + 2 * i) * v2;
                            auto D = at(j + 3 * i) * v3;
                            at(j)         = (A + C) + (B + D);
                            at(j + i)     = (A + C) - (B + D);
                            at(j + 2 * i) = (A - C) + vi * (B - D);
                            at(j + 3 * i) = (A - C) - vi * (B - D);
                        }
                    });
                } else { // radix-2 fallback
                    exec_on_evals<2>(n / (2 * i), [&](size_t k, point rt) {
                        k *= 2 * i;
                        vpoint vrt = {vz + real(rt), vz + imag(rt)};
                        for(size_t j = k; j < k + i; j += flen) {
                            auto t = at(j + i) * vrt;
                            at(j + i) = at(j) - t;
                            at(j) += t;
                        }
                    });
                }
            }
            checkpoint("fft");
        }
        static constexpr size_t pre_roots = 1 << 14;
        static constexpr size_t pre_evals = 1 << 16;
        static constexpr std::array<point, pre_roots> roots = []() {
            std::array<point, pre_roots> res = {};
            for(size_t n = 1; n < res.size(); n *= 2) {
                for(size_t k = 0; k < n; k++) {
                    res[n + k] = polar(1., std::numbers::pi / ftype(n) * ftype(k));
                }
            }
            return res;
        }();
        static constexpr std::array<size_t, pre_evals> eval_args = []() {
            std::array<size_t, pre_evals> res = {};
            for(size_t i = 1; i < pre_evals; i++) {
                res[i] = res[i >> 1] | (i & 1) << (std::bit_width(i) - 1);
            }
            return res;
        }();
        static constexpr std::array<point, pre_evals> evalp = []() {
            std::array<point, pre_evals> res = {};
            res[0] = 1;
            for(size_t n = 1; n < pre_evals; n++) {
                res[n] = polar(1., std::numbers::pi * ftype(eval_args[n]) / ftype(4 * std::bit_floor(n)));
            }
            return res;
        }();
    };
}

#line 1 "cp-algorithms-aux/cp-algo/random/rng.hpp"


#line 4 "cp-algorithms-aux/cp-algo/random/rng.hpp"
#include <random>
namespace cp_algo::random {
    uint64_t rng() {
        static std::mt19937_64 rng(
            std::chrono::steady_clock::now().time_since_epoch().count()
        );
        return rng();
    }
}

#line 7 "cp-algorithms-aux/verify/poly/wildcard.test.cpp"
#include <bits/stdc++.h>

using namespace std;
using namespace cp_algo::math;

using fft::ftype;
using fft::point;
using fft::vftype;
using fft::cvector;

void semicorr(auto &a, auto &b) {
    a.fft();
    b.fft();
    a.dot(b);
    a.ifft();
}

vftype abs(vftype a) {
    return a < 0 ? -a : a;
}

using v4di [[gnu::vector_size(32)]] = long;

auto round(vftype a) {
    return __builtin_convertvector(__builtin_convertvector(a < 0 ? a - 0.5 : a + 0.5, v4di), vftype);
}

auto is_integer(auto a) {
    static const double eps = 1e-8;
    return abs(imag(a)) < eps
        && abs(real(a) - round(real(a))) < eps;
}

string matches(string const& A, string const& B, char wild = '*') {
    static point project[2][128];
    static bool init = false;
    if(!init) {
        init = true;
        for(int i = 0; i < 128; i++) {
            project[0][i] = cp_algo::polar(1., (ftype)cp_algo::random::rng());
            project[1][i] = conj(project[0][i]);
        }
    }
    project[0][(int)wild] = project[1][(int)wild] = 0;
    vector<cvector> P;
    P.emplace_back(size(A));
    P.emplace_back(size(A));
    for(auto [i, c]: A | views::enumerate) {
        P[0].set(i, project[0][(int)c]);
    }
    for(auto [i, c]: B | views::reverse | views::enumerate) {
        P[1].set(i, project[1][(int)c]);
    }
    cp_algo::checkpoint("cvector fill");
    semicorr(P[0], P[1]);
    string ans(size(P[0]), '0');
    auto start = (size(B) - 1) / fft::flen * fft::flen;
    for(size_t j = start; j < size(ans); j += fft::flen) {
        auto r = P[0].at(j);
        auto check = is_integer(r);
        for(int z = 0; z < 4; z++) {
            ans[j + z] ^= (bool)check[z];
        }
    }
    cp_algo::checkpoint("fill answer");
    return ans.substr(size(B) - 1, size(A) - size(B) + 1);
}

void solve() {
    string a, b;
    cin >> a >> b;
    cp_algo::checkpoint("input");
    cout << matches(a, b) << "\n";
    cp_algo::checkpoint("output");
    cp_algo::checkpoint<true>("done");
}

signed main() {
    //freopen("input.txt", "r", stdin);
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
    //cin >> t;
    while(t--) {
        solve();
    }
}
