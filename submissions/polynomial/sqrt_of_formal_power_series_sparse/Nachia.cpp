#define PROBLEM "https://judge.yosupo.jp/problem/sqrt_of_formal_power_series_sparse"
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

namespace nachia {

template <class Modint>
Modint SqrtModPrimeIfExist(Modint x){
    using uintmax = unsigned long long;
    if(x.val() == 0) return x;
    if(x.val() == 1) return -x;
    uintmax m = x.mod();
    uintmax q = m - 1;
    static Xoshiro256pp rng;
    int M = 0; while(q % 2 == 0){ q >>= 1; M++; }
    Modint c = 0;
    while(true){
        c = Modint(uintmax(rng()));
        if(c.val() == 0) continue;
        if(c.pow((m-1)/2).val() != 1) break;
    }
    c = c.pow(q);
    Modint t = x.pow(q);
    Modint R = x.pow((q+1)/2);
    while(t.val() != 1){
        int i = 1;
        auto tt = t*t;
        while(tt.val() != 1){ tt *= tt; i++; }
        auto b = c;
        for(int j=0; j<M-i-1; j++) b *= b;
        c = b*b;
        t *= c;
        R *= b;
        M = i;
    }
    return R;
}

} // namespace nachia
#include <utility>

namespace nachia{

template<class Modint>
class SparsePolynomialFp{
public:

    struct Term{
        int index;
        Modint coeff;
    };

    std::vector<Term> a;
    bool m_good;

private:

    void refine(){
        if(m_good) return;
        std::sort(a.begin(), a.end(), [](const Term& l, const Term& r){ return l.index < r.index; });
        int p = -1;
        for(size_t i=0; i<a.size(); i++){
            if(a[i].index < 0) continue;
            if(p < 0 || a[p].index != a[i].index) a[++p] = a[i];
            else a[p].coeff += a[i].coeff;
            if(a[p].coeff.val() == 0) p--;
        }
        a.resize(p+1);
        m_good = false;
    }
    
    static std::vector<Modint> InvTable(int n) {
        if(n == 0) return {};
        std::vector<Modint> res(n);
        res[0] = Modint::raw(1);
        for(int i=1; i<n; i++) res[i] = res[i-1] * Modint::raw(i);
        res[n-1] = res[n-1].inv();
        for(int i=n-1; i>=1; i--){
            Modint x = res[i];
            res[i] = x * res[i-1];
            res[i-1] = x * Modint::raw(i);
        }
        return res;
    }

    Term uniconst() {
        refine();
        int i = lowestIndex(-1);
        if(i < 0) return { -1, Modint(0) };
        Term res = a[0];
        Modint a0inv = res.coeff.inv();
        shift(-i); times(a0inv);
        return res;
    }

public:

    int lowestIndex(int whenEmpty = 1001001001){
        refine();
        return a.empty() ? whenEmpty : a.front().index;
    }
    void shift(int width){
        refine();
        for(auto& b : a) b.index += width;
    }
    void times(Modint t){
        if(t.val() == 0){ a.clear(); }
        for(auto& b : a) b.coeff *= t;
    }
    
    SparsePolynomialFp difference() const {
        SparsePolynomialFp res;
        res.a.reserve(a.size());
        for(auto& b : a){
            if(b.index == 0){ continue; }
            res.a.push_back({ b.index - 1, b.coeff * b.index });
        }
        return res;
    }

    std::vector<Modint> asVector(int n) const {
        std::vector<Modint> res(n);
        for(auto& term : a) if(term.index < n){
            res[term.index] += term.coeff;
        }
        return res;
    }
    
    void divInplace(std::vector<Modint>& target){
        auto b = *this; b.refine();
        assert(b.lowestIndex() == 0);
        int n = target.size();
        Modint invA0 = b.a.front().coeff.inv();
        for(auto& t : b.a) t.coeff *= invA0;
        for(auto& x : target) x *= invA0;
        for(int u=0; u<n; u++){
            for(int i=1; i<(int)b.a.size(); i++){
                auto& tm = b.a[i];
                if(u < tm.index) break;
                target[u] -= target[u - tm.index] * tm.coeff;
            }
        }
    }

    std::vector<Modint> exp(int n) const {
        assert(lowestIndex() > 0);
        refine();
        std::vector<Modint> res = InvTable(n);
        if(n == 0) return res;
        auto fp = difference();
        fp.shift(1);
        for(int i=1; i<n; i++){
            Modint buf;
            for(auto b : fp.a){
                if(i < b.index) break;
                buf += b.coeff * res[i - b.index];
            }
            res[i] *= buf;
        }
        return res;
    }

    std::vector<Modint> pow(int n, unsigned long long i) const {
        const Modint Zero = Modint(0);
        auto pown = Modint(i);
        SparsePolynomialFp f = *this;
        Term li = f.uniconst();
        if(i == 0){
            std::vector<Modint> res(n, Zero);
            if(n > 0) res[0] = Modint(1);
            return res;
        }
        if(n == 0 || li.index < 0 || (li.index > 0 && n / li.index <= i)){
            return std::vector<Modint>(n, Zero);
        }
        int k = f.a.size() - 1;
        std::vector<int> fi(k);
        std::vector<Modint> fc(k);
        std::vector<Modint> fdc(k);
        for(int i=0; i<k; i++){
            fi[i] = f.a[i+1].index;
            fc[i] = f.a[i+1].coeff;
            fdc[i] = fc[i] * Modint::raw(fi[i]);
        }
        int shp = (int)(li.index * i);
        std::vector<Modint> F = InvTable(n-shp);
        // f F' = n f' F
        F[0] = Modint(1);
        int t = 0;
        for(int i=1; i<n-shp; i++){
            Modint fd_F = Zero;
			if(t < k && fi[t] <= i) t++;
            for(int j=0; j<t; j++) fd_F += fdc[j] * F[i-fi[j]];
            Modint f_Fd = Zero;
            for(int j=0; j<t; j++) f_Fd += fc[j] * F[i-fi[j]] * Modint::raw(i-fi[j]);
            F[i] *= fd_F * pown - f_Fd;
        }
        F.resize(n, Zero);
        auto q = li.coeff.pow(i);
        for(int i=0; i<n-shp; i++) F[i] *= q;
        if(shp != 0){
            for(int i=n-shp-1; i>=0; i--) F[i+shp] = F[i];
            for(int i=0; i<shp; i++) F[i] = Zero;
        }
        return F;
    }
    
};

} // namespace nachia

namespace nachia{

// ax + by = gcd(a,b)
// return ( x, - )
std::pair<long long, long long> ExtGcd(long long a, long long b){
    long long x = 1, y = 0;
    while(b){
        long long u = a / b;
        std::swap(a-=b*u, b);
        std::swap(x-=y*u, y);
    }
    return std::make_pair(x, a);
}

} // namespace nachia

namespace nachia{

template<unsigned int MOD>
struct StaticModint{
private:
    using u64 = unsigned long long;
    unsigned int x;
public:

    using my_type = StaticModint;
    template< class Elem >
    static Elem safe_mod(Elem x){
        if(x < 0){
            if(0 <= x+MOD) return x + MOD;
            return MOD - ((-(x+MOD)-1) % MOD + 1);
        }
        return x % MOD;
    }

    StaticModint() : x(0){}
    StaticModint(const my_type& a) : x(a.x){}
    StaticModint& operator=(const my_type&) = default;
    template< class Elem >
    StaticModint(Elem v) : x(safe_mod(v)){}
    unsigned int operator*() const noexcept { return x; }
    my_type& operator+=(const my_type& r) noexcept { auto t = x + r.x; if(t >= MOD) t -= MOD; x = t; return *this; }
    my_type operator+(const my_type& r) const noexcept { my_type res = *this; return res += r; }
    my_type& operator-=(const my_type& r) noexcept { auto t = x + MOD - r.x; if(t >= MOD) t -= MOD; x = t; return *this; }
    my_type operator-(const my_type& r) const noexcept { my_type res = *this; return res -= r; }
    my_type operator-() const noexcept { my_type res = *this; res.x = ((res.x == 0) ? 0 : (MOD - res.x)); return res; }
    my_type& operator*=(const my_type& r)noexcept { x = (u64)x * r.x % MOD; return *this; }
    my_type operator*(const my_type& r) const noexcept { my_type res = *this; return res *= r; }
    my_type pow(unsigned long long i) const noexcept {
        my_type a = *this, res = 1;
        while(i){ if(i & 1){ res *= a; } a *= a; i >>= 1; }
        return res;
    }
    my_type inv() const { return my_type(ExtGcd(x, MOD).first); }
    unsigned int val() const noexcept { return x; }
    static constexpr unsigned int mod() { return MOD; }
    static my_type raw(unsigned int val) noexcept { auto res = my_type(); res.x = val; return res; }
    my_type& operator/=(const my_type& r){ return operator*=(r.inv()); }
    my_type operator/(const my_type& r) const { return operator*(r.inv()); }
};

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

void solve(){
    using nachia::cin;
    using nachia::cout;
    using Modint = nachia::StaticModint<998244353>;
    using SparsePoly = nachia::SparsePolynomialFp<Modint>;

    int n; cin >> n;
    int k; cin >> k;
    SparsePoly poly;
    for(int i=0; i<k; i++){
        int x, a; cin >> x >> a;
        poly.a.push_back({ x, Modint::raw(a) });
    }
    int w = poly.lowestIndex(n);
    std::vector<Modint> ans(n);
    if(w != n){
        if(w % 2 == 1){ cout << "-1\n"; return; }
        poly.shift(-w);
        Modint a0;
        for(auto x : poly.a) if(x.index == 0) a0 = x.coeff;
        if(a0.pow(a0.mod() / 2).val() != 1){ cout << "-1\n"; return; }
        auto ra0 = nachia::SqrtModPrimeIfExist(a0);
        poly.times(a0.inv());
        w /= 2;
        auto tmp = poly.pow(n-w*2, (a0.mod() + 1) / 2);
        for(int i=0; i<n-w*2; i++) ans[i+w] = tmp[i] * ra0;
    }
    for(int i=0; i<n; i++){
        if(i) cout << ' ';
        cout << ans[i].val();
    } cout << '\n';
}

int main(){
    solve();
    return 0;
}
