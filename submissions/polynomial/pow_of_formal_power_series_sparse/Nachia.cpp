
#include <cstdio>
#include <cctype>
#include <cstdint>
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
	void next_dig9(u32 x){
		u32 y;
		Fil(y, x, P10(6));
		nextCstr(TB.LZ[y]);
		Fil(y, x, P10(3));
		nextCstr(TB.LZ[y]); nextCstr(TB.LZ[x]);
	}
public:
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

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <utility>

namespace nachia{

template<unsigned int MOD>
struct PrimitiveRoot{
    using u64 = unsigned long long;
    static constexpr u64 powm(u64 a, u64 i) {
        u64 res = 1, aa = a;
        while(i){
            if(i & 1) res = res * aa % MOD;
            aa = aa * aa % MOD;
            i /= 2;
        }
        return res;
    }
    static constexpr bool ExamineVal(unsigned int g){
        unsigned int t = MOD - 1;
        for(u64 d=2; d*d<=t; d++) if(t % d == 0){
            if(powm(g, (MOD - 1) / d) == 1) return false;
            while(t % d == 0) t /= d;
        }
        if(t != 1) if(powm(g, (MOD - 1) / t) == 1) return false;
        return true;
    }
    static constexpr unsigned int GetVal(){
        for(unsigned int x=2; x<MOD; x++) if(ExamineVal(x)) return x;
        return 0;
    }
    static const unsigned int val = GetVal();
};

} // namespace nachia

namespace nachia{

template<class Modint>
class Comb{
private:
    std::vector<Modint> F;
    std::vector<Modint> iF;
public:
    void extend(int newN){
        int prevN = (int)F.size() - 1;
        if(prevN >= newN) return;
        F.resize(newN+1);
        iF.resize(newN+1);
        for(int i=prevN+1; i<=newN; i++) F[i] = F[i-1] * Modint::raw(i);
        iF[newN] = F[newN].inv();
        for(int i=newN; i>prevN; i--) iF[i-1] = iF[i] * Modint::raw(i);
    }
    Comb(int n = 1){
        F.assign(2, Modint(1));
        iF.assign(2, Modint(1));
        extend(n);
    }
    Modint factorial(int n) const { return F[n]; }
    Modint invFactorial(int n) const { return iF[n]; }
    Modint invOf(int n) const { return iF[n] * F[n-1]; }
    Modint comb(int n, int r) const {
        if(n < 0 || n < r || r < 0) return Modint(0);
        return F[n] * iF[r] * iF[n-r];
    }
    Modint invComb(int n, int r) const {
        if(n < 0 || n < r || r < 0) return Modint(0);
        return iF[n] * F[r] * F[n-r];
    }
    Modint perm(int n, int r) const {
        if(n < 0 || n < r || r < 0) return Modint(0);
        return F[n] * iF[n-r];
    }
    Modint invPerm(int n, int r) const {
        if(n < 0 || n < r || r < 0) return Modint(0);
        return iF[n] * F[n-r];
    }
    Modint operator()(int n, int r) const { return comb(n,r); }
};

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


namespace nachia {

template<class mint>
struct NttInterface{

template<class Iter>
void Butterfly(Iter, int) const {}

template<class Iter>
void IButterfly(Iter, int) const {}

template<class Iter>
void BitReversal(Iter a, int N) const {
    for(int i=0, j=0; j<N; j++){
        if(i < j) std::swap(a[i], a[j]);
        for(int k = N>>1; k > (i^=k); k>>=1);
    }
}

};

} // namespace nachia
#include <iterator>
#include <array>

namespace nachia{
    
constexpr int bsf_constexpr(unsigned int n) {
    int x = 0;
    while (!(n & (1 << x))) x++;
    return x;
}

template <class mint>
struct NttFromAcl : NttInterface<mint> {

using u32 = unsigned int;
using u64 = unsigned long long;
    
static int ceil_pow2(int n) {
    int x = 0;
    while ((1U << x) < (u32)(n)) x++;
    return x;
}

struct fft_info {
    static constexpr u32 g = nachia::PrimitiveRoot<mint::mod()>::val;
    static constexpr int rank2 = bsf_constexpr(mint::mod()-1);
    std::array<mint, rank2+1> root;
    std::array<mint, rank2+1> iroot;

    std::array<mint, std::max(0, rank2-1)> rate2;
    std::array<mint, std::max(0, rank2-1)> irate2;

    std::array<mint, std::max(0, rank2-2)> rate3;
    std::array<mint, std::max(0, rank2-2)> irate3;

    fft_info(){
        root[rank2] = mint(g).pow((mint::mod() - 1) >> rank2);
        iroot[rank2] = root[rank2].inv();
        for(int i=rank2-1; i>=0; i--){
            root[i] = root[i+1] * root[i+1];
            iroot[i] = iroot[i+1] * iroot[i+1];
        }
        mint prod = 1, iprod = 1;
        for(int i=0; i<=rank2-2; i++){
            rate2[i] = root[i+2] * prod;
            irate2[i] = iroot[i+2] * iprod;
            prod *= iroot[i+2];
            iprod *= root[i+2];
        }
        prod = 1; iprod = 1;
        for(int i=0; i<=rank2-3; i++){
            rate3[i] = root[i+3] * prod;
            irate3[i] = iroot[i+3] * iprod;
            prod *= iroot[i+3];
            iprod *= root[i+3];
        }
    }
};

template<class RandomAccessIterator>
void Butterfly(RandomAccessIterator a, int n) const {
    int h = ceil_pow2(n);

    static const fft_info info;

    int len = 0;
    while(len < h){
        if(h-len == 1){
            int p = 1 << (h-len-1);
            mint rot = 1;
            for(int s=0; s<(1<<len); s++){
                int offset = s << (h-len);
                for(int i=0; i<p; i++){
                    auto l = a[i+offset];
                    auto r = a[i+offset+p] * rot;
                    a[i+offset] = l+r;
                    a[i+offset+p] = l-r;
                }
                if(s+1 != (1<<len)) rot *= info.rate2[LsbIndex(~(u32)(s))];
            }
            len++;
        } else {
            int p = 1 << (h-len-2);
            mint rot = 1, imag = info.root[2];
            for(int s=0; s<(1<<len); s++){
                mint rot2 = rot * rot;
                mint rot3 = rot2 * rot;
                int offset = s << (h-len);
                for(int i=0; i<p; i++){
                    auto mod2 = 1ULL * mint::mod() * mint::mod();
                    auto a0 = 1ULL * a[i+offset].val();
                    auto a1 = 1ULL * a[i+offset+p].val() * rot.val();
                    auto a2 = 1ULL * a[i+offset+2*p].val() * rot2.val();
                    auto a3 = 1ULL * a[i+offset+3*p].val() * rot3.val();
                    auto a1na3imag = 1ULL * mint(a1 + mod2 - a3).val() * imag.val();
                    auto na2 = mod2 - a2;
                    a[i+offset] = a0 + a2 + a1 + a3;
                    a[i+offset+1*p] = a0 + a2 + (2 * mod2 - (a1 + a3));
                    a[i+offset+2*p] = a0 + na2 + a1na3imag;
                    a[i+offset+3*p] = a0 + na2 + (mod2 - a1na3imag);
                }
                if(s+1 != (1<<len)) rot *= info.rate3[LsbIndex(~(u32)(s))];
            }
            len += 2;
        }
    }
}

template<class RandomAccessIterator>
void IButterfly(RandomAccessIterator a, int n) const {
    int h = ceil_pow2(n);

    static const fft_info info;
    constexpr int MOD = mint::mod();

    int len = h;
    while(len){
        if(len == 1){
            int p = 1 << (h-len);
            mint irot = 1;
            for(int s=0; s<(1<<(len-1)); s++){
                int offset = s << (h-len+1);
                for(int i=0; i<p; i++){
                    auto l = a[i+offset];
                    auto r = a[i+offset+p];
                    a[i+offset] = l+r;
                    a[i+offset+p] = (u64)(MOD + l.val() - r.val()) * irot.val();
                }
                if(s+1 != (1<<(len-1))) irot *= info.irate2[LsbIndex(~(u32)(s))];
            }
            len--;
        } else {
            int p = 1 << (h-len);
            mint irot = 1, iimag = info.iroot[2];
            for(int s=0; s<(1<<(len-2)); s++){
                mint irot2 = irot * irot;
                mint irot3 = irot2 * irot;
                int offset = s << (h-len+2);
                for(int i=0; i<p; i++){
                    auto a0 = 1ULL * a[i+offset+0*p].val();
                    auto a1 = 1ULL * a[i+offset+1*p].val();
                    auto a2 = 1ULL * a[i+offset+2*p].val();
                    auto a3 = 1ULL * a[i+offset+3*p].val();

                    auto a2na3iimag = 1ULL * mint((MOD + a2 - a3) * iimag.val()).val();

                    a[i+offset] = a0 + a1 + a2 + a3;
                    a[i+offset+1*p] = (a0 + (MOD - a1) + a2na3iimag) * irot.val();
                    a[i+offset+2*p] = (a0 + a1 + (MOD - a2) + (MOD - a3)) * irot2.val();
                    a[i+offset+3*p] = (a0 + (MOD - a1) + (MOD - a2na3iimag)) * irot3.val();
                }
                if(s+1 != (1<<(len-2))) irot *= info.irate3[LsbIndex(~(u32)(s))];
            }
            len -= 2;
        }
    }
}

};

} // namespace nachia

namespace nachia {

template<class Elem, class NttInst = NttFromAcl<Elem>>
struct FpsNtt {
public:
    using Fps = FpsNtt;
    using ElemTy = Elem;
    static constexpr unsigned int MOD = Elem::mod();
    static constexpr int CONV_THRES = 30;
    static const NttInst nttInst;
    static const unsigned int zeta = nachia::PrimitiveRoot<MOD>::GetVal();
private:
    using u32 = unsigned int;
    static Elem ZeroElem() noexcept { return Elem(0); }
    static Elem OneElem() noexcept { return Elem(1); }
    static Comb<Elem> comb;
    std::vector<Elem> a;
    int RSZ(int& sz) const { return sz = (sz < 0 ? size() : sz); }
public:

    int size() const noexcept { return a.size(); }
    Elem& operator[](int x) noexcept { return a[x]; }
    const Elem& operator[](int x) const noexcept { return a[x]; }
    Elem getCoeff(int x) const noexcept { return (0 <= x && x < size()) ? a[x] : ZeroElem(); }
    static Comb<Elem>& GetComb() { return comb; }
    static int BestNttSize(int x) noexcept { assert(x); return 1 << MsbIndex(x*2-1); }
    Fps move(){ return std::move(*this); }
    Fps& set(int i, Elem c){ a[i] = c; return *this; }

    Fps& removeLeadingZeros(){
        int newsz = size();
        while(newsz && a[newsz-1].val() == 0) newsz--;
        a.resize(newsz);
        if((int)a.capacity() / 4 > newsz) a.shrink_to_fit();
        return *this;
    }

    FpsNtt(){}
    FpsNtt(int sz) : a(sz, ZeroElem()) {}
    FpsNtt(int sz, Elem e) : a(sz, e) {}
    FpsNtt(std::vector<Elem>&& src) : a(std::move(src)) {}
    FpsNtt(const std::vector<Elem>& src) : a(src) {}
    
    Fps& ntt() {
        capSize(BestNttSize(size()));
        nttInst.Butterfly(a.begin(), size());
        return *this;
    }
    Fps& intt() {
        nttInst.IButterfly(a.begin(), a.size());
        return times(Elem::raw(size()).inv());
    }
    Fps nttDouble(Fps vanilla) const {
        int n = size();
        assert(n == (n&-n)); // n is a power of 2
        Elem q = Elem::raw(zeta).pow((Elem::mod() - 1) / (n*2));
        Elem qq = OneElem();
        for(int i=0; i<n; i++){ vanilla[i] *= qq; qq *= q; }
        vanilla.ntt();
        Fps res = clip(0, n*2);
        for(int i=0; i<n; i++) res[n+i] = vanilla[i];
        return res;
    }
    Fps nttDouble() const { return nttDouble(clip().intt().move()); }

    // Fps res(resSz);
    // for(int j=0; j<resSz-destL && j+srcL < srcR; j++) res[j+destL] = a.getCoeff(j+srcL)
    // if srcR is unspecified -> srcR = max(srcL, size());
    // if resSz is unspecified -> resSz = destL + srcR - srcL
    Fps clip(int srcL, int srcR = -1, int destL = 0, int resSz = -1) const {
        srcR = RSZ(srcR);
        if(resSz < 0) resSz = destL + srcR - srcL;
        int rj = std::min(std::min(srcR, size()) - srcL, resSz - destL);
        Fps res(resSz);
        for(int j=std::max(0, -srcL); j<rj; j++) res[j+destL] = a[j+srcL];
        return res;
    }
    Fps clip() const { return *this; }

    Fps& capSize(int l, int r) {
        if(r <= (int)size()) a.resize(r);
        if(size() <= l) a.resize(l, ZeroElem());
        return *this;
    }
    Fps& capSize(int z){ a.resize(RSZ(z), ZeroElem()); return *this; }
    Fps& times(Elem x){ for(int i=0; i<size(); i++){ a[i] *= x; } return *this; }
    Fps& timesFactorial(int z = -1){ comb.extend(RSZ(z)); for(int i=0; i<z; i++){ a[i] *= comb.factorial(i); } return *this; }
    Fps& timesInvFactorial(int z = -1){ comb.extend(RSZ(z)); for(int i=0; i<z; i++){ a[i] *= comb.invFactorial(i); } return *this; }
    Fps& clrRange(int l, int r){ for(int i=l; i<r; i++){ a[i] = ZeroElem(); } return *this; }
    Fps& negate(){ for(auto& e : a){ e = -e; } return *this; }
    Fps& mulEach(const Fps& other, int maxi = -1){
        maxi = std::min(RSZ(maxi), std::min(size(), other.size()));
        for(int i=0; i<maxi; i++) a[i] *= other[i];
        return *this;
    }
    Fps& reverse(int sz = -1){ RSZ(sz); std::reverse(a.begin(), a.begin() + sz); return *this; }

    static Fps convolution(const Fps& a, const Fps& b, int sz = -1){
        if(std::min(a.size(), b.size()) <= CONV_THRES){
            if(a.size() > b.size()) return convolution(b, a, sz);
            if(sz < 0) sz = std::max(0, a.size() + b.size() - 1);
            std::vector<Elem> res(sz);
            for(int i=0; i<a.size(); i++) for(int j=0; j<b.size() && i+j<sz; j++) res[i+j] += a[i] * b[j];
            return res;
        }
        int Z = BestNttSize(a.size() + b.size() - 1);
        return a.clip(0, Z).ntt().mulEach(b.clip(0, Z).ntt()).intt().capSize(sz).move();
    }
    Fps convolve(const Fps& r, int sz = -1) const { return convolution(*this, r, sz); }
    
    //   1
    // ----- = 1 + f + f^2 + f^3 + ...
    //  1-f
    Fps powerSum(int sz) const {
        RSZ(sz);
        if(sz == 0) return {};
        int q = std::min(sz, 32);
        Fps x = Fps(q).set(0, OneElem()).move();
        for(int i=1; i<q; i++) for(int j=1; j<=std::min(i,(int)a.size()-1); j++) x[i] += x[i-j] * a[j];
        while(x.size() < sz){
            int hN = x.size(), N = hN*2;
            Fps a = x.clip(0, N).ntt().move();
            Fps b = clip(0, N).ntt().mulEach(a).intt().clrRange(0,hN).ntt().mulEach(a).intt().move();
            for(int i=0; i<hN; i++) b[i] = x[i];
            std::swap(b, x);
        }
        return x.capSize(sz).move();
    }

    Fps inv(int sz = -1) const {
        RSZ(sz);
        Elem iA0 = a[0].inv();
        return clip(0, std::min(sz, size())).times(-iA0).set(0, ZeroElem()).powerSum(sz).times(iA0).move();
    }
    
    Fps& difference(){
        if(size() == 0) return *this;
        for(int i=0; i+1<size(); i++) a[i] = a[i+1] * Elem::raw(i+1);
        return capSize(size()-1);
    }
    Fps& integral(){
        if(size() == 0) return capSize(1);
        capSize(size()+1);
        comb.extend(size());
        for(int i=size()-1; i>=1; i--) a[i] = a[i-1] * comb.invOf(i);
        return set(0, ZeroElem());
    }
    Fps& EgfToOgf(){
        comb.extend(size());
        for(int i=0; i<size(); i++) a[i] *= comb.factorial(i);
        return *this;
    }
    Fps& OgfToEgf(){
        comb.extend(size());
        for(int i=0; i<size(); i++) a[i] *= comb.invFactorial(i);
        return *this;
    }
    
    Fps log(int sz = -1){
        RSZ(sz);
        assert(sz != 0);
        assert(a[0].val() == 1);
        return convolution(inv(sz), clip().difference(), sz-1).integral();
    }

    Fps exp(int sz = -1){
        RSZ(sz);
        Fps res = Fps(1).set(0, OneElem());
        while(res.size() < sz){
            auto z = res.size();
            auto tmp = res.capSize(z*2).log().set(0, -OneElem()).move();
            for(int i=0; i<z*2 && i<size(); i++) tmp[i] -= a[i];
            auto resntt = res.clip().ntt().mulEach(tmp.ntt()).intt().move();
            for(int i=z; i<z*2; i++) res[i] = -resntt[i];
        }
        return res.capSize(0, sz).move();
    }
    
    Fps pow(unsigned long long k, int sz = -1){
        int n = RSZ(sz);
        if(k == 0) return Fps(n).set(0, OneElem()).move();
        int ctz = 0;
        while(ctz<n && a[ctz].val() == 0) ctz++;
        if((unsigned long long)ctz >= (n-1) / k + 1) return Fps(n);
        Elem a0 = a[ctz];
        return clip(ctz, ctz+n-ctz*k).times(a0.inv()).log().times(Elem(k)).exp().times(a0.pow(k)).clip(0, -1, ctz*k);
    }

    auto begin(){ return a.begin(); }
    auto end(){ return a.end(); }
    auto begin() const { return a.begin(); }
    auto end() const { return a.end(); }

    std::string toString(std::string beg = "[ ", std::string delim = " ", std::string en = " ]") const {
        std::string res = beg;
        bool f = false;
        for(auto x : a){ if(f){ res += delim; } f = true; res += std::to_string(x.val()); }
        res += en;
        return res;
    }

    std::vector<Elem> getVectorMoved(){ return std::move(a); }

    Fps& operator+=(const Fps& r){
        capSize(std::max(size(), r.size()));
        for(int i=0; i<r.size(); i++) a[i] += r[i];
        return *this;
    }
    Fps& operator-=(const Fps& r){
        capSize(std::max(size(), r.size()));
        for(int i=0; i<r.size(); i++) a[i] -= r[i];
        return *this;
    }
    Fps operator+(const Fps& r) const { return (clip(0, std::max(size(), r.size())) += r).move(); }
    Fps operator-(const Fps& r) const { return (clip(0, std::max(size(), r.size())) -= r).move(); }
    Fps operator-() const { return (clip().negate()).move(); }
    Fps operator*(const Fps& r) const { return convolve(r).removeLeadingZeros().move(); }
    Fps& operator*=(const Fps& r){ return (*this) = operator*(r); }
    Fps& operator*=(Elem m){ return times(m); }
    Fps operator*(Elem m) const { return (clip() *= m).move(); }

    Elem eval(Elem x) const {
        Elem res = 0;
        for(int i=size()-1; i>=0; i--) res = res * x + a[i];
        return res;
    }
};

template<class Elem, class NttInst> Comb<Elem> FpsNtt<Elem, NttInst>::comb;
template<class Elem, class NttInst> const NttInst FpsNtt<Elem, NttInst>::nttInst;

} // namespace nachia




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
int main(){
    using nachia::cin;
    using nachia::cout;
    using Modint = nachia::StaticModint<998244353>;
    using Sparce = nachia::SparsePolynomialFp<Modint>;
    int N; cin >> N;
    int K; cin >> K;
    long long M; cin >> M;
    Sparce f;
    for(int i=0; i<K; i++){
        int j, a; cin >> j >> a;
        f.a.push_back({ j, Modint::raw(a) });
    }
    auto ans = f.pow(N, M);
    for(int i=0; i<N; i++){
        if(i) cout << ' ';
        cout << ans[i].val();
    } cout << '\n';
    return 0;
}
