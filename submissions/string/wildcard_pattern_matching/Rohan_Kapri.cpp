#line 1 "cp-algorithms-aux/verify/poly/wildcard.test.cpp"
// @brief Wildcard Pattern Matching
#define PROBLEM "https://judge.yosupo.jp/problem/wildcard_pattern_matching"
#pragma GCC optimize("Ofast,unroll-loops")
#define CP_ALGO_CHECKPOINT
#line 1 "cp-algorithms-aux/cp-algo/math/cvector.hpp"


#line 1 "cp-algorithms-aux/cp-algo/util/simd.hpp"


#include <experimental/simd>
#include <cstdint>
#include <cstddef>
namespace cp_algo {
    template<typename T, size_t len>
    using simd [[gnu::vector_size(len * sizeof(T))]] = T;
    using i64x4 = simd<int64_t, 4>;
    using u64x4 = simd<uint64_t, 4>;
    using u32x8 = simd<uint32_t, 8>;
    using i32x4 = simd<int32_t, 4>;
    using u32x4 = simd<uint32_t, 4>;
    using dx4 = simd<double, 4>;

    [[gnu::always_inline]] inline dx4 abs(dx4 a) {
    return a < 0 ? -a : a;
    }

    // https://stackoverflow.com/a/77376595
    // works for ints in (-2^51, 2^51)
    static constexpr dx4 magic = dx4() + (3ULL << 51);
    [[gnu::always_inline]] inline i64x4 lround(dx4 x) {
        return i64x4(x + magic) - i64x4(magic);
    }
    [[gnu::always_inline]] inline dx4 to_double(i64x4 x) {
        return dx4(x + i64x4(magic)) - magic;
    }

    [[gnu::always_inline]] inline dx4 round(dx4 a) {
        return dx4{
            std::nearbyint(a[0]),
            std::nearbyint(a[1]),
            std::nearbyint(a[2]),
            std::nearbyint(a[3])
        };
    }

    [[gnu::always_inline]] inline u64x4 montgomery_reduce(u64x4 x, u64x4 mod, u64x4 imod) {
        auto x_ninv = u64x4(u32x8(x) * u32x8(imod));
#ifdef __AVX2__
        x += u64x4(_mm256_mul_epu32(__m256i(x_ninv), __m256i(mod)));
#else
        x += x_ninv * mod;
#endif
        return x >> 32;
    }

    [[gnu::always_inline]] inline u64x4 montgomery_mul(u64x4 x, u64x4 y, u64x4 mod, u64x4 imod) {
#ifdef __AVX2__
        return montgomery_reduce(u64x4(_mm256_mul_epu32(__m256i(x), __m256i(y))), mod, imod);
#else
        return montgomery_reduce(x * y, mod, imod);
#endif
    }

    [[gnu::always_inline]] inline dx4 rotate_right(dx4 x) {
        static constexpr u64x4 shuffler = {3, 0, 1, 2};
        return __builtin_shuffle(x, shuffler);
    }
}

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
        T const real() const {return x;}
        T const imag() const {return y;}
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
    template<typename T> T const real(complex<T> const& x) {return x.real();}
    template<typename T> T const imag(complex<T> const& x) {return x.imag();}
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

#line 7 "cp-algorithms-aux/cp-algo/math/cvector.hpp"
#include <ranges>
#include <bit>

namespace stdx = std::experimental;
namespace cp_algo::math::fft {
    static constexpr size_t flen = 4;
    using ftype = double;
    using vftype = dx4;
    using point = complex<ftype>;
    using vpoint = complex<vftype>;
    static constexpr vftype vz = {};
    vpoint vi(vpoint const& r) {
        return {-imag(r), real(r)};
    }

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
        static constexpr size_t eval_arg(size_t n) {
            if(n < pre_evals) {
                return eval_args[n];
            } else {
                return eval_arg(n / 2) | (n & 1) << (std::bit_width(n) - 1);
            }
        }
        static constexpr point eval_point(size_t n) {
            if(n % 2) {
                return -eval_point(n - 1);
            } else if(n % 4) {
                return eval_point(n - 2) * point(0, 1);
            } else if(n / 4 < pre_evals) {
                return evalp[n / 4];
            } else {
                return polar<ftype>(1., std::numbers::pi / (ftype)std::bit_floor(n) * (ftype)eval_arg(n));
            }
        }
        static constexpr std::array<point, 32> roots = []() {
            std::array<point, 32> res;
            for(size_t i = 2; i < 32; i++) {
                res[i] = polar<ftype>(1., std::numbers::pi / (1ull << (i - 2)));
            }
            return res;
        }();
        static constexpr point root(size_t n) {
            return roots[std::bit_width(n)];
        }
        template<int step>
        static void exec_on_eval(size_t n, size_t k, auto &&callback) {
            callback(k, root(4 * step * n) * eval_point(step * k));
        }
        template<int step>
        static void exec_on_evals(size_t n, auto &&callback) {
            point factor = root(4 * step * n);
            for(size_t i = 0; i < n; i++) {
                callback(i, factor * eval_point(step * i));
            }
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
                    real(Bv) = rotate_right(real(Bv));
                    imag(Bv) = rotate_right(imag(Bv));
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
            bool parity = std::countr_zero(n) % 2;
            if(parity) {
                exec_on_evals<2>(n / (2 * flen), [&](size_t k, point rt) {
                    k *= 2 * flen;
                    vpoint cvrt = {vz + real(rt), vz - imag(rt)};
                    auto B = at(k) - at(k + flen);
                    at(k) += at(k + flen);
                    at(k + flen) = B * cvrt;
                });
            }

            for(size_t leaf = 3 * flen; leaf < n; leaf += 4 * flen) {
                size_t level = std::countr_one(leaf + 3);
                for(size_t lvl = 4 + parity; lvl <= level; lvl += 2) {
                    size_t i = (1 << lvl) / 4;
                    exec_on_eval<4>(n >> lvl, leaf >> lvl, [&](size_t k, point rt) {
                        k <<= lvl;
                        vpoint v1 = {vz + real(rt), vz - imag(rt)};
                        vpoint v2 = v1 * v1;
                        vpoint v3 = v1 * v2;
                        for(size_t j = k; j < k + i; j += flen) {
                            auto A = at(j);
                            auto B = at(j + i);
                            auto C = at(j + 2 * i);
                            auto D = at(j + 3 * i);
                            at(j) = ((A + B) + (C + D));
                            at(j + 2 * i) = ((A + B) - (C + D)) * v2;
                            at(j +     i) = ((A - B) - vi(C - D)) * v1;
                            at(j + 3 * i) = ((A - B) + vi(C - D)) * v3;
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
            bool parity = std::countr_zero(n) % 2;
            for(size_t leaf = 0; leaf < n; leaf += 4 * flen) {
                size_t level = std::countr_zero(n + leaf);
                level -= level % 2 != parity;
                for(size_t lvl = level; lvl >= 4; lvl -= 2) {
                    size_t i = (1 << lvl) / 4;
                    exec_on_eval<4>(n >> lvl, leaf >> lvl, [&](size_t k, point rt) {
                        k <<= lvl;
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
                            at(j + 2 * i) = (A - C) + vi(B - D);
                            at(j + 3 * i) = (A - C) - vi(B - D);
                        }
                    });
                }
            }
            if(parity) {
                exec_on_evals<2>(n / (2 * flen), [&](size_t k, point rt) {
                    k *= 2 * flen;
                    vpoint vrt = {vz + real(rt), vz + imag(rt)};
                    auto t = at(k + flen) * vrt;
                    at(k + flen) = at(k) - t;
                    at(k) += t;
                });
            }
            checkpoint("fft");
        }
        static constexpr size_t pre_evals = 1 << 16;
        static const std::array<size_t, pre_evals> eval_args;
        static const std::array<point, pre_evals> evalp;
    };

    const std::array<size_t, cvector::pre_evals> cvector::eval_args = []() {
        std::array<size_t, pre_evals> res = {};
        for(size_t i = 1; i < pre_evals; i++) {
            res[i] = res[i >> 1] | (i & 1) << (std::bit_width(i) - 1);
        }
        return res;
    }();
    const std::array<point, cvector::pre_evals> cvector::evalp = []() {
        std::array<point, pre_evals> res = {};
        res[0] = 1;
        for(size_t n = 1; n < pre_evals; n++) {
            res[n] = polar<ftype>(1., std::numbers::pi * ftype(eval_args[n]) / ftype(4 * std::bit_floor(n)));
        }
        return res;
    }();
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

auto is_integer(auto a) {
    static const ftype eps = 1e-8;
    return cp_algo::abs(a - cp_algo::round(a)) < eps;
}

void matches(char *input, size_t n, size_t m, char *output, char wild = '*') {
    static ftype project[2][128];
    static bool init = false;
    if(!init) {
        init = true;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(.5, 2.);
        for(int i = 0; i < 128; i++) {
            ftype x = dis(gen);
            project[0][i] = x;
            project[1][i] = 1. / x;
        }
    }
    project[0][(int)wild] = project[1][(int)wild] = 0;
    vector<cvector> P;
    P.emplace_back((n + 1) / 2);
    P.emplace_back((n + 1) / 2);
    int N = P[0].size();
    auto assign = [&](int z) {
        return [&, z](auto ic) {
            auto [i, c] = ic;
            if(i < N) {
                real(P[z].r[i / fft::flen])[i % fft::flen] = project[z][(int)c];
            } else {
                i -= N;
                imag(P[z].r[i / fft::flen])[i % fft::flen] = project[z][(int)c];
            }
        };
    };
    ranges::for_each(span(input, input + n) | views::enumerate, assign(0));
    ranges::for_each(span(input + n + 1, input + n + 1 + m) | views::reverse | views::enumerate, assign(1));
    cp_algo::checkpoint("cvector fill");
    semicorr(P[0], P[1]);
    int start = (m - 1) / fft::flen * fft::flen;
    for(int j = start; j < n; j += fft::flen) {
        decltype(is_integer(real(P[0].at(j)))) check;
        if(j < N) {
            check = is_integer(real(P[0].at(j)));
        } else {
            check = is_integer(imag(P[0].at(j - N)));
        }
        for(int z = 0; z < 4; z++) {
            output[j - (m - 1) + z] = '0' + (bool)check[z];
        }
    }
    cp_algo::checkpoint("fill answer");
}

#include <fcntl.h>
#include <sys/mman.h>

auto make_output(size_t n, size_t m) {
    int stdout_fd = open("/proc/self/fd/1", O_RDWR);
    dup2(stdout_fd, 1);
    close(stdout_fd);
    ftruncate(1, n - m + 1);
    size_t file_size = (n - m + 1 + 4095) & -4096;
    auto ans_base = (char*)mmap(NULL, 8192 + file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    mmap(ans_base + 4096, file_size, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, 1, 0);
    return ans_base + 4096;
}

void solve() {
    fseek(stdin, 0, 2);
    auto *input = (char*)mmap(NULL, ftell(stdin), PROT_READ, MAP_PRIVATE, 0, 0);
    size_t n = strchr(input, '\n') - input;
    size_t m = strchr(input + n + 1, '\n') - input - n - 1;
    cp_algo::checkpoint("input");
    matches(input, n, m, make_output(n, m));
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
