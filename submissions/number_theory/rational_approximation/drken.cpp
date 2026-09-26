//
// Stern-Brocot 木上の二分探索
//
// verified
//   Library Checker - Rational Approximation
//     https://judge.yosupo.jp/problem/rational_approximation
//


#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")

#include <bits/stdc++.h>
using namespace std;


// rational number
template<class T = long long> struct frac {
    // gcd
    static T gcd(T a, T b) {
        a = max(a, -a), b = max(b, -b);
        while (b) {
            a %= b;
            swap(a, b);
        }
        return a;
    }

    // inner values
    T first, second;

    // constructor
    frac& normalize() {
        if (first == 0 && second != 0) {
            second = 1;
            return *this;
        }
        if (second == 0 && first != 0) {
            first = 1;
            return *this;
        }
        if (second < 0) first = -first, second = -second;
        T d = gcd(max(first, -first), second);
        if (d == 0) first = 0, second = 1;
        else first /= d, second /= d;
        return *this;
    }
    frac(const frac&) = default;
    frac& operator = (const frac&) = default;
    constexpr frac(T f = 0, T s = 1) : first(f), second(s) { 
        normalize(); 
    }
    constexpr frac& operator = (T a) { 
        *this = frac(a, 1); 
        return *this;
    }

    // comparison operators
    constexpr bool operator == (const frac &r) const {
        return this->first == r.first && this->second == r.second;
    }
    constexpr bool operator != (const frac &r) const {
        return this->first != r.first || this->second != r.second;
    }
    constexpr bool operator < (const frac &r) const {
        return this->first * r.second < this->second * r.first;
    }
    constexpr bool operator > (const frac &r) const {
        return this->first * r.second > this->second * r.first;
    }
    constexpr bool operator <= (const frac &r) const {
        return this->first * r.second <= this->second * r.first;
    }
    constexpr bool operator >= (const frac &r) const {
        return this->first * r.second >= this->second * r.first;
    }
    
    // arithmetic operators
    constexpr frac& operator += (const frac &r) {
        this->first = this->first * r.second + this->second * r.first;
        this->second *= r.second;
        this->normalize();
        return *this;
    }
    constexpr frac& operator -= (const frac &r) {
        this->first = this->first * r.second - this->second * r.first;
        this->second *= r.second;
        this->normalize();
        return *this;
    }
    constexpr frac& operator *= (const frac &r) {
        this->first *= r.first;
        this->second *= r.second;
        this->normalize();
        return *this;
    }
    constexpr frac& operator /= (const frac &r) {
        this->first *= r.second;
        this->second *= r.first;
        this->normalize();
        return *this;
    }
    constexpr frac operator + () const { return frac(*this); }
    constexpr frac operator - () const { return frac(0) - frac(*this); }
    constexpr frac operator + (const frac &r) const { return frac(*this) += r; }
    constexpr frac operator - (const frac &r) const { return frac(*this) -= r; }
    constexpr frac operator * (const frac &r) const { return frac(*this) *= r; }
    constexpr frac operator / (const frac &r) const { return frac(*this) /= r; }
    friend constexpr ostream& operator << (ostream &os, const frac<T> &x) {
        os << x.first; 
        if (x.second != 1) os << "/" << x.second;
        return os;
    }
};

// binary search on Stern-Brocot tree
// return {l (= a/b), r (= c/d(} s.t. l: OK, r: NG, and a, b, c, d are maximized where a, b, c, d <= lim
template<class T, class Func> tuple<T, T, T, T> rational_binary_search(Func check, T lim) {
    assert(check(0, 1));
    assert(!check(1, 0));
    auto rec = [&](auto &&rec, bool which, T &a, T &b, T c, T d) -> void {
        if (a + c > lim || b + d > lim) return;
        if (check(a + c, b + d) == which) {
            a += c, b += d;
            rec(rec, which, a, b, c + c, d + d);
        }
        if (a + c <= lim && b + d <= lim && check(a + c, b + d) == which) a += c, b += d;
    };
    T a = 0, b = 1, c = 1, d = 0;
    while (a + c <= lim && b + d <= lim) {
        rec(rec, true, a, b, c, d);
        rec(rec, false, c, d, a, b);
    }
    return {a, b, c, d};
}


//------------------------------//
// Fast IO
//------------------------------//

struct FastRead {
    static constexpr int BUF_SIZE = 1 << 17;

private:
    FILE *stream_;
    array<char, BUF_SIZE> buf_;
    char *begin_, *end_, *ptr_;

    // reader
    void skip_space() {
        while (*ptr_ <= ' ') ++ptr_;
    }
    template<int N = 0> void read() {
        if (const auto n = end_ - ptr_; n <= N) {
            ignore = fread(copy_n(ptr_, n, begin_), 1, BUF_SIZE - n, stream_);
            ptr_ = begin_;
        }
    }
    
    // parser
    template<typename T> void parse(T &x) {
        common_type_t<T, uint64_t> x2 = 0;
        while (true) {
            uint64_t v;
            memcpy(&v, ptr_, 8);
            if ((v -= 0x3030303030303030) & 0x8080808080808080) break;
            v = (v * 10 + (v >> 8)) & 0xff00ff00ff00ff;
            v = (v * 100 + (v >> 16)) & 0xffff0000ffff;
            v = (v * 10000 + (v >> 32)) & 0xffffffff;
            x2 = 100000000 * x2 + v;
            ptr_ += 8;
        }
        while (true) {
            uint32_t v;
            memcpy(&v, ptr_, 4);
            if ((v -= 0x30303030) & 0x80808080) break;
            v = (v * 10 + (v >> 8)) & 0xff00ff;
            v = (v * 100 + (v >> 16)) & 0xffff;
            x2 = 10000 * x2 + v;
            ptr_ += 4;
            break;
        }
        while (true) {
            uint16_t v;
            memcpy(&v, ptr_, 2);
            if ((v -= 0x3030) & 0x8080) break;
            v = (v * 10 + (v >> 8)) & 0xff;
            x2 = 100 * x2 + v;
            ptr_ += 2;
            break;
        }
        if (' ' < *ptr_) {
            x2 *= 10;
            x2 += *ptr_++ - '0';
        }
        ++ptr_;
        x = static_cast<T>(x2);
    }
    
public:
    // constructor
    FastRead() : FastRead(stdin) {}
    explicit FastRead(const filesystem::path& p) : FastRead(fopen(p.c_str(), "r")) {}
    explicit FastRead(FILE *stream)
    : stream_(stream), begin_(buf_.data()), end_(begin_ + BUF_SIZE), ptr_(end_) { 
        read(); 
    }
    ~FastRead() { 
        if (stream_ != stdin) fclose(stream_); 
    }
    FastRead(const FastRead&) = delete;
    FastRead &operator = (const FastRead&) = delete;
    
    // operators
    template<unsigned_integral T> void operator () (T &x) {
        skip_space();
        read<64>();
        parse(x);
    }
    template<signed_integral T> void operator () (T &x) {
        skip_space();
        read<64>();
        make_unsigned_t<T> u;
        if (*ptr_ == '-') {
            ++ptr_;
            parse(u);
            u = -u;
        } else {
            parse(u);
        }
        x = u;
    }
    void operator () (char &x) {
        skip_space();
        read<64>();
        x = *ptr_;
        ++ptr_;
    }
    void operator () (string &x) {
        x = "";
        skip_space();
        read<64>();
        while (*ptr_ > ' ' && *ptr_ != '\0') {
            x.push_back(*ptr_);
            ++ptr_;
        }
        ++ptr_;
    }
    template<class... Ts> requires(sizeof...(Ts) != 1) void operator () (Ts&... xs) {
        ((*this)(xs), ...);
    }
    template<class T> FastRead& operator >> (T &x) { (*this)(x); return *this; }
};

class FastWrite {
    static constexpr int BUF_SIZE = 1 << 17;

private:
    FILE *stream_;
    array<char, BUF_SIZE> buf_;
    char *begin_, *end_, *ptr_;
    
    // preparation
    template <class T> static constexpr int DIGITS = numeric_limits<T>::digits10 + 1;
    template <class T> static constexpr auto POW10 = [] {
        array<T, DIGITS<T>> ret;
        ret[0] = 1;
        for (int i = 1; i < DIGITS<T>; ++i) {
            ret[i] = 10 * ret[i - 1];
        }
        return ret;
    } ();
    static constexpr auto LUT = [] {
        array<char, 40000> res;
        char* p = res.data();
        char a = '0', b = '0', c = '0', d = '0';
        do {
            *p++ = a, *p++ = b, *p++ = c, *p++ = d;
        } while (d++ < '9'
                 || (d = '0', c++ < '9'
                     || (c = '0', b++ < '9'
                         || (b = '0', a++ < '9'))));
        return res;
    } ();
    
    // flush
    template<int N = BUF_SIZE> void flush() {
        if (end_ - ptr_ <= N) {
            fwrite(begin_, 1, ptr_ - begin_, stream_);
            ptr_ = begin_;
        }
    }
    
    // writer
    template<int N = 4> void le4(uint64_t x) {
        if constexpr (1 < N) {
            if (x < POW10<uint64_t>[N - 1]) {
                le4<N - 1>(x);
                return;
            }
        }
        ptr_ = copy_n(&LUT[x * 4 + (4 - N)], N, ptr_);
    }
    template<int N> void w4(uint64_t x) {
        if constexpr (0 < N) {
            ptr_ = copy_n(&LUT[x / POW10<uint64_t>[N - 4] * 4], 4, ptr_);
            w4<N - 4>(x % POW10<uint64_t>[N - 4]);
        }
    }
    template<int N> void write(uint64_t x) {
        if constexpr (N < DIGITS<uint64_t>) {
            if (POW10<uint64_t>[N] <= x) {
                write<N + 4>(x);
                return;
            }
        }
        le4(x / POW10<uint64_t>[N - 4]);
        w4<N - 4>(x % POW10<uint64_t>[N - 4]);
    }
    template<typename T> void write(T x) {
        write<4>(x);
    }
    void write(__uint128_t x) {
        if (x < POW10<__uint128_t>[16]) {
            write(static_cast<uint64_t>(x));
        } else if (x < POW10<__uint128_t>[32]) {
            write(static_cast<uint64_t>(x / POW10<__uint128_t>[16]));
            w4<16>(static_cast<uint64_t>(x % POW10<__uint128_t>[16]));
        } else {
            write(static_cast<uint64_t>(x / POW10<__uint128_t>[32]));
            x %= POW10<__uint128_t>[32];
            w4<16>(static_cast<uint64_t>(x / POW10<__uint128_t>[16]));
            w4<16>(static_cast<uint64_t>(x % POW10<__uint128_t>[16]));
        }
    }
    
public:
    // constructor
    FastWrite() : FastWrite(stdout) {}
    explicit FastWrite(const filesystem::path& p) : FastWrite(fopen(p.c_str(), "w")) {}
    explicit FastWrite(FILE* stream)
    : stream_(stream), begin_(buf_.data()), end_(begin_ + BUF_SIZE), ptr_(begin_) {}
    ~FastWrite() {
        flush();
        if (stream_ != stdout) { fclose(stream_); }
    }
    FastWrite(const FastWrite&) = delete;
    FastWrite& operator = (const FastWrite&) = delete;
    
    // operators
    template<unsigned_integral T> void operator () (T x) {
        flush<DIGITS<T>>();
        write(x);
    }
    template<signed_integral T> void operator () (T x) {
        flush<1 + DIGITS<T>>();
        using U = make_unsigned_t<T>;
        const U u = x;
        if (x < 0) {
            *ptr_++ = '-';
            write(static_cast<U>(-u));
        } else {
            write(u);
        }
    }
    void operator () (char c) {
        flush<1>();
        *ptr_++ = c;
    }
    void operator () (string_view s) {
        while (!s.empty()) {
            flush<0>();
            const auto n = min(ssize(s), end_ - ptr_);
            if (n == BUF_SIZE) {
                fwrite(s.data(), 1, BUF_SIZE, stream_);
            } else {
                ptr_ = copy_n(s.data(), n, ptr_);
            }
            s.remove_prefix(n);
        }
        flush<0>();
    }
    template <char End = '\n', char Sep = ' ', class T, class... Ts>
    void ln(T&& x, Ts&&... xs) {
        (*this)(std::forward<T>(x));
        if constexpr (sizeof...(Ts) == 0) {
            *ptr_++ = End;
        } else {
            *ptr_++ = Sep;
            ln<End, Sep>(std::forward<Ts>(xs)...);
        }
    }
    template<class T> FastWrite& operator << (T x) { (*this)(x); return *this; }
};


//------------------------------//
// Examples
//------------------------------//

// Library Checker - Rational Approximation
void LibraryCheckerRatilnalApproximation() {
    FastRead Read; FastWrite Write;
    int T;
    Read(T);
    while (T--) {
        long long N, x, y;
        Read(N, x, y);
        auto check = [&](long long a, long long b) -> bool { return a * y <= b * x; };
        auto [a, b, c, d] = rational_binary_search(check, N);
        if (a * y == b * x)
            Write(a), Write(' '), Write(b), Write(' '), Write(a), Write(' '), Write(b), Write('\n');
        else 
            Write(a), Write(' '), Write(b), Write(' '), Write(c), Write(' '), Write(d), Write('\n');
    }
}


int main() {
    LibraryCheckerRatilnalApproximation();
}