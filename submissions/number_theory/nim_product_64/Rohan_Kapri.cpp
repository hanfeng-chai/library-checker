#line 1 "main.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/nim_product_64"
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt,pclmul")
#define CP_ALGO_CHECKPOINT
#include <iostream>
#include <immintrin.h>
#line 1 "cp-algorithms-aux/cp-algo/util/big_alloc.hpp"
#include <set>
#include <map>
#include <deque>
#include <stack>
#include <queue>
#include <vector>
#include <string>
#include <cstddef>
#line 13 "cp-algorithms-aux/cp-algo/util/big_alloc.hpp"
#include <generator>
#include <forward_list>

#if defined(__linux__) || defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#  define CP_ALGO_USE_MMAP 1
#  include <sys/mman.h>
#else
#  define CP_ALGO_USE_MMAP 0
#endif

namespace cp_algo {
    template <typename T, size_t Align = 32>
    class big_alloc {
        static_assert( Align >= alignof(void*), "Align must be at least pointer-size");
        static_assert(std::popcount(Align) == 1, "Align must be a power of two");
    public:
        using value_type = T;
        template <class U> struct rebind { using other = big_alloc<U, Align>; };
        constexpr bool operator==(const big_alloc&) const = default;
        constexpr bool operator!=(const big_alloc&) const = default;

        big_alloc() noexcept = default;
        template <typename U, std::size_t A>
        big_alloc(const big_alloc<U, A>&) noexcept {}

        [[nodiscard]] T* allocate(std::size_t n) {
            std::size_t padded = round_up(n * sizeof(T));
            std::size_t align = std::max<std::size_t>(alignof(T),  Align);
#if CP_ALGO_USE_MMAP
            if (padded >= MEGABYTE) {
                void* raw = mmap(nullptr, padded,
                                PROT_READ | PROT_WRITE,
                                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                madvise(raw, padded, MADV_HUGEPAGE);
                madvise(raw, padded, MADV_POPULATE_WRITE);
                return static_cast<T*>(raw);
            }
#endif
            return static_cast<T*>(::operator new(padded, std::align_val_t(align)));
        }

        void deallocate(T* p, std::size_t n) noexcept {
            if (!p) return;
            std::size_t padded = round_up(n * sizeof(T));
            std::size_t align  = std::max<std::size_t>(alignof(T),  Align);
    #if CP_ALGO_USE_MMAP
            if (padded >= MEGABYTE) { munmap(p, padded); return; }
    #endif
            ::operator delete(p, padded, std::align_val_t(align));
        }

    private:
        static constexpr std::size_t MEGABYTE = 1 << 20;
        static constexpr std::size_t round_up(std::size_t x) noexcept {
            return (x + Align - 1) / Align * Align;
        }
    };

    template<typename T> using big_vector = std::vector<T, big_alloc<T>>;
    template<typename T> using big_basic_string = std::basic_string<T, std::char_traits<T>, big_alloc<T>>;
    template<typename T> using big_deque = std::deque<T, big_alloc<T>>;
    template<typename T> using big_stack = std::stack<T, big_deque<T>>;
    template<typename T> using big_queue = std::queue<T, big_deque<T>>;
    template<typename T> using big_priority_queue = std::priority_queue<T, big_vector<T>>;
    template<typename T> using big_forward_list = std::forward_list<T, big_alloc<T>>;
    using big_string = big_basic_string<char>;

    template<typename Key, typename Value, typename Compare = std::less<Key>>
    using big_map = std::map<Key, Value, Compare, big_alloc<std::pair<const Key, Value>>>;
    template<typename T, typename Compare = std::less<T>>
    using big_multiset = std::multiset<T, Compare, big_alloc<T>>;
    template<typename T, typename Compare = std::less<T>>
    using big_set = std::set<T, Compare, big_alloc<T>>;
    template<typename Ref, typename V = void>

    using big_generator = std::generator<Ref, V, big_alloc<std::byte>>;
}

// Deduction guide to make elements_of with big_generator default to big_alloc
namespace std::ranges {
    template<typename Ref, typename V>
    elements_of(cp_algo::big_generator<Ref, V>&&) -> elements_of<cp_algo::big_generator<Ref, V>&&, cp_algo::big_alloc<std::byte>>;
}


#line 1 "cp-algorithms-aux/cp-algo/util/checkpoint.hpp"


#line 5 "cp-algorithms-aux/cp-algo/util/checkpoint.hpp"
#include <chrono>
#line 8 "cp-algorithms-aux/cp-algo/util/checkpoint.hpp"
namespace cp_algo {
#ifdef CP_ALGO_CHECKPOINT
    big_map<big_string, double> checkpoints;
    double last;
#endif
    template<bool final = false>
    void checkpoint([[maybe_unused]] auto const& _msg) {
#ifdef CP_ALGO_CHECKPOINT
        big_string msg = _msg;
        double now = (double)clock() / CLOCKS_PER_SEC;
        double delta = now - last;
        last = now;
        if(msg.size() && !final) {
            checkpoints[msg] += delta;
        }
        if(final) {
            for(auto const& [key, value] : checkpoints) {
                std::cerr << key << ": " << value * 1000 << " ms\n";
            }
            std::cerr << "Total: " << now * 1000 << " ms\n";
        }
#endif
    }
    template<bool final = false>
    void checkpoint() {
        checkpoint<final>("");
    }
}

#line 10 "main.cpp"
#include <bits/stdc++.h>



#define LL long long
#define LLL __int128
#define uint unsigned
#define ldb long double
#define uLL unsigned long long
using namespace std;
#define FASTIO
namespace FastIO
{
// ------------------------------
// #define DISABLE_MMAP
// ------------------------------
#if ( defined(LOCAL) || defined(_WIN32) ) && !defined(DISABLE_MMAP)
#define DISABLE_MMAP
#endif
#ifdef LOCAL
	inline constexpr void _chk_i() {}
	inline char _gc_nochk() { return getchar(); }
	inline char _gc() { return getchar(); }
	inline void _chk_o() {}
	inline void _pc_nochk(char c) { putchar(c); }
	inline void _pc(char c) { putchar(c); }
	template < int n > inline void _pnc_nochk(const char *c) { for ( int i = 0 ; i < n ; i++ ) putchar(c[i]); }
#else
#ifdef DISABLE_MMAP
	inline constexpr int _READ_SIZE = 1 << 18; inline static char _read_buffer[_READ_SIZE + 40], *_read_ptr = nullptr, *_read_ptr_end = nullptr; static inline bool _eof = false;
	inline void _chk_i() { if ( __builtin_expect(!_eof, true) && __builtin_expect(_read_ptr_end - _read_ptr < 40, false) ) { int sz = _read_ptr_end - _read_ptr; if ( sz ) memcpy(_read_buffer, _read_ptr, sz); char *beg = _read_buffer + sz; _read_ptr = _read_buffer, _read_ptr_end = beg + fread(beg, 1, _READ_SIZE, stdin); if ( __builtin_expect(_read_ptr_end != beg + _READ_SIZE, false) ) _eof = true, *_read_ptr_end = EOF; } }
	inline char _gc_nochk() { return __builtin_expect(_eof && _read_ptr == _read_ptr_end, false) ? EOF : *_read_ptr++; }
	inline char _gc() { _chk_i(); return _gc_nochk(); }
#else
#include<sys/mman.h>
	inline static char *_read_ptr = (char *)mmap(nullptr, 0x7fffffff, 1, 2, 0, 0);
	inline constexpr void _chk_i() {}
	inline char _gc_nochk() { return *_read_ptr++; }
	inline char _gc() { return *_read_ptr++; }
#endif
	inline constexpr int _WRITE_SIZE = 1 << 18; inline static char _write_buffer[_WRITE_SIZE + 40], *_write_ptr = _write_buffer;
	inline void _chk_o() { if ( __builtin_expect(_write_ptr - _write_buffer > _WRITE_SIZE, false) ) fwrite(_write_buffer, 1, _write_ptr - _write_buffer, stdout), _write_ptr = _write_buffer; }
	inline void _pc_nochk(char c) { *_write_ptr++ = c; }
	inline void _pc(char c) { *_write_ptr++ = c, _chk_o(); }
	template < int n > inline void _pnc_nochk(const char *c) { memcpy(_write_ptr, c, n), _write_ptr += n; }
	inline struct _auto_flush { inline ~_auto_flush() { fwrite(_write_buffer, 1, _write_ptr - _write_buffer, stdout); } } _auto_flush;
#endif
#define println println_ // don't use C++23 std::println
	template < class T > inline constexpr bool _is_signed = numeric_limits < T >::is_signed;
	template < class T > inline constexpr bool _is_unsigned = numeric_limits < T >::is_integer && !_is_signed < T >;
#if __SIZEOF_LONG__ == 64
	template <> inline constexpr bool _is_signed < __int128 > = true;
	template <> inline constexpr bool _is_unsigned < __uint128_t > = true;
#endif
	inline bool _isgraph(char c) { return c >= 33; }
	inline bool _isdigit(char c) { return 48 <= c && c <= 57; } // or faster, remove c <= 57
	constexpr struct _table {
#ifndef LOCAL
	int i[65536];
#endif
	char o[40000]; constexpr _table() :
#ifndef LOCAL
	i{},
#endif
	o{} {
#ifndef LOCAL
	for ( int x = 0 ; x < 65536 ; x++ ) i[x] = -1; for ( int x = 0 ; x <= 9 ; x++ ) for ( int y = 0 ; y <= 9 ; y++ ) i[x + y * 256 + 12336] = x * 10 + y;
#endif
	for ( int x = 0 ; x < 10000 ; x++ ) for ( int y = 3, z = x ; ~y ; y-- ) o[x * 4 + y] = z % 10 + 48, z /= 10; } } _table;
	template < class T, int digit > inline constexpr T _pw10 = 10 * _pw10 < T, digit - 1 >;
	template < class T > inline constexpr T _pw10 < T, 0 > = 1;
	inline void read(char &c) { do c = _gc(); while ( !_isgraph(c) ); }
	inline void read_cstr(char *s) { char c = _gc(); while ( !_isgraph(c) ) c = _gc(); while ( _isgraph(c) ) *s++ = c, c = _gc(); *s = 0; }
	inline void read(string &s) { char c = _gc(); s.clear(); while ( !_isgraph(c) ) c = _gc(); while ( _isgraph(c) ) s.push_back(c), c = _gc(); }
	template < class T, bool neg >
#ifndef LOCAL
	__attribute__((no_sanitize("undefined")))
#endif
	inline void _read_int_suf(T &x) { _chk_i(); char c; while
#ifndef LOCAL
	( ~_table.i[*reinterpret_cast < unsigned short *& >(_read_ptr)] ) if constexpr ( neg ) x = x * 100 - _table.i[*reinterpret_cast < unsigned short *& >(_read_ptr)++]; else x = x * 100 + _table.i[*reinterpret_cast < unsigned short *& >(_read_ptr)++]; if
#endif
	( _isdigit(c = _gc_nochk()) ) if constexpr ( neg ) x = x * 10 - ( c & 15 ); else x = x * 10 + ( c & 15 ); }
	template < class T, enable_if_t < _is_signed < T >, int > = 0 > inline void read(T &x) { char c; while ( !_isdigit(c = _gc()) ) if ( c == 45 ) { _read_int_suf < T, true >(x = -( _gc_nochk() & 15 )); return; } _read_int_suf < T, false >(x = c & 15); }
	template < class T, enable_if_t < _is_unsigned < T >, int > = 0 > inline void read(T &x) { char c; while ( !_isdigit(c = _gc()) ); _read_int_suf < T, false >(x = c & 15); }
	inline void write(bool x) { _pc(x | 48); }
	inline void write(char c) { _pc(c); }
	inline void write_cstr(const char *s) { while ( *s ) _pc(*s++); }
	inline void write(const string &s) { for ( char c : s ) _pc(c); }
	template < class T, bool neg, int digit > inline void _write_int_suf(T x) { if constexpr ( digit == 4 ) _pnc_nochk < 4 >(_table.o + ( neg ? -x : x ) * 4); else _write_int_suf < T, neg, digit / 2 >(x / _pw10 < T, digit / 2 >), _write_int_suf < T, neg, digit / 2 >(x % _pw10 < T, digit / 2 >); }
	template < class T, bool neg, int digit > inline void _write_int_pre(T x) { if constexpr ( digit <= 4 ) if ( digit >= 3 && ( neg ? x <= -100 : x >= 100 ) ) if ( digit >= 4 && ( neg ? x <= -1000 : x >= 1000 ) ) _pnc_nochk < 4 >(_table.o + ( neg ? -x : x ) * 4); else _pnc_nochk < 3 >(_table.o + ( neg ? -x : x ) * 4 + 1); else if ( digit >= 2 && ( neg ? x <= -10 : x >= 10 ) ) _pnc_nochk < 2 >(_table.o + ( neg ? -x : x ) * 4 + 2); else _pc_nochk(( neg ? -x : x ) | 48); else { constexpr int cur = 1 << __lg(digit - 1); if ( neg ? x <= -_pw10 < T, cur > : x >= _pw10 < T, cur > ) _write_int_pre < T, neg, digit - cur >(x / _pw10 < T, cur >), _write_int_suf < T, neg, cur >(x % _pw10 < T, cur >); else _write_int_pre < T, neg, cur >(x); } }
	template < class T, enable_if_t < _is_signed < T >, int > = 0 > inline void write(T x) { if ( x >= 0 ) _write_int_pre < T, false, numeric_limits < T >::digits10 + 1 >(x); else _pc_nochk(45), _write_int_pre < T, true, numeric_limits < T >::digits10 + 1 >(x); _chk_o(); }
	template < class T, enable_if_t < _is_unsigned < T >, int > = 0 > inline void write(T x) { _write_int_pre < T, false, numeric_limits < T >::digits10 + 1 >(x), _chk_o(); }
	template < class T > inline auto read(T &x) -> decltype(x.read(), void()) { x.read(); }
	template < class T > inline auto write(const T &x) -> decltype(x.write(), void()) { x.write(); }
	template < class T1, class ...T2 > inline void read(T1 &x, T2 &...y) { read(x), read(y...); }
	template < class ...T > inline void read_cstr(char *x, T *...y) { read_cstr(x), read_cstr(y...); }
	template < class T1, class ...T2 > inline void write(const T1 &x, const T2 &...y) { write(x), write(y...); }
	template < class ...T > inline void write_cstr(const char *x, const T *...y) { write_cstr(x), write_cstr(y...); }
	template < class T > inline void print(const T &x) { write(x); }
	inline void print_cstr(const char *x) { write_cstr(x); }
	template < class T1, class ...T2 > inline void print(const T1 &x, const T2 &...y) { write(x), _pc(32), print(y...); }
	template < class ...T > inline void print_cstr(const char *x, const T *...y) { write_cstr(x), _pc(32), print_cstr(y...); }
	inline void println() { _pc(10); }
	inline void println_cstr() { _pc(10); }
	template < class ...T > inline void println(const T &...x) { print(x...), _pc(10); }
	template < class ...T > inline void println_cstr(const T *...x) { print_cstr(x...), _pc(10); }
}	using FastIO::read, FastIO::read_cstr, FastIO::write, FastIO::write_cstr, FastIO::println, FastIO::println_cstr;



using namespace std;

constexpr auto BASIS_COL = array{
    0x0000000000000001ull, 0x5211145c804b6109ull, 0x7c8bc2cad259879full, 0x565854b4c60c1e0bull,
    0x4068acf7104c20c3ull, 0x662d2bd0f2739155ull, 0x7a90c83701fa8323ull, 0x21cfa750247e8755ull,
    0x67d1044e545abf47ull, 0x4d9d3b5a8568f839ull, 0x567a9d7331b6b3c6ull, 0x1ca54bfdd6d1ae59ull,
    0x454fa483275db25cull, 0x6766df6fec4e9d44ull, 0x35cb621cec1fe7f9ull, 0x4c606d3e52faf263ull,
    0x57640dc825a57954ull, 0x7aca87838b7f6315ull, 0x6d53c884ebf2b0edull, 0x3721d998bb50164bull,
    0x7aa7c62fd6cd53abull, 0x47cbb2c51f7c040full, 0x132063b7f5e42489ull, 0x0c1b36c8b2993f8aull,
    0x60119ecff680497aull, 0x5175da444cc11791ull, 0x5792ff4554765b09ull, 0x0c9fdb8a01334e82ull,
    0x2be0a763a68a4725ull, 0x3c2dc8260ad051f6ull, 0x6c4c9fed8816bb9cull, 0x630062753ffaf766ull,
    0x7b37d31b5d519225ull, 0x2364f7f79705691cull, 0x453eb8a83e2fec71ull, 0x7c0121b37e828666ull,
    0x59190d3250e66011ull, 0x103207f9dda18caeull, 0x28233dce01c69b76ull, 0x4fa519899227a5e7ull,
    0x4567ba46ee7bc6cdull, 0x0a284773d021afd5ull, 0x63894079bbe3a824ull, 0x11013c7fdfaaa5c2ull,
    0x1aa984f18574f3b0ull, 0x0cbaba126fd0c4dbull, 0x0b8797719e6dc725ull, 0x4a2845680aefaa72ull,
    0x536d2535f6934e15ull, 0x01db7a57effcd689ull, 0x7e1ed0ad01e2a5adull, 0x0aedc9b3cee826f6ull,
    0x7ba716eccf9f68e1ull, 0x5d5e23bc0f3dc38full, 0x0b5f2a3b88674d83ull, 0x2de9bafc2f00f8d4ull,
    0x3b56712ad419c7e0ull, 0x3ab4be8c30c19253ull, 0x2708522ffaa654b0ull, 0x2b8bca57bf643598ull,
    0x588825d1a5fa8e1cull, 0x86adf8bf4d45962full, 0x51b4c15d8719dd73ull, 0xe4a2b3b59783d0aaull
};

constexpr auto INV_COL = array{
    0x0000000000000001ull, 0x19c9369f278adc02ull, 0xa181e7d66f5ff795ull, 0x5db84357ce785d09ull,
    0xa0bae2f9d2430cc8ull, 0xb7ea5a9705b771c0ull, 0xba4f3cd82801769dull, 0x4886cde01b8241d0ull,
    0x0a6f43f2aaf612edull, 0xebd0142f98030a32ull, 0xa81f89cda43f3792ull, 0xe99aec6b66ccb814ull,
    0xa69d1ff025fc2f82ull, 0x48a81132d25db068ull, 0x4a900f9dcaa9644full, 0xe5ce4ea88259972aull,
    0xf7094c336029f04cull, 0xe191dde287bc9c6bull, 0xaacaff12bff239b8ull, 0x49bc5212be1bc1caull,
    0xfe57defb454446cfull, 0xa1dffcf944bdf6a7ull, 0xb9f1bdb5cee941eeull, 0x12e5e889275c22deull,
    0x5bcb6b117b77eeedull, 0x03eb1ab59d05ae4bull, 0x02a25d7076ddd386ull, 0x53164a606c612245ull,
    0xebb33f5822f66059ull, 0xe9be765f5747b93eull, 0x552a78df373a354full, 0xbcf5ac65f31fb8bfull,
    0xe411e728becdc77bull, 0xf35c26d7b57cdca6ull, 0x4499da83de4ca5f7ull, 0x40ab25bdca4ae226ull,
    0xee004b6f1dff7218ull, 0x0d122da9821c5b41ull, 0x51fbfcb058120efeull, 0xa148b1fa84905b22ull,
    0xbb8ed3e647604d8dull, 0xe2d93fef2472776full, 0x4c17a2541a10e6b5ull, 0x1d879e08903708e7ull,
    0x0fbe7d0d1934da90ull, 0x5bf977d9c6f61d30ull, 0x06832fc918260412ull, 0x0fe22e843ebf73e3ull,
    0x4d7ef4e4fa28d60dull, 0x402250d979afbed5ull, 0x067902b8c8ca2d4full, 0xf38d113fe1d6bb16ull,
    0x414f0248b02b5b7dull, 0xf041922915824ce9ull, 0x11a72fb5e30c93d9ull, 0x12e54f4d63102aeeull,
    0xbc46ac14b3141c6cull, 0x1f172b3c16c645bbull, 0x584b492ed4e8fa6cull, 0x00a852e9a32cc133ull,
    0xa180861bce00a45eull, 0xa194b6bcb4645fb9ull, 0x4509002ad808a4fbull, 0xc5172a0055602f69ull
};

template <const auto& COLS>
constexpr auto make_byte_tables() {
    array<array<uint64_t, 1 << 8>, 8> T{};
    for (int pos = 0; pos < 8; pos++) {
        for(int col = 0; col < 8; col++) {
            for(int mask = 0; mask < (1 << col); mask++) {
                T[pos][mask | (1 << col)] = T[pos][mask] ^ COLS[pos * 8 + col];
            }
        }
    }
    return T;
}

static constexpr auto INV_BYTE  = make_byte_tables<INV_COL>();
static constexpr auto BASIS_BYTE = make_byte_tables<BASIS_COL>();

[[gnu::always_inline]]
inline uint64_t nim_to_poly(uint64_t x) {
    auto xb = bit_cast<array<uint8_t, 8>>(x);
    return INV_BYTE[0][xb[0]] ^ INV_BYTE[1][xb[1]]
         ^ INV_BYTE[2][xb[2]] ^ INV_BYTE[3][xb[3]]
         ^ INV_BYTE[4][xb[4]] ^ INV_BYTE[5][xb[5]]
         ^ INV_BYTE[6][xb[6]] ^ INV_BYTE[7][xb[7]];
}

[[gnu::always_inline]]
inline uint64_t poly_to_nim(uint64_t c) {
    auto cb = bit_cast<array<uint8_t, 8>>(c);
    return BASIS_BYTE[0][cb[0]] ^ BASIS_BYTE[1][cb[1]]
         ^ BASIS_BYTE[2][cb[2]] ^ BASIS_BYTE[3][cb[3]]
         ^ BASIS_BYTE[4][cb[4]] ^ BASIS_BYTE[5][cb[5]]
         ^ BASIS_BYTE[6][cb[6]] ^ BASIS_BYTE[7][cb[7]];
}

// Carryless multiply over GF(2)
[[gnu::always_inline]]
inline auto clmul(uint64_t a, uint64_t b) {
    return _mm_clmulepi64_si128(__m128i{a, 0}, __m128i{b, 0}, 0);
}

// Reduce modulo x^64 + x^4 + x^3 + x + 1
[[gnu::always_inline]]
inline uint64_t reduce_mod(__m128i v) {
    v[0] ^= v[1] ^ (v[1] << 1) ^ (v[1] << 3) ^ (v[1] << 4);
    static constexpr auto RED_OVER = [] {
        array<uint64_t, 16> red{};
        for (int q = 0; q < 16; ++q) {
            uint64_t o = q ^ (q >> 1) ^ (q >> 3);
            red[q] = o ^ (o << 1) ^ (o << 3) ^ (o << 4);
        }
        return red;
    }();
    return v[0] ^ RED_OVER[v[1] >> 60];
}

// Public nimber product via isomorphism (no recursion, no Gauss at runtime)
[[gnu::always_inline]]
inline uint64_t nim_mul(uint64_t a, uint64_t b) {
    uint64_t pa = nim_to_poly(a);
    uint64_t pb = nim_to_poly(b);
    auto prod = clmul(pa, pb);
    uint64_t red = reduce_mod(prod);
    return poly_to_nim(red);
}

int main() {
    //freopen("input.txt", "r", stdin);
    int t;
    read(t);
    cp_algo::big_vector<uint64_t> A(t), B(t);
    for (int i = 0; i < t; i++) {
        read(A[i]);
        read(B[i]);
    }
    cp_algo::checkpoint("read");
    for(int i = 0; i < t; i++) {
        A[i] = nim_to_poly(A[i]);
        B[i] = nim_to_poly(B[i]);
    }
    cp_algo::checkpoint("to_poly");
    for(int i = 0; i < t; i++) {
        A[i] = reduce_mod(clmul(A[i], B[i]));
    }
    cp_algo::checkpoint("clmul+reduce");
    for(int i = 0; i < t; i++) {
        A[i] = poly_to_nim(A[i]);
    }
    cp_algo::checkpoint("to_nim");
    for(int i = 0; i < t; i++) {
        println(A[i]);
    }
    cp_algo::checkpoint("print");
    cp_algo::checkpoint<1>();
}
