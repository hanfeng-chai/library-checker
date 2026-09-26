/* This code was formatted by `clang-format` and `online-judge-extension`. */
/* This code was bundled by `oj-bundle` and `online-judge-extension`. */
#if !defined(__clang__) && defined(__GNUC__)
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#endif
#ifdef EVAL
#define ONLINE_JUDGE
#endif
#pragma GCC diagnostic ignored "-Wsign-compare"
#ifndef _GLIBCXX_NO_ASSERT
#include <cassert>
#endif
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cinttypes>
#include <cstdint>
#include <algorithm>
#include <bitset>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>
#include <array>
#include <chrono>
#include <initializer_list>
#include <random>
#include <regex>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <charconv>
#include <bit>
#include <compare>
#include <concepts>
#include <ranges>
#include <span>
#include <source_location>
#if true || defined(INCLUDE_UTILITY) || defined(INCLUDE_ALL)
class fprec {
int d;
public:
fprec(int d_): d(d_) {}
template<class T> friend auto& operator<<(T& os, fprec fp) {
os << std::fixed << std::setprecision(fp.d);
return os;
}
};
template<class T> void make_unique(T& v) {
std::sort(v.begin(), v.end());
v.erase(std::unique(v.begin(), v.end()), v.end());
}
template<class T> constexpr T reversed(const T& x) {
T res= x;
std::reverse(std::begin(res), std::end(res));
return res;
}
template<std::ranges::range T, class U> constexpr int find_loc(const T& x, const U& t) {
int cnt= 0;
for(const auto& y: x) {
if(t == y) return cnt;
++cnt;
}
return -1;
}
template<std::ranges::range T> constexpr auto unique_element(const T& x) {
using U= std::decay_t<decltype(std::ranges::cbegin(x))>;
std::map<U, size_t, decltype([](const U& a, const U& b) { return *a < *b; })> m;
for(U itr= std::ranges::cbegin(x), sen= std::ranges::cend(x); itr != sen; ++itr) ++m[itr];
for(const auto& [k, v]: m)
if(v == 1) return k;
return std::ranges::cend(x);
}
#if __GNUC__ >= 12
class bitwise_search {
size_t n;
public:
constexpr bitwise_search(size_t sz): n(sz) {}
class end_iterator {};
class iterator {
friend class bitwise_search;
size_t n, bit= 0;
constexpr iterator(size_t sz): n(sz) {}
public:
constexpr void operator++() { ++bit; }
constexpr std::basic_string<bool> operator*() const {
std::basic_string<bool> res(n, false);
for(size_t copy= bit, i= 0; copy != 0; copy>>= 1, ++i) res[i]= copy & 1;
return res;
}
constexpr bool operator!=(end_iterator) const { return bit != (1ull << n); }
};
constexpr iterator begin() { return iterator(n); }
constexpr end_iterator end() { return end_iterator(); }
};
class permutation_search {
size_t n;
public:
constexpr permutation_search(size_t sz): n(sz) {}
class end_iterator {};
class iterator {
friend class permutation_search;
std::vector<size_t> p;
bool f= true;
constexpr iterator(size_t sz): p(sz) { std::iota(p.begin(), p.end(), 0); }
public:
constexpr void operator++() { f= std::next_permutation(p.begin(), p.end()); }
constexpr std::vector<size_t> operator*() const { return p; }
constexpr bool operator!=(end_iterator) const { return f; }
};
constexpr iterator begin() { return iterator(n); }
constexpr end_iterator end() { return end_iterator(); }
};
class combination_search {
size_t n, b;
public:
constexpr combination_search(size_t sz, size_t bound): n(sz), b(bound) {}
class end_iterator {};
class iterator {
friend class combination_search;
std::vector<size_t> p;
size_t b;
bool f= true;
constexpr iterator(size_t sz, size_t bound): p(sz), b(bound) { std::fill(p.end() - (sz - b), p.end(), 1); }
public:
constexpr void operator++() { f= std::next_permutation(p.begin(), p.end()); }
constexpr std::vector<size_t> operator*() const {
std::vector<size_t> res;
res.reserve(b);
for(size_t i= 0, sz= p.size(); i < sz; ++i)
if(!p[i]) res.push_back(i);
return res;
}
constexpr bool operator!=(end_iterator) const { return f; }
};
constexpr iterator begin() { return iterator(n, b); }
constexpr end_iterator end() { return end_iterator(); }
};
#endif
#endif
#if true || defined(INCLUDE_FASTIO)
/*
#include <unistd.h>
#include <immintrin.h>
// Thanks for https://zenn.dev/mizar/articles/fc87d667153080
class FastIstream : public std::ios_base {
    constexpr static int buffersize = (1 << 18) - 1;
    char buffer[buffersize + 1];
    char* cur = buffer;
    char* eof = buffer;
    inline void reload(ptrdiff_t w) {
        if (eof - w < cur) [[unlikely]] {
            if (eof == buffer + buffersize) [[likely]] {
                ptrdiff_t rem = eof - cur;
                std::memcpy(buffer, cur, rem);
                *(eof = buffer + rem + read(0, buffer + rem, buffersize - rem)) = '\0';
                cur = buffer;
            } else if (eof <= cur) {
                *(eof = buffer + read(0, buffer, buffersize)) = '\0';
                cur = buffer;
            }
        }
    }
public:
    FastIstream& operator>>(bool& n) {
        reload(2);
        n = *cur == '1';
        cur += 2;
        return *this;
    }
    FastIstream& operator>>(short& n) {
        reload(8);
        short neg = (*cur == '-') * -2 + 1;
        cur += neg == -1;
        uint64_t tmp = *(uint64_t*) cur ^ 0x3030303030303030u;
        int clz = std::countl_zero((tmp & 0x1010101010101010u) & (-(tmp & 0x1010101010101010u))) + 5;
        cur += (72 - clz) >> 3;
        tmp = ((tmp << clz) * 0xa01ull) >> 8 & 0x00ff00ff00ff00ffull;
        tmp = (tmp * 0x640001ull) >> 16 & 0x0000ffff0000ffffull;
        n = (short) ((tmp * 0x271000000001ull) >> 32) * neg;
        return *this;
    }
    FastIstream& operator>>(unsigned short& n) {
        reload(8);
        uint64_t tmp = *(uint64_t*) cur ^ 0x3030303030303030u;
        int clz = std::countl_zero((tmp & 0x1010101010101010u) & (-(tmp & 0x1010101010101010u))) + 5;
        cur += (72 - clz) >> 3;
        tmp = ((tmp << clz) * 0xa01ull) >> 8 & 0x00ff00ff00ff00ffull;
        tmp = (tmp * 0x640001ull) >> 16 & 0x0000ffff0000ffffull;
        n = (unsigned short) ((tmp * 0x271000000001ull) >> 32);
        return *this;
    }
    FastIstream& operator>>(unsigned int& n) {
        reload(16);
        uint64_t tmp = *(uint64_t*) cur ^ 0x3030303030303030u, tmp2 = tmp & 0x1010101010101010u;
        if (tmp2) {
            int clz = std::countl_zero(tmp2 & -tmp2) + 5;
            cur += (72 - clz) >> 3;
            tmp = ((tmp << clz) * 0xa01ull) >> 8 & 0x00ff00ff00ff00ffull;
            tmp = (tmp * 0x640001ull) >> 16 & 0x0000ffff0000ffffull;
            n = (unsigned) ((tmp * 0x271000000001ull) >> 32);
        } else {
            cur += 8;
            tmp = (tmp * 0xa01ull) >> 8 & 0x00ff00ff00ff00ffull;
            tmp = (tmp * 0x640001ull) >> 16 & 0x0000ffff0000ffffull;
            n = (unsigned) ((tmp * 0x271000000001ull) >> 32);
            if (char c = *(cur++); c >= '0') {
                n = 10 * n + (c - '0');
                if ((c = *(cur++)) >= '0') n = 10 * n + (c - '0'), ++cur;
            }
        }
        return *this;
    }
    FastIstream& operator>>(int& n) {
        reload(16);
        int neg = (*cur == '-') * -2 + 1;
        cur += neg == -1;
        uint64_t tmp = *(uint64_t*) cur ^ 0x3030303030303030u, tmp2 = tmp & 0x1010101010101010u;
        if (tmp2) {
            int clz = std::countl_zero(tmp2 & -tmp2) + 5;
            cur += (72 - clz) >> 3;
            tmp = ((tmp << clz) * 0xa01ull) >> 8 & 0x00ff00ff00ff00ffull;
            tmp = (tmp * 0x640001ull) >> 16 & 0x0000ffff0000ffffull;
            n = (int) ((tmp * 0x271000000001ull) >> 32);
        } else {
            cur += 8;
            tmp = (tmp * 0xa01ull) >> 8 & 0x00ff00ff00ff00ffull;
            tmp = (tmp * 0x640001ull) >> 16 & 0x0000ffff0000ffffull;
            n = (int) ((tmp * 0x271000000001ull) >> 32);
            if (char c = *(cur++); c >= '0') {
                n = 10 * n + (c - '0');
                if ((c = *(cur++)) >= '0') n = 10 * n + (c - '0'), ++cur;
            }
        }
        n *= neg;
        return *this;
    }
    FastIstream& operator>>(unsigned long long& n) {
        reload(32);
#ifndef __AVX512VL__
        n = 0;
        while (*cur >= '0') n = 10 * n + (*(cur++) - '0');
        ++cur;
#else
        unsigned long long tmp[3], tmp2[3];
        std::memcpy(tmp, cur, 24);
        int width;
        if ((tmp2[0] = (tmp[0] ^= 0x3030303030303030) & 0x1010101010101010)) [[unlikely]] {
            width = std::countr_zero(tmp2[0]) - 4;
            n = ((((((tmp[0] << (64 - width)) * 0xa01ull) >> 8 & 0x00ff00ff00ff00ffull) * 0x640001ull) >> 16 & 0x0000ffff0000ffffull) * 0x271000000001ull) >> 32;
            cur += (width >> 3) + 1;
        } else {
            __m256i tmp3;
            if ((tmp2[1] = (tmp[1] ^= 0x3030303030303030) & 0x1010101010101010)) [[unlikely]] {
                width = 60 + std::countr_zero(tmp2[1]);
                if (width == 64) [[unlikely]]
                    tmp3 = _mm256_setr_epi64x(0, 0, 0, tmp[0]);
                else tmp3 = _mm256_setr_epi64x(0, 0, tmp[0] << (128 - width), tmp[1] << (128 - width) | tmp[0] >> (width - 64));
            } else {
                width = 124 + std::countr_zero((tmp[2] ^= 0x3030303030303030) & 0x1010101010101010);
                if (width == 128) [[unlikely]]
                    tmp3 = _mm256_setr_epi64x(0, 0, tmp[0], tmp[1]);
                else tmp3 = _mm256_setr_epi64x(0, tmp[0] << (192 - width), tmp[1] << (192 - width) | tmp[0] >> (width - 128), tmp[2] << (192 - width) | tmp[1] >> (width - 128));
            }
            cur += (width >> 3) + 1;
            alignas(32) unsigned long long res[4];
            _mm256_store_epi64(res, _mm256_srli_epi64(_mm256_mullo_epi64(_mm256_srli_epi32(_mm256_mullo_epi32(_mm256_srli_epi16(_mm256_mullo_epi16(_mm256_and_si256(tmp3, _mm256_set1_epi8(0x0f)), _mm256_set1_epi16(0xa01)), 8), _mm256_set1_epi32(0x640001)), 16), _mm256_set1_epi64x(0x271000000001)), 32));
            n = res[1] * 10000000000000000 + res[2] * 100000000 + res[3];
        }
#endif
        return *this;
    }
    FastIstream& operator>>(long long& n) {
        reload(32);
        long long neg = (*cur == '-') * -2 + 1;
        cur += neg == -1;
#ifndef __AVX512VL__
        n = 0;
        while (*cur >= '0') n = 10 * n + (*(cur++) - '0');
        ++cur;
        n *= neg;
#else
        unsigned long long tmp[3], tmp2[3];
        std::memcpy(tmp, cur, 24);
        int width;
        if ((tmp2[0] = (tmp[0] ^= 0x3030303030303030) & 0x1010101010101010)) [[unlikely]] {
            width = std::countr_zero(tmp2[0]) - 4;
            n = neg * (((((((tmp[0] << (64 - width)) * 0xa01ull) >> 8 & 0x00ff00ff00ff00ffull) * 0x640001ull) >> 16 & 0x0000ffff0000ffffull) * 0x271000000001ull) >> 32);
            cur += (width >> 3) + 1;
        } else {
            __m256i tmp3;
            if ((tmp2[1] = (tmp[1] ^= 0x3030303030303030) & 0x1010101010101010)) [[unlikely]] {
                width = 60 + std::countr_zero(tmp2[1]);
                if (width == 64) [[unlikely]]
                    tmp3 = _mm256_setr_epi64x(0, 0, 0, tmp[0]);
                else tmp3 = _mm256_setr_epi64x(0, 0, tmp[0] << (128 - width), tmp[1] << (128 - width) | tmp[0] >> (width - 64));
            } else {
                width = 124 + std::countr_zero((tmp[2] ^= 0x3030303030303030) & 0x1010101010101010);
                if (width == 128) [[unlikely]]
                    tmp3 = _mm256_setr_epi64x(0, 0, tmp[0], tmp[1]);
                else tmp3 = _mm256_setr_epi64x(0, tmp[0] << (192 - width), tmp[1] << (192 - width) | tmp[0] >> (width - 128), tmp[2] << (192 - width) | tmp[1] >> (width - 128));
            }
            cur += (width >> 3) + 1;
            alignas(32) long long res[4];
            _mm256_store_epi64(res, _mm256_srli_epi64(_mm256_mullo_epi64(_mm256_srli_epi32(_mm256_mullo_epi32(_mm256_srli_epi16(_mm256_mullo_epi16(_mm256_and_si256(tmp3, _mm256_set1_epi8(0x0f)), _mm256_set1_epi16(0xa01)), 8), _mm256_set1_epi32(0x640001)), 16), _mm256_set1_epi64x(0x271000000001)), 32));
            n = neg * (res[1] * 10000000000000000 + res[2] * 100000000 + res[3]);
        }
#endif
        return *this;
    }
    FastIstream& operator>>(long& n) {
        long long x;
        operator>>(x);
        n = x;
        return *this;
    }
    FastIstream& operator>>(unsigned long& n) {
        unsigned long long x;
        operator>>(x);
        n = x;
        return *this;
    }
    friend FastIstream& operator>>(FastIstream& is, char& c) {
        is.reload(2);
        c = *is.cur;
        is.cur += 2;
        return is;
    }
    friend FastIstream& operator>>(FastIstream& is, unsigned char& c) {
        is.reload(2);
        c = *is.cur;
        is.cur += 2;
        return is;
    }
    friend FastIstream& operator>>(FastIstream& is, signed char& c) {
        is.reload(2);
        c = *is.cur;
        is.cur += 2;
        return is;
    }
    friend FastIstream& operator>>(FastIstream& is, char* s) {
        while (true) {
            while (*is.cur > ' ' && is.cur != is.eof) *(s++) = *is.cur, ++is.cur;
            if (is.cur == is.eof) is.reload(is.buffersize);
            else break;
        }
        ++is.cur;
        *s = '\0';
        return is;
    }
    friend FastIstream& operator>>(FastIstream& is, std::string& s) {
        s.clear();
        while (true) {
            char* st = is.cur;
            while (*is.cur > ' ' && is.cur != is.eof) ++is.cur;
            s += std::string_view(st, is.cur - st);
            if (is.cur == is.eof) is.reload(is.buffersize);
            else break;
        }
        ++is.cur;
        return is;
    }
#ifndef __clang__
    FastIstream& operator>>(float& f) {
        std::string s;
        (*this) >> s;
        std::from_chars(s.c_str(), s.c_str() + s.length(), f);
        return *this;
    }
    FastIstream& operator>>(double& f) {
        std::string s;
        (*this) >> s;
        std::from_chars(s.c_str(), s.c_str() + s.length(), f);
        return *this;
    }
    FastIstream& operator>>(long double& f) {
        std::string s;
        (*this) >> s;
        std::from_chars(s.c_str(), s.c_str() + s.length(), f);
        return *this;
    }
#endif
    template<std::ranges::range T> friend FastIstream& operator>>(FastIstream& is, T& x) {
        for (auto& v : x) is >> v;
        return is;
    }
    char getc() {
        reload(1);
        return *(cur++);
    }
    void seek(int n) {
        reload(n);
        cur += n;
    }
} fin;
class FastOstream : public std::ios_base {
    constexpr static int buffersize = 1 << 18;
    char buffer[buffersize];
    char* cur = buffer;
    inline void reload(ptrdiff_t w) {
        if (buffer + buffersize - w < cur) [[unlikely]] {
            [[maybe_unused]] int r = write(1, buffer, cur - buffer);
            cur = buffer;
        }
    }
    constexpr static std::array<unsigned, 10000> strtable = []() {
        std::array<unsigned, 10000> res;
        for (unsigned i = 0; i < 10000; ++i) {
            unsigned tmp[4];
            unsigned n = i;
            tmp[3] = (n % 10 + '0') << 24, n /= 10;
            tmp[2] = (n % 10 + '0') << 16, n /= 10;
            tmp[1] = (n % 10 + '0') << 8, n /= 10;
            tmp[0] = n % 10 + '0';
            res[i] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        }
        return res;
    }();
    constexpr static std::array<unsigned, 10000> strtable2 = []() {
        std::array<unsigned, 10000> res;
        for (unsigned i = 0; i < 10000; ++i) {
            unsigned tmp[4];
            unsigned n = i;
            if (i < 10) n *= 1000;
            else if (i < 100) n *= 100;
            else if (i < 1000) n *= 10;
            tmp[3] = (n % 10 + '0') << 24, n /= 10;
            tmp[2] = (n % 10 + '0') << 16, n /= 10;
            tmp[1] = (n % 10 + '0') << 8, n /= 10;
            tmp[0] = n % 10 + '0';
            res[i] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        }
        return res;
    }();
    template<class T> void putfloat(T f) {
        bool fixed = flags() & std::ios_base::fixed;
        bool scientific = flags() & std::ios_base::scientific;
        bool uppercase = flags() & std::ios_base::uppercase;
        if (fixed && scientific && (flags() & std::ios_base::showbase)) {
            std::memcpy(cur, (uppercase ? "0X" : "0x"), 2);
            cur += 2;
        }
        std::chars_format fmt = (fixed ? (scientific ? std::chars_format::hex : std::chars_format::fixed) : (scientific ? std::chars_format::scientific : std::chars_format::general));
        auto conv = [&]() {
            return std::to_chars(cur, buffer + buffersize, f, fmt, precision());
        };
        auto [ptr, ec] = conv();
        char* p;
        if (ec == std::errc::value_too_large) {
            reload(buffersize);
            p = cur;
            cur = conv().ptr;
        } else p = cur, cur = ptr;
        if (uppercase) {
            while (p != cur) {
                if (*p > '9') *p -= ('a' - 'A');
                ++p;
            }
        }
    }
public:
    FastOstream() : std::ios_base{} {
        precision(6);
        setf(std::ios_base::showbase);
    }
    ~FastOstream() { reload(buffersize); }
    FastOstream& flush() {
        reload(buffersize);
        return *this;
    }
    char widen(char c) const { return c; }
    FastOstream& put(char c) {
        reload(1);
        *(cur++) = c;
        return *this;
    }
    FastOstream& operator<<(std::basic_ostream<FastOstream, void>& (*pf)(std::basic_ostream<FastOstream, void>&) );
    FastOstream& operator<<(std::basic_ios<FastOstream, void>& (*pf)(std::basic_ios<FastOstream, void>&) );
    FastOstream& operator<<(std::ios_base& (*pf)(std::ios_base&) ) {
        pf(*this);
        return *this;
    }
    FastOstream& operator<<(bool n) {
        if (ios_base::flags() & std::ios_base::boolalpha) {
            if (n) {
                reload(4);
                std::memcpy(cur, "true", 4);
                cur += 4;
            } else {
                reload(5);
                std::memcpy(cur, "false", 5);
                cur += 5;
            }
        } else {
            reload(1);
            *(cur++) = '0' + n;
        }
        return *this;
    }
    FastOstream& operator<<(unsigned short n) {
        reload(5);
        if (n >= 10000) {
            *(cur++) = '0' + n / 10000, n %= 10000;
            *reinterpret_cast<unsigned*>(cur) = strtable[n];
            cur += 4;
        } else if (n >= 1000) {
            *reinterpret_cast<unsigned*>(cur) = strtable[n];
            cur += 4;
        } else if (n >= 100) {
            *reinterpret_cast<unsigned*>(cur) = strtable[n * 10];
            cur += 3;
        } else if (n >= 10) {
            *(cur++) = '0' + n / 10;
            *(cur++) = '0' + n % 10;
        } else *(cur++) = '0' + n;
        return *this;
    }
    FastOstream& operator<<(short n) {
        reload(6);
        if (n < 0) *(cur++) = '-', n = -n;
        if (n >= 10000) {
            *(cur++) = '0' + n / 10000, n %= 10000;
            *reinterpret_cast<unsigned*>(cur) = strtable[n];
            cur += 4;
        } else if (n >= 1000) {
            *reinterpret_cast<unsigned*>(cur) = strtable[n];
            cur += 4;
        } else if (n >= 100) {
            *reinterpret_cast<unsigned*>(cur) = strtable[n * 10];
            cur += 3;
        } else if (n >= 10) {
            *reinterpret_cast<unsigned*>(cur) = strtable[n * 100];
            cur += 2;
        } else *(cur++) = '0' + n;
        return *this;
    }
    FastOstream& operator<<(unsigned n) {
        reload(10);
        unsigned long long buf = 0;
        char d = 0;
        if (n >= 100000000) {
            d = 8;
            buf = static_cast<unsigned long long>(strtable[n % 10000]) << 32 | strtable[(n / 10000) % 10000];
            n /= 100000000;
        } else if (n >= 10000) {
            d = 4;
            buf = strtable[n % 10000];
            n /= 10000;
        }
        *reinterpret_cast<unsigned*>(cur) = strtable2[n];
        cur += (n >= 10) + (n >= 100) + (n >= 1000) + 1;
        *reinterpret_cast<unsigned long long*>(cur) = buf;
        cur += d;
        return *this;
    }
    FastOstream& operator<<(int n) {
        reload(11);
        if (n < 0) *(cur++) = '-', n = -n;
        unsigned long long buf = 0;
        char d = 0;
        if (n >= 100000000) {
            d = 8;
            buf = static_cast<unsigned long long>(strtable[n % 10000]) << 32 | strtable[(n / 10000) % 10000];
            n /= 100000000;
        } else if (n >= 10000) {
            d = 4;
            buf = strtable[n % 10000];
            n /= 10000;
        }
        *reinterpret_cast<unsigned*>(cur) = strtable2[n];
        cur += (n >= 10) + (n >= 100) + (n >= 1000) + 1;
        *reinterpret_cast<unsigned long long*>(cur) = buf;
        cur += d;
        return *this;
    }
    FastOstream& operator<<(unsigned long long n) {
        reload(20);
        static unsigned buf[4];
        int d = 0;
        if (n >= 10000000000000000) {
            d = 16;
            buf[3] = strtable[n % 10000], n /= 10000;
            buf[2] = strtable[n % 10000], n /= 10000;
            buf[1] = strtable[n % 10000], n /= 10000;
            buf[0] = strtable[n % 10000], n /= 10000;
        } else if (n >= 1000000000000) {
            d = 12;
            buf[2] = strtable[n % 10000], n /= 10000;
            buf[1] = strtable[n % 10000], n /= 10000;
            buf[0] = strtable[n % 10000], n /= 10000;
        } else if (n >= 100000000) {
            d = 8;
            buf[1] = strtable[n % 10000], n /= 10000;
            buf[0] = strtable[n % 10000], n /= 10000;
        } else if (n >= 10000) {
            d = 4;
            buf[0] = strtable[n % 10000], n /= 10000;
        }
        *(unsigned*) cur = strtable2[n];
        cur += (n >= 10) + (n >= 100) + (n >= 1000) + 1;
        std::memcpy(cur, buf, d);
        cur += d;
        return *this;
    }
    FastOstream& operator<<(long long n) {
        reload(21);
        if (n < 0) *(cur++) = '-', n = -n;
        static unsigned buf[4];
        char d = 0;
        if (n >= 10000000000000000) {
            d = 16;
            buf[3] = strtable[n % 10000], n /= 10000;
            buf[2] = strtable[n % 10000], n /= 10000;
            buf[1] = strtable[n % 10000], n /= 10000;
            buf[0] = strtable[n % 10000], n /= 10000;
        } else if (n >= 1000000000000) {
            d = 12;
            buf[2] = strtable[n % 10000], n /= 10000;
            buf[1] = strtable[n % 10000], n /= 10000;
            buf[0] = strtable[n % 10000], n /= 10000;
        } else if (n >= 100000000) {
            d = 8;
            buf[1] = strtable[n % 10000], n /= 10000;
            buf[0] = strtable[n % 10000], n /= 10000;
        } else if (n >= 10000) {
            d = 4;
            buf[0] = strtable[n % 10000], n /= 10000;
        }
        *(unsigned*) cur = strtable2[n];
        cur += (n >= 10) + (n >= 100) + (n >= 1000) + 1;
        std::memcpy(cur, buf, d);
        cur += d;
        return *this;
    }
    FastOstream& operator<<(long n) { return operator<<(static_cast<long long>(n)); }
    FastOstream& operator<<(unsigned long n) { return operator<<(static_cast<unsigned long long>(n)); }
    FastOstream& operator<<(float f) {
        reload(16);
        putfloat(f);
        return *this;
    }
    FastOstream& operator<<(double f) {
        reload(32);
        putfloat(f);
        return *this;
    }
    FastOstream& operator<<(long double f) {
        reload(64);
        putfloat(f);
        return *this;
    }
    FastOstream& operator<<(const void* p) {
        reload(18);
        if (flags() & std::ios_base::showbase) {
            *cur = '0';
            *(cur + 1) = flags() & std::ios_base::uppercase ? 'X' : 'x';
            cur += 2;
        }
        cur = std::to_chars(cur, buffer + buffersize, reinterpret_cast<unsigned long long>(p), 16).ptr;
        return *this;
    }
    FastOstream& operator<<(std::nullptr_t) {
        reload(7);
        std::memcpy(cur, "nullptr", 7);
        cur += 7;
        return *this;
    }
    friend FastOstream& operator<<(FastOstream& os, char c) {
        os.reload(1);
        *(os.cur++) = c;
        return os;
    }
    friend FastOstream& operator<<(FastOstream& os, signed char c) {
        os.reload(1);
        *(os.cur++) = c;
        return os;
    }
    friend FastOstream& operator<<(FastOstream& os, unsigned char c) {
        os.reload(1);
        *(os.cur++) = c;
        return os;
    }
    friend FastOstream& operator<<(FastOstream& os, const char* s) {
        size_t n = std::strlen(s);
        if (n >= os.buffersize) {
            os.reload(buffersize);
            write(1, s, n);
        } else {
            os.reload(n);
            std::memcpy(os.cur, s, n);
            os.cur += n;
        }
        return os;
    }
    friend FastOstream& operator<<(FastOstream& os, const std::string& s) {
        size_t n = s.length();
        if (n >= os.buffersize) {
            os.reload(buffersize);
            write(1, s.data(), n);
        } else {
            os.reload(n);
            std::memcpy(os.cur, s.data(), n);
            os.cur += n;
        }
        return os;
    }
    friend FastOstream& operator<<(FastOstream& os, std::string_view s) {
        size_t n = s.length();
        if (n >= os.buffersize) {
            os.reload(buffersize);
            write(1, s.data(), n);
        } else {
            os.reload(n);
            std::memcpy(os.cur, s.data(), n);
            os.cur += n;
        }
        return os;
    }
    template<std::ranges::range T> friend FastOstream& operator<<(FastOstream& os, const T& v) {
        size_t n = std::distance(std::ranges::begin(v), std::ranges::end(v)), cnt = 0;
        for (const auto& x : v) {
            os << x;
            if (++cnt != n) os << ' ';
        }
        return os;
    }
#if __GNUC__ >= 6
    friend FastOstream& operator<<(FastOstream& os, std::_Setprecision prec) {
        os.precision(prec._M_n);
        return os;
    }
#endif
} fout;
namespace std {
template<> class basic_ios<FastOstream, void> {
protected:
    FastOstream& ref;
public:
    basic_ios(FastOstream& r) : ref(r) {}
    char widen(char c) { return ref.widen(c); }
};
template<> class basic_ostream<FastOstream, void> : public basic_ios<FastOstream, void> {
public:
    basic_ostream(FastOstream& r) : basic_ios(r) {}
    basic_ostream& put(char c) {
        basic_ios::ref.put(c);
        return *this;
    }
    basic_ostream& flush() {
        basic_ios::ref.flush();
        return *this;
    }
};
}  // namespace std
inline FastOstream& FastOstream::operator<<(std::basic_ostream<FastOstream, void>& (*pf)(std::basic_ostream<FastOstream, void>&) ) {
    std::basic_ostream<FastOstream, void> tmp(*this);
    pf(tmp);
    return *this;
}
inline FastOstream& FastOstream::operator<<(std::basic_ios<FastOstream, void>& (*pf)(std::basic_ios<FastOstream, void>&) ) {
    std::basic_ios<FastOstream, void> tmp(*this);
    pf(tmp);
    return *this;
}
*/
#endif
#if true || defined(INCLUDE_COMPRESS) || defined(INCLUDE_ALL)
template<class Compare, class Iterator> constexpr std::vector<size_t> Compress(Iterator first, Iterator last, Compare comp= Compare()) {
if(first == last) [[unlikely]] {
return std::vector<size_t>{};
}
std::vector<std::pair<std::decay_t<decltype(*first)>, size_t>> v;
size_t i= 0, sz= std::distance(first, last);
v.reserve(sz);
for(Iterator itr= first; i != sz; ++itr, ++i) v.emplace_back(*itr, i);
std::sort(v.begin(), v.end(), [comp](const auto& a, const auto& b) { return comp(a.first, b.first); });
std::vector<size_t> res(sz);
res[v[0].second]= 0;
for(size_t i= 1, k= 0; i != sz; ++i) {
k+= bool(comp(v[i - 1].first, v[i].first));
res[v[i].second]= k;
}
return res;
}
template<class Iterator> constexpr std::vector<size_t> Compress(Iterator first, Iterator last) {
return Compress<std::less<std::decay_t<decltype(*first)>>, Iterator>(first, last);
}
#endif
#if true || defined(INCLUDE_RLE) || defined(INCLUDE_ALL)
template<class Iterator> constexpr auto RLE(Iterator first, Iterator last) {
std::vector<std::pair<std::decay_t<decltype(*first)>, size_t>> res;
if(first == last) return res;
res.emplace_back(*(first++), 1);
for(auto itr= first; itr != last; ++itr) {
if(*itr == res.back().first) ++res.back().second;
else res.emplace_back(*itr, 1);
}
return res;
}
#endif
#if true || defined(INCLUDE_INVNUM) || defined(INCLUDE_ALL)
template<class Iterator> std::vector<size_t> Invnum(Iterator first, Iterator last) {
size_t i= 0, sz= std::distance(first, last);
std::vector<size_t> res(sz), bit(sz);
for(Iterator itr= first; i != sz; ++itr, ++i) {
for(size_t j= *itr; j; j&= j - 1) res[i]+= bit[j - 1];
for(size_t j= *itr + 1; j <= sz; j+= (j & (0 - j))) ++bit[j - 1];
}
return res;
}
template<class Iterator> size_t Invnum_sum(Iterator first, Iterator last) {
size_t i= 0, sz= std::distance(first, last), res= 0;
std::vector<size_t> bit(sz);
for(Iterator itr= first; i != sz; ++itr, ++i) {
for(size_t j= *itr; j; j&= j - 1) res+= bit[j - 1];
for(size_t j= *itr + 1; j <= sz; j+= (j & (0 - j))) ++bit[j - 1];
}
return res;
}
template<class Iterator> size_t Invnum_sum_eq(Iterator first, Iterator last) {
size_t i= 0, sz= std::distance(first, last), res= 0;
std::vector<size_t> bit(sz);
for(Iterator itr= first; i != sz; ++itr, ++i) {
for(size_t j= *itr + 1; j <= sz; j+= (j & (0 - j))) ++bit[j - 1];
for(size_t j= *itr + 1; j; j&= j - 1) res+= bit[j - 1];
}
return res;
}
#endif
#if true || defined(INCLUDE_SLIDEMIN) || defined(INCLUDE_ALL)
template<class T, class Operator= std::less<T>> class SlideMin {
std::deque<std::pair<T, size_t>> deq;
size_t cnt1= 0, cnt2= 0;
[[no_unique_address]] Operator op;
public:
SlideMin(): op(Operator()) {}
SlideMin(Operator op_): op(op_) {}
void push(const T& x) {
while(!deq.empty() && op(x, deq.back().first)) deq.pop_back();
deq.emplace_back(x, ++cnt1);
}
void pop() {
if(deq.front().second == (++cnt2)) deq.pop_front();
}
const T& get() const { return deq.front().first; }
};
#endif
#if true || defined(INCLUDE_INTERVALSCHEDULING) || defined(INCLUDE_ALL)
template<class T> std::basic_string<bool> IntervalScheduling(std::vector<std::pair<T, T>> x) {
std::sort(x.begin(), x.end(), [](const std::pair<T, T>& a, const std::pair<T, T>& b) { return a.second < b.second; });
std::basic_string<bool> res(x.size(), false);
auto cur= std::numeric_limits<T>::lowest();
for(size_t i= 0, sz= x.size(); i != sz; ++i) {
const bool f= cur <= x[i].first;
res[i]= f;
cur= (f ? x[i].second : cur);
}
return res;
}
template<class T> size_t IntervalSchedulingCount(std::vector<std::pair<T, T>> x) {
std::sort(x.begin(), x.end(), [](const std::pair<T, T>& a, const std::pair<T, T>& b) { return a.second < b.second; });
size_t res= 0;
auto cur= std::numeric_limits<T>::lowest();
for(size_t i= 0, sz= x.size(); i != sz; ++i) {
const bool f= cur <= x[i].first;
res+= f;
cur= (f ? x[i].second : cur);
}
return res;
}
#endif
#if true || defined(INCLUDE_BOOLEANSET) || defined(INCLUDE_ALL)
class BooleanSet {
std::vector<size_t> index1;
std::vector<size_t> index2;
size_t true_counter;
public:
BooleanSet(size_t N): index1(N), index2(N), true_counter(0) {
for(size_t i= 0; i < N; ++i) index1[i]= index2[i]= i;
}
bool test(size_t N) { return index1[N] < true_counter; }
void set(size_t N) {
if(!test(N)) {
index1[N]= true_counter;
std::swap(index2[N], index2[true_counter]);
++true_counter;
}
}
void reset(size_t N) {
if(test(N)) {
--true_counter;
index1[N]= true_counter;
std::swap(index2[N], index2[true_counter]);
}
}
std::vector<size_t> enumrate() {
std::vector<size_t> ans(true_counter);
for(size_t i= 0; i < true_counter; ++i) ans[i]= index2[i];
return ans;
}
};
#endif
#if true || defined(INCLUDE_BIT) || defined(INCLUDE_ALL)
template<class T, class Allocator= std::allocator<T>> class RangeSumQuery {
std::vector<T, Allocator> bit;
public:
using reference= T&;
using const_reference= const T&;
using size_type= size_t;
using difference_type= ptrdiff_t;
using value_type= T;
using allocator_type= Allocator;
using pointer= typename std::allocator_traits<Allocator>::pointer;
using const_pointer= typename std::allocator_traits<Allocator>::const_pointer;
constexpr RangeSumQuery() noexcept(noexcept(Allocator())): RangeSumQuery(Allocator()) {}
constexpr explicit RangeSumQuery(const Allocator& alloc) noexcept: bit(alloc) {}
constexpr explicit RangeSumQuery(size_type n, const Allocator& alloc= Allocator()): bit(n, alloc) {}
constexpr RangeSumQuery(size_type n, const T& value, const Allocator& alloc= Allocator()): bit(alloc) { assign(n, value); }
template<class InputIter> constexpr RangeSumQuery(InputIter first, InputIter last, const Allocator& alloc= Allocator()): bit(alloc) { assign(first, last); }
constexpr RangeSumQuery(const RangeSumQuery&)= default;
constexpr RangeSumQuery(RangeSumQuery&&) noexcept= default;
constexpr RangeSumQuery(const RangeSumQuery& x, const Allocator& alloc): bit(x.bit, alloc) {}
constexpr RangeSumQuery(RangeSumQuery&& x, const Allocator& alloc): bit(std::move(x.bit), alloc) {}
constexpr RangeSumQuery(std::initializer_list<T> il, const Allocator& alloc= Allocator()): RangeSumQuery(il.begin(), il.end(), alloc) {}
constexpr RangeSumQuery& operator=(const RangeSumQuery&)= default;
constexpr RangeSumQuery& operator=(RangeSumQuery&&) noexcept(std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value || std::allocator_traits<Allocator>::is_always_equal::value)= default;
constexpr RangeSumQuery& operator=(std::initializer_list<T> il) {
assign(il);
return *this;
}
constexpr size_type size() const noexcept { return bit.size(); }
/*
    constexpr void resize(size_type sz) { resize(sz, value_type{}); }
    constexpr void resize(size_type sz, const T &c) {
        size_type n = bit.size();
        bit.resize(sz);
        if (n >= sz) return;
        // TODO:
    }
    */
[[nodiscard]] constexpr bool empty() const noexcept { return bit.empty(); }
constexpr value_type operator[](size_type n) const {
value_type res= bit[n];
if(!(n & 1)) return res;
size_type tmp= n & (n + 1);
for(size_type i= n; i != tmp; i&= i - 1) res-= bit[i - 1];
return res;
}
constexpr value_type at(size_type n) const {
if(n >= size()) throw std::out_of_range("RangeSumQuery::at / Index is out of range.");
return operator[](n);
}
template<class InputIterator> constexpr void assign(InputIterator first, InputIterator last) {
bit.assign(first, last);
size_type n= bit.size();
for(size_type i= 1; i != n; ++i) {
const size_type a= i - 1;
const size_type b= i & -i;
bit[a + b]+= (a + b < n ? bit[a] : value_type{});
}
}
constexpr void assign(size_type n, const T& u) {
if(n == 0) return;
bit= std::vector<value_type, Allocator>(n, get_allocator());
std::vector<value_type, Allocator> mul(std::bit_width(n), get_allocator());
mul[0]= u;
for(size_type i= 1, sz= mul.size(); i < sz; ++i) mul[i]= mul[i - 1], mul[i]+= mul[i - 1];
for(size_type i= 1; i <= n; ++i) bit[i - 1]= mul[std::countr_zero(i)];
}
constexpr void assign(std::initializer_list<T> il) { assign(il.begin(), il.end()); }
constexpr void swap(RangeSumQuery& x) noexcept(std::allocator_traits<Allocator>::propagate_on_container_swap::value || std::allocator_traits<Allocator>::is_always_equal::value) { bit.swap(x.bit); };
constexpr void clear() { bit.clear(); }
constexpr allocator_type get_allocator() const noexcept { return bit.get_allocator(); }
constexpr void add(size_type n, const value_type& x) {
for(size_type i= n + 1, sz= size(); i <= sz; i+= (i & -i)) bit[i - 1]+= x;
}
constexpr void minus(size_type n, const value_type& x) {
for(size_type i= n + 1, sz= size(); i <= sz; i+= (i & -i)) bit[i - 1]-= x;
}
constexpr void increme(size_type n) {
for(size_type i= n + 1, sz= size(); i <= sz; i+= (i & (-i))) ++bit[i - 1];
}
constexpr void decreme(size_type n) {
for(size_type i= n + 1, sz= size(); i <= sz; i+= (i & (-i))) --bit[i - 1];
}
constexpr value_type sum(size_type n) const {
value_type res= {};
for(size_type i= n; i != 0; i&= i - 1) res+= bit[i - 1];
return res;
}
constexpr value_type sum(size_type l, size_type r) const {
size_type n= l & ~((std::bit_floor(l ^ r) << 1) - 1);
value_type res= {};
for(size_type i= r; i != n; i&= i - 1) res+= bit[i - 1];
for(size_type i= l; i != n; i&= i - 1) res-= bit[i - 1];
return res;
}
constexpr size_type lower_bound(value_type x) const {
static_assert(std::is_unsigned_v<value_type>, "RangeSumQuery::lower_bound / value_type must be unsigned.");
size_type res= 0, n= size();
for(size_type len= std::bit_floor(n); len != 0; len>>= 1) {
if(res + len <= n && bit[res + len - 1] < x) {
x-= bit[res + len - 1];
res+= len;
}
}
return res;
}
constexpr size_type upper_bound(value_type x) const {
static_assert(std::is_unsigned_v<value_type>, "RangeSumQuery::upper_bound / value_type must be unsigned.");
size_type res= 0, n= size();
for(size_type len= std::bit_floor(n); len != 0; len>>= 1) {
if(res + len <= n && !(x < bit[res + len - 1])) {
x-= bit[res + len - 1];
res+= len;
}
}
return res;
}
};
template<class U, class Alloc> constexpr void swap(RangeSumQuery<U, Alloc>& x, RangeSumQuery<U, Alloc>& y) noexcept(noexcept(x.swap(y))) {
x.swap(y);
}
template<class InputIterator, class Allocator= std::allocator<typename std::iterator_traits<InputIterator>::value_type>> RangeSumQuery(InputIterator, InputIterator, Allocator= Allocator()) -> RangeSumQuery<typename std::iterator_traits<InputIterator>::value_type, Allocator>;
template<class T, class Allocator= std::allocator<T>> using RSQ= RangeSumQuery<T, Allocator>;
#endif
#if true || defined(INCLUDE_STATICRSQ) || defined(INCLUDE_ALL)
template<class T, class Allocator= std::allocator<T>> class StaticRangeSumQuery: public std::vector<T, Allocator> {
public:
using std::vector<T, Allocator>::vector;
constexpr void build() {
for(size_t i= 0, last= this->size() - 1; i != last; ++i) this->operator[](i + 1)+= this->operator[](i);
}
constexpr T sum(size_t n) const { return (n == 0 ? T{} : this->operator[](n - 1)); }
constexpr T sum(size_t l, size_t r) const { return (l == 0 ? (r == 0 ? T{} : this->operator[](r - 1)) : this->operator[](r - 1) - this->operator[](l - 1)); }
};
template<class T, class Allocator= std::allocator<T>> class StaticRangeSumQuery2D: public std::vector<std::vector<T, Allocator>, typename std::allocator_traits<Allocator>::template rebind_alloc<std::vector<T, Allocator>>> {
using base= std::vector<std::vector<T, Allocator>, typename std::allocator_traits<Allocator>::template rebind_alloc<std::vector<T, Allocator>>>;
public:
using base::base;
constexpr void build() {
for(size_t i= 0, h= this->size(); i != h; ++i) {
for(size_t j= 0, w= (*this)[i].size() - 1; j != w; ++j) (*this)[i][j + 1]+= (*this)[i][j];
}
for(size_t i= 0, h= this->size() - 1; i != h; ++i) {
for(size_t j= 0, w= (*this)[i].size(); j != w; ++j) (*this)[i + 1][j]+= (*this)[i][j];
}
}
constexpr T sum(size_t x1, size_t y1, size_t x2, size_t y2) const {
if(x2 == 0 || y2 == 0) return 0;
if(x1 == 0) {
if(y1 == 0) return (*this)[x2 - 1][y2 - 1];
else return (*this)[x2 - 1][y2 - 1] - (*this)[x2 - 1][y1 - 1];
} else {
if(y1 == 0) return (*this)[x2 - 1][y2 - 1] - (*this)[x1 - 1][y2 - 1];
else return (*this)[x2 - 1][y2 - 1] - (*this)[x1 - 1][y2 - 1] - (*this)[x2 - 1][y1 - 1] + (*this)[x1 - 1][y1 - 1];
}
}
};
#endif
#if true || defined(INCLUDE_SEGTREE) || defined(INCLUDE_ALL)
template<class T, class Operator, class Allocator= std::allocator<T>> class SegmentTree {
size_t sz= 0;
std::vector<T, Allocator> tree;
[[no_unique_address]] Operator f;
const T e;
public:
using reference= T&;
using const_reference= const T&;
using size_type= size_t;
using difference_type= ptrdiff_t;
using value_type= T;
using allocator_type= Allocator;
using pointer= typename std::allocator_traits<Allocator>::pointer;
using const_pointer= typename std::allocator_traits<Allocator>::const_pointer;
using operator_type= Operator;
constexpr SegmentTree(const T& ex): tree(Allocator()), f(Operator()), e(ex) {}
constexpr explicit SegmentTree(const Operator& opr, const T& ex, const Allocator& alloc= Allocator()): tree(alloc), f(opr), e(ex) {}
constexpr explicit SegmentTree(const T& ex, const Allocator& alloc): tree(alloc), f(Operator()), e(ex) {}
constexpr explicit SegmentTree(size_type n, const Operator& opr, const T& ex, const Allocator& alloc= Allocator()): sz(n), tree((std::bit_ceil(n) << 1) - 1, ex, alloc), f(opr), e(ex) {}
constexpr explicit SegmentTree(size_type n, const T& ex, const Allocator& alloc= Allocator()): SegmentTree(n, Operator(), ex, alloc) {}
constexpr SegmentTree(size_type n, const T& value, const Operator& opr, const T& ex, const Allocator& alloc= Allocator()): SegmentTree(opr, ex, alloc) { assign(n, value); }
constexpr SegmentTree(size_type n, const T& value, const T& ex, const Allocator& alloc): SegmentTree(n, value, Operator(), ex, alloc) {}
template<class InputIter> constexpr SegmentTree(InputIter first, InputIter last, const Operator& opr, const T& ex, const Allocator& alloc= Allocator()): tree(alloc), f(opr), e(ex) { assign(first, last); }
template<class InputIter> constexpr SegmentTree(InputIter first, InputIter last, const T& ex, const Allocator& alloc= Allocator()): SegmentTree(first, last, Operator(), ex, alloc) {}
constexpr SegmentTree(const SegmentTree&)= default;
constexpr SegmentTree(SegmentTree&&)= default;
constexpr SegmentTree(const SegmentTree& x, const Allocator& alloc): sz(x.sz), tree(x.tree, alloc), f(x.f), e(x.e) {}
constexpr SegmentTree(SegmentTree&& y, const Allocator& alloc): sz(y.sz), tree(std::move(y.tree), alloc), f(y.f), e(std::move(y.e)) {}
constexpr SegmentTree(std::initializer_list<T> init, const T& ex, const Allocator& alloc= Allocator()): SegmentTree(init.begin(), init.end(), Operator(), ex, alloc) {}
constexpr SegmentTree(std::initializer_list<T> init, const Operator& opr, const T& ex, const Allocator& alloc= Allocator()): SegmentTree(init.begin(), init.end(), opr, ex, alloc) {}
constexpr SegmentTree& operator=(const SegmentTree&)= default;
constexpr SegmentTree& operator=(SegmentTree&&) noexcept(std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value || std::allocator_traits<Allocator>::is_always_equal::value)= default;
constexpr SegmentTree& operator=(std::initializer_list<value_type> il) {
assign(il);
return *this;
}
constexpr size_type size() const noexcept { return sz; }
constexpr void resize(size_type n) { resize(n, e); }
constexpr void resize(size_type n, const T& c) {
std::vector<value_type> tmp;
tmp.reserve(sz);
for(size_t i= 0; i != sz; ++i) tmp.emplace_back(std::move(operator[](i)));
tmp.resize(n, c);
assign(tmp.begin(), tmp.end());
}
[[nodiscard]] bool empty() const noexcept { return sz == 0; }
constexpr const_reference operator[](size_type n) const { return tree[n + tree.size() / 2]; }
constexpr const_reference at(size_type n) const {
if(n >= sz) throw std::out_of_range("SegmentTree::at / Index is out of range.");
return tree[n + tree.size() / 2];
}
template<class InputIterator> constexpr void assign(InputIterator first, InputIterator last) {
sz= std::distance(first, last);
tree.assign((std::bit_ceil(sz) << 1) - 1, e);
size_t h= tree.size() / 2;
InputIterator itr= first;
for(size_t i= 0; i != sz; ++i, ++itr) tree[h + i]= *itr;
for(size_t i= 0; i != h; ++i) {
size_t tmp= h - i - 1;
tree[tmp]= f(tree[(tmp << 1) + 1], tree[(tmp << 1) + 2]);
}
}
constexpr void assign(size_type n, const value_type& u) {
sz= n;
tree.assign((std::bit_ceil(sz) << 1) - 1, e);
size_t h= tree.size() / 2;
for(size_t i= 0; i != sz; ++i) tree[h + i]= u;
for(size_t i= 0; i != h; ++i) {
size_t tmp= h - i - 1;
tree[tmp]= f(tree[(tmp << 1) + 1], tree[(tmp << 1) + 2]);
}
}
constexpr void assign(std::initializer_list<value_type> il) { assign(il.begin(), il.end()); }
constexpr void swap(SegmentTree& x) noexcept(std::allocator_traits<Allocator>::propagate_on_container_swap::value || std::allocator_traits<Allocator>::is_always_equal::value) {
using std::swap;
swap(sz, x.sz), swap(tree, x.tree), swap(f, x.f);
};
constexpr void clear() { sz= 0, tree.clear(); }
constexpr allocator_type get_allocator() const noexcept { return tree.get_allocator(); }
constexpr void set(size_type n, const value_type& x) {
size_type i= n + (tree.size() >> 1);
tree[i]= x;
while(i > 0) {
i= (i - 1) >> 1;
tree[i]= f(tree[(i << 1) + 1], tree[(i << 1) + 2]);
}
}
constexpr const_reference get(size_type n) const { return operator[](n); }
constexpr value_type prod(size_type l, size_type r) {
value_type resl= e, resr= e;
size_type h= (tree.size() + 1) >> 1;
for(l+= h, r+= h; l < r; l= (l + 1) >> 1, r>>= 1) {
resl= l & 1 ? f(resl, tree[l - 1]) : resl;
resr= r & 1 ? f(tree[r - 2], resr) : resr;
}
return f(resl, resr);
}
constexpr value_type all_prod() const { return (sz ? tree[0] : e); }
// p(prod[l,r)) == true && (r == N || p(prod[l,r]) == false)となるrを一つ見つける
// p(prod[l,i))がiに関して単調な時、rはp(prod[l,r)) == trueとなる最大のもの
template<class F> constexpr size_type max_right(size_type l, F p= F()) {
value_type x= e;
size_type h= (tree.size() + 1) >> 1;
for(size_type curl= l + h, curr= tree.size() + 1; curl < curr; curl>>= 1, curr>>= 1) {
if(curl & 1) {
value_type tmp= f(x, tree[(++curl) - 2]);
if(!p(tmp)) {
--curl;
while(curl < h) {
curl<<= 1;
value_type tmp= f(x, tree[curl - 1]);
if(p(tmp)) {
x= tmp, ++curl;
}
}
return curl - h;
} else x= tmp;
}
}
return sz;
}
// p(prod[l,r)) == true && (l == 0 || p(prod[l - 1, r)) == false)となるlを一つ見つける
// p(prod[i,r))がiに関して単調な時、lはp(prod[l,r)) == trueとなる最小のもの
/*
    template<class F> constexpr size_type min_left(size_type r, F p = F()) {
        //TODO
        return 0;
    }
    */
};
template<class U, class Opr, class Alloc> constexpr void swap(SegmentTree<U, Opr, Alloc>& x, SegmentTree<U, Opr, Alloc>& y) noexcept(noexcept(x.swap(y))) {
x.swap(y);
}
template<class T, class Operator, class E, class Allocator= std::allocator<T>> class SegmentTreeWrapper: public SegmentTree<T, Operator, Allocator> {
using base= SegmentTree<T, Operator, Allocator>;
using size_type= base::size_type;
public:
constexpr SegmentTreeWrapper(): base(E()()) {}
constexpr explicit SegmentTreeWrapper(const Allocator& alloc): base(E()(), alloc) {}
constexpr explicit SegmentTreeWrapper(size_type n, const Allocator& alloc= Allocator()): base(n, E()(), alloc) {}
constexpr SegmentTreeWrapper(size_type n, const T& value, const Allocator& alloc): base(n, value, E()(), alloc) {}
template<class InputIter> constexpr SegmentTreeWrapper(InputIter first, InputIter last, const Allocator& alloc= Allocator()): base(first, last, E()(), alloc) {}
constexpr SegmentTreeWrapper(const SegmentTreeWrapper&)= default;
constexpr SegmentTreeWrapper(SegmentTreeWrapper&&)= default;
constexpr SegmentTreeWrapper(const SegmentTreeWrapper& x, const Allocator& alloc): base(x, E()(), alloc) {}
constexpr SegmentTreeWrapper(SegmentTreeWrapper&& y, const Allocator& alloc): base(std::move(y), E()(), alloc) {}
constexpr SegmentTreeWrapper(std::initializer_list<T> init, const Allocator& alloc= Allocator()): base(init, E()(), alloc) {}
};
template<class T, class Allocator= std::allocator<T>> using RangeMaximumQuery= SegmentTreeWrapper<T, decltype([](const T& a, const T& b) { return std::max(a, b); }), decltype([]() { return std::numeric_limits<T>::lowest(); }), Allocator>;
template<class T, class Allocator= std::allocator<T>> using RangeMinimumQuery= SegmentTreeWrapper<T, decltype([](const T& a, const T& b) { return std::min(a, b); }), decltype([]() { return std::numeric_limits<T>::max(); }), Allocator>;
template<class T, class Allocator= std::allocator<T>> using RangeUnionQuery= SegmentTreeWrapper<T, decltype([](const T& a, const T& b) { return a + b; }), decltype([]() { return T{}; }), Allocator>;
template<class T, class Allocator= std::allocator<T>> using RangeOrQuery= SegmentTreeWrapper<T, decltype([](const T& a, const T& b) { return a | b; }), decltype([]() -> T { return 0; }), Allocator>;
template<class T, class Allocator= std::allocator<T>> using RangeAndQuery= SegmentTreeWrapper<T, decltype([](const T& a, const T& b) { return a & b; }), decltype([]() -> T { return ~static_cast<T>(0); }), Allocator>;
template<class T, class Allocator= std::allocator<T>> using RangeXorQuery= SegmentTreeWrapper<T, decltype([](const T& a, const T& b) { return a ^ b; }), decltype([]() -> T { return 0; }), Allocator>;
template<class T, class Allocator= std::allocator<T>> using RangeMulQuery= SegmentTreeWrapper<T, decltype([](const T& a, const T& b) { return a * b; }), decltype([]() -> T { return 1; }), Allocator>;
template<class T, class Allocator= std::allocator<T>> using RangeCompositeQuery= SegmentTreeWrapper<std::pair<T, T>, decltype([](const std::pair<T, T>& a, const std::pair<T, T>& b) { return std::pair<T, T>{a.first * b.first, a.second * b.first + b.second}; }), decltype([]() { return std::pair<T, T>{1, 0}; }), typename std::allocator_traits<Allocator>::template rebind_alloc<std::pair<T, T>>>;
template<class T, class Allocator= std::allocator<T>> using RangeGCDQuery= SegmentTreeWrapper<T, decltype([](const T& a, const T& b) { return std::gcd(a, b); }), decltype([]() -> T { return 0; }), Allocator>;
template<class T, class Allocator= std::allocator<T>> using RangeLCMQuery= SegmentTreeWrapper<T, decltype([](const T& a, const T& b) { return std::lcm(a, b); }), decltype([]() -> T { return 1; }), Allocator>;
#endif
#if true || defined(INCLUDE_DSU) || defined(INCLUDE_ALL)
template<class Container= std::vector<int>> class DisjointSetUnion {
public:
using value_type= typename Container::value_type;
using reference= typename Container::reference;
using const_reference= typename Container::const_reference;
using size_type= typename Container::size_type;
using container_type= Container;
using allocator_type= typename Container::allocator_type;
static_assert(std::is_signed_v<value_type>, "DisjointSetUnion / Container::value_type must be signed.");
protected:
Container parent;
size_type counter= 0;
private:
value_type root(value_type n) noexcept {
if(parent[n] < 0) return n;
return parent[n]= root(parent[n]);
}
public:
DisjointSetUnion(): parent() {}
explicit DisjointSetUnion(size_type n): parent(n, -1), counter(n) {}
DisjointSetUnion(const DisjointSetUnion&)= default;
DisjointSetUnion(DisjointSetUnion&&)= default;
explicit DisjointSetUnion(const allocator_type& alloc): parent(alloc) {}
DisjointSetUnion(size_type n, const allocator_type& alloc): parent(n, -1, alloc), counter(n) {}
DisjointSetUnion(const DisjointSetUnion& x, const allocator_type& alloc): parent(x.parent, alloc), counter(x.counter) {}
DisjointSetUnion(DisjointSetUnion&& x, const allocator_type& alloc): parent(std::move(x.parent), alloc), counter(x.counter) {}
size_type size() const noexcept { return parent.size(); }
[[nodiscard]] bool empty() const noexcept { return parent.empty(); }
void resize(size_type n) {
if(n < size()) throw std::invalid_argument("DisjointSetUnion::resize");
counter+= n - size();
parent.resize(n, -1);
}
size_type leader(size_type n) {
if(n >= size()) throw std::out_of_range("DisjointSetUnion::leader");
return static_cast<size_type>(root(static_cast<value_type>(n)));
}
bool is_leader(size_type n) const {
if(n >= size()) throw std::out_of_range("DisjointSetUnion::is_leader");
return parent[n] < 0;
}
bool same(size_type a, size_type b) {
if(a >= size() || b >= size()) throw std::out_of_range("DisjointSetUnion::same");
return root(static_cast<value_type>(a)) == root(static_cast<value_type>(b));
}
bool merge(size_type a, size_type b) {
if(a >= size() || b >= size()) throw std::out_of_range("DisjointSetUnion::merge");
value_type ar= root(static_cast<value_type>(a)), br= root(static_cast<value_type>(b));
if(ar == br) return true;
if(parent[ar] < parent[br]) {
parent[ar]+= parent[br];
parent[br]= ar;
} else {
parent[br]+= parent[ar];
parent[ar]= br;
}
--counter;
return false;
}
size_type size(size_type n) {
if(n >= size()) throw std::out_of_range("DisjointSetUnion::size");
return -parent[root(static_cast<value_type>(n))];
}
size_type count_groups() const noexcept { return counter; }
std::vector<value_type> extract(size_type n) {
if(n >= size()) throw std::out_of_range("DisjointSetUnion::extract");
int nr= root(static_cast<value_type>(n));
std::vector<value_type> res;
for(size_type i= 0, s= size(); i < s; ++i)
if(root(static_cast<value_type>(i)) == nr) res.push_back(i);
return res;
}
std::vector<std::vector<value_type>> groups() {
value_type* key= parent.get_allocator().allocate(size());
value_type cnt= 0;
for(value_type i= 0, s= static_cast<value_type>(size()); i < s; ++i) {
if(parent[i] >= 0) continue;
key[i]= cnt++;
}
std::vector<std::vector<value_type>> res(cnt);
for(value_type i= 0, s= static_cast<value_type>(size()); i < s; ++i) res[key[root(i)]].push_back(i);
parent.get_allocator().deallocate(key, size());
return res;
}
};
template<class Container= std::vector<int>> using DSU= DisjointSetUnion<Container>;
template<class Abel, class Container= std::vector<int>> class WeightedDisjointSetUnion {
public:
using value_type= typename Container::value_type;
using reference= typename Container::reference;
using const_reference= typename Container::const_reference;
using size_type= typename Container::size_type;
using container_type= Container;
using allocator_type= typename Container::allocator_type;
using weight_type= Abel;
static_assert(std::is_signed_v<value_type>, "WeightedDisjointSetUnion / Container::value_type must be signed.");
protected:
Container parent;
std::vector<weight_type> diff;
size_type counter= 0;
private:
value_type root(value_type n) noexcept {
if(parent[n] < 0) return n;
value_type r= root(parent[n]);
diff[n]+= diff[parent[n]];
return parent[n]= r;
}
public:
WeightedDisjointSetUnion(): parent(), diff() {}
explicit WeightedDisjointSetUnion(size_type n): parent(n, -1), diff(n, 0), counter(n) {}
WeightedDisjointSetUnion(const WeightedDisjointSetUnion&)= default;
WeightedDisjointSetUnion(WeightedDisjointSetUnion&&)= default;
explicit WeightedDisjointSetUnion(const allocator_type& alloc): parent(alloc), diff(alloc) {}
WeightedDisjointSetUnion(size_type n, const allocator_type& alloc): parent(n, -1, alloc), diff(n, 0, alloc), counter(n) {}
WeightedDisjointSetUnion(const WeightedDisjointSetUnion& x, const allocator_type& alloc): parent(x.parent, alloc), diff(x.diff, alloc), counter(x.counter) {}
WeightedDisjointSetUnion(WeightedDisjointSetUnion&& x, const allocator_type& alloc): parent(std::move(x.parent), alloc), diff(std::move(x.diff), alloc), counter(x.counter) {}
size_type size() const noexcept { return parent.size(); }
[[nodiscard]] bool empty() const noexcept { return parent.empty(); }
void resize(size_type n) {
if(n < size()) throw std::invalid_argument("WeightedDisjointSetUnion::resize");
counter+= n - size();
parent.resize(n, -1);
}
size_type leader(size_type n) {
if(n >= size()) throw std::out_of_range("WeightedDisjointSetUnion::leader");
return static_cast<size_type>(root(static_cast<value_type>(n)));
}
bool is_leader(size_type n) const {
if(n >= size()) throw std::out_of_range("DisjointSetUnion::is_leader");
return parent[n] < 0;
}
weight_type weight(size_type n) {
if(n >= size()) throw std::out_of_range("WeightedDisjointSetUnion::weight");
root(static_cast<value_type>(n));
return diff[n];
}
bool same(size_type a, size_type b) {
if(a >= size() || b >= size()) throw std::out_of_range("WeightedDisjointSetUnion::same");
return root(static_cast<value_type>(a)) == root(static_cast<value_type>(b));
}
bool merge(size_type a, size_type b, const weight_type& w) {
if(a >= size() || b >= size()) throw std::out_of_range("WeightedDisjointSetUnion::merge");
value_type ar= root(static_cast<value_type>(a)), br= root(static_cast<value_type>(b));
if(ar == br) return diff[a] - diff[b] == w;
if(parent[ar] < parent[br]) {
parent[ar]+= parent[br];
parent[br]= ar;
diff[br]= -w + diff[a] - diff[b];
} else {
parent[br]+= parent[ar];
parent[ar]= br;
diff[ar]= w - diff[a] + diff[b];
}
--counter;
return true;
}
size_type size(size_type n) {
if(n >= size()) throw std::out_of_range("WeightedDisjointSetUnion::size");
return -parent[root(static_cast<value_type>(n))];
}
size_type count_groups() const noexcept { return counter; }
std::vector<value_type> extract(size_type n) {
if(n >= size()) throw std::out_of_range("WeightedDisjointSetUnion::extract");
int nr= root(static_cast<value_type>(n));
std::vector<value_type> res;
for(size_type i= 0, s= size(); i < s; ++i)
if(root(static_cast<value_type>(i)) == nr) res.push_back(i);
return res;
}
std::vector<std::vector<value_type>> groups() {
value_type* key= parent.get_allocator().allocate(size());
value_type cnt= 0;
for(value_type i= 0, s= static_cast<value_type>(size()); i < s; ++i) {
if(parent[i] >= 0) continue;
key[i]= cnt++;
}
std::vector<std::vector<value_type>> res(cnt);
for(value_type i= 0, s= static_cast<value_type>(size()); i < s; ++i) res[key[root(i)]].push_back(i);
parent.get_allocator().deallocate(key, size());
return res;
}
};
template<class Abel, class Container= std::vector<int>> using WDSU= WeightedDisjointSetUnion<Abel, Container>;
#endif
#if true || defined(INCLUDE_GRAPH) || defined(INCLUDE_ALL)
template<class Weight> class GraphBase;
namespace Graph {
template<class Weight> struct Edge {
friend class GraphBase<Weight>;
using weight_type= Weight;
size_t to;
Weight weight;
constexpr operator size_t() const noexcept { return to; }
private:
constexpr Edge(size_t t= 0, Weight w= {}): to(t), weight(w) {}
};
template<> struct Edge<void> {
friend class GraphBase<void>;
using weight_type= size_t;
size_t to;
constexpr operator size_t() const noexcept { return to; }
private:
constexpr Edge(size_t t= 0): to(t) {}
};
struct dsu {
std::vector<int> par;
constexpr dsu(size_t n): par(n, -1) {}
constexpr int root(int n) {
if(par[n] < 0) return n;
return par[n]= root(par[n]);
}
constexpr bool merge(int a, int b) {
int ar= root(a), br= root(b);
if(ar == br) return false;
if(par[ar] < par[br]) {
par[ar]+= par[br];
par[br]= ar;
} else {
par[br]+= par[ar];
par[ar]= br;
}
return true;
}
};
}// namespace Graph
template<class Weight> class GraphBase {
public:
using edge_type= Graph::Edge<Weight>;
using weight_type= typename edge_type::weight_type;
constexpr static bool is_weighted= !std::is_void_v<Weight>;
private:
struct node {
edge_type edge;
node* next;
constexpr node(size_t t, node* p): edge(t), next(p) {}
constexpr node(size_t t, const weight_type& w, node* p): edge(t, w), next(p) {}
constexpr node(const node&)= delete;
};
std::vector<node*> vertices;
std::vector<std::unique_ptr<node[]>> edges;
size_t edges_last= 0;
protected:
constexpr void connect(size_t from, size_t to) {
if(edges_last == 0) [[unlikely]]
edges.emplace_back(static_cast<node*>(::operator new[]((1ull << edges.size()) * sizeof(node))));
node* tmp= vertices[from];
vertices[from]= edges.back().get() + edges_last;
new(vertices[from]) node(to, tmp);
edges_last*= (++edges_last != (1ull << (edges.size() - 1)));
}
constexpr void connect(size_t from, size_t to, const weight_type& w) {
if(edges_last == 0) [[unlikely]]
edges.emplace_back(static_cast<node*>(::operator new[]((1ull << edges.size()) * sizeof(node))));
node* tmp= vertices[from];
vertices[from]= edges.back().get() + edges_last;
new(vertices[from]) node(to, w, tmp);
edges_last*= (++edges_last != (1ull << (edges.size() - 1)));
}
public:
constexpr GraphBase(): vertices() {}
constexpr GraphBase(size_t n): vertices(n, nullptr) {}
constexpr GraphBase(const GraphBase&)= default;
constexpr GraphBase(GraphBase&&)= default;
protected:
#if __GNUC__ >= 12
constexpr ~GraphBase()= default;
#else
~GraphBase()= default;
#endif
public:
class AdjacencyList {
friend class GraphBase;
protected:
size_t vertex;
const GraphBase& graph;
constexpr AdjacencyList(size_t n, const GraphBase& ref) noexcept: vertex(n), graph(ref) {}
public:
class iterator {
friend class AdjacencyList;
const node* ptr;
constexpr iterator(const node* p) noexcept: ptr(p) {}
public:
constexpr const edge_type& operator*() const noexcept { return ptr->edge; }
constexpr const edge_type* operator->() const noexcept { return &(ptr->edge); }
constexpr iterator& operator++() noexcept {
ptr= ptr->next;
return *this;
}
constexpr iterator& operator++(int) noexcept {
iterator copy(*this);
ptr= ptr->next;
return copy;
}
constexpr bool operator==(iterator r) noexcept { return ptr == r.ptr; }
constexpr bool operator!=(iterator r) noexcept { return ptr != r.ptr; }
};
using const_iterator= iterator;
constexpr iterator begin() noexcept { return iterator(graph.vertices[vertex]); }
constexpr const_iterator begin() const noexcept { return const_iterator(graph.vertices[vertex]); }
constexpr const_iterator cbegin() const noexcept { return const_iterator(graph.vertices[vertex]); }
constexpr iterator end() noexcept { return iterator(nullptr); }
constexpr const_iterator end() const noexcept { return const_iterator(nullptr); }
constexpr const_iterator cend() const noexcept { return const_iterator(nullptr); }
constexpr operator size_t() const noexcept { return vertex; }
};
class iterator: AdjacencyList {
friend class GraphBase;
protected:
using AdjacencyList::vertex, AdjacencyList::graph;
constexpr iterator(size_t n, const GraphBase& ref): AdjacencyList(n, ref) {}
public:
constexpr const AdjacencyList& operator*() const noexcept { return *this; }
constexpr const AdjacencyList* operator->() const noexcept { return this; }
constexpr iterator& operator++() noexcept {
++vertex;
return *this;
}
constexpr iterator& operator++(int) noexcept {
iterator copy(*this);
++vertex;
return copy;
}
constexpr bool operator==(const iterator& r) noexcept { return vertex == r.vertex && (&graph) == (&r.graph); }
constexpr bool operator!=(const iterator& r) noexcept { return vertex != r.vertex || (&graph) != (&r.graph); }
};
using const_iterator= iterator;
constexpr iterator begin() noexcept { return iterator(0, *this); }
constexpr const_iterator begin() const noexcept { return const_iterator(0, *this); }
constexpr const_iterator cbegin() const noexcept { return const_iterator(0, *this); }
constexpr iterator end() noexcept { return iterator(count_vertices(), *this); }
constexpr const_iterator end() const noexcept { return const_iterator(count_vertices(), *this); }
constexpr const_iterator cend() const noexcept { return const_iterator(count_vertices(), *this); }
constexpr inline static weight_type inf= std::numeric_limits<weight_type>::max();
constexpr size_t count_vertices() const noexcept { return vertices.size(); }
protected:
constexpr size_t count_edges_impl() const noexcept { return edges.empty() ? 0 : (1ull << (edges.size() - 1)) - 1 + edges_last; }
public:
constexpr AdjacencyList operator[](size_t n) const {
if(n >= count_vertices()) throw std::out_of_range("GraphBase::operator[] / Index is out of range");
return AdjacencyList(n, *this);
}
};
template<class Weight= void> class DirectedGraph: public GraphBase<Weight> {
using base= GraphBase<Weight>;
std::vector<size_t> ideg, odeg;
public:
using edge_type= typename base::edge_type;
using weight_type= typename base::weight_type;
constexpr DirectedGraph(): base() {}
constexpr DirectedGraph(size_t n): base(n), ideg(n), odeg(n) {}
constexpr DirectedGraph(const DirectedGraph&)= default;
constexpr DirectedGraph(DirectedGraph&&)= default;
constexpr DirectedGraph& connect(size_t n, size_t m) {
if(n >= base::count_vertices() || m >= base::count_vertices()) throw std::out_of_range("DirectedGraph::connect / Index is out of range");
++odeg[n], ++ideg[m];
base::connect(n, m);
return *this;
}
constexpr DirectedGraph& connect(size_t n, size_t m, weight_type w) {
if(n >= base::count_vertices() || m >= base::count_vertices()) throw std::out_of_range("DirectedGraph::connect / Index is out of range");
++odeg[n], ++ideg[m];
base::connect(n, m, w);
return *this;
}
constexpr size_t count_edges() const noexcept { return base::count_edges_impl(); }
constexpr size_t indegree(size_t n) const {
if(n >= base::count_vertices()) throw std::out_of_range("DirectedGraph::indegree / Index is out of range");
return ideg[n];
}
constexpr const std::vector<size_t>& indegree() const { return ideg; }
constexpr size_t outdegree(size_t n) const {
if(n >= base::count_vertices()) throw std::out_of_range("DirectedGraph::outdegree / Index is out of range");
return odeg[n];
}
constexpr const std::vector<size_t>& outdegree() const { return odeg; }
};
template<class Weight= void> class UndirectedGraph: public GraphBase<Weight> {
using base= GraphBase<Weight>;
std::vector<size_t> deg;
public:
using edge_type= typename base::edge_type;
using weight_type= typename base::weight_type;
constexpr UndirectedGraph(): base() {}
constexpr UndirectedGraph(size_t n): base(n), deg(n) {}
constexpr UndirectedGraph(const UndirectedGraph&)= default;
constexpr UndirectedGraph(UndirectedGraph&&)= default;
constexpr UndirectedGraph& connect(size_t n, size_t m) {
if(n >= base::count_vertices() || m >= base::count_vertices()) throw std::out_of_range("UndirectedGraph::connect / Index is out of range");
++deg[n], ++deg[m];
base::connect(n, m);
base::connect(m, n);
return *this;
}
constexpr UndirectedGraph& connect(size_t n, size_t m, weight_type w) {
if(n >= base::count_vertices() || m >= base::count_vertices()) throw std::out_of_range("UndirectedGraph::connect / Index is out of range");
++deg[n], ++deg[m];
base::connect(n, m, w);
base::connect(m, n, w);
return *this;
}
constexpr size_t count_edges() const noexcept { return base::count_edges_impl() >> 1; }
constexpr size_t degree(size_t n) const {
if(n >= base::count_vertices()) throw std::out_of_range("UndirectedGraph::degree / Index is out of range");
return deg[n];
}
constexpr const std::vector<size_t>& degree() const { return deg; }
constexpr auto toDirected() const {
DirectedGraph<weight_type> res(base::count_vertices());
for(size_t i= 0, n= base::count_vertices(); i != n; ++i) {
for(auto [j, cost]: base::operator[](i)) {
res.connect(i, j, cost);
}
}
return res;
}
constexpr operator DirectedGraph<weight_type>() const { return toDirected(); }
};
struct ConnectedComponents {
std::vector<std::vector<size_t>> vertex;
std::vector<size_t> aff;
#if __GNUC__ >= 12
constexpr size_t size() const noexcept { return aff.size(); }
[[nodiscard]] constexpr bool empty() const noexcept { return aff.empty(); }
constexpr size_t leader(size_t n) const { return vertex[aff[n]][0]; }
constexpr size_t is_leader(size_t n) const { return vertex[aff[n]][0] == n; }
constexpr bool same(size_t a, size_t b) const { return aff[a] == aff[b]; }
constexpr size_t size(size_t n) const { return vertex[aff[n]].size(); }
constexpr size_t count_groups() const { return vertex.size(); }
constexpr const std::vector<size_t>& extract(size_t n) const { return vertex[aff[n]]; }
constexpr const std::vector<std::vector<size_t>>& groups() const { return vertex; }
constexpr size_t affiliation(size_t n) const { return aff[n]; }
constexpr const std::vector<size_t>& affiliation() const { return aff; }
#else
size_t size() const noexcept { return aff.size(); }
[[nodiscard]] bool empty() const noexcept { return aff.empty(); }
size_t leader(size_t n) const { return vertex[aff[n]][0]; }
size_t is_leader(size_t n) const { return vertex[aff[n]][0] == n; }
bool same(size_t a, size_t b) const { return aff[a] == aff[b]; }
size_t size(size_t n) const { return vertex[aff[n]].size(); }
size_t count_groups() const { return vertex.size(); }
const std::vector<size_t>& extract(size_t n) const { return vertex[aff[n]]; }
const std::vector<std::vector<size_t>>& groups() const { return vertex; }
size_t affiliation(size_t n) const { return aff[n]; }
const std::vector<size_t>& affiliation() const { return aff; }
#endif
template<class DG> constexpr DG to_directedgraph(const DG& g) const {
if(aff.size() != g.count_vertices()) throw std::runtime_error("ConnectedComponents::to_directedgraph / The number of vertices does not match.");
size_t N= aff.size();
DG res(N);
for(size_t i= 0; i != N; ++i) {
if constexpr(DG::is_weighted) {
for(auto [j, w]: g[i]) {
if(aff[i] == aff[j]) res.connect(i, j, w);
}
} else {
for(size_t j: g[i]) {
if(aff[i] == aff[j]) res.connect(i, j);
}
}
}
return res;
}
template<class UG> constexpr UG to_undirectedgraph(const UG& g) const {
if(aff.size() != g.count_vertices()) throw std::runtime_error("ConnectedComponents::to_undirectedgraph / The number of vertices does not match.");
size_t N= aff.size();
UG res(N);
for(size_t i= 0; i != N; ++i) {
if constexpr(UG::is_weighted) {
for(auto [j, w]: g[i]) {
if(i < j && aff[i] == aff[j]) res.connect(i, j, w);
}
} else {
for(size_t j: g[i]) {
if(i < j && aff[i] == aff[j]) res.connect(i, j);
}
}
}
return res;
}
};
template<class UG> constexpr size_t CountConnectedComponents(const UG& g) {
size_t N= g.count_vertices();
Graph::dsu uf(N);
size_t res= N;
for(size_t i= 0; i < N; ++i) {
for(size_t j: g[i])
if(uf.merge(i, j)) --res;
}
return res;
}
template<class UG> constexpr bool isConnectedGraph(const UG& g) {
return CountConnectedComponents(g) == 1;
}
template<class UG> constexpr bool isPathGraph(const UG& g) {
if(g.count_vertices() - 1 != g.count_edges()) return false;
for(size_t i= 0, n= g.count_vertices(); i != n; ++i) {
size_t cnt= 0;
for([[maybe_unused]] auto& tmp: g[i]) ++cnt;
if(cnt > 2) return false;
}
return isConnectedGraph(g);
}
template<class UG> constexpr bool isTree(const UG& g) {
if(g.count_vertices() - 1 != g.count_edges()) return false;
return isConnectedGraph(g);
}
template<class DG> constexpr std::vector<size_t> TopologicalSort(const DG& g) {
size_t N= g.count_vertices();
std::vector<size_t> res;
res.reserve(N);
std::vector<size_t> d= g.indegree();
std::vector<size_t> s;
for(size_t i= 0; i != N; ++i)
if(d[i] == 0) s.push_back(i);
size_t cnt= 0;
while(!s.empty()) {
++cnt;
size_t n= s.back();
s.pop_back();
res.push_back(n);
for(size_t m: g[n]) {
if(--d[m] == 0) s.push_back(m);
}
}
if(cnt != N) res.clear();
return res;
}
template<class DG, class Comp> constexpr std::vector<size_t> TopologicalSort(const DG& g, const Comp& c= Comp()) {
size_t N= g.count_vertices();
std::vector<size_t> res;
res.reserve(N);
std::vector<size_t> d= g.indegree();
auto comp= [&c](size_t a, size_t b) {
return !c(a, b);
};
std::priority_queue<size_t, std::vector<size_t>, decltype(comp)> s(comp);
for(size_t i= 0; i != N; ++i)
if(d[i] == 0) s.push(i);
size_t cnt= 0;
while(!s.empty()) {
++cnt;
size_t n= s.top();
s.pop();
res.push_back(n);
for(size_t m: g[n]) {
if(--d[m] == 0) s.push(m);
}
}
if(cnt != N) res.clear();
return res;
}
template<class DG> constexpr std::vector<size_t> LongestPathLength(const DG& g) {
size_t N= g.count_vertices();
std::vector<size_t> res(N);
std::vector<size_t> d= g.indegree();
std::vector<size_t> s;
for(size_t i= 0; i != N; ++i)
if(d[i] == 0) s.push_back(i);
size_t cnt= 0;
while(!s.empty()) {
++cnt;
size_t n= s.back();
s.pop_back();
for(size_t m: g[n]) {
res[m]= std::max(res[m], res[n] + 1);
if(--d[m] == 0) s.push_back(m);
}
}
if(cnt != N) res.clear();
return res;
}
template<class G> constexpr auto TravelingSalesmanProblemLength(const G& g) {
const size_t N= g.count_vertices();
if(N >= 32) throw std::runtime_error("TravelingSalesmanProblemLength / The number of vertices is too large.");
constexpr auto e= std::numeric_limits<typename G::weight_type>::max();
std::vector dist(N, std::vector(N, e));
std::vector<size_t> mask(N);
for(size_t i= 0; i != N; ++i) {
for(auto [j, w]: g[i]) dist[i][j]= (dist[i][j] < w ? dist[i][j] : w);
for(size_t j= 0; j != N; ++j)
if(dist[i][j] != e) mask[i]|= (1ull << j);
}
std::vector dp(1ull << N, std::vector(N, e));
dp[1][0]= 0;
for(size_t i= 1; i != (1ull << N); ++i) {
const size_t u= (~i) & ((1ull << N) - 1);
for(size_t s= i; s != 0;) {
const int n= std::countr_zero(s);
s^= 1ull << n;
const auto x= dp[i][n];
if(dp[i][n] == e) continue;
for(size_t t= u & mask[n]; t != 0;) {
const size_t m= std::countr_zero(t);
const auto a= dp[i | (1ull << m)][m];
const auto b= x + dist[n][m];
dp[i | (1ull << m)][m]= (a < b ? a : b);
t^= 1ull << m;
}
}
}
auto res= e;
for(size_t i= 0; i != N; ++i) {
const auto d= dp[(1ull << N) - 1][i] + dist[i][0];
res= (res < d ? res : d);
}
return res;
}
namespace Graph {
namespace ShortestPath {
class BFS {};
class DPonDAG {};
class Dijkstra {};
class BFS01 {};
class BellmanFord {};
class DFSonTree {};
class SPFA {};
class FloydWarshall {};
}// namespace ShortestPath
template<class G> struct ShortestPathResult {
constexpr static size_t e= std::numeric_limits<size_t>::max();
constexpr static auto inf= std::numeric_limits<typename G::weight_type>::max();
std::vector<size_t> prev;
std::vector<typename G::weight_type> dist;
constexpr ShortestPathResult(size_t n): prev(n, e), dist(n, inf) {}
constexpr size_t size() const noexcept { return prev.size(); }
constexpr auto distance(size_t n) const { return dist[n]; }
constexpr const std::vector<typename G::weight_type>& distance() const { return dist; }
constexpr std::vector<size_t> path(size_t n) const {
if(dist[n] == inf) return std::vector<size_t>{};
std::vector<size_t> res;
while(n != e) {
res.push_back(n);
n= prev[n];
}
std::reverse(res.begin(), res.end());
return res;
}
constexpr auto tree() const {
DirectedGraph res(prev.size());
for(size_t i= 0, n= prev.size(); i != n; ++i) {
if(prev[i] == e) continue;
res.connect(prev[i], i);
}
return res;
}
constexpr auto weighted_tree() const {
DirectedGraph<typename G::weight_type> res(prev.size());
for(size_t i= 0, n= prev.size(); i != n; ++i) {
if(prev[i] == e) continue;
res.connect(prev[i], i, dist[i] - dist[prev[i]]);
}
return res;
}
};
}// namespace Graph
template<class Algorithm, class G> Graph::ShortestPathResult<G> ShortestPath(const G& g, size_t n) {
size_t N= g.count_vertices();
Graph::ShortestPathResult<G> res(N);
res.dist[n]= 0;
if constexpr(std::is_same_v<Algorithm, Graph::ShortestPath::BFS>) {
std::basic_string<bool> seen(N, false);
std::vector<size_t> q(N);
size_t push= 0, pop= 0;
q[push++]= n;
seen[n]= true;
while(push != pop) {
size_t node= q[pop++];
for(size_t next: g[node]) {
if(seen[next]) continue;
res.dist[next]= res.dist[node] + 1;
res.prev[next]= node;
q[push++]= next;
seen[next]= true;
}
}
} else if constexpr(std::is_same_v<Algorithm, Graph::ShortestPath::DPonDAG>) {
std::vector<size_t> indeg(N);
std::vector<size_t> q(N);
size_t push= 0, pop= 0;
q[push++]= n;
std::basic_string<bool> seen(N, false);
seen[n]= true;
while(push != pop) {
size_t node= q[pop++];
for(size_t next: g[node]) {
++indeg[next];
if(seen[next]) continue;
seen[next]= true;
q[push++]= next;
}
}
push= 0, pop= 0;
q[push++]= n;
while(push != pop) {
size_t node= q[pop++];
for(auto [next, cost]: g[node]) {
if(!indeg[next]) continue;
if(--indeg[next] == 0) {
q[push++]= next;
auto d= res.dist[node] + cost;
if(d < res.dist[next]) {
res.dist[next]= d;
res.prev[next]= node;
}
}
}
}
} else if constexpr(std::is_same_v<Algorithm, Graph::ShortestPath::BFS01>) {
std::basic_string<bool> seen(N, false);
std::vector<size_t> q(2 * N);
size_t front= N, back= N;
q[back++]= n;
while(front != back) {
size_t node= q[front++];
if(seen[node]) continue;
seen[node]= true;
for(auto [next, cost]: g[node]) {
bool c= static_cast<bool>(cost);
auto d= res.dist[node] + c;
if(d < res.dist[next]) {
res.dist[next]= d;
res.prev[next]= node;
if(c) q[back++]= next;
else q[--front]= next;
}
}
}
} else if constexpr(std::is_same_v<Algorithm, Graph::ShortestPath::Dijkstra>) {
std::vector<std::pair<typename G::weight_type, size_t>> q;
q.reserve(N);
constexpr size_t e= std::numeric_limits<size_t>::max();
std::vector<size_t> loc(N, e);
auto prioritize= [&](size_t n) {
auto tmp= q[loc[n]];
for(size_t i= loc[n], p; i != 0; i= p) {
p= (i - 1) >> 1;
if(q[p].first <= tmp.first) {
q[i]= tmp;
loc[n]= i;
return;
}
loc[q[p].second]= i;
q[i]= q[p];
}
q[0]= tmp;
loc[q[0].second]= 0;
};
auto push= [&](size_t n, typename G::weight_type w) {
loc[n]= q.size();
q.emplace_back(w, n);
prioritize(n);
};
auto pop= [&]() {
loc[q[0].second]= e;
std::swap(q[0], q.back());
q.pop_back();
auto tmp= q[0];
size_t i= 0, m= q.size() >> 1;
while(i < m) {
size_t l= (i << 1) + 1, r= (i << 1) + 2;
if(q[l].first <= q[r].first) {
if(tmp.first <= q[l].first) break;
loc[q[l].second]= i;
q[i]= q[l];
i= l;
} else {
if(tmp.first <= q[r].first) break;
loc[q[r].second]= i;
q[i]= q[r];
i= r;
}
}
loc[tmp.second]= i;
q[i]= tmp;
};
push(n, 0);
while(!q.empty()) {
auto [d, node]= q[0];
pop();
for(auto [next, cost]: g[node]) {
auto tmp= d + cost;
if(res.dist[next] <= tmp) continue;
res.dist[next]= tmp;
res.prev[next]= node;
if(loc[next] == e) push(next, tmp);
else {
q[loc[next]].first= tmp;
prioritize(next);
}
}
}
/*
        using node_type = std::pair<typename G::weight_type, size_t>;
        std::vector<node_type> c;
        c.reserve(g.count_vertices());
        std::priority_queue<node_type, std::vector<node_type>, std::greater<node_type>> q(std::greater<node_type>{}, std::move(c));
        q.emplace(0, n);
        while (!q.empty()) {
            auto [d, node] = q.top();
            q.pop();
            if (d > res.dist[node]) continue;
            for (auto [next, cost] : g[node]) {
                if (d + cost >= res.dist[next]) continue;
                q.emplace(d + cost, next);
                res.dist[next] = d + cost;
                res.prev[next] = node;
            }
        }
        */
} else if constexpr(std::is_same_v<Algorithm, Graph::ShortestPath::BellmanFord>) {
} else if constexpr(std::is_same_v<Algorithm, Graph::ShortestPath::DFSonTree>) {
std::vector<std::pair<size_t, size_t>> s;
s.reserve(N);
s.emplace_back(n, std::numeric_limits<size_t>::max());
while(!s.empty()) {
auto [node, prev]= s.back();
s.pop_back();
for(auto [next, cost]: g[node]) {
if(next == prev) continue;
res.dist[next]= res.dist[node] + cost;
res.prev[next]= node;
s.emplace_back(next, node);
}
}
} else if constexpr(std::is_same_v<Algorithm, Graph::ShortestPath::SPFA>) {
} else {
static_assert(std::conditional_t<false, Algorithm, bool>{}, "ShortestPath / Template parameter 'Algorithm' is invalid.");
}
return res;
}
template<class Algorithm, class G> constexpr std::vector<Graph::ShortestPathResult<G>> ShortestPath(const G& g) {
size_t N= g.count_vertices();
if constexpr(std::is_same_v<Algorithm, Graph::ShortestPath::FloydWarshall>) {
std::vector<Graph::ShortestPathResult<G>> res(N, Graph::ShortestPathResult<G>(N));
for(size_t i= 0; i != N; ++i) {
res[i].dist[i]= 0;
for(auto [j, w]: g[i]) {
if(w < res[i].dist[j]) {
res[i].dist[j]= w;
res[i].prev[j]= i;
}
}
}
for(size_t k= 0; k != N; ++k) {
for(size_t i= 0; i != N; ++i) {
for(size_t j= 0; j != N; ++j) {
if(res[i].dist[k] != Graph::ShortestPathResult<G>::inf && res[k].dist[j] != Graph::ShortestPathResult<G>::inf && res[i].dist[j] > res[i].dist[k] + res[k].dist[j]) {
res[i].dist[j]= res[i].dist[k] + res[k].dist[j];
res[i].prev[j]= res[k].prev[j];
}
}
}
}
return res;
} else {
std::vector<Graph::ShortestPathResult<G>> res;
for(size_t i= 0; i != N; ++i) res.emplace_back(ShortestPath<Algorithm>(g, i));
return res;
}
}
template<class UG> constexpr auto MinimumSpanningForest(const UG& g) {
size_t N= g.count_vertices();
size_t M= g.count_edges();
struct result_type {
UG graph;
typename UG::weight_type cost;
} res{UG(N), 0};
std::vector<std::pair<typename UG::weight_type, std::pair<size_t, size_t>>> edges;
edges.reserve(M);
for(size_t v= 0; v != N; ++v) {
for(auto [next, cost]: g[v]) {
edges.emplace_back(cost, std::pair<size_t, size_t>{v, next});
}
}
std::sort(edges.begin(), edges.end());
Graph::dsu uf(N);
for(auto [cost, tmp]: edges) {
auto [a, b]= tmp;
if(uf.merge(a, b)) {
res.graph.connect(a, b, cost);
res.cost+= cost;
}
}
return res;
}
template<class UG> constexpr auto MinimumSpanningForestCost(const UG& g) {
size_t N= g.count_vertices();
size_t M= g.count_edges();
typename UG::weight_type res{};
std::vector<std::pair<typename UG::weight_type, std::pair<size_t, size_t>>> edges;
edges.reserve(M);
for(size_t v= 0; v != N; ++v) {
for(auto [next, cost]: g[v]) {
edges.emplace_back(cost, std::pair<size_t, size_t>{v, next});
}
}
std::sort(edges.begin(), edges.end());
Graph::dsu uf(N);
for(auto [cost, tmp]: edges) {
auto [a, b]= tmp;
if(uf.merge(a, b)) res+= cost;
}
return res;
}
template<class UG> constexpr std::basic_string<bool> BipartiteGraphColoring(const UG& g) {
std::basic_string<bool> res(g.count_vertices(), false);
std::basic_string<bool> seen(g.count_vertices(), false);
std::vector<size_t> s;
s.reserve(g.count_vertices());
for(size_t v= 0, n= g.count_vertices(); v != n; ++v) {
s.push_back(v);
seen[v]= true;
while(!s.empty()) {
auto node= s.back();
s.pop_back();
for(size_t next: g[node]) {
if(seen[next]) {
if(res[node] == res[next]) return std::basic_string<bool>{};
continue;
}
res[next]= !res[node];
seen[next]= true;
s.push_back(next);
}
}
}
return res;
}
template<class UG> constexpr bool isBipartiteGraph(const UG& g) {
size_t N= g.count_vertices();
Graph::dsu uf(N * 2);
for(size_t i= 0; i != N; ++i) {
for(size_t j: g[i]) {
uf.merge(i, j + N);
uf.merge(i + N, j);
}
}
for(size_t i= 0; i != N; ++i) {
if(uf.root(i) == uf.root(i + N)) return false;
}
return true;
}
template<class DG> constexpr ConnectedComponents StronglyConnectedComponentsDecomposition(const DG& g) {
constexpr size_t e= std::numeric_limits<size_t>::max();
size_t N= g.count_vertices();
size_t new_ord= 0, group_num= 0;
std::vector<size_t> visited, low(N), ord(N, e), ids(N);
visited.reserve(N);
#ifdef ONLINE_JUDGE
auto dfs= [&](auto self, size_t v) -> void {
#else
auto dfs= [&](auto& self, size_t v) -> void {
#endif
low[v]= ord[v]= new_ord++;
visited.push_back(v);
for(size_t to: g[v]) {
if(ord[to] == e) {
if(!low[to]) self(self, to);
low[v]= std::min(low[v], low[to]);
} else low[v]= std::min(low[v], ord[to]);
}
if(low[v] == ord[v]) {
while(true) {
size_t u= visited.back();
visited.pop_back();
ord[u]= N;
ids[u]= group_num;
if(u == v) break;
}
++group_num;
}
};
for(size_t i= 0; i != N; ++i)
if(ord[i] == e) dfs(dfs, i);
std::vector<size_t> cnt(group_num);
for(size_t& x: ids) {
x= group_num - 1 - x;
++cnt[x];
}
ConnectedComponents res;
res.vertex.resize(group_num);
for(size_t i= 0; i != group_num; ++i) res.vertex[i].resize(cnt[i]);
for(size_t i= 0; i != N; ++i) res.vertex[ids[i]][--cnt[ids[i]]]= i;
res.aff= std::move(ids);
return res;
}
template<class UG= UndirectedGraph<void>> constexpr UG GridtoGraph(const std::vector<std::string>& grid, char c= '#') {
size_t H= grid.size(), W= grid[0].length();
UG res(H * W);
for(size_t i= 0; i != H; ++i) {
for(size_t j= 0; j != W; ++j) {
if(grid[i][j] == c) continue;
if(i != 0 && grid[i - 1][j] != c) res.connect((i - 1) * W + j, i * W + j);
if(j != 0 && grid[i][j - 1] != c) res.connect(i * W + j, i * W + (j - 1));
}
}
return res;
}
#endif
#if true || defined(INCLUDE_MODINT) || defined(INCLUDE_ALL)
template<class T> class ModintTraits: public T {
using base_type= T;
public:
using value_type= std::decay_t<decltype(base_type::mod())>;
using modint_type= ModintTraits;
constexpr static bool is_staticmod= !requires { base_type::set_mod(0); };
constexpr ModintTraits() noexcept: T() {}
template<class U> constexpr ModintTraits(U x) noexcept { operator=(x); }
constexpr explicit operator value_type() const noexcept { return val(); }
constexpr static void set_mod(value_type x) {
static_assert(!is_staticmod, "ModintTraits::set_mod / Mod must be dynamic.");
if(x <= 1) throw std::runtime_error("ModintTraits::set_mod / Mod must be at least 2.");
if(x == mod()) return;
base_type::set_mod(x);
}
constexpr value_type val() const noexcept { return base_type::val(); }
constexpr static value_type mod() noexcept { return base_type::mod(); }
template<class U> constexpr modint_type& operator=(U x) noexcept {
static_assert(std::is_integral_v<U>, "ModintTraits::operator= / Only integer types can be assigned.");
if constexpr(std::is_unsigned_v<U>) {
if constexpr(std::is_same_v<U, unsigned long long> || std::is_same_v<U, unsigned long>) base_type::assign(static_cast<std::uint64_t>(x));
else base_type::assign(static_cast<std::uint32_t>(x));
} else {
if(x < 0) {
if constexpr(std::is_same_v<U, long long> || std::is_same_v<U, long>) base_type::assign(static_cast<std::uint64_t>(-x));
else base_type::assign(static_cast<std::uint32_t>(-x));
base_type::neg();
} else {
if constexpr(std::is_same_v<U, long long> || std::is_same_v<U, long>) base_type::assign(static_cast<std::uint64_t>(x));
else base_type::assign(static_cast<std::uint32_t>(x));
}
}
return *this;
}
constexpr static modint_type raw(value_type x) noexcept {
modint_type res;
res.rawassign(x);
return res;
}
template<class Istream> friend Istream& operator>>(Istream& ist, modint_type& x) {
value_type n;
ist >> n;
x= n;
return ist;
}
template<class Ostream> friend Ostream& operator<<(Ostream& ost, modint_type x) { return ost << x.val(); }
constexpr modint_type inv() const {
value_type a= 1, b= 0, x= val(), y= mod();
if(x == 0) throw std::runtime_error("ModintTraits::inv / Zero division is not possible.");
while(true) {
if(x <= 1) {
if(x == 0) [[unlikely]]
break;
else return modint_type::raw(a);
}
b+= a * (y / x);
y%= x;
if(y <= 1) {
if(y == 0) [[unlikely]]
break;
else return modint_type::raw(mod() - b);
}
a+= b * (x / y);
x%= y;
}
throw std::runtime_error("ModintTraits::inv / Cannot calculate inverse element.");
}
constexpr modint_type pow(uint64_t e) const noexcept {
modint_type res= modint_type::raw(1), pow= *this;
while(e) {
modint_type tmp= pow * pow;
if(e & 1) res*= pow;
pow= tmp;
e>>= 1;
}
return res;
}
constexpr modint_type operator+() const noexcept { return *this; }
constexpr modint_type operator-() const noexcept {
modint_type res= *this;
res.neg();
return res;
}
constexpr modint_type& operator++() noexcept {
base_type::inc();
return *this;
}
constexpr modint_type& operator--() noexcept {
base_type::dec();
return *this;
}
constexpr modint_type operator++(int) noexcept {
modint_type copy= *this;
operator++();
return copy;
}
constexpr modint_type operator--(int) noexcept {
modint_type copy= *this;
operator--();
return copy;
}
constexpr modint_type& operator+=(modint_type x) noexcept {
base_type::add(x);
return *this;
}
constexpr modint_type& operator-=(modint_type x) noexcept {
base_type::sub(x);
return *this;
}
constexpr modint_type& operator*=(modint_type x) noexcept {
base_type::mul(x);
return *this;
}
constexpr modint_type& operator/=(modint_type x) {
operator*=(x.inv());
return *this;
}
friend constexpr modint_type operator+(modint_type l, modint_type r) noexcept { return modint_type(l)+= r; }
friend constexpr modint_type operator-(modint_type l, modint_type r) noexcept { return modint_type(l)-= r; }
friend constexpr modint_type operator*(modint_type l, modint_type r) noexcept { return modint_type(l)*= r; }
friend constexpr modint_type operator/(modint_type l, modint_type r) { return modint_type(l)/= r; }
friend constexpr bool operator==(modint_type l, modint_type r) noexcept { return l.val() == r.val(); }
friend constexpr bool operator!=(modint_type l, modint_type r) noexcept { return l.val() != r.val(); }
constexpr int legendre() const noexcept {
value_type res= pow((mod() - 1) >> 1).val();
return (res <= 1 ? static_cast<int>(res) : -1);
}
constexpr int jacobi() const noexcept {
value_type a= val(), n= mod();
if(a == 1) return 1;
if(std::gcd(a, n) != 1) return 0;
int res= 1;
while(a != 0) {
while(!(a & 1) && a != 0) {
a>>= 1;
if((n & 0b111) == 3 || (n & 0b111) == 5) res= -res;
}
if((a & 0b11) == 3 || (n & 0b11) == 3) res= -res;
std::swap(a, n);
a%= n;
}
if(n != 1) return 0;
return res;
}
constexpr modint_type sqrt() const noexcept {
const value_type vl= val(), md= mod();
if(vl <= 1) return *this;
auto get_min= [](modint_type x) {
return x.val() > (mod() >> 1) ? -x : x;
};
if((md & 0b11) == 3) return get_min(pow((md + 1) >> 2));
else if((md & 0b111) == 5) {
modint_type res= pow((md + 3) >> 3);
if constexpr(is_staticmod) {
constexpr modint_type p= modint_type::raw(2).pow((md - 1) >> 2);
res*= p;
} else if(res * res != *this) res*= modint_type::raw(2).pow((md - 1) >> 2);
return get_min(res);
} else {
value_type Q= md - 1;
uint32_t S= 0;
while((Q & 1) == 0) Q>>= 1, ++S;
if(std::countr_zero(md - 1) < 6) {
modint_type z= modint_type::raw(1);
while(z.legendre() != -1) ++z;
modint_type t= pow(Q), R= pow((Q + 1) / 2);
if(t.val() == 1) return R;
uint32_t M= S;
modint_type c= z.pow(Q);
do {
modint_type U= t * t;
uint32_t i= 1;
while(U.val() != 1) U= U * U, ++i;
modint_type b= c;
for(uint32_t j= 0; j < (M - i - 1); ++j) b*= b;
M= i, c= b * b, t*= c, R*= b;
} while(t.val() != 1);
return get_min(R);
} else {
modint_type a= 1;
while((a * a - *this).legendre() != -1) ++a;
modint_type res1= modint_type::raw(1), res2, pow1= a, pow2= modint_type::raw(1), w= a * a - *this;
value_type e= (md + 1) / 2;
while(true) {
if(e & 1) {
modint_type tmp= res1;
res1= res1 * pow1 + res2 * pow2 * w;
res2= tmp * pow2 + res2 * pow1;
}
e>>= 1;
if(e == 0) return get_min(res1);
modint_type tmp= pow1;
pow1= pow1 * pow1 + pow2 * pow2 * w;
pow2*= modint_type::raw(2) * tmp;
}
}
}
}
};
template<std::uint32_t mod_> class StaticModint32_impl {
using value_type= std::uint32_t;
using modint_type= StaticModint32_impl;
value_type val_= 0;
protected:
constexpr StaticModint32_impl() noexcept {}
constexpr value_type val() const noexcept { return val_; }
static constexpr value_type mod() noexcept { return mod_; }
constexpr void assign(std::uint32_t x) noexcept { val_= x % mod_; }
constexpr void assign(std::uint64_t x) noexcept { val_= x % mod_; }
constexpr void rawassign(value_type x) noexcept { val_= x; }
constexpr void neg() noexcept { val_= (val_ == 0 ? 0 : mod_ - val_); }
constexpr void inc() noexcept { val_= (val_ == mod_ - 1 ? 0 : val_ + 1); }
constexpr void dec() noexcept { val_= (val_ == 0 ? mod_ - 1 : val_ - 1); }
constexpr void add(modint_type x) noexcept {
if(mod_ - val_ > x.val_) val_+= x.val_;
else val_= x.val_ - (mod_ - val_);
}
constexpr void sub(modint_type x) noexcept {
if(val_ >= x.val_) val_-= x.val_;
else val_= mod_ - (x.val_ - val_);
}
constexpr void mul(modint_type x) noexcept { val_= static_cast<std::uint64_t>(val_) * x.val_ % mod_; }
};
template<std::uint32_t mod_= 998244353> using StaticModint32= ModintTraits<StaticModint32_impl<mod_>>;
template<std::uint64_t mod_> class StaticModint64_impl {
using value_type= std::uint64_t;
using modint_type= StaticModint64_impl;
value_type val_= 0;
protected:
constexpr StaticModint64_impl() noexcept {}
constexpr value_type val() const noexcept { return val_; }
static constexpr value_type mod() noexcept { return mod_; }
constexpr void assign(std::uint32_t x) noexcept {
if constexpr(mod_ < (1ull << 32)) val_= x % mod_;
else val_= x;
}
constexpr void assign(std::uint64_t x) noexcept { val_= x % mod_; }
constexpr void rawassign(value_type x) noexcept { val_= x; }
constexpr void neg() noexcept { val_= (val_ == 0 ? 0 : mod_ - val_); }
constexpr void inc() noexcept { val_= (val_ == mod_ - 1 ? 0 : val_ + 1); }
constexpr void dec() noexcept { val_= (val_ == 0 ? mod_ - 1 : val_ - 1); }
constexpr void add(modint_type x) noexcept {
if(mod_ - val_ > x.val_) val_+= x.val_;
else val_= x.val_ - (mod_ - val_);
}
constexpr void sub(modint_type x) noexcept {
if(val_ >= x.val_) val_-= x.val_;
else val_= mod_ - (x.val_ - val_);
}
constexpr void mul(modint_type x) noexcept { val_= static_cast<__uint128_t>(val_) * x.val_ % mod_; }
};
template<std::uint64_t mod_= 998244353> using StaticModint64= ModintTraits<StaticModint64_impl<mod_>>;
template<std::uint64_t mod_= 998244353> using StaticModint= std::conditional_t<(mod_ < (1ull << 32)), StaticModint32<mod_>, StaticModint64<mod_>>;
template<int id> class DynamicModint32_impl {
using value_type= std::uint32_t;
using modint_type= DynamicModint32_impl;
static inline value_type mod_= 0;
static inline std::uint64_t mod64_= 0;
static inline __uint128_t L_= 0;
value_type val_= 0;
value_type reduce(std::uint32_t c) const noexcept {
std::uint32_t q= (c * L_) >> 96;
return c - q * mod_;
}
value_type reduce(std::uint64_t c) const noexcept {
std::uint64_t q= (c * L_) >> 96;
return c - q * mod64_;
}
protected:
DynamicModint32_impl() noexcept {}
static void set_mod(value_type newmod) noexcept {
mod_= newmod, mod64_= newmod;
L_= ((__uint128_t(1) << 96) - 1) / mod_ + 1;
}
value_type val() const noexcept { return val_; }
static value_type mod() noexcept { return mod_; }
void assign(std::uint32_t x) noexcept { val_= reduce(x); }
void assign(std::uint64_t x) noexcept { val_= reduce(x); }
void rawassign(value_type x) noexcept { val_= x; }
void neg() noexcept { val_= (val_ == 0 ? 0 : mod_ - val_); }
void inc() noexcept { val_= (val_ == mod_ - 1 ? 0 : val_ + 1); }
void dec() noexcept { val_= (val_ == 0 ? mod_ - 1 : val_ - 1); }
void add(modint_type x) noexcept {
if(mod_ - val_ > x.val_) val_+= x.val_;
else val_= x.val_ - (mod_ - val_);
}
void sub(modint_type x) noexcept {
if(val_ >= x.val_) val_-= x.val_;
else val_= mod_ - (x.val_ - val_);
}
void mul(modint_type x) noexcept { val_= reduce(static_cast<std::uint64_t>(val_) * x.val_); }
};
template<int id= 0> using DynamicModint32= ModintTraits<DynamicModint32_impl<id>>;
template<int id> class DynamicModint64_impl {
using value_type= std::uint64_t;
using modint_type= DynamicModint64_impl;
static inline value_type mod_= 0;
static inline __uint128_t M_= 0;
value_type val_= 0;
protected:
DynamicModint64_impl() noexcept {}
static void set_mod(value_type newmod) noexcept {
mod_= newmod;
M_= std::numeric_limits<__uint128_t>::max() / mod_ + std::has_single_bit(mod_);
}
value_type val() const noexcept { return val_; }
static value_type mod() noexcept { return mod_; }
void assign(std::uint64_t x) noexcept { val_= x % mod_; }
void rawassign(value_type x) noexcept { val_= x; }
void neg() noexcept { val_= (val_ == 0 ? 0 : mod_ - val_); }
void inc() noexcept { val_= (val_ == mod_ - 1 ? 0 : val_ + 1); }
void dec() noexcept { val_= (val_ == 0 ? mod_ - 1 : val_ - 1); }
void add(modint_type x) noexcept {
if(mod_ - val_ > x.val_) val_+= x.val_;
else val_= x.val_ - (mod_ - val_);
}
void sub(modint_type x) noexcept {
if(val_ >= x.val_) val_-= x.val_;
else val_= mod_ - (x.val_ - val_);
}
void mul(modint_type x) noexcept {
const std::uint64_t a= (((M_ * val_) >> 64) * x.val_) >> 64;
const std::uint64_t b= val_ * x.val_;
const std::uint64_t c= a * mod_;
const std::uint64_t d= b - c;
const bool e= d < mod_;
const std::uint64_t f= d - mod_;
val_= e ? d : f;
}
};
template<int id= 0> using DynamicModint64= ModintTraits<DynamicModint64_impl<id>>;
template<class Modint> class ModManager {
Modint::value_type prev;
public:
ModManager() { prev= Modint::mod(); }
~ModManager() {
if(prev != 0) Modint::set_mod(prev);
}
void set_mod(Modint::value_type newmod) { Modint::set_mod(newmod); }
};
template<class Modint> class SwitchModint;
template<uint32_t mod> class SwitchModint<StaticModint32<mod>> {
public:
using modint_type= StaticModint32<mod>;
using value_type= typename modint_type::value_type;
};
template<uint64_t mod> class SwitchModint<StaticModint64<mod>> {
public:
using modint_type= StaticModint64<mod>;
using value_type= typename modint_type::value_type;
};
template<int id> class SwitchModint<DynamicModint32<id>> {
public:
using modint_type= DynamicModint32<id>;
using value_type= typename modint_type::value_type;
SwitchModint(value_type mod) { modint_type::set_mod(mod); }
};
template<int id> class SwitchModint<DynamicModint64<id>> {
public:
using modint_type= DynamicModint64<id>;
using value_type= typename modint_type::value_type;
SwitchModint(value_type mod) { modint_type::set_mod(mod); }
};
#endif
#if true || defined(INCLUDE_RANDOM) || defined(INCLUDE_ALL)
constexpr uint32_t mwc(uint32_t a, uint32_t b) noexcept {
a= 36969 * (a & 65535) + (a >> 16);
b= 18000 * (b & 65535) + (b >> 16);
return (a << 16) + (b & 65535);
}
constexpr uint32_t splitmax(uint32_t n) noexcept {
n+= 0x9e3779b9;
n= (n ^ (n >> 15)) * 0x85ebca6b;
n= (n ^ (n >> 13)) * 0xc2b2ae35;
return (n ^ (n >> 16));
}
constexpr uint64_t splitmax(uint64_t n) noexcept {
n+= 0x9e3779b97f4a7c15;
n= (n ^ (n >> 30)) * 0xbf58476d1ce4e5b9;
n= (n ^ (n >> 27)) * 0x94d049bb133111eb;
return n ^ (n >> 31);
}
#if __cplusplus >= 202001U
consteval uint64_t Randomnum(std::source_location loc= std::source_location::current()) noexcept {
auto string_hash_= [](const char* s) {
const char* last= s + strlen(s);
uint64_t res= 0;
while(s != last) {
uint64_t tmp= 0;
for(size_t i= 0; i < 16 && s != last; ++i) tmp= tmp << 4 | *(s++);
res^= splitmax(tmp);
}
return res;
};
return splitmax(splitmax(static_cast<uint64_t>(loc.column()) << 32 | loc.line()) ^ string_hash_(loc.file_name()) ^ string_hash_(__TIME__));
}
#endif
//https://ja.wikipedia.org/wiki/Xorshift
class xorshift64 {
using value_type= uint64_t;
value_type val;
public:
using result_type= uint64_t;
static constexpr size_t word_size= sizeof(result_type) * 8;
static constexpr value_type default_seed= 0xcafef00dd15ea5e5;
constexpr xorshift64(): xorshift64(default_seed) {}
constexpr explicit xorshift64(value_type value): val(value) {}
constexpr result_type operator()() {
val^= val << 7;
val^= val >> 9;
return val;
};
constexpr void discard(unsigned long long z) {
for(unsigned long long i= 0; i < z; ++i) operator()();
}
static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
static constexpr result_type min() { return 1; }
constexpr void seed(value_type value= default_seed) { val= value; }
constexpr bool operator==(xorshift64 x) { return val == x.val; }
};
//https://twitter.com/rho__o/status/1734784410808160661
class pcg32 {
using value_type= uint64_t;
value_type val;
public:
using result_type= uint32_t;
static constexpr size_t word_size= sizeof(result_type) * 8;
static constexpr value_type default_seed= 0xcafef00dd15ea5e5;
constexpr pcg32(): pcg32(default_seed) {}
constexpr explicit pcg32(value_type value): val(value) {}
constexpr result_type operator()() {
uint64_t x= val;
x^= x >> 22;
val*= 0xcafef00dd15ea5e5;
return (x >> (22 + (x >> 61)));
};
constexpr void discard(unsigned long long z) {
for(unsigned long long i= 0; i < z; ++i) operator()();
}
static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
static constexpr result_type min() { return 0; }
constexpr void seed(value_type value= default_seed) { val= value; }
constexpr bool operator==(pcg32 x) { return val == x.val; }
};
//https://twitter.com/rho__o/status/1734786100873564174
class lcgs32 {
using value_type= uint64_t;
value_type val;
public:
using result_type= uint32_t;
static constexpr size_t word_size= sizeof(result_type) * 8;
static constexpr value_type default_seed= 0xcafef00dd15ea5e5;
constexpr lcgs32(): lcgs32(default_seed) {}
constexpr explicit lcgs32(value_type value): val(value) {}
constexpr result_type operator()() {
uint64_t x= val;
val*= 0xcafef00dd15ea5e5;
return (x >> 32);
};
constexpr void discard(unsigned long long z) {
for(unsigned long long i= 0; i < z; ++i) operator()();
}
static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
static constexpr result_type min() { return 0; }
constexpr void seed(value_type value= default_seed) { val= value; }
constexpr bool operator==(lcgs32 x) { return val == x.val; }
};
//https://www.pcg-random.org/posts/bounded-rands.html
template<class URBG> constexpr uint32_t randi32(URBG& g, uint32_t max) {
return (static_cast<std::uint64_t>(g() & 4294967295) * max) >> 32;
}
template<class URBG> constexpr uint32_t randi32(URBG& g, uint32_t min, uint32_t max) {
return static_cast<std::uint32_t>((static_cast<std::uint64_t>(g() & 4294967295) * (max - min)) >> 32) + min;
}
template<class URBG> constexpr uint64_t randi64(URBG& g, uint64_t max) {
return (static_cast<__uint128_t>(g()) * max) >> 64;
}
template<class URBG> constexpr uint64_t randi64(URBG& g, uint64_t min, uint64_t max) {
return static_cast<uint64_t>((static_cast<__uint128_t>(g()) * (max - min)) >> 64) + min;
}
//https://speakerdeck.com/hole/rand01?slide=31
template<class URBG> constexpr float canocicaled(URBG& g) {
return std::bit_cast<float>((127u << 23) | (static_cast<std::uint32_t>(g()) & 0x7fffff)) - 1.0f;
}
template<class URBG> constexpr float randf32(URBG& g, float max) {
return canocicaled(g) * max;
}
template<class URBG> constexpr float randf64(URBG& g, float min, float max) {
return canocicaled(g) * (max - min) + min;
}
#endif
#if true || defined(INCLUDE_NUMERIC) || defined(INCLUDE_ALL)
constexpr uint32_t isqrt32(const uint32_t n) {
return std::sqrt((long double)n);
}
//https://zenn.dev/mizar/articles/791698ea860581#%E5%B9%B3%E6%96%B9%E6%95%B0%E3%81%AE%E5%88%A4%E5%AE%9A
constexpr uint64_t isqrt64(const uint64_t n) {
if(n <= 1) [[unlikely]]
return n;
const int k= 32 - std::countl_zero(n - 1) / 2;
uint64_t s= 1ull << k;
uint64_t t= (s + (n >> k)) / 2;
while(t < s) {
s= t;
t= (s + (n / s)) / 2;
}
return s;
}
//https://lpha-z.hatenablog.com/entry/2020/05/24/231500
template<class T> constexpr T BinaryGCD(T x, T y) {
if(x == 0 || y == 0) [[unlikely]]
return x | y;
const int n= std::countr_zero(x);
const int m= std::countr_zero(y);
const int l= n < m ? n : m;
x>>= n;
y>>= m;
T s;
int t;
while(x != y) {
s= y < x ? x - y : y - x;
t= std::countr_zero(s);
y= y < x ? y : x;
x= s >> t;
}
return x << l;
}
template<bool trial_division= true> bool isPrime32(const std::uint32_t x) {
if constexpr(trial_division) {
if(x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0 || x % 11 == 0 || x % 13 == 0 || x % 17 == 0 || x % 19 == 0 || x % 23 == 0 || x % 29 == 0 || x % 31 == 0 || x % 37 == 0 || x % 41 == 0 || x % 43 == 0) return x <= 43 && (x == 2 || x == 3 || x == 5 || x == 7 || x == 11 || x == 13 || x == 17 || x == 19 || x == 23 || x == 29 || x == 31 || x == 37 || x == 41 || x == 43);
if(x < 47 * 47) return (x > 1);
} else {
if(x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0) return x <= 7 && (x == 2 || x == 3 || x == 5 || x == 7);
if(x < 11 * 11) return (x > 1);
}
//https://www.techneon.com/download/is.prime.32.base.data
const static std::uint16_t bases[]= {1216, 1836, 8885, 4564, 10978, 5228, 15613, 13941, 1553, 173, 3615, 3144, 10065, 9259, 233, 2362, 6244, 6431, 10863, 5920, 6408, 6841, 22124, 2290, 45597, 6935, 4835, 7652, 1051, 445, 5807, 842, 1534, 22140, 1282, 1733, 347, 6311, 14081, 11157, 186, 703, 9862, 15490, 1720, 17816, 10433, 49185, 2535, 9158, 2143, 2840, 664, 29074, 24924, 1035, 41482, 1065, 10189, 8417, 130, 4551, 5159, 48886,
786, 1938, 1013, 2139, 7171, 2143, 16873, 188, 5555, 42007, 1045, 3891, 2853, 23642, 148, 3585, 3027, 280, 3101, 9918, 6452, 2716, 855, 990, 1925, 13557, 1063, 6916, 4965, 4380, 587, 3214, 1808, 1036, 6356, 8191, 6783, 14424, 6929, 1002, 840, 422, 44215, 7753, 5799, 3415, 231, 2013, 8895, 2081, 883, 3855, 5577, 876, 3574, 1925, 1192, 865, 7376, 12254, 5952, 2516, 20463, 186,
5411, 35353, 50898, 1084, 2127, 4305, 115, 7821, 1265, 16169, 1705, 1857, 24938, 220, 3650, 1057, 482, 1690, 2718, 4309, 7496, 1515, 7972, 3763, 10954, 2817, 3430, 1423, 714, 6734, 328, 2581, 2580, 10047, 2797, 155, 5951, 3817, 54850, 2173, 1318, 246, 1807, 2958, 2697, 337, 4871, 2439, 736, 37112, 1226, 527, 7531, 5418, 7242, 2421, 16135, 7015, 8432, 2605, 5638, 5161, 11515, 14949,
748, 5003, 9048, 4679, 1915, 7652, 9657, 660, 3054, 15469, 2910, 775, 14106, 1749, 136, 2673, 61814, 5633, 1244, 2567, 4989, 1637, 1273, 11423, 7974, 7509, 6061, 531, 6608, 1088, 1627, 160, 6416, 11350, 921, 306, 18117, 1238, 463, 1722, 996, 3866, 6576, 6055, 130, 24080, 7331, 3922, 8632, 2706, 24108, 32374, 4237, 15302, 287, 2296, 1220, 20922, 3350, 2089, 562, 11745, 163, 11951};
using mint= DynamicModint32<-1>;
mint::set_mod(x);
const std::uint32_t h= x * 0xad625b89;
std::uint32_t d= x - 1;
mint cur= bases[h >> 24];
int s= std::countr_zero(d);
d>>= s;
cur= cur.pow(d);
if(cur.val() == 1) return true;
while(--s) {
if(cur.val() == x - 1) return true;
cur*= cur;
}
return cur.val() == x - 1;
}
template<bool trial_division= true, bool hashing= false> bool isPrime64(const std::uint64_t x) {
if(x < 4294967296) return isPrime32<trial_division>(x);
if constexpr(trial_division) {
if(x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0 || x % 11 == 0 || x % 13 == 0 || x % 17 == 0 || x % 19 == 0 || x % 23 == 0 || x % 29 == 0 || x % 31 == 0 || x % 37 == 0 || x % 41 == 0 || x % 43 == 0) return false;
} else {
if(x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0) return false;
}
using mint= DynamicModint64<-1>;
mint::set_mod(x);
std::uint64_t d= x - 1;
const int s= std::countr_zero(d);
d>>= s;
{
auto test= [&](std::uint64_t a) -> bool {
mint cur= mint(a).pow(d);
if(cur.val() <= 1) return true;
int i= s;
while(--i) {
if(cur.val() == x - 1) return true;
cur*= cur;
}
return cur.val() == x - 1;
};
//http://miller-rabin.appspot.com/
if(x < 585226005592931977ull) {
if(x < 7999252175582851ull) {
if(x < 350269456337ull) return test(4230279247111683200ull) && test(14694767155120705706ull) && test(16641139526367750375ull);
else if(x < 55245642489451ull) return test(2ull) && test(141889084524735ull) && test(1199124725622454117ull) && test(11096072698276303650ull);
else return test(2ull) && test(4130806001517ull) && test(149795463772692060ull) && test(186635894390467037ull) && test(3967304179347715805ull);
} else return test(2ull) && test(123635709730000ull) && test(9233062284813009ull) && test(43835965440333360ull) && test(761179012939631437ull) && test(1263739024124850375ull);
} else return test(2ull) && test(325ull) && test(9375ull) && test(28178ull) && test(450775ull) && test(9780504ull) && test(1795265022ull);
}
}
uint64_t FindFactor(uint64_t x) {
if(x % 2 == 0) return 2;
using mint= DynamicModint64<-1>;
mint::set_mod(x);
static xorshift64 engine;
constexpr size_t repeat= 8192;
retry:
mint r= mint::raw(randi64(engine, 1, x)), a, b= mint::raw(randi64(engine, 1, x));
size_t k= repeat;
while(true) {
for(size_t i= k + 1; --i;) b= b * b + r;
a= b;
for(size_t i= 0; i < k; i+= repeat) {
mint mul= mint::raw(1), prev= b;
for(size_t j= repeat + 1; --j;) mul*= a - (b= b * b + r);
uint64_t g= BinaryGCD(mul.val(), x);
if(g == x) {
mul= mint::raw(1);
do {
mul*= a - (prev= prev * prev + r);
g= BinaryGCD(mul.val(), x);
} while(g == 1);
if(g == x) goto retry;
}
if(g != 1) return g;
}
k*= 2;
}
}
template<bool sort= true, bool trial_division= true, bool hashing= false> std::vector<uint64_t> EnumrateFactors(uint64_t x) {
std::vector<uint64_t> res;
if(x == 0) return res;
if constexpr(trial_division) {
#define DIV(d) \
while(x % d == 0) x/= d, res.push_back(d);
{
DIV(2)
DIV(3)
DIV(5) DIV(7) DIV(11) DIV(13) DIV(17) DIV(19) DIV(23) DIV(29) DIV(31) DIV(37) DIV(41) DIV(43)
}
#undef DIV
}
if(x == 1) return res;
uint64_t tmp1[64];
uint64_t tmp2[64];
uint64_t *begin1= tmp1, *begin2= tmp2, *end1= tmp1, *end2= tmp2;
*(end1++)= x;
while(begin1 != end1) {
for(uint64_t* i= begin1; i != end1; ++i) {
uint64_t n= *i;
if(isPrime64<false, hashing>(n)) res.push_back(n);
else {
uint64_t g= FindFactor(n);
*(end2++)= g;
*(end2++)= n / g;
}
}
uint64_t* tmp= begin1;
begin1= begin2;
end1= end2;
begin2= tmp;
end2= tmp;
}
if constexpr(sort) std::sort(res.begin(), res.end());
return res;
}
template<class Modint> class InvTable: SwitchModint<Modint> {
using mint= Modint;
std::vector<mint> table;
public:
using SwitchModint<Modint>::SwitchModint;
constexpr void init(size_t mx) {
table.resize(mx);
table[1]= mint::raw(1);
if(mx == 1) return;
auto mod= mint::mod();
for(size_t i= 2; i != mx; ++i) table[i]= -table[mod % i] * mint::raw(mod / i);
}
constexpr mint operator()(size_t n) const noexcept { return table[n]; }
};
template<class Modint> class COMTable_primemod: SwitchModint<Modint> {
using mint= Modint;
std::vector<mint> fac, finv;
public:
using SwitchModint<Modint>::SwitchModint;
constexpr void init(size_t mx) {
fac.resize(mx);
finv.resize(mx);
fac[0]= finv[0]= mint::raw(1);
if(mx > 1) fac[1]= finv[1]= mint::raw(1);
if(mx > 2) {
for(size_t i= 2; i != mx; ++i) fac[i]= fac[i - 1] * mint::raw(i);
finv.back()= fac.back().inv();
for(size_t i= mx - 1; i != 2; --i) finv[i - 1]= finv[i] * mint::raw(i);
}
}
constexpr mint operator()(size_t n, size_t k) const noexcept {
if(n < k) return 0;
else return fac[n] * finv[k] * finv[n - k];
}
};
#if __GNUC__ >= 12
constexpr
#endif
std::vector<uint32_t>
generate_primes(uint32_t size) {
if(size <= 1000) {
constexpr uint32_t primes[]= {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433,
439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997};
return std::vector<uint32_t>(std::begin(primes), std::upper_bound(std::begin(primes), std::end(primes), size));
}
const uint32_t flag_size= size / 30 + (size % 30 != 0);
constexpr uint32_t table1[]= {0, 1, 7, 1, 11, 1, 7, 1, 13, 1, 7, 1, 11, 1, 7, 1, 17, 1, 7, 1, 11, 1, 7, 1, 13, 1, 7, 1, 11, 1, 7, 1, 19, 1, 7, 1, 11, 1, 7, 1, 13, 1, 7, 1, 11, 1, 7, 1, 17, 1, 7, 1, 11, 1, 7, 1, 13, 1, 7, 1, 11, 1, 7, 1, 23, 1, 7, 1, 11, 1, 7, 1, 13, 1, 7, 1, 11, 1, 7, 1, 17, 1, 7, 1, 11, 1, 7, 1, 13, 1, 7, 1, 11, 1, 7, 1, 19, 1, 7, 1, 11, 1, 7, 1, 13, 1, 7, 1, 11, 1, 7, 1, 17, 1, 7, 1, 11, 1, 7, 1, 13, 1, 7, 1, 11, 1, 7, 1,
29, 1, 7, 1, 11, 1, 7, 1, 13, 1, 7, 1, 11, 1, 7, 1, 17, 1, 7, 1, 11, 1, 7, 1, 13, 1, 7, 1, 11, 1, 7, 1, 19, 1, 7, 1, 11, 1, 7, 1, 13, 1, 7, 1, 11, 1, 7, 1, 17, 1, 7, 1, 11, 1, 7, 1, 13, 1, 7, 1, 11, 1, 7, 1, 23, 1, 7, 1, 11, 1, 7, 1, 13, 1, 7, 1, 11, 1, 7, 1, 17, 1, 7, 1, 11, 1, 7, 1, 13, 1, 7, 1, 11, 1, 7, 1, 19, 1, 7, 1, 11, 1, 7, 1, 13, 1, 7, 1, 11, 1, 7, 1, 17, 1, 7, 1, 11, 1, 7, 1, 13, 1, 7, 1, 11, 1, 7, 1};
constexpr uint8_t table2[]= {0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 4, 0, 8, 0, 0, 0, 16, 0, 32, 0, 0, 0, 64, 0, 0, 0, 0, 0, 128};
std::vector<uint8_t> flag(flag_size, 0xffu);
flag[0]= 0b11111110u;
std::vector<uint32_t> primes{2, 3, 5};
#if __GNUC__ >= 12
double primes_size= std::is_constant_evaluated() ? size / 8 : size / std::log(size);
#else
double primes_size= size / std::log(size);
#endif
primes.reserve(static_cast<size_t>(1.1 * primes_size));
std::vector<uint32_t> sieved(static_cast<size_t>(primes_size));
uint32_t *first= sieved.data(), *last;
uint32_t k, l, x, y;
uint8_t temp;
for(k= 0; k * k < flag_size; ++k) {
while(flag[k] != 0) {
x= 30ull * k + table1[flag[k]];
uint32_t limit= size / x;
primes.push_back(x);
last= first;
bool smaller= true;
for(l= k; smaller; ++l) {
for(temp= flag[l]; temp != 0; temp&= (temp - 1)) {
y= 30u * l + table1[temp];
if(y > limit) {
smaller= false;
break;
}
*(last++)= x * y;
}
}
flag[k]&= (flag[k] - 1);
for(uint32_t* i= first; i < last; ++i) flag[*i / 30]^= table2[*i % 30];
}
}
for(; k < flag_size; k++) {
while(flag[k] != 0) {
x= 30 * k + table1[flag[k]];
if(x > size) return primes;
primes.push_back(x);
flag[k]&= (flag[k] - 1);
}
}
return primes;
}
#endif
#if true || defined(INCLUDE_LIS) || defined(INCLUDE_ALL)
template<std::ranges::range T, class Comp= std::less<std::ranges::range_value_t<T>>> constexpr auto LongestIncreasingSubsequence(const T& R, const Comp& c= Comp()) {
using U= std::ranges::range_value_t<T>;
std::vector<U> dp(std::ranges::size(R));
std::vector<size_t> idx(dp.size());
size_t* i= idx.data();
U *begin= dp.data(), *last= dp.data();
for(const U& x: R) {
U* loc= std::lower_bound(begin, last, x, c);
*(i++)= loc - begin;
last+= (loc == last);
*loc= x;
}
size_t cnt= last - begin - 1;
std::vector<size_t> res(last - begin);
for(size_t i= dp.size(); i != 0;)
if(idx[--i] == cnt) res[cnt--]= i;
return res;
}
template<std::ranges::range T, class Comp= std::less<std::ranges::range_value_t<T>>> constexpr std::vector<size_t> LongestIncreasingSubsequenceLength(const T& R, const Comp& c= Comp()) {
using U= std::ranges::range_value_t<T>;
std::vector<U> dp(std::ranges::size(R));
U *begin= dp.data(), *last= dp.data();
std::vector<size_t> res(dp.size());
for(size_t i= 0; const U& x: R) {
U* loc= std::lower_bound(begin, last, x, c);
last+= (loc == last);
*loc= x;
res[i++]= last - begin;
}
return res;
}
#endif
#if true || defined(INCLUDE_STRING) || defined(INCLUDE_ALL)
std::string unionchar(char a, char b) {
return {a, b};
}
bool is_palindrome(const std::string& S) {
size_t n= S.length();
for(size_t i= 0; i < (n >> 1); i++)
if(S[i] != S[n - i - 1]) return false;
return true;
}
std::vector<std::string> strsplit(const std::string& s, char c= ' ') {
std::vector<std::string> res;
const char* prev= s.data();
bool f= false;
for(const char* cur= s.data(); *cur != '\0'; ++cur) {
if(*cur == c) {
if(f) {
res.emplace_back(prev, cur);
f= false;
}
} else if(!f) {
prev= cur;
f= true;
}
}
if(s.back() != c) {
res.emplace_back(prev, s.data() + s.length());
}
return res;
}
bool in_char(const std::string& S, char c) {
for(char d: S)
if(c == d) return true;
return false;
}
bool in_str(const std::string& S, const std::string& T) {
if(T.length() > S.length()) return false;
for(size_t i= 0, n= S.length() - T.length(); i != n; ++i) {
bool f= true;
for(size_t j= 0, m= T.length(); j != m; ++j)
if(S[i + j] != T[j]) {
f= false;
break;
}
if(f) return true;
}
return false;
}
std::string fill_str(const std::string& S, size_t N, char c= '0', bool dir= true) {
if(S.length() >= N) return S;
if(dir) return std::string(N - S.length(), c) + S;
else return S + std::string(N - S.length(), c);
}
size_t hamming_distance(const std::string& S, const std::string& T) {
if(S.length() != T.length()) throw std::runtime_error("hamming_distance / S.length() and T.length() must be the same.");
size_t res= 0;
for(size_t i= 0, n= S.length(); i != n; ++i)
if(S[i] != T[i]) ++res;
return res;
}
template<class T= int> constexpr T str_to_int(const std::string& s, int base= 10) {
static_assert(std::is_integral_v<T>, "str_to_int / Result type must be integral.");
T res{};
std::from_chars(s.c_str(), s.c_str() + s.length(), res, base);
return res;
}
template<class T> constexpr std::string int_to_str(T val, int base= 10) {
static_assert(std::is_integral_v<T>, "int_to_str / Result type must be integral.");
char buf[sizeof(T) * 8];
char* last= std::to_chars(buf, buf + sizeof(T) * 8, val, base).ptr;
return std::string(buf, last);
}
bool is_capitalized(const std::string& S) {
if(S.length() == 0) return true;
if(S[0] < 'A' || S[0] > 'Z') return false;
for(size_t i= 1, n= S.length(); i < n; ++i) {
if(S[i] < 'a' || S[i] > 'z') return false;
}
return true;
}
std::string strreplace(const std::string& S, const char before, const char after) {
std::string res= S;
for(char& c: res) {
if(c == before) c= after;
}
return res;
}
#endif
#if true || defined(INCLUDE_GEOMETRY) || defined(INCLUDE_ALL)
double to_rad(double d) {
return (d / 180.0) * 3.141592653589793;
}
double to_deg(double r) {
return (r / 3.141592653589793) * 180.0;
}
double sind(double d) {
return std::sin(to_rad(d));
}
double cosd(double d) {
return std::cos(to_rad(d));
}
double tand(double d) {
return std::tan(to_rad(d));
}
double asind(double x) {
return to_deg(std::asin(x));
}
double acosd(double x) {
return to_deg(std::acos(x));
}
double atand(double x) {
return to_deg(std::atan(x));
}
double atan2d(double y, double x) {
return to_deg(std::atan2(y, x));
}
long double to_rad(long double d) {
return (d / 180.0) * 3.141592653589793;
}
long double to_deg(long double r) {
return (r / 3.141592653589793) * 180.0;
}
long double sind(long double d) {
return std::sin(to_rad(d));
}
long double cosd(long double d) {
return std::cos(to_rad(d));
}
long double tand(long double d) {
return std::tan(to_rad(d));
}
long double asind(long double x) {
return to_deg(std::asin(x));
}
long double acosd(long double x) {
return to_deg(std::acos(x));
}
long double atand(long double x) {
return to_deg(std::atan(x));
}
long double atan2d(long double y, long double x) {
return to_deg(std::atan2(y, x));
}
float hypot_fl(float x1, float y1, float x2, float y2) {
return std::hypot(x1 - x2, y1 - y2);
}
double hypot_db(double x1, double y1, double x2, double y2) {
return std::hypot(x1 - x2, y1 - y2);
}
long double hypot_ld(long double x1, long double y1, long double x2, long double y2) {
return std::hypot(x1 - x2, y1 - y2);
}
template<class T> struct vector2D {
T x, y;
constexpr vector2D(): x(0), y(0) {}
constexpr vector2D(const T& X, const T& Y): x(X), y(Y) {}
constexpr T length() { return static_cast<T>(sqrt(static_cast<double>(x * x + y * y))); }
constexpr double slope() { return atan2(y, x); }
constexpr vector2D<T>& rorate(double rad) { return *this= vector2D<T>(x * cos(rad) - y * sin(rad), x * sin(rad) + y * cos(rad)); }
constexpr vector2D<T>& rorate_deg(double deg) {
double rad= (deg / 180) * 3.141592653589793;
return *this= vector2D<T>(x * cos(rad) - y * sin(rad), x * sin(rad) + y * cos(rad));
}
constexpr vector2D<T> operator~() {
T&& len= length();
return vector2D<T>(x / len, y / len);
}
constexpr vector2D<T>& operator=(const vector2D<T>& v) {
x= v.x, y= v.y;
return *this;
}
constexpr vector2D<T>& operator+=(const vector2D<T>& v) {
x+= v.x, y+= v.y;
return *this;
}
constexpr vector2D<T>& operator-=(const vector2D<T>& v) {
x-= v.x, y-= v.y;
return *this;
}
constexpr vector2D<T>& operator*=(const T& s) {
x*= s, y*= s;
return *this;
}
constexpr vector2D<T>& operator/=(const T& s) {
x/= s, y/= s;
return *this;
}
friend constexpr bool operator==(const vector2D& v1, const vector2D& v2) { return v1.x == v2.x && v1.y == v2.y; }
friend constexpr bool operator!=(const vector2D& v1, const vector2D& v2) { return v1.x != v2.x || v1.y != v2.y; }
friend constexpr bool operator<(const vector2D& v1, const vector2D& v2) {
T &&len1= v1.length(), len2= v2.length();
if(len1 == len2) return v1.slope() < v2.slope();
else return len1 < len2;
}
friend constexpr bool operator>(const vector2D& v1, const vector2D& v2) {
T &&len1= v1.length(), len2= v2.length();
if(len1 == len2) return v1.slope() > v2.slope();
else return len1 > len2;
}
friend constexpr bool operator<=(const vector2D& v1, const vector2D& v2) { return !(v1 > v2); }
friend constexpr bool operator>=(const vector2D& v1, const vector2D& v2) { return !(v1 < v2); }
friend constexpr vector2D<T> operator+(const vector2D<T>& v1, const vector2D<T>& v2) { return vector2D<T>(v1.x + v2.x, v1.y + v2.y); }
friend constexpr vector2D<T> operator-(const vector2D<T>& v1, const vector2D<T>& v2) { return vector2D<T>(v1.x - v2.x, v1.y - v2.y); }
friend constexpr vector2D<T> operator*(const vector2D<T>& v, const T& s) { return vector2D<T>(v.x * s, v.y * s); }
friend constexpr vector2D<T> operator*(const T& s, const vector2D<T>& v) { return vector2D<T>(v.x * s, v.y * s); }
friend constexpr vector2D<T> operator/(const vector2D<T>& v, const T& s) { return vector2D<int>(v.x / s, v.y / s); }
friend constexpr T operator^(const vector2D<T>& v1, const vector2D<T>& v2) { return v1.x * v2.x + v1.y * v2.y; }
friend constexpr T operator*(const vector2D<T>& v1, const vector2D<T>& v2) { return v1.x * v2.y - v1.y * v2.x; }
};
#endif
#if true || defined(INCLUDE_GCDLCM) || defined(INCLUDE_ALL)
template<class T> constexpr T gcd(const std::vector<T>& v) {
if(v.empty()) return 1;
T ans= v[0];
for(size_t i= 1; i < v.size(); i++) ans= std::gcd(ans, v[i]);
return ans;
}
template<class T> constexpr T lcm(const std::vector<T>& v) {
if(v.empty()) return 1;
T ans= v[0];
for(size_t i= 1; i < v.size(); i++) ans= std::lcm(ans, v[i]);
return ans;
}
template<class T> constexpr T extend_gcd(T a, T b, T& x, T& y) {
T c= 1, d= 0;
x= 0, y= 1;
for(T div= a / b; div * b < a; div= a / b) {
T e= a - b * div, f= c - x * div, g= d - y * div;
a= b, b= e, c= x, d= y, x= f, y= g;
}
return b;
}
#endif
#if true || defined(INCLUDE_FLOORSUM) || defined(INCLUDE_ALL)
// calc ∑ floor((A × i + B) / M) (0 <= i < N)
template<class T> constexpr T FloorSum(T n, T m, T a, T b) {
T res= 0;
while(true) {
if(m <= a) {
res+= n * (n - 1) / 2 * (a / m);
a%= m;
}
if(m <= b) {
res+= n * (b / m);
b%= m;
}
T last= a * n + b;
if(last < m) return res;
n= last / m;
b= last % m;
std::swap(m, a);
}
}
#endif
#if true || defined(INCLUDE_TIMER) || defined(INCLUDE_ALL)
class timer {
std::chrono::system_clock::time_point start;
public:
std::vector<decltype(start - start)> lapped;
void reset() { start= std::chrono::system_clock::now(); }
timer() { reset(); }
auto get() { return std::chrono::system_clock::now() - start; }
void lap() { lapped.push_back(get()); }
void clear() { lapped.clear(); }
#if __GNUC__ >= 12
void print() { std::cout << "##Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(get()) << std::endl; }
void print_lap() {
std::cout << "##lap: ";
for(auto time: lapped) std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(time) << ' ';
std::cout << std::endl;
}
#endif
};
#endif
//#include "atcoder/all"
#ifdef ONLINE_JUDGE
#define GSH_USE_COMPILE_TIME_CALCULATION
#define NDEBUG
#else
#define GSH_DIAGNOSTICS_COLOR
// #define NDEBUG
#endif
#if __has_include(<stdfloat>)
#include <stdfloat>
#endif
namespace gsh {
namespace itype {
#if !defined(INT8_MAX) || !defined(UINT8_MAX)
static_assert(false, "This library needs std::int8_t and std::uint8_t.");
#endif
#if !defined(INT16_MAX) || !defined(UINT16_MAX)
static_assert(false, "This library needs std::int16_t and std::uint16_t.");
#endif
#if !defined(INT32_MAX) || !defined(UINT32_MAX)
static_assert(false, "This library needs std::int32_t and std::uint32_t.");
#endif
#if !defined(INT64_MAX) || !defined(UINT64_MAX)
static_assert(false, "This library needs std::int64_t and std::uint64_t.");
#endif
using i8= std::int8_t;
using u8= std::uint8_t;
using i16= std::int16_t;
using u16= std::uint16_t;
using i32= std::int32_t;
using u32= std::uint32_t;
using i64= std::int64_t;
using u64= std::uint64_t;
}// namespace itype
namespace ftype {
class InvalidFloat16Tag;
class InvalidBfloat16Tag;
class InvalidFloat128Tag;
#ifdef __STDCPP_FLOAT16_T__
using f16= std::float16_t;
#else
using f16= InvalidFloat16Tag;
#endif
#ifdef __STDCPP_FLOAT32_T__
using f32= std::float32_t;
#else
static_assert(std::numeric_limits<float>::is_iec559, "There are no types compliant with IEC 559 binary32.");
using f32= float;
#endif
#ifdef __STDCPP_FLOAT64_T__
using f64= std::float64_t;
#else
static_assert(std::numeric_limits<double>::is_iec559, "There are no types compliant with IEC 559 binary64.");
using f64= double;
#endif
#ifdef __STDCPP_FLOAT128_T__
using f128= std::float128_t;
#elif defined(__SIZEOF_FLOAT128__)
using f128= std::conditional_t<std::numeric_limits<long double>::is_iec559 && sizeof(long double) == 16, long double, __float128>;
#else
using f128= std::conditional_t<std::numeric_limits<long double>::is_iec559 && sizeof(long double) == 16, long double, InvalidFloat128Tag>;
#endif
#ifdef __STDCPP_BFLOAT16_T__
using bf16= std::bfloat16_t;
#else
using bf16= InvalidBfloat16Tag;
#endif
}// namespace ftype
namespace ctype {
using c8= char;
using wc= wchar_t;
using utf8= char8_t;
using utf16= char16_t;
using utf32= char32_t;
}// namespace ctype
}// namespace gsh
namespace gsh {
class Exception {
char str[512];
char* cur= str;
void write(const char* x) {
for(int i= 0; i != 512; ++i, ++cur) {
if(x[i] == '\0') break;
*cur= x[i];
}
}
void write(long long x) {
if(x == 0) *(cur++)= '0';
else {
if(x < 0) {
*(cur++)= '-';
x= -x;
}
char buf[20];
int i= 0;
while(x != 0) buf[i++]= x % 10 + '0', x/= 10;
while(i--) *(cur++)= buf[i];
}
}
template<class T, class... Args> void generate_message(T x, Args... args) {
write(x);
if constexpr(sizeof...(Args) > 0) generate_message(args...);
}
public:
Exception() noexcept { *cur= '\0'; }
Exception(const Exception& x) noexcept {
for(int i= 0; i != 512; ++i) str[i]= x.str[i];
cur= x.cur;
}
explicit Exception(const char* what_arg) noexcept {
for(int i= 0; i != 512; ++i, ++cur) {
*cur= what_arg[i];
if(what_arg[i] == '\0') break;
}
}
template<class... Args> explicit Exception(Args... args) noexcept {
generate_message(args...);
*cur= '\0';
}
Exception& operator=(const Exception& x) noexcept {
for(int i= 0; i != 512; ++i) str[i]= x.str[i];
cur= x.cur;
return *this;
}
const char* what() const noexcept { return str; }
};
}// namespace gsh
namespace gsh {
namespace internal {
template<class D> class ArithmeticInterface {
constexpr D& derived() { return *static_cast<D*>(this); }
constexpr const D& derived() const { return *static_cast<const D*>(this); }
public:
constexpr D operator++(int) noexcept(std::is_nothrow_copy_constructible_v<D> && noexcept(++derived())) {
D copy= derived();
++derived();
return copy;
}
constexpr D operator--(int) noexcept(std::is_nothrow_copy_constructible_v<D> && noexcept(--derived())) {
D copy= derived();
--derived();
return copy;
}
constexpr D operator+() const noexcept(std::is_nothrow_copy_constructible_v<D>)
requires requires(D x) { -x; }
{
return derived();
}
constexpr bool operator!() const noexcept(noexcept(static_cast<bool>(derived()))) { return !static_cast<bool>(derived()); }
friend constexpr auto operator+(const D& t1, const D& t2) noexcept(noexcept(D(t1)+= t2)) { return D(t1)+= t2; }
friend constexpr auto operator-(const D& t1, const D& t2) noexcept(noexcept(D(t1)-= t2)) { return D(t1)-= t2; }
friend constexpr auto operator*(const D& t1, const D& t2) noexcept(noexcept(D(t1)*= t2)) { return D(t1)*= t2; }
friend constexpr auto operator/(const D& t1, const D& t2) noexcept(noexcept(D(t1)/= t2)) { return D(t1)/= t2; }
friend constexpr auto operator%(const D& t1, const D& t2) noexcept(noexcept(D(t1)%= t2)) { return D(t1)%= t2; }
friend constexpr auto operator&(const D& t1, const D& t2) noexcept(noexcept(D(t1)&= t2)) { return D(t1)&= t2; }
friend constexpr auto operator|(const D& t1, const D& t2) noexcept(noexcept(D(t1)|= t2)) { return D(t1)|= t2; }
friend constexpr auto operator^(const D& t1, const D& t2) noexcept(noexcept(D(t1)^= t2)) { return D(t1)^= t2; }
template<class T> friend constexpr auto operator<<(const D& t1, const T& t2) noexcept(noexcept(D(t1)<<= t2)) { return D(t1)<<= t2; }
template<class T> friend constexpr auto operator>>(const D& t1, const T& t2) noexcept(noexcept(D(t1)>>= t2)) { return D(t1)>>= t2; }
};
template<class D> class IteratorInterface {
constexpr D& derived() { return *static_cast<D*>(this); }
constexpr const D& derived() const { return *static_cast<const D*>(this); }
public:
using size_type= itype::u32;
using difference_type= itype::i32;
constexpr D operator++(int) noexcept(std::is_nothrow_copy_constructible_v<D> && noexcept(++derived())) {
D copy= derived();
++derived();
return copy;
}
constexpr D operator--(int) noexcept(std::is_nothrow_copy_constructible_v<D> && noexcept(--derived())) {
D copy= derived();
--derived();
return copy;
}
constexpr auto operator->() noexcept(noexcept(&*derived())) { return &*derived(); }
constexpr auto operator->() const noexcept(noexcept(&*derived())) { return &*derived(); }
template<class T> friend constexpr D operator+(const D& a, T&& n) noexcept(noexcept(D(a)+= std::forward<T>(n))) { return D(a)+= std::forward<T>(n); }
template<class T> friend constexpr D operator-(const D& a, T&& n) noexcept(noexcept(D(a)-= std::forward<T>(n))) { return D(a)-= std::forward<T>(n); }
};
}// namespace internal
}// namespace gsh
#define GSH_INTERNAL_SELECT1(a, ...) a
#define GSH_INTERNAL_SELECT2(a, b, ...) b
#define GSH_INTERNAL_SELECT3(a, b, c, ...) c
#define GSH_INTERNAL_SELECT4(a, b, c, d, ...) d
#define GSH_INTERNAL_SELECT5(a, b, c, d, e, ...) e
#define GSH_INTERNAL_SELECT6(a, b, c, d, e, f, ...) f
#define GSH_INTERNAL_SELECT7(a, b, c, d, e, f, g, ...) g
#define GSH_INTERNAL_SELECT8(a, b, c, d, e, f, g, h, ...) h
#define GSH_INTERNAL_SELECT9(a, b, c, d, e, f, g, h, i, ...) i
#define GSH_INTERNAL_STR(s) #s
#define GSH_INTERNAL_CONCAT(a, b) a##b
#define GSH_INTERNAL_VA_SIZE(...) GSH_INTERNAL_SELECT8(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0)
#if defined(__clang__) || defined(__ICC)
#define GSH_INTERNAL_UNROLL(n) _Pragma(GSH_INTERNAL_STR(unroll n))
#elif defined __GNUC__
#define GSH_INTERNAL_UNROLL(n) _Pragma(GSH_INTERNAL_STR(GCC unroll n))
#else
#define GSH_INTERNAL_UNROLL(n)
#endif
#ifdef __GNUC__
#define GSH_INTERNAL_INLINE __attribute__((always_inline))
#define GSH_INTERNAL_NOINLINE __attribute__((noinline))
#elif defined _MSC_VER
#define GSH_INTERNAL_INLINE [[msvc::forceinline]]
#define GSH_INTERNAL_NOINLINE [[msvc::noinline]]
#else
#define GSH_INTERNAL_INLINE
#define GSH_INTERNAL_NOINLINE
#endif
#if defined(__GNUC__) || defined(__ICC)
#define GSH_INTERNAL_RESTRICT __restrict__
#elif defined _MSC_VER
#define GSH_INTERNAL_RESTRICT __restrict
#else
#define GSH_INTERNAL_RESTRICT
#endif
#ifdef __clang__
#define GSH_INTERNAL_PUSH_ATTRIBUTE(apply, ...) _Pragma(GSH_INTERNAL_STR(clang attribute push(__attribute__((__VA_ARGS__)), apply_to= apply)))
#define GSH_INTERNAL_POP_ATTRIBUTE _Pragma("clang attribute pop")
#elif defined __GNUC__
#define GSH_INTERNAL_PUSH_ATTRIBUTE(apply, ...) _Pragma("GCC push_options") _Pragma(GSH_INTERNAL_STR(GCC __VA_ARGS__))
#define GSH_INTERNAL_POP_ATTRIBUTE _Pragma("GCC pop_options")
#else
#define GSH_INTERNAL_PUSH_ATTRIBUTE(apply, ...)
#define GSH_INTERNAL_POP_ATTRIBUTE
#endif
namespace gsh {
[[noreturn]] void Unreachable() {
#if defined __GNUC__ || defined __clang__
__builtin_unreachable();
#elif _MSC_VER
__assume(false);
#else
[[maybe_unused]] itype::u32 n= 1 / 0;
#endif
};
GSH_INTERNAL_INLINE constexpr void Assume(const bool f) {
if(std::is_constant_evaluated()) return;
#if defined __clang__
__builtin_assume(f);
#elif defined __GNUC__
if(!f) __builtin_unreachable();
#elif _MSC_VER
__assume(f);
#else
if(!f) Unreachable();
#endif
}
template<bool Likely= true> GSH_INTERNAL_INLINE constexpr bool Expect(const bool f) {
if(std::is_constant_evaluated()) return f;
#if defined __GNUC__ || defined __clang__
return __builtin_expect(f, Likely);
#else
if constexpr(Likely) {
if(f) [[likely]]
return true;
else return false;
} else {
if(f) [[unlikely]]
return false;
else return true;
}
#endif
}
GSH_INTERNAL_INLINE constexpr bool Unpredictable(const bool f) {
if(std::is_constant_evaluated()) return f;
#if defined __clang__
return __builtin_unpredictable(f);
#elif defined __GNUC__
return __builtin_expect_with_probability(f, 1, 0.5);
#else
return f;
#endif
}
class InPlaceTag {};
[[maybe_unused]] constexpr InPlaceTag InPlace;
template<class T>
requires std::is_trivial_v<T>
GSH_INTERNAL_INLINE constexpr void MemorySet(T* p, ctype::c8 byte, itype::u32 len) {
if(std::is_constant_evaluated()) {
struct mem {
ctype::c8 buf[sizeof(T)]= {};
};
mem init;
for(itype::u32 i= 0; i != sizeof(T); ++i) init.buf[i]= byte;
for(itype::u32 i= 0; i != len / sizeof(T); ++i) p[i]= std::bit_cast<T>(init);
if(len % sizeof(T) != 0) {
auto& ref= p[len / sizeof(T)];
mem tmp= std::bit_cast<mem>(ref);
for(itype::u32 i= 0; i != len % sizeof(T); ++i) tmp.buf[i]= byte;
ref= std::bit_cast<T>(tmp);
}
} else std::memset(p, byte, len);
}
template<class T>
requires std::is_trivial_v<T>
GSH_INTERNAL_INLINE constexpr itype::u32 MemoryChar(T* p, ctype::c8 byte, itype::u32 len) {
if(std::is_constant_evaluated()) {
struct mem {
ctype::c8 buf[sizeof(T)]= {};
};
for(itype::u32 i= 0; i != len / sizeof(T); ++i) {
mem tmp= std::bit_cast<mem>(p[i]);
for(itype::u32 j= 0; j != sizeof(T); ++j) {
if(tmp.buf[j] == byte) return i * sizeof(T) + j;
}
}
if(len % sizeof(T) != 0) {
mem tmp= std::bit_cast<mem>(p[len / sizeof(T)]);
for(itype::u32 i= 0; i != len % sizeof(T); ++i) {
if(tmp.buf[i] == byte) return len / sizeof(T) * sizeof(T) + i;
}
}
return 0xffffffff;
} else {
const void* tmp= std::memchr(p, byte, len);
return (tmp == nullptr ? 0xffffffff : static_cast<const ctype::c8*>(tmp) - reinterpret_cast<const ctype::c8*>(p));
}
}
template<class T, class U>
requires std::is_trivial_v<T>
GSH_INTERNAL_INLINE constexpr void MemoryCopy(T* GSH_INTERNAL_RESTRICT dst, U* GSH_INTERNAL_RESTRICT src, itype::u32 len) {
if(std::is_constant_evaluated()) {
struct mem1 {
ctype::c8 buf[sizeof(T)]= {};
};
struct mem2 {
ctype::c8 buf[sizeof(U)]= {};
};
mem1 tmp1;
mem2 tmp2;
for(itype::u32 i= 0; i != len; ++i) {
if(i % sizeof(U) == 0) tmp2= std::bit_cast<mem2>(src[i / sizeof(U)]);
tmp1.buf[i % sizeof(T)]= tmp2.buf[i % sizeof(U)];
if((i + 1) % sizeof(T) == 0) {
dst[i / sizeof(T)]= std::bit_cast<T>(tmp1);
tmp1= mem1{};
}
}
if(len % sizeof(T) != 0) {
mem1 tmp3= std::bit_cast<mem1>(dst[len / sizeof(T)]);
for(itype::u32 i= 0; i != len % sizeof(T); ++i) tmp3.buf[i]= tmp1.buf[i];
dst[len / sizeof(T)]= std::bit_cast<T>(tmp3);
}
} else std::memcpy(dst, src, len);
}
/*
template<class T, class U>
    requires std::is_trivially_copyable_v<T> && std::is_trivially_copyable_v<U>
GSH_INTERNAL_INLINE constexpr void MemoryMove(T* dst, U* src, itype::u32 len) {
    if (std::is_constant_evaluated()) {
    } else std::memmove(dst, src, len);
}
*/
GSH_INTERNAL_INLINE constexpr itype::u32 StrLen(const ctype::c8* p) {
if(std::is_constant_evaluated()) {
auto q= p;
while(*q != '\0') ++q;
return q - p;
} else return std::strlen(p);
}
template<class T, class U, class V> constexpr bool InRange(const T& l, const U& x, const V& r) {
return l <= x && x < r;
}
namespace internal {
template<itype::u32 N, class First, class... Tail> class TypeAtImpl: public TypeAtImpl<N - 1, Tail...> {};
template<class T, class... Types> class TypeAtImpl<0, T, Types...> {
public:
using type= T;
};
}// namespace internal
template<itype::u32 N, class... Types> using TypeAt= typename internal::TypeAtImpl<N, Types...>::type;
template<class... Types> class TypeArr {
public:
constexpr static itype::u32 size() noexcept { return sizeof...(Types); }
template<itype::u32 N> using type= std::conditional_t<(N < sizeof...(Types)), TypeAt<N, Types...>, void>;
};
template<> class TypeArr<> {
public:
constexpr static itype::u32 size() noexcept { return 0; }
template<itype::u32 N> using type= void;
};
}// namespace gsh
#ifdef _MSC_VER
#include <intrin.h>
#include <immintrin.h>
#pragma intrinsic(_umul128, __umulh, _udiv128, __shiftleft128, __shiftright128)
#endif
namespace gsh {
namespace internal {
GSH_INTERNAL_INLINE constexpr std::pair<itype::u64, itype::u64> Mulu128(itype::u64 muler, itype::u64 mulnd) noexcept {
#if defined(__SIZEOF_INT128__)
__uint128_t tmp= static_cast<__uint128_t>(muler) * mulnd;
return {tmp >> 64, tmp};
#else
#if defined(_MSC_VER)
if(!std::is_constant_evaluated()) {
itype::u64 high;
itype::u64 low= _umul128(muler, mulnd, &high);
return {high, low};
}
#endif
itype::u64 u1= (muler & 0xffffffff);
itype::u64 v1= (mulnd & 0xffffffff);
itype::u64 t= (u1 * v1);
itype::u64 w3= (t & 0xffffffff);
itype::u64 k= (t >> 32);
muler>>= 32;
t= (muler * v1) + k;
k= (t & 0xffffffff);
itype::u64 w1= (t >> 32);
mulnd>>= 32;
t= (u1 * mulnd) + k;
k= (t >> 32);
return {(muler * mulnd) + w1 + k, (t << 32) + w3};
#endif
}
GSH_INTERNAL_INLINE constexpr itype::u64 Mulu128High(itype::u64 muler, itype::u64 mulnd) noexcept {
#if defined(__SIZEOF_INT128__)
return static_cast<itype::u64>((static_cast<__uint128_t>(muler) * mulnd) >> 64);
#else
#if defined(_MSC_VER)
if(!std::is_constant_evaluated()) return __umulh(muler, mulnd);
#endif
return Mulu128(muler, mulnd).first;
#endif
}
GSH_INTERNAL_INLINE constexpr std::pair<itype::u64, itype::u64> Divu128(itype::u64 high, itype::u64 low, itype::u64 div) noexcept {
#if(defined(__GNUC__) || defined(__ICC)) && defined(__x86_64__)
if constexpr(sizeof(void*) == 8) {
if(!std::is_constant_evaluated()) {
itype::u64 res, rem;
__asm__("divq %[v]" : "=a"(res), "=d"(rem) : [v] "r"(div), "a"(low), "d"(high));
return {res, rem};
}
}
#elif defined(_MSC_VER)
if(!std::is_constant_evaluated()) {
itype::u64 rem;
itype::u64 res= _udiv128(high, low, div, &rem);
return {res, rem};
}
#endif
#if defined(__SIZEOF_INT128__)
__uint128_t n= (static_cast<__uint128_t>(high) << 64 | low);
__uint128_t res= n / div;
return {res, n - res * div};
#else
itype::u64 res= 0;
itype::u64 cur= high;
for(itype::u64 i= 0; i != 64; ++i) {
itype::u64 large= cur >> 63;
cur= cur << 1 | (low >> 63);
low<<= 1;
large|= (cur >= div);
res= res << 1 | large;
cur-= div & (0 - large);
}
return {res, cur};
#endif
}
GSH_INTERNAL_INLINE constexpr std::pair<itype::u64, itype::u64> Divu128(itype::u64 high, itype::u64 low, itype::u64 dhigh, itype::u64 dlow) noexcept {
if(dhigh == 0) {
if(high >= dlow) {
itype::u64 qh= high / dlow, r= high % dlow;
itype::u64 ql= internal::Divu128(r, low, dlow).first;
high= qh, low= ql;
} else {
low= internal::Divu128(high, low, dlow).first;
high= 0;
}
} else if(high >= dhigh) {
Assume(dhigh != 0);
itype::i32 s= std::countl_zero(dhigh);
if(s != 0) {
itype::u64 yh= dhigh << s | dlow >> (64 - s), yl= dlow << s;
auto [q, r]= internal::Divu128(high >> (64 - s), high << s | low >> (64 - s), yh);
auto [mh, ml]= internal::Mulu128(q, yl);
low= q - (mh >= r && (q >= (low << s) || mh != r));
high= 0;
} else {
low= (high > dhigh || low >= dlow);
high= 0;
}
} else {
low= 0;
high= 0;
}
return {high, low};
}
GSH_INTERNAL_INLINE constexpr std::pair<itype::u64, itype::u64> Modu128(itype::u64 high, itype::u64 low, itype::u64 dhigh, itype::u64 dlow) noexcept {
if(dhigh == 0) {
low= internal::Divu128(high % dlow, low, dlow).second;
high= 0;
} else if(high >= dhigh) {
Assume(dhigh != 0);
itype::i32 s= std::countl_zero(dhigh);
if(s != 0) {
itype::u64 yh= dhigh << s | dlow >> (64 - s), yl= dlow << s;
auto [q, r]= internal::Divu128(high >> (64 - s), high << s | low >> (64 - s), yh);
auto [mh, ml]= internal::Mulu128(q, yl);
itype::u64 d= q - (mh >= r && (q >= (low << s) || mh != r));
auto [dh, dl]= internal::Mulu128(d, dlow);
high-= dh + d * dhigh;
high-= low < dl;
low-= dl;
} else if(high > dhigh || low >= dlow) {
high-= dhigh;
high-= low < dlow;
low-= dlow;
}
}
return {high, low};
}
GSH_INTERNAL_INLINE constexpr itype::u64 ShiftLeft128High(itype::u64 high, itype::u64 low, itype::i32 shift) noexcept {
Assume(0 <= shift && shift < 64);
#ifdef _MSC_VER
if(!std::is_constant_evaluated()) return __shiftleft128(low, high, shift);
#endif
return high << shift | (low >> 1 >> (63 - shift));
}
GSH_INTERNAL_INLINE constexpr itype::u64 ShiftRight128Low(itype::u64 high, itype::u64 low, itype::i32 shift) noexcept {
Assume(0 <= shift && shift < 64);
#ifdef _MSC_VER
if(!std::is_constant_evaluated()) return __shiftright128(low, high, shift);
#endif
return low >> shift | (high << 1 << (63 - shift));
}
}// namespace internal
#if defined(__SIZEOF_INT128__)
namespace itype {
using i128= __int128_t;
using u128= __uint128_t;
}// namespace itype
#else
namespace internal {
struct LittleEndian128 {
itype::u64 high, low;
};
struct BigEndian128 {
itype::u64 low, high;
};
using SwitchEndian128= std::conditional_t<std::endian::big != std::endian::native, internal::LittleEndian128, internal::BigEndian128>;
}// namespace internal
namespace itype {
class alignas(16) i128;
class alignas(16) u128: protected internal::SwitchEndian128, public internal::ArithmeticInterface<u128> {
public:
constexpr u128() noexcept { high= 0, low= 0; }
constexpr u128(const i128& n) noexcept;
template<std::unsigned_integral T> constexpr u128(const T& n) noexcept { high= 0, low= n; }
template<std::signed_integral T> constexpr u128(const T& n) noexcept {
if(n < 0) {
high= -1, low= ~(0 - n);
operator++();
} else {
high= 0, low= n;
}
}
constexpr u128(const u128&) noexcept= default;
constexpr u128& operator=(const u128&) noexcept= default;
constexpr u128 operator-() const noexcept {
u128 res= ~*this;
++res;
return res;
}
constexpr u128& operator+=(const u128& n) noexcept {
high+= n.high;
low+= n.low;
high+= low < n.low;
return *this;
}
constexpr u128& operator-=(const u128& n) noexcept {
high-= n.high;
high-= low < n.low;
low-= n.low;
return *this;
}
constexpr u128& operator*=(const u128& n) noexcept {
auto [hi, lw]= internal::Mulu128(low, n.low);
high= low * n.high + high * n.low + hi;
low= lw;
return *this;
}
constexpr u128& operator/=(const u128& n) noexcept {
auto [hi, lo]= internal::Divu128(high, low, n.high, n.low);
high= hi, low= lo;
return *this;
}
constexpr u128& operator%=(const u128& n) noexcept {
auto [hi, lo]= internal::Modu128(high, low, n.high, n.low);
high= hi, low= lo;
return *this;
}
constexpr u128& operator++() noexcept {
++low;
high+= (low == 0);
return *this;
}
constexpr u128& operator--() noexcept {
high-= (low == 0);
--low;
return *this;
}
constexpr u128 operator~() const noexcept {
u128 res;
res.high= ~high;
res.low= ~low;
return res;
}
constexpr u128& operator&=(const u128& n) noexcept {
high&= n.high;
low&= n.low;
return *this;
}
constexpr u128& operator|=(const u128& n) noexcept {
high|= n.high;
low|= n.low;
return *this;
}
constexpr u128& operator^=(const u128& n) noexcept {
high^= n.high;
low^= n.low;
return *this;
}
constexpr u128& operator<<=(itype::i32 shift) noexcept {
if(shift >= 64) {
high= low << (shift - 64);
low= 0;
} else {
high= internal::ShiftLeft128High(high, low, shift);
low<<= shift;
}
return *this;
}
constexpr u128& operator>>=(itype::i32 shift) noexcept {
if(shift >= 64) {
low= high >> (shift - 64);
high= 0;
} else {
low= internal::ShiftRight128Low(high, low, shift);
high>>= shift;
}
return *this;
}
friend constexpr bool operator==(const u128& a, const u128& b) noexcept { return a.high == b.high && a.low == b.low; }
friend constexpr std::strong_ordering operator<=>(const u128& a, const u128& b) noexcept {
if(a.high < b.high || (a.high == b.high && a.low < b.low)) return std::strong_ordering::less;
if(a.high == b.high && a.low == b.low) return std::strong_ordering::equal;
if(a.high > b.high || (a.high == b.high && a.low > b.low)) return std::strong_ordering::greater;
Unreachable();
}
constexpr operator bool() const noexcept { return low != 0 || high != 0; }
template<std::integral T>
requires(!std::same_as<T, i128>)
constexpr operator T() const noexcept {
return static_cast<T>(low);
}
};
class i128: private u128, public internal::ArithmeticInterface<i128> {
friend class u128;
public:
constexpr i128() noexcept: u128() {}
constexpr i128(const u128& n) noexcept: u128(n) {}
template<std::integral T> constexpr i128(const T& n) noexcept: u128(n) {}
constexpr i128(const i128&) noexcept= default;
constexpr i128& operator=(const i128&) noexcept= default;
constexpr i128 operator-() const noexcept { return u128::operator-(); }
constexpr i128& operator+=(const i128& n) noexcept { return static_cast<i128&>(u128::operator+=(n)); }
constexpr i128& operator-=(const i128& n) noexcept { return static_cast<i128&>(u128::operator-=(n)); }
constexpr i128& operator*=(const i128& n) noexcept { return static_cast<i128&>(u128::operator*=(n)); }
constexpr i128& operator/=(const i128&) noexcept {
// TODO
return *this;
}
constexpr i128& operator%=(const i128&) noexcept {
// TODO
return *this;
}
constexpr i128& operator++() noexcept { return static_cast<i128&>(u128::operator++()); }
constexpr i128& operator--() noexcept { return static_cast<i128&>(u128::operator--()); }
constexpr i128 operator~() const noexcept { return static_cast<i128>(u128::operator~()); }
constexpr i128& operator&=(const i128& n) noexcept { return static_cast<i128&>(u128::operator&=(n)); }
constexpr i128& operator|=(const i128& n) noexcept { return static_cast<i128&>(u128::operator|=(n)); }
constexpr i128& operator^=(const i128& n) noexcept { return static_cast<i128&>(u128::operator^=(n)); }
constexpr i128& operator<<=(itype::i32 shift) noexcept { return static_cast<i128&>(u128::operator<<=(shift)); }
constexpr i128& operator>>=(itype::i32 shift) noexcept { return static_cast<i128&>(u128::operator>>=(shift)); }
constexpr operator bool() const noexcept { return static_cast<bool>(static_cast<const u128&>(*this)); }
template<std::integral T>
requires(!std::same_as<T, u128>)
constexpr operator T() const noexcept {
return static_cast<T>(static_cast<const u128&>(*this));
}
friend constexpr bool operator==(const i128& a, const i128& b) noexcept { return a.high == b.high && a.low == b.low; }
friend constexpr std::strong_ordering operator<=>(const i128& a, const i128& b) noexcept {
constexpr itype::u64 mask= static_cast<itype::u64>(1) << 63;
itype::u64 ahigh= a.high ^ mask, bhigh= b.high ^ mask;
if(ahigh < bhigh || (ahigh == bhigh && a.low < b.low)) return std::strong_ordering::less;
if(ahigh == bhigh && a.low == b.low) return std::strong_ordering::equal;
if(ahigh > bhigh || (ahigh == bhigh && a.low > b.low)) return std::strong_ordering::greater;
Unreachable();
}
};
constexpr u128::u128(const i128& n) noexcept {
high= n.high, low= n.low;
}
}// namespace itype
}
namespace std {
template<> struct common_type<gsh::itype::u128, gsh::itype::i128> {
using type= gsh::itype::u128;
};
template<> struct common_type<gsh::itype::i128, gsh::itype::u128> {
using type= gsh::itype::u128;
};
template<std::integral T> struct common_type<gsh::itype::u128, T> {
using type= gsh::itype::u128;
};
template<std::integral T> struct common_type<T, gsh::itype::u128> {
using type= gsh::itype::u128;
};
template<std::floating_point T> struct common_type<gsh::itype::u128, T> {
using type= T;
};
template<std::floating_point T> struct common_type<T, gsh::itype::u128> {
using type= T;
};
template<std::integral T> struct common_type<gsh::itype::i128, T> {
using type= gsh::itype::i128;
};
template<std::integral T> struct common_type<T, gsh::itype::i128> {
using type= gsh::itype::i128;
};
template<std::floating_point T> struct common_type<gsh::itype::i128, T> {
using type= T;
};
template<std::floating_point T> struct common_type<T, gsh::itype::i128> {
using type= T;
};
}// namespace std
namespace gsh {
namespace internal {
template<class T, class U> constexpr bool U128OrI128= (std::same_as<T, itype::u128> || std::same_as<T, itype::i128> || std::same_as<U, itype::u128> || std::same_as<U, itype::i128>);
}
namespace itype {
template<class T, class U>
requires internal::U128OrI128<T, U>
constexpr auto operator+(const T& a, const U& b) noexcept(noexcept(static_cast<std::common_type_t<T, U>>(a) + static_cast<std::common_type_t<T, U>>(b))) {
return static_cast<std::common_type_t<T, U>>(a) + static_cast<std::common_type_t<T, U>>(b);
}
template<class T, class U>
requires internal::U128OrI128<T, U>
constexpr auto operator-(const T& a, const U& b) noexcept(noexcept(static_cast<std::common_type_t<T, U>>(a) - static_cast<std::common_type_t<T, U>>(b))) {
return static_cast<std::common_type_t<T, U>>(a) - static_cast<std::common_type_t<T, U>>(b);
}
template<class T, class U>
requires internal::U128OrI128<T, U>
constexpr auto operator*(const T& a, const U& b) noexcept(noexcept(static_cast<std::common_type_t<T, U>>(a) * static_cast<std::common_type_t<T, U>>(b))) {
return static_cast<std::common_type_t<T, U>>(a) * static_cast<std::common_type_t<T, U>>(b);
}
template<class T, class U>
requires internal::U128OrI128<T, U>
constexpr auto operator/(const T& a, const U& b) noexcept(noexcept(static_cast<std::common_type_t<T, U>>(a) / static_cast<std::common_type_t<T, U>>(b))) {
return static_cast<std::common_type_t<T, U>>(a) / static_cast<std::common_type_t<T, U>>(b);
}
template<class T, class U>
requires internal::U128OrI128<T, U>
constexpr auto operator&(const T& a, const U& b) noexcept(noexcept(static_cast<std::common_type_t<T, U>>(a) & static_cast<std::common_type_t<T, U>>(b))) {
return static_cast<std::common_type_t<T, U>>(a) & static_cast<std::common_type_t<T, U>>(b);
}
template<class T, class U>
requires internal::U128OrI128<T, U>
constexpr auto operator|(const T& a, const U& b) noexcept(noexcept(static_cast<std::common_type_t<T, U>>(a) | static_cast<std::common_type_t<T, U>>(b))) {
return static_cast<std::common_type_t<T, U>>(a) | static_cast<std::common_type_t<T, U>>(b);
}
template<class T, class U>
requires internal::U128OrI128<T, U>
constexpr auto operator^(const T& a, const U& b) noexcept(noexcept(static_cast<std::common_type_t<T, U>>(a) ^ static_cast<std::common_type_t<T, U>>(b))) {
return static_cast<std::common_type_t<T, U>>(a) ^ static_cast<std::common_type_t<T, U>>(b);
}
template<class T, class U>
requires internal::U128OrI128<T, U>
constexpr auto operator==(const T& a, const U& b) noexcept(noexcept(static_cast<std::common_type_t<T, U>>(a) == static_cast<std::common_type_t<T, U>>(b))) {
return static_cast<std::common_type_t<T, U>>(a) == static_cast<std::common_type_t<T, U>>(b);
}
template<class T, class U>
requires internal::U128OrI128<T, U>
constexpr auto operator<=>(const T& a, const U& b) noexcept(noexcept(static_cast<std::common_type_t<T, U>>(a) <=> static_cast<std::common_type_t<T, U>>(b))) {
return static_cast<std::common_type_t<T, U>>(a) <=> static_cast<std::common_type_t<T, U>>(b);
}
}// namespace itype
#endif
}// namespace gsh
namespace gsh {
namespace internal {
template<class T> constexpr bool IsReferenceWrapper= false;
template<class U> constexpr bool IsReferenceWrapper<std::reference_wrapper<U>> = true;
// https://en.cppreference.com/w/cpp/utility/functional/invoke
template<class C, class Pointed, class Object, class... Args> GSH_INTERNAL_INLINE constexpr decltype(auto) InvokeMemPtr(Pointed C::*member, Object&& object, Args&&... args) {
using object_t= std::remove_cvref_t<Object>;
constexpr bool is_member_function= std::is_function_v<Pointed>;
constexpr bool is_wrapped= IsReferenceWrapper<object_t>;
constexpr bool is_derived_object= std::is_same_v<C, object_t> || std::is_base_of_v<C, object_t>;
if constexpr(is_member_function) {
if constexpr(is_derived_object) return (std::forward<Object>(object).*member)(std::forward<Args>(args)...);
else if constexpr(is_wrapped) return (object.get().*member)(std::forward<Args>(args)...);
else return ((*std::forward<Object>(object)).*member)(std::forward<Args>(args)...);
} else {
static_assert(std::is_object_v<Pointed> && sizeof...(args) == 0);
if constexpr(is_derived_object) return std::forward<Object>(object).*member;
else if constexpr(is_wrapped) return object.get().*member;
else return (*std::forward<Object>(object)).*member;
}
}
}// namespace internal
template<class F, class... Args> GSH_INTERNAL_INLINE constexpr std::invoke_result_t<F, Args...> Invoke(F&& f, Args&&... args) noexcept(std::is_nothrow_invocable_v<F, Args...>) {
if constexpr(std::is_member_function_pointer_v<std::remove_cvref_t<F>>) return internal::InvokeMemPtr(f, std::forward<Args>(args)...);
else return std::forward<F>(f)(std::forward<Args>(args)...);
}
namespace internal {
template<typename T, typename U> concept LessPtrCmp= requires(T&& t, U&& u) {
{ t < u } -> std::same_as<bool>;
} && std::convertible_to<T, const volatile void*> && std::convertible_to<U, const volatile void*> && (!requires(T&& t, U&& u) { operator<(std::forward<T>(t), std::forward<U>(u)); } && !requires(T&& t, U&& u) { std::forward<T>(t).operator<(std::forward<U>(u)); });
}// namespace internal
class Less {
public:
template<class T, class U>
requires std::totally_ordered_with<T, U>
GSH_INTERNAL_INLINE constexpr bool operator()(T&& t, U&& u) const noexcept(noexcept(std::declval<T>() < std::declval<U>())) {
if constexpr(internal::LessPtrCmp<T, U>) {
if(std::is_constant_evaluated()) return t < u;
auto x= reinterpret_cast<itype::u64>(static_cast<const volatile void*>(std::forward<T>(t)));
auto y= reinterpret_cast<itype::u64>(static_cast<const volatile void*>(std::forward<U>(u)));
return x < y;
} else return std::forward<T>(t) < std::forward<U>(u);
}
using is_transparent= void;
};
class Greater {
public:
template<class T, class U>
requires std::totally_ordered_with<T, U>
GSH_INTERNAL_INLINE constexpr bool operator()(T&& t, U&& u) const noexcept(noexcept(std::declval<U>() < std::declval<T>())) {
if constexpr(internal::LessPtrCmp<U, T>) {
if(std::is_constant_evaluated()) return u < t;
auto x= reinterpret_cast<itype::u64>(static_cast<const volatile void*>(std::forward<T>(t)));
auto y= reinterpret_cast<itype::u64>(static_cast<const volatile void*>(std::forward<U>(u)));
return y < x;
} else return std::forward<U>(u) < std::forward<T>(t);
}
using is_transparent= void;
};
class EqualTo {
public:
template<class T, class U>
requires std::equality_comparable_with<T, U>
GSH_INTERNAL_INLINE constexpr bool operator()(T&& t, U&& u) const noexcept(noexcept(std::declval<T>() == std::declval<U>())) {
return std::forward<T>(t) == std::forward<U>(u);
}
using is_transparent= void;
};
class Identity {
public:
template<class T> [[nodiscard]]
GSH_INTERNAL_INLINE constexpr T&& operator()(T&& t) const noexcept {
return std::forward<T>(t);
}
using is_transparent= void;
};
template<class F> class SwapArgs: public F {
public:
constexpr SwapArgs() noexcept(std::is_nothrow_default_constructible_v<F>): F() {}
constexpr SwapArgs(const F& f) noexcept(std::is_nothrow_copy_constructible_v<F>): F(f) {}
constexpr SwapArgs(F&& f) noexcept(std::is_nothrow_move_constructible_v<F>): F(std::move(f)) {}
constexpr SwapArgs& operator=(const F& f) noexcept(std::is_nothrow_copy_assignable_v<F>) {
F::operator=(f);
return *this;
}
constexpr SwapArgs& operator=(F&& f) noexcept(std::is_nothrow_move_assignable_v<F>) {
F::operator=(std::move(f));
return *this;
}
constexpr SwapArgs& operator=(const SwapArgs&) noexcept(std::is_nothrow_copy_assignable_v<F>)= default;
constexpr SwapArgs& operator=(SwapArgs&&) noexcept(std::is_nothrow_move_assignable_v<F>)= default;
template<class T, class U> GSH_INTERNAL_INLINE constexpr decltype(auto) operator()(T&& x, U&& y) noexcept(noexcept(F::operator()(std::declval<U>(), std::declval<T>()))) { return F::operator()(std::forward<U>(y), std::forward<T>(x)); }
template<class T, class U> GSH_INTERNAL_INLINE constexpr decltype(auto) operator()(T&& x, U&& y) const noexcept(noexcept(F::operator()(std::declval<U>(), std::declval<T>()))) { return F::operator()(std::forward<U>(y), std::forward<T>(x)); }
};
template<class F, class... G> class BindFront {
[[no_unique_address]] F func;
[[no_unique_address]] BindFront<G...> bind;
public:
constexpr BindFront() noexcept(std::is_nothrow_default_constructible_v<F> && noexcept(BindFront<G...>())): func(), bind() {}
template<class Arg, class... Args>
requires(sizeof...(Args) == sizeof...(G))
constexpr BindFront(Arg&& arg, Args&&... args) noexcept(std::is_nothrow_constructible_v<F, Arg> && noexcept(BindFront<G...>(std::forward<Args>(args)...))): func(std::forward<Arg>(arg)),
                                                                                                                                                            bind(std::forward<Args>(args)...) {}
template<class... Args> constexpr decltype(auto) operator()(Args&&... args) & noexcept(std::is_nothrow_invocable_v<F, Args...>) { return Invoke(bind, Invoke(func, std::forward<Args>(args)...)); }
template<class... Args> constexpr decltype(auto) operator()(Args&&... args) && noexcept(std::is_nothrow_invocable_v<F, Args...>) { return Invoke(std::move(bind), Invoke(std::move(func), std::forward<Args>(args)...)); }
template<class... Args> constexpr decltype(auto) operator()(Args&&... args) const& noexcept(std::is_nothrow_invocable_v<F, Args...>) { return Invoke(bind, Invoke(func, std::forward<Args>(args)...)); }
template<class... Args> constexpr decltype(auto) operator()(Args&&... args) const&& noexcept(std::is_nothrow_invocable_v<F, Args...>) { return Invoke(std::move(bind), Invoke(std::move(func), std::forward<Args>(args)...)); }
};
template<class F> class BindFront<F>: public F {
public:
constexpr BindFront() noexcept(std::is_nothrow_default_constructible_v<F>): F() {}
template<class... Args> constexpr BindFront(Args&&... args) noexcept(std::is_nothrow_constructible_v<F, Args...>): F(std::forward<Args>(args)...) {}
};
template<class T> class CustomizedHash;
namespace internal {
template<class T> concept Nocvref= std::same_as<T, std::remove_cv_t<T>> && !std::is_reference_v<T>;
constexpr itype::u64 MixIntegers(itype::u64 a, itype::u64 b) {
itype::u128 tmp= static_cast<itype::u128>(a) * b;
return static_cast<itype::u64>(tmp) ^ static_cast<itype::u64>(tmp >> 64);
}
constexpr itype::u64 HashBytes(const ctype::c8* ptr, itype::u32 len) noexcept {
constexpr itype::u64 m= 0xc6a4a7935bd1e995;
constexpr itype::u64 seed= 0xe17a1465;
constexpr itype::u32 r= 47;
itype::u64 h= seed ^ (len * m);
const itype::u32 n_blocks= len / 8;
for(itype::u64 i= 0; i < n_blocks; ++i) {
itype::u64 k;
const auto p= ptr + i * 8;
if(std::is_constant_evaluated()) {
k= 0;
for(itype::u32 j= 0; j != 8; ++j) k|= static_cast<itype::u64>(p[j]) << (8 * j);
} else {
for(int j= 0; j != 8; ++j) *(reinterpret_cast<ctype::c8*>(&k) + j)= *(p + j);
}
k*= m;
k^= k >> r;
k*= m;
h^= k;
h*= m;
}
const auto data8= ptr + n_blocks * 8;
switch(len & 7u) {
case 7: h^= static_cast<itype::u64>(data8[6]) << 48U; [[fallthrough]];
case 6: h^= static_cast<itype::u64>(data8[5]) << 40U; [[fallthrough]];
case 5: h^= static_cast<itype::u64>(data8[4]) << 32U; [[fallthrough]];
case 4: h^= static_cast<itype::u64>(data8[3]) << 24U; [[fallthrough]];
case 3: h^= static_cast<itype::u64>(data8[2]) << 16U; [[fallthrough]];
case 2: h^= static_cast<itype::u64>(data8[1]) << 8U; [[fallthrough]];
case 1:
h^= static_cast<itype::u64>(data8[0]);
h*= m;
[[fallthrough]];
default: break;
}
h^= h >> r;
return h;
}
constexpr itype::u64 HashBytes(const ctype::c8* ptr) noexcept {
auto last= ptr;
while(*last != '\0') ++last;
return HashBytes(ptr, last - ptr);
}
template<class T> concept StdHashCallable= requires(T x) {
{ std::hash<T>{}(x) } -> std::integral;
};
template<class T> concept CustomizedHashCallable= requires(T x) {
{ CustomizedHash<T>{}(x) } -> std::integral;
};
}// namespace internal
// https://raw.githubusercontent.com/martinus/unordered_dense/v1.3.0/include/ankerl/unordered_dense.h
class Hash {
public:
template<class T>
requires internal::CustomizedHashCallable<T>
constexpr itype::u64 operator()(const T& x) const {
return static_cast<itype::u64>(CustomizedHash<T>{}(x));
}
template<class T>
requires internal::CustomizedHashCallable<T>
constexpr itype::u64 operator()(const T& x, const CustomizedHash<T>& h) const {
return static_cast<itype::u64>(h(x));
}
template<class T>
requires(!internal::CustomizedHashCallable<T> && !std::is_volatile_v<T>)
constexpr itype::u64 operator()(const T& x) const {
if constexpr(std::same_as<T, std::nullptr_t>) return operator()(static_cast<void*>(x));
else if constexpr(std::is_pointer_v<T>) {
static_assert(sizeof(x) == 4 || sizeof(x) == 8);
if constexpr(sizeof(x) == 8) return operator()(std::bit_cast<itype::u64>(x));
else return operator()(std::bit_cast<itype::u32>(x));
} else if constexpr(std::same_as<T, itype::u64>) return internal::MixIntegers(x, 0x9e3779b97f4a7c15);
else if constexpr(std::same_as<T, itype::u128>) {
itype::u64 a= internal::MixIntegers(static_cast<itype::u64>(x), 0x9e3779b97f4a7c15);
itype::u64 b= internal::MixIntegers(static_cast<itype::u64>(x >> 64), 12638153115695167455ull);
return a ^ b;
} else if constexpr(std::integral<T>) {
static_assert(sizeof(T) <= 16);
if constexpr(sizeof(T) <= 8) return operator()(static_cast<itype::u64>(x));
else return operator()(static_cast<itype::u128>(x));
} else if constexpr(std::floating_point<T>) {
static_assert(sizeof(T) <= 16);
if constexpr(sizeof(T) == 2) return operator()(std::bit_cast<itype::u16>(x));
else if constexpr(sizeof(T) == 4) return operator()(std::bit_cast<itype::u32>(x));
else if constexpr(sizeof(T) == 8) return operator()(std::bit_cast<itype::u64>(x));
else if constexpr(sizeof(T) == 16) return operator()(std::bit_cast<itype::u128>(x));
else if constexpr(sizeof(T) < 8) {
struct a {
ctype::c8 b[sizeof(T)];
};
struct c {
a d;
ctype::c8 e[8 - sizeof(T)]{};
} f;
f.d= std::bit_cast<a>(x);
return operator()(std::bit_cast<itype::u64>(f));
} else {
struct a {
struct b {
ctype::c8 c[sizeof(T)];
} d;
ctype::c8 e[16 - sizeof(T)]{};
} f;
f.d= std::bit_cast<a::b>(x);
return operator()(std::bit_cast<itype::u128>(f));
}
} else if constexpr(internal::StdHashCallable<std::remove_cvref_t<T>>) return static_cast<itype::u64>(std::hash<std::remove_cvref_t<T>>{}(static_cast<std::remove_cvref_t<T>>(x)));
else {
static_assert((std::declval<T>(), false), "Cannot find the appropriate hash function.");
return 0ull;
}
}
using is_transparent= void;
};
class Plus {
public:
template<class T, class U> constexpr decltype(auto) operator()(T&& t, U&& u) const noexcept(noexcept(std::forward<T>(t) + std::forward<U>(u))) { return std::forward<T>(t) + std::forward<U>(u); }
using is_transparent= void;
};
class Negate {
public:
template<class T> constexpr decltype(auto) operator()(T&& t) const noexcept(noexcept(-std::forward<T>(t))) { return -std::forward<T>(t); }
using is_transparent= void;
};
}// namespace gsh
namespace gsh {
enum class RangeKind { Sized,
Unsized };
namespace internal {
template<class F, class T, class I, class U> concept IndirectlyBinaryLeftFoldableImpl= std::movable<T> && std::movable<U> && std::convertible_to<T, U> && std::invocable<F&, U, std::iter_reference_t<I>> && std::assignable_from<U&, std::invoke_result_t<F&, U, std::iter_reference_t<I>>>;
template<class F, class T, class I> concept IndirectlyBinaryLeftFoldable= std::copy_constructible<F> && std::indirectly_readable<I> && std::invocable<F&, T, std::iter_reference_t<I>> && std::convertible_to<std::invoke_result_t<F&, T, std::iter_reference_t<I>>, std::decay_t<std::invoke_result_t<F&, T, std::iter_reference_t<I>>>> && IndirectlyBinaryLeftFoldableImpl<F, T, I, std::decay_t<std::invoke_result_t<F&, T, std::iter_reference_t<I>>>>;
// These functions are defined in gsh/Algorithm.hpp
template<class R, class Comp, class Proj> constexpr auto MinImpl(R&& r, Comp&& comp, Proj&& proj);
template<class R, class Comp, class Proj> constexpr auto MaxImpl(R&& r, Comp&& comp, Proj&& proj);
template<class R, class T, class F> constexpr auto FoldImpl(R&& r, T init, F&& f);
template<class R, class F> constexpr auto SumImpl(const R& r, F&& f);
template<class R, class F> constexpr auto SumImpl(R&& r, F&& f);
template<class R> constexpr void ReverseImpl(R&& r);
template<class R, class Comp= Less, class Proj= Identity> constexpr void SortImpl(R&& r, Comp&& comp= {}, Proj&& proj= {});
template<class R, class Comp, class Proj> constexpr auto SortIndexImpl(R&& r, Comp&& comp, Proj&& proj);
}// namespace internal
template<class D, class V>
requires std::is_class_v<D> && std::same_as<D, std::remove_cv_t<D>>
class ViewInterface {
constexpr D& derived() { return *static_cast<D*>(this); }
constexpr const D& derived() const { return *static_cast<const D*>(this); }
constexpr auto get_begin() { return std::ranges::begin(derived()); }
constexpr auto get_begin() const { return std::ranges::begin(derived()); }
constexpr auto get_end() { return std::ranges::end(derived()); }
constexpr auto get_end() const { return std::ranges::end(derived()); }
constexpr auto get_rbegin() { return std::ranges::rbegin(derived()); }
constexpr auto get_rbegin() const { return std::ranges::rbegin(derived()); }
constexpr auto get_rend() { return std::ranges::rend(derived()); }
constexpr auto get_rend() const { return std::ranges::rend(derived()); }
using derived_type= D;
public:
using value_type= V;
constexpr derived_type copy() const& { return derived(); }
constexpr derived_type copy() & { return derived(); }
constexpr derived_type copy() && { return std::move(derived()); }
constexpr derived_type slice(itype::u32 start) const& { return derived_type(std::ranges::next(get_begin(), start), get_end()); }
constexpr derived_type slice(itype::u32 start, itype::u32 end) const& { return derived_type(std::ranges::next(get_begin(), start), std::ranges::next(get_begin(), end)); }
constexpr derived_type slice(itype::u32 start) const&&
requires(!std::ranges::borrowed_range<derived_type>)
{
return derived_type(std::move_iterator(std::ranges::next(get_begin(), start)), std::move_sentinel(get_end()));
}
constexpr derived_type slice(itype::u32 start, itype::u32 end) const&&
requires(!std::ranges::borrowed_range<derived_type>)
{
return derived_type(std::move_iterator(std::ranges::next(get_begin(), start)), std::move_iterator(std::ranges::next(get_begin(), end)));
}
template<class Proj= Identity, std::indirect_unary_predicate<std::projected<std::ranges::iterator_t<derived_type>, Proj>> Pred>
requires std::ranges::input_range<derived_type> && std::ranges::view<derived_type> && std::is_object_v<Proj>
[[nodiscard]] constexpr derived_type filter(Pred&& pred= {}, Proj&& proj= {}) const {
if constexpr(std::same_as<Proj, Identity>) {
auto v= derived() | std::views::filter(std::forward<Pred>(pred));
return derived_type(v.begin(), v.end());
} else {
auto v= derived() | std::views::transform(std::forward<Proj>(proj)) | std::views::filter(std::forward<Pred>(pred));
return derived_type(v.begin(), v.end());
}
}
template<class Proj= Identity, std::indirect_unary_predicate<std::projected<std::ranges::iterator_t<derived_type>, Proj>> Pred>
requires std::ranges::forward_range<derived_type> && std::permutable<std::ranges::iterator_t<derived_type>>
constexpr auto erase_if(Pred&& pred= {}, Proj&& proj= {}) {
auto [itr, sent]= std::ranges::remove_if(get_begin(), get_end(), std::forward<Pred>(pred), std::forward<Proj>(proj));
auto len= std::ranges::distance(itr, sent);
derived().erase(itr, sent);
return len;
}
template<std::invocable<value_type> Proj>
requires std::same_as<value_type, std::invoke_result_t<Proj, value_type>>
[[nodiscard]] constexpr derived_type transform(Proj&& proj= {}) const {
auto v= derived() | std::views::transform(std::forward<Proj>(proj));
return derived_type(v.begin(), v.end());
}
template<std::invocable<value_type> Proj>
requires std::same_as<value_type, std::invoke_result_t<Proj, value_type>>
constexpr void apply(Proj&& proj= {}) {
for(auto& x: derived()) x= Invoke(proj, std::move(x));
}
template<std::predicate<value_type> Pred> constexpr bool all_of(Pred f) const {
for(const auto& el: derived())
if(!f(el)) return false;
return true;
}
constexpr bool all_of(const value_type& x) const {
for(const auto& el: derived())
if(!(el == x)) return false;
return true;
}
template<std::predicate<value_type> Pred> constexpr bool any_of(Pred f) const {
for(const auto& el: derived())
if(f(el)) return true;
return false;
}
constexpr bool any_of(const value_type& x) const {
for(const auto& el: derived())
if(el == x) return true;
return false;
}
template<std::predicate<value_type> Pred> constexpr bool none_of(Pred f) const {
for(const auto& el: derived())
if(f(el)) return false;
return true;
}
constexpr bool none_of(const value_type& x) const {
for(const auto& el: derived())
if(el == x) return false;
return true;
}
constexpr bool contains(const value_type& x) const {
for(const auto& el: derived())
if(el == x) return true;
return false;
}
constexpr auto find(const value_type& x) const {
const auto end= get_end();
for(auto itr= get_begin(); itr != end; ++itr)
if(*itr == x) return itr;
return end;
}
constexpr itype::u32 count(const value_type& x) const {
itype::u32 res= 0;
for(const auto& el: derived()) res+= (el == x);
return res;
}
template<class Proj= Identity, std::indirect_strict_weak_order<std::projected<std::ranges::iterator_t<derived_type>, Proj>> Comp= Less>
requires std::ranges::forward_range<derived_type> && std::indirectly_copyable_storable<std::ranges::iterator_t<derived_type>, value_type*>
constexpr auto min(Comp&& comp= {}, Proj&& proj= {}) const {
return internal::MinImpl(derived(), std::forward<Comp>(comp), std::forward<Proj>(proj));
}
template<class Proj= Identity, std::indirect_strict_weak_order<std::projected<std::ranges::iterator_t<derived_type>, Proj>> Comp= Less>
requires std::ranges::forward_range<derived_type> && std::indirectly_copyable_storable<std::ranges::iterator_t<derived_type>, value_type*>
constexpr auto max(Comp&& comp= {}, Proj&& proj= {}) const {
return internal::MaxImpl(derived(), std::forward<Comp>(comp), std::forward<Proj>(proj));
}
template<class T= value_type, internal::IndirectlyBinaryLeftFoldable<T, std::ranges::iterator_t<derived_type>> F= Plus>
requires std::ranges::forward_range<derived_type>
constexpr auto fold(T&& init= {}, F&& f= {}) const {
return internal::FoldImpl(derived(), std::forward<T>(init), std::forward<F>(f));
}
template<internal::IndirectlyBinaryLeftFoldable<value_type, std::ranges::iterator_t<derived_type>> F= Plus>
requires std::ranges::forward_range<derived_type>
constexpr auto sum(F&& f= {}) const {
return internal::SumImpl(derived(), std::forward<F>(f));
}
void reverse()
requires std::ranges::bidirectional_range<derived_type> && std::permutable<std::ranges::iterator_t<derived_type>>
{
internal::ReverseImpl(derived());
}
[[nodiscard]] auto reversed() const
requires std::ranges::bidirectional_range<derived_type> && std::permutable<std::ranges::iterator_t<derived_type>>
{
auto res= copy();
internal::ReverseImpl(res);
return res;
}
template<class Comp= Less, class Proj= Identity>
requires std::ranges::forward_range<derived_type> && std::sortable<std::ranges::iterator_t<derived_type>, Comp, Proj>
constexpr void sort(Comp&& comp= {}, Proj&& proj= {}) {
internal::SortImpl(derived(), std::forward<Comp>(comp), std::forward<Proj>(proj));
}
template<class Comp= Less, class Proj= Identity>
requires std::ranges::forward_range<derived_type> && std::sortable<std::ranges::iterator_t<derived_type>, Comp, Proj>
[[nodiscard]] constexpr auto sorted(Comp&& comp= {}, Proj&& proj= {}) const {
auto res= copy();
internal::SortImpl(res, std::forward<Comp>(comp), std::forward<Proj>(proj));
return res;
}
template<class Comp= Less, class Proj= Identity>
requires std::ranges::random_access_range<derived_type> && std::sortable<std::ranges::iterator_t<derived_type>, Comp, Proj>
constexpr auto sort_index(Comp&& comp= {}, Proj&& proj= {}) const {
return internal::SortIndexImpl(derived(), std::forward<Comp>(comp), std::forward<Proj>(proj));
}
};
namespace internal {
template<class T, class U> concept difference_from= std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;
template<class From, class To> concept convertible_to_non_slicing= std::convertible_to<From, To> && !(std::is_pointer_v<std::decay_t<From>> && std::is_pointer_v<std::decay_t<To>> && !std::convertible_to<std::remove_pointer_t<std::decay_t<From>> (*)[], std::remove_pointer_t<std::decay_t<To>> (*)[]>);
template<class T> concept pair_like= /* tuple-like<T> && */ std::tuple_size_v<std::remove_cvref_t<T>> == 2;
template<class T, class U, class V> concept pair_like_convertible_from= !std::ranges::range<T> && !std::is_reference_v<T> && pair_like<T> && std::constructible_from<T, U, V> && convertible_to_non_slicing<U, std::tuple_element_t<0, T>> && std::convertible_to<V, std::tuple_element_t<1, T>>;
}// namespace internal
template<std::input_or_output_iterator I, std::sentinel_for<I> S= I, RangeKind K= std::sized_sentinel_for<S, I> ? RangeKind::Sized : RangeKind::Unsized>
requires(K == RangeKind::Sized || !std::sized_sentinel_for<S, I>)
class Subrange: public ViewInterface<Subrange<I, S, K>, std::iter_value_t<I>> {
I itr;
S sent;
static constexpr bool StoreSize= (K == RangeKind::Sized && !std::sized_sentinel_for<S, I>);
struct empty_sz {};
[[no_unique_address]] std::conditional_t<StoreSize, std::make_unsigned_t<std::iter_difference_t<I>>, empty_sz> sz;
public:
constexpr Subrange()= default;
constexpr Subrange(internal::convertible_to_non_slicing<I> auto i, S s)
requires(!StoreSize)
: itr(i),
  sent(s) {}
constexpr Subrange(internal::convertible_to_non_slicing<I> auto i, S s, std::make_unsigned_t<std::iter_difference_t<I>> n)
requires(K == RangeKind::Sized)
: itr(i),
  sent(s) {
if constexpr(StoreSize) sz= n;
}
template<internal::difference_from<Subrange> R>
requires std::ranges::borrowed_range<R> && internal::convertible_to_non_slicing<std::ranges::iterator_t<R>, I>
&& std::convertible_to<std::ranges::sentinel_t<R>, S>
constexpr Subrange(R&& r)
requires(!StoreSize || std::ranges::sized_range<R>)
: itr(std::ranges::begin(r)),
  sent(std::ranges::end(r)) {}
template<std::ranges::borrowed_range R>
requires internal::convertible_to_non_slicing<std::ranges::iterator_t<R>, I>
&& std::convertible_to<std::ranges::sentinel_t<R>, S>
constexpr Subrange(R&& r, std::make_unsigned_t<std::iter_difference_t<I>> n)
requires(K == RangeKind::Sized)
: Subrange{std::ranges::begin(r), std::ranges::end(r), n} {}
template<internal::difference_from<Subrange> PairLike>
requires internal::pair_like_convertible_from<PairLike, const I&, const S&>
constexpr operator PairLike() const {
return PairLike(itr, sent);
}
constexpr I begin() const
requires std::copyable<I>
{
return itr;
}
[[nodiscard]] constexpr I begin()
requires(!std::copyable<I>)
{
return std::move(itr);
}
constexpr S end() const { return sent; }
constexpr bool empty() const { return itr == sent; }
constexpr I data() const
requires(std::is_pointer_v<I> && std::copyable<I>)
{
return itr;
}
constexpr I data() const
requires(std::is_pointer_v<I> && !std::copyable<I>)
{
return std::move(itr);
}
[[nodiscard]] constexpr Subrange next(std::iter_difference_t<I> n= 1) const&
requires std::forward_iterator<I>
{
auto tmp= *this;
tmp.advance(n);
return tmp;
}
[[nodiscard]] constexpr Subrange next(std::iter_difference_t<I> n= 1) && {
advance(n);
return std::move(*this);
}
[[nodiscard]] constexpr Subrange prev(std::iter_difference_t<I> n= 1) const
requires std::bidirectional_iterator<I>
{
auto tmp= *this;
tmp.advance(-n);
return tmp;
}
constexpr Subrange& advance(std::iter_difference_t<I> n) {
if constexpr(StoreSize) {
auto d= n - std::ranges::advance(itr, n, sent);
if(d >= 0) sz-= static_cast<std::make_unsigned_t<std::remove_cvref_t<decltype(d)>>>(d);
else sz+= static_cast<std::make_unsigned_t<std::remove_cvref_t<decltype(d)>>>(d);
return *this;
} else {
std::ranges::advance(itr, n, sent);
return *this;
}
}
};
template<std::input_or_output_iterator I, std::sentinel_for<I> S> Subrange(I, S) -> Subrange<I, S>;
template<std::input_or_output_iterator I, std::sentinel_for<I> S> Subrange(I, S, std::make_unsigned_t<std::iter_difference_t<I>>) -> Subrange<I, S, RangeKind::Sized>;
template<std::ranges::borrowed_range R> Subrange(R&&) -> Subrange<std::ranges::iterator_t<R>, std::ranges::sentinel_t<R>, (std::ranges::sized_range<R> || std::sized_sentinel_for<std::ranges::sentinel_t<R>, std::ranges::iterator_t<R>>) ? RangeKind::Sized : RangeKind::Unsized>;
template<std::ranges::borrowed_range R> Subrange(R&&, std::make_unsigned_t<std::ranges::range_difference_t<R>>) -> Subrange<std::ranges::iterator_t<R>, std::ranges::sentinel_t<R>, RangeKind::Sized>;
}// namespace gsh
namespace std::ranges {
template<class I, class S, gsh::RangeKind K> constexpr bool enable_borrowed_range<gsh::Subrange<I, S, K>> = true;
}
namespace gsh {
namespace internal {
template<class T, class U> struct GetPtr {
using type= U*;
};
template<class T, class U>
requires requires { typename T::pointer; }
struct GetPtr<T, U> {
using type= typename T::pointer;
};
template<class T, class U> struct RepFirst {};
template<template<class, class...> class SomeTemplate, class U, class T, class... Types> struct RepFirst<SomeTemplate<T, Types...>, U> {
using type= SomeTemplate<U, Types...>;
};
template<class T, class U> struct Rebind {
using type= typename RepFirst<T, U>::type;
};
template<class T, class U>
requires requires { typename T::template rebind<U>; }
struct Rebind<T, U> {
using type= typename T::template rebind<U>;
};
template<class T, class U> struct GetRebindPtr {
using type= typename Rebind<T, U>::type;
};
template<class T, class U> struct GetRebindPtr<T*, U> {
using type= U;
};
template<class T, class U, class V> struct GetConstPtr {
using type= typename GetRebindPtr<U, const V*>::type;
};
template<class T, class U, class V>
requires requires { typename T::const_pointer; }
struct GetConstPtr<T, U, V> {
using type= typename T::const_pointer;
};
template<class T, class U> struct GetVoidPtr {
using type= typename GetRebindPtr<U, void*>::type;
};
template<class T, class U>
requires requires { typename T::void_pointer; }
struct GetVoidPtr<T, U> {
using type= typename T::void_pointer;
};
template<class T, class U> struct GetConstVoidPtr {
using type= typename GetRebindPtr<U, const void*>::type;
};
template<class T, class U>
requires requires { typename T::const_void_pointer; }
struct GetConstVoidPtr<T, U> {
using type= typename T::const_void_pointer;
};
template<class T> struct GetDifferenceTypeSub {
using type= itype::i32;
};
template<class T>
requires requires { typename T::difference_type; }
struct GetDifferenceTypeSub<T> {
using type= typename T::difference_type;
};
template<class T, class U> struct GetDifferenceType {
using type= typename GetDifferenceTypeSub<U>::type;
};
template<class T, class U>
requires requires { typename T::difference_type; }
struct GetDifferenceType<T, U> {
using type= typename T::difference_type;
};
template<class T, class U> struct GetSizeType {
using type= std::make_unsigned_t<U>;
};
template<class T, class U>
requires requires { typename T::size_type; }
struct GetSizeType<T, U> {
using type= typename T::size_type;
};
template<class T> struct IsPropCopy {
using type= std::false_type;
};
template<class T>
requires requires { typename T::propagate_on_container_copy_assignment; }
struct IsPropCopy<T> {
using type= typename T::propagate_on_container_copy_assignment;
};
template<class T> struct IsPropMove {
using type= std::false_type;
};
template<class T>
requires requires { typename T::propagate_on_container_move_assignment; }
struct IsPropMove<T> {
using type= typename T::propagate_on_container_move_assignment;
};
template<class T> struct IsPropSwap {
using type= std::false_type;
};
template<class T>
requires requires { typename T::propagate_on_container_swap; }
struct IsPropSwap<T> {
using type= typename T::propagate_on_container_swap;
};
template<class T> struct IsAlwaysEqual {
using type= typename std::is_empty<T>::type;
};
template<class T>
requires requires { typename T::is_always_equal; }
struct IsAlwaysEqual<T> {
using type= typename T::is_always_equal;
};
template<class T, class U> struct RebindAlloc {
using type= typename internal::RepFirst<T, U>::type;
};
template<class T, class U>
requires requires { typename T::template rebind<U>::other; }
struct RebindAlloc<T, U> {
using type= typename T::template rebind<U>::other;
};
}// namespace internal
template<class Alloc> class AllocatorTraits {
public:
using allocator_type= Alloc;
using value_type= typename Alloc::value_type;
using pointer= typename internal::GetPtr<Alloc, value_type>::type;
using const_pointer= typename internal::GetConstPtr<Alloc, pointer, value_type>::type;
using void_pointer= typename internal::GetVoidPtr<Alloc, pointer>::type;
using const_void_pointer= typename internal::GetConstVoidPtr<Alloc, pointer>::type;
using difference_type= typename internal::GetDifferenceType<Alloc, pointer>::type;
using size_type= typename internal::GetSizeType<Alloc, difference_type>::type;
using propagate_on_container_copy_assignment= typename internal::IsPropCopy<Alloc>::type;
using propagate_on_container_move_assignment= typename internal::IsPropMove<Alloc>::type;
using propagate_on_container_swap= typename internal::IsPropSwap<Alloc>::type;
using is_always_equal= typename internal::IsAlwaysEqual<Alloc>::type;
template<class U> using rebind_alloc= typename internal::RebindAlloc<Alloc, U>::type;
template<class U> using rebind_traits= AllocatorTraits<typename internal::RebindAlloc<Alloc, U>::type>;
private:
constexpr static bool with_hint= requires(Alloc& a, size_type n, const_void_pointer hint) { a.allocate(n, hint); };
constexpr static bool aligned_with_hint= requires(Alloc& a, size_type n, std::align_val_t align, const_void_pointer hint) { a.allocate(n, align, hint); };
template<class... Args> constexpr static bool constructible= requires(Alloc& a, pointer p, Args&&... args) { a.construct(p, std::forward<Args>(args)...); };
constexpr static bool destructible= requires(Alloc& a, pointer p) { a.destroy(p); };
constexpr static bool selectable= requires(Alloc& a) { a.select_on_container_copy_construction(); };
public:
[[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n) noexcept(noexcept(a.allocate(n))) { return a.allocate(n); }
[[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, std::align_val_t align) noexcept(noexcept(a.allocate(n, align))) { return a.allocate(n, align); }
[[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, const_void_pointer hint) noexcept(noexcept(a.allocate(n, hint)))
requires(with_hint)
{
return a.allocate(n, hint);
}
[[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, const_void_pointer) noexcept(noexcept(a.allocate(n)))
requires(!with_hint)
{
return a.allocate(n);
}
[[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, std::align_val_t align, const_void_pointer hint) noexcept(noexcept(a.allocate(n, align, hint)))
requires(aligned_with_hint)
{
return a.allocate(n, align, hint);
}
[[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, std::align_val_t align, const_void_pointer) noexcept(noexcept(a.allocate(align, n)))
requires(!aligned_with_hint)
{
return a.allocate(align, n);
}
static constexpr void deallocate(Alloc& a, pointer p, size_type n) noexcept(noexcept(a.deallocate(p, n))) { a.deallocate(p, n); }
static constexpr void deallocate(Alloc& a, pointer p, size_type n, std::align_val_t align) noexcept(noexcept(a.deallocate(p, n, align))) { a.deallocate(p, n, align); }
static constexpr size_type max_size(const Alloc& a) noexcept {
if constexpr(requires { a.max_size(); }) return a.max_size();
else return std::numeric_limits<size_type>::max() / sizeof(value_type);
}
template<class T, class... Args> static constexpr void construct(Alloc& a, T* p, Args&&... args) noexcept(noexcept(a.construct(p, std::forward<Args>(args)...)))
requires(constructible<Args...>)
{
a.construct(p, std::forward<Args>(args)...);
}
template<class T, class... Args> static constexpr void construct(Alloc&, T* p, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
requires(!constructible<Args...>)
{
std::construct_at(p, std::forward<Args>(args)...);
}
template<class T> static constexpr void destroy(Alloc& a, T* p) noexcept(noexcept(a.destroy(p)))
requires(destructible)
{
return a.destroy(p);
}
template<class T> static constexpr void destroy(Alloc&, T* p) noexcept(std::is_nothrow_destructible_v<T>)
requires(!destructible)
{
return std::destroy_at(p);
}
static constexpr Alloc select_on_container_copy_construction(const Alloc& a) noexcept(noexcept(a.select_on_container_copy_construction()))
requires(selectable)
{
return a.select_on_container_copy_construction();
}
static constexpr Alloc select_on_container_copy_construction(const Alloc& a) noexcept(std::is_nothrow_copy_constructible_v<Alloc>)
requires(!selectable)
{
return a;
}
};
template<class T> class Allocator {
public:
using value_type= T;
using propagate_on_container_move_assignment= std::true_type;
using size_type= itype::u32;
using difference_type= itype::i32;
using is_always_equal= std::true_type;
constexpr Allocator() noexcept {}
constexpr Allocator(const Allocator&) noexcept {}
template<class U> constexpr Allocator(const Allocator<U>&) noexcept {}
[[nodiscard]] constexpr T* allocate(size_type n) {
if(std::is_constant_evaluated()) return std::allocator<T>().allocate(n);
if constexpr(alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) return static_cast<T*>(::operator new(sizeof(T) * n, static_cast<std::align_val_t>(alignof(T))));
else return static_cast<T*>(::operator new(sizeof(T) * n));
}
[[nodiscard]] T* allocate(size_type n, std::align_val_t align) { return static_cast<T*>(::operator new(sizeof(T) * n, align)); }
constexpr void deallocate(T* p, [[maybe_unused]] size_type n) noexcept {
if(std::is_constant_evaluated()) return std::allocator<T>().deallocate(p, n);
#ifdef __cpp_sized_deallocation
if constexpr(alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) ::operator delete(p, n, static_cast<std::align_val_t>(alignof(T)));
else ::operator delete(p, n);
#else
if constexpr(alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) ::operator delete(p, static_cast<std::align_val_t>(alignof(T)));
else ::operator delete(p);
#endif
}
void deallocate(T* p, [[maybe_unused]] size_type n, std::align_val_t align) noexcept {
#ifdef __cpp_sized_deallocation
::operator delete(p, n, align);
#else
::operator delete(p, align);
#endif
}
constexpr Allocator& operator=(const Allocator&)= default;
template<class U> friend constexpr bool operator==(const Allocator&, const Allocator<U>&) noexcept { return true; }
};
template<itype::u32 Size> class MemoryPool {
template<class T> friend class PoolAllocator;
itype::u32 cnt= 0;
itype::u32 ref= 0;
ctype::c8 buf[Size];
public:
constexpr ~MemoryPool() noexcept(false) {
if(ref != 0) throw Exception("gsh::MemoryPool::~MemoryPool / There are some gsh::PoolAllocator tied to this object have not yet been destroyed.");
}
};
template<class T> class PoolAllocator {
template<class U> friend class PoolAllocator;
itype::u32* cnt;
itype::u32* ref;
ctype::c8 *buf, *end;
public:
using value_type= T;
using propagate_on_container_copy_assignmant= std::true_type;
using propagate_on_container_move_assignment= std::true_type;
using propagate_on_container_swap= std::true_type;
using size_type= itype::u32;
using difference_type= itype::i32;
using is_always_equal= std::false_type;
constexpr PoolAllocator() noexcept: cnt(nullptr), ref(nullptr), buf(nullptr), end(nullptr) {}
constexpr PoolAllocator(const PoolAllocator& a) noexcept: cnt(a.cnt), ref(a.ref), buf(a.buf), end(a.end) { ++*ref; }
template<class U> constexpr PoolAllocator(const PoolAllocator<U>& a) noexcept: cnt(a.cnt), ref(a.ref), buf(a.buf), end(a.end) { ++*ref; }
template<itype::u32 Size> constexpr PoolAllocator(MemoryPool<Size>& p) noexcept: cnt(&p.cnt), ref(&p.ref), buf(p.buf), end(p.buf + Size) { ++*ref; }
constexpr ~PoolAllocator() noexcept {
if(ref != nullptr) --*ref;
}
[[nodiscard]] constexpr T* allocate(size_type n) {
if(std::is_constant_evaluated()) return Allocator<T>().allocate(n);
constexpr itype::u32 align= __STDCPP_DEFAULT_NEW_ALIGNMENT__ < alignof(T) ? alignof(T) : __STDCPP_DEFAULT_NEW_ALIGNMENT__;
void* ptr= static_cast<void*>(buf + *cnt);
std::size_t space= end - static_cast<ctype::c8*>(ptr);
std::align(align, sizeof(T) * n, ptr, space);
if(ptr == nullptr) throw Exception("gsh::PoolAllocator::allocate / Failed to allocate memory.");
T* res= static_cast<T*>(ptr);
*cnt= static_cast<ctype::c8*>(ptr) - buf;
return res;
}
[[nodiscard]] T* allocate(size_type n, std::align_val_t align) {
void* ptr= static_cast<void*>(buf + *cnt);
std::size_t space= end - static_cast<ctype::c8*>(ptr);
std::align(static_cast<std::size_t>(align), sizeof(T) * n, ptr, space);
if(ptr == nullptr) throw Exception("gsh::PoolAllocator::allocate / Failed to allocate memory.");
T* res= static_cast<T*>(ptr);
*cnt= static_cast<ctype::c8*>(ptr) - buf;
return res;
}
constexpr void deallocate(T* p, [[maybe_unused]] size_type n) noexcept {
if(std::is_constant_evaluated()) return Allocator<T>().deallocate(p, n);
}
void deallocate(T*, size_type, std::align_val_t) noexcept {}
constexpr PoolAllocator& operator=(const PoolAllocator& a) noexcept {
if(ref != nullptr) --*ref;
cnt= a.cnt, ref= a.ref, buf= a.buf, end= a.end;
++*ref;
return *this;
}
template<itype::u32 Size> constexpr PoolAllocator& operator=(MemoryPool<Size>& p) noexcept {
if(ref != nullptr) --*ref;
cnt= &p.cnt, ref= &p.ref, buf= p.buf, end= p.buf + Size;
++*ref;
return *this;
}
template<class U> friend constexpr bool operator==(const PoolAllocator& a, const PoolAllocator<U>& b) noexcept { return a.cnt == b.cnt && a.ref == b.ref && a.buf == b.buf && a.end == b.end; }
};
template<class T> class SingleAllocator {
T* buffer[24]= {};
itype::u32 x= 0xffffffff, y= 0;
T** del= nullptr;
itype::u32 end= 0;
[[no_unique_address]] Allocator<T> alloc;
[[no_unique_address]] Allocator<T*> del_alloc;
using traits= AllocatorTraits<Allocator<T>>;
using del_alloc_traits= AllocatorTraits<Allocator<T*>>;
public:
using value_type= T;
using propagate_on_container_copy_assignmant= std::false_type;
using propagate_on_container_move_assignment= std::false_type;
using propagate_on_container_swap= std::false_type;
using size_type= itype::u32;
using difference_type= itype::i32;
using is_always_equal= std::false_type;
constexpr SingleAllocator() noexcept {}
constexpr SingleAllocator(const SingleAllocator&) noexcept {}
template<class U> constexpr SingleAllocator(const SingleAllocator<U>&) noexcept {}
constexpr ~SingleAllocator() noexcept {
for(itype::u32 i= 0; i != x + 1; ++i) traits::deallocate(alloc, buffer[i], 1 << i);
if(del != nullptr) del_alloc_traits::deallocate(del_alloc, del, (1u << (x + 1)) - 1);
}
constexpr SingleAllocator& operator=(const SingleAllocator&) noexcept {}
constexpr T* allocate(itype::u32) {
if(y == (1u << (x + 1)) >> 1) {
if(end != 0) [[likely]] {
return del[--end];
} else {
x+= 1, y= 0;
buffer[x]= traits::allocate(alloc, 1 << x);
T** new_del= del_alloc_traits::allocate(del_alloc, (1 << (x + 1)) - 1);
if(del != nullptr) [[likely]] {
for(itype::u32 i= 0; i != end; ++i) new_del[i]= del[i];
del_alloc_traits::deallocate(del_alloc, del, (1 << x) - 1);
}
del= new_del;
}
}
return &buffer[x][y++];
}
constexpr void deallocate(T* p, itype::u32) noexcept { del[end++]= p; }
constexpr itype::u32 max_size() const noexcept { return (1 << 24) - 1; }
constexpr SingleAllocator select_on_container_copy_construction() const noexcept { return {}; }
template<class U> friend constexpr bool operator==(const SingleAllocator&, const SingleAllocator<U>&) noexcept { return false; }
};
template<class Alloc> class SharedAllocator {
static inline Alloc alloc;
using traits= AllocatorTraits<Alloc>;
public:
using value_type= typename traits::value_type;
using propagate_on_container_copy_assignmant= std::true_type;
using propagate_on_container_move_assignment= std::true_type;
using propagate_on_container_swap= std::true_type;
using size_type= typename traits::size_type;
using difference_type= typename traits::difference_type;
using is_always_equal= std::true_type;
using allocator_type= Alloc;
template<class U> class rebind {
public:
~rebind()= delete;
using other= SharedAllocator<typename traits::template rebind_alloc<U>>;
};
constexpr SharedAllocator() noexcept {}
constexpr SharedAllocator(const SharedAllocator&) noexcept= default;
template<class T> constexpr SharedAllocator(const SharedAllocator<T>&) noexcept {}
constexpr SharedAllocator& operator=(const SharedAllocator&) noexcept= default;
template<class... Args> auto allocate(Args&&... args) noexcept(noexcept(alloc.allocate(std::forward<Args>(args)...))) { return alloc.allocate(std::forward<Args>(args)...); }
template<class... Args> void deallocate(Args&&... args) noexcept(noexcept(alloc.deallocate(std::forward<Args>(args)...))) { return alloc.deallocate(std::forward<Args>(args)...); }
size_type max_size() const noexcept { return alloc.max_size(); }
static Alloc& get_allocator() noexcept { return alloc; }
template<class T> friend constexpr bool operator==(const SharedAllocator&, const SharedAllocator<T>&) noexcept { return true; }
};
template<class Alloc> class ConstexprAllocator {
[[no_unique_address]] Alloc alloc;
using traits= AllocatorTraits<Alloc>;
public:
using allocator_type= Alloc;
using value_type= typename traits::value_type;
using pointer= typename traits::pointer;
using const_pointer= typename traits::const_pointer;
using void_pointer= typename traits::void_pointer;
using const_void_pointer= typename traits::const_void_pointer;
using difference_type= typename traits::difference_type;
using size_type= typename traits::size_type;
using propagate_on_container_copy_assignment= typename traits::propagate_on_container_copy_assignment;
using propagate_on_container_move_assignment= typename traits::propagate_on_container_move_assignment;
using propagate_on_container_swap= typename traits::propagate_on_container_swap;
using is_always_equal= typename traits::is_always_equal;
template<class U> class rebind {
public:
~rebind()= delete;
using other= ConstexprAllocator<typename traits::template rebind_alloc<U>>;
};
constexpr ConstexprAllocator() noexcept(noexcept(Alloc())) {}
constexpr ConstexprAllocator(const ConstexprAllocator&) noexcept(std::is_nothrow_copy_constructible_v<Alloc>)= default;
constexpr ConstexprAllocator(ConstexprAllocator&&) noexcept(std::is_nothrow_move_constructible_v<Alloc>)= default;
template<class U> constexpr ConstexprAllocator(const ConstexprAllocator<U>& a) noexcept(std::is_nothrow_constructible_v<Alloc, const U&>): alloc(a.alloc) {}
template<class U> constexpr ConstexprAllocator(ConstexprAllocator<U>&& a) noexcept(std::is_nothrow_constructible_v<Alloc, U&&>): alloc(std::move(a.alloc)) {}
constexpr ConstexprAllocator& operator=(const ConstexprAllocator& a) noexcept(std::is_nothrow_copy_assignable_v<Alloc>) {
alloc= a.alloc;
return *this;
}
constexpr ConstexprAllocator& operator=(ConstexprAllocator&& a) noexcept(std::is_nothrow_move_assignable_v<Alloc>) {
alloc= std::move(a.alloc);
return *this;
}
template<class... Args> constexpr auto allocate(Args&&... args) noexcept(noexcept(alloc.allocate(std::forward<Args>(args)...))) {
if(std::is_constant_evaluated()) return Allocator<value_type>().allocate(std::forward<Args>(args)...);
else return alloc.allocate(std::forward<Args>(args)...);
}
template<class... Args> constexpr void deallocate(Args&&... args) noexcept(noexcept(alloc.deallocate(std::forward<Args>(args)...))) {
if(std::is_constant_evaluated()) return Allocator<value_type>().deallocate(std::forward<Args>(args)...);
else return alloc.deallocate(std::forward<Args>(args)...);
}
constexpr size_type max_size() const noexcept {
if(std::is_constant_evaluated()) return AllocatorTraits<Allocator<value_type>>::max_size();
else return alloc.max_size();
}
constexpr Alloc& get_allocator() noexcept { return alloc; }
template<class U> friend constexpr bool operator==(const ConstexprAllocator& a, const ConstexprAllocator<U>& b) noexcept(noexcept(a.alloc == b.alloc)) { return a.alloc == b.alloc; }
};
}// namespace gsh
namespace gsh {
template<class T>
requires std::same_as<T, std::remove_cv_t<T>>
class ArrInitTag {};
template<class T= void> constexpr ArrInitTag<T> ArrInit;
class ArrNoInitTag {};
constexpr ArrNoInitTag ArrNoInit;
template<class T, class Allocator= Allocator<T>>
requires std::same_as<T, typename AllocatorTraits<Allocator>::value_type> && std::same_as<T, std::remove_cv_t<T>>
class Arr: public ViewInterface<Arr<T, Allocator>, T> {
using traits= AllocatorTraits<Allocator>;
public:
using reference= T&;
using const_reference= const T&;
using iterator= T*;
using const_iterator= const T*;
using size_type= itype::u32;
using difference_type= itype::i32;
using value_type= T;
using allocator_type= Allocator;
using pointer= typename traits::pointer;
using const_pointer= typename traits::const_pointer;
using reverse_iterator= std::reverse_iterator<iterator>;
using const_reverse_iterator= std::reverse_iterator<const_iterator>;
private:
[[no_unique_address]] allocator_type alloc;
pointer ptr= nullptr;
size_type len= 0;
public:
constexpr Arr() noexcept(noexcept(Allocator())): Arr(Allocator()) {}
constexpr explicit Arr(const allocator_type& a) noexcept: alloc(a) {}
constexpr explicit Arr(size_type n, const allocator_type& a= Allocator()): alloc(a) {
if(n == 0) [[unlikely]]
return;
ptr= traits::allocate(alloc, n);
len= n;
for(size_type i= 0; i != n; ++i) traits::construct(alloc, ptr + i);
}
constexpr explicit Arr(ArrNoInitTag, size_type n, const allocator_type& a= Allocator()): alloc(a) {
if(n == 0) [[unlikely]]
return;
ptr= traits::allocate(alloc, n);
len= n;
}
constexpr explicit Arr(const size_type n, const value_type& value, const allocator_type& a= Allocator()): alloc(a) {
if(n == 0) [[unlikely]]
return;
ptr= traits::allocate(alloc, n);
len= n;
for(size_type i= 0; i != n; ++i) traits::construct(alloc, ptr + i, value);
}
template<std::input_iterator InputIter> constexpr Arr(InputIter first, InputIter last, const allocator_type& a= Allocator()): alloc(a) {
const size_type n= std::distance(first, last);
if(n == 0) [[unlikely]]
return;
ptr= traits::allocate(alloc, n);
len= n;
size_type i= 0;
for(InputIter itr= first; i != n; ++itr, ++i) traits::construct(alloc, ptr + i, *itr);
}
constexpr Arr(const Arr& x): Arr(x, traits::select_on_container_copy_construction(x.alloc)) {}
constexpr Arr(Arr&& x) noexcept: alloc(std::move(x.alloc)), ptr(x.ptr), len(x.len) { x.ptr= nullptr, x.len= 0; }
constexpr Arr(const Arr& x, const allocator_type& a): alloc(a), len(x.len) {
if(len == 0) [[unlikely]]
return;
ptr= traits::allocate(alloc, len);
for(size_type i= 0; i != len; ++i) traits::construct(alloc, ptr + i, *(x.ptr + i));
}
constexpr Arr(Arr&& x, const allocator_type& a): alloc(a) {
if(traits::is_always_equal || x.get_allocator() == a) {
ptr= x.ptr, len= x.len;
x.ptr= nullptr, x.len= 0;
} else {
if(x.len == 0) [[unlikely]]
return;
len= x.len;
ptr= traits::allocate(alloc, len);
for(size_type i= 0; i != len; ++i) traits::construct(alloc, ptr + i, std::move(*(x.ptr + i)));
traits::deallocate(x.alloc, x.ptr, x.len);
x.ptr= nullptr, x.len= 0;
}
}
constexpr Arr(std::initializer_list<value_type> il, const allocator_type& a= Allocator()): Arr(il.begin(), il.end(), a) {}
constexpr ~Arr() {
if(len != 0) {
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
traits::deallocate(alloc, ptr, len);
}
}
constexpr Arr& operator=(const Arr& x) {
if(&x == this) return *this;
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
if(traits::propagate_on_container_copy_assignment::value || len != x.len) {
if(len != 0) traits::deallocate(alloc, ptr, len);
if constexpr(traits::propagate_on_container_copy_assignment::value) alloc= x.alloc;
ptr= traits::allocate(alloc, x.len);
}
len= x.len;
for(size_type i= 0; i != len; ++i) *(ptr + i)= *(x.ptr + i);
return *this;
}
constexpr Arr& operator=(Arr&& x) noexcept(traits::propagate_on_container_move_assignment::value || traits::is_always_equal::value) {
if(&x == this) return *this;
if(len != 0) {
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
traits::deallocate(alloc, ptr, len);
}
if constexpr(traits::propagate_on_container_move_assignment::value) alloc= std::move(x.alloc);
ptr= x.ptr, len= x.len;
x.ptr= nullptr, x.len= 0;
return *this;
}
constexpr Arr& operator=(std::initializer_list<value_type> init) {
assign(init.begin(), init.end());
return *this;
}
constexpr iterator begin() noexcept { return ptr; }
constexpr const_iterator begin() const noexcept { return ptr; }
constexpr iterator end() noexcept { return ptr + len; }
constexpr const_iterator end() const noexcept { return ptr + len; }
constexpr const_iterator cbegin() const noexcept { return ptr; }
constexpr const_iterator cend() const noexcept { return ptr + len; }
constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(ptr + len); }
constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(ptr + len); }
constexpr reverse_iterator rend() noexcept { return reverse_iterator(ptr); }
constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(ptr); }
constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(ptr + len); }
constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(ptr); }
constexpr size_type size() const noexcept { return len; }
constexpr size_type max_size() const noexcept {
const auto tmp= traits::max_size(alloc);
return tmp < 2147483647 ? tmp : 2147483647;
}
constexpr void resize(const size_type sz) {
if(len == sz) return;
if(sz == 0) {
clear();
return;
}
const pointer new_ptr= traits::allocate(alloc, sz);
const size_type mn= len < sz ? len : sz;
if(len != 0) {
for(size_type i= 0; i != mn; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
traits::deallocate(alloc, ptr, len);
}
ptr= new_ptr;
for(size_type i= len; i < sz; ++i) traits::construct(alloc, ptr + i);
len= sz;
}
constexpr void resize(const size_type sz, const value_type& c) {
if(len == sz) return;
if(sz == 0) {
clear();
return;
}
const pointer new_ptr= traits::allocate(alloc, sz);
const size_type mn= len < sz ? len : sz;
if(len != 0) {
for(size_type i= 0; i != mn; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
traits::deallocate(alloc, ptr, len);
}
ptr= new_ptr;
for(size_type i= len; i < sz; ++i) traits::construct(alloc, ptr + i, c);
len= sz;
}
constexpr void resize(const size_type sz, ArrNoInitTag) {
if(len == sz) return;
if(sz == 0) {
clear();
return;
}
const pointer new_ptr= traits::allocate(alloc, sz);
const size_type mn= len < sz ? len : sz;
if(len != 0) {
for(size_type i= 0; i != mn; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
traits::deallocate(alloc, ptr, len);
}
ptr= new_ptr;
len= sz;
}
[[nodiscard]] constexpr bool empty() const noexcept { return len == 0; }
GSH_INTERNAL_INLINE constexpr reference operator[](const size_type n) {
#ifndef NDEBUG
if(n >= len) [[unlikely]]
throw gsh::Exception("gsh::Arr::operator[] / The index is out of range. ( n=", n, ", size=", len, " )");
#endif
Assume(n < len);
return *(ptr + n);
}
GSH_INTERNAL_INLINE constexpr const_reference operator[](const size_type n) const {
#ifndef NDEBUG
if(n >= len) [[unlikely]]
throw gsh::Exception("gsh::Arr::operator[] / The index is out of range. ( n=", n, ", size=", len, " )");
#endif
Assume(n < len);
return *(ptr + n);
}
GSH_INTERNAL_INLINE constexpr reference at(const size_type n) {
if(n >= len) [[unlikely]]
throw gsh::Exception("gsh::Arr::at / The index is out of range. ( n=", n, ", size=", len, " )");
return *(ptr + n);
}
GSH_INTERNAL_INLINE constexpr const_reference at(const size_type n) const {
if(n >= len) [[unlikely]]
throw gsh::Exception("gsh::Arr::at / The index is out of range. ( n=", n, ", size=", len, " )");
return *(ptr + n);
}
GSH_INTERNAL_INLINE constexpr reference at_unchecked(const size_type n) noexcept {
Assume(n < len);
return *(ptr + n);
}
GSH_INTERNAL_INLINE constexpr const_reference at_unchecked(const size_type n) const noexcept {
Assume(n < len);
return *(ptr + n);
}
constexpr pointer data() noexcept { return ptr; }
constexpr const_pointer data() const noexcept { return ptr; }
constexpr reference front() noexcept { return *ptr; }
constexpr const_reference front() const noexcept { return *ptr; }
constexpr reference back() noexcept { return *(ptr + len - 1); }
constexpr const_reference back() const noexcept { return *(ptr + len - 1); }
template<std::input_iterator InputIter> constexpr void assign(const InputIter first, const InputIter last) {
const size_type n= std::distance(first, last);
if(n == 0) {
clear();
} else if(len == n) {
InputIter itr= first;
for(size_type i= 0; i != len; ++itr, ++i) *(ptr + i)= *itr;
} else {
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
traits::deallocate(alloc, ptr, len);
ptr= traits::allocate(alloc, n);
len= n;
InputIter itr= first;
for(size_type i= 0; i != n; ++itr, ++i) traits::construct(alloc, ptr + i, *itr);
}
}
constexpr void assign(const size_type n, const value_type& t) {
if(n == 0) {
clear();
} else if(len == n) {
for(size_type i= 0; i != len; ++i) *(ptr + i)= t;
} else {
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
traits::deallocate(alloc, ptr, len);
ptr= traits::allocate(alloc, n);
len= n;
for(size_type i= 0; i != n; ++i) traits::construct(alloc, ptr + i, t);
}
}
constexpr void assign(const size_type n, ArrNoInitTag) {
clear();
if(n != 0) {
ptr= traits::allocator(alloc, n);
len= n;
}
}
constexpr void assign(std::initializer_list<value_type> il) { assign(il.begin(), il.end()); }
constexpr void swap(Arr& x) noexcept(traits::propagate_on_container_swap::value || traits::is_always_equal::value) {
using std::swap;
swap(ptr, x.ptr);
swap(len, x.len);
if constexpr(traits::propagate_on_container_swap::value) swap(alloc, x.alloc);
}
constexpr void clear() {
if(len != 0) {
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
traits::deallocate(alloc, ptr, len);
ptr= nullptr, len= 0;
}
}
constexpr void reset() {
if(len != 0) {
traits::deallocate(alloc, ptr, len);
ptr= nullptr, len= 0;
}
}
constexpr allocator_type get_allocator() const noexcept { return alloc; }
friend constexpr bool operator==(const Arr& x, const Arr& y) {
if(x.len != y.len) return false;
bool res= true;
for(size_type i= 0; i != x.len;) {
const bool f= *(x.ptr + i) == *(y.ptr + i);
res&= f;
i= f ? i + 1 : x.len;
}
return res;
}
friend constexpr auto operator<=>(const Arr& x, const Arr& y) { return std::lexicographical_compare_three_way(x.begin(), x.end(), y.begin(), y.end()); }
friend constexpr void swap(Arr& x, Arr& y) noexcept(noexcept(x.swap(y))) { x.swap(y); }
};
template<std::input_iterator InputIter, class Alloc= Allocator<typename std::iterator_traits<InputIter>::value_type>> Arr(InputIter, InputIter, Alloc= Alloc()) -> Arr<typename std::iterator_traits<InputIter>::value_type, Alloc>;
template<class T, itype::u32 N>
requires std::same_as<T, std::remove_cv_t<T>>
class StaticArr: public ViewInterface<StaticArr<T, N>, T> {
union {
T elems[(N == 0 ? 1 : N)];
};
public:
using reference= T&;
using const_reference= const T&;
using iterator= T*;
using const_iterator= const T*;
using size_type= itype::u32;
using difference_type= itype::i32;
using value_type= T;
using pointer= T*;
using const_pointer= const T*;
using reverse_iterator= std::reverse_iterator<iterator>;
using const_reverse_iterator= std::reverse_iterator<const_iterator>;
constexpr StaticArr() noexcept(noexcept(value_type{})): elems{} {}
constexpr StaticArr(ArrNoInitTag) noexcept {}
template<class U, class... Args> constexpr StaticArr(ArrInitTag<U>, Args&&... args): elems{static_cast<U>(std::forward<Args>(args))...} {
static_assert(std::same_as<T, U>, "gsh::StaticArr::StaticArr / The type specified in gsh::ArrInitTag is different from value_type.");
static_assert(sizeof...(Args) == N, "gsh::StaticArr::StaticArr / The number of arguments is greater than the length of the array.");
}
template<class... Args> constexpr StaticArr(ArrInitTag<void>, Args&&... args): elems{static_cast<T>(std::forward<Args>(args))...} { static_assert(sizeof...(Args) <= N, "gsh::StaticArr::StaticArr / The number of arguments is greater than the length of the array."); }
constexpr explicit StaticArr(const value_type& value) {
for(itype::u32 i= 0; i != N; ++i) std::construct_at(elems + i, value);
}
template<std::input_iterator InputIter> constexpr explicit StaticArr(InputIter first) {
for(itype::u32 i= 0; i != N; ++first, ++i) std::construct_at(elems + i, *first);
}
template<std::input_iterator InputIter> constexpr StaticArr(InputIter first, InputIter last) {
const itype::u32 n= std::distance(first, last);
if(n != N) throw gsh::Exception("gsh::StaticArr::StaticArr / The size of the given range differs from the size of the array.");
for(itype::u32 i= 0; i != N; ++first, ++i) std::construct_at(elems + i, *first);
}
constexpr StaticArr(const value_type (&a)[N]) {
for(itype::u32 i= 0; i != N; ++i) std::construct_at(elems + i, a[i]);
}
constexpr StaticArr(value_type (&&a)[N]) {
for(itype::u32 i= 0; i != N; ++i) std::construct_at(elems + i, std::move(a[i]));
}
constexpr StaticArr(const StaticArr& x) {
for(itype::u32 i= 0; i != N; ++i) std::construct_at(elems + i, x.elems[i]);
}
constexpr StaticArr(StaticArr&& y) {
for(itype::u32 i= 0; i != N; ++i) std::construct_at(elems + i, std::move(y.elems[i]));
}
constexpr StaticArr(std::initializer_list<value_type> il): StaticArr(il.begin(), il.end()) {}
constexpr ~StaticArr() noexcept {
if constexpr(!std::is_trivially_destructible_v<value_type>)
for(itype::u32 i= 0; i != N; ++i) std::destroy_at(elems + i);
}
constexpr StaticArr& operator=(const StaticArr& x) {
for(itype::u32 i= 0; i != N; ++i) elems[i]= x.elems[i];
return *this;
}
constexpr StaticArr& operator=(StaticArr&& x) noexcept {
for(itype::u32 i= 0; i != N; ++i) elems[i]= std::move(x.elems[i]);
return *this;
}
constexpr StaticArr& operator=(std::initializer_list<value_type> init) {
assign(init.begin(), init.end());
return *this;
}
constexpr iterator begin() noexcept { return elems; }
constexpr const_iterator begin() const noexcept { return elems; }
constexpr iterator end() noexcept { return elems + N; }
constexpr const_iterator end() const noexcept { return elems + N; }
constexpr const_iterator cbegin() const noexcept { return elems; }
constexpr const_iterator cend() const noexcept { return elems + N; }
constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(elems + N); }
constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(elems + N); }
constexpr reverse_iterator rend() noexcept { return reverse_iterator(elems); }
constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(elems); }
constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(elems + N); }
constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(elems); }
constexpr size_type size() const noexcept { return N; }
constexpr size_type max_size() const noexcept { return N; }
[[nodiscard]] constexpr bool empty() const noexcept { return N != 0; }
GSH_INTERNAL_INLINE constexpr reference operator[](const size_type n) {
#ifndef NDEBUG
if(n >= N) [[unlikely]]
throw gsh::Exception("gsh::StaticArr::operator[] / The index is out of range. ( n=", n, ", size=", N, " )");
#endif
Assume(n < N);
return elems[n];
}
GSH_INTERNAL_INLINE constexpr const_reference operator[](const size_type n) const {
#ifndef NDEBUG
if(n >= N) [[unlikely]]
throw gsh::Exception("gsh::StaticArr::operator[] / The index is out of range. ( n=", n, ", size=", N, " )");
#endif
Assume(n < N);
return elems[n];
}
GSH_INTERNAL_INLINE constexpr reference at(const size_type n) {
if(n >= N) [[unlikely]]
throw gsh::Exception("gsh::StaticArr::at / The index is out of range. ( n=", n, ", size=", N, " )");
return elems[n];
}
GSH_INTERNAL_INLINE constexpr const_reference at(const size_type n) const {
if(n >= N) [[unlikely]]
throw gsh::Exception("gsh::StaticArr::at / The index is out of range. ( n=", n, ", size=", N, " )");
return elems[n];
}
GSH_INTERNAL_INLINE constexpr reference at_unchecked(const size_type n) noexcept {
Assume(n < N);
return elems[n];
}
GSH_INTERNAL_INLINE constexpr const_reference at_unchecked(const size_type n) const noexcept {
Assume(n < N);
return elems[n];
}
constexpr pointer data() noexcept { return elems; }
constexpr const_pointer data() const noexcept { return elems; }
constexpr reference front() noexcept { return elems[0]; }
constexpr const_reference front() const noexcept { return elems[0]; }
constexpr reference back() noexcept { return elems[N - 1]; }
constexpr const_reference back() const noexcept { return elems[N - 1]; }
template<std::input_iterator InputIter> constexpr void assign(InputIter first) {
for(itype::u32 i= 0; i != N; ++first, ++i) elems[i]= *first;
}
template<std::input_iterator InputIter> constexpr void assign(InputIter first, const InputIter last) {
const itype::u32 n= std::distance(first, last);
if(n != N) throw gsh::Exception("gsh::StaticArr::assign / The size of the given range differs from the size of the array.");
for(itype::u32 i= 0; i != N; ++first, ++i) elems[i]= *first;
}
constexpr void assign(const value_type& value) {
for(itype::u32 i= 0; i != N; ++i) elems[i]= value;
}
constexpr void assign(std::initializer_list<value_type> il) { assign(il.begin(), il.end()); }
constexpr void swap(StaticArr& x) {
using std::swap;
for(itype::u32 i= 0; i != N; ++i) swap(elems[i], x.elems[i]);
}
friend constexpr bool operator==(const StaticArr& x, const StaticArr& y) {
bool res= true;
for(size_type i= 0; i != N;) {
const bool f= x.elems[i] == y.elems[i];
res&= f;
i= f ? i + 1 : N;
}
return res;
}
friend constexpr auto operator<=>(const StaticArr& x, const StaticArr& y) { return std::lexicographical_compare_three_way(x.begin(), x.end(), y.begin(), y.end()); }
friend constexpr void swap(StaticArr& x, StaticArr& y) noexcept(noexcept(x.swap(y))) { x.swap(y); }
};
template<class U, class... Args> StaticArr(ArrInitTag<U>, Args...) -> StaticArr<std::conditional_t<std::is_void_v<U>, std::common_type_t<Args...>, U>, sizeof...(Args)>;
}// namespace gsh
namespace std {
template<class T, gsh::itype::u32 N> struct tuple_size<gsh::StaticArr<T, N>>: integral_constant<size_t, N> {};
template<std::size_t M, class T, gsh::itype::u32 N> struct tuple_element<M, gsh::StaticArr<T, N>> {
static_assert(M < N, "std::tuple_element<gsh::StaticArr<T, N>> / The index is out of range.");
using type= T;
};
}// namespace std
namespace gsh {
template<std::size_t M, class T, itype::u32 N> const T& get(const StaticArr<T, N>& a) {
static_assert(M < N, "gsh::get(gsh::StaticArr<T, N>) / The index is out of range.");
return a[M];
}
template<std::size_t M, class T, itype::u32 N> T& get(StaticArr<T, N>& a) {
static_assert(M < N, "gsh::get(gsh::StaticArr<T, N>) / The index is out of range.");
return a[M];
}
template<std::size_t M, class T, itype::u32 N> T&& get(StaticArr<T, N>&& a) {
static_assert(M < N, "gsh::get(gsh::StaticArr<T, N>) / The index is out of range.");
return std::move(a[M]);
}
}// namespace gsh
#include <immintrin.h>
namespace gsh {
template<itype::u32 Size>
requires(Size <= (1u << 24))
class BitTree24 {
constexpr static itype::u32 s1= ((Size + 262143) / 262144 + 63) / 64 * 64, s2= ((Size + 4095) / 4096 + 63) / 64 * 64, s3= ((Size + 63) / 64 + 63) / 64 * 64;
itype::u64 v0;
alignas(32) itype::u64 v1[s1], v2[s2], v3[s3];
constexpr void build() noexcept {
if(std::is_constant_evaluated()) {
v0= 0;
for(itype::u32 i= 0; i != s1; ++i) v1[i]= 0;
for(itype::u32 i= 0; i != s2; ++i) v2[i]= 0;
for(itype::u32 i= 0; i != s3; ++i) v2[i / 64]|= (static_cast<itype::u64>(v3[i] != 0) << (i % 64));
for(itype::u32 i= 0; i != s2; ++i) v1[i / 64]|= (static_cast<itype::u64>(v2[i] != 0) << (i % 64));
for(itype::u32 i= 0; i != s1; ++i) v0|= (static_cast<itype::u64>(v1[i] != 0) << (i % 64));
return;
}
for(itype::u32 x= 0; x < s3; x+= 64) {
auto get= [&](itype::u32 n) -> itype::u32 {
return _mm256_movemask_pd(_mm256_castsi256_pd(_mm256_cmpeq_epi64(_mm256_load_si256(reinterpret_cast<const __m256i*>(&v3[x + n])), _mm256_setzero_si256())));
};
const itype::u64 a= get(0), b= get(4), c= get(8), d= get(12), e= get(16), f= get(20), g= get(24), h= get(28);
const itype::u64 i= get(32), j= get(36), k= get(40), l= get(44), m= get(48), n= get(52), o= get(56), p= get(60);
v2[x / 64]= ~(a | b << 4 | c << 8 | d << 12 | e << 16 | f << 20 | g << 24 | h << 28 | i << 32 | j << 36 | k << 40 | l << 44 | m << 48 | n << 52 | o << 56 | p << 60);
}
for(itype::u32 i= s3 / 64; i != s2; ++i) v2[i]= 0;
for(itype::u32 i= s3 / 64 * 64; i != s3; ++i) v2[i / 64]|= (static_cast<itype::u64>(v3[i] != 0) << (i % 64));
for(itype::u32 x= 0; x < s2; x+= 64) {
auto get= [&](itype::u32 n) -> itype::u32 {
return _mm256_movemask_pd(_mm256_castsi256_pd(_mm256_cmpeq_epi64(_mm256_load_si256(reinterpret_cast<const __m256i*>(&v2[x + n])), _mm256_setzero_si256())));
};
const itype::u64 a= get(0), b= get(4), c= get(8), d= get(12), e= get(16), f= get(20), g= get(24), h= get(28);
const itype::u64 i= get(32), j= get(36), k= get(40), l= get(44), m= get(48), n= get(52), o= get(56), p= get(60);
v1[x / 64]= ~(a | b << 4 | c << 8 | d << 12 | e << 16 | f << 20 | g << 24 | h << 28 | i << 32 | j << 36 | k << 40 | l << 44 | m << 48 | n << 52 | o << 56 | p << 60);
}
for(itype::u32 i= s2 / 64; i != s1; ++i) v1[i]= 0;
for(itype::u32 i= s2 / 64 * 64; i != s2; ++i) v1[i / 64]|= (static_cast<itype::u64>(v2[i] != 0) << (i % 64));
v0= 0;
for(itype::u32 i= 0; i != s1; ++i) v0|= (static_cast<itype::u64>(v1[i] != 0) << (i % 64));
}
public:
constexpr BitTree24() noexcept: v0{}, v1{}, v2{}, v3{} {}
constexpr BitTree24(itype::u64 val) noexcept: v0{}, v1{}, v2{}, v3{} {
if(val != 0) {
v0= v1[0]= v2[0]= 1ull;
v3[0]= val;
}
}
constexpr BitTree24(const ctype::c8* p) { assign(p); }
constexpr BitTree24(const ctype::c8* p, itype::u32 sz, const ctype::c8 one= '1') { assign(p, sz, one); }
constexpr BitTree24& operator=(const BitTree24&) noexcept= default;
constexpr void assign(const ctype::c8* p) { assign(p, StrLen(p)); }
constexpr void assign(const ctype::c8* p, itype::u32 sz, const ctype::c8 one= '1') {
sz= sz < Size ? sz : Size;
if(std::is_constant_evaluated()) {
for(itype::u32 i= 0; i != s3; ++i) v3[i]= 0;
for(itype::u32 i= 0; i < sz; ++i) v3[i / 64]|= static_cast<itype::u64>(p[i] == one) << (i % 64);
build();
return;
}
const __m256i ones= _mm256_set1_epi8(one);
for(itype::u32 i= 0; i < sz; i+= 64) {
auto get= [&](itype::u32 n) -> itype::u32 {
return _mm256_movemask_epi8(_mm256_cmpeq_epi8(_mm256_loadu_si256(reinterpret_cast<const __m256i_u*>(p + i + n)), ones));
};
const itype::u64 a= get(0), b= get(8), c= get(16), d= get(24), e= get(32), f= get(40), g= get(48), h= get(56);
v3[i / 64]= a | b << 8 | c << 16 | d << 24 | e << 32 | f << 40 | g << 48 | h << 56;
}
for(itype::u32 i= sz / 64; i != s3; ++i) v3[i]= 0;
for(itype::u32 i= sz / 64 * 64; i < sz; i+= 4) {
const itype::u32 n= _mm256_movemask_epi8(_mm256_cmpeq_epi8(_mm256_loadu_si256(reinterpret_cast<const __m256i_u*>(p + i)), ones));
v3[i / 64]|= static_cast<itype::u64>(n) << (i % 64);
}
const itype::u32 b= sz / 4 * 4;
switch(sz % 4) {
case 3: v3[(b + 2) / 64]|= static_cast<itype::u64>(p[b + 2] == one) << ((b + 2) % 64); [[fallthrough]];
case 2: v3[(b + 1) / 64]|= static_cast<itype::u64>(p[b + 1] == one) << ((b + 1) % 64); [[fallthrough]];
case 1: v3[(b + 0) / 64]|= static_cast<itype::u64>(p[b + 0] == one) << ((b + 0) % 64); [[fallthrough]];
default: break;
}
build();
}
constexpr BitTree24& operator&=(const BitTree24& rhs) noexcept {
for(itype::u32 i= 0; i != s3; ++i) v3[i]&= rhs.v3[i];
build();
return *this;
}
constexpr BitTree24& operator|=(const BitTree24& rhs) noexcept {
v0|= rhs.v0;
for(itype::u32 i= 0; i != s1; ++i) v1[i]|= rhs.v1[i];
for(itype::u32 i= 0; i != s2; ++i) v2[i]|= rhs.v2[i];
for(itype::u32 i= 0; i != s3; ++i) v3[i]|= rhs.v3[i];
return *this;
}
constexpr BitTree24& operator^=(const BitTree24& rhs) noexcept {
for(itype::u32 i= 0; i != s3; ++i) v3[i]^= rhs.v3[i];
build();
return *this;
}
constexpr BitTree24& operator<<=(itype::u32 pos) noexcept;
constexpr BitTree24& operator>>=(itype::u32 pos) noexcept;
constexpr BitTree24& set() noexcept {
for(itype::u32 i= 0; i != (Size + 4095) / 262144; ++i) v1[i]= 0xffffffffffffffff;
for(itype::u32 i= 0; i != (Size + 63) / 4096; ++i) v2[i]= 0xffffffffffffffff;
for(itype::u32 i= 0; i != Size / 64; ++i) v3[i]= 0xffffffffffffffff;
if constexpr(Size + 262143 >= (1ull << 24)) v0= 0xffffffffffffffff;
else v0= (1ull << ((Size + 262143) / 262144)) - 1;
if constexpr(constexpr itype::u32 x= (Size + 4095) / 4096; x % 64 != 0) v1[x / 64]= (1ull << (x % 64)) - 1;
if constexpr(constexpr itype::u32 x= (Size + 63) / 64; x % 64 != 0) v2[x / 64]= (1ull << (x % 64)) - 1;
if constexpr(Size % 64 != 0) v3[Size / 64]= (1ull << (Size % 64)) - 1;
return *this;
}
constexpr BitTree24& set(itype::u32 pos) {
v0|= (1ull << (pos / 262144));
v1[pos / 262144]|= (1ull << (pos / 4096) % 64);
v2[pos / 4096]|= (1ull << ((pos / 64) % 64));
v3[pos / 64]|= (1ull << (pos % 64));
return *this;
}
constexpr BitTree24& set(itype::u32 pos, bool val) {
if(val) return set(pos);
else return reset(pos);
}
constexpr BitTree24& reset() noexcept {
std::memset(this, 0, sizeof(BitTree24));
return *this;
}
constexpr BitTree24& reset(itype::u32 pos) {
const itype::u64 m1= (1ull << ((pos / 4096) % 64)), m2= (1ull << ((pos / 64) % 64)), m3= (1ull << (pos % 64));
const bool f1= v1[pos / 262144] == m1, f2= v2[pos / 4096] == m2, f3= v3[pos / 64] == m3;
v3[pos / 64]&= ~m3;
v2[pos / 4096]&= (f3 ? ~m2 : 0xffffffffffffffff);
v1[pos / 262144]&= (f2 && f3 ? ~m1 : 0xffffffffffffffff);
v0&= (f1 && f2 && f3 ? ~(1ull << (pos / 262144)) : 0xffffffffffffffff);
return *this;
}
constexpr BitTree24& flip() noexcept {
for(itype::u32 i= 0; i != s3; ++i) v3[i]= ~v3[i];
build();
}
constexpr BitTree24& flip(itype::u32 pos) noexcept { return set(pos, !test(pos)); }
class reference {
friend class BitTree24;
BitTree24& ref;
itype::u32 idx;
constexpr reference(BitTree24& ref_, itype::u32 idx_) noexcept: ref(ref_), idx(idx_) {}
public:
constexpr ~reference() noexcept {}
constexpr reference& operator=(bool x) noexcept {
ref.set(idx, x);
return *this;
}
constexpr reference& operator=(const reference& x) {
ref.set(idx, x);
return *this;
}
constexpr bool operator~() const noexcept { return ~ref.test(idx); }
constexpr operator bool() const noexcept { return ref.test(idx); }
constexpr reference& flip() const noexcept {
ref.flip(idx);
return *this;
}
};
constexpr bool operator[](itype::u32 pos) const { return test(pos); }
constexpr reference operator[](itype::u32 pos) { return reference(*this, pos); }
constexpr itype::u32 count() const noexcept {
itype::u32 res= 0;
for(itype::u32 i= 0; i != s3; ++i) res+= std::popcount(v3[i]);
return res;
}
constexpr itype::u32 size() const noexcept { return Size; }
constexpr bool test(itype::u32 pos) const { return v3[pos / 64] >> (pos % 64) & 1; }
constexpr bool all() const noexcept {
if constexpr(Size + 262143 >= (1ull << 24)) {
if(v0 != 0xffffffffffffffff) return false;
} else {
if(v0 != (1ull << ((Size + 262143) / 262144)) - 1) return false;
}
for(itype::u32 i= 0; i != (Size + 4095) / 262144; ++i)
if(v1[i] != 0xffffffffffffffff) return false;
if constexpr(constexpr itype::u32 x= (Size + 4095) / 4096; x % 64 != 0) {
if(v1[x / 64] != (1ull << (x % 64)) - 1) return false;
}
for(itype::u32 i= 0; i != (Size + 63) / 4096; ++i)
if(v2[i] != 0xffffffffffffffff) return false;
if constexpr(constexpr itype::u32 x= (Size + 63) / 64; x % 64 != 0) {
if(v2[x / 64] != (1ull << (x % 64)) - 1) return false;
}
for(itype::u32 i= 0; i != Size / 64; ++i)
if(v3[i] != 0xffffffffffffffff) return false;
if constexpr(Size % 64 != 0) {
if(v3[Size / 64] != (1ull << (Size % 64)) - 1) return false;
}
return true;
}
constexpr bool any() const noexcept { return v0 != 0; }
constexpr bool none() const noexcept { return v0 == 0; }
constexpr itype::u64 to_u64() const {
if(v0 > 1 || v1[0] > 1 || v2[0] > 1) throw Exception("gsh::BitTree::to_u64 / Result overflowed.");
return v3[0];
}
constexpr unsigned long to_ulong() const { return to_u64(); }
constexpr unsigned long long to_ullong() const { return to_u64(); }
constexpr bool operator==(const BitTree24& rhs) const noexcept {
if(v0 != rhs.v0) return false;
if(std::is_constant_evaluated()) {
for(itype::u32 i= 0; i != s1; ++i)
if(v1[i] != rhs.v1[i]) return false;
for(itype::u32 i= 0; i != s2; ++i)
if(v2[i] != rhs.v2[i]) return false;
for(itype::u32 i= 0; i != s3; ++i)
if(v3[i] != rhs.v3[i]) return false;
return true;
}
for(itype::u32 i= 0; i != s1; i+= 4) {
const itype::u32 t= _mm256_movemask_epi8(_mm256_cmpeq_epi8(_mm256_load_si256((const __m256i*)&v1[i]), _mm256_load_si256((const __m256i*)&rhs.v1[i])));
if(t != 0xffffffff) return false;
}
for(itype::u32 i= 0; i != s2; i+= 4) {
const itype::u32 t= _mm256_movemask_epi8(_mm256_cmpeq_epi8(_mm256_load_si256((const __m256i*)&v2[i]), _mm256_load_si256((const __m256i*)&rhs.v2[i])));
if(t != 0xffffffff) return false;
}
for(itype::u32 i= 0; i != s3; i+= 4) {
const itype::u32 t= _mm256_movemask_epi8(_mm256_cmpeq_epi8(_mm256_load_si256((const __m256i*)&v3[i]), _mm256_load_si256((const __m256i*)&rhs.v3[i])));
if(t != 0xffffffff) return false;
}
return true;
}
constexpr bool operator!=(const BitTree24& rhs) const noexcept { return !operator==(rhs); }
constexpr BitTree24 operator<<(itype::u32 pos) const noexcept { return BitTree24(*this)<<= pos; }
constexpr BitTree24 operator>>(itype::u32 pos) const noexcept { return BitTree24(*this)>>= pos; }
friend constexpr BitTree24 operator&(const BitTree24& lhs, const BitTree24& rhs) noexcept { return BitTree24(lhs)&= rhs; }
friend constexpr BitTree24 operator|(const BitTree24& lhs, const BitTree24& rhs) noexcept { return BitTree24(lhs)|= rhs; }
friend constexpr BitTree24 operator^(const BitTree24& lhs, const BitTree24& rhs) noexcept { return BitTree24(lhs)^= rhs; }
constexpr static itype::u32 npos= -1;
constexpr itype::u32 find_next(itype::u32 pos) const {
if(const itype::u64 tmp= v3[pos / 64] & -(1ull << (pos % 64)); tmp != 0) return pos / 64 * 64 + std::countr_zero(tmp);
if(const itype::u64 tmp= v2[pos / 4096] & -(2ull << (pos / 64 % 64)); tmp != 0) {
const itype::u64 a= pos / 4096 * 64 + std::countr_zero(tmp), b= v3[a];
Assume(b != 0);
return a * 64 + std::countr_zero(b);
}
if(const itype::u64 tmp= v1[pos / 262144] & -(2ull << (pos / 4096 % 64)); tmp != 0) {
const itype::u64 a= pos / 262144 * 64 + std::countr_zero(tmp), b= v2[a];
Assume(b != 0);
const itype::u64 c= a * 64 + std::countr_zero(b), d= v3[c];
Assume(d != 0);
return c * 64 + std::countr_zero(d);
}
if(const itype::u64 tmp= v0 & -(2ull << (pos / 262144 % 64)); tmp != 0) {
const itype::u64 a= std::countr_zero(tmp), b= v1[a];
Assume(b != 0);
const itype::u64 c= a * 64 + std::countr_zero(b), d= v2[c];
Assume(d != 0);
const itype::u64 e= c * 64 + std::countr_zero(d), f= v3[e];
Assume(f != 0);
return e * 64 + std::countr_zero(f);
}
return npos;
}
constexpr itype::u32 find_first() const noexcept { return find_next(0); }
constexpr itype::u32 find_prev(itype::u32 pos) const {
if(const itype::u64 tmp= v3[pos / 64] & ((2ull << (pos % 64)) - 1); tmp != 0) return pos / 64 * 64 + std::bit_width(tmp) - 1;
if(const itype::u64 tmp= v2[pos / 4096] & ((1ull << (pos / 64 % 64)) - 1); tmp != 0) {
const itype::u64 a= pos / 4096 * 64 + std::bit_width(tmp) - 1, b= v3[a];
Assume(b != 0);
return a * 64 + std::bit_width(b) - 1;
}
if(const itype::u64 tmp= v1[pos / 262144] & ((1ull << (pos / 4096 % 64)) - 1); tmp != 0) {
const itype::u64 a= pos / 262144 * 64 + std::bit_width(tmp) - 1, b= v2[a];
Assume(b != 0);
const itype::u64 c= a * 64 + std::bit_width(b) - 1, d= v3[c];
Assume(d != 0);
return c * 64 + std::bit_width(d) - 1;
}
if(const itype::u64 tmp= v0 & ((1ull << (pos / 262144 % 64)) - 1); tmp != 0) {
const itype::u64 a= std::bit_width(tmp) - 1, b= v1[a];
Assume(b != 0);
const itype::u64 c= a * 64 + std::bit_width(b) - 1, d= v2[c];
Assume(d != 0);
const itype::u64 e= c * 64 + std::bit_width(d) - 1, f= v3[e];
Assume(f != 0);
return e * 64 + std::bit_width(f) - 1;
}
return npos;
}
constexpr itype::u32 find_last() const noexcept { return find_prev(Size - 1); }
};
}// namespace gsh
namespace gsh {
template<class T, class Alloc= Allocator<T>> class RangeSumQuery {
Arr<T, Alloc> bit;
public:
using reference= T&;
using const_reference= const T&;
using size_type= itype::u32;
using difference_type= itype::i32;
using value_type= T;
using allocator_type= Alloc;
using pointer= typename AllocatorTraits<Alloc>::pointer;
using const_pointer= typename AllocatorTraits<Alloc>::const_pointer;
constexpr RangeSumQuery() noexcept(noexcept(Alloc())): RangeSumQuery(Alloc()) {}
constexpr explicit RangeSumQuery(const Alloc& alloc) noexcept: bit(alloc) {}
constexpr explicit RangeSumQuery(itype::u32 n, const Alloc& alloc= Alloc()): bit(n, alloc) {}
constexpr RangeSumQuery(itype::u32 n, const T& value, const Alloc& alloc= Alloc()): bit(alloc) { assign(n, value); }
template<class InputIter> constexpr RangeSumQuery(InputIter first, InputIter last, const Alloc& alloc= Alloc()): bit(alloc) { assign(first, last); }
constexpr RangeSumQuery(const RangeSumQuery&)= default;
constexpr RangeSumQuery(RangeSumQuery&&) noexcept= default;
constexpr RangeSumQuery(const RangeSumQuery& x, const Alloc& alloc): bit(x.bit, alloc) {}
constexpr RangeSumQuery(RangeSumQuery&& x, const Alloc& alloc): bit(std::move(x.bit), alloc) {}
constexpr RangeSumQuery(std::initializer_list<T> il, const Alloc& alloc= Alloc()): RangeSumQuery(il.begin(), il.end(), alloc) {}
constexpr RangeSumQuery& operator=(const RangeSumQuery&)= default;
constexpr RangeSumQuery& operator=(RangeSumQuery&&) noexcept(AllocatorTraits<Alloc>::propagate_on_container_move_assignment::value || AllocatorTraits<Alloc>::is_always_equal::value)= default;
constexpr RangeSumQuery& operator=(std::initializer_list<T> il) {
assign(il);
return *this;
}
constexpr itype::u32 size() const noexcept { return bit.size(); }
constexpr void resize(itype::u32 sz) { resize(sz, value_type{}); }
/*
    constexpr void resize(itype::u32 sz, const value_type& c) {
        itype::u32 n = bit.size();
        bit.resize(sz);
        if (n >= sz) return;
        // TODO
    }
    */
[[nodiscard]] constexpr bool empty() const noexcept { return bit.empty(); }
constexpr value_type operator[](itype::u32 n) const {
value_type res= bit[n];
if(!(n & 1)) return res;
itype::u32 tmp= n & (n + 1);
for(itype::u32 i= n; i != tmp; i&= i - 1) res-= bit[i - 1];
return res;
}
constexpr value_type at(itype::u32 n) const {
if(n >= size()) throw Exception("gsh::RangeSumQuery::at / Index is out of range.");
return operator[](n);
}
template<class InputIterator> constexpr void assign(InputIterator first, InputIterator last) {
bit.assign(first, last);
itype::u32 n= bit.size();
if(n == 0) return;
const auto tmp= bit[0];
for(itype::u32 i= 0; i != n - 1; ++i) {
const itype::u32 j= i + ((i + 1) & -(i + 1));
bit[j < n ? j : 0]+= bit[i];
}
bit[0]= tmp;
}
constexpr void assign(itype::u32 n, const T& u) {
if(n == 0) return;
bit= Arr<value_type, Alloc>(n, get_allocator());
Arr<value_type, Alloc> mul(std::bit_width(n), get_allocator());
mul[0]= u;
for(itype::u32 i= 1, sz= mul.size(); i < sz; ++i) mul[i]= mul[i - 1], mul[i]+= mul[i - 1];
for(itype::u32 i= 0; i != n; ++i) bit[i]= mul[std::countr_zero(i + 1)];
}
constexpr void assign(std::initializer_list<T> il) { assign(il.begin(), il.end()); }
constexpr void swap(RangeSumQuery& x) noexcept(AllocatorTraits<Alloc>::propagate_on_container_swap::value || AllocatorTraits<Alloc>::is_always_equal::value) { bit.swap(x.bit); };
constexpr void clear() { bit.clear(); }
constexpr allocator_type get_allocator() const noexcept { return bit.get_allocator(); }
constexpr void add(itype::u32 n, const value_type& x) {
for(itype::u32 i= n + 1, sz= size(); i <= sz; i+= (i & -i)) bit[i - 1]+= x;
}
constexpr void sub(itype::u32 n, const value_type& x) {
for(itype::u32 i= n + 1, sz= size(); i <= sz; i+= (i & -i)) bit[i - 1]-= x;
}
constexpr void inc(itype::u32 n) {
for(itype::u32 i= n + 1, sz= size(); i <= sz; i+= (i & (-i))) ++bit[i - 1];
}
constexpr void dec(itype::u32 n) {
for(itype::u32 i= n + 1, sz= size(); i <= sz; i+= (i & (-i))) --bit[i - 1];
}
constexpr value_type sum(itype::u32 n) const {
value_type res= {};
for(itype::u32 i= n; i != 0; i&= i - 1) res+= bit[i - 1];
return res;
}
constexpr value_type sum(itype::u32 l, itype::u32 r) const {
itype::u32 n= l & ~((std::bit_floor(l ^ r) << 1) - 1);
value_type res1= {}, res2= {};
for(itype::u32 i= r; i != n; i&= i - 1) res1+= bit[i - 1];
for(itype::u32 i= l; i != n; i&= i - 1) res2+= bit[i - 1];
return res1 - res2;
}
constexpr itype::u32 lower_bound(value_type x) const {
static_assert(std::is_unsigned_v<value_type>, "gsh::RangeSumQuery::lower_bound / value_type must be unsigned.");
itype::u32 res= 0, n= size();
for(itype::u32 len= std::bit_floor(n); len != 0; len>>= 1) {
if(res + len <= n && bit[res + len - 1] < x) {
x-= bit[res + len - 1];
res+= len;
}
}
return res;
}
constexpr itype::u32 upper_bound(value_type x) const {
static_assert(std::is_unsigned_v<value_type>, "gsh::RangeSumQuery::upper_bound / value_type must be unsigned.");
itype::u32 res= 0, n= size();
for(itype::u32 len= std::bit_floor(n); len != 0; len>>= 1) {
if(res + len <= n && !(x < bit[res + len - 1])) {
x-= bit[res + len - 1];
res+= len;
}
}
return res;
}
};
template<class U, class Alloc> constexpr void swap(RangeSumQuery<U, Alloc>& x, RangeSumQuery<U, Alloc>& y) noexcept(noexcept(x.swap(y))) {
x.swap(y);
}
template<class InputIterator, class Alloc= Allocator<typename std::iterator_traits<InputIterator>::value_type>> RangeSumQuery(InputIterator, InputIterator, Alloc= Alloc()) -> RangeSumQuery<typename std::iterator_traits<InputIterator>::value_type, Alloc>;
}// namespace gsh
namespace gsh {
namespace internal {
template<class CharT> using StrImpl= std::basic_string<CharT, std::char_traits<CharT>, Allocator<CharT>>;
template<class CharT> using StrViewImpl= std::basic_string_view<CharT, std::char_traits<CharT>>;
}// namespace internal
using Str= internal::StrImpl<ctype::c8>;
using Str8= internal::StrImpl<ctype::utf8>;
using Str16= internal::StrImpl<ctype::utf16>;
using Str32= internal::StrImpl<ctype::utf32>;
using Strw= internal::StrImpl<ctype::wc>;
template<class T> class Parser;
template<> class Parser<Str> {
public:
template<class Stream> constexpr Str operator()(Stream&& stream) const {
stream.reload(16);
Str res;
while(true) {
const ctype::c8* e= stream.current();
while(*e >= '!') ++e;
const itype::u32 len= e - stream.current();
const itype::u32 curlen= res.size();
res.resize(curlen + len);
MemoryCopy(res.data() + curlen, stream.current(), len);
stream.skip(len);
if(stream.avail() == 0) stream.reload();
else break;
}
stream.skip(1);
return res;
}
template<class Stream> constexpr Str operator()(Stream&& stream, itype::u32 n) const {
itype::u32 rem= n;
Str res;
itype::u32 avail= stream.avail();
while(avail <= rem) {
const itype::u32 curlen= res.size();
res.resize(curlen + avail);
MemoryCopy(res.data() + curlen, stream.current(), avail);
rem-= avail;
stream.skip(avail);
if(rem == 0) return res;
stream.reload();
avail= stream.avail();
}
const itype::u32 curlen= res.size();
res.resize(curlen + rem);
MemoryCopy(res.data() + curlen, stream.current(), rem);
stream.skip(rem + 1);
return res;
}
};
template<class T> class Formatter;
template<> class Formatter<Str> {
public:
template<class Stream> constexpr void operator()(Stream&& stream, const Str& str) const {
const ctype::c8* s= str.data();
itype::u32 len= str.size();
itype::u32 avail= stream.avail();
if(avail >= len) [[likely]] {
MemoryCopy(stream.current(), s, len);
stream.skip(len);
} else {
MemoryCopy(stream.current(), s, avail);
len-= avail;
s+= avail;
stream.skip(avail);
while(len != 0) {
stream.reload();
avail= stream.avail();
const itype::u32 tmp= len < avail ? len : avail;
MemoryCopy(stream.current(), s, tmp);
len-= tmp;
s+= tmp;
stream.skip(tmp);
}
}
}
};
}// namespace gsh
namespace gsh {
template<class T, class Allocator= Allocator<T>>
requires std::is_same_v<T, typename AllocatorTraits<Allocator>::value_type> && (!std::is_const_v<T>)
class Vec: public ViewInterface<Vec<T, Allocator>, T> {
using traits= AllocatorTraits<Allocator>;
public:
using reference= T&;
using const_reference= const T&;
using iterator= T*;
using const_iterator= const T*;
using size_type= itype::u32;
using difference_type= itype::i32;
using value_type= T;
using allocator_type= Allocator;
using pointer= typename traits::pointer;
using const_pointer= typename traits::const_pointer;
using reverse_iterator= std::reverse_iterator<iterator>;
using const_reverse_iterator= std::reverse_iterator<const_iterator>;
private:
[[no_unique_address]] allocator_type alloc;
pointer ptr= nullptr;
size_type len= 0, cap= 0;
public:
constexpr Vec() noexcept(noexcept(Allocator())) {}
constexpr explicit Vec(const allocator_type& a) noexcept: alloc(a) {}
constexpr explicit Vec(size_type n, const Allocator& a= Allocator()): alloc(a) {
if(n == 0) [[unlikely]]
return;
ptr= traits::allocate(alloc, n);
len= n, cap= n;
for(size_type i= 0; i != n; ++i) traits::construct(alloc, ptr + i);
}
constexpr explicit Vec(const size_type n, const value_type& value, const allocator_type& a= Allocator()): alloc(a) {
if(n == 0) [[unlikely]]
return;
ptr= traits::allocate(alloc, n);
len= n, cap= n;
for(size_type i= 0; i != n; ++i) traits::construct(alloc, ptr + i, value);
}
template<std::input_iterator InputIter> constexpr Vec(const InputIter first, const InputIter last, const allocator_type& a= Allocator()): alloc(a) {
const size_type n= std::distance(first, last);
if(n == 0) [[unlikely]]
return;
ptr= traits::allocate(alloc, n);
len= n, cap= n;
size_type i= 0;
for(InputIter itr= first; i != n; ++itr, ++i) traits::construct(alloc, ptr + i, *itr);
}
constexpr Vec(const Vec& x): Vec(x, traits::select_on_container_copy_construction(x.alloc)) {}
constexpr Vec(Vec&& x) noexcept: alloc(std::move(x.alloc)), ptr(x.ptr), len(x.len), cap(x.cap) { x.ptr= nullptr, x.len= 0, x.cap= 0; }
constexpr Vec(const Vec& x, const allocator_type& a): alloc(a), len(x.len), cap(x.len) {
if(len == 0) [[unlikely]]
return;
ptr= traits::allocate(alloc, cap);
for(size_type i= 0; i != len; ++i) traits::construct(alloc, ptr + i, *(x.ptr + i));
}
constexpr Vec(Vec&& x, const allocator_type& a): alloc(a) {
if(traits::is_always_equal || x.get_allocator() == a) {
ptr= x.ptr, len= x.len, cap= x.cap;
x.ptr= nullptr, x.len= 0, x.cap= 0;
} else {
if(x.len == 0) [[unlikely]]
return;
len= x.len, cap= x.cap;
ptr= traits::allocate(alloc, len);
for(size_type i= 0; i != len; ++i) traits::construct(alloc, ptr + i, std::move(*(x.ptr + i)));
traits::deallocate(x.alloc, x.ptr, x.cap);
x.ptr= nullptr, x.len= 0, x.cap= 0;
}
}
constexpr Vec(std::initializer_list<value_type> il, const allocator_type& a= Allocator()): Vec(il.begin(), il.end(), a) {}
constexpr ~Vec() {
if(cap != 0) {
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
traits::deallocate(alloc, ptr, cap);
}
}
constexpr Vec& operator=(const Vec& x) {
if(&x == this) return *this;
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
if(traits::propagate_on_container_copy_assignment::value || cap < x.len) {
if(cap != 0) traits::deallocate(alloc, ptr, cap);
if constexpr(traits::propagate_on_container_copy_assignment::value) alloc= x.alloc;
cap= x.len;
ptr= traits::allocate(alloc, cap);
}
len= x.len;
for(size_type i= 0; i != len; ++i) *(ptr + i)= *(x.ptr + i);
return *this;
}
constexpr Vec& operator=(Vec&& x) noexcept(traits::propagate_on_container_move_assignment::value || traits::is_always_equal::value) {
if(&x == this) return *this;
if(cap != 0) {
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
traits::deallocate(alloc, ptr, cap);
}
if constexpr(traits::propagate_on_container_move_assignment::value) alloc= std::move(x.alloc);
ptr= x.ptr, len= x.len, cap= x.cap;
x.ptr= nullptr, x.len= 0, x.cap= 0;
return *this;
}
constexpr Vec& operator=(std::initializer_list<value_type> init) {
assign(init.begin(), init.end());
return *this;
}
constexpr iterator begin() noexcept { return ptr; }
constexpr const_iterator begin() const noexcept { return ptr; }
constexpr iterator end() noexcept { return ptr + len; }
constexpr const_iterator end() const noexcept { return ptr + len; }
constexpr const_iterator cbegin() const noexcept { return ptr; }
constexpr const_iterator cend() const noexcept { return ptr + len; }
constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(ptr + len); }
constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(ptr + len); }
constexpr reverse_iterator rend() noexcept { return reverse_iterator(ptr); }
constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(ptr); }
constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(ptr + len); }
constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(ptr); }
constexpr size_type size() const noexcept { return len; }
constexpr size_type max_size() const noexcept {
const auto tmp= traits::max_size(alloc);
return tmp < 2147483647 ? tmp : 2147483647;
}
constexpr void resize(const size_type sz) {
if(cap < sz) {
const pointer new_ptr= traits::allocate(alloc, sz);
if(cap != 0) {
for(size_type i= 0; i != len; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
traits::deallocate(alloc, ptr, cap);
}
ptr= new_ptr;
for(size_type i= len; i != sz; ++i) traits::construct(alloc, ptr + i);
len= sz, cap= sz;
} else if(len < sz) {
for(size_type i= len; i != sz; ++i) traits::construct(alloc, ptr + i);
len= sz;
} else {
for(size_type i= sz; i != len; ++i) traits::destroy(alloc, ptr + i);
len= sz;
}
}
constexpr void resize(const size_type sz, const value_type& c) {
if(cap < sz) {
const pointer new_ptr= traits::allocate(sz);
if(cap != 0) {
for(size_type i= 0; i != len; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
traits::deallocate(alloc, ptr, cap);
}
ptr= new_ptr;
for(size_type i= len; i != sz; ++i) traits::construct(alloc, *(ptr + i), c);
len= sz, cap= sz;
} else if(len < sz) {
for(size_type i= len; i != sz; ++i) traits::construct(alloc, *(ptr + i), c);
len= sz;
} else {
for(size_type i= sz; i != len; ++i) traits::destroy(alloc, ptr + i);
len= sz;
}
}
constexpr size_type capacity() const noexcept { return cap; }
[[nodiscard]] constexpr bool empty() const noexcept { return len == 0; }
constexpr void reserve(const size_type n) {
if(n > cap) {
const pointer new_ptr= traits::allocate(alloc, n);
if(cap != 0) {
for(size_type i= 0; i != len; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
traits::deallocate(alloc, ptr, cap);
}
ptr= new_ptr, cap= n;
}
}
constexpr void shrink_to_fit() {
if(len == 0) {
if(cap != 0) traits::deallocate(alloc, ptr, cap);
ptr= nullptr, cap= 0;
return;
}
if(len != cap) {
const pointer new_ptr= traits::allocate(alloc, len);
for(size_type i= 0; i != len; ++i) traits::construct(alloc, new_ptr + i, std::move(*(ptr + i)));
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
traits::deallocate(alloc, ptr, cap);
ptr= new_ptr, cap= len;
}
}
GSH_INTERNAL_INLINE constexpr reference operator[](const size_type n) {
#ifndef NDEBUG
if(n >= len) [[unlikely]]
throw gsh::Exception("gsh::Vec::operator[] / The index is out of range. ( n=", n, ", size=", len, " )");
#endif
Assume(n < len);
return *(ptr + n);
}
GSH_INTERNAL_INLINE constexpr const_reference operator[](const size_type n) const {
#ifndef NDEBUG
if(n >= len) [[unlikely]]
throw gsh::Exception("gsh::Vec::operator[] / The index is out of range. ( n=", n, ", size=", len, " )");
#endif
Assume(n < len);
return *(ptr + n);
}
GSH_INTERNAL_INLINE constexpr reference at(const size_type n) {
if(n >= len) [[unlikely]]
throw gsh::Exception("gsh::Vec::at / The index is out of range. ( n=", n, ", size=", len, " )");
return *(ptr + n);
}
GSH_INTERNAL_INLINE constexpr const_reference at(const size_type n) const {
if(n >= len) [[unlikely]]
throw gsh::Exception("gsh::Vec::at / The index is out of range. ( n=", n, ", size=", len, " )");
return *(ptr + n);
}
GSH_INTERNAL_INLINE constexpr reference at_unchecked(const size_type n) noexcept {
Assume(n < len);
return *(ptr + n);
}
GSH_INTERNAL_INLINE constexpr const_reference at_unchecked(const size_type n) const noexcept {
Assume(n < len);
return *(ptr + n);
}
constexpr pointer data() noexcept { return ptr; }
constexpr const_pointer data() const noexcept { return ptr; }
constexpr reference front() noexcept { return *ptr; }
constexpr const_reference front() const noexcept { return *ptr; }
constexpr reference back() noexcept { return *(ptr + len - 1); }
constexpr const_reference back() const noexcept { return *(ptr + len - 1); }
template<std::ranges::range R> constexpr void assign(R&& r) {
const size_type n= std::ranges::size(r);
if(n > cap) {
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
traits::deallocate(alloc, ptr, cap);
ptr= traits::allocate(alloc, n);
cap= n;
auto itr= std::ranges::begin(r);
for(size_type i= 0; i != n; ++itr, ++i) traits::construct(alloc, ptr + i, *itr);
} else if(n > len) {
size_type i= 0;
auto itr= std::ranges::begin(r);
for(; i != len; ++itr, ++i) *(ptr + i)= *itr;
for(; i != n; ++itr, ++i) traits::construct(alloc, ptr + i, *itr);
} else {
for(size_type i= n; i != len; ++i) traits::destroy(alloc, ptr + i);
auto itr= std::ranges::begin(r);
for(size_type i= 0; i != n; ++itr, ++i) *(ptr + i)= *itr;
}
len= n;
}
constexpr void assign(const size_type n, const value_type& t) {
if(n > cap) {
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
traits::deallocate(alloc, ptr, cap);
ptr= traits::allocate(alloc, n);
cap= n;
for(size_type i= 0; i != n; ++i) traits::construct(alloc, ptr + i, t);
} else if(n > len) {
size_type i= 0;
for(; i != len; ++i) *(ptr + i)= t;
for(; i != n; ++i) traits::construct(alloc, ptr + i, t);
} else {
for(size_type i= n; i != len; ++i) traits::destroy(alloc, ptr + i);
for(size_type i= 0; i != n; ++i) *(ptr + i)= t;
}
len= n;
}
constexpr void assign(std::initializer_list<value_type> il) { assign(il.begin(), il.end()); }
private:
constexpr void extend_one() {
if(len == cap) {
const pointer new_ptr= traits::allocate(alloc, cap * 2 + 8);
if(cap != 0) {
for(size_type i= 0; i != len; ++i) traits::construct(alloc, new_ptr + i, std::move_if_noexcept(*(ptr + i)));
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
traits::deallocate(alloc, ptr, cap);
}
ptr= new_ptr, cap= cap * 2 + 8;
}
}
public:
constexpr void push_back(const T& x) {
extend_one();
traits::construct(alloc, ptr + (len++), x);
}
constexpr void push_back(T&& x) {
extend_one();
traits::construct(alloc, ptr + (len++), std::move(x));
}
template<class... Args> constexpr reference emplace_back(Args&&... args) {
extend_one();
traits::construct(alloc, ptr + len, std::forward<Args>(args)...);
return *(ptr + (len++));
}
constexpr void pop_back() {
#ifndef NDEBUG
if(len == 0) [[unlikely]]
throw gsh::Exception("gsh::Vec::pop_back / The container is empty.");
#endif
traits::destroy(alloc, ptr + (--len));
}
/*
    constexpr iterator insert(const const_iterator position, const value_type& x);
    constexpr iterator insert(const const_iterator position, value_type&& x);
    constexpr iterator insert(const const_iterator position, const size_type n, const value_type& x);
    template<class InputIter> constexpr iterator insert(const const_iterator position, const InputIter first, const InputIter last);
    constexpr iterator insert(const const_iterator position, const std::initializer_list<value_type> il);
    template<class... Args> constexpr iterator emplace(const_iterator position, Args&&... args);
    constexpr iterator erase(const_iterator position);
    constexpr iterator erase(const_iterator first, const_iterator last);
    */
constexpr void swap(Vec& x) noexcept(traits::propagate_on_container_swap::value || traits::is_always_equal::value) {
using std::swap;
swap(ptr, x.ptr);
swap(len, x.len);
swap(cap, x.cap);
if constexpr(traits::propagate_on_container_swap::value) swap(alloc, x.alloc);
}
constexpr void clear() {
for(size_type i= 0; i != len; ++i) traits::destroy(alloc, ptr + i);
len= 0;
}
constexpr void reset() {
if(cap != 0) {
traits::deallocate(alloc, ptr, cap);
ptr= nullptr, len= 0, cap= 0;
}
}
constexpr allocator_type get_allocator() const noexcept { return alloc; }
friend constexpr bool operator==(const Vec& x, const Vec& y) {
if(x.len != y.len) return false;
bool res= true;
for(size_type i= 0; i != x.len;) {
const bool f= *(x.ptr + i) == *(y.ptr + i);
res&= f;
i= f ? i + 1 : x.len;
}
return res;
}
friend constexpr auto operator<=>(const Vec& x, const Vec& y) { return std::lexicographical_compare_three_way(x.begin(), x.end(), y.begin(), y.end()); }
friend constexpr void swap(Vec& x, Vec& y) noexcept(noexcept(x.swap(y))) { x.swap(y); }
};
template<std::input_iterator InputIter, class Alloc= Allocator<typename std::iterator_traits<InputIter>::value_type>> Vec(InputIter, InputIter, Alloc= Alloc()) -> Vec<typename std::iterator_traits<InputIter>::value_type, Alloc>;
template<class T, class Alloc= Allocator<T>> using Vec2= Vec<Vec<T, Alloc>, typename AllocatorTraits<Alloc>::template rebind_alloc<Vec<T, Alloc>>>;
template<class T, class Alloc= Allocator<T>> using Vec3= Vec<Vec<Vec<T, Alloc>, typename AllocatorTraits<Alloc>::template rebind_alloc<Vec<T, Alloc>>>, typename AllocatorTraits<Alloc>::template rebind_alloc<Vec<Vec<T, Alloc>, typename AllocatorTraits<Alloc>::template rebind_alloc<Vec<T, Alloc>>>>>;
}// namespace gsh
namespace gsh {
template<class T, class U> constexpr std::common_type_t<T, U> Min(const T& a, const U& b) {
return a < b ? a : b;
}
template<class T, class... Args>
requires(sizeof...(Args) >= 2)
constexpr auto Min(const T& x, const Args&... args) {
return Min(x, Min(args...));
}
template<class T, class U> constexpr std::common_type_t<T, U> Max(const T& a, const U& b) {
return a < b ? b : a;
}
template<class T, class... Args>
requires(sizeof...(Args) >= 2)
constexpr auto Max(const T& x, const Args&... args) {
return Max(x, Max(args...));
}
template<class T, class U> constexpr bool Chmin(T& a, const U& b) {
const bool f= b < a;
a= f ? b : a;
return f;
}
template<class T, class... Args>
requires(sizeof...(Args) >= 2)
constexpr bool Chmin(T& a, const Args&... b) {
return Chmin(a, Min(b...));
}
template<class T, class U> constexpr bool Chmax(T& a, const U& b) {
const bool f= a < b;
a= f ? b : a;
return f;
}
template<class T, class... Args>
requires(sizeof...(Args) >= 2)
constexpr bool Chmax(T& a, const Args&... b) {
return Chmax(a, Max(b...));
}
namespace internal {
template<class R, class Comp, class Proj> constexpr auto MinImpl(R&& r, Comp&& comp, Proj&& proj) {
auto first= std::ranges::begin(r);
auto last= std::ranges::end(r);
if(first == last) throw Exception("gsh::Min / The input is empty.");
auto res= *(first++);
for(; first != last; ++first) {
const bool f= Invoke(comp, Invoke(proj, static_cast<const decltype(res)&>(*first)), Invoke(proj, res));
if constexpr(std::is_trivial_v<decltype(res)>) res= f ? *first : res;
else if(f) res= *first;
}
return res;
}
template<class R, class Comp, class Proj> constexpr auto MaxImpl(R&& r, Comp&& comp, Proj&& proj) {
if constexpr(std::same_as<std::remove_cvref_t<Comp>, Less>) return MinImpl(std::forward<R>(r), Greater(), proj);
else if constexpr(std::same_as<std::remove_cvref_t<Comp>, Greater>) return MinImpl(std::forward<R>(r), Less(), proj);
else return MinImpl(std::forward<R>(r), SwapArgs(std::forward<Comp>(comp)), proj);
}
template<class R, class T, class F> constexpr auto FoldImpl(R&& r, T init, F&& f) {
for(auto&& x: std::forward<R>(r)) init= Invoke(f, std::move(init), std::forward<decltype(x)>(x));
return init;
}
template<class R, class F> constexpr auto SumImpl(const R& r, F&& f) {
auto itr= std::ranges::begin(r);
auto sent= std::ranges::end(r);
auto res= *itr;
for(; itr != sent; ++itr) res= Invoke(f, std::move(res), *itr);
return res;
}
template<class R, class F>
requires(!std::ranges::borrowed_range<R>)
constexpr auto SumImpl(R&& r, F&& f) {
auto itr= std::ranges::begin(r);
auto sent= std::ranges::end(r);
auto res= std::move(*itr);
for(; itr != sent; ++itr) res= Invoke(f, std::move(res), std::move(*itr));
return res;
}
template<class R> constexpr void ReverseImpl(R&& r) {
std::ranges::reverse(std::forward<R>(r));
}
template<class T, class Proj= Identity> void SortUnsigned8(T* const p, const itype::u32 n, Proj&& proj= {}) {
static itype::u32 cnt[1 << 8];
std::memset(cnt, 0, sizeof(cnt));
for(itype::u32 i= 0; i != n; ++i) ++cnt[Invoke(proj, p[i]) & 0xff];
for(itype::u32 i= 0; i != (1 << 8) - 1; ++i) cnt[i + 1]+= cnt[i];
Arr<T> tmp(ArrNoInit, n);
for(itype::u32 i= n; i--;) std::construct_at(&tmp[--cnt[Invoke(proj, p[i]) & 0xffff]], std::move(p[i]));
for(itype::u32 i= 0; i != n; ++i) p[i]= std::move(tmp[i]);
}
template<class T, class Proj= Identity> void SortUnsigned16(T* const p, const itype::u32 n, Proj&& proj= {}) {
static itype::u32 cnt[1 << 16];
std::memset(cnt, 0, sizeof(cnt));
for(itype::u32 i= 0; i != n; ++i) ++cnt[Invoke(proj, p[i]) & 0xffff];
for(itype::u32 i= 0; i != (1 << 16) - 1; ++i) cnt[i + 1]+= cnt[i];
Arr<T> tmp(ArrNoInit, n);
for(itype::u32 i= n; i--;) std::construct_at(&tmp[--cnt[Invoke(proj, p[i]) & 0xffff]], std::move(p[i]));
for(itype::u32 i= 0; i != n; ++i) p[i]= std::move(tmp[i]);
}
template<class T, class Proj= Identity> void SortUnsigned32(T* const p, const itype::u32 n, Proj&& proj= {}) {
static itype::u32 cnt1[1 << 16], cnt2[1 << 16];
std::memset(cnt1, 0, sizeof(cnt1));
std::memset(cnt2, 0, sizeof(cnt2));
for(itype::u32 i= 0; i != n; ++i) {
auto tmp= Invoke(proj, p[i]);
const itype::u16 a= tmp & 0xffff;
const itype::u16 b= tmp >> 16 & 0xffff;
++cnt1[a];
++cnt2[b];
}
for(itype::u32 i= 0; i != (1 << 16) - 1; ++i) {
cnt1[i + 1]+= cnt1[i];
cnt2[i + 1]+= cnt2[i];
}
Arr<T> tmp(ArrNoInit, n);
for(itype::u32 i= n; i--;) std::construct_at(&tmp[--cnt1[Invoke(proj, p[i]) & 0xffff]], std::move(p[i]));
for(itype::u32 i= n; i--;) p[--cnt2[Invoke(proj, tmp[i]) >> 16 & 0xffff]]= std::move(tmp[i]);
}
template<class T, class Proj= Identity> void SortUnsigned64(T* const p, const itype::u32 n, Proj&& proj= {}) {
static itype::u32 cnt1[1 << 16], cnt2[1 << 16], cnt3[1 << 16], cnt4[1 << 16];
std::memset(cnt1, 0, sizeof(cnt1));
std::memset(cnt2, 0, sizeof(cnt2));
std::memset(cnt3, 0, sizeof(cnt3));
std::memset(cnt4, 0, sizeof(cnt4));
for(itype::u32 i= 0; i != n; ++i) {
auto tmp= Invoke(proj, p[i]);
const itype::u16 a= tmp & 0xffff;
const itype::u16 b= tmp >> 16 & 0xffff;
const itype::u16 c= tmp >> 32 & 0xffff;
const itype::u16 d= tmp >> 48 & 0xffff;
++cnt1[a];
++cnt2[b];
++cnt3[c];
++cnt4[d];
}
for(itype::u32 i= 0; i != (1 << 16) - 1; ++i) {
cnt1[i + 1]+= cnt1[i];
cnt2[i + 1]+= cnt2[i];
cnt3[i + 1]+= cnt3[i];
cnt4[i + 1]+= cnt4[i];
}
Arr<T> tmp(ArrNoInit, n);
for(itype::u32 i= n; i--;) std::construct_at(&tmp[--cnt1[Invoke(proj, p[i]) & 0xffff]], std::move(p[i]));
for(itype::u32 i= n; i--;) p[--cnt2[Invoke(proj, tmp[i]) >> 16 & 0xffff]]= std::move(tmp[i]);
for(itype::u32 i= n; i--;) tmp[--cnt3[Invoke(proj, p[i]) >> 32 & 0xffff]]= std::move(p[i]);
for(itype::u32 i= n; i--;) p[--cnt4[Invoke(proj, tmp[i]) >> 48 & 0xffff]]= std::move(tmp[i]);
}
struct Revmsb {
template<class T> constexpr auto operator()(T x) const {
using result_type= std::make_unsigned_t<T>;
return std::bit_cast<result_type>(x) ^ (result_type(1) << (sizeof(T) * 8 - 1));
}
};
struct ToUnsigned {
template<class T> constexpr auto operator()(T x) const {
if constexpr(std::same_as<T, ftype::f16>) {
itype::u16 y= std::bit_cast<itype::u16>(x);
return itype::u16(y ^ ((y >> 15) != 0 ? ~itype::u16(0) : (itype::u16(1) << 15)));
} else if constexpr(std::same_as<T, ftype::f32>) {
itype::u32 y= std::bit_cast<itype::u32>(x);
return itype::u32(y ^ ((y >> 31) != 0 ? ~itype::u32(0) : (itype::u32(1) << 31)));
} else if constexpr(std::same_as<T, ftype::f64>) {
itype::u64 y= std::bit_cast<itype::u64>(x);
return itype::u64(y ^ ((y >> 63) != 0 ? ~itype::u64(0) : (itype::u64(1) << 63)));
} else if constexpr(std::same_as<T, ftype::f128>) {
itype::u128 y= std::bit_cast<itype::u128>(x);
return itype::u128(y ^ ((y >> 127) != 0 ? ~itype::u128(0) : (itype::u128(1) << 127)));
}
}
};
template<class T, class Comp, class Proj>
requires std::is_scalar_v<T>
GSH_INTERNAL_INLINE constexpr void CompSwap(T* const p, itype::u32 a, itype::u32 b, Comp&& comp= {}, Proj&& proj= {}) {
bool f= Invoke(comp, Invoke(proj, p[a]), Invoke(proj, p[b]));
const auto tmp_a= p[a], tmp_b= p[b];
p[a]= f ? tmp_a : tmp_b;
p[b]= f ? tmp_b : tmp_a;
}
template<class T, class Comp, class Proj> GSH_INTERNAL_NOINLINE constexpr void CompSwap(T* const p, itype::u32 a, itype::u32 b, Comp&& comp= {}, Proj&& proj= {}) {
using std::swap;
if(!Invoke(comp, Invoke(proj, p[a]), Invoke(proj, p[b]))) swap(p[a], p[b]);
}
template<itype::u32 N, class T, class Comp= Less, class Proj= Identity> GSH_INTERNAL_NOINLINE constexpr void SortBlock(T* const p, itype::u32 len, Comp&& comp= {}, Proj&& proj= {}) {
if constexpr(N <= 8) Assume(len == 1);
for(itype::u32 i= 0; i != len; ++i) {
T* const q= p + i * N;
// clang-format off
#ifdef F
#define GSH_INTERNAL_DEFINED_MACRO_F
#pragma push_macro("F")
#undef F
#endif
#define F(A,B) CompSwap(q,A,B,comp,proj);
if constexpr(N==2){F(0,1)}
if constexpr(N==3){F(0,2)F(0,1)F(1,2)}
if constexpr(N==4){F(0,2)F(1,3)F(0,1)F(2,3)F(1,2)}
if constexpr(N==5){F(0,3)F(1,4)F(0,2)F(1,3)F(0,1)F(2,4)F(1,2)F(3,4)F(2,3)}
if constexpr(N==6){F(0,5)F(1,3)F(2,4)F(1,2)F(3,4)F(0,3)F(2,5)F(0,1)F(2,3)F(4,5)F(1,2)F(3,4)}
if constexpr(N==7){F(0,6)F(2,3)F(4,5)F(0,2)F(1,4)F(3,6)F(0,1)F(2,5)F(3,4)F(1,2)F(4,6)F(2,3)F(4,5)F(1,2)F(3,4)F(5,6)}
if constexpr(N==8){F(0,2)F(1,3)F(4,6)F(5,7)F(0,4)F(1,5)F(2,6)F(3,7)F(0,1)F(2,3)F(4,5)F(6,7)F(2,4)F(3,5)F(1,4)F(3,6)F(1,2)F(3,4)F(5,6)}
if constexpr(N==9){F(0,3)F(1,7)F(2,5)F(4,8)F(0,7)F(2,4)F(3,8)F(5,6)F(0,2)F(1,3)F(4,5)F(7,8)F(1,4)F(3,6)F(5,7)F(0,1)F(2,4)F(3,5)F(6,8)F(2,3)F(4,5)F(6,7)F(1,2)F(3,4)F(5,6)}
if constexpr(N==10){F(0,8)F(1,9)F(2,7)F(3,5)F(4,6)F(0,2)F(1,4)F(5,8)F(7,9)F(0,3)F(2,4)F(5,7)F(6,9)F(0,1)F(3,6)F(8,9)F(1,5)F(2,3)F(4,8)F(6,7)F(1,2)F(3,5)F(4,6)F(7,8)F(2,3)F(4,5)F(6,7)F(3,4)F(5,6)}
if constexpr(N==11){F(0,9)F(1,6)F(2,4)F(3,7)F(5,8)F(0,1)F(3,5)F(4,10)F(6,9)F(7,8)F(1,3)F(2,5)F(4,7)F(8,10)F(0,4)F(1,2)F(3,7)F(5,9)F(6,8)F(0,1)F(2,6)F(4,5)F(7,8)F(9,10)F(2,4)F(3,6)F(5,7)F(8,9)F(1,2)F(3,4)F(5,6)F(7,8)F(2,3)F(4,5)F(6,7)}
if constexpr(N==12){F(0,8)F(1,7)F(2,6)F(3,11)F(4,10)F(5,9)F(0,1)F(2,5)F(3,4)F(6,9)F(7,8)F(10,11)F(0,2)F(1,6)F(5,10)F(9,11)F(0,3)F(1,2)F(4,6)F(5,7)F(8,11)F(9,10)F(1,4)F(3,5)F(6,8)F(7,10)F(1,3)F(2,5)F(6,9)F(8,10)F(2,3)F(4,5)F(6,7)F(8,9)F(4,6)F(5,7)F(3,4)F(5,6)F(7,8)}
if constexpr(N==13){F(0,12)F(1,10)F(2,9)F(3,7)F(5,11)F(6,8)F(1,6)F(2,3)F(4,11)F(7,9)F(8,10)F(0,4)F(1,2)F(3,6)F(7,8)F(9,10)F(11,12)F(4,6)F(5,9)F(8,11)F(10,12)F(0,5)F(3,8)F(4,7)F(6,11)F(9,10)F(0,1)F(2,5)F(6,9)F(7,8)F(10,11)F(1,3)F(2,4)F(5,6)F(9,10)F(1,2)F(3,4)F(5,7)F(6,8)F(2,3)F(4,5)F(6,7)F(8,9)F(3,4)F(5,6)}
if constexpr(N==14){F(0,1)F(2,3)F(4,5)F(6,7)F(8,9)F(10,11)F(12,13)F(0,2)F(1,3)F(4,8)F(5,9)F(10,12)F(11,13)F(0,4)F(1,2)F(3,7)F(5,8)F(6,10)F(9,13)F(11,12)F(0,6)F(1,5)F(3,9)F(4,10)F(7,13)F(8,12)F(2,10)F(3,11)F(4,6)F(7,9)F(1,3)F(2,8)F(5,11)F(6,7)F(10,12)F(1,4)F(2,6)F(3,5)F(7,11)F(8,10)F(9,12)F(2,4)F(3,6)F(5,8)F(7,10)F(9,11)F(3,4)F(5,6)F(7,8)F(9,10)F(6,7)}
if constexpr(N==15){F(1,2)F(3,10)F(4,14)F(5,8)F(6,13)F(7,12)F(9,11)F(0,14)F(1,5)F(2,8)F(3,7)F(6,9)F(10,12)F(11,13)F(0,7)F(1,6)F(2,9)F(4,10)F(5,11)F(8,13)F(12,14)F(0,6)F(2,4)F(3,5)F(7,11)F(8,10)F(9,12)F(13,14)F(0,3)F(1,2)F(4,7)F(5,9)F(6,8)F(10,11)F(12,13)F(0,1)F(2,3)F(4,6)F(7,9)F(10,12)F(11,13)F(1,2)F(3,5)F(8,10)F(11,12)F(3,4)F(5,6)F(7,8)F(9,10)F(2,3)F(4,5)F(6,7)F(8,9)F(10,11)F(5,6)F(7,8)}
if constexpr(N==16){F(0,13)F(1,12)F(2,15)F(3,14)F(4,8)F(5,6)F(7,11)F(9,10)F(0,5)F(1,7)F(2,9)F(3,4)F(6,13)F(8,14)F(10,15)F(11,12)F(0,1)F(2,3)F(4,5)F(6,8)F(7,9)F(10,11)F(12,13)F(14,15)F(0,2)F(1,3)F(4,10)F(5,11)F(6,7)F(8,9)F(12,14)F(13,15)F(1,2)F(3,12)F(4,6)F(5,7)F(8,10)F(9,11)F(13,14)F(1,4)F(2,6)F(5,8)F(7,10)F(9,13)F(11,14)F(2,4)F(3,6)F(9,12)F(11,13)F(3,5)F(6,8)F(7,9)F(10,12)F(3,4)F(5,6)F(7,8)F(9,10)F(11,12)F(6,7)F(8,9)}
// clang-format on
static_assert(N <= 16);
#undef F
#ifdef GSH_INTERNAL_DEFINED_MACRO_F
#undef GSH_INTERNAL_DEFINED_MACRO_F
#pragma pop_macro("F")
#endif
}
}
template<class R, class Comp, class Proj> constexpr void SortImpl(R&& r, Comp&& comp, Proj&& proj) {
if constexpr(!requires { std::ranges::data(r); }) {
Arr tmp(std::move_iterator(std::ranges::begin(r)), std::move_sentinel(std::ranges::end(r)));
Sort(tmp, std::forward<Comp>(comp), std::forward<Proj>(proj));
for(itype::u32 i= 0; auto&& el: r) el= std::move(tmp[i++]);
return;
} else {
const itype::u32 n= std::ranges::size(r);
auto* const p= std::ranges::data(r);
using value_type= std::remove_cvref_t<decltype(*p)>;
if(!std::is_constant_evaluated()) {
constexpr bool is_less= std::same_as<std::remove_cvref_t<Comp>, Less>;
constexpr bool is_greater= std::same_as<std::remove_cvref_t<Comp>, Greater>;
if constexpr((is_less || is_greater) && std::is_nothrow_move_constructible_v<value_type>) {
using inv_result= std::remove_cvref_t<std::invoke_result_t<Proj, std::ranges::range_value_t<R>>>;
if constexpr(std::same_as<inv_result, itype::u8> || std::same_as<inv_result, itype::i8> || std::same_as<inv_result, ctype::c8>) {
if(n >= 33) {
if constexpr(std::is_unsigned_v<inv_result>) internal::SortUnsigned8(p, n, std::forward<Proj>(proj));
else internal::SortUnsigned8(p, n, BindFront<Proj, internal::Revmsb>(std::forward<Proj>(proj), internal::Revmsb()));
if constexpr(is_greater) Reverse(std::forward<R>(r));
return;
}
}
if constexpr(std::same_as<inv_result, itype::u16> || std::same_as<inv_result, itype::i16>) {
if(n >= 1200) {
if constexpr(std::is_unsigned_v<inv_result>) internal::SortUnsigned16(p, n, std::forward<Proj>(proj));
else internal::SortUnsigned16(p, n, BindFront<Proj, internal::Revmsb>(std::forward<Proj>(proj), internal::Revmsb()));
if constexpr(is_greater) Reverse(std::forward<R>(r));
return;
}
}
if constexpr(std::same_as<inv_result, itype::u32> || std::same_as<inv_result, itype::i32>) {
if(n >= 3000) {
if constexpr(std::is_unsigned_v<inv_result>) internal::SortUnsigned32(p, n, std::forward<Proj>(proj));
else internal::SortUnsigned32(p, n, BindFront<Proj, internal::Revmsb>(std::forward<Proj>(proj), internal::Revmsb()));
if constexpr(is_greater) Reverse(std::forward<R>(r));
return;
}
}
if constexpr(std::same_as<inv_result, itype::u64> || std::same_as<inv_result, itype::i64>) {
if(n >= 8000) {
if constexpr(std::is_unsigned_v<inv_result>) internal::SortUnsigned64(p, n, std::forward<Proj>(proj));
else internal::SortUnsigned64(p, n, BindFront<Proj, internal::Revmsb>(std::forward<Proj>(proj), internal::Revmsb()));
if constexpr(is_greater) Reverse(std::forward<R>(r));
return;
}
}
if constexpr(std::same_as<inv_result, ftype::f16>) {
if(n >= 1500) {
internal::SortUnsigned16(p, n, BindFront<Proj, internal::ToUnsigned>(std::forward<Proj>(proj), internal::ToUnsigned()));
if constexpr(is_greater) Reverse(std::forward<R>(r));
return;
}
}
if constexpr(std::same_as<inv_result, ftype::f32>) {
if(n >= 4000) {
internal::SortUnsigned32(p, n, BindFront<Proj, internal::ToUnsigned>(std::forward<Proj>(proj), internal::ToUnsigned()));
if constexpr(is_greater) Reverse(std::forward<R>(r));
return;
}
}
if constexpr(std::same_as<inv_result, ftype::f64>) {
if(n >= 10000) {
internal::SortUnsigned64(p, n, BindFront<Proj, internal::ToUnsigned>(std::forward<Proj>(proj), internal::ToUnsigned()));
if constexpr(is_greater) Reverse(std::forward<R>(r));
return;
}
}
}
}
//std::ranges::sort(std::forward<R>(r), std::forward<Comp>(comp), std::forward<Proj>(proj));
const itype::u32 minrun= n <= 16 ? 32 : ((n >> (std::bit_width(n) - 4)) + ((n & ((1ull << (std::bit_width(n) - 4)) - 1)) != 0)) << std::has_single_bit(n);
const itype::u32 rem= (n - 1) % minrun + 1, blocks= (n - 1) / minrun;
switch(rem) {
case 2: internal::SortBlock<2>(p, 1, comp, proj); break;
case 3: internal::SortBlock<3>(p, 1, comp, proj); break;
case 4: internal::SortBlock<4>(p, 1, comp, proj); break;
case 5: internal::SortBlock<5>(p, 1, comp, proj); break;
case 6: internal::SortBlock<6>(p, 1, comp, proj); break;
case 7: internal::SortBlock<7>(p, 1, comp, proj); break;
case 8: internal::SortBlock<8>(p, 1, comp, proj); break;
case 9: internal::SortBlock<9>(p, 1, comp, proj); break;
case 10: internal::SortBlock<10>(p, 1, comp, proj); break;
case 11: internal::SortBlock<11>(p, 1, comp, proj); break;
case 12: internal::SortBlock<12>(p, 1, comp, proj); break;
case 13: internal::SortBlock<13>(p, 1, comp, proj); break;
case 14: internal::SortBlock<14>(p, 1, comp, proj); break;
case 15: internal::SortBlock<15>(p, 1, comp, proj); break;
case 16: internal::SortBlock<16>(p, 1, comp, proj); break;
default: break;
}
if(minrun == 32) return;
switch(minrun) {
case 9: internal::SortBlock<9>(p + rem, blocks, comp, proj); break;
case 10: internal::SortBlock<10>(p + rem, blocks, comp, proj); break;
case 11: internal::SortBlock<11>(p + rem, blocks, comp, proj); break;
case 12: internal::SortBlock<12>(p + rem, blocks, comp, proj); break;
case 13: internal::SortBlock<13>(p + rem, blocks, comp, proj); break;
case 14: internal::SortBlock<14>(p + rem, blocks, comp, proj); break;
case 15: internal::SortBlock<15>(p + rem, blocks, comp, proj); break;
case 16: internal::SortBlock<16>(p + rem, blocks, comp, proj); break;
default: Unreachable();
}
std::unique_ptr<value_type[]> tmp(new value_type[n]);
auto merge_seq= [&](value_type* a, itype::u32 a_size, value_type* b, itype::u32 b_size, value_type* dst, auto construct) {
constexpr bool construct_v= std::same_as<decltype(construct), std::true_type>;
itype::u32 i= 0, j= 0, k= 0;
while(i != a_size && j != b_size) {
bool f= Invoke(comp, Invoke(proj, a[i]), Invoke(proj, b[j]));
if constexpr(construct_v) std::construct_at(&dst[k], std::move(f ? a[i] : b[j]));
else dst[k]= std::move(f ? a[i] : b[j]);
i+= f;
j+= !f;
k+= 1;
}
for(; i != a_size; ++i, ++k) {
if constexpr(construct_v) std::construct_at(&dst[k], std::move(a[i]));
else dst[k]= std::move(a[i]);
}
for(; j != b_size; ++j, ++k) {
if constexpr(construct_v) std::construct_at(&dst[k], std::move(b[j]));
else dst[k]= std::move(b[j]);
}
};
merge_seq(p, rem, p + rem, minrun, tmp.get(), std::true_type());
itype::u32 i= rem + minrun;
for(; i + 2 * minrun <= n; i+= 2 * minrun) {
merge_seq(p + i, minrun, p + i + minrun, minrun, tmp.get() + i, std::true_type());
}
for(; i < n; ++i) {
std::construct_at(tmp.get() + i, std::move(p[i]));
}
value_type *from= tmp.get(), *to= p;
itype::u32 run= 2 * minrun;
while(true) {
itype::u32 nrem= run - minrun + rem;
if(nrem + run >= n) {
merge_seq(from, nrem, from + nrem, n - nrem, to, std::false_type());
break;
}
merge_seq(from, nrem, from + nrem, run, to, std::false_type());
itype::u32 i= nrem + run;
for(; i + 2 * run <= n; i+= 2 * run) {
merge_seq(from + i, run, from + i + run, run, to + i, std::false_type());
}
if(i + run < n) merge_seq(from + i, run, from + i + run, n - (i + run), to + i, std::false_type());
else {
for(; i != n; ++i) to[i]= std::move(from[i]);
}
run*= 2;
std::swap(from, to);
}
if(to != p) {
for(itype::u32 i= 0; i != n; ++i) p[i]= std::move(tmp[i]);
}
}
}
template<class R, class Comp, class Proj> constexpr auto SortIndexImpl(R&& r, Comp&& comp, Proj&& proj) {
itype::u32 n= std::ranges::size(r);
Arr<itype::u32> res(n);
for(itype::u32 i= 0; i != n; ++i) res[i]= i;
SortImpl(res, std::forward<Comp>(comp), [start= std::ranges::begin(r), pj= std::forward<Proj>(proj)](itype::u32 n) { return Invoke(pj, *std::ranges::next(start, n)); });
return res;
}
}// namespace internal
template<std::ranges::forward_range R, class T, class Proj= Identity, std::indirect_strict_weak_order<const T*, std::projected<std::ranges::iterator_t<R>, Proj>> Comp= Less> constexpr auto LowerBound(R&& r, const T& value, Comp&& comp= {}, Proj&& proj= {}) {
auto st= std::ranges::begin(r);
for(auto len= std::ranges::size(r) + 1; len > 1;) {
auto half= len / 2;
len-= half;
auto tmp= std::ranges::next(st, half);
auto md= std::ranges::next(tmp, -1);
st= Invoke(comp, Invoke(proj, *md), value) ? tmp : st;
}
return st;
}
template<std::ranges::forward_range R, class T, class Proj= Identity, std::indirect_strict_weak_order<const T*, std::projected<std::ranges::iterator_t<R>, Proj>> Comp= Less> constexpr auto UpperBound(R&& r, const T& value, Comp&& comp= {}, Proj&& proj= {}) {
auto st= std::ranges::begin(r);
for(auto len= std::ranges::size(r) + 1; len > 1;) {
auto half= len / 2;
len-= half;
auto tmp= std::ranges::next(st, half);
auto md= std::ranges::next(tmp, -1);
st= !static_cast<bool>(Invoke(comp, value, Invoke(proj, *md))) ? tmp : st;
}
return st;
}
template<std::ranges::forward_range R, class Proj= Identity, class Comp= Less> constexpr Arr<itype::u32> LongestIncreasingSubsequence(R&& r, Comp&& comp= {}, Proj&& proj= {}) {
using T= std::ranges::range_value_t<R>;
Arr<itype::u32> idx(std::ranges::size(r));
itype::u32 len= 0;
{
Arr<T> dp(idx.size());
itype::u32 i= 0;
T *begin= dp.data(), *last= dp.data();
for(auto&& x: r) {
if(begin == last || Invoke(comp, *(last - 1), Invoke(proj, x))) [[unlikely]] {
idx[i++]= last - begin;
*last= x;
++last;
} else {
T* loc= LowerBound(Subrange{begin, last - 1}, x, comp, proj);
idx[i++]= loc - begin;
*loc= x;
}
}
len= last - begin;
}
itype::u32 cnt= len - 1;
Arr<itype::u32> res(len);
for(itype::u32 i= idx.size(); i--;) {
if(idx[i] == cnt) res[cnt--]= i;
}
return res;
}
template<std::ranges::forward_range R, class Proj= Identity, class Comp= Less> constexpr itype::u32 LongestIncreasingSubsequenceLength(R&& r, Comp&& comp= {}, Proj&& proj= {}) {
using T= std::ranges::range_value_t<R>;
Arr<T> dp(std::ranges::size(r));
T *begin= dp.data(), *last= dp.data();
for(auto&& x: r) {
if(begin == last || Invoke(comp, *(last - 1), Invoke(proj, x))) [[unlikely]] {
*last= x;
++last;
} else {
T* loc= LowerBound(Subrange{begin, last - 1}, x, comp, proj);
*loc= x;
}
}
return last - begin;
}
template<std::ranges::random_access_range R> constexpr Arr<itype::u32> LongestCommonPrefixArray(R&& r) {
const itype::u32 n= std::ranges::size(r);
Arr<itype::u32> res(n);
if(n == 0) return res;
res[0]= n;
const auto itr= std::ranges::begin(r);
itype::u32 i= 1, j= 0;
while(i != n) {
while(i + j < n && *std::ranges::next(itr, j) == *std::ranges::next(itr, i + j)) ++j;
res[i]= j;
if(j == 0) {
++i;
continue;
}
itype::u32 k= 1;
while(k < j && k + res[k] < j) ++k;
const auto a= res.data() + i + 1;
const auto b= res.data() + 1;
for(itype::u32 l= 0; l != k - 1; ++l) a[l]= b[l];
i+= k;
j-= k;
}
return res;
}
template<class T= itype::u64, std::ranges::range R>
requires std::same_as<std::ranges::range_value_t<R>, itype::u32>
constexpr T CountDistinctSubsequences(R&& r) {
const itype::u32 n= std::ranges::size(r);
if(n == 0) return 0;
Arr<itype::u64> s(n);
for(itype::u32 i= 0; itype::u32 x: r) {
s[i]= static_cast<itype::u64>(i) << 32 | x;
++i;
}
s.sort({}, [](itype::u64 x) { return static_cast<itype::u32>(x); });
Arr<itype::u32> rank(n);
rank[s[0] >> 32]= 0;
itype::u32 cnt= 0, end= s[0];
for(itype::u32 i= 1; i != n; ++i) {
cnt+= end != static_cast<itype::u32>(s[i]);
rank[s[i] >> 32]= cnt;
end= s[i];
}
Arr<itype::u32> last(cnt + 1, n);
Arr<T> dp(n + 1);
dp[0]= 1;
const T m= 2;
for(itype::u32 i= 0; i != n; ++i) {
dp[i + 1]= m * dp[i] - dp[last[rank[i]]];
last[rank[i]]= i;
}
return dp[n] - dp[0];
}
template<std::ranges::forward_range R> constexpr auto Majority(R&& r) {
itype::u32 c= 0;
itype::u32 len= 0;
auto i= std::ranges::begin(r);
auto j= std::ranges::end(r), k= j;
for(; i != j; ++i) {
++len;
if(c == 0) k= i, c= 1;
else c+= static_cast<itype::u32>(static_cast<bool>(*i == *k)) * 2 - 1;
}
c= 0;
for(i= std::ranges::begin(r); i != j; ++i) c+= static_cast<bool>(*i == *k);
return (2 * c >= len ? k : j);
}
class Mo {
struct rg {
itype::u32 l, r;
constexpr rg(itype::u32 a, itype::u32 b): l(a), r(b) {}
};
Vec<rg> qu;
public:
constexpr Mo() {}
constexpr void reserve(itype::u32 q) { qu.reserve(q); }
constexpr void query(itype::u32 l, itype::u32 r) { qu.emplace_back(l, r); }
template<class F1, class F2, class F3> void run(F1&& add, F2&& del, F3&& slv) const { solve(add, add, del, del, std::forward<F3>(slv)); }
template<class F1, class F2, class F3, class F4, class F5> void run(F1 addl, F2 addr, F3 dell, F4 delr, F5 slv) const {
const itype::u32 Q= qu.size();
itype::u32 N= 0;
for(itype::u32 i= 0; i != Q; ++i) N= N < qu[i].r ? qu[i].r : N;
itype::u32 width= 1.1 * std::sqrt(static_cast<ftype::f64>(3ull * N * N) / (2 * Q));
width+= width == 0;
Arr<itype::u32> cnt(N + 1), buf(Q), block(Q), idx(Q);
for(itype::u32 i= 0; i != Q; ++i) ++cnt[qu[i].r];
for(itype::u32 i= 0; i != N; ++i) cnt[i + 1]+= cnt[i];
for(itype::u32 i= 0; i != Q; ++i) buf[--cnt[qu[i].r]]= i;
cnt.assign(N / width + 2, 0);
for(itype::u32 i= 0; i != Q; ++i) block[i]= qu[i].l / width;
for(itype::u32 i= 0; i != Q; ++i) ++cnt[block[i]];
for(itype::u32 i= 0; i != cnt.size() - 1; ++i) cnt[i + 1]+= cnt[i];
for(itype::u32 i= 0; i != Q; ++i) idx[--cnt[block[buf[i]]]]= buf[i];
for(itype::u32 i= 0; i < cnt.size() - 1; i+= 2) {
const itype::u32 l= cnt[i], r= cnt[i + 1];
for(itype::u32 j= 0; j != (r - l) / 2; ++j) {
const itype::u32 t= idx[l + j];
idx[l + j]= idx[r - j - 1], idx[r - j - 1]= t;
}
}
itype::u32 nl= 0, nr= 0;
for(itype::u32 i: idx) {
while(nl > qu[i].l) Invoke(addl, --nl);
while(nr < qu[i].r) Invoke(addr, nr++);
while(nl < qu[i].l) Invoke(dell, nl++);
while(nr > qu[i].r) Invoke(delr, --nr);
Invoke(slv, i);
}
}
};
}// namespace gsh
#if __has_include(<source_location>)
#endif
namespace gsh {
namespace internal {
constexpr itype::u64 Splitmix(itype::u64 x) {
itype::u64 z= (x + 0x9e3779b97f4a7c15);
z= (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
z= (z ^ (z >> 27)) * 0x94d049bb133111eb;
return z ^ (z >> 31);
}
}// namespace internal
// @brief 64bit pseudo random number generator using xoroshiro128+
class Rand64 {
itype::u64 s0, s1;
public:
using result_type= itype::u64;
static constexpr itype::u32 word_size= sizeof(result_type) * 8;
static constexpr result_type default_seed= 0xcafef00dd15ea5e5;
constexpr Rand64(): Rand64(default_seed) {}
constexpr explicit Rand64(result_type value): s0(value), s1(internal::Splitmix(value)) {}
constexpr result_type operator()() {
itype::u64 t0= s0, t1= s1;
const itype::u64 res= t0 + t1;
t1^= t0;
s0= std::rotr(t0, 9) ^ t1 ^ (t1 << 14);
s1= std::rotr(t1, 28);
return res;
};
constexpr void discard(itype::u64 z) {
for(itype::u64 i= 0; i < z; ++i) operator()();
}
static constexpr result_type max() { return 18446744073709551615u; }
static constexpr result_type min() { return 0; }
constexpr void seed(result_type value= default_seed) { s0= value, s1= internal::Splitmix(value); }
friend constexpr bool operator==(Rand64 x, Rand64 y) { return x.s0 == y.s0 && x.s1 == y.s1; }
};
// @brief 32bit pseudo random number generator using Permuted congruential generator
class Rand32 {
itype::u64 val;
public:
using result_type= itype::u32;
static constexpr itype::u32 word_size= sizeof(result_type) * 8;
static constexpr result_type default_seed= 0xcafef00d;
constexpr Rand32(): Rand32(default_seed) {}
constexpr explicit Rand32(result_type value): val(internal::Splitmix((itype::u64)value << 32 | value)) {}
constexpr result_type operator()() {
itype::u64 x= val;
const itype::i32 count= x >> 61;
val= x * 0xcafef00dd15ea5e5;
x^= x >> 22;
return x >> (22 + count);
};
constexpr void discard(itype::u64 z) {
itype::u64 pow= 0xcafef00dd15ea5e5;
while(z != 0) {
if(z & 1) val*= pow;
z>>= 1;
pow*= pow;
}
}
static constexpr result_type max() { return 4294967295u; }
static constexpr result_type min() { return 0; }
constexpr void seed(result_type value= default_seed) { val= internal::Splitmix((itype::u64)value << 32 | value); }
friend constexpr bool operator==(Rand32 x, Rand32 y) { return x.val == y.val; }
};
// @brief Generate random numbers from std::time, std::clock, and std::source_location.
#ifdef __cpp_lib_source_location
class RandomDevice {
Rand64 engine;
constexpr itype::u64 from_time() {
if(!std::is_constant_evaluated()) {
itype::u64 a= internal::Splitmix(static_cast<itype::u64>(std::time(nullptr)));
itype::u64 b= Hash{}(internal::Splitmix(static_cast<itype::u64>(std::clock())));
return a ^ b;
} else return 0x9e3779b97f4a7c15;
}
constexpr itype::u64 from_compile_time() {
itype::u64 a= internal::Splitmix(Hash{}(internal::HashBytes(__DATE__)));
itype::u64 b= Hash{}(internal::Splitmix(internal::HashBytes(__TIME__)));
itype::u64 c= internal::Splitmix(internal::Splitmix(internal::HashBytes(__TIMESTAMP__)));
return internal::Splitmix(internal::MixIntegers(a, c)) ^ b;
}
constexpr itype::u64 from_location(const std::source_location& loc) {
itype::u64 a= Hash{}(internal::Splitmix(loc.column()));
itype::u64 b= internal::Splitmix(loc.line());
itype::u64 c= Hash{}(internal::HashBytes(loc.file_name()));
itype::u64 d= internal::HashBytes(loc.function_name());
return internal::MixIntegers(a, d) ^ internal::Splitmix(internal::MixIntegers(b, c));
}
constexpr itype::u64 get_val(const std::source_location& loc) { return internal::Splitmix(from_time()) ^ from_location(loc) ^ (Hash{}(from_compile_time())); }
public:
using result_type= itype::u64;
constexpr RandomDevice(const std::source_location& loc= std::source_location::current()): engine(internal::Splitmix(get_val(loc))) {}
constexpr RandomDevice(const RandomDevice&)= default;
constexpr RandomDevice& operator=(const RandomDevice&)= default;
constexpr ftype::f64 entropy() const noexcept { return 0.0; }
static constexpr result_type max() { return 0xffffffffffffffff; }
static constexpr result_type min() { return 0; }
constexpr result_type operator()(const std::source_location& loc= std::source_location::current()) { return engine() ^ get_val(loc); }
};
#else
class RandomDevice {
Rand64 engine;
constexpr itype::u64 from_time() {
if(!std::is_constant_evaluated()) {
itype::u64 a= internal::Splitmix(static_cast<itype::u64>(std::time(nullptr)));
itype::u64 b= Hash{}(internal::Splitmix(static_cast<itype::u64>(std::clock())));
return a ^ b;
} else return 0x9e3779b97f4a7c15;
}
constexpr itype::u64 from_compile_time() {
itype::u64 a= internal::Splitmix(Hash{}(internal::HashBytes(__DATE__)));
itype::u64 b= Hash{}(internal::Splitmix(internal::HashBytes(__TIME__)));
itype::u64 c= internal::Splitmix(internal::Splitmix(internal::HashBytes(__TIMESTAMP__)));
return internal::Splitmix(internal::MixIntegers(a, c)) ^ b;
}
constexpr itype::u64 get_val() { return internal::Splitmix(from_time()) ^ (Hash{}(from_compile_time())); }
public:
using result_type= itype::u64;
constexpr RandomDevice(): engine(internal::Splitmix(get_val())) {}
constexpr RandomDevice(const RandomDevice&)= default;
constexpr RandomDevice& operator=(const RandomDevice&)= default;
constexpr ftype::f64 entropy() const noexcept { return 0.0; }
static constexpr result_type max() { return 0xffffffffffffffff; }
static constexpr result_type min() { return 0; }
constexpr result_type operator()() { return engine() ^ get_val(); }
};
#endif
template<itype::u32 Size, class URBG> class RandBuffer: public URBG {
typename URBG::result_type buf[Size];
itype::u32 x, cnt;
public:
constexpr RandBuffer() { init(); }
constexpr explicit RandBuffer(URBG::result_type value): URBG(value) { init(); }
constexpr void reload() { x= Invoke(static_cast<URBG&>(*this)), cnt= 0; }
constexpr void init() {
for(itype::u32 i= 0; i != Size; ++i) buf[i]= Invoke(static_cast<URBG&>(*this));
x= Invoke(static_cast<URBG&>(*this)), cnt= 0;
}
constexpr URBG::result_type operator()() { return x ^ buf[cnt++]; }
};
template<itype::u32 Size> using RandBuffer32= RandBuffer<Size, Rand32>;
template<itype::u32 Size> using RandBuffer64= RandBuffer<Size, Rand64>;
// @brief Generate 32bit uniform random numbers in [0, max) (https://www.pcg-random.org/posts/bounded-rands.html)
template<class URBG> constexpr itype::u32 Uniform32(URBG&& g, itype::u32 max) {
return (static_cast<itype::u64>(Invoke(g) & 4294967295u) * max) >> 32;
}
// @brief Generate 32bit uniform random numbers in [min, max) (https://www.pcg-random.org/posts/bounded-rands.html)
template<class URBG> constexpr itype::u32 Uniform32(URBG&& g, itype::u32 min, itype::u32 max) {
return static_cast<itype::u32>((static_cast<itype::u64>(Invoke(g) & 4294967295u) * (max - min)) >> 32) + min;
}
// @brief Generate 64bit uniform random numbers in [0, max) (https://www.pcg-random.org/posts/bounded-rands.html)
template<class URBG> constexpr itype::u64 Uniform64(URBG&& g, itype::u64 max) {
return (static_cast<itype::u128>(Invoke(g)) * max) >> 64;
}
// @brief Generate 64bit uniform random numbers in [min, max) (https://www.pcg-random.org/posts/bounded-rands.html)
template<class URBG> constexpr itype::u64 Uniform64(URBG&& g, itype::u64 min, itype::u64 max) {
return static_cast<itype::u64>((static_cast<itype::u128>(Invoke(g)) * (max - min)) >> 64) + min;
}
template<std::ranges::random_access_range R, class URBG> constexpr void Shuffle(R&& r, URBG&& g) {
itype::u32 sz= std::ranges::size(r);
auto itr= std::ranges::begin(r);
for(itype::u32 i= 0; i != sz; ++i, ++itr) {
std::ranges::swap(*itr, *std::ranges::next(itr, Uniform32(g, sz - i)));
}
}
template<class URBG> constexpr itype::u32 UnbiasedUniform32(URBG&& g, itype::u32 max) {
itype::u32 mask= ~0u;
--max;
mask>>= std::countl_zero(max | 1);
itype::u32 x;
do {
x= Invoke(g) & mask;
} while(x > max);
return x;
}
template<class URBG> constexpr itype::u32 UnbiasedUniform32(URBG&& g, itype::u32 min, itype::u32 max) {
return min + UnbiasedUniform32(g, max - min);
}
template<class URBG> constexpr itype::u64 UnbiasedUniform64(URBG&& g, itype::u64 max) {
itype::u64 mask= ~0ull;
--max;
mask>>= std::countl_zero(max | 1);
itype::u64 x;
do {
x= Invoke(g) & mask;
} while(x > max);
return x;
}
template<class URBG> constexpr itype::u32 UnbiasedUniform64(URBG&& g, itype::u64 min, itype::u64 max) {
return min + UnbiasedUniform64(g, max - min);
}
//https://speakerdeck.com/hole/rand01?slide=31
template<class URBG> constexpr ftype::f32 Canocicaled32(URBG&& g) {
return std::bit_cast<ftype::f32>((127u << 23) | (static_cast<itype::u32>(Invoke(g)) & 0x7fffff)) - 1.0f;
}
template<class URBG> constexpr ftype::f32 Uniformf32(URBG&& g, ftype::f32 max) {
return Canocicaled32(g) * max;
}
template<class URBG> constexpr ftype::f32 Uniformf32(URBG&& g, ftype::f32 min, ftype::f32 max) {
return Canocicaled32(g) * (max - min) + min;
}
template<class URBG> constexpr ftype::f64 Canocicaled64(URBG&& g) {
return std::bit_cast<ftype::f64>((1023ull << 52) | (static_cast<itype::u64>(Invoke(g)) & 0xfffffffffffffull)) - 1.0;
}
template<class URBG> constexpr ftype::f64 Uniformf64(URBG&& g, ftype::f64 max) {
return Canocicaled64(g) * max;
}
template<class URBG> constexpr ftype::f64 Uniformf64(URBG&& g, ftype::f64 min, ftype::f64 max) {
return Canocicaled64(g) * (max - min) + min;
}
template<class URBG> constexpr bool Bernoulli(URBG&& g) {
return Invoke(g) & 1;
}
template<class URBG> constexpr bool Bernoulli32(URBG&& g, ftype::f32 p) {
return Canocicaled32(g) < p;
}
template<class URBG> constexpr bool Bernoulli64(URBG&& g, ftype::f64 p) {
return Canocicaled64(g) < p;
}
}// namespace gsh
namespace gsh {
template<class T>
requires std::is_arithmetic_v<T>
class Point2: public internal::ArithmeticInterface<Point2<T>> {
public:
T x{}, y{};
constexpr Point2() {}
constexpr Point2(const T& a, const T& b): x(a), y(b) {}
using value_type= T;
constexpr Point2& operator+=(const Point2& p) noexcept {
x+= p.x, y+= p.y;
return *this;
}
constexpr Point2& operator-=(const Point2& p) noexcept {
x-= p.x, y-= p.y;
return *this;
}
friend constexpr bool operator==(const Point2& a, const Point2& b) { return a.x == b.x && a.y == b.y; }
};
template<class T= ftype::f64, class U> constexpr T Norm(const Point2<U>& a) {
return std::hypot(static_cast<T>(a.x), static_cast<T>(a.y));
}
template<class T> constexpr T NormSquare(const Point2<T>& a) {
return a.x * a.x + a.y * a.y;
}
template<class T> constexpr T Dot(const Point2<T>& a, const Point2<T>& b) {
return a.x * b.x + a.y * b.y;
}
template<class T> constexpr T Cross(const Point2<T>& a, const Point2<T>& b) {
return a.x * b.y + a.y * b.x;
}
template<class T, class U> constexpr T NormSquare(const Point2<U>& a) {
return static_cast<T>(a.x) * static_cast<T>(a.x) + static_cast<T>(a.y) * static_cast<T>(a.y);
}
template<class T, class U> constexpr T Dot(const Point2<U>& a, const Point2<U>& b) {
return static_cast<T>(a.x) * static_cast<T>(b.x) + static_cast<T>(a.y) * static_cast<T>(b.y);
}
template<class T, class U> constexpr T Cross(const Point2<U>& a, const Point2<U>& b) {
return static_cast<T>(a.x) * static_cast<T>(b.y) - static_cast<T>(a.y) * static_cast<T>(b.x);
}
template<class T>
requires std::is_arithmetic_v<T>
class Point3: public internal::ArithmeticInterface<Point3<T>> {
public:
T x{}, y{}, z{};
constexpr Point3() {}
constexpr Point3(const T& a, const T& b, const T& c): x(a), y(b), z(c) {}
using value_type= T;
constexpr Point3& operator+=(const Point3& p) {
x+= p.x, y+= p.y, z+= p.z;
return *this;
}
constexpr Point3& operator-=(const Point3& p) {
x-= p.x, y-= p.y, z-= p.z;
return *this;
}
friend constexpr bool operator==(const Point3& a, const Point3& b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
};
template<class T= ftype::f64, class U> constexpr T Norm(const Point3<U>& a) {
return std::hypot(static_cast<T>(a.x), static_cast<T>(a.y), static_cast<T>(a.z));
}
template<class T> constexpr T NormSquare(const Point3<T>& a) {
return a.x * a.x + a.y * a.y + a.z * a.z;
}
template<class T> constexpr T Dot(const Point3<T>& a, const Point3<T>& b) {
return a.x * b.x + a.y * b.y + a.z * b.z;
}
template<class T> constexpr Point3<T> Cross(const Point3<T>& a, const Point3<T>& b) {
return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}
template<class T, class U> constexpr T NormSquare(const Point3<U>& a) {
return static_cast<T>(a.x) * static_cast<T>(a.x) + static_cast<T>(a.y) * static_cast<T>(a.y) + static_cast<T>(a.z) * static_cast<T>(a.z);
}
template<class T, class U> constexpr T Dot(const Point3<U>& a, const Point3<U>& b) {
return static_cast<T>(a.x) * static_cast<T>(b.x) + static_cast<T>(a.y) * static_cast<T>(b.y) + static_cast<T>(a.z) * static_cast<T>(b.z);
}
template<class T, class U> constexpr Point3<T> Cross(const Point3<U>& a, const Point3<U>& b) {
return {static_cast<T>(a.y) * static_cast<T>(b.z) - static_cast<T>(a.z) * static_cast<T>(b.y), static_cast<T>(a.z) * static_cast<T>(b.x) - static_cast<T>(a.x) * static_cast<T>(b.z), static_cast<T>(a.x) * static_cast<T>(b.y) - static_cast<T>(a.y) * static_cast<T>(b.x)};
}
template<std::ranges::input_range T>
requires std::same_as<std::ranges::range_value_t<T>, Point2<itype::i32>>
constexpr Arr<Point2<itype::i32>> ArgumentSort(T&& r) {
Arr<itype::u128> v(std::ranges::size(r));
for(itype::u32 i= 0; auto&& p: r) {
auto [x, y]= p;
itype::u64 ord= 0;
const bool xs= (x >= 0), ys= (y >= 0);
const itype::u64 xu= (xs ? x : -x), yu= (ys ? y : -y);
const itype::u64 mx= (xu < yu ? yu : xu), mn= (xu < yu ? xu : yu);
const bool rev= (xs ^ ys) ^ (xu < yu);
const itype::u64 id= ys * 4ull + (xs ^ ys) * 2ull + rev;
if(x == 0 && y == 0) ord= 4ull << 61;
else {
constexpr itype::u64 li= (1ull << 61) - 1;
auto [hi, lo]= internal::Mulu128(mn, li);
const itype::u64 tmp= internal::Divu128(hi, lo, mx).first;
ord= id << 61 | (rev ? li - tmp : tmp);
}
v[i++]= static_cast<itype::u128>(std::bit_cast<itype::u64>(p)) << 64 | ord;
}
v.sort({}, [](itype::u128 x) { return static_cast<itype::u64>(x); });
Arr<Point2<itype::i32>> res(v.size());
for(itype::u32 i= 0, j= v.size(); i != j; ++i) res[i]= std::bit_cast<Point2<itype::i32>>(static_cast<itype::u64>(v[i] >> 64));
return res;
}
template<std::ranges::input_range T>
requires std::same_as<std::remove_cvref_t<std::ranges::range_value_t<T>>, Point2<itype::i32>>
constexpr Arr<Point2<itype::i32>> ConvexHull(T&& r) {
const itype::u32 n= std::ranges::size(r);
if(n <= 1) return r;
itype::u32 m= 1;
Arr<Point2<itype::i32>> p(n);
{
Arr<itype::u64> sorted(n);
for(itype::u32 i= 0; auto&& e: r) sorted[i++]= std::bit_cast<itype::u64>(e) ^ 0x8000000080000000;
sorted.sort();
p[0]= std::bit_cast<Point2<itype::i32>>(sorted[0] ^ 0x8000000080000000);
for(itype::u32 i= 1; i != n; ++i) {
p[m]= std::bit_cast<Point2<itype::i32>>(sorted[i] ^ 0x8000000080000000);
m+= sorted[i] != sorted[i - 1];
}
}
if(m <= 2) {
p.resize(m);
return p;
}
Arr<Point2<itype::i32>> ch(2 * m);
itype::u32 k= 0;
for(itype::u32 i= 0; i < m; ch[k++]= p[i++]) {
while(k >= 2 && Cross<itype::i64>(ch[k - 1] - ch[k - 2], p[i] - ch[k - 2]) <= 0) --k;
}
for(itype::u32 i= m - 1, t= k + 1; i > 0; ch[k++]= p[--i]) {
while(k >= t && Cross<itype::i64>(ch[k - 1] - ch[k - 2], p[i - 1] - ch[k - 2]) <= 0) --k;
}
ch.resize(k - 1);
return ch;
}
template<std::ranges::random_access_range T>
requires std::same_as<std::remove_cvref_t<std::ranges::range_value_t<T>>, Point2<itype::i32>>
constexpr auto ConvexDiameter(T&& p) {
struct result_type {
Point2<itype::i32> a, b;
constexpr auto distance() const noexcept { return Norm<ftype::f64>(a - b); }
constexpr const auto& first() const noexcept { return a; }
constexpr const auto& second() const noexcept { return b; }
};
const itype::u32 n= std::ranges::size(p);
if(n == 0) throw Exception("gsh::ConvexDiameter / Input is empty.");
const auto bg= std::ranges::begin(p);
if(n <= 2) {
if(n == 1) return result_type{*bg, *bg};
else return result_type{*bg, *std::ranges::next(bg)};
}
itype::u32 is= 0, js= 0;
for(itype::u32 i= 1; i != n; i++) {
auto a= std::ranges::next(bg, i)->y, b= std::ranges::next(bg, is)->y, c= std::ranges::next(bg, js)->y;
is= (a > b ? i : is);
js= (a < c ? i : js);
}
itype::i64 maxdis= NormSquare<itype::i64>(*std::ranges::next(bg, is) - *std::ranges::next(bg, js));
itype::u32 maxi= is, maxj= js, i= is, j= js;
do {
const itype::u32 in= (i + 1 == n ? 0 : i + 1), jn= (j + 1 == n ? 0 : j + 1);
const bool f= Cross<itype::i64>(*std::ranges::next(bg, in) - *std::ranges::next(bg, i), *std::ranges::next(bg, jn) - *std::ranges::next(bg, j)) > 0;
j= f ? jn : j;
i= f ? i : in;
const itype::i64 tmp= NormSquare<itype::i64>(*std::ranges::next(bg, i) - *std::ranges::next(bg, j));
const bool g= tmp > maxdis;
maxdis= g ? tmp : maxdis;
maxi= g ? i : maxi;
maxj= g ? j : maxj;
} while(i != is || j != js);
return result_type{*std::ranges::next(bg, maxi), *std::ranges::next(bg, maxj)};
}
template<std::ranges::range T> auto FurthestPair(T&& r) {
return ConvexDiameter(ConvexHull(r));
}
/*
template<RandomAccessRange T>
    requires Rangeof<T, Point2<itype::i32>>
constexpr auto ClosestPair(T&& r) {
    using traits = RangeTraits<T>;
    struct result_type {
        Point2<itype::i32> a, b;
        template<class U = ftype::f64> constexpr auto distance() const noexcept { return Norm<U>(a - b); }
        constexpr const auto& first() const noexcept { return a; }
        constexpr const auto& second() const noexcept { return b; }
    };
    const auto n = traits::size(r);
    if (n == 0) throw Exception("gsh::ConvexDiameter / Input is empty.");
    const auto bg = traits::begin(r);
    if (n == 1) return result_type{ *bg, *std::next(bg) };
    RandBuffer32<64> engine;
    itype::u64 d = -1;
    result_type res;
    for (itype::u32 i = 0; i != (n + 31) / 32; ++i) {
        engine.init();
        for (itype::u32 j = 0; j != 32; ++j) {
            itype::u32 a = Uniform32(engine, n), b = Uniform32(engine, n - 1);
            b += b >= a;
            const auto &&c = *std::next(bg, a), d = *std::next(bg, b);
            const itype::u64 tmp = NormSquare<itype::u64>(c - d);
            d = tmp < d ? tmp : d;
            res = tmp < d ? result_type{ c, d } : res;
        }
    }
    if (d == 0) return res;
    std::unordered_multimap<itype::u64, Point2<itype::i32>> m;
    m.reserve(n);
    for (auto&& p : r) {
        const itype::u64 key = (static_cast<itype::u64>((std::bit_cast<itype::u32>(p.x) ^ (1u << 31)) / d) << 32) | ((std::bit_cast<itype::u32>(p.y) ^ (1u << 31)) / d);
        m.emplace(key, p);
    }
    const auto ed = traits::end(r);
    for (auto i = bg; i != ed;) {
        auto j = std::next(i);
        while (j != ed && *i != *j) std::advance(j);

        i = j;
    }
}
*/
}// namespace gsh
namespace gsh {
template<class T, class Comp= Less, class Alloc= Allocator<T>> class Heap {
Vec<T, Alloc> data;
[[no_unique_address]] Comp comp_func;
itype::u32 mx= 0;
public:
using value_type= T;
using reference= T&;
using const_reference= const T&;
using pointer= T*;
using const_pointer= const T*;
using size_type= itype::u32;
using difference_type= itype::i32;
using compare_type= Comp;
using allocator_type= Alloc;
constexpr Heap() noexcept {}
constexpr explicit Heap(const Comp& comp, const Alloc& alloc= Alloc()): data(alloc), comp_func(comp) {}
constexpr explicit Heap(const Alloc& alloc): data(alloc) {}
template<class InputIterator> constexpr Heap(InputIterator first, InputIterator last, const Comp& comp= Comp(), const Alloc& alloc= Alloc()): data(first, last, alloc), comp_func(comp) { make_heap(); }
template<class InputIterator> Heap(InputIterator first, InputIterator last, const Alloc& alloc): data(first, last, alloc) { make_heap(); }
constexpr Heap(const Heap& x)= default;
constexpr Heap(Heap&& y) noexcept= default;
constexpr Heap(const Heap& x, const Alloc& alloc): data(x.data, alloc), comp_func(x.comp_func), mx(x.mx) {}
constexpr Heap(Heap&& y, const Alloc& alloc): data(std::move(y.data), alloc), comp_func(y.comp_func), mx(y.mx) {}
constexpr Heap(std::initializer_list<value_type> init, const Comp& comp= Comp(), const Alloc& alloc= Alloc()): data(init, alloc), comp_func(comp) { make_heap(); }
constexpr Heap(std::initializer_list<value_type> init, const Alloc& alloc): data(init, alloc) { make_heap(); }
constexpr Heap& operator=(const Heap&)= default;
constexpr Heap& operator=(Heap&&) noexcept(std::is_nothrow_move_assignable_v<Comp>)= default;
private:
constexpr static bool nothrow_op= std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T> && std::is_nothrow_invocable_v<Comp, T&, T&>;
GSH_INTERNAL_INLINE constexpr bool is_min_level(itype::u32 idx) const noexcept {
Assume(idx + 1 != 0);
return std::bit_width(idx + 1) & 1;
}
GSH_INTERNAL_INLINE constexpr void set_mx() noexcept(nothrow_op) {
if(data.size() >= 3) [[likely]]
mx= 1 + Invoke(comp_func, data[1], data[2]);
else mx= data.size() == 2;
}
template<bool Min, bool SetMax> GSH_INTERNAL_INLINE constexpr void push_down(itype::u32 idx) noexcept(nothrow_op) {
itype::u32 lim= (data.size() + 1) / 4 - 1;
auto comp= [&](auto&& a, auto&& b) GSH_INTERNAL_INLINE {
if constexpr(Min) return static_cast<bool>(Invoke(comp_func, a, b));
else return static_cast<bool>(Invoke(comp_func, b, a));
};
itype::u32 cur= idx;
T tmp= std::move(data[idx]);
while(true) {
itype::u32 grdch= (cur + 1) * 4 - 1;
if(cur >= lim) [[unlikely]] {
itype::u32 ch= (cur + 1) * 2 - 1;
if(grdch < data.size()) [[unlikely]] {
itype::u32 m= ch + comp(data[ch + 1], data[ch]);
switch(data.size() - grdch) {
case 3:
{
itype::u32 n= grdch + 1 + comp(data[grdch + 2], data[grdch + 1]);
m= comp(data[m], data[grdch]) ? m : grdch;
m= comp(data[m], data[n]) ? m : n;
break;
}
case 2:
{
itype::u32 n= grdch + comp(data[grdch + 1], data[grdch]);
m= comp(data[m], data[n]) ? m : n;
break;
}
case 1:
{
m= comp(data[m], data[grdch]) ? m : grdch;
break;
}
default: Unreachable();
};
if(m < grdch) {
if(comp(data[m], tmp)) {
data[cur]= std::move(data[m]);
data[m]= std::move(tmp);
} else {
data[cur]= std::move(tmp);
}
} else {
itype::u32 p= (m + 1) / 2 - 1;
if(comp(data[m], tmp)) {
data[cur]= std::move(data[m]);
if(comp(data[p], tmp)) {
data[m]= std::move(data[p]);
data[p]= std::move(tmp);
} else {
data[m]= std::move(tmp);
}
} else {
data[cur]= std::move(tmp);
}
}
} else if(ch >= data.size()) [[likely]] {
data[cur]= std::move(tmp);
} else if(ch < data.size() - 1) [[likely]] {
bool f= comp(data[ch + 1], data[ch]);
T m= std::move(f ? data[ch + 1] : data[ch]);
bool g= comp(m, tmp);
data[cur]= std::move(g ? m : tmp);
data[ch + f]= std::move(g ? tmp : m);
} else if(comp(data[ch], tmp)) {
data[cur]= std::move(data[ch]);
data[ch]= std::move(tmp);
} else {
data[cur]= std::move(tmp);
}
if constexpr(SetMax) {
Assume(data.size() >= 3);
set_mx();
}
return;
}
itype::u32 a= grdch + comp(data[grdch + 1], data[grdch]);
itype::u32 b= grdch + 2 + comp(data[grdch + 3], data[grdch + 2]);
itype::u32 c= a + comp(data[b], data[a]) * (b - a);
itype::u32 p= (c + 1) / 2 - 1;
if(!comp(data[c], tmp)) {
data[cur]= std::move(tmp);
if constexpr(SetMax) {
Assume(data.size() >= 3);
set_mx();
}
return;
}
data[cur]= std::move(data[c]);
cur= c;
bool f= comp(data[p], tmp);
T tmp2= data[p];
data[p]= std::move(f ? tmp : tmp2);
tmp= std::move(f ? tmp2 : tmp);
}
}
GSH_INTERNAL_INLINE constexpr void pop_min_impl() noexcept(nothrow_op) {
if(data.size() <= 3) [[unlikely]] {
switch(data.size()) {
case 0: break;
case 1: mx= 0; break;
case 2:
{
if(Invoke(comp_func, data[1], data[0])) {
auto tmp= std::move(data[0]);
data[0]= std::move(data[1]);
data[1]= std::move(tmp);
}
mx= 1;
break;
}
case 3:
{
itype::u32 m= 1 + Invoke(comp_func, data[2], data[1]);
if(Invoke(comp_func, data[m], data[0])) {
auto tmp= std::move(data[0]);
data[0]= std::move(data[m]);
data[m]= std::move(tmp);
}
mx= 1 + Invoke(comp_func, data[1], data[2]);
break;
}
default: Unreachable();
}
return;
}
push_down<true, true>(0);
}
GSH_INTERNAL_INLINE constexpr void pop_max_impl() noexcept(nothrow_op) {
if(data.size() <= 3) [[unlikely]] {
set_mx();
return;
}
push_down<false, true>(mx);
}
constexpr void make_heap() noexcept(nothrow_op) {
if(data.size() <= 1) [[unlikely]]
return;
itype::u32 lim1= data.size() / 2;
if(data.size() % 2 == 0) {
--lim1;
itype::u32 ch= (lim1 + 1) * 2 - 1;
if(Invoke(comp_func, data[lim1], data[ch]) ^ is_min_level(lim1)) {
auto tmp= std::move(data[lim1]);
data[lim1]= std::move(data[ch]);
data[ch]= std::move(tmp);
}
}
itype::u32 lim2= data.size() / 4;
Assume(lim2 + 1 != 0);
itype::u32 lr= std::bit_floor(lim2 + 1) * 2 - 1;
lr= lim1 < lr ? lim1 : lr;
bool lim2_min= is_min_level(lim2);
for(itype::u32 i= lim2_min ? lim2 : lr, j= lim2_min ? lr : lim1; i < j; ++i) {
itype::u32 ch= (i + 1) * 2 - 1;
itype::u32 m= ch + static_cast<bool>(Invoke(comp_func, data[ch + 1], data[ch]));
bool f= Invoke(comp_func, data[i], data[m]);
T tmp1= std::move(data[i]);
T tmp2= std::move(data[m]);
data[i]= std::move(f ? tmp1 : tmp2);
data[m]= std::move(f ? tmp2 : tmp1);
}
for(itype::u32 i= lim2_min ? lr : lim2, j= lim2_min ? lim1 : lr; i < j; ++i) {
itype::u32 ch= (i + 1) * 2 - 1;
itype::u32 m= ch + static_cast<bool>(Invoke(comp_func, data[ch], data[ch + 1]));
bool f= Invoke(comp_func, data[m], data[i]);
T tmp1= std::move(data[i]);
T tmp2= std::move(data[m]);
data[i]= std::move(f ? tmp1 : tmp2);
data[m]= std::move(f ? tmp2 : tmp1);
}
for(itype::u32 i= lim2; i--;) {
if(is_min_level(i)) {
push_down<true, false>(i);
} else {
push_down<false, false>(i);
}
}
set_mx();
}
constexpr void push_up() noexcept(nothrow_op) {
const itype::u32 idx= data.size() - 1;
if(idx <= 2) [[unlikely]] {
if(Invoke(comp_func, data[idx], data[0])) {
auto tmp= std::move(data[idx]);
data[idx]= std::move(data[0]);
data[0]= std::move(tmp);
}
set_mx();
return;
}
itype::u32 p= ((idx + 1) >> 1) - 1;
if(is_min_level(idx)) {
if(Invoke(comp_func, data[p], data[idx])) {
// push_up_max(p)
T tmp= std::move(data[idx]);
data[idx]= std::move(data[p]);
itype::u32 cur= p;
while(cur > 2 && Invoke(comp_func, data[p= ((cur + 1) / 4) - 1], tmp)) {
data[cur]= std::move(data[p]);
cur= p;
}
data[cur]= std::move(tmp);
Assume(data.size() >= 3);
set_mx();
} else {
// push_up_min(idx)
T tmp= std::move(data[idx]);
itype::u32 cur= idx;
while(Invoke(comp_func, tmp, data[p= ((cur + 1) / 4) - 1])) {
data[cur]= std::move(data[p]);
cur= p;
if(cur == 0) [[unlikely]]
break;
}
data[cur]= std::move(tmp);
}
} else {
if(Invoke(comp_func, data[idx], data[p])) {
// push_up_min(p)
T tmp= std::move(data[idx]);
data[idx]= std::move(data[p]);
itype::u32 cur= p;
while(cur != 0 && Invoke(comp_func, tmp, data[p= ((cur + 1) / 4) - 1])) {
data[cur]= std::move(data[p]);
cur= p;
}
data[cur]= std::move(tmp);
} else {
// push_up_max(idx)
T tmp= std::move(data[idx]);
itype::u32 cur= idx;
while(Invoke(comp_func, data[p= ((cur + 1) / 4) - 1], tmp)) {
data[cur]= std::move(data[p]);
cur= p;
if(cur <= 2) [[unlikely]] {
data[cur]= std::move(tmp);
Assume(data.size() >= 3);
set_mx();
return;
}
}
data[cur]= std::move(tmp);
}
}
}
public:
template<std::ranges::range R> constexpr void assign(R&& r) {
data.assign(std::forward<R>(r));
make_heap();
}
constexpr const_reference top() const noexcept { return data[0]; }
constexpr const_reference min() const noexcept { return data[0]; }
constexpr const_reference max() const noexcept { return data[mx]; }
[[nodiscard]] constexpr bool empty() const noexcept { return data.empty(); }
constexpr itype::u32 size() const noexcept { return data.size(); }
constexpr void reserve(itype::u32 n) { data.reserve(n); }
constexpr void push(const T& x) {
data.push_back(x);
push_up();
}
constexpr void push(T&& x) {
data.push_back(std::move(x));
push_up();
}
template<class... Args> constexpr void emplace(Args&&... args) {
data.emplace_back(std::forward<Args>(args)...);
push_up();
}
constexpr void pop() noexcept(nothrow_op) { pop_min(); }
constexpr void pop_min() noexcept(nothrow_op) {
data[0]= std::move(data.back());
data.pop_max();
pop_min_impl();
}
constexpr void pop_max() noexcept(nothrow_op) {
data[mx]= std::move(data.back());
data.pop_max();
pop_max_impl();
}
constexpr void replace(const T& x) noexcept(nothrow_op && std::is_nothrow_copy_assignable_v<T>) { replace_min(x); }
constexpr void replace(T&& x) noexcept(nothrow_op) { replace_min(std::move(x)); }
constexpr void replace_min(const T& x) noexcept(nothrow_op && std::is_nothrow_copy_assignable_v<T>) {
data[0]= x;
pop_min_impl();
}
constexpr void replace_min(T&& x) noexcept(nothrow_op) {
data[0]= std::move(x);
pop_min_impl();
}
constexpr void replace_max(const T& x) noexcept(nothrow_op && std::is_nothrow_copy_assignable_v<T>) {
data[mx]= x;
pop_max_impl();
}
constexpr void replace_max(T&& x) noexcept(nothrow_op) {
data[mx]= std::move(x);
pop_max_impl();
}
constexpr void pushpop(const T& x) noexcept(nothrow_op && std::is_nothrow_copy_assignable_v<T>) { pushpop_min(x); }
constexpr void pushpop(T&& x) noexcept(nothrow_op) { pushpop_min(std::move(x)); }
constexpr void pushpop_min(const T& x) noexcept(nothrow_op && std::is_nothrow_copy_assignable_v<T>) {
if(Invoke(comp_func, data[0], x)) {
data[0]= x;
pop_min_impl();
}
}
constexpr void pushpop_min(T&& x) noexcept(nothrow_op) {
if(Invoke(comp_func, data[0], x)) {
data[0]= std::move(x);
pop_min_impl();
}
}
constexpr void pushpop_max(const T& x) noexcept(nothrow_op && std::is_nothrow_copy_assignable_v<T>) {
if(Invoke(comp_func, x, data[mx])) {
data[mx]= x;
pop_max_impl();
}
}
constexpr void pushpop_max(T&& x) noexcept(nothrow_op) {
if(Invoke(comp_func, x, data[mx])) {
data[mx]= std::move(x);
pop_max_impl();
}
}
};
}// namespace gsh
namespace gsh {
namespace internal {
template<class T> concept IsStaticModint= !requires(T x, typename T::value_type m) { x.set(m); };
template<class T, itype::u32 id, bool IsThreadLocal> class ModintBase {
protected:
static inline T mint{};
};
template<class T, itype::u32 id> class ModintBase<T, id, true> {
protected:
thread_local static inline T mint{};
};
template<IsStaticModint T, itype::u32 id> class ModintBase<T, id, false> {
protected:
constexpr static T mint{};
};
template<class T, itype::u32 id= 0, bool IsThreadLocal= false> class ModintInterface: public ModintBase<T, id, IsThreadLocal> {
typename T::value_type val_{};
constexpr static auto& mint() noexcept { return ModintBase<T, id, IsThreadLocal>::mint; }
constexpr static ModintInterface construct(typename T::value_type x) noexcept {
ModintInterface res;
res.val_= x;
return res;
}
public:
using value_type= typename T::value_type;
constexpr static bool is_static_mod= IsStaticModint<T>;
constexpr ModintInterface() noexcept {}
template<class U> constexpr ModintInterface(U x) noexcept { operator=(x); }
constexpr explicit operator value_type() const noexcept { return val(); }
constexpr static void set_mod(value_type x) { mint().set(x); }
constexpr value_type val() const noexcept { return mint().val(val_); }
constexpr static value_type mod() noexcept { return mint().mod(); }
template<class U> constexpr ModintInterface& operator=(U x) noexcept {
val_= mint().build(x);
return *this;
}
constexpr static ModintInterface raw(value_type x) noexcept { return construct(mint().raw(x)); }
constexpr static ModintInterface nan() noexcept { return construct(mint().nan()); }
constexpr static bool isnan(value_type x) noexcept { return mint.isnan(x); }
constexpr ModintInterface inv() const noexcept { return construct(mint().inv(val_)); }
constexpr ModintInterface pow(itype::u64 e) const noexcept { return construct(mint().pow(val_, e)); }
constexpr ModintInterface operator+() const noexcept { return *this; }
constexpr ModintInterface operator-() const noexcept { return construct(mint().neg(val_)); }
constexpr ModintInterface& operator++() noexcept {
val_= mint().inc(val_);
return *this;
}
constexpr ModintInterface& operator--() noexcept {
val_= mint().dec(val_);
return *this;
}
constexpr ModintInterface operator++(int) noexcept {
ModintInterface copy= *this;
val_= mint().inc(val_);
return copy;
}
constexpr ModintInterface operator--(int) noexcept {
ModintInterface copy= *this;
val_= mint().dec(val_);
return copy;
}
constexpr ModintInterface& operator+=(ModintInterface x) noexcept {
val_= mint().add(val_, x.val_);
return *this;
}
constexpr ModintInterface& operator-=(ModintInterface x) noexcept {
val_= mint().sub(val_, x.val_);
return *this;
}
constexpr ModintInterface& operator*=(ModintInterface x) noexcept {
val_= mint().mul(val_, x.val_);
return *this;
}
constexpr ModintInterface& operator/=(ModintInterface x) {
val_= mint().div(val_, x.val_);
return *this;
}
friend constexpr ModintInterface operator+(ModintInterface l, ModintInterface r) noexcept { return construct(mint().add(l.val_, r.val_)); }
friend constexpr ModintInterface operator-(ModintInterface l, ModintInterface r) noexcept { return construct(mint().sub(l.val_, r.val_)); }
friend constexpr ModintInterface operator*(ModintInterface l, ModintInterface r) noexcept { return construct(mint().mul(l.val_, r.val_)); }
friend constexpr ModintInterface operator/(ModintInterface l, ModintInterface r) { return construct(mint().div(l.val_, r.val_)); }
friend constexpr bool operator==(ModintInterface l, ModintInterface r) noexcept { return mint().same(l.val_, r.val_); }
friend constexpr bool operator!=(ModintInterface l, ModintInterface r) noexcept { return !mint().same(l.val_, r.val_); }
};
template<class D, class T> class ModintImpl {
constexpr const D& derived() const noexcept { return *static_cast<const D*>(this); }
constexpr static bool is_static_mod= IsStaticModint<D>;
public:
using value_type= T;
constexpr value_type val(value_type x) const noexcept { return x; }
constexpr value_type build(itype::u32 x) const noexcept { return x % derived().mod(); }
constexpr value_type build(itype::u64 x) const noexcept { return x % derived().mod(); }
template<class U> constexpr value_type build(U x) const noexcept {
static_assert(std::is_integral_v<U>, "gsh::internal::ModintImpl::build<U> / Only integer types can be assigned.");
if constexpr(std::is_unsigned_v<U>) {
if constexpr(std::is_same_v<U, itype::u128>) return derived().raw(static_cast<value_type>(x % derived().mod()));
else if constexpr(std::is_same_v<U, unsigned long long> || std::is_same_v<U, unsigned long>) return derived().build(static_cast<itype::u64>(x));
else return derived().build(static_cast<itype::u32>(x));
} else {
if(x < 0) {
if constexpr(std::is_same_v<U, itype::i128>) return derived().neg(derived().raw(static_cast<value_type>(-x % derived().mod())));
else if constexpr(std::is_same_v<U, long long> || std::is_same_v<U, long>) return derived().neg(derived().build(static_cast<itype::u64>(-x)));
else return derived().neg(derived().build(static_cast<itype::u32>(-x)));
} else {
if constexpr(std::is_same_v<U, itype::i128>) return derived().raw(static_cast<value_type>(x % derived().mod()));
else if constexpr(std::is_same_v<U, long long> || std::is_same_v<U, long>) return derived().build(static_cast<itype::u64>(x));
else return derived().build(static_cast<itype::u32>(x));
}
}
}
constexpr value_type raw(value_type x) const noexcept {
Assume(x < derived().mod());
return x;
}
constexpr value_type nan() const noexcept { return std::numeric_limits<value_type>::max(); }
constexpr value_type isnan(value_type x) const noexcept { return x == derived().nan(); }
constexpr value_type zero() const noexcept { return derived().raw(0); }
constexpr value_type one() const noexcept { return derived().raw(1); }
constexpr value_type neg(value_type x) const noexcept {
Assume(x < derived().mod());
return x == 0 ? 0 : derived().mod() - x;
}
constexpr value_type inc(value_type x) const noexcept {
Assume(x < derived().mod());
return x + 1 == derived().mod() ? 0 : x + 1;
}
constexpr value_type dec(value_type x) const noexcept {
Assume(x < derived().mod());
return x == 0 ? derived().mod() - 1 : x - 1;
}
constexpr value_type add(value_type x, value_type y) const noexcept {
Assume(x < derived().mod() && y < derived().mod());
return x + y - (derived().mod() - x <= y) * derived().mod();
}
constexpr value_type sub(value_type x, value_type y) const noexcept {
Assume(x < derived().mod() && y < derived().mod());
return x - y + (x < y) * derived().mod();
}
constexpr value_type fma(value_type x, value_type y, value_type z) const noexcept { return derived().add(derived().mul(x, y), z); }
constexpr value_type div(value_type x, value_type y) const noexcept {
const value_type iv= derived().inv(y);
if(derived().same(iv, derived().zero())) [[unlikely]]
throw gsh::Exception("gsh::internal::ModintImpl::div / Cannot calculate inverse.");
return derived().mul(x, iv);
}
constexpr bool same(value_type x, value_type y) const noexcept { return x == y; }
constexpr value_type abs(value_type x) const noexcept {
Assume(x < derived().mod());
return derived().val(x) > (derived().mod() / 2) ? derived().neg(x) : x;
}
constexpr value_type pow(value_type x, itype::u64 e) const noexcept {
value_type res= derived().one();
while(e) {
auto tmp= derived().mul(x, x);
if(e & 1) res= derived().mul(res, x);
x= tmp;
e>>= 1;
}
return res;
}
constexpr value_type inv(value_type t) const noexcept {
value_type a= 1, b= 0, x= derived().val(t), y= derived().mod();
while(true) {
if(x <= 1) {
if(x == 0) [[unlikely]]
return derived().nan();
return derived().raw(a);
}
b+= a * (y / x);
y%= x;
if(y <= 1) {
if(y == 0) [[unlikely]]
return derived().nan();
return derived().raw(derived().mod() - b);
}
a+= b * (x / y);
x%= y;
}
}
constexpr itype::i32 legendre(value_type x) const noexcept {
auto res= derived().pow(x, (derived().mod() - 1) >> 1);
const bool a= derived().same(res, derived().zero()), b= derived().same(res, derived().one());
return a ? 0 : (b ? 1 : -1);
}
constexpr itype::i32 jacobi(value_type x) const noexcept {
auto a= derived().val(x), n= derived().mod();
if(a == 1) return 1;
itype::i32 res= 1;
while(a != 0) {
while(!(a & 1) && a != 0) {
a>>= 1;
res= ((n & 0b111) == 3 || (n & 0b111) == 5) ? -res : res;
}
res= ((a & 0b11) == 3 || (n & 0b11) == 3) ? -res : res;
auto tmp= n;
n= a;
a= tmp;
a%= n;
}
return n == 1 ? res : 0;
}
constexpr value_type sqrt(value_type n) const noexcept {
const auto md= derived().mod();
if(md % 4 == 3) {
auto res= derived().pow(n, (md + 1) >> 2);
if(!derived().same(derived().mul(res, res), n)) return derived().nan();
else return derived().abs(res);
} else if(md % 8 == 5) {
auto res= derived().pow(n, (md + 3) >> 3);
if(!derived().same(derived().mul(res, res), n)) {
const auto p= derived().pow(derived().raw(2), (md - 1) >> 2);
res= derived().mul(res, p);
if(!derived().same(derived().mul(res, res), n)) return derived().nan();
else return derived().abs(res);
}
return derived().abs(res);
} else {
if(derived().same(n, derived().zero()) || derived().same(n, derived().one())) return n;
const itype::u32 S= std::countr_zero(md - 1);
const itype::u32 W= std::bit_width(md);
if(S * S <= 12 * W) {
const auto Q= (md - 1) >> S;
const auto tmp= derived().pow(n, Q / 2);
auto R= derived().mul(tmp, n), t= derived().mul(tmp, R);
if(derived().same(t, derived().one())) return R;
auto u= t;
for(itype::u32 i= 0; i != S - 1; ++i) u= derived().mul(u, u);
if(!derived().same(u, derived().one())) return derived().nan();
const auto base= [&]() GSH_INTERNAL_INLINE {
if(md % 3 == 2) return derived().raw(3);
if(auto x= md % 5; x == 2 || x == 3) return derived().raw(5);
if(auto x= md % 7; x == 3 || x == 5 || x == 6) return derived().raw(7);
if(auto x= md % 11; x == 2 || x == 6 || x == 7 || x == 8 || x == 10) return derived().build(11);
if(auto x= md % 13; x == 2 || x == 5 || x == 6 || x == 7 || x == 8 || x == 11) return derived().build(13);
for(const itype::u32 x: {17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97}) {
const auto y= derived().build(x);
if(derived().legendre(y) == -1) return y;
}
auto z= derived().build(101);
while(derived().legendre(z) != -1) z= derived().add(z, derived().raw(2));
return z;
}();
const auto z= derived().pow(base, Q);
itype::u32 M= S;
auto c= z;
do {
auto U= derived().mul(t, t);
itype::u32 i= 1;
while(!derived().same(U, derived().one())) U= derived().mul(U, U), ++i;
auto b= c;
for(itype::u32 j= 0, k= M - i - 1; j != k; ++j) b= derived().mul(b, b);
M= i, c= derived().mul(b, b), t= derived().mul(t, c), R= derived().mul(R, b);
} while(!derived().same(t, derived().one()));
return derived().abs(R);
} else {
if(derived().legendre(n) != 1) return derived().nan();
auto a= derived().raw(4);
decltype(a) w;
while(derived().legendre(w= derived().sub(derived().mul(a, a), n)) != -1) a= derived().inc(a);
auto res1= derived().one(), res2= derived().zero(), pow1= a, pow2= res1;
auto e= (md + 1) / 2;
while(true) {
const auto tmp2= derived().mul(pow2, w);
if(e & 1) {
const auto tmp= res1;
res1= derived().add(derived().mul(res1, pow1), derived().mul(res2, tmp2));
res2= derived().add(derived().mul(tmp, pow2), derived().mul(res2, pow1));
}
e>>= 1;
if(e == 0) return derived().abs(res1);
const auto tmp= pow1;
pow1= derived().add(derived().mul(pow1, pow1), derived().mul(pow2, tmp2));
pow2= derived().mul(pow2, derived().add(tmp, tmp));
}
}
}
}
};
template<itype::u32 mod_> class StaticModint32Impl: public ModintImpl<StaticModint32Impl<mod_>, itype::u32> {
public:
constexpr StaticModint32Impl() noexcept {}
constexpr itype::u32 mod() const noexcept { return mod_; }
constexpr itype::u32 mul(itype::u32 x, itype::u32 y) const noexcept {
Assume(x < mod_ && y < mod_);
return static_cast<itype::u64>(x) * y % mod_;
}
};
template<itype::u64 mod_> class StaticModint64Impl: public ModintImpl<StaticModint64Impl<mod_>, itype::u64> {
public:
constexpr StaticModint64Impl() noexcept {}
constexpr itype::u64 mod() const noexcept { return mod_; }
constexpr itype::u64 mul(itype::u64 x, itype::u64 y) const noexcept {
Assume(x < mod_ && y < mod_);
constexpr itype::u128 M_= std::numeric_limits<itype::u128>::max() / mod_ + std::has_single_bit(mod_);
if constexpr(mod_ < (1ull << 63)) {
const itype::u64 a= (((M_ * x) >> 64) * y) >> 64;
const itype::u64 b= x * y;
const itype::u64 c= a * mod_;
const itype::u64 d= b - c;
const bool e= d < mod_;
const itype::u64 f= d - mod_;
return e ? d : f;
} else {
const itype::u64 a= (((M_ * x) >> 64) * y) >> 64;
const itype::u128 b= static_cast<itype::u128>(x) * y;
const itype::u128 c= static_cast<itype::u128>(a) * mod_;
const itype::u128 d= b - c;
const bool e= d < mod_;
const itype::u128 f= d - mod_;
return e ? d : f;
}
}
};
template<itype::u64 mod_> using StaticModintImpl= std::conditional_t<(mod_ <= 0xffffffff), StaticModint32Impl<mod_>, StaticModint64Impl<mod_>>;
class DynamicModint32Impl: public ModintImpl<DynamicModint32Impl, itype::u32> {
itype::u32 mod_= 0;
itype::u64 M_= 0;
public:
constexpr DynamicModint32Impl() noexcept {}
constexpr void set(itype::u32 n) {
if(n <= 1) [[unlikely]]
throw Exception("gsh::internal::DynamicModint32Impl::set / Mod must be at least 2.");
mod_= n;
M_= std::numeric_limits<itype::u64>::max() / mod_ + 1;
}
constexpr itype::u32 mod() const noexcept { return mod_; }
constexpr itype::u64 build(itype::u32 x) const noexcept {
itype::u64 lowbit= M_ * x;
return (static_cast<itype::u128>(lowbit) * mod_) >> 64;
}
constexpr itype::u64 build(itype::u64 x) const noexcept { return x % mod_; }
template<class U> constexpr itype::u64 build(U x) const noexcept { return ModintImpl::build(x); }
constexpr itype::u32 mul(itype::u32 x, itype::u32 y) const noexcept {
Assume(x < mod_ && y < mod_);
const itype::u64 a= static_cast<itype::u64>(x) * y;
const itype::u64 b= (static_cast<itype::u128>(M_) * a) >> 64;
const itype::u64 c= a - b * mod_;
return c + (c >= mod_) * mod_;
}
};
class DynamicModint64Impl: public ModintImpl<DynamicModint64Impl, itype::u64> {
itype::u64 mod_= 0;
itype::u128 M_= 0;
public:
constexpr DynamicModint64Impl() noexcept {}
constexpr void set(itype::u64 n) {
if(n <= 1) [[unlikely]]
throw Exception("gsh::internal::DynamicModint64Impl::set / Mod must be at least 2.");
mod_= n;
M_= std::numeric_limits<itype::u128>::max() / mod_ + std::has_single_bit(mod_);
}
constexpr itype::u64 mod() const noexcept { return mod_; }
constexpr itype::u64 mul(itype::u64 x, itype::u64 y) const noexcept {
Assume(x < mod_ && y < mod_);
const itype::u64 a= (((M_ * x) >> 64) * y) >> 64;
const itype::u64 b= x * y;
const itype::u64 c= a * mod_;
const itype::u64 d= b - c;
const bool e= d < mod_;
const itype::u64 f= d - mod_;
return e ? d : f;
}
};
class MontgomeryModint64Impl: public ModintImpl<MontgomeryModint64Impl, itype::u64> {
itype::u64 mod_= 0, rs= 0, nr= 0, np= 0;
constexpr itype::u64 reduce(const itype::u64 t) const noexcept {
itype::u64 q= t * nr;
itype::u64 m= (static_cast<itype::u128>(q) * mod_) >> 64;
return mod_ - m;
}
constexpr itype::u64 reduce(const itype::u64 a, const itype::u64 b) const noexcept {
itype::u128 t= static_cast<itype::u128>(a) * b;
itype::u64 c= t, d= t >> 64;
itype::u64 q= c * nr;
itype::u64 m= (static_cast<itype::u128>(q) * mod_) >> 64;
return d + mod_ - m;
}
public:
constexpr MontgomeryModint64Impl() noexcept {}
constexpr void set(itype::u64 n) {
if(n <= 1) [[unlikely]]
throw Exception("gsh::internal::MontgomeryModint64Impl::set / Mod must be at least 2.");
if(n % 2 == 0) [[unlikely]]
throw Exception("gsh::internal::MontgomeryModint64Impl::set / It is not allowed to set the modulo to an even number.");
mod_= n;
rs= -static_cast<itype::u128>(n) % n;
nr= n;
for(itype::u32 i= 0; i != 6; ++i) nr*= 2 - n * nr;
np= reduce(1, rs);
}
constexpr itype::u64 val(itype::u64 x) const noexcept {
itype::u64 tmp= reduce(x);
return tmp - (tmp == mod_) * mod_;
}
constexpr itype::u64 mod() const noexcept { return mod_; }
constexpr itype::u64 build(itype::u32 x) const noexcept { return reduce(x % mod_, rs); }
constexpr itype::u64 build(itype::u64 x) const noexcept { return reduce(x % mod_, rs); }
template<class U> constexpr itype::u64 build(U x) const noexcept { return ModintImpl::build(x); }
constexpr itype::u64 raw(itype::u64 x) const noexcept {
Assume(x < mod_);
return reduce(x, rs);
}
constexpr itype::u64 zero() const noexcept { return 0; }
constexpr itype::u64 one() const noexcept {
Assume(np < 2 * mod_);
return np;
}
constexpr itype::u64 neg(itype::u64 x) const noexcept {
Assume(x < 2 * mod_);
return (2 * mod_ - x) * (x != 0);
}
constexpr itype::u64 inc(itype::u64 x) const noexcept { return add(x, np); }
constexpr itype::u64 dec(itype::u64 x) const noexcept { return sub(x, np); }
constexpr itype::u64 add(itype::u64 x, itype::u64 y) const noexcept {
Assume(x < 2 * mod_ && y < 2 * mod_);
return x + y - (x + y >= mod_) * mod_;
}
constexpr itype::u64 sub(itype::u64 x, itype::u64 y) const noexcept {
Assume(x < 2 * mod_ && y < 2 * mod_);
return x - y + (x < y) * (2 * mod_);
}
constexpr itype::u64 mul(itype::u64 x, itype::u64 y) const noexcept {
Assume(x < 2 * mod_ && y < 2 * mod_);
return reduce(x, y);
}
constexpr bool same(itype::u64 x, itype::u64 y) const noexcept {
Assume(x < 2 * mod_ && y < 2 * mod_);
itype::u64 tmp= x - y;
return (tmp == 0) || (tmp == mod_) || (tmp == -mod_);
}
constexpr itype::u64 abs(itype::u64 x) const noexcept {
itype::u64 tmp= neg(x);
return val(x) > mod_ / 2 ? tmp : x;
}
constexpr itype::u64 norm(itype::u64 x) const noexcept { return x >= mod_ ? x - mod_ : x; }
};
}// namespace internal
template<itype::u32 mod_= 998244353> using StaticModint32= internal::ModintInterface<internal::StaticModint32Impl<mod_>>;
template<itype::u64 mod_= 998244353> using StaticModint64= internal::ModintInterface<internal::StaticModint64Impl<mod_>>;
template<itype::u64 mod_= 998244353> using StaticModint= internal::ModintInterface<internal::StaticModintImpl<mod_>>;
template<itype::u32 id= 0> using DynamicModint32= internal::ModintInterface<internal::DynamicModint32Impl, id>;
template<itype::u32 id= 0> using DynamicModint64= internal::ModintInterface<internal::DynamicModint64Impl, id>;
template<itype::u32 id= 0> using MontgomeryModint64= internal::ModintInterface<internal::MontgomeryModint64Impl, id>;
template<itype::u32 id= 0> using ThreadLocalDynamicModint32= internal::ModintInterface<internal::DynamicModint32Impl, id, true>;
template<itype::u32 id= 0> using ThreadLocalDynamicModint64= internal::ModintInterface<internal::DynamicModint64Impl, id, true>;
template<itype::u32 id= 0> using ThreadLocalMontgomeryModint64= internal::ModintInterface<internal::MontgomeryModint64Impl, id, true>;
}// namespace gsh
namespace gsh {
//@brief Find the largest x for which x * x <= n (https://rsk0315.hatenablog.com/entry/2023/11/07/221428)
constexpr itype::u32 IntSqrt32(const itype::u32 x) {
if(x == 0) return 0;
if(std::is_constant_evaluated()) {
itype::u32 low= 0, high= 0xffff;
while(low != high) {
itype::u32 mid= low + (high - low + 1) / 2;
if(mid * mid > x) high= mid - 1;
else low= mid;
}
return low;
} else {
itype::u32 tmp= static_cast<itype::u32>(std::sqrt(static_cast<ftype::f32>(x))) - 1;
return tmp + (tmp * (tmp + 2) < x);
}
}
constexpr itype::u64 IntSqrt64(const itype::u64 x) {
if(x == 0) return 0;
if(std::is_constant_evaluated()) {
itype::u64 low= 0, high= 0xffffffff;
while(low != high) {
itype::u64 mid= low + (high - low + 1) / 2;
if(mid * mid > x) high= mid - 1;
else low= mid;
}
return low;
} else {
itype::u64 tmp= static_cast<itype::u64>(std::sqrt(static_cast<ftype::f64>(x))) - 1;
return tmp + (tmp * (tmp + 2) < x);
}
}
namespace internal {
template<itype::u32> struct isSquareMod9360 {
// clang-format off
        constexpr static itype::u64 table[147] = {0x2001002010213u,0x200001000020001u,0x20100010000u,0x10000200000010u,0x200000001u,0x20000000010u,0x200000000010000u,0x1200000000u,0x20000u,0x2000002000201u,0x1000000201u,0x20002100000u,0x10000000010000u,0x1000000000200u,0x2000000000010u,0x2010002u,0x100001u,0x20002u,0x210u,0x1000200000200u,0x110000u,0x2000000u,0x201001100000000u,0x2000100000000u,0x2000002000000u,0x201u,
        0x20002u,0x10001000000002u,0x200000000000000u,0x2100000u,0x10012u,0x200020100000000u,0x20100000000u,0x2000000000010u,0x1000200100200u,0u,0x10001000000003u,0x1200000000u,0x10000000000000u,0x2000002000010u,0x21000000001u,0x20100000000u,0x10000000010000u,0x200000200000000u,0u,0x2001000010200u,0x1000020000u,0x20000u,0x12000000000000u,0x1000200000201u,0x2020000100000u,0x10000002010000u,0x1001000000000u,0x20000u,
        0x2000000u,0x1u,0x10000000130000u,0x2u,0x201000300000200u,0x2000000100010u,0x2000010u,0x200001000000001u,0x100000002u,0x2000000000000u,0x1000000000201u,0x2010000u,0x10000000000002u,0x200020100000000u,0x100020010u,0x10u,0x200u,0x20100100000u,0x1000010000u,0x201000200020200u,0x2000000u,0x2000000000002u,0x21000000000u,0x20000000000u,0x13000000000010u,0x1u,0x20000000002u,0x10000002010001u,0x200000200020000u,
        0x100020000u,0x2000200000000u,0x1000000000u,0x120000u,0x211000000000000u,0x1000200000200u,0x100000u,0x2010201u,0x1000020001u,0x10020000020000u,0u,0x200000001u,0x100010u,0x200000000000002u,0x201001200000000u,0x100020000u,0x2000210u,0x1000000201u,0x10000100100000u,0x200000002u,0x1000000000200u,0x2000000000010u,0x2000000000012u,0x200000000000000u,0x20100020000u,0x10000000000010u,0x1000000000200u,0x20000110000u,
        0x10000u,0x201000200000000u,0x2000100000000u,0x3000000000000u,0x1000100000u,0x20000000000u,0x10001000010002u,0x200000000020000u,0x2000000u,0x2010010u,0x200000000000001u,0x20100020000u,0x203000000000000u,0x200100000u,0x100000u,0x10001002000001u,0x1001200000000u,0u,0x2000000u,0x1000000201u,0x20000020000u,0x200000000010002u,0x200000000u,0x100000u,0x212u,0x200001000000000u,0x100030000u,0x200000010u,0x1000000000201u,
        0x2000000100000u,0x2000002u,0x1000000000000u,0x20000u,0x2000000000011u,0u,0u};
// clang-format on
constexpr static bool calc(const itype::u16 x) { return (table[x / 64] >> (x % 64)) & 1; }
};
}// namespace internal
constexpr bool isSquare32(const itype::u32 x) {
const itype::u32 tmp= IntSqrt32(x);
return tmp * tmp == x;
}
constexpr bool isSquare64(const itype::u64 x) {
if(!internal::isSquareMod9360<0>::calc(x % 9360)) return false;
const itype::u64 tmp= IntSqrt64(x);
return tmp * tmp == x;
}
template<class T, class U> constexpr auto Umod(const T& x, const U& m) {
if((m >= 0) ^ (x >= 0)) {
m= m >= 0 ? m : -m;
auto res= x % m + m;
res= res >= m ? res - m : res;
return static_cast<std::make_unsigned_t<decltype(res)>>(res);
} else {
auto res= x % m;
return static_cast<std::make_unsigned_t<decltype(res)>>(res);
}
}
template<class T> constexpr T IntPow(const T& x, itype::u64 e) {
T res= 1, pow= x;
while(e != 0) {
const T tmp= pow * pow;
if(e & 1) res*= pow;
pow= tmp;
e>>= 1;
}
return res;
}
template<class T, class U> constexpr T ModPow(const T& x, itype::u64 e, const U& mod) {
T res= 1, pow= x % mod;
while(e != 0) {
const T tmp= (pow * pow) % mod;
if(e & 1) res= (res * pow) % mod;
pow= tmp;
e>>= 1;
}
return res;
}
template<class T, class U> constexpr auto DivCeil(const T& a, const U& b) {
return (a + b - 1) / b;
}
// @brief Find the greatest common divisor as in std::gcd. (https://lpha-z.hatenablog.com/entry/2020/05/24/231500)
template<class T, class U> constexpr std::common_type_t<T, U> GCD(T x, U y) {
static_assert(!std::is_same_v<T, bool> && !std::is_same_v<U, bool> && std::is_integral_v<T> && std::is_integral_v<U>, "gsh::GCD / The input must be an integral type.");
if constexpr(std::is_same_v<T, U>) {
if constexpr(std::is_unsigned_v<T>) {
if(x == 0 || y == 0) return x | y;
const itype::i32 n= std::countr_zero(x);
const itype::i32 m= std::countr_zero(y);
const itype::i32 l= n < m ? n : m;
x>>= n;
y>>= m;
while(x != y) {
const T a= y - x, b= x - y;
const itype::i32 m= std::countr_zero(a), n= std::countr_zero(b);
Assume(m == n);
const T s= y < x ? b : a;
const T t= x < y ? x : y;
x= s >> m;
y= t;
}
return x << l;
} else {
return static_cast<T>(GCD<std::make_unsigned_t<T>, std::make_unsigned<T>>((x < 0 ? -x : x), (y < 0 ? -y : y)));
}
} else {
return GCD<std::common_type_t<T, U>, std::common_type_t<T, U>>(x, y);
}
}
// @brief Find the greatest common divisor of multiple numbers.
template<class T, class... Args> constexpr auto GCD(T x, Args... y) {
return GCD(x, GCD(y...));
}
// @brief Find the  least common multiple as in std::lcm.
template<class T, class U> constexpr auto LCM(T x, U y) {
return static_cast<std::common_type_t<T, U>>(x < 0 ? -x : x) / GCD(x, y) * static_cast<std::common_type_t<T, U>>(y < 0 ? -y : y);
}
// @brief Find the least common multiple of multiple numbers.
template<class T, class... Args> constexpr auto LCM(T x, Args... y) {
return LCM(x, LCM(y...));
}
namespace internal {
template<itype::u32> struct KthRootImpl {
// clang-format off
constexpr static itype::u64 pw3[] = {
1,3,9,27,81,243,729,2187,6561,19683,59049,177147,531441,1594323,4782969,14348907,43046721,129140163,387420489,1162261467,3486784401,10460353203,31381059609,94143178827,282429536481,847288609443,2541865828329,7625597484987,
22876792454961,68630377364883,205891132094649,617673396283947,1853020188851841,5559060566555523,16677181699666569,50031545098999707,150094635296999121,450283905890997363,1350851717672992089,4052555153018976267,12157665459056928801u
};
constexpr static itype::u64 pw5[] = {
1,5,25,125,625,3125,15625,78125,390625,1953125,9765625,48828125,244140625,1220703125,6103515625,30517578125,152587890625,762939453125,3814697265625,
19073486328125,95367431640625,476837158203125,2384185791015625,11920928955078125,59604644775390625,298023223876953125,1490116119384765625,7450580596923828125
};
constexpr static itype::u64 pw7[] = {
1,7,49,343,2401,16807,117649,823543,5764801,40353607,282475249,1977326743,13841287201,96889010407,678223072849,4747561509943,33232930569601,
232630513987207,1628413597910449,11398895185373143,79792266297612001,558545864083284007,3909821048582988049,8922003266371364727,7113790643470898241
};
constexpr static itype::u64 pw11[] = {
1,11,121,1331,14641,161051,1771561,19487171,214358881,2357947691,25937424601,285311670611,3138428376721,34522712143931,379749833583241,4177248169415651,45949729863572161,505447028499293771,5559917313492231481
};
constexpr static itype::u64 pw13[] = {
1,13,169,2197,28561,371293,4826809,62748517,815730721,10604499373,137858491849,1792160394037,23298085122481,302875106592253,3937376385699289,51185893014090757,665416609183179841,8650415919381337933
};
constexpr static itype::u64 pw17[] = {
1,17,289,4913,83521,1419857,24137569,410338673,6975757441,118587876497,2015993900449,34271896307633,582622237229761,9904578032905937,168377826559400929,2862423051509815793
};
constexpr static itype::u64 pw19[] = {
1,19,361,6859,130321,2476099,47045881,893871739,16983563041,322687697779,6131066257801,116490258898219,2213314919066161,42052983462257059,799006685782884121,15181127029874798299u
};
constexpr static itype::u64 pw23[] = {
1,23,529,12167,279841,6436343,148035889,3404825447,78310985281,1801152661463,41426511213649,952809757913927,21914624432020321,504036361936467383,11592836324538749809u
};
constexpr static itype::u64 pw29[] = {
1,29,841,24389,707281,20511149,594823321,17249876309,500246412961,14507145975869,420707233300201,12200509765705829,353814783205469041,10260628712958602189u
};
constexpr static itype::u64 pw31[] = {
1,31,961,29791,923521,28629151,887503681,27512614111,852891037441,26439622160671,819628286980801,25408476896404831,787662783788549761
};
constexpr static itype::u64 pw37[] = {
1,37,1369,50653,1874161,69343957,2565726409,94931877133,3512479453921,129961739795077,4808584372417849,177917621779460413,6582952005840035281
};
constexpr static ftype::f64 iv[] = {
0.0,0x1.fffffffffffffp-1,0x1.fffffffffffffp-2,0x1.5555555555554p-2,0x1.fffffffffffffp-3,0x1.9999999999999p-3,0x1.5555555555554p-3,0x1.2492492492491p-3,0x1.fffffffffffffp-4,0x1.c71c71c71c71bp-4,0x1.9999999999999p-4,0x1.745d1745d1745p-4
};
constexpr static itype::u64 lim[] = {
0,18446744073709551615u,4294967295,2642245,65535,7131,1625,565,255,138,84,56,40,30,23,19,15,13,11,10,9,8,7,6,6,5,5,5,4,4,4,4
};
constexpr static ftype::f64 rt3[] = {
0x1p+0,0x1.965fea53d6e3cp-1,0x1.428a2f98d728bp-1,0x1p-1,0x1.965fea53d6e3cp-2,0x1.428a2f98d728bp-2,0x1p-2,0x1.965fea53d6e3cp-3,0x1.428a2f98d728bp-3,0x1p-3,0x1.965fea53d6e3cp-4,0x1.428a2f98d728bp-4,0x1p-4,0x1.965fea53d6e3cp-5,0x1.428a2f98d728bp-5,0x1p-5,0x1.965fea53d6e3cp-6,
0x1.428a2f98d728bp-6,0x1p-6,0x1.965fea53d6e3cp-7,0x1.428a2f98d728bp-7,0x1p-7,0x1.965fea53d6e3cp-8,0x1.428a2f98d728bp-8,0x1p-8,0x1.965fea53d6e3cp-9,0x1.428a2f98d728bp-9,0x1p-9,0x1.965fea53d6e3cp-10,0x1.428a2f98d728bp-10,0x1p-10,0x1.965fea53d6e3cp-11,0x1.428a2f98d728bp-11,
0x1p-11,0x1.965fea53d6e3cp-12,0x1.428a2f98d728bp-12,0x1p-12,0x1.965fea53d6e3cp-13,0x1.428a2f98d728bp-13,0x1p-13,0x1.965fea53d6e3cp-14,0x1.428a2f98d728bp-14,0x1p-14,0x1.965fea53d6e3cp-15,0x1.428a2f98d728bp-15,0x1p-15,0x1.965fea53d6e3cp-16,0x1.428a2f98d728bp-16,0x1p-16,
0x1.965fea53d6e3cp-17,0x1.428a2f98d728bp-17,0x1p-17,0x1.965fea53d6e3cp-18,0x1.428a2f98d728bp-18,0x1p-18,0x1.965fea53d6e3cp-19,0x1.428a2f98d728bp-19,0x1p-19,0x1.965fea53d6e3cp-20,0x1.428a2f98d728bp-20,0x1p-20,0x1.965fea53d6e3cp-21,0x1.428a2f98d728bp-21,0x1p-21
};
// clang-format on
template<itype::u64 K> constexpr static itype::u64 calc(itype::u64 n) {
if constexpr(K == 3) {
const itype::u32 t= std::bit_width(n) - 1;
ftype::f64 x= rt3[t];
GSH_INTERNAL_UNROLL(3)
for(itype::u32 i= 0; i != 3; ++i) {
ftype::f64 h= 1.0 - n * x * x * x;
x+= x * h * ((1.0 / 3) + h * ((2.0 / 9) + h * ((14.0 / 81) + (h * ((35.0 / 243) + h * (91.0 / 729))))));
}
const itype::u64 r= 1.0 / x;
return r + (r < lim[K] && (r + 1) * (r + 1) * (r + 1) <= n) - (r > lim[K] || r * r * r > n);
} else if constexpr(K == 2) return IntSqrt64(n);
else if constexpr(K == 1) return n;
else if constexpr(K == 0) return 0xffffffffffffffff;
else if constexpr(K >= 12) {
itype::u64 res= 1 + (n >= (1ull << K));
if constexpr(K < 41) res+= (n >= pw3[K]);
if constexpr(K < 32) res+= (n >= (1ull << (2 * K)));
if constexpr(K < 28) res+= (n >= pw5[K]);
if constexpr(K < 25) res+= (n >= (pw3[K] << K));
if constexpr(K < 23) res+= (n >= pw7[K]);
if constexpr(K < 22) res+= (n >= (1ull << (3 * K)));
if constexpr(K < 21) res+= (n >= pw3[K] * pw3[K]);
if constexpr(K < 20) res+= (n >= (pw5[K] << K));
if constexpr(K < 19) res+= (n >= pw11[K]);
if constexpr(K < 18) res+= (n >= (pw3[K] << (2 * K))) + (n >= pw13[K]);
if constexpr(K < 17) res+= (n >= (pw7[K] << K)) + (n >= (pw3[K] * pw5[K]));
if constexpr(K < 16) res+= (n >= (1ull << (4 * K))) + (n >= pw17[K]) + (n >= ((pw3[K] * pw3[K]) << K)) + (n >= pw19[K]);
if constexpr(K < 15) res+= (n >= (pw5[K] << (2 * K))) + (n >= (pw3[K] * pw7[K])) + (n >= (pw11[K] << K)) + (n >= pw23[K]);
if constexpr(K < 14) res+= (n >= (pw3[K] << (3 * K))) + (n >= (pw5[K] * pw5[K])) + (n >= (pw13[K] << K)) + (n >= (pw3[K] * pw3[K] * pw3[K])) + (n >= (pw7[K] << (2 * K))) + (n >= pw29[K]) + (n >= ((pw3[K] * pw5[K]) << K));
if constexpr(K < 13) res+= (n >= pw31[K]) + (n >= (1ull << (5 * K))) + (n >= (pw3[K] * pw11[K])) + (n >= (pw17[K] << K)) + (n >= (pw5[K] * pw7[K])) + (n >= ((pw3[K] * pw3[K]) << (2 * K))) + (n >= pw37[K]) + (n >= (pw19[K] << K)) + (n >= (pw3[K] * pw13[K])) + (n >= (pw5[K] << (3 * K)));
return res;
} else {
const itype::u64 r= static_cast<itype::u64>(std::pow(n, iv[K]));
itype::u64 a= 1, p= r + 1;
GSH_INTERNAL_UNROLL(8)
for(itype::u64 e= K; e != 0; e>>= 1) {
if(e & 1) a*= p;
p*= p;
}
return r + (r < lim[K] && a <= n);
}
}
constexpr static itype::u64 calc2(itype::u64 n, itype::u64 k) {
if(n == 0) return 0;
// clang-format off
#ifdef F
#define GSH_INTERNAL_DEFINED_F
#pragma push_macro("F")
#undef F
#endif
#define F(x) case x : return calc<x>(n)
            switch (k) {
F(0);F(1);F(2);F(3);F(4);F(5);F(6);F(7);F(8);F(9);F(10);F(11);F(12);F(13);F(14);F(15);F(16);F(17);F(18);F(19);F(20);F(21);F(22);F(23);F(24);F(25);F(26);F(27);F(28);F(29);F(30);F(31);F(32);
F(33);F(34);F(35);F(36);F(37);F(38);F(39);F(40);F(41);F(42);F(43);F(44);F(45);F(46);F(47);F(48);F(49);F(50);F(51);F(52);F(53);F(54);F(55);F(56);F(57);F(58);F(59);F(60);F(61);F(62);F(63);
default : return 1;
            }
#undef F
#ifdef GSH_INTERNAL_DEFINED_F
#pragma pop_macro("F")
#endif
// clang-format on
}
};
}// namespace internal
constexpr itype::u64 KthRoot(itype::u64 n, itype::u64 k) {
return internal::KthRootImpl<0>::calc2(n, k);
}
constexpr itype::u64 LinearFloorSum(itype::u32 n, itype::u32 m, itype::u32 a, itype::u32 b) {
itype::u64 res= 0;
while(true) {
const itype::u32 p= a / m, q= b / m;
a%= m;
b%= m;
res+= static_cast<itype::u64>(n) * (n - 1) / 2 * p + static_cast<itype::u64>(n) * q;
const itype::u64 last= a * static_cast<itype::u64>(n) + b;
if(last < m) return res;
n= last / m;
b= last % m;
itype::u32 tmp= a;
a= m, m= tmp;
}
}
constexpr itype::u32 LinearModMin(itype::u32 n, itype::u32 m, itype::u32 a, itype::u32 b) {
itype::u32 res= 0;
bool z= true;
itype::u32 p= 1, q= 1;
while(a != 0) {
const itype::u32 e= (z ? a : m) - 1;
const itype::u32 d= m / a, r= m % a;
const itype::u32 g= d * p + q;
if((z ? b + 1 : m - b) > a) {
const itype::u32 t= (m - b + (z ? a : 0) - 1) / a;
const itype::u32 c= (t - z) * p + (z ? q : 0);
if(n <= c) {
const itype::u32 h= z ? 0 : a * ((n - 1) / p);
res+= (z ? h : -h);
break;
}
n-= c, b+= a * t - (z ? m : 0);
}
q= g, p= g - p;
res+= z ? e : -e;
m= a, a= r, b= e - b, z= !z;
}
res+= (z ? b : -b);
return res;
}
class QuotientsList {
const itype::u64 x;
const itype::u32 sq;
itype::u32 m;
public:
using value_type= itype::u32;
constexpr QuotientsList(itype::u64 n): x(n), sq(IntSqrt64(n)) { m= (itype::u64(sq) * sq + sq <= n ? sq : sq - 1); }
constexpr itype::u32 size() const noexcept { return sq + m; }
constexpr itype::u32 iota_limit() const noexcept { return sq; }
constexpr itype::u32 div_limit() const noexcept { return m; }
constexpr itype::u64 val() const noexcept { return x; }
constexpr itype::u64 operator[](itype::u32 n) { return n < m ? n + 1 : x / (sq - (n - m)); }
};
namespace internal {
template<class T> class BinCoeffTable {
T mint;
Arr<typename T::value_type> fac, finv;
public:
using value_type= typename T::value_type;
constexpr BinCoeffTable(itype::u32 mx, value_type mod): fac(mx), finv(mx) {
if(mx > mod) throw Exception("gsh::internal::BinCoeffTable:::BinCoeffTable / The table size cannot be larger than mod.");
mint.set(mod);
fac[0]= mint.raw(1), finv[0]= mint.raw(1);
if(mx > 1) {
fac[1]= mint.raw(1), finv[1]= mint.raw(1);
if(mx > 2) {
auto cnt= mint.raw(1);
for(itype::u32 i= 2; i != mx; ++i) {
cnt= mint.inc(cnt);
fac[i]= mint.mul(fac[i - 1], cnt);
}
finv.back()= mint.inv(fac.back());
for(itype::u32 i= mx - 1; i != 2; --i) {
finv[i - 1]= mint.mul(finv[i], cnt);
cnt= mint.dec(cnt);
}
}
}
}
constexpr value_type operator()(itype::u32 n, itype::u32 k) const {
if(n < k) return 0;
else return mint.val(mint.mul(mint.mul(fac[n], finv[k]), finv[n - k]));
}
};
template<IsStaticModint T> class BinCoeffTable<T> {
[[no_unique_address]] T mint;
Arr<typename T::value_type> fac, finv;
public:
using value_type= typename T::value_type;
constexpr BinCoeffTable(itype::u32 mx): fac(mx), finv(mx) {
if(mx > mint.mod()) throw Exception("gsh::internal::BinCoeffTable:::BinCoeffTable / The table size cannot be larger than mod.");
fac[0]= mint.raw(1), finv[0]= mint.raw(1);
if(mx > 1) {
fac[1]= mint.raw(1), finv[1]= mint.raw(1);
if(mx > 2) {
auto cnt= mint.raw(1);
for(itype::u32 i= 2; i != mx; ++i) {
cnt= mint.inc(cnt);
fac[i]= mint.mul(fac[i - 1], cnt);
}
finv.back()= mint.inv(fac.back());
for(itype::u32 i= mx - 1; i != 2; --i) {
finv[i - 1]= mint.mul(finv[i], cnt);
cnt= mint.dec(cnt);
}
}
}
}
constexpr value_type operator()(itype::u32 n, itype::u32 k) const {
if(n < k) return 0;
else return mint.val(mint.mul(mint.mul(fac[n], finv[k]), finv[n - k]));
}
};
}// namespace internal
using BinCoeffTable32= internal::BinCoeffTable<internal::DynamicModint32Impl>;
using BinCoeffTable64= internal::BinCoeffTable<internal::DynamicModint64Impl>;
template<itype::u64 mod= 998244353> using BinCoeffTableStaticMod= internal::BinCoeffTable<internal::StaticModintImpl<mod>>;
}// namespace gsh
namespace gsh {
namespace internal {
template<itype::u32> struct IsPrime8 {
constexpr static itype::u64 flag_table[4]= {2891462833508853932u, 9223979663092122248u, 9234666804958202376u, 577166812715155618u};
GSH_INTERNAL_INLINE constexpr static bool calc(const itype::u8 n) noexcept { return (flag_table[n / 64] >> (n % 64)) & 1; }
};
template<itype::u32> struct IsPrime16 {
constexpr static itype::u64 flag_table[512]= {
0x816d129a64b4cb6eu, 0x2196820d864a4c32u, 0xa48961205a0434c9u, 0x4a2882d129861144u, 0x834992132424030u, 0x148a48844225064bu, 0xb40b4086c304205u, 0x65048928125108a0u, 0x80124496804c3098u, 0xc02104c941124221u, 0x804490000982d32u, 0x220825b082689681u, 0x9004265940a28948u, 0x6900924430434006u, 0x12410da408088210u, 0x86122d22400c060u, 0x110d301821b0484u, 0x14916022c044a002u, 0x92094d204a6400cu, 0x4ca2100800522094u, 0xa48b081051018200u, 0x34c108144309a25u, 0x2084490880522502u, 0x241140a218003250u, 0xa41a00101840128u, 0x2926000836004512u, 0x10100480c0618283u, 0xc20c26584822006du, 0x4520582024894810u, 0x10c0250219002488u, 0x802832ca01140868u, 0x60901300264b0400u,
0x32100100d0258082u, 0x430800112186430cu, 0x92900c10480424u, 0x24880906002d2043u, 0x530082090932c040u, 0x4000814196800880u, 0x2058489608481048u, 0x926094022080c329u, 0x5a0104422812000u, 0xa042049019040u, 0xc02c801348348924u, 0x800084524002982u, 0x4d0048452043698u, 0x1865328244908a00u, 0x28024001020a0090u, 0x861104309204a440u, 0xc90804522c004208u, 0x4424990912486084u, 0x1000211403002400u, 0x4040208805321a01u, 0x6030014084c30906u, 0xa2020c9011680218u, 0x8224148929860004u, 0x880190480084102u, 0x20004a442681210u, 0x120100100c061061u, 0x6512422194032010u, 0x140128040a0c9418u, 0x14000d040a40a29u, 0x4882402d20410490u, 0x24080130100020c1u, 0x8229020024845904u,
0x4816814802586100u, 0xa0ca000611210010u, 0x4200b09104000240u, 0x2514480906810c04u, 0x860a00a011252092u, 0x84520004802c10cu, 0x22130406980032u, 0x1282441481480482u, 0xd028804340101824u, 0x2c00d86424812004u, 0x20000a241081209u, 0x180110c04120ca41u, 0x20941220a41804a4u, 0x48044320240a083u, 0x8a6086400c001800u, 0x82010512886400u, 0x4096110c101a24au, 0x840b40160008801u, 0x494400880030106u, 0x2520c028029208au, 0x264848000844201u, 0x2122404430004832u, 0x20d004a0c3080200u, 0x5228004040161840u, 0x810180114820890u, 0x809320a00a408209u, 0x10500522000c008u, 0x820c06114010u, 0x908028009a44904bu, 0x28024309064a04u, 0x4480096500180134u, 0x1448618202240003u,
0x5108340028120041u, 0x6084892890120504u, 0x8249402610491012u, 0x8840240a01109100u, 0x2ca2500004104c10u, 0x125001b00a489040u, 0x9228a00904a40008u, 0x4120022110430002u, 0x520c0408003281u, 0x8101021020844921u, 0x6984010122404810u, 0x884402c80130c1u, 0x6112c02d02010cu, 0x812014030c000a0u, 0x840140948000200bu, 0xb00841000320040u, 0x41848a2906010024u, 0x80034c9408081080u, 0x5020204140964001u, 0x20a44040a2892522u, 0x104a212001288602u, 0x4225044008140008u, 0x2100920410432102u, 0x84030922184ca011u, 0x124228204108941u, 0x900c10884080814u, 0x368000028a41b042u, 0x200009124a04904u, 0x806080102924194u, 0x80892816d0010009u, 0x500c900168000060u, 0x4130424080400120u,
0x49400681252000u, 0x1820a00049120108u, 0x28241000a6010530u, 0x12880020c8200200u, 0x420126020092900cu, 0x102422404004916u, 0x1008801a0c8088u, 0x1169008844940260u, 0x841324a0120830u, 0x30002810c0650082u, 0xc801061101200304u, 0xc82100820c20080u, 0xb0004006520c0213u, 0x1004869801104061u, 0x4180416014920884u, 0x204140228104101au, 0x1060340841005229u, 0x884004010012800u, 0x252040448209042u, 0xd820004200800u, 0x4020480510024082u, 0xc0240601000099u, 0x844101221048268u, 0x916d020a6400004u, 0x92090c20024124c9u, 0x4309004000001240u, 0x24110102982084u, 0x3041089003002443u, 0x100882804c205824u, 0x2010094106812524u, 0x244a001080441018u, 0xc00030802894010du,
0x900020c84106002u, 0x20c2041008018202u, 0x1100001804060968u, 0xc028221100b0890u, 0x24100260008b610u, 0x8024201a21244a01u, 0x2402d00024400u, 0xa69020001020948bu, 0x16186112c001340u, 0x4830810402104180u, 0x108a218050282048u, 0x4248101009100804u, 0x520c06092820ca0u, 0x82080400014020d2u, 0x484180480002822du, 0x84030404910010u, 0x22c06400006804c2u, 0x9100860944320840u, 0x2400486400012802u, 0x8652210043009010u, 0x8808204020908b41u, 0x6084020020134404u, 0x1008003040249081u, 0x4320041001020808u, 0x4c800168129040b4u, 0x10404912c0080018u, 0x104c248941001a24u, 0x41204a0910520400u, 0x610081411692248u, 0x4000100028848024u, 0x2806480826080110u, 0x200a048442011400u,
0x1224820008820100u, 0x4109040a0404004u, 0x10802c2010402290u, 0x8101005804004328u, 0x4832120094810u, 0xa0106c000044a442u, 0xc948808300804844u, 0x4b0100502000000u, 0x408409210290413u, 0x1900201900228244u, 0x41008a6090810120u, 0xa2020004104502c0u, 0x4201204921104009u, 0x422014414002c30u, 0x1080210489089202u, 0x4804140200105u, 0x1325864b0400912u, 0x80c1090441009008u, 0x124009a00900861u, 0x806820526020812u, 0x2418002048200008u, 0x9001100020348u, 0x4009801104a0184u, 0x80812000c0008618u, 0x4a0cb40005301004u, 0x4420002802912982u, 0xa2014080912c00c0u, 0x80020c309041200u, 0x2c00000422100c02u, 0x32120000c0008611u, 0x5005024040808940u, 0x4d120a60a4826086u,
0x1402098012089080u, 0x9044008a20240148u, 0x12d10002010404u, 0x248121320040040au, 0x8908040220841908u, 0x4482186802022480u, 0x8001280040210042u, 0x20c801140208245u, 0x2020400190402400u, 0x2009400019282050u, 0x820804060048008u, 0x2424110034094930u, 0x2920400c2410082u, 0x100a0020c008024u, 0x100d02104416006u, 0x1291048412480001u, 0x1841120044240008u, 0x2004520080410c26u, 0x218482090240009u, 0x8a0014d009a20300u, 0x40149820004a2584u, 0x144000000005a200u, 0x90084802c205801u, 0x41b0020802912020u, 0x218001009003008u, 0x844240000020221u, 0xc021244b2006012u, 0x20500420c84080c0u, 0x5329040b04b00005u, 0x2920820030486100u, 0x1043202253001600u, 0x4000d204800048u,
0x8040029800344a2u, 0x84092830406404c0u, 0xc000920221805044u, 0x800822886010u, 0x2081009683048418u, 0x5100848845000205u, 0x944b4186512020u, 0x80584c2011080080u, 0x805008920060304u, 0x982004000900522u, 0x20c241a000000050u, 0xd021264008160008u, 0x4402004190810890u, 0x49009860a0c1008u, 0x8920300804a0c800u, 0x800402c22110084u, 0x200901024801b002u, 0x4260028000040304u, 0x20944104a2130u, 0xa480218212002401u, 0x1840a09104021020u, 0x500096906020004u, 0x480000010258u, 0xc801340020920300u, 0x2080420830084820u, 0x212400401689091u, 0x1100a00108120061u, 0xc00922404482104u, 0x9612010000048401u, 0x8828228841a00140u, 0x114122480424400u, 0x108104101a609042u,
0x240028329060848u, 0x4010800510806424u, 0x2009018442080202u, 0x1340301160005004u, 0x4520080900810402u, 0x2080c269061104au, 0x200040260009121u, 0x884480806080c00u, 0x205a00a480000211u, 0x9000204048800u, 0x400c82014490814u, 0x101200805940a091u, 0x4000065808000u, 0x6084032100194080u, 0x808061121a2404c0u, 0x820124209040208u, 0xa0010120900434u, 0x340240929108000bu, 0x4000021961108840u, 0x2104086880c02504u, 0x84010ca000042280u, 0x8a20008a08004120u, 0x882110404884800u, 0x100040a449098640u, 0x800c805004a20101u, 0x41121801a0824800u, 0x1240041480401u, 0x168000200148800u, 0x808308224a0820u, 0x34000000c2010489u, 0x4a41020228820004u, 0x424800902820590u,
0x1401288092010041u, 0x4304b0104c205000u, 0x44000201049021a4u, 0x2042000608640048u, 0x5020004a01920208u, 0x800090422902532u, 0x3200051001218011u, 0xc10d240808948808u, 0x4121840200b4080u, 0x82c1052610402200u, 0x841220224300100u, 0x2812d225001a4824u, 0x200413040040042u, 0x890884d124201300u, 0xa4184400520480u, 0x2042091091200600u, 0x4040840028304024u, 0x4004080904100880u, 0x8000000219002208u, 0x402090012102022cu, 0x120584834000c00u, 0x90001480200443u, 0x30020400000116du, 0x65004a0530884010u, 0x8003288418082410u, 0x1969100040b04220u, 0x4c20480000004u, 0x9608252200050001u, 0x12910d000220204u, 0x44160104100860a0u, 0x8440488202280210u, 0x4000048028229020u,
0x6010032980002404u, 0x205100a081000048u, 0x920420410100d10cu, 0x504420092100000u, 0x2052201080408601u, 0xd000020a48100021u, 0x4800000480484112u, 0x1043002400042209u, 0x82c201244000a60u, 0x806400984004420u, 0x12980020804000c1u, 0xc048840020a21048u, 0x82980812902010u, 0xc328000304a00au, 0x40040804104244u, 0x480032100100500u, 0x408040010691288u, 0x1820044948840204u, 0x2010830806402u, 0x1088412008491252u, 0xd005860100340848u, 0x4102402184830000u, 0x5120a240488010u, 0x1840209001004900u, 0x880400522024002u, 0x8201050018201082u, 0x129908104005840u, 0xa20140220064a0u, 0x94806000000d0418u, 0x120c30800d108260u, 0x2120c04012000020u, 0x203448010410258u,
0xc044000829901304u, 0x1801a0026002100u, 0x320020140a201413u, 0x8009204240000861u, 0x6800426080810106u, 0x8002048042088290u, 0x810c009800040b09u, 0x92032884484406u, 0x2810c000a408001u, 0x920029028045108u, 0xca0810900006010u, 0x208028020009a400u, 0x4148104020200u, 0x120406012110904u, 0x860a080011403048u, 0xd001048160040000u, 0x200a0090184102u, 0x10ca6480080106c1u, 0x5020820148809904u, 0x22902084804890u, 0x8610242018040019u, 0x4410122400c240u, 0x106120024100816u, 0x80104d0212009008u, 0x1104300225040u, 0x140100000a2130u, 0xa2910c1048410u, 0x490c120120008a01u, 0x6004014800810420u, 0x44a4810080c1280u, 0x5045844028001028u, 0x980014406106010u,
0x9000a042018600u, 0x8008004140229005u, 0x4930580100c00802u, 0x80020c0241001409u, 0x9005100824008940u, 0x61120008820a4032u, 0x2410042200210400u, 0x4020001001040a08u, 0x12902022880484u, 0x140b400401240653u, 0x80c90100d028260u, 0x2480800914000920u, 0x2001440201400082u, 0x41100a4084400cu, 0x2020084480090530u, 0x2000212043490002u, 0x208044008b60100u, 0x2410084080410180u, 0x12c0098612042000u, 0x8920020004148121u, 0x6900d100801244b4u, 0x418001242008040u, 0x228040221064900u, 0x820006810c00184u, 0x2481011091080040u, 0x100086884c10d204u, 0x40908a0014020c80u, 0x245800a480212018u, 0x484130c160101020u, 0x502094000094802u, 0x21824204a208211u, 0x300040a0c22100cu,
0x2100020404484806u, 0x12020c0018008480u, 0x8941108205140001u, 0x48840121a4400812u, 0x1400280240601002u, 0xc200125120a04008u, 0x4c128940301a0100u, 0xa001011400008002u, 0x140061821221821u, 0x430024804900080u, 0x448082488050008u, 0x8000060224u, 0x4820a0090116510u, 0x2920424486004c3u, 0x8029061840808844u, 0x2110c84400000110u, 0x141001a04b003089u, 0x65200040940200u, 0x2012812022400ca2u, 0x88080010010482u, 0x4140804204801100u, 0x424802c32400014u, 0x83200091000019u, 0x4040840109204005u, 0x2090414000112020u, 0x618489290400000u, 0x1024340148808108u, 0x2d06180420000420u, 0x220a009000011090u, 0x101841100220001u, 0x122004400882000u, 0x1120060240a600u,
0x1928008a04a0c801u, 0x9121224a0520080u, 0x2400040048012408u, 0x4048040008840240u, 0x8148801220a6090u, 0x90c02000d3080201u, 0xa08b00100001024u, 0x20000901008000a0u, 0x8402042400250252u, 0x40a00240921024u, 0x22010804110822u, 0x3000219009001442u, 0x900922000c00006cu, 0x20c02000402810u, 0x1212058201400090u, 0x812802806104c109u, 0x2986100804490024u, 0x908849300a218041u, 0x941808129044100u, 0x4010004010124000u, 0x2040210280050248u, 0x48900060205800u, 0x4400004880c02880u, 0x212000609000280u, 0x1245108308100001u, 0x2020004404082c00u, 0x20c80500012010c0u, 0x224001008109804u, 0x2412886100884016u, 0x61008004200a680u, 0x8104205000a04048u, 0x1801008001840a4u};
GSH_INTERNAL_INLINE constexpr static bool calc(const itype::u16 x) noexcept { return x == 2 || (x % 2 == 1 && (flag_table[x / 128] & (1ull << (x % 128 / 2)))); }
};
template<itype::u32> struct IsPrime32 {
// clang-format off
        constexpr static itype::u16 bases[] = {
1216,1836,8885,4564,10978,5228,15613,13941,1553,173,3615,3144,10065,9259,233,2362,6244,6431,10863,5920,6408,6841,22124,2290,45597,6935,4835,7652,1051,445,5807,842,1534,22140,1282,1733,347,6311,14081,11157,186,703,9862,15490,1720,17816,10433,49185,2535,9158,2143,2840,664,29074,24924,1035,41482,1065,10189,8417,130,4551,5159,48886,
786,1938,1013,2139,7171,2143,16873,188,5555,42007,1045,3891,2853,23642,148,3585,3027,280,3101,9918,6452,2716,855,990,1925,13557,1063,6916,4965,4380,587,3214,1808,1036,6356,8191,6783,14424,6929,1002,840,422,44215,7753,5799,3415,231,2013,8895,2081,883,3855,5577,876,3574,1925,1192,865,7376,12254,5952,2516,20463,186,
5411,35353,50898,1084,2127,4305,115,7821,1265,16169,1705,1857,24938,220,3650,1057,482,1690,2718,4309,7496,1515,7972,3763,10954,2817,3430,1423,714,6734,328,2581,2580,10047,2797,155,5951,3817,54850,2173,1318,246,1807,2958,2697,337,4871,2439,736,37112,1226,527,7531,5418,7242,2421,16135,7015,8432,2605,5638,5161,11515,14949,
748,5003,9048,4679,1915,7652,9657,660,3054,15469,2910,775,14106,1749,136,2673,61814,5633,1244,2567,4989,1637,1273,11423,7974,7509,6061,531,6608,1088,1627,160,6416,11350,921,306,18117,1238,463,1722,996,3866,6576,6055,130,24080,7331,3922,8632,2706,24108,32374,4237,15302,287,2296,1220,20922,3350,2089,562,11745,163,11951};
// clang-format on
GSH_INTERNAL_INLINE constexpr static bool calc(const itype::u32 x) noexcept {
internal::MontgomeryModint64Impl mint;
mint.set(x);
const itype::u32 h= x * 0xad625b89;
itype::u32 d= x - 1;
auto pow= mint.raw(bases[h >> 24]);
itype::u32 s= std::countr_zero(d);
d>>= s;
const auto one= mint.one(), mone= mint.neg(one);
auto cur= one;
while(d) {
auto tmp= mint.mul(pow, pow);
if(d & 1) cur= mint.mul(cur, pow);
pow= tmp;
d>>= 1;
}
if(mint.same(cur, one)) return true;
while(--s && !mint.same(cur, mone)) cur= mint.mul(cur, cur);
return mint.same(cur, mone);
}
};
template<itype::u32> struct IsPrime64 {
GSH_INTERNAL_INLINE constexpr static bool calc(const itype::u64 x) noexcept {
internal::MontgomeryModint64Impl mint;
mint.set(x);
const itype::u32 S= std::countr_zero(x - 1);
const itype::u64 D= (x - 1) >> S;
const auto one= mint.one(), mone= mint.neg(one);
auto test2= [&](itype::u64 base1, itype::u64 base2) {
auto a= one, b= one;
auto c= mint.build(base1), d= mint.build(base2);
itype::u64 ex= D;
while(ex) {
auto e= mint.mul(c, c), f= mint.mul(d, d);
if(ex & 1) a= mint.mul(a, e), b= mint.mul(b, f);
c= e, d= f;
ex>>= 1;
}
bool res1= mint.same(a, one) || mint.same(a, mone);
bool res2= mint.same(b, one) || mint.same(b, mone);
if(!(res1 && res2)) {
for(itype::u32 i= 0; i != S - 1; ++i) {
a= mint.mul(a, a), b= mint.mul(b, b), c= mint.mul(c, c);
res1|= mint.same(a, mone), res2|= mint.same(b, mone);
}
if(!res1 || !res2) return false;
}
return true;
};
auto test3= [&](itype::u64 base1, itype::u64 base2, itype::u64 base3) {
auto a= one, b= one, c= one;
auto d= mint.build(base1), e= mint.build(base2), f= mint.build(base3);
itype::u64 ex= D;
while(ex) {
const auto g= mint.mul(d, d), h= mint.mul(e, e), i= mint.mul(f, f);
if(ex & 1) a= mint.mul(a, d), b= mint.mul(b, e), c= mint.mul(c, f);
d= g, e= h, f= i;
ex>>= 1;
}
bool res1= mint.same(a, one) || mint.same(a, mone);
bool res2= mint.same(b, one) || mint.same(b, mone);
bool res3= mint.same(c, one) || mint.same(c, mone);
if(!(res1 && res2 && res3)) {
for(itype::u32 i= 0; i != S - 1; ++i) {
a= mint.mul(a, a), b= mint.mul(b, b), c= mint.mul(c, c);
res1|= mint.same(a, mone), res2|= mint.same(b, mone), res3|= mint.same(c, mone);
}
if(!res1 || !res2 || !res3) return false;
}
return true;
};
auto test4= [&](itype::u64 base1, itype::u64 base2, itype::u64 base3, itype::u64 base4) {
auto a= one, b= one, c= one, d= one;
auto e= mint.build(base1), f= mint.build(base2), g= mint.build(base3), h= mint.build(base4);
itype::u64 ex= D;
while(ex) {
auto i= mint.mul(e, e), j= mint.mul(f, f), k= mint.mul(g, g), l= mint.mul(h, h);
if(ex & 1) a= mint.mul(a, e), b= mint.mul(b, f), c= mint.mul(c, g), d= mint.mul(d, h);
e= i, f= j, g= k, h= l;
ex>>= 1;
}
bool res1= mint.same(a, one) || mint.same(a, mone);
bool res2= mint.same(b, one) || mint.same(b, mone);
bool res3= mint.same(c, one) || mint.same(c, mone);
bool res4= mint.same(d, one) || mint.same(d, mone);
if(!(res1 && res2 && res3 && res4)) {
for(itype::u32 i= 0; i != S - 1; ++i) {
a= mint.mul(a, a), b= mint.mul(b, b), c= mint.mul(c, c), d= mint.mul(d, d);
res1|= mint.same(a, mone), res2|= mint.same(b, mone), res3|= mint.same(c, mone), res4|= mint.same(d, mone);
}
if(!res1 || !res2 || !res3 || !res4) return false;
}
return true;
};
if(x < 585226005592931977ull) {
if(x < 7999252175582851ull) {
if(x < 350269456337ull) return test3(4230279247111683200ull, 14694767155120705706ull, 16641139526367750375ull);
else if(x < 55245642489451ull) return test4(2ull, 141889084524735ull, 1199124725622454117ull, 11096072698276303650ull);
else return test2(2ull, 4130806001517ull) && test3(149795463772692060ull, 186635894390467037ull, 3967304179347715805ull);
} else return test3(2ull, 123635709730000ull, 9233062284813009ull) && test3(43835965440333360ull, 761179012939631437ull, 1263739024124850375ull);
} else return test3(2ull, 325ull, 9375ull) && test4(28178ull, 450775ull, 9780504ull, 1795265022ull);
}
};
}// namespace internal
// @brief Prime number determination
constexpr bool IsPrime(const itype::u64 x) noexcept {
if(x < 65536u) {
return internal::IsPrime16<0>::calc(x);
} else {
if(x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0 || x % 11 == 0 || x % 13 == 0 || x % 17 == 0 || x % 19 == 0) return false;
if(x <= 0xffffffff) return internal::IsPrime32<0>::calc(x);
else return internal::IsPrime64<0>::calc(x);
}
}
constexpr itype::u32 CountPrimes(itype::u64 N) {
if(N <= 1) return 0;
const itype::u32 v= IntSqrt64(N);
itype::u32 s= (v + 1) / 2;
itype::u64* const invs= new itype::u64[s];
itype::u32* const smalls= new itype::u32[s];
itype::u32* const larges= new itype::u32[s];
itype::u32* const roughs= new itype::u32[s];
bool* const smooth= new bool[v + 1];
for(itype::u32 i= 0; i != v; ++i) smooth[i]= false;
for(itype::u32 i= 0; i != s; ++i) smalls[i]= i;
for(itype::u32 i= 0; i != s; ++i) roughs[i]= 2 * i + 1;
for(itype::u32 i= 0; i != s; ++i) invs[i]= (ftype::f64)N / roughs[i];
for(itype::u32 i= 0; i != s; ++i) larges[i]= (invs[i] - 1) / 2;
itype::u32 pc= 0;
for(itype::u64 p= 3; p * p <= v; p+= 2) {
if(smooth[p]) continue;
for(itype::u64 i= p * p; i <= v; i+= 2 * p) smooth[i]= true;
smooth[p]= true;
const auto divide_p= [invp= 0xffffffffffffffffu / p + 1](itype::u64 inv_j) -> itype::u64 {
return (itype::u128(inv_j) * invp) >> 64;
};
itype::u32 ns= 0;
itype::u32 k= 0;
GSH_INTERNAL_UNROLL(16)
for(; true; ++k) {
const itype::u32 j= roughs[k];
if(j * p > v) break;
if(smooth[j]) continue;
larges[ns]= larges[k] - larges[smalls[j * p / 2] - pc] + pc;
invs[ns]= invs[k];
roughs[ns]= roughs[k];
++ns;
}
GSH_INTERNAL_UNROLL(16)
for(; k < s; ++k) {
const itype::u32 j= roughs[k];
if(smooth[j]) continue;
larges[ns]= larges[k] - smalls[(divide_p(invs[k]) - 1) / 2] + pc;
invs[ns]= invs[k];
roughs[ns]= roughs[k];
++ns;
}
s= ns;
itype::u64 i= (v - 1) / 2;
for(itype::u64 j= (divide_p(v) - 1) | 1; j >= p; j-= 2) {
const itype::u32 d= smalls[j / 2] - pc;
for(; i >= j * p / 2; --i) smalls[i]-= d;
}
++pc;
}
itype::u32 ret= 1;
ret+= larges[0] + s * (s - 1) / 2 + (pc - 1) * (s - 1);
for(itype::u32 k= 1; k < s; ++k) ret-= larges[k];
for(itype::u32 k1= 1; k1 < s; ++k1) {
const itype::u64 p= roughs[k1];
const auto divide_p= [invp= 0xffffffffffffffffu / p + 1](itype::u64 inv_j) -> itype::u64 {
return (itype::u128(inv_j) * invp) >> 64;
};
const itype::u32 k2_max= smalls[(divide_p(invs[k1]) - 1) / 2] - pc;
if(k2_max <= k1) break;
for(itype::u32 k2= k1 + 1; k2 <= k2_max; ++k2) ret+= smalls[(divide_p(invs[k2]) - 1) / 2];
ret-= (k2_max - k1) * (pc + k1 - 1);
}
delete[] invs;
delete[] smalls;
delete[] larges;
delete[] roughs;
delete[] smooth;
return ret;
}
//constexpr auto EnumeratePrimes(itype::u32 N, itype::u32 gap, itype::u32 start) {}
namespace internal {
#ifndef GSH_USE_COMPILE_TIME_CALCULATION
struct TinyPrimesT {
itype::u16 table[6548]= {
#define GSH_INTERNAL_INCLUDE_TINYPRIMES "internal/TinyPrimes.txt"
#include GSH_INTERNAL_INCLUDE_TINYPRIMES
, 0, 0, 0, 0, 0, 0};
};
template<itype::u32> constexpr TinyPrimesT TinyPrimes;
#else
template<itype::u32 id> constexpr auto TinyPrimes= []() {
struct {
itype::u16 table[6548]= {};
} res;
itype::u32 cnt= 0;
for(itype::u32 i= 0; i != (1 << 16); ++i) {
if(IsPrime16<id>::calc(i)) res.table[cnt++]= i;
}
return res;
}();
#endif
template<itype::u32 id> constexpr auto InvPrime= []() {
struct {
itype::u64 table[6548]= {};
} res;
for(itype::u32 i= 0; i != 6542; ++i) {
res.table[i]= 0xffffffffffffffff / TinyPrimes<id>.table[i] + 1;
}
return res;
}();
constexpr itype::u64* FactorizeSub64(itype::u64 n, itype::u64* res) noexcept {
Assume(n % 2 != 0 && n % 3 != 0 && n % 5 != 0 && n % 7 != 0 && n % 11 != 0 && n % 13 != 0 && n % 17 != 0 && n % 19 != 0);
if(IsPrime(n)) {
*(res++)= n;
return res;
}
if(n <= 0xffffffff) {
for(itype::u32 i= 8; n >= 529; ++i) {
itype::u64 m= InvPrime<0>.table[i];
if(m * n < m) {
itype::u64 p= TinyPrimes<0>.table[i];
do {
*(res++)= p;
n= (static_cast<itype::u128>(m) * n) >> 64;
} while(m * n < m);
}
}
if(n != 1) *(res++)= n;
return res;
}
itype::u64 m= n;
{
internal::MontgomeryModint64Impl mint;
mint.set(n);
for(itype::u32 k= 2; m == n; ++k) {
itype::u64 f= mint.raw(k);
for(itype::u32 i= 1;; i+= 64) {
itype::u64 prev= f;
itype::u64 s= mint.one();
for(itype::u32 j= 0; j != 64; ++j) {
f= mint.pow(f, i + j);
s= mint.mul(s, mint.dec(f));
}
itype::u64 g= GCD(mint.val(s), n);
if(g == 1) continue;
if(g == n) {
g= 1;
f= prev;
for(itype::u32 j= 0; g == 1; ++j) {
f= mint.pow(f, i + j);
g= GCD(mint.val(mint.dec(f)), n);
}
}
m= g;
break;
}
}
}
if(n / m < 529) *(res++)= n / m;
else res= FactorizeSub64(n / m, res);
if(m < 529) {
*(res++)= m;
return res;
} else return FactorizeSub64(m, res);
}
}// namespace internal
constexpr Arr<itype::u64> Factorize(itype::u64 n) {
if(n <= 1) [[unlikely]]
return {};
itype::u64 res[64];
itype::u64* p= res;
{
Assume(n != 0);
itype::u32 rz= std::countr_zero(n);
n>>= rz;
for(itype::u32 i= 0; i != rz; ++i) *(p++)= 2;
}
{
const bool a= n % 3 == 0, b= n % 5 == 0, c= n % 7 == 0, d= n % 11 == 0, e= n % 13 == 0, f= n % 17 == 0, g= n % 19 == 0;
if(a) [[unlikely]] {
do {
n/= 3;
*(p++)= 3;
} while(n % 3 == 0);
}
if(b) [[unlikely]] {
do {
n/= 5;
*(p++)= 5;
} while(n % 5 == 0);
}
if(c) [[unlikely]] {
do {
n/= 7;
*(p++)= 7;
} while(n % 7 == 0);
}
if(d) [[unlikely]] {
do {
n/= 11;
*(p++)= 11;
} while(n % 11 == 0);
}
if(e) [[unlikely]] {
do {
n/= 13;
*(p++)= 13;
} while(n % 13 == 0);
}
if(f) [[unlikely]] {
do {
n/= 17;
*(p++)= 17;
} while(n % 17 == 0);
}
if(g) [[unlikely]] {
do {
n/= 19;
*(p++)= 19;
} while(n % 19 == 0);
}
}
if(n >= 529) [[likely]] {
p= internal::FactorizeSub64(n, p);
} else {
*p= n;
p+= n != 1;
}
return {res, p};
}
}// namespace gsh
namespace gsh {
namespace internal {
template<class D> class UnionFindImpl {
D& derived() noexcept { return static_cast<D&>(*this); }
const D& derived() const noexcept { return static_cast<const D&>(*this); }
public:
constexpr itype::u32 size() const noexcept { return derived().parent.size(); }
[[nodiscard]] constexpr bool empty() const noexcept { return derived().parent.empty(); }
constexpr itype::u32 leader(itype::u32 n) {
#ifndef NDEBUG
if(n >= derived().parent.size()) throw gsh::Exception("gsh::internal::UnionFindImpl::leader / The index is out of range. ( n=", n, ", size=", derived().parent.size(), " )");
#endif
return derived().root(n);
}
constexpr bool is_leader(itype::u32 n) const {
#ifndef NDEBUG
if(n >= derived().parent.size()) throw gsh::Exception("gsh::internal::UnionFindImpl::is_leader / The index is out of range. ( n=", n, ", size=", derived().parent.size(), " )");
#endif
return derived().parent[n] < 0;
}
constexpr bool same(itype::u32 a, itype::u32 b) {
#ifndef NDEBUG
if(a >= derived().parent.size() || b >= derived().parent.size()) throw gsh::Exception("gsh::UnionFind::same / The index is out of range. ( a=", a, ", b=", b, ", size=", derived().parent.size(), " )");
#endif
return derived().root(a) == derived().root(b);
}
protected:
GSH_INTERNAL_INLINE constexpr itype::u32 merge_impl(itype::u32 ar, itype::u32 br) noexcept {
const itype::i32 sa= derived().parent[ar], sb= derived().parent[br];
const itype::i32 tmp1= sa < sb ? ar : br, tmp2= sa < sb ? br : ar;
derived().parent[tmp1]+= derived().parent[tmp2];
derived().parent[tmp2]= tmp1;
--derived().cnt;
return tmp1;
}
public:
constexpr itype::u32 group_size(itype::u32 n) {
#ifndef NDEBUG
if(n >= derived().parent.size()) throw gsh::Exception("gsh::internal::UnionFindImpl::group_size(itype::u32) / The index is out of range. ( n=", n, ", size=", derived().parent.size(), " )");
#endif
return -derived().parent[derived().root(n)];
}
constexpr itype::u32 max_group_size() const noexcept {
itype::i32 res= 1;
for(itype::u32 i= 0; i != size(); ++i) {
res= -derived().parent[i] < res ? res : -derived().parent[i];
}
return res;
}
constexpr itype::u32 count_groups() const noexcept { return derived().cnt; }
constexpr Arr<itype::u32> extract(itype::u32 n) {
#ifndef NDEBUG
if(n >= derived().parent.size()) throw gsh::Exception("gsh::internal::UnionFindImpl::extract / The index is out of range. ( n=", n, ", size=", derived().parent.size(), " )");
#endif
const itype::i32 nr= derived().root(n);
itype::u32 ccnt= 0;
for(itype::u32 i= 0; i != derived().parent.size(); ++i) ccnt+= derived().root(i) == nr;
Arr<itype::u32> res(ccnt);
for(itype::u32 i= 0, j= 0; i != derived().parent.size(); ++i)
if(i == static_cast<itype::u32>(nr) || derived().parent[i] == nr) res[j++]= i;
return res;
}
constexpr Arr<Arr<itype::u32>> groups() {
Arr<itype::u32> key(derived().parent.size());
itype::u32 cnt= 0;
for(itype::u32 i= 0; i != derived().parent.size(); ++i) {
if(derived().parent[i] < 0) key[i]= cnt++;
}
Arr<itype::u32> cnt2(cnt);
for(itype::u32 i= 0; i != derived().parent.size(); ++i) ++cnt2[key[derived().root(i)]];
Arr<Arr<itype::u32>> res(cnt);
for(itype::u32 i= 0; i != cnt; ++i) {
res[i].resize(cnt2[i]);
cnt2[i]= 0;
}
for(itype::u32 i= 0; i != derived().parent.size(); ++i) {
const itype::u32 idx= key[derived().parent[i] < 0 ? i : derived().parent[i]];
res[idx][cnt2[idx]++]= i;
}
return res;
}
};
}// namespace internal
class UnionFind: public internal::UnionFindImpl<UnionFind> {
friend class internal::UnionFindImpl<UnionFind>;
Arr<itype::i32> parent;
itype::u32 cnt= 0;
constexpr itype::i32 root(itype::i32 n) noexcept {
if(parent[n] < 0) return n;
return parent[n]= root(parent[n]);
}
public:
using size_type= itype::u32;
constexpr UnionFind() noexcept {}
constexpr explicit UnionFind(itype::u32 n): parent(n, -1), cnt(n) {}
constexpr void reset() noexcept {
cnt= parent.size();
for(itype::u32 i= 0; i != cnt; ++i) parent[i]= -1;
}
constexpr void resize(itype::u32 n) {
if(n < size()) throw gsh::Exception("gsh::UnionFind::resize / It cannot be smaller than it is now.");
cnt+= n - size();
parent.resize(n, -1);
}
constexpr itype::u32 merge(const itype::u32 a, const itype::u32 b) {
#ifndef NDEBUG
if(a >= parent.size() || b >= parent.size()) throw gsh::Exception("gsh::UnionFind::merge / The index is out of range. ( a=", a, ", b=", b, ", size=", parent.size(), " )");
#endif
const itype::i32 ar= root(a), br= root(b);
if(ar == br) return ar;
return merge_impl(ar, br);
}
constexpr bool merge_same(const itype::u32 a, const itype::u32 b) {
#ifndef NDEBUG
if(a >= size() || b >= size()) throw gsh::Exception("gsh::UnionFind::merge_same / The index is out of range. ( a=", a, ", b=", b, ", size=", size(), " )");
#endif
const itype::i32 ar= root(a), br= root(b);
if(ar == br) return true;
merge_impl(ar, br);
return false;
}
};
class RollbackUnionFind: public internal::UnionFindImpl<RollbackUnionFind> {
friend class internal::UnionFindImpl<RollbackUnionFind>;
Arr<itype::i32> parent;
itype::u32 cnt= 0;
struct change {
itype::u32 a, b;
itype::i32 c, d;
bool merged;
constexpr change(itype::u32 A, itype::u32 B, itype::i32 C, itype::i32 D, bool M): a(A), b(B), c(C), d(D), merged(M) {}
};
Vec<change> history;
constexpr itype::i32 root(itype::i32 n) const noexcept {
while(parent[n] >= 0) n= parent[n];
return n;
}
public:
using size_type= itype::u32;
constexpr RollbackUnionFind() noexcept {}
constexpr explicit RollbackUnionFind(itype::u32 n): parent(n, -1), cnt(n) {}
constexpr void reset() noexcept {
cnt= parent.size();
history.clear();
for(itype::u32 i= 0; i != parent.size(); ++i) parent[i]= -1;
}
constexpr void reserve(itype::u32 q) { history.reserve(history.size() + q); }
constexpr void resize(itype::u32 n) {
if(n < parent.size()) throw gsh::Exception("gsh::RollbackUnionFind::resize / It cannot be smaller than it is now.");
cnt+= n - parent.size();
parent.resize(n, -1);
}
constexpr itype::u32 merge(const itype::u32 a, const itype::u32 b) {
#ifndef NDEBUG
if(a >= parent.size() || b >= parent.size()) throw gsh::Exception("gsh::RollbackUnionFind::merge / The index is out of range. ( a=", a, ", b=", b, ", size=", parent.size(), " )");
#endif
const itype::i32 ar= root(a), br= root(b);
history.emplace_back(ar, br, parent[ar], parent[br], ar != br);
if(ar == br) return ar;
return merge_impl(ar, br);
}
constexpr bool merge_same(const itype::u32 a, const itype::u32 b) {
#ifndef NDEBUG
if(a >= parent.size() || b >= parent.size()) throw gsh::Exception("gsh::RollbackUnionFind::merge_same / The index is out of range. ( a=", a, ", b=", b, ", size=", parent.size(), " )");
#endif
const itype::i32 ar= root(a), br= root(b);
history.emplace_back(ar, br, parent[ar], parent[br], ar != br);
if(ar == br) return true;
merge_impl(ar, br);
return false;
}
constexpr void undo() {
#ifndef NDEBUG
if(history.empty()) throw Exception("gsh::RollbackUnionFind::undo / The history is empty.");
#endif
auto [a, b, c, d, e]= history.back();
history.pop_back();
parent[a]= c, parent[b]= d;
cnt+= e;
}
class state {
friend class RollbackUnionFind;
itype::u32 s;
state()= delete;
constexpr state(itype::u32 n) noexcept: s(n) {}
public:
constexpr state(const state&) noexcept= default;
constexpr state& operator=(const state&) noexcept= default;
constexpr state next(itype::i32 n) const noexcept { return state{s + n}; }
constexpr state prev(itype::i32 n) const noexcept { return state{s - n}; }
constexpr void advance(itype::i32 n) noexcept { s+= n; }
};
constexpr state current() const noexcept { return state{history.size()}; }
constexpr void rollback(state st) {
while(st.s < history.size()) {
auto [a, b, c, d, e]= history.back();
history.pop_back();
parent[a]= c, parent[b]= d;
cnt+= e;
}
}
};
template<class T= itype::i64, class F= Plus, class I= Negate> class PotentializedUnionFind: public internal::UnionFindImpl<PotentializedUnionFind<T, F, I>> {
friend class internal::UnionFindImpl<PotentializedUnionFind<T, F, I>>;
Arr<itype::i32> parent;
Arr<T> diff;
itype::u32 cnt= 0;
[[no_unique_address]] F func{};
[[no_unique_address]] I inv{};
T el{};
constexpr itype::i32 root(itype::i32 n) noexcept {
if(parent[n] < 0) return n;
const itype::i32 r= root(parent[n]);
diff[n]= Invoke(func, diff[parent[n]], diff[n]);
return parent[n]= r;
}
public:
using value_type= T;
using size_type= itype::u32;
constexpr PotentializedUnionFind() noexcept(std::is_nothrow_default_constructible_v<F> && std::is_nothrow_default_constructible_v<I> && std::is_nothrow_default_constructible_v<T>) {}
constexpr explicit PotentializedUnionFind(F f, I i= I(), const T& e= T()): func(f), inv(i), el(e) {}
constexpr explicit PotentializedUnionFind(itype::u32 n, F f= F(), I i= I(), const T& e= T()): parent(n, -1), diff(n, e), cnt(n), func(f), inv(i), el(e) {}
constexpr void reset() {
cnt= parent.size();
for(itype::u32 i= 0; i != parent.size(); ++i) parent[i]= -1;
diff.assign(parent.size(), el);
}
constexpr void resize(itype::u32 n) {
if(n < parent.size()) throw gsh::Exception("gsh::PotentializedUnionFind::resize / It cannot be smaller than it is now.");
cnt+= n - parent.size();
parent.resize(n, -1);
diff.resize(n, el);
}
constexpr const T& operator[](itype::u32 n) { return potential(n); }
constexpr const T& potential(itype::u32 n) {
#ifndef NDEBUG
if(n >= parent.size()) throw gsh::Exception("gsh::PotentializedUnionFind::potential / The index is out of range. ( n=", n, ", size=", parent.size(), " )");
#endif
root(n);
return diff[n];
}
// A[a] = func(A[b], result)
constexpr T potential(itype::u32 a, itype::u32 b) {
#ifndef NDEBUG
if(a >= parent.size() || b >= parent.size()) throw gsh::Exception("gsh::PotentializedUnionFind::potential / The index is out of range. ( a=", a, ", b=", b, ", size=", parent.size(), " )");
#endif
root(a);
root(b);
return Invoke(func, Invoke(inv, diff[b]), diff[a]);
}
// A[a] = func(A[b], w) return leader(a)
constexpr itype::u32 merge(const itype::u32 a, const itype::u32 b, const T& w) {
#ifndef NDEBUG
if(a >= parent.size() || b >= parent.size()) throw gsh::Exception("gsh::PotentializedUnionFind::merge / The index is out of range. ( a=", a, ", b=", b, ", size=", parent.size(), " )");
#endif
const itype::i32 ar= root(a), br= root(b);
if(ar == br) return ar;
const itype::i32 sa= parent[ar], sb= parent[br];
const bool f= sa < sb;
const itype::i32 tmp1= f ? ar : br, tmp2= f ? br : ar;
parent[tmp1]+= parent[tmp2];
parent[tmp2]= tmp1;
diff[tmp2]= Invoke(func, diff[f ? a : b], Invoke(inv, Invoke(func, diff[f ? b : a], f ? w : Invoke(inv, w))));
--cnt;
return tmp1;
}
// A[a] = func(A[b], w) return same(a, b)
constexpr bool merge_same(const itype::u32 a, const itype::u32 b, const T& w) {
#ifndef NDEBUG
if(a >= parent.size() || b >= parent.size()) throw gsh::Exception("gsh::PotentializedUnionFind::merge_same / The index is out of range. ( a=", a, ", b=", b, ", size=", parent.size(), " )");
#endif
const itype::i32 ar= root(a), br= root(b);
if(ar == br) return true;
const itype::i32 sa= parent[ar], sb= parent[br];
const bool f= sa < sb;
const itype::i32 tmp1= f ? ar : br, tmp2= f ? br : ar;
parent[tmp1]+= parent[tmp2];
parent[tmp2]= tmp1;
diff[tmp2]= Invoke(func, diff[f ? a : b], Invoke(inv, Invoke(func, diff[f ? b : a], f ? w : Invoke(inv, w))));
--cnt;
return false;
}
// A[a] = func(A[b], w)
constexpr bool merge_valid(const itype::u32 a, const itype::u32 b, const T& w) {
#ifndef NDEBUG
if(a >= parent.size() || b >= parent.size()) throw gsh::Exception("gsh::PotentializedUnionFind::merge_valid / The index is out of range. ( a=", a, ", b=", b, ", size=", parent.size(), " )");
#endif
const itype::i32 ar= root(a), br= root(b);
if(ar == br) return diff[a] == Invoke(func, diff[b], w);
const itype::i32 sa= parent[ar], sb= parent[br];
const bool f= sa < sb;
const itype::i32 tmp1= f ? ar : br, tmp2= f ? br : ar;
parent[tmp1]+= parent[tmp2];
parent[tmp2]= tmp1;
diff[tmp2]= Invoke(func, diff[f ? a : b], Invoke(inv, Invoke(func, diff[f ? b : a], f ? w : Invoke(inv, w))));
--cnt;
return true;
}
};
template<class T, class F> class MonoidalUnionFind: public internal::UnionFindImpl<MonoidalUnionFind<T, F>> {
friend class internal::UnionFindImpl<MonoidalUnionFind<T, F>>;
Arr<itype::i32> parent;
Arr<T> monoid;
[[no_unique_address]] F func;
itype::u32 cnt= 0;
constexpr itype::i32 root(itype::i32 n) noexcept {
if(parent[n] < 0) return n;
return parent[n]= root(parent[n]);
}
public:
using value_type= T;
using size_type= itype::u32;
constexpr MonoidalUnionFind() noexcept {}
constexpr explicit MonoidalUnionFind(F f): func(std::move(f)) {}
constexpr explicit MonoidalUnionFind(itype::u32 n, F f= F()): parent(n, -1), monoid(n), func(std::move(f)), cnt(n) {}
constexpr MonoidalUnionFind(itype::u32 n, F f, const T& init): parent(n, -1), monoid(n, init), func(std::move(f)), cnt(n) {}
constexpr ~MonoidalUnionFind() {
for(itype::u32 i= 0; i != parent.size(); ++i) {
if(parent[i] < 0) std::destroy_at(&monoid[i]);
}
monoid.reset();
}
constexpr void reset() {
cnt= parent.size();
for(itype::u32 i= 0; i != parent.size(); ++i) {
if(parent[i] < 0) std::destroy_at(&monoid[i]);
parent[i]= -1;
std::construct_at(&monoid[i]);
}
}
constexpr void reset(const T& init) {
cnt= parent.size();
for(itype::u32 i= 0; i != parent.size(); ++i) {
if(parent[i] < 0) std::destroy_at(&monoid[i]);
parent[i]= -1;
std::construct_at(&monoid[i], init);
}
}
constexpr void resize(itype::u32 n) {
if(n < parent.size()) throw gsh::Exception("gsh::MonoidalUnionFind::resize / It cannot be smaller than it is now.");
cnt+= n - parent.size();
parent.resize(n, -1);
monoid.resize(n);
}
constexpr void resize(itype::u32 n, const T& value) {
if(n < parent.size()) throw gsh::Exception("gsh::MonoidalUnionFind::resize / It cannot be smaller than it is now.");
cnt+= n - parent.size();
parent.resize(n, -1);
monoid.resize(n, value);
}
constexpr T& operator[](itype::u32 n) {
#ifndef NDEBUG
if(n >= parent.size()) throw gsh::Exception("gsh::MonoidalUnionFind::operator[] / The index is out of range. ( n=", n, ", size=", parent.size(), " )");
#endif
return monoid[root(n)];
}
constexpr itype::u32 merge(const itype::u32 a, const itype::u32 b) {
#ifndef NDEBUG
if(a >= parent.size() || b >= parent.size()) throw gsh::Exception("gsh::MonoidalUnionFind::merge / The index is out of range. ( a=", a, ", b=", b, ", size=", parent.size(), " )");
#endif
const itype::i32 ar= root(a), br= root(b);
if(ar == br) return ar;
const itype::i32 sa= parent[ar], sb= parent[br];
const bool f= sa < sb;
const itype::i32 tmp1= f ? ar : br, tmp2= f ? br : ar;
parent[tmp1]+= parent[tmp2];
parent[tmp2]= tmp1;
monoid[tmp1]= Invoke(func, std::move(monoid[tmp1]), std::move(monoid[tmp2]));
std::destroy_at(&monoid[tmp2]);
--cnt;
return tmp1;
}
constexpr bool merge_same(const itype::u32 a, const itype::u32 b) {
#ifndef NDEBUG
if(a >= parent.size() || b >= parent.size()) throw gsh::Exception("gsh::MonoidalUnionFind::merge / The index is out of range. ( a=", a, ", b=", b, ", size=", parent.size(), " )");
#endif
const itype::i32 ar= root(a), br= root(b);
if(ar == br) return true;
const itype::i32 sa= parent[ar], sb= parent[br];
const bool f= sa < sb;
const itype::i32 tmp1= f ? ar : br, tmp2= f ? br : ar;
parent[tmp1]+= parent[tmp2];
parent[tmp2]= tmp1;
monoid[tmp1]= Invoke(func, std::move(monoid[tmp1]), std::move(monoid[tmp2]));
std::destroy_at(&monoid[tmp2]);
--cnt;
return false;
}
};
/*
class OfflinePersistentUnionfind {
    itype::u32 n = 0;
    struct query {
        bool t;
        itype::u32 k, u, v;
    };
    Vec<query> g;
public:
    constexpr OfflinePersistentUnionfind() {}
    constexpr OfflinePersistentUnionfind(itype::u32 m) : n(m) {}
    constexpr itype::u32 size() const noexcept { return n; }
    constexpr void same(itype::i32 k, itype::u32 u, itype::u32 v) { g.emplace_back(true, k + 1, u, v); }
    constexpr void merge(itype::i32 k, itype::u32 u, itype::u32 v) { g.emplace_back(false, k + 1, u, v); }
    constexpr void reserve(itype::u32 q) { g.reserve(q); }
    constexpr Arr<bool> solve() const {
        struct solver {
            Arr<itype::u32> cnt;
            Arr<query> gr;
            RollbackUnionFind ds;
            Arr<itype::u8> res;
            Arr<bool> res2;
            constexpr void dfs(itype::u32 idx) {
                auto [t, k, u, v] = gr[idx];
                if (t) {
                    res[k - 1] = ds.same(u, v) + 1;
                } else {
                    ds.merge(u, v);
                    for (itype::u32 i = cnt[k], j = cnt[k + 1]; i != j; ++i) dfs(i);
                    ds.undo();
                }
            }
            constexpr solver(itype::u32 n, const Vec<query>& g) : cnt(g.size() + 2), gr(g.size() + 1), ds(n + 1), res(g.size()) {
                const itype::u32 q = g.size();
                itype::u32 same_cnt = 0;
                for (itype::u32 i = 0; i != q; ++i) {
                    ++cnt[g[i].k];
                    same_cnt += g[i].t;
                }
                for (itype::u32 i = 0; i != q; ++i) cnt[i + 1] += cnt[i];
                cnt[q + 1] = cnt[q];
                for (itype::u32 i = q; i != 0; --i) {
                    itype::u32 idx = --cnt[g[i - 1].k];
                    (gr[idx] = g[i - 1]).k = i;
                }
                gr[q] = { false, 0, n, n };
                ds.reserve(q - same_cnt + 1);
                dfs(q);
                res2.resize(same_cnt);
                itype::u32 s = 0;
                for (itype::u32 i = 0; i != q; ++i) {
                    bool f = res[i] != 0;
                    res2[s] = (f ? res[i] - 1 : res2[s]);
                    s += f;
                }
            }
        } sl(n, g);
        return std::move(sl.res2);
    }
};
*/
}// namespace gsh
namespace gsh {
namespace itype {
struct i4dig;
struct u4dig;
struct i8dig;
struct u8dig;
struct i16dig;
struct u16dig;
}// namespace itype
template<class T> class Parser;
namespace internal {
template<class Stream> constexpr itype::u8 Parseu8(Stream& stream) {
itype::u32 v;
MemoryCopy(&v, stream.current(), 4);
v^= 0x30303030;
itype::i32 tmp= std::countr_zero(v & 0xf0f0f0f0) >> 3;
v<<= (32 - (tmp << 3));
v= (v * 10 + (v >> 8)) & 0x00ff00ff;
v= (v * 100 + (v >> 16)) & 0x0000ffff;
stream.skip(tmp + 1);
return v;
}
template<class Stream> constexpr itype::u16 Parseu16(Stream& stream) {
itype::u64 v;
MemoryCopy(&v, stream.current(), 8);
v^= 0x3030303030303030;
itype::i32 tmp= std::countr_zero(v & 0xf0f0f0f0f0f0f0f0) >> 3;
v<<= (64 - (tmp << 3));
v= (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
v= (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
v= (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
stream.skip(tmp + 1);
return v;
}
template<class Stream> constexpr itype::u32 Parseu32(Stream& stream) {
itype::u32 res= 0;
{
itype::u64 v;
MemoryCopy(&v, stream.current(), 8);
if(!((v^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0)) {
v= (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
v= (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
v= (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
res= v;
stream.skip(8);
}
}
itype::u64 buf;
MemoryCopy(&buf, stream.current(), 8);
{
itype::u32 v= buf;
if(!((v^= 0x30303030) & 0xf0f0f0f0)) {
buf>>= 32;
v= (v * 10 + (v >> 8)) & 0x00ff00ff;
v= (v * 100 + (v >> 16)) & 0x0000ffff;
res= 10000 * res + v;
stream.skip(4);
}
}
{
itype::u16 v= buf;
if(!((v^= 0x3030) & 0xf0f0)) {
buf>>= 16;
v= (v * 10 + (v >> 8)) & 0x00ff;
res= 100 * res + v;
stream.skip(2);
}
}
{
const ctype::c8 v= ctype::c8(buf) ^ 0x30;
const bool f= !(v & 0xf0);
res= f ? 10 * res + v : res;
stream.skip(f + 1);
}
return res;
};
template<class Stream> constexpr itype::u64 Parseu64(Stream& stream) {
itype::u64 res= 0;
{
itype::u64 v;
MemoryCopy(&v, stream.current(), 8);
if(!((v^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0)) {
stream.skip(8);
itype::u64 u;
MemoryCopy(&u, stream.current(), 8);
if(!((u^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0)) {
v= (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
u= (u * 10 + (u >> 8)) & 0x00ff00ff00ff00ff;
v= (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
u= (u * 100 + (u >> 16)) & 0x0000ffff0000ffff;
v= (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
u= (u * 10000 + (u >> 32)) & 0x00000000ffffffff;
res= v * 100000000 + u;
stream.skip(8);
} else {
v= (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
v= (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
v= (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
res= v;
}
}
}
itype::u64 buf;
MemoryCopy(&buf, stream.current(), 8);
{
itype::u32 v= buf;
if(!((v^= 0x30303030) & 0xf0f0f0f0)) {
buf>>= 32;
v= (v * 10 + (v >> 8)) & 0x00ff00ff;
v= (v * 100 + (v >> 16)) & 0x0000ffff;
res= 10000 * res + v;
stream.skip(4);
}
}
{
itype::u16 v= buf;
if(!((v^= 0x3030) & 0xf0f0)) {
buf>>= 16;
v= (v * 10 + (v >> 8)) & 0x00ff;
res= 100 * res + v;
stream.skip(2);
}
}
{
const ctype::c8 v= ctype::c8(buf) ^ 0x30;
const bool f= !(v & 0xf0);
res= f ? 10 * res + v : res;
stream.skip(f + 1);
}
return res;
}
template<class Stream> constexpr itype::u128 Parseu128(Stream& stream) {
itype::u128 res= 0;
GSH_INTERNAL_UNROLL(4)
for(itype::u32 i= 0; i != 4; ++i) {
itype::u64 v;
MemoryCopy(&v, stream.current(), 8);
if(((v^= 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0) != 0) break;
v= (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
v= (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
v= (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
if(i == 0) res= v;
else res= res * 100000000 + v;
stream.skip(8);
}
itype::u64 buf;
MemoryCopy(&buf, stream.current(), 8);
itype::u64 res2= 0, pw= 1;
{
itype::u32 v= buf;
if(!((v^= 0x30303030) & 0xf0f0f0f0)) {
buf>>= 32;
v= (v * 10 + (v >> 8)) & 0x00ff00ff;
v= (v * 100 + (v >> 16)) & 0x0000ffff;
res2= v;
pw= 10000;
stream.skip(4);
}
}
{
itype::u16 v= buf;
if(!((v^= 0x3030) & 0xf0f0)) {
buf>>= 16;
v= (v * 10 + (v >> 8)) & 0x00ff;
res2= res2 * 100 + v;
pw*= 100;
stream.skip(2);
}
}
{
const ctype::c8 v= ctype::c8(buf) ^ 0x30;
const bool f= (v & 0xf0) == 0;
const volatile auto tmp1= pw * 10, tmp2= res2 * 10 + v;
const auto tmp3= tmp1, tmp4= tmp2;
pw= f ? tmp3 : pw;
res2= f ? tmp4 : res2;
stream.skip(f + 1);
}
return res * pw + res2;
}
template<class Stream> constexpr itype::u16 Parseu4dig(Stream& stream) {
itype::u32 v;
MemoryCopy(&v, stream.current(), 4);
v^= 0x30303030;
itype::i32 tmp= std::countr_zero(v & 0xf0f0f0f0) >> 3;
v<<= (32 - (tmp << 3));
v= (v * 10 + (v >> 8)) & 0x00ff00ff;
v= (v * 100 + (v >> 16)) & 0x0000ffff;
stream.skip(tmp + 1);
return v;
}
template<class Stream> constexpr itype::u32 Parseu8dig(Stream& stream) {
itype::u64 v;
MemoryCopy(&v, stream.current(), 8);
v^= 0x3030303030303030;
const itype::u64 msk= v & 0xf0f0f0f0f0f0f0f0;
itype::i32 tmp= std::countr_zero(msk) >> 3;
v<<= (64 - (tmp << 3));
v= (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ff;
v= (v * 100 + (v >> 16)) & 0x0000ffff0000ffff;
v= (v * 10000 + (v >> 32)) & 0x00000000ffffffff;
stream.skip(tmp + 1);
return v;
}
}// namespace internal
template<> class Parser<itype::u8> {
public:
template<class Stream> constexpr itype::u8 operator()(Stream& stream) const {
stream.reload(8);
return internal::Parseu8(stream);
}
};
template<> class Parser<itype::i8> {
public:
template<class Stream> constexpr itype::i8 operator()(Stream& stream) const {
stream.reload(8);
bool neg= *stream.current() == '-';
stream.skip(neg);
itype::i8 tmp= internal::Parseu8(stream);
if(neg) tmp= -tmp;
return tmp;
}
};
template<> class Parser<itype::u16> {
public:
template<class Stream> constexpr itype::u16 operator()(Stream& stream) const {
stream.reload(8);
return internal::Parseu16(stream);
}
};
template<> class Parser<itype::i16> {
public:
template<class Stream> constexpr itype::i16 operator()(Stream& stream) const {
stream.reload(8);
bool neg= *stream.current() == '-';
stream.skip(neg);
itype::i16 tmp= internal::Parseu16(stream);
if(neg) tmp= -tmp;
return tmp;
}
};
template<> class Parser<itype::u32> {
public:
template<class Stream> constexpr itype::u32 operator()(Stream& stream) const {
stream.reload(16);
return internal::Parseu32(stream);
}
};
template<> class Parser<itype::i32> {
public:
template<class Stream> constexpr itype::i32 operator()(Stream& stream) const {
stream.reload(16);
bool neg= *stream.current() == '-';
stream.skip(neg);
itype::i32 tmp= internal::Parseu32(stream);
if(neg) tmp= -tmp;
return tmp;
}
};
template<> class Parser<itype::u64> {
public:
template<class Stream> constexpr itype::u64 operator()(Stream& stream) const {
stream.reload(32);
return internal::Parseu64(stream);
}
};
template<> class Parser<itype::i64> {
public:
template<class Stream> constexpr itype::i64 operator()(Stream& stream) const {
stream.reload(32);
bool neg= *stream.current() == '-';
stream.skip(neg);
itype::i64 tmp= internal::Parseu64(stream);
if(neg) tmp= -tmp;
return tmp;
}
};
template<> class Parser<itype::u128> {
public:
template<class Stream> constexpr itype::u128 operator()(Stream& stream) const {
stream.reload(64);
return internal::Parseu128(stream);
}
};
template<> class Parser<itype::i128> {
public:
template<class Stream> constexpr itype::i128 operator()(Stream& stream) const {
stream.reload(64);
bool neg= *stream.current() == '-';
stream.skip(neg);
itype::i128 tmp= internal::Parseu128(stream);
if(neg) tmp= -tmp;
return tmp;
}
};
template<> class Parser<itype::u4dig> {
public:
using value_type= itype::u16;
template<class Stream> constexpr itype::u16 operator()(Stream& stream) const {
stream.reload(8);
return internal::Parseu4dig(stream);
}
};
template<> class Parser<itype::i4dig> {
public:
using value_type= itype::i16;
template<class Stream> constexpr itype::i16 operator()(Stream& stream) const {
stream.reload(8);
bool neg= *stream.current() == '-';
stream.skip(neg);
itype::i16 tmp= internal::Parseu4dig(stream);
if(neg) tmp= -tmp;
return tmp;
}
};
template<> class Parser<itype::u8dig> {
public:
using value_type= itype::u32;
template<class Stream> constexpr itype::u32 operator()(Stream& stream) const {
stream.reload(16);
return internal::Parseu8dig(stream);
}
};
template<> class Parser<itype::i8dig> {
public:
using value_type= itype::i32;
template<class Stream> constexpr itype::i32 operator()(Stream& stream) const {
stream.reload(16);
bool neg= *stream.current() == '-';
stream.skip(neg);
itype::i32 tmp= internal::Parseu8dig(stream);
if(neg) tmp= -tmp;
return tmp;
}
};
template<> class Parser<ctype::c8> {
public:
template<class Stream> constexpr ctype::c8 operator()(Stream& stream) const {
stream.reload(2);
ctype::c8 tmp= *stream.current();
stream.skip(2);
return tmp;
}
};
template<> class Parser<ctype::c8*> {
public:
template<class Stream> constexpr ctype::c8* operator()(Stream& stream, ctype::c8* s) const {
stream.reload(16);
ctype::c8* c= s;
while(true) {
const ctype::c8* e= stream.current();
while(*e >= '!') ++e;
const itype::u32 len= e - stream.current();
MemoryCopy(c, stream.current(), len);
stream.skip(len);
c+= len;
if(stream.avail() == 0) stream.reload();
else break;
}
stream.skip(1);
*c= '\0';
return s;
}
template<class Stream> constexpr ctype::c8* operator()(Stream& stream, ctype::c8* s, itype::u32 n) const {
itype::u32 rem= n;
ctype::c8* c= s;
itype::u32 avail= stream.avail();
while(avail <= rem) {
MemoryCopy(c, stream.current(), avail);
c+= avail;
rem-= avail;
stream.skip(avail);
if(rem == 0) {
*c= '\0';
return s;
}
stream.reload();
avail= stream.avail();
}
MemoryCopy(c, stream.current(), rem);
c+= rem;
stream.skip(rem + 1);
*c= '\0';
return s;
}
};
namespace internal {
template<class T, class P, class Stream, class... Args> struct ParsingIterator {
using value_type= T;
using difference_type= itype::i32;
using pointer= T*;
using reference= T&;
using iterator_category= std::input_iterator_tag;
itype::u32 n;
P* ref;
Stream* stream;
std::tuple<Args...>* args;
constexpr ParsingIterator(itype::u32 m, P* r, Stream* s, std::tuple<Args...>* a) noexcept: n(m), ref(r), stream(s), args(a) {}
GSH_INTERNAL_INLINE friend constexpr bool operator==(const ParsingIterator& a, const ParsingIterator& b) noexcept { return a.n == b.n; }
GSH_INTERNAL_INLINE constexpr ParsingIterator& operator++() noexcept { return ++n, *this; }
GSH_INTERNAL_INLINE constexpr ParsingIterator operator++(int) noexcept { return {n++, *ref, *stream, *args}; }
GSH_INTERNAL_INLINE constexpr T operator*() const {
return [this]<itype::u32... I>(std::integer_sequence<itype::u32, I...>) -> T {
return (*ref)(*stream, std::get<I>(*args)...);
}(std::make_integer_sequence<itype::u32, sizeof...(Args)>());
}
};
}// namespace internal
template<class R> concept ParsableRange= std::ranges::forward_range<R> && requires { sizeof(Parser<std::decay_t<std::ranges::range_value_t<R>>>) != 0; };
template<ParsableRange R> class Parser<R> {
public:
template<class Stream, class... Args> constexpr R operator()(Stream&& stream, itype::u32 len, Args&&... args) const {
Parser<std::ranges::range_value_t<R>> p;
std::tuple<Args...> a(std::forward<Args>(args)...);
using iter= internal::ParsingIterator<std::ranges::range_value_t<R>, decltype(p), std::remove_cvref_t<Stream>, Args...>;
return R(iter(0, &p, &stream, &a), iter(len, nullptr, nullptr, nullptr));
}
};
/*
namespace internal {
    template<class T, class U> constexpr bool ParsableTupleImpl = false;
    template<class T, std::size_t... I> constexpr bool ParsableTupleImpl<T, std::integer_sequence<std::size_t, I...>> = (... && requires { sizeof(Parser<std::decay_t<typename std::tuple_element<I, T>::type>>) != 0; });
}  // namespace internal
template<class T> concept ParsableTuple = requires { std::tuple_size<T>::value; } && internal::ParsableTupleImpl<T, std::make_index_sequence<std::tuple_size<T>::value>>;
template<ParsableTuple T>
    requires(!ParsableRange<T>)
class Parser<T> {
public:
    template<class Stream> constexpr T operator()(Stream&& stream) const {}
};
*/
}// namespace gsh
namespace gsh {
namespace itype {
struct i4dig;
struct u4dig;
struct i8dig;
struct u8dig;
struct i16dig;
struct u16dig;
}// namespace itype
template<class T> class Formatter;
namespace internal {
#ifndef GSH_USE_COMPILE_TIME_CALCULATION
struct InttoStrT {
#define GSH_INTERNAL_INCLUDE_INTTOSTR "internal/InttoStr.txt"
const ctype::c8* table=
#include GSH_INTERNAL_INCLUDE_INTTOSTR
};
template<itype::u32> constexpr InttoStrT InttoStr;
#else
template<itype::u32> constexpr auto InttoStr= [] {
struct {
ctype::c8 table[40004]= {};
} res;
for(itype::u32 i= 0; i != 10000; ++i) {
res.table[4 * i + 0]= (i / 1000 + '0');
res.table[4 * i + 1]= (i / 100 % 10 + '0');
res.table[4 * i + 2]= (i / 10 % 10 + '0');
res.table[4 * i + 3]= (i % 10 + '0');
}
return res;
}();
#endif
template<class Stream> constexpr void Formatu16(Stream& stream, itype::u16 n) {
auto copy1= [&](itype::u16 x) {
itype::u32 off= (x < 10) + (x < 100) + (x < 1000);
MemoryCopy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
stream.skip(4 - off);
};
auto copy2= [&](itype::u16 x) {
MemoryCopy(stream.current(), InttoStr<0>.table + 4 * x, 4);
stream.skip(4);
};
if(n < 10000) copy1(n);
else {
copy1(n / 10000);
copy2(n % 10000);
}
}
template<class Stream> constexpr void Formatu32(Stream& stream, itype::u32 n) {
auto copy1= [&](itype::u32 x) {
itype::u32 off= (x < 10) + (x < 100) + (x < 1000);
MemoryCopy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
stream.skip(4 - off);
};
auto copy2= [&](itype::u32 x) {
MemoryCopy(stream.current(), InttoStr<0>.table + 4 * x, 4);
stream.skip(4);
};
if(n < 100000000) {
if(n < 10000) copy1(n);
else {
copy1(n / 10000);
copy2(n % 10000);
}
} else {
copy1(n / 100000000);
copy2(n / 10000 % 10000);
copy2(n % 10000);
}
}
template<class Stream> constexpr void Formatu64(Stream& stream, itype::u64 n) {
auto copy1= [&](itype::u32 x) {
itype::u32 off= (x < 10) + (x < 100) + (x < 1000);
MemoryCopy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
stream.skip(4 - off);
};
auto copy2= [&](itype::u32 x) {
MemoryCopy(stream.current(), InttoStr<0>.table + 4 * x, 4);
stream.skip(4);
};
if(n < 10000000000000000) {
if(n < 1000000000000) {
if(n < 100000000) {
if(n < 10000) copy1(n);
else {
copy1(n / 10000);
copy2(n % 10000);
}
} else {
copy1(n / 100000000);
copy2(n / 10000 % 10000);
copy2(n % 10000);
}
} else {
copy1(n / 1000000000000);
copy2(n / 100000000 % 10000);
copy2(n / 10000 % 10000);
copy2(n % 10000);
}
} else {
copy1(n / 10000000000000000);
copy2(n / 1000000000000 % 10000);
copy2(n / 100000000 % 10000);
copy2(n / 10000 % 10000);
copy2(n % 10000);
}
}
template<class Stream> constexpr void Formatu128(Stream& stream, itype::u128 n) {
auto copy1= [&](itype::u32 x) {
itype::u32 off= (x < 10) + (x < 100) + (x < 1000);
MemoryCopy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
stream.skip(4 - off);
};
auto copy2= [&](itype::u32 x) {
MemoryCopy(stream.current(), InttoStr<0>.table + 4 * x, 4);
stream.skip(4);
};
constexpr itype::u128 t= static_cast<itype::u128>(10000000000000000) * 10000000000000000;
if(n >= t) {
const itype::u32 dv= n / t;
n-= dv * t;
if(dv >= 10000) {
copy1(dv / 10000);
copy2(dv % 10000);
} else copy1(dv);
auto [a, b]= Divu128(n >> 64, n, 10000000000000000);
const itype::u32 c= a / 100000000, d= a % 100000000, e= b / 100000000, f= b % 100000000;
copy2(c / 10000), copy2(c % 10000);
copy2(d / 10000), copy2(d % 10000);
copy2(e / 10000), copy2(e % 10000);
copy2(f / 10000), copy2(f % 10000);
} else {
auto [a, b]= Divu128(n >> 64, n, 10000000000000000);
const itype::u32 c= a / 100000000, d= a % 100000000, e= b / 100000000, f= b % 100000000;
const itype::u32 g= c / 10000, h= c % 10000, i= d / 10000, j= d % 10000, k= e / 10000, l= e % 10000, m= f / 10000, n= f % 10000;
if(a == 0) {
if(e == 0) {
if(m == 0) copy1(n);
else copy1(m), copy2(n);
} else {
if(k == 0) copy1(l), copy2(m), copy2(n);
else copy1(k), copy2(l), copy2(m), copy2(n);
}
} else {
if(c == 0) {
if(i == 0) copy1(j), copy2(k), copy2(l), copy2(m), copy2(n);
else copy1(i), copy2(j), copy2(k), copy2(l), copy2(m), copy2(n);
} else {
if(g == 0) copy1(h), copy2(i), copy2(j), copy2(k), copy2(l), copy2(m), copy2(n);
else copy1(g), copy2(h), copy2(i), copy2(j), copy2(k), copy2(l), copy2(m), copy2(n);
}
}
}
}
template<class Stream> constexpr void Formatu4dig(Stream& stream, itype::u16 x) {
itype::u32 off= (x < 10) + (x < 100) + (x < 1000);
MemoryCopy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
stream.skip(4 - off);
}
template<class Stream> constexpr void Formatu8dig(Stream& stream, itype::u32 x) {
const itype::u32 n= x;
auto copy1= [&](itype::u32 x) {
itype::u32 off= (x < 10) + (x < 100) + (x < 1000);
MemoryCopy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
stream.skip(4 - off);
};
auto copy2= [&](itype::u32 x) {
MemoryCopy(stream.current(), InttoStr<0>.table + 4 * x, 4);
stream.skip(4);
};
if(n < 10000) copy1(n);
else {
copy1(n / 10000);
copy2(n % 10000);
}
}
template<class Stream> constexpr void Formatu16dig(Stream& stream, itype::u64 x) {
const itype::u64 n= x;
auto copy1= [&](itype::u64 x) {
itype::u32 off= (x < 10) + (x < 100) + (x < 1000);
MemoryCopy(stream.current(), InttoStr<0>.table + (4 * x + off), 4);
stream.skip(4 - off);
};
auto copy2= [&](itype::u64 x) {
MemoryCopy(stream.current(), InttoStr<0>.table + 4 * x, 4);
stream.skip(4);
};
if(n < 1000000000000) {
if(n < 100000000) {
if(n < 10000) copy1(n);
else {
copy1(n / 10000);
copy2(n % 10000);
}
} else {
copy1(n / 100000000);
copy2(n / 10000 % 10000);
copy2(n % 10000);
}
} else {
copy1(n / 1000000000000);
copy2(n / 100000000 % 10000);
copy2(n / 10000 % 10000);
copy2(n % 10000);
}
}
}// namespace internal
template<> class Formatter<itype::u16> {
public:
template<class Stream> constexpr void operator()(Stream& stream, itype::u16 n) const {
stream.reload(8);
internal::Formatu16(stream, n);
}
};
template<> class Formatter<itype::i16> {
public:
template<class Stream> constexpr void operator()(Stream& stream, itype::i16 n) const {
stream.reload(8);
*stream.current()= '-';
stream.skip(n < 0);
internal::Formatu16(stream, n < 0 ? -n : n);
}
};
template<> class Formatter<itype::u32> {
public:
template<class Stream> constexpr void operator()(Stream& stream, itype::u32 n) const {
stream.reload(16);
internal::Formatu32(stream, n);
}
};
template<> class Formatter<itype::i32> {
public:
template<class Stream> constexpr void operator()(Stream& stream, itype::i32 n) const {
stream.reload(16);
*stream.current()= '-';
stream.skip(n < 0);
internal::Formatu32(stream, n < 0 ? -n : n);
}
};
template<> class Formatter<itype::u64> {
public:
template<class Stream> constexpr void operator()(Stream& stream, itype::u64 n) const {
stream.reload(32);
internal::Formatu64(stream, n);
}
};
template<> class Formatter<itype::i64> {
public:
template<class Stream> constexpr void operator()(Stream& stream, itype::i64 n) const {
stream.reload(32);
*stream.current()= '-';
stream.skip(n < 0);
internal::Formatu64(stream, n < 0 ? -n : n);
}
};
template<> class Formatter<itype::u128> {
public:
template<class Stream> constexpr void operator()(Stream& stream, itype::u128 n) const {
stream.reload(64);
internal::Formatu128(stream, n);
}
};
template<> class Formatter<itype::i128> {
public:
template<class Stream> constexpr void operator()(Stream& stream, itype::i128 n) const {
stream.reload(64);
*stream.current()= '-';
stream.skip(n < 0);
internal::Formatu128(stream, n < 0 ? -n : n);
}
};
template<> class Formatter<itype::u4dig> {
public:
template<class Stream> constexpr void operator()(Stream& stream, itype::u16 n) const {
stream.reload(4);
internal::Formatu4dig(stream, n);
}
};
template<> class Formatter<itype::i4dig> {
public:
template<class Stream> constexpr void operator()(Stream& stream, itype::i16 n) const {
stream.reload(5);
*stream.current()= '-';
stream.skip(n < 0);
internal::Formatu4dig(stream, static_cast<itype::u16>(n < 0 ? -n : n));
}
};
template<> class Formatter<itype::u8dig> {
public:
template<class Stream> constexpr void operator()(Stream& stream, itype::u32 n) const {
stream.reload(8);
internal::Formatu8dig(stream, n);
}
};
template<> class Formatter<itype::i8dig> {
public:
template<class Stream> constexpr void operator()(Stream& stream, itype::i32 n) const {
stream.reload(9);
*stream.current()= '-';
stream.skip(n < 0);
internal::Formatu8dig(stream, static_cast<itype::u32>(n < 0 ? -n : n));
}
};
template<> class Formatter<itype::u16dig> {
public:
template<class Stream> constexpr void operator()(Stream& stream, itype::u64 n) const {
stream.reload(16);
internal::Formatu16dig(stream, n);
}
};
template<> class Formatter<itype::i16dig> {
public:
template<class Stream> constexpr void operator()(Stream& stream, itype::i64 n) const {
stream.reload(17);
*stream.current()= '-';
stream.skip(n < 0);
internal::Formatu16dig(stream, static_cast<itype::u64>(n < 0 ? -n : n));
}
};
template<> class Formatter<ctype::c8> {
public:
template<class Stream> constexpr void operator()(Stream& stream, ctype::c8 c) const {
stream.reload(1);
*stream.current()= c;
stream.skip(1);
}
};
namespace internal {
template<class T> class FloatFormatter {
public:
template<class Stream> constexpr void operator()(Stream& stream, T f, std::chars_format fmt= std::chars_format::general, itype::i32 precision= 6) {
stream.reload(32);
auto [ptr, err]= std::to_chars(stream.current(), stream.current() + stream.avail(), f, fmt, precision);
if(err != std::errc{}) [[unlikely]] {
stream.reload();
auto [ptr, err]= std::to_chars(stream.current(), stream.current() + stream.avail(), f, fmt, precision);
if(err != std::errc{}) throw Exception("gsh::Formatter<ftype::f32>::operator() / The value is too large.");
stream.skip(ptr - stream.current());
} else {
stream.skip(ptr - stream.current());
}
}
};
}// namespace internal
template<> class Formatter<float>: public internal::FloatFormatter<float> {};
template<> class Formatter<double>: public internal::FloatFormatter<double> {};
template<> class Formatter<long double>: public internal::FloatFormatter<long double> {};
#ifdef __STDCPP_FLOAT16_T__
template<> class Formatter<std::float16_t>: public internal::FloatFormatter<std::float16_t> {};
#endif
#ifdef __STDCPP_FLOAT32_T__
template<> class Formatter<std::float32_t>: public internal::FloatFormatter<std::float32_t> {};
#endif
#ifdef __STDCPP_FLOAT64_T__
template<> class Formatter<std::float64_t>: public internal::FloatFormatter<std::float64_t> {};
#endif
#ifdef __STDCPP_FLOAT128_T__
template<> class Formatter<std::float128_t>: public internal::FloatFormatter<std::float128_t> {};
#endif
#ifdef __STDCPP_BFLOAT16_T__
template<> class Formatter<std::bfloat16_t>: public internal::FloatFormatter<std::bfloat16_t> {};
#endif
#ifdef __SIZEOF_FLOAT128__
template<> class Formatter<__float128>: public internal::FloatFormatter<__float128> {};
#endif
template<> class Formatter<ftype::InvalidFloat16Tag> {};
template<> class Formatter<ftype::InvalidFloat128Tag> {};
template<> class Formatter<ftype::InvalidBfloat16Tag> {};
template<> class Formatter<bool> {
public:
template<class Stream> constexpr void operator()(Stream& stream, bool b) const {
stream.reload(1);
*stream.current()= '0' + b;
stream.skip(1);
}
};
template<> class Formatter<const ctype::c8*> {
public:
template<class Stream> constexpr void operator()(Stream&& stream, const ctype::c8* s) const { operator()(stream, s, StrLen(s)); }
template<class Stream> constexpr void operator()(Stream&& stream, const ctype::c8* s, itype::u32 len) const {
itype::u32 avail= stream.avail();
if(avail >= len) [[likely]] {
MemoryCopy(stream.current(), s, len);
stream.skip(len);
} else {
MemoryCopy(stream.current(), s, avail);
len-= avail;
s+= avail;
stream.skip(avail);
while(len != 0) {
stream.reload();
avail= stream.avail();
const itype::u32 tmp= len < avail ? len : avail;
MemoryCopy(stream.current(), s, tmp);
len-= tmp;
s+= tmp;
stream.skip(tmp);
}
}
}
};
template<> class Formatter<ctype::c8*>: public Formatter<const ctype::c8*> {};
class NoOutTag {};
constexpr NoOutTag NoOut;
template<> class Formatter<NoOutTag> {
public:
template<class Stream> constexpr void operator()(Stream&&, NoOutTag) const {}
};
template<class R> concept FormatableRange= std::ranges::forward_range<R> && requires { sizeof(Formatter<std::decay_t<std::ranges::range_value_t<R>>>) != 0; };
template<FormatableRange R> class Formatter<R> {
template<class Stream, class T, class... Args> constexpr void print(Stream&& stream, T&& r, Args&&... args) const {
auto first= std::ranges::begin(r);
auto last= std::ranges::end(r);
if(first == last) return;
Formatter<std::decay_t<std::ranges::range_value_t<R>>> formatter;
while(true) {
formatter(stream, *first, args...);
++first;
if(first != last) {
Formatter<ctype::c8>{}(stream, ' ');
} else break;
}
}
public:
template<class Stream, class... Args> constexpr void operator()(Stream&& stream, R& r, Args&&... args) const { print(std::forward<Stream>(stream), r, std::forward<Args>(args)...); }
template<class Stream, class... Args> constexpr void operator()(Stream&& stream, const R& r, Args&&... args) const { print(std::forward<Stream>(stream), r, std::forward<Args>(args)...); }
template<class Stream, class... Args> constexpr void operator()(Stream&& stream, R&& r, Args&&... args) const { print(std::forward<Stream>(stream), r, std::forward<Args>(args)...); }
template<class Stream, class... Args> constexpr void operator()(Stream&& stream, const R&& r, Args&&... args) const { print(std::forward<Stream>(stream), r, std::forward<Args>(args)...); }
};
namespace internal {
template<class T, class U> constexpr bool FormatableTupleImpl= false;
template<class T, std::size_t... I> constexpr bool FormatableTupleImpl<T, std::integer_sequence<std::size_t, I...>> = (... && requires { sizeof(Formatter<std::decay_t<typename std::tuple_element<I, T>::type>>) != 0; });
}// namespace internal
template<class T> concept FormatableTuple= requires { std::tuple_size<T>::value; } && internal::FormatableTupleImpl<T, std::make_index_sequence<std::tuple_size<T>::value>>;
template<FormatableTuple T>
requires(!FormatableRange<T>)
class Formatter<T> {
template<std::size_t I, class Stream, class U, class... Args> constexpr void print_element(Stream&& stream, U&& x, Args&&... args) const {
using std::get;
using element_type= std::decay_t<std::tuple_element_t<I, T>>;
if constexpr(requires { x.template get<I>(); }) Formatter<element_type>{}(stream, x.template get<I>(), args...);
else Formatter<element_type>{}(stream, get<I>(x), args...);
if constexpr(I < std::tuple_size<T>::value - 1) {
Formatter<ctype::c8>{}(stream, ' ');
print_element<I + 1>(std::forward<Stream>(stream), x, std::forward<Args>(args)...);
}
}
template<class Stream, class U, class... Args> constexpr void print(Stream&& stream, U&& x, Args&&... args) const {
if constexpr(std::tuple_size<T>::value != 0) print_element<0>(std::forward<Stream>(stream), x, std::forward<Args>(args)...);
}
public:
template<class Stream, class... Args> constexpr void operator()(Stream&& stream, T& x, Args&&... args) const { print(std::forward<Stream>(stream), x, std::forward<Args>(args)...); }
template<class Stream, class... Args> constexpr void operator()(Stream&& stream, const T& x, Args&&... args) const { print(std::forward<Stream>(stream), x, std::forward<Args>(args)...); }
template<class Stream, class... Args> constexpr void operator()(Stream&& stream, T&& x, Args&&... args) const { print(std::forward<Stream>(stream), x, std::forward<Args>(args)...); }
template<class Stream, class... Args> constexpr void operator()(Stream&& stream, const T&& x, Args&&... args) const { print(std::forward<Stream>(stream), x, std::forward<Args>(args)...); }
};
}// namespace gsh
#include <unistd.h>
#if defined(__linux__)
#include <sys/mman.h>// mmap
#include <sys/stat.h>// stat, fstat
#endif
namespace gsh {
namespace internal {
template<class D> class IstreamInterface;
}// namespace internal
template<class D, class Types, class... Args> class ParsingChain;
class NoParsingResult {
template<class D, class Types, class... Args> friend class ParsingChain;
constexpr NoParsingResult() noexcept {}
NoParsingResult(const NoParsingResult&)= delete;
NoParsingResult(NoParsingResult&&)= delete;
};
class CustomParser {
~CustomParser()= delete;
};
template<class D, class... Types, class... Args> class ParsingChain<D, TypeArr<Types...>, Args...> {
friend class internal::IstreamInterface<D>;
template<class D2, class Types2, class... Args2> friend class ParsingChain;
D& ref;
[[no_unique_address]] std::tuple<Args...> args;
GSH_INTERNAL_INLINE constexpr ParsingChain(D& r, std::tuple<Args...>&& a) noexcept: ref(r), args(std::move(a)) {}
template<class... Options>
requires(sizeof...(Args) < sizeof...(Types))
GSH_INTERNAL_INLINE constexpr auto next_chain(Options&&... options) const noexcept {
return ParsingChain<D, TypeArr<Types...>, Args..., std::tuple<Options...>>(ref, std::tuple_cat(args, std::make_tuple(std::forward_as_tuple(std::forward<Options>(options)...))));
};
public:
ParsingChain()= delete;
ParsingChain(const ParsingChain&)= delete;
ParsingChain(ParsingChain&&)= delete;
ParsingChain& operator=(const ParsingChain&)= delete;
ParsingChain& operator=(ParsingChain&&)= delete;
template<class... Options>
requires(sizeof...(Args) == 0)
[[nodiscard]] constexpr auto option(Options&&... options) const noexcept {
return next_chain(std::forward<Options>(options)...);
}
template<class... Options>
requires(sizeof...(Args) != 0)
[[nodiscard]] constexpr auto operator()(Options&&... options) const noexcept {
return next_chain(std::forward<Options>(options)...);
}
template<std::size_t N> friend constexpr decltype(auto) get(const ParsingChain& chain) {
auto get_result= [](auto&& parser, auto&&... args) GSH_INTERNAL_INLINE -> decltype(auto) {
if constexpr(std::is_void_v<std::invoke_result_t<decltype(parser), decltype(args)...>>) {
Invoke(std::forward<decltype(parser)>(parser), std::forward<decltype(args)>(args)...);
return NoParsingResult{};
} else {
return Invoke(std::forward<decltype(parser)>(parser), std::forward<decltype(args)>(args)...);
}
};
using value_type= typename TypeArr<Types...>::template type<N>;
if constexpr(N < sizeof...(Args)) {
if constexpr(std::same_as<CustomParser, value_type>) {
return std::apply([get_result, &chain](auto&& parser, auto&&... args) -> decltype(auto) { return get_result(std::forward<decltype(parser)>(parser), chain.ref, std::forward<decltype(args)>(args)...); }, std::get<N>(chain.args));
} else {
return std::apply([get_result, &chain](auto&&... args) -> decltype(auto) { return get_result(Parser<value_type>(), chain.ref, std::forward<decltype(args)>(args)...); }, std::get<N>(chain.args));
}
} else {
return get_result(Parser<value_type>(), chain.ref);
}
}
constexpr void ignore() const noexcept {
[this]<itype::u32... I>(std::integer_sequence<itype::u32, I...>) {
(..., get<I>(*this));
}(std::make_integer_sequence<itype::u32, sizeof...(Types)>());
}
template<class T> constexpr operator T() const noexcept {
static_assert(sizeof...(Types) == 1);
return static_cast<T>(get<0>(*this));
}
constexpr decltype(auto) val() const noexcept {
static_assert(sizeof...(Types) == 1);
return get<0>(*this);
}
template<class... To>
requires(sizeof...(To) == 0 || sizeof...(To) == sizeof...(Types))
constexpr auto bind() const noexcept {
if constexpr(sizeof...(To) == 0) {
return [this]<itype::u32... I>(std::integer_sequence<itype::u32, I...>) {
return std::tuple{get<I>(*this)...};
}(std::make_integer_sequence<itype::u32, sizeof...(Types)>());
} else {
return [this]<itype::u32... I>(std::integer_sequence<itype::u32, I...>) {
return std::tuple<To...>{static_cast<To>(get<I>(*this))...};
}(std::make_integer_sequence<itype::u32, sizeof...(Types)>());
}
}
};
}// namespace gsh
namespace std {
template<class D, class... Types, class... Args> class tuple_size<gsh::ParsingChain<D, gsh::TypeArr<Types...>, Args...>>: public integral_constant<size_t, sizeof...(Types)> {};
template<size_t N, class D, class... Types, class... Args> class tuple_element<N, gsh::ParsingChain<D, gsh::TypeArr<Types...>, Args...>> {
public:
using type= decltype(get<N>(std::declval<const gsh::ParsingChain<D, gsh::TypeArr<Types...>, Args...>&>()));
};
}// namespace std
namespace gsh {
namespace internal {
template<class D> class IstreamInterface {
constexpr D& derived() { return *static_cast<D*>(this); }
public:
template<class T, class... Types> [[nodiscard]] constexpr auto read() { return ParsingChain<D, TypeArr<T, Types...>>(derived(), std::tuple<>()); }
};
template<class D> class OstreamInterface {
constexpr D& derived() { return *static_cast<D*>(this); }
public:
template<class Sep> constexpr void write_sep(Sep&&) {}
template<class Sep, class T, class... Args> constexpr void write_sep(Sep&& sep, T&& x, Args&&... args) {
Formatter<std::decay_t<T>>{}(derived(), std::forward<T>(x));
if constexpr(sizeof...(Args) != 0) {
Formatter<std::decay_t<Sep>>{}(derived(), sep);
write_sep(std::forward<Sep>(sep), std::forward<Args>(args)...);
}
}
template<class... Args> constexpr void write(Args&&... args) { write_sep(' ', std::forward<Args>(args)...); }
template<class Sep, class... Args> constexpr void writeln_sep(Sep&& sep, Args&&... args) {
write_sep(std::forward<Sep>(sep), std::forward<Args>(args)...);
Formatter<ctype::c8>{}(derived(), '\n');
}
template<class... Args> constexpr void writeln(Args&&... args) {
write_sep(' ', std::forward<Args>(args)...);
Formatter<ctype::c8>{}(derived(), '\n');
}
};
}// namespace internal
template<itype::u32 Bufsize= (1 << 17)> class BasicReader: public internal::IstreamInterface<BasicReader<Bufsize>> {
itype::i32 fd= 0;
ctype::c8 buf[Bufsize + 1]= {};
ctype::c8 *cur= buf, *eof= buf;
public:
BasicReader() {}
BasicReader(itype::i32 filehandle): fd(filehandle) {}
BasicReader(const BasicReader& rhs) {
fd= rhs.fd;
std::memcpy(buf, rhs.buf, rhs.eof - rhs.cur);
cur= buf + (rhs.cur - rhs.buf);
eof= buf + (rhs.cur - rhs.eof);
}
BasicReader& operator=(const BasicReader& rhs) {
fd= rhs.fd;
std::memcpy(buf, rhs.buf, rhs.eof - rhs.cur);
cur= buf + (rhs.cur - rhs.buf);
eof= buf + (rhs.cur - rhs.eof);
return *this;
}
void reload() {
if(eof == buf + Bufsize || eof == cur || [&] {
auto p= cur;
while(*p >= '!') ++p;
return p;
}() == eof) [[likely]] {
itype::u32 rem= eof - cur;
std::memmove(buf, cur, rem);
*(eof= buf + rem + read(fd, buf + rem, Bufsize - rem))= '\0';
cur= buf;
}
}
void reload(itype::u32 len) {
if(avail() < len) [[unlikely]]
reload();
}
itype::u32 avail() const { return eof - cur; }
const ctype::c8* current() const { return cur; }
void skip(itype::u32 n) { cur+= n; }
};
class MmapReader: public internal::IstreamInterface<MmapReader> {
[[maybe_unused]] const itype::i32 fh;
ctype::c8 *buf, *cur, *eof;
public:
MmapReader(): fh(0) {
#if !defined(__linux__)
buf= nullptr;
write(1, "gsh::MmapReader / gsh::MmapReader is not available for Windows.\n", 64);
std::exit(1);
#else
struct stat st;
fstat(0, &st);
buf= reinterpret_cast<ctype::c8*>(mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0));
cur= buf;
eof= buf + st.st_size;
#endif
}
void reload() const {}
void reload(itype::u32) const {}
itype::u32 avail() const { return eof - cur; }
const ctype::c8* current() const { return cur; }
void skip(itype::u32 n) { cur+= n; }
};
class StaticStrReader: public internal::IstreamInterface<StaticStrReader> {
const ctype::c8* cur;
public:
constexpr StaticStrReader() {}
constexpr StaticStrReader(const ctype::c8* c): cur(c) {}
constexpr void reload() const {}
constexpr void reload(itype::u32) const {}
constexpr itype::u32 avail() const { return static_cast<itype::u32>(-1); }
constexpr const ctype::c8* current() { return cur; }
constexpr void skip(itype::u32 n) { cur+= n; }
};
template<itype::u32 Bufsize= (1 << 17)> class BasicWriter: public internal::OstreamInterface<BasicWriter<Bufsize>> {
itype::i32 fd= 1;
ctype::c8 buf[Bufsize + 1]= {};
ctype::c8 *cur= buf, *eof= buf + Bufsize;
public:
BasicWriter() {}
BasicWriter(itype::i32 filehandle): fd(filehandle) {}
BasicWriter(const BasicWriter& rhs) {
fd= rhs.fd;
std::memcpy(buf, rhs.buf, rhs.cur - rhs.buf);
cur= buf + (rhs.cur - rhs.buf);
}
BasicWriter& operator=(const BasicWriter& rhs) {
fd= rhs.fd;
std::memcpy(buf, rhs.buf, rhs.cur - rhs.buf);
cur= buf + (rhs.cur - rhs.buf);
return *this;
}
void reload() {
[[maybe_unused]] itype::i32 tmp= write(fd, buf, cur - buf);
cur= buf;
}
void reload(itype::u32 len) {
if(eof - cur < len) [[unlikely]]
reload();
}
itype::u32 avail() const { return eof - cur; }
ctype::c8* current() { return cur; }
void skip(itype::u32 n) { cur+= n; }
};
class StaticStrWriter: public internal::OstreamInterface<StaticStrWriter> {
ctype::c8* cur;
public:
constexpr StaticStrWriter() {}
constexpr StaticStrWriter(ctype::c8* c): cur(c) {}
constexpr void reload() const {}
constexpr void reload(itype::u32) const {}
constexpr itype::u32 avail() const { return static_cast<itype::u32>(-1); }
constexpr ctype::c8* current() { return cur; }
constexpr void skip(itype::u32 n) { cur+= n; }
};
}// namespace gsh
namespace gsh {
class Timer {
std::clock_t start_time;
public:
Timer() { start_time= std::clock(); }
void restart() { start_time= std::clock(); }
itype::u64 elapsed() const { return static_cast<itype::u64>(std::clock() - start_time) * 1000 / CLOCKS_PER_SEC; }
};
template<> class Formatter<Timer> {
public:
template<class Stream> void operator()(Stream&& stream, const Timer& t) {
Formatter<itype::u64>{}(stream, t.elapsed());
Formatter<ctype::c8>{}(stream, 'm');
Formatter<ctype::c8>{}(stream, 's');
}
};
}// namespace gsh
#define RANGE(V) gsh::Subrange(std::ranges::begin(V), std::ranges::end(V));
#define RRANGE(V) gsh::Subrange(std::ranges::rbegin(V), std::ranges::rend(V));
#define NMIN(T) (std::numeric_limits<T>::lowest())
#define NMAX(T) (std::numeric_limits<T>::max())
// clang-format off
#define RET_WITH(...) { __VA_ARGS__; return; } []{}
#define RETV_WITH(val, ...) { __VA_ARGS__; return val; } []{}
#define BRK_WITH(...) { __VA_ARGS__; break; } []{}
#define CTN_WITH(...) { __VA_ARGS__; continue; } []{}
#define EXT_WITH(...) { __VA_ARGS__; std::exit(0); } []{}

#define GSH_INTERNAL_ARGS0() ()
#define GSH_INTERNAL_ARGS1(a) (auto&& a)
#define GSH_INTERNAL_ARGS2(a, b) (auto&& a, auto&& b)
#define GSH_INTERNAL_ARGS3(a, b, c) (auto&& a, auto&& b, auto&& c)
#define GSH_INTERNAL_ARGS4(a, b, c, d) (auto&& a, auto&& b, auto&& c, auto&& d)
#define GSH_INTERNAL_ARGS5(a, b, c, d, e) (auto&& a, auto&& b, auto&& c, auto&& d, auto&& e)
#define GSH_INTERNAL_ARGS6(a, b, c, d, e, f) (auto&& a, auto&& b, auto&& c, auto&& d, auto&& e, auto&& f)
#define GSH_INTERNAL_ARGS7(a, b, c, d, e, f, g) (auto&& a, auto&& b, auto&& c, auto&& d, auto&& e, auto&& f, auto&& g)
#define GSH_INTERNAL_ARGS(...) GSH_INTERNAL_SELECT8(__VA_ARGS__, GSH_INTERNAL_ARGS7, GSH_INTERNAL_ARGS6, GSH_INTERNAL_ARGS5, GSH_INTERNAL_ARGS4, GSH_INTERNAL_ARGS3, GSH_INTERNAL_ARGS2, GSH_INTERNAL_ARGS1, GSH_INTERNAL_ARGS0)(__VA_ARGS__)
#define GSH_INTERNAL_LAMBDA1(...) [&]() { return (__VA_ARGS__); }
#define GSH_INTERNAL_LAMBDA2(args, ...) [&] GSH_INTERNAL_ARGS##args { return (__VA_ARGS__); }
#define LAMBDA(...) GSH_INTERNAL_SELECT3(__VA_ARGS__, GSH_INTERNAL_LAMBDA2, GSH_INTERNAL_LAMBDA1)(__VA_ARGS__)
// clang-format on
#define GSH_INTERNAL_REP1(n) std::views::iota(decltype(n)(), n)
#define GSH_INTERNAL_REP2(n, m) std::views::iota(static_cast<std::common_type_t<decltype(n), decltype(m)>>(n), static_cast<std::common_type_t<decltype(n), decltype(m)>>(m))
#define REP(varname, ...) for([[maybe_unused]] const auto& varname: GSH_INTERNAL_SELECT3(__VA_ARGS__, GSH_INTERNAL_REP2, GSH_INTERNAL_REP1)(__VA_ARGS__))
#define RREP(varname, ...) for([[maybe_unused]] const auto& varname: GSH_INTERNAL_SELECT3(__VA_ARGS__, GSH_INTERNAL_REP2, GSH_INTERNAL_REP1)(__VA_ARGS__) | std::views::reverse)
namespace gsh {
namespace internal {
template<class T> class InputAdapter {
T& ref;
template<class... Args> class with_options {
friend class InputAdapter;
T& ref;
std::tuple<Args...> args;
constexpr with_options(T& r, Args&&... a): ref(r), args(std::forward<Args>(a)...) {}
public:
template<class U> constexpr operator U() const {
return [&]<std::size_t... I>(std::index_sequence<I...>) {
return ref.template read<U>().option(std::get<I>(args)...).val();
}(std::make_index_sequence<sizeof...(Args)>{});
}
};
public:
constexpr InputAdapter(T& r) noexcept: ref(r) {}
template<class U> constexpr operator U() const { return ref.template read<U>().val(); }
template<class... Args> constexpr auto operator()(Args&&... args) const { return with_options<Args...>(ref, std::forward<Args>(args)...); }
template<class... Args> constexpr auto tied_containers(itype::u32 n) const {
std::tuple<Args...> containers;
std::tuple<Parser<typename Args::value_type>...> parsers;
[&]<std::size_t... I>(std::index_sequence<I...>) {
auto reserve= [&](auto& container) {
if constexpr(requires { container.reserve(n); }) container.reserve(n);
};
(..., reserve(std::get<I>(containers)));
}(std::make_index_sequence<sizeof...(Args)>());
for(itype::u32 i= 0; i != n; ++i) {
[&]<std::size_t... I>(std::index_sequence<I...>) {
auto add_value= [&]<class C>(C& container, auto& parser) {
if constexpr(requires { container.push_back(parser(ref)); }) container.push_back(parser(ref));
else if constexpr(requires { container.insert(parser(ref)); }) container.insert(parser(ref));
else if constexpr(requires { container.push(parser(ref)); }) container.push(parser(ref));
else static_assert((container, false));
};
(..., add_value(std::get<I>(containers), std::get<I>(parsers)));
}(std::make_index_sequence<sizeof...(Args)>());
}
return containers;
}
};
}// namespace internal
}// namespace gsh
// clang-format off
#define DECLARE_INPUT_STREAM(name) [[maybe_unused]] const gsh::internal::InputAdapter GSH_INTERNAL_INPUT{ name }; [](){}
// clang-format on
#define GSH_INTERNAL_INPUT1(a) a= GSH_INTERNAL_INPUT
#define GSH_INTERNAL_INPUT2(a, b) a= GSH_INTERNAL_INPUT, b= GSH_INTERNAL_INPUT
#define GSH_INTERNAL_INPUT3(a, b, c) a= GSH_INTERNAL_INPUT, b= GSH_INTERNAL_INPUT, c= GSH_INTERNAL_INPUT
#define GSH_INTERNAL_INPUT4(a, b, c, d) a= GSH_INTERNAL_INPUT, b= GSH_INTERNAL_INPUT, c= GSH_INTERNAL_INPUT, d= GSH_INTERNAL_INPUT
#define GSH_INTERNAL_INPUT5(a, b, c, d, e) a= GSH_INTERNAL_INPUT, b= GSH_INTERNAL_INPUT, c= GSH_INTERNAL_INPUT, d= GSH_INTERNAL_INPUT, e= GSH_INTERNAL_INPUT
#define GSH_INTERNAL_INPUT6(a, b, c, d, e, f) a= GSH_INTERNAL_INPUT, b= GSH_INTERNAL_INPUT, c= GSH_INTERNAL_INPUT, d= GSH_INTERNAL_INPUT, e= GSH_INTERNAL_INPUT, f= GSH_INTERNAL_INPUT
#define GSH_INTERNAL_INPUT7(a, b, c, d, e, f, g) a= GSH_INTERNAL_INPUT, b= GSH_INTERNAL_INPUT, c= GSH_INTERNAL_INPUT, d= GSH_INTERNAL_INPUT, e= GSH_INTERNAL_INPUT, f= GSH_INTERNAL_INPUT, g= GSH_INTERNAL_INPUT
#define INPUT(...) GSH_INTERNAL_SELECT8(__VA_ARGS__, GSH_INTERNAL_INPUT7, GSH_INTERNAL_INPUT6, GSH_INTERNAL_INPUT5, GSH_INTERNAL_INPUT4, GSH_INTERNAL_INPUT3, GSH_INTERNAL_INPUT2, GSH_INTERNAL_INPUT1)(__VA_ARGS__)
#define TIED(...) auto [__VA_ARGS__]= GSH_INTERNAL_INPUT.tied_containers
namespace kyopro {
constexpr gsh::itype::i32 inf32= (static_cast<gsh::itype::i32>(1) << 29) - 1;
constexpr gsh::itype::u32 infu32= (static_cast<gsh::itype::u32>(1) << 30) - 1;
constexpr gsh::itype::i64 inf64= (static_cast<gsh::itype::i64>(1) << 61) - 1;
constexpr gsh::itype::u64 infu64= (static_cast<gsh::itype::u64>(1) << 62) - 1;
using ll= gsh::itype::i64;
using ull= gsh::itype::u64;
using db= gsh::ftype::f64;
using ld= long double;
using vb= gsh::Vec<bool>;
using vi= gsh::Vec<gsh::itype::i32>;
using vu= gsh::Vec<gsh::itype::u32>;
using vll= gsh::Vec<gsh::itype::i64>;
using vull= gsh::Vec<gsh::itype::u64>;
using vvb= gsh::Vec2<bool>;
using vvi= gsh::Vec2<gsh::itype::i32>;
using vvu= gsh::Vec2<gsh::itype::u32>;
using vvll= gsh::Vec2<gsh::itype::i64>;
using vvull= gsh::Vec2<gsh::itype::u64>;
using vvvb= gsh::Vec3<bool>;
using vvvi= gsh::Vec3<gsh::itype::i32>;
using vvvu= gsh::Vec3<gsh::itype::u32>;
using vvvll= gsh::Vec3<gsh::itype::i64>;
using vvvull= gsh::Vec3<gsh::itype::u64>;
using str= gsh::Str;
using vstr= gsh::Vec<gsh::Str>;
using pii= std::pair<gsh::itype::i32, gsh::itype::i32>;
using pll= std::pair<gsh::itype::i64, gsh::itype::i64>;
}// namespace kyopro
#if defined(HEURISTIC) && !defined(HEURISTIC_TEST) && !defined(ONLINE_JUDGE)
#include <fcntl.h>
namespace kyopro {
gsh::BasicReader rd(open("IO-In.txt", O_RDONLY));
gsh::BasicWriter wt(open("IO-Out.txt", O_WRONLY | O_TRUNC));
}// namespace kyopro
#else
namespace kyopro {
#if defined(ONLINE_JUDGE)
gsh::MmapReader rd;
#else
gsh::BasicReader rd;
#endif
#if 1 || !defined ONLINE_JUDGE
gsh::BasicWriter wt;
#else
#define NO_WT
#endif
}// namespace kyopro
#endif
namespace kyopro {
void Yes() {
wt.writeln("Yes");
}
void No() {
wt.writeln("No");
}
void YesNo(bool f) {
wt.writeln(f ? "Yes" : "No");
}
}// namespace kyopro
void Main();
int main() {
#ifdef ONLINE_JUDGE
Main();
#ifndef NO_WT
kyopro::wt.reload();
#endif
#else
try {
Main();
kyopro::wt.reload();
} catch(gsh::Exception& e) {
kyopro::wt.writeln("gsh::Exception was throwed:", e.what());
kyopro::wt.reload();
return 1;
}
#endif
}
namespace atcoder {
}
#define rep REP
#define rrep RREP
#define in INPUT
#define tied TIED
#define out wt.writeln
#define Debug DEBUG
#define All(...) std::ranges::begin(__VA_ARGS__), std::ranges::end(__VA_ARGS__)
#define RAll(...) std::ranges::rbegin(__VA_ARGS__), std::ranges::rend(__VA_ARGS__)
using namespace std;
using namespace gsh;
using namespace gsh::itype;
using namespace gsh::ftype;
using namespace gsh::ctype;
using namespace kyopro;
using namespace atcoder;
void Main() {
DECLARE_INPUT_STREAM(rd);
/*--------------------------------------------------------------*/
in(u32 T);
while(T--) {
in(u32 N);
Arr<Point2<i32>> p(N);
for(u32 i= 0; i != N; ++i) in(p[i].x, p[i].y);
auto res= ConvexHull(p);
out(res.size());
for(auto [x, y]: res) {
out(x, y);
}
}
}
