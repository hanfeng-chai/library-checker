#define PROBLEM "https://judge.yosupo.jp/problem/bipartite_edge_coloring"
#include <vector>
#include <utility>
#include <algorithm>

namespace nachia {

struct RegularBipartiteGraph {
    int n;
    int k;
    std::vector<std::pair<int, int>> edges;
};

RegularBipartiteGraph BipartiteRegularizeEdgeColorEquivalent(
    int n1,
    int n2,
    std::vector<std::pair<int,int>> edges
){
    std::vector<int> ind1(n1);
    std::vector<int> ind2(n2);
    for(auto [u,v] : edges){ ind1[u]++; ind2[v]++; }
    int k = std::max(
        *std::max_element(ind1.begin(), ind1.end()),
        *std::max_element(ind2.begin(), ind2.end()));
    std::vector<int> map1(n1);
    std::vector<int> map2(n2);
    std::vector<int> indx1(std::max(n1, n2));
    std::vector<int> indx2(std::max(n1, n2));
    indx1[0] += ind1[0];
    for(int i=1; i<n1; i++){
        map1[i] = map1[i-1];
        if(indx1[map1[i]] + ind1[i] > k) map1[i]++;
        indx1[map1[i]] += ind1[i];
    }
    indx2[0] += ind2[0];
    for(int i=1; i<n2; i++){
        map2[i] = map2[i-1];
        if(indx2[map2[i]] + ind2[i] > k) map2[i]++;
        indx2[map2[i]] += ind2[i];
    }
    int n = std::max(map1.back() + 1, map2.back() + 1);
    RegularBipartiteGraph res = { n, k, std::vector<std::pair<int,int>>(n*k) };
    for(int i=0; i<int(edges.size()); i++){
        res.edges[i] = { map1[edges[i].first], map2[edges[i].second] };
    }
    int s1 = 0;
    int s2 = 0;
    for(int i=int(edges.size()); i<n*k; i++){
        while(indx1[s1] == k) s1++;
        while(indx2[s2] == k) s2++;
        res.edges[i] = { s1, s2 };
        indx1[s1]++; indx2[s2]++;
    }
    return res;
}

std::vector<int> RegularBipartiteEdgeColor(const RegularBipartiteGraph& g){
    int n = g.n * 2;
    std::vector<int> inci(n * g.k);
    int m = g.n * g.k;
    std::vector<int> xedge(m); {
        std::vector<int> head(n);
        for(int e=0; e<m; e++){
            auto [u,v] = g.edges[e];
            inci[g.k*(u*2+0) + head[u*2+0]++] = e;
            inci[g.k*(v*2+1) + head[v*2+1]++] = e;
            xedge[e] = (u*2+0) ^ (v*2+1);
        }
    }
    std::vector<int> flag_e(m);
    int nx_flag = 0;
    auto euler_splitting = [&](
        std::vector<int> pl,
        std::vector<int> pr
    ) -> std::vector<int> {
        nx_flag++;
        for(int sp=0; sp<n; sp++){
            int v = sp;
            while(true){
                if(pl[v] == pr[v]){
                    if(sp == v) break;
                    v ^= 1;
                    continue;
                }
                int e = inci[pl[v]++];
                int w = v;
                if(flag_e[e] != nx_flag){
                    flag_e[e] = nx_flag;
                    w = v ^ xedge[e];
                }
                if(w % 2 == 0) std::swap(inci[--pl[v]], inci[--pr[v]]);
                v = w;
            }
        }
        return pl;
    };
    auto swap_group = [&](
        const std::vector<int>& el,
        std::vector<int>& em,
        const std::vector<int>& er
    ) -> void {
        for(int i=0; i<n; i++){
            int len = std::min(em[i] - el[i], er[i] - em[i]);
            std::swap_ranges(
                inci.begin() + el[i],
                inci.begin() + (el[i] + len),
                inci.begin() + (er[i] - len));
            em[i] = er[i] + el[i] - em[i];
        }
    };
    auto take_matching = [&](
        int s, int d
    ) -> void {
        std::vector<int> pl(n);
        std::vector<int> pr(n);
        for(int i=0; i<n; i++) pl[i] = i * g.k + s;
        for(int i=0; i<n; i++) pr[i] = i * g.k + s + d;
        std::vector<int> pm = pr;
        int md = 1; while(md < n/2*d) md *= 2;
        int alpha = md / d;
        while(alpha % 2 == 0){ alpha /= 2; md /= 2; }
        for(int w=1; w<md; w*=2){
            if(alpha & w){
                auto plm = euler_splitting(pl, pm);
                int count_edges = 0;
                for(int i=0; i<n; i+=2) count_edges += pm[i] + pl[i] - plm[i] * 2;
                if(count_edges < 0) swap_group(pl, plm, pm);
                std::swap(pm, plm);
            } else {
                auto pmr = euler_splitting(pm, pr);
                int count_edges = 0;
                for(int i=0; i<n; i+=2) count_edges += pr[i] + pm[i] - pmr[i] * 2;
                if(count_edges < 0) swap_group(pm, pmr, pr);
                std::swap(pm, pmr);
            }
        }
    };
    auto part_color = [&](
        auto& rec,
        int s, int d
    ) -> void {
        if(d <= 1) return;
        int d2 = d;
        if(d2 % 2 == 1){
            if(s+d2 < g.k) d2++;
            else{ take_matching(s, d2); d2--; }
        }
        std::vector<int> pl(n);
        std::vector<int> pr(n);
        for(int i=0; i<n; i++) pl[i] = i * g.k + s;
        for(int i=0; i<n; i++) pr[i] = i * g.k + s + d2;
        euler_splitting(std::move(pl), std::move(pr));
        rec(rec, s+d2/2, d2/2);
        rec(rec, s, d2/2);
    };
    part_color(part_color, 0, g.k);
    std::vector<int> ans(m);
    for(int i=0; i<n; i+=2) for(int j=0; j<g.k; j++) ans[inci[i*g.k+j]] = j;
    return ans;
}

std::vector<int> BipartiteEdgeColor(
    int n1,
    int n2,
    std::vector<std::pair<int,int>> edges
){
    int m = edges.size();
    auto regularized = BipartiteRegularizeEdgeColorEquivalent(n1, n2, std::move(edges));
    auto ans = RegularBipartiteEdgeColor(regularized);
    ans.resize(m);
    return ans;
}

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

int main(){
    using nachia::cin;
    using nachia::cout;
    int L, R, M; cin >> L >> R >> M;
    std::vector<std::pair<int,int>> edges(M);
    for(auto &[u,v] : edges) cin >> u >> v;
    auto ans = nachia::BipartiteEdgeColor(L, R, std::move(edges));
    int k = 1 + *std::max_element(ans.begin(), ans.end());
    cout << k << '\n';
    for(auto a : ans) cout << a << '\n';
    return 0;
}
