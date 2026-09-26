#define PROBLEM "https://judge.yosupo.jp/problem/common_interval_decomposition_tree"
#include <vector>
#include <functional>
#include <cassert>
#include <algorithm>

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
    constexpr u64 hi = 0x88888888;
    constexpr u64 mi = 0x11111111;
    m = (((m | ~(hi - (m & ~hi))) & hi) * mi) >> 35;
    m = (((m | ~(hi - (x & ~hi))) & hi) * mi) >> 31;
    q += (m & 0xf) << 2;
    q += 0x3333333322221100 >> (((x >> q) & 0xf) << 2) & 0xf;
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

namespace nachia{

template<class T, class CompT = std::less<T>>
struct RangeMinFast{
private:
    static constexpr int B = 16;
    std::vector<T> A;
    std::vector<T> LB;
    std::vector<T> RB;
    std::vector<std::vector<T>> spa;
    CompT comp;
public:
    RangeMinFast() {}
    RangeMinFast(std::vector<T> a)
        : A(std::move(a)) , comp()
    {
        int n = (int)A.size();
        LB = A;
        for(int i=n-2; i>=0; i--) if(i%B != 0 && i%B != B-1){
            if(comp(LB[i+1],LB[i])) LB[i] = LB[i+1];
        }
        RB = A;
        for(int i=1; i<n; i++) if(i%B != 0 && i%B != B-1){
            if(comp(RB[i-1],RB[i])) RB[i] = RB[i-1];
        }
        int n2 = n / B;
        if(n2 != 0){
            int logn2 = MsbIndex(n2+1) + 1;
            spa.resize(logn2);
            for(int d=0; d<logn2; d++) spa[d].resize(n2-(1<<d)+1);
            for(int i=0; i<n2; i++) spa[0][i] = std::min(A[i*B], LB[i*B+1], comp);
            for(int d=0; d+1<logn2; d++){
                int len = spa[d+1].size();
                for(int i=0; i<len; i++){
                    spa[d+1][i] = std::min(spa[d][i], spa[d][i+(1<<d)], comp);
                }
            }
        }
    }
    T min(int l, int r) const {
        if(r-l <= B){
            T d = A[l];
            for(int j=l+1; j<r; j++) if(comp(A[j], d)) d = A[j];
            return d;
        }
        int x = std::min(LB[l], RB[r-1], comp);
        int lb = (l+(B-1)) / B;
        int rb = r / B;
        if(lb == rb) return x;
        int q = MsbIndex(rb-lb);
        return std::min(std::min(x, spa[q][lb], comp), spa[q][rb-(1<<q)], comp);
    }
};

} // namespace nachia
#include <utility>

namespace nachia {

struct CommonIntervalDecompositionTree {
    enum NodeType {
        Prime,
        Dec,
        Inc,
        One
    };
    struct Node {
        int parent;
        NodeType type;
        int l;
        int r;
    };
    std::vector<Node> tree;
    CommonIntervalDecompositionTree();
    CommonIntervalDecompositionTree(std::vector<int> P){
        int n = int(P.size());
        calc(n, std::move(P));
    }
private:
    void calc(int n, std::vector<int> P){
        std::vector<int> Q(n);
        for(int i=0; i<n; i++) Q[P[i]] = i;
        auto rm_h = RangeMinFast(Q);
        struct LeftBase { int l; int vl; int vr; };
        struct Common { int l; int r; int v; };
        std::vector<LeftBase> st;
        std::vector<Common> coms;
        for(int r=1; r<=n; r++){
            int a = P[r-1];
            LeftBase bs = { r-1, a, a+1 };
            while(!st.empty()){
                if(bs.vl < st.back().vl) st.back().vl = bs.vl;
                if(bs.vr > st.back().vr) st.back().vr = bs.vr;
                auto nx = st.back();
                if(rm_h.min(nx.vl, nx.vr) < nx.l){
                    st.pop_back();
                    auto& nx2 = st.back();
                    if(nx.vl < nx2.vl) nx2.vl = nx.vl;
                    if(nx.vr > nx2.vr) nx2.vr = nx.vr;
                }
                else if(nx.vr - nx.vl == r - nx.l){
                    bs = nx;
                    st.pop_back();
                    coms.push_back({ nx.l, r, nx.vl });
                }
                else break;
            }
            st.push_back(bs);
        }
        while(st.size() >= 2){
            auto nx = st.back(); st.pop_back();
            auto& nx2 = st.back();
            if(nx.vl < nx2.vl) nx2.vl = nx.vl;
            if(nx.vr > nx2.vr) nx2.vr = nx.vr;
            if(nx2.vr - nx2.vl == n - nx2.l) coms.push_back({ nx2.l, n, nx2.vl });
        }
        if(st.size() != 1) coms.push_back({ 0, n, 0 });
        std::vector<Node> res;
        for(int i=0; i<n; i++) res.push_back({ -1,One,i,i+1 });
        std::vector<int> nodeid(n);
        for(int i=0; i<n; i++) nodeid[i] = i;
        std::vector<int> sll(n);
        for(int i=0; i<n; i++) sll[i] = i+1;
        for(auto com : coms){
            int m = sll[com.l];
            if(sll[m] == com.r){
                int a = nodeid[com.l];
                int b = nodeid[m];
                sll[com.l] = com.r;
                auto tgty = P[com.l] < P[com.r-1] ? Inc : Dec;
                if(res[a].type == tgty){
                    res[b].parent = a;
                    res[a].r = com.r;
                } else {
                    int c = int(res.size());
                    res.push_back({ -1, tgty, com.l, com.r });
                    res[a].parent = c;
                    res[b].parent = c;
                    nodeid[com.l] = c;
                }
            } else {
                int c = int(res.size());
                res.push_back({ -1, Prime, com.l, com.r });
                for(int p=com.l; p<com.r; p=sll[p]){
                    res[nodeid[p]].parent = c;
                }
                nodeid[com.l] = c;
                sll[com.l] = com.r;
            }
        }
        std::swap(tree, res);
    }
};

} // namespace nachia
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

// #include "nachia/permutation/common-interval-decomposition-tree.hpp"
// #include "nachia/misc/fastio.hpp"

int main(){
    using nachia::cin;
    using nachia::cout;
    int N; cin >> N;
    std::vector<int> A(N);
    for(int i=0; i<N; i++) cin >> A[i];
    using Util = nachia::CommonIntervalDecompositionTree;
    auto cidt = Util(A);
    cout << cidt.tree.size() << '\n';
    for(auto& v : cidt.tree){
        cout << v.parent << ' ' << v.l << ' ' << (v.r-1) << ' ';
        cout << (v.type == Util::Prime ? "prime\n" : "linear\n");
    }
    return 0;
}
