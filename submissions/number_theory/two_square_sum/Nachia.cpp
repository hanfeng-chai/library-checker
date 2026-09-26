#define PROBLEM "https://judge.yosupo.jp/problem/two_square_sum"
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cassert>
#include <cstdint>
#include <array>


namespace nachia{

class Xoshiro256pp{
public:

    using i32 = int32_t;
    using u32 = uint32_t;
    using i64 = int64_t;
    using u64 = uint64_t;


private:
    std::array<u64, 4> s;

    // https://prng.di.unimi.it/xoshiro256plusplus.c
    static inline uint64_t rotl(const uint64_t x, int k) noexcept {
        return (x << k) | (x >> (64 - k));
    }
    inline uint64_t gen(void) noexcept {
        const uint64_t result = rotl(s[0] + s[3], 23) + s[0];
        const uint64_t t = s[1] << 17;
        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];
        s[2] ^= t;
        s[3] = rotl(s[3], 45);
        return result;
    }

    // https://xoshiro.di.unimi.it/splitmix64.c
    u64 splitmix64(u64& x) {
        u64 z = (x += 0x9e3779b97f4a7c15);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    }

public:

    void seed(u64 x = 7001){
        assert(x != 0);
        s[0] = x;
        for(int i=1; i<4; i++) s[i] = splitmix64(x);
    }
    
    std::array<u64, 4> getState() const { return s; }
    void setState(std::array<u64, 4> a){ s = a; }

    Xoshiro256pp(){ seed(); }
    
    u64 rng64() { return gen(); }
    u64 operator()(){ return gen(); }

    // generate x : l <= x <= r
    u64 random_unsigned(u64 l,u64 r){
        assert(l<=r);
        r-=l;
        auto res = rng64();
        if(res<=r) return res+l;
        u64 d = r+1;
        u64 max_valid = 0xffffffffffffffff/d*d;
        while(true){
            auto res = rng64();
            if(res<=max_valid) break;
        }
        return res%d+l;
    }

    // generate x : l <= x <= r
    i64 random_signed(i64 l,i64 r){
        assert(l<=r);
        u64 unsigned_l = (u64)l ^ (1ull<<63);
        u64 unsigned_r = (u64)r ^ (1ull<<63);
        u64 unsigned_res = random_unsigned(unsigned_l,unsigned_r) ^ (1ull<<63);
        return (i64)unsigned_res;
    }


    // permute x : n_left <= x <= n_right
    // output r from the front
    template<class Int>
    std::vector<Int> random_nPr(Int n_left, Int n_right, Int r){
        Int n = n_right-n_left;

        assert(n>=0);
        assert(r<=(1ll<<27));
        if(r==0) return {};  
        assert(n>=r-1);

        std::vector<Int> V;
        std::unordered_map<Int,Int> G;
        for(int i=0; i<r; i++){
            Int p = random_signed(i,n);
            Int x = p - G[p];
            V.push_back(x);
            G[p] = p - (i - G[i]);
        }

        for(Int& v : V) v+=n_left;
        return V;
    }



    // V[i] := V[perm[i]]
    // using swap
    template<class E,class PermInt_t>
    void permute_inplace(std::vector<E>& V,std::vector<PermInt_t> perm){
        assert(V.size() == perm.size());
        int N=V.size();
        for(int i=0; i<N; i++){
            int p=i;
            while(perm[p]!=i){
                assert(0 <= perm[p] && perm[p] < N);
                assert(perm[p] != perm[perm[p]]);
                std::swap(V[p],V[perm[p]]);
                int pbuf = perm[p]; perm[p] = p; p = pbuf;
            }
            perm[p] = p;
        }
    }

    template<class E>
    std::vector<E> shuffle(const std::vector<E>& V){
        int N=V.size();
        auto P = random_nPr(0,N-1,N);
        std::vector<E> res;
        res.reserve(N);
        for(int i=0; i<N; i++) res.push_back(V[P[i]]);
        return res;
    }

    // shuffle using swap
    template<class E>
    void shuffle_inplace(std::vector<E>& V){
        int N=V.size();
        permute_inplace(V,random_nPr(0,N-1,N));
    }


};

} // namespace nachia
#include <initializer_list>

namespace nachia{

bool IsPrime(unsigned long long x) noexcept {
    if(x <= 1) return false;
    if(x % 2 == 0) return x == 2;
    using u64 = unsigned long long;
    using u128 = __uint128_t;
    u64 d = x-1;
    int s = 0;
    int q = 63;
    while(!(d&1)){ d >>= 1; s++; }
    while(!(d >> q)) q--;
    u64 r = x; for(int t=0; t<6; t++) r*=2-r*x;
    u128 n2 = -(u128)x % x;
    auto red = [=](u128 t) noexcept -> u64 {
        t = (t + (u128)((u64)t*-r)*x) >> 64;
        return (t >= x) ? t-x : t;
    };
    u64 one = red(n2);
    for(u64 base : { 2, 325, 9375, 28178, 450775, 9780504, 1795265022 }){
        if(base%x==0) continue;
        u64 a = base = red(base%x*n2);
        for(int e=q-1; e>=0; e--){ a = red((u128)a*a); if((d>>e)&1) a = red((u128)a*base); }
        if(a == one) continue;
        for(int t=1; t<s&&a!=x-one; t++) a = red((u128)a*a);
        if(a != x-one) return false;
    }
    return true;
}

} // namespace nachia

namespace nachia{

int Popcount(unsigned long long c) noexcept {
#ifdef __GNUC__
    return __builtin_popcountll(c);
#else
    c = (c & (~0ull/3)) + ((c >> 1) & (~0ull/3));
    c = (c & (~0ull/5)) + ((c >> 2) & (~0ull/5));
    c = (c & (~0ull/17)) + ((c >> 4) & (~0ull/17));
    c = (c * (~0ull/257)) >> 56;
    return c;
#endif
}

// please ensure x != 0
int MsbIndex(unsigned long long x) noexcept {
#ifdef __GNUC__
    return 63 - __builtin_clzll(x);
#else
    using u64 = unsigned long long;
    int q = (x >> 32) ? 32 : 0;
    auto m = x >> q;
    constexpr u64 hi = 0x8888'8888;
    constexpr u64 mi = 0x1111'1111;
    m = (((m | ~(hi - (m & ~hi))) & hi) * mi) >> 35;
    m = (((m | ~(hi - (x & ~hi))) & hi) * mi) >> 31;
    q += (m & 0xf) << 2;
    q += 0x3333'3333'2222'1100 >> (((x >> q) & 0xf) << 2) & 0xf;
    return q;
#endif
}

// please ensure x != 0
int LsbIndex(unsigned long long x) noexcept {
#ifdef __GNUC__
    return __builtin_ctzll(x);
#else
    return MsbIndex(x & -x);
#endif
}

}

#include <utility>

namespace nachia{

std::vector<std::pair<unsigned long long, int>> Factorize(unsigned long long x){
    if(x == 1) return {};
    if(IsPrime(x)) return {{x,1}};
    using u64 = unsigned long long;
    using u128 = __uint128_t;
    u64 X = x;
    std::vector<u64> p;
    for(u64 i=2; i<100; i+=1+i%2) if(x%i==0){ p.push_back(i); while(x%i==0) x/=i; }
    u64 r=1; u128 n2=1;
    auto updX = [&](){
        r = x; for(int t=0; t<6; t++) r*=2-r*x;
        n2 = -(u128)x % x;
    };
    auto red = [&](u128 t) noexcept -> u64 {
        u64 s = ((u128)x*((u64)t*r)) >> 64;
        u64 t2 = t >> 64;
        return t2-s + (t2 < s ? x : 0);
    };
    auto mult = [&](u64 a, u64 b) noexcept { return red((u128)red((u128)a*n2)*b); };
    auto gcd = [](u64 a, u64 b) noexcept {
        if(!a || !b) return a|b;
        int q = LsbIndex(a|b);
        b >>= LsbIndex(b);
        a >>= LsbIndex(a);
        while(a!=b){
            if(a<b){ b-=a; b>>=LsbIndex(b); }
            else{ a-=b; a>>=LsbIndex(a); }
        }
        return a<<q;
    };
    static u64 v = 7001;
    p.push_back(x);
    for(int pi=p.size()-1; pi<(int)p.size(); pi++) while(p[pi] != 1 && !IsPrime(p[pi])){
        x = p[pi]; updX();
        while(p[pi] == x){
            v^=v<<13; v^=v>>7; v^=v<<17; // Xorshift https://www.jstatsoft.org/article/download/v008i14/916
            u64 c = red(v); if(c == 0) continue;
            auto f = [=](u64 a) noexcept -> u64 { return red((u128)a*a+c); };
            u64 a=0, b=f(a);
            u64 buf = 1, sz = 1, nx = 10;
            while(true){
                while(nx != sz && a != b){
                    buf = mult(buf, a<=b?b-a:a-b); sz++;
                    a = f(a); b = f(f(b));
                }
                u64 g = gcd(buf, x);
                if(g != 1){
                    while(p[pi] % g == 0) p[pi] /= g;
                    p.push_back(g);
                    break;
                }
                if(a == b) break;
                nx = sz * 3 / 2;
            }
        }
    }
    std::vector<std::pair<u64, int>> res;
    for(u64 q : p) if(q != 1){
        int e=0; while(X%q == 0){ e++; X/=q; }
        if(e) res.push_back({ q, e });
    }
    return res;
}

unsigned long long Totient(unsigned long long x){
    auto F = Factorize(x);
    for(auto f : F) x -= x / f.first;
    return x;
}

} // namespace nachia

namespace nachia {

std::pair<unsigned long long, unsigned long long>
    TwoSquareSumFactorization_4Nplus1Prime64(unsigned long long x)
{
    static Xoshiro256pp rng;
    using u64 = unsigned long long;
    using i64 = long long;
    using u128 = __uint128_t;
    using i128 = __int128_t;
    auto powmod = [x](u64 f, u64 i) -> u64 {
        u64 a = 1;
        while(i){
            if(i%2 == 1) a = u128(a) * f % x;
            f = u128(f) * f % x;
            i /= 2;
        }
        return a;
    };
    i64 a = 0;
    while(true){
        u64 p = powmod(rng()%x, (x-1)/4);
        if(p == 0 || p == 1 || p == x-1) continue;
        if(x/2 <= p) p = x - p;
        a = p;
        break;
    }
    i64 b = 1;
    i64 k = (i128(a)*a + i128(b)*b) / x;
    while(k > 1){
        i64 p = a % k;
        i64 q = b % k;
        if(p >= k/2) p -= k;
        if(q >= k/2) q -= k;
        i64 f = (i128(a) * p + i128(b) * q) / k;
        i64 g = (i128(a) * q - i128(b) * p) / k;
        a = f < 0 ? -f : f;
        b = g < 0 ? -g : g;
        k = (i128(a)*a + i128(b)*b) / x;
    }
    if(a > b) std::swap(a, b);
    return std::make_pair(a, b);
}

std::vector<std::pair<unsigned long long, unsigned long long>>
    TwoSquareSumFactorization(unsigned long long x)
{
    using u64 = unsigned long long;
    if(x == 0) return {{0,0}};
    auto multigi = [](u64& a, u64& b, u64 c, u64 d){
        u64 u = a * c;
        u64 v = b * d;
        if(u < v){
            a = a * d + b * c;
            b = v - u;
        } else {
            b = a * d + b * c;
            a = u - v;
        }
    };
    std::vector<std::vector<std::pair<u64, u64>>> buf;
    auto fp = Factorize(x);
    unsigned long long m = 1;
    for(auto [p,e] : fp){
        if(p == 2){
            if(e % 2 == 0) buf.push_back({ { u64(1) << (e/2), u64(0) } });
            else buf.push_back({ { u64(1) << (e/2), u64(1) << (e/2) } });
        } else if(p % 4 != 1){
            if(e % 2 == 1) return {};
            u64 n = 1;
            for(int i=0; i<e/2; i++) n *= p;
            buf.push_back({ { n, 0 } });
        } else {
            auto [v,u] = TwoSquareSumFactorization_4Nplus1Prime64(p);
            std::vector<u64> powp(e/2+1);
            powp[0] = 1;
            for(int j=0; j*2+2<=e; j++) powp[j+1] = powp[j] * p;
            u64 uu = 1;
            u64 vv = 0;
            if(e%2 == 1){ uu = u; vv = v; }
            std::vector<std::pair<u64, u64>> buf2;
            buf2.push_back({ uu * powp[e/2], vv * powp[e/2] });
            if(e >= 2){
                u64 u2 = u * u - v * v;
                u64 v2 = u * v * 2;
                for(int t=e/2-1; t>=0; t--){
                    multigi(uu, vv, u2, v2);
                    buf2.push_back({ uu * powp[t], vv * powp[t] });
                }
            }
            buf.push_back(std::move(buf2));
        }
    }
    std::vector<std::pair<u64, u64>> ans;
    ans.push_back({ 1, 0 });
    for(auto& se : buf){
        int p = int(ans.size());
        int q = int(se.size());
        std::vector<std::pair<u64, u64>> nx;
        for(int i=0; i<p; i++) for(int j=0; j<q; j++){
            auto [u,v] = ans[i];
            auto [a,b] = se[j];
            u64 u2 = u;
            u64 v2 = v;
            multigi(u2, v2, a, b);
            nx.push_back({ u2,v2 }); 
            if(u != 0 && v != 0 && a != 0 && b != 0 && u != v && a != b){
                u2 = u;
                v2 = v;
                multigi(u2, v2, b, a);
                nx.push_back({ u2,v2 }); 
            }
        }
        std::swap(ans, nx);
    }
    for(auto& [u,v] : ans) if(u > v) std::swap(u, v);
    return ans;
}

} // namespace nachia
#include <cstdio>
#include <cctype>
#include <string>

namespace nachia{

struct CInStream{
private:
	static const unsigned int INPUT_BUF_SIZE = 1 << 17;
	unsigned int p = INPUT_BUF_SIZE;
	static char Q[INPUT_BUF_SIZE];
public:
	using MyType = CInStream;
	char seekChar(){
		if(p == INPUT_BUF_SIZE){
			size_t len = fread(Q, 1, INPUT_BUF_SIZE, stdin);
			if(len != INPUT_BUF_SIZE) Q[len] = '\0';
			p = 0;
		}
		return Q[p];
	}
	void skipSpace(){ while(isspace(seekChar())) p++; }
private:
	template<class T, int sp = 1>
	T nextUInt(){
		if constexpr (sp) skipSpace();
		T buf = 0;
		while(true){
			char tmp = seekChar();
			if('9' < tmp || tmp < '0') break;
			buf = buf * 10 + (tmp - '0');
			p++;
		}
		return buf;
	}
public:
	uint32_t nextU32(){ return nextUInt<uint32_t>(); }
	int32_t nextI32(){
		skipSpace();
		if(seekChar() == '-'){
			p++; return (int32_t)(-nextUInt<uint32_t, 0>());
		}
		return (int32_t)nextUInt<uint32_t, 0>();
	}
	uint64_t nextU64(){ return nextUInt<uint64_t>();}
	int64_t nextI64(){
		skipSpace();
		if(seekChar() == '-'){
			p++; return (int64_t)(-nextUInt<int64_t, 0>());
		}
		return (int64_t)nextUInt<int64_t, 0>();
	}
	template<class T>
	T nextInt(){
		skipSpace();
		if(seekChar() == '-'){
			p++;
			return - nextUInt<T, 0>();
		}
		return nextUInt<T, 0>();
	}
	char nextChar(){ skipSpace(); char buf = seekChar(); p++; return buf; }
	std::string nextToken(){
		skipSpace();
		std::string buf;
		while(true){
			char ch = seekChar();
			if(isspace(ch) || ch == '\0') break;
			buf.push_back(ch);
			p++;
		}
		return buf;
	}
	MyType& operator>>(unsigned int& dest){ dest = nextU32(); return *this; }
	MyType& operator>>(int& dest){ dest = nextI32(); return *this; }
	MyType& operator>>(unsigned long& dest){ dest = nextU64(); return *this; }
	MyType& operator>>(long& dest){ dest = nextI64(); return *this; }
	MyType& operator>>(unsigned long long& dest){ dest = nextU64(); return *this; }
	MyType& operator>>(long long& dest){ dest = nextI64(); return *this; }
	MyType& operator>>(std::string& dest){ dest = nextToken(); return *this; }
	MyType& operator>>(char& dest){ dest = nextChar(); return *this; }
} cin;

struct FastOutputTable{
	char LZ[1000][4] = {};
	char NLZ[1000][4] = {};
	constexpr FastOutputTable(){
		using u32 = uint_fast32_t;
		for(u32 d=0; d<1000; d++){
			LZ[d][0] = ('0' + d / 100 % 10);
			LZ[d][1] = ('0' + d /  10 % 10);
			LZ[d][2] = ('0' + d /   1 % 10);
			LZ[d][3] = '\0';
		}
		for(u32 d=0; d<1000; d++){
			u32 i = 0;
			if(d >= 100) NLZ[d][i++] = ('0' + d / 100 % 10);
			if(d >=  10) NLZ[d][i++] = ('0' + d /  10 % 10);
			if(d >=   1) NLZ[d][i++] = ('0' + d /   1 % 10);
			NLZ[d][i++] = '\0';
		}
	}
};

struct COutStream{
private:
	using u32 = uint32_t;
	using u64 = uint64_t;
	using MyType = COutStream;
	static const u32 OUTPUT_BUF_SIZE = 1 << 17;
	static char Q[OUTPUT_BUF_SIZE];
	static constexpr FastOutputTable TB = FastOutputTable();
	u32 p = 0;
	static constexpr u32 P10(u32 d){ return d ? P10(d-1)*10 : 1; }
	static constexpr u64 P10L(u32 d){ return d ? P10L(d-1)*10 : 1; }
	template<class T, class U> static void Fil(T& m, U& l, U x){ m = l/x; l -= m*x; }
public:
	void next_dig9(u32 x){
		u32 y;
		Fil(y, x, P10(6));
		nextCstr(TB.LZ[y]);
		Fil(y, x, P10(3));
		nextCstr(TB.LZ[y]); nextCstr(TB.LZ[x]);
	}
	void nextChar(char c){
		Q[p++] = c;
		if(p == OUTPUT_BUF_SIZE){ fwrite(Q, p, 1, stdout); p = 0; }
	}
	void nextEoln(){ nextChar('\n'); }
	void nextCstr(const char* s){ while(*s) nextChar(*(s++)); }
	void nextU32(uint32_t x){
		u32 y = 0;
		if(x >= P10(9)){
			Fil(y, x, P10(9));
			nextCstr(TB.NLZ[y]); next_dig9(x);
		}
		else if(x >= P10(6)){
			Fil(y, x, P10(6));
			nextCstr(TB.NLZ[y]);
			Fil(y, x, P10(3));
			nextCstr(TB.LZ[y]); nextCstr(TB.LZ[x]);
		}
		else if(x >= P10(3)){
			Fil(y, x, P10(3));
			nextCstr(TB.NLZ[y]); nextCstr(TB.LZ[x]);
		}
		else if(x >= 1) nextCstr(TB.NLZ[x]);
		else nextChar('0');
	}
	void nextI32(int32_t x){
		if(x >= 0) nextU32(x);
		else{ nextChar('-'); nextU32((u32)-x); }
	}
	void nextU64(uint64_t x){
		u32 y = 0;
		if(x >= P10L(18)){
			Fil(y, x, P10L(18));
			nextU32(y);
			Fil(y, x, P10L(9));
			next_dig9(y); next_dig9(x);
		}
		else if(x >= P10L(9)){
			Fil(y, x, P10L(9));
			nextU32(y); next_dig9(x);
		}
		else nextU32(x);
	}
	void nextI64(int64_t x){
		if(x >= 0) nextU64(x);
		else{ nextChar('-'); nextU64((u64)-x); }
	}
	template<class T>
	void nextInt(T x){
		if(x < 0){ nextChar('-'); x = -x; }
		if(!(0 < x)){ nextChar('0'); return; }
		std::string buf;
		while(0 < x){
			buf.push_back('0' + (int)(x % 10));
			x /= 10;
		}
		for(int i=(int)buf.size()-1; i>=0; i--){
			nextChar(buf[i]);
		}
	}
	void writeToFile(bool flush = false){
		fwrite(Q, p, 1, stdout);
		if(flush) fflush(stdout);
		p = 0;
	}
	COutStream(){ Q[0] = 0; }
	~COutStream(){ writeToFile(); }
	MyType& operator<<(unsigned int tg){ nextU32(tg); return *this; }
	MyType& operator<<(unsigned long tg){ nextU64(tg); return *this; }
	MyType& operator<<(unsigned long long tg){ nextU64(tg); return *this; }
	MyType& operator<<(int tg){ nextI32(tg); return *this; }
	MyType& operator<<(long tg){ nextI64(tg); return *this; }
	MyType& operator<<(long long tg){ nextI64(tg); return *this; }
	MyType& operator<<(const std::string& tg){ nextCstr(tg.c_str()); return *this; }
	MyType& operator<<(const char* tg){ nextCstr(tg); return *this; }
	MyType& operator<<(char tg){ nextChar(tg); return *this; }
} cout;

char CInStream::Q[INPUT_BUF_SIZE];
char COutStream::Q[OUTPUT_BUF_SIZE];

} // namespace nachia

int main(){
    using nachia::cin;
    using nachia::cout;
    using u64 = unsigned long long;
    int Q; cin >> Q;
    for(int qi=0; qi<Q; qi++){
        u64 n; cin >> n;
        auto ans = nachia::TwoSquareSumFactorization(n);
        int cnt = 0;
        for(auto [u,v] : ans) cnt += u == v ? 1 : 2;
        cout << cnt << '\n';
        for(auto [u,v] : ans){
            cout << u << ' ' << v << '\n';
            if(u != v) cout << v << ' ' << u << '\n';
        }
    }
    return 0;
}
