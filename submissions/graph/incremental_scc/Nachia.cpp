#define PROBLEM "https://judge.yosupo.jp/problem/incremental_scc"
#include <vector>
#include <utility>
#include <cassert>
#include <algorithm>

namespace nachia{

template<class Elem>
class CsrArray{
public:
    struct ListRange{
        using iterator = typename std::vector<Elem>::iterator;
        iterator begi, endi;
        iterator begin() const { return begi; }
        iterator end() const { return endi; }
        int size() const { return (int)std::distance(begi, endi); }
        Elem& operator[](int i) const { return begi[i]; }
    };
    struct ConstListRange{
        using iterator = typename std::vector<Elem>::const_iterator;
        iterator begi, endi;
        iterator begin() const { return begi; }
        iterator end() const { return endi; }
        int size() const { return (int)std::distance(begi, endi); }
        const Elem& operator[](int i) const { return begi[i]; }
    };
private:
    int m_n;
    std::vector<Elem> m_list;
    std::vector<int> m_pos;
public:
    CsrArray() : m_n(0), m_list(), m_pos() {}
    static CsrArray Construct(int n, std::vector<std::pair<int, Elem>> items){
        CsrArray res;
        res.m_n = n;
        std::vector<int> buf(n+1, 0);
        for(auto& [u,v] : items){ ++buf[u]; }
        for(int i=1; i<=n; i++) buf[i] += buf[i-1];
        res.m_list.resize(buf[n]);
        for(int i=(int)items.size()-1; i>=0; i--){
            res.m_list[--buf[items[i].first]] = std::move(items[i].second);
        }
        res.m_pos = std::move(buf);
        return res;
    }
    static CsrArray FromRaw(std::vector<Elem> list, std::vector<int> pos){
        CsrArray res;
        res.m_n = pos.size() - 1;
        res.m_list = std::move(list);
        res.m_pos = std::move(pos);
        return res;
    }
    ListRange operator[](int u) { return ListRange{ m_list.begin() + m_pos[u], m_list.begin() + m_pos[u+1] }; }
    ConstListRange operator[](int u) const { return ConstListRange{ m_list.begin() + m_pos[u], m_list.begin() + m_pos[u+1] }; }
    int size() const { return m_n; }
    int fullSize() const { return (int)m_list.size(); }
};

} // namespace nachia

namespace nachia{


struct Graph {
public:
    struct Edge{
        int from, to;
        void reverse(){ std::swap(from, to); }
        int xorval() const { return from ^ to; }
    };
    Graph(int n = 0, bool undirected = false, int m = 0) : m_n(n), m_e(m), m_isUndir(undirected) {}
    Graph(int n, const std::vector<std::pair<int, int>>& edges, bool undirected = false) : m_n(n), m_isUndir(undirected){
        m_e.resize(edges.size());
        for(std::size_t i=0; i<edges.size(); i++) m_e[i] = { edges[i].first, edges[i].second };
    }
    template<class Cin>
    static Graph Input(Cin& cin, int n, bool undirected, int m, bool offset = 0){
        Graph res(n, undirected, m);
        for(int i=0; i<m; i++){
            int u, v; cin >> u >> v;
            res[i].from = u - offset;
            res[i].to = v - offset;
        }
        return res;
    }
    int numVertices() const noexcept { return m_n; }
    int numEdges() const noexcept { return int(m_e.size()); }
    int addNode() noexcept { return m_n++; }
    int addEdge(int from, int to){ m_e.push_back({ from, to }); return numEdges() - 1; }
    Edge& operator[](int ei) noexcept { return m_e[ei]; }
    const Edge& operator[](int ei) const noexcept { return m_e[ei]; }
    Edge& at(int ei) { return m_e.at(ei); }
    const Edge& at(int ei) const { return m_e.at(ei); }
    auto begin(){ return m_e.begin(); }
    auto end(){ return m_e.end(); }
    auto begin() const { return m_e.begin(); }
    auto end() const { return m_e.end(); }
    bool isUndirected() const noexcept { return m_isUndir; }
    void reverseEdges() noexcept { for(auto& e : m_e) e.reverse(); }
    void contract(int newV, const std::vector<int>& mapping){
        assert(numVertices() == int(mapping.size()));
        for(int i=0; i<numVertices(); i++) assert(0 <= mapping[i] && mapping[i] < newV);
        for(auto& e : m_e){ e.from = mapping[e.from]; e.to = mapping[e.to]; }
        m_n = newV;
    }
    std::vector<Graph> induce(int num, const std::vector<int>& mapping) const {
        int n = numVertices();
        assert(n == int(mapping.size()));
        for(int i=0; i<n; i++) assert(-1 <= mapping[i] && mapping[i] < num);
        std::vector<int> indexV(n), newV(num);
        for(int i=0; i<n; i++) if(mapping[i] >= 0) indexV[i] = newV[mapping[i]]++;
        std::vector<Graph> res; res.reserve(num);
        for(int i=0; i<num; i++) res.emplace_back(newV[i], isUndirected());
        for(auto e : m_e) if(mapping[e.from] == mapping[e.to] && mapping[e.to] >= 0) res[mapping[e.to]].addEdge(indexV[e.from], indexV[e.to]);
        return res;
    }
    CsrArray<int> getEdgeIndexArray(bool undirected) const {
        std::vector<std::pair<int, int>> src;
        src.reserve(numEdges() * (undirected ? 2 : 1));
        for(int i=0; i<numEdges(); i++){
            auto e = operator[](i);
            src.emplace_back(e.from, i);
            if(undirected) src.emplace_back(e.to, i);
        }
        return CsrArray<int>::Construct(numVertices(), src);
    }
    CsrArray<int> getEdgeIndexArray() const { return getEdgeIndexArray(isUndirected()); }
    CsrArray<int> getAdjacencyArray(bool undirected) const {
        std::vector<std::pair<int, int>> src;
        src.reserve(numEdges() * (undirected ? 2 : 1));
        for(auto e : m_e){
            src.emplace_back(e.from, e.to);
            if(undirected) src.emplace_back(e.to, e.from);
        }
        return CsrArray<int>::Construct(numVertices(), src);
    }
    CsrArray<int> getAdjacencyArray() const { return getAdjacencyArray(isUndirected()); }
private:
    int m_n;
    std::vector<Edge> m_e;
    bool m_isUndir;
};

} // namespace nachia

namespace nachia{

struct StronglyConnectedComponents{
private:
    int m_n;
    CsrArray<int> induce;
    int componentNum;
public:

    StronglyConnectedComponents() : m_n(0), induce(), componentNum(0) {}
    StronglyConnectedComponents(Graph E) {
        int n = E.numVertices();
        m_n = n;
        std::vector<int> O(n);
        {
            auto adj = E.getAdjacencyArray();
            int Oi = n;
            std::vector<int> P(n, -1), EI(n, 0);
            for(int s=0; s<n; s++) if(P[s] == -1){
                P[s] = -2;
                int p = s;
                while(p >= 0){
                    if(EI[p] == adj[p].size()){ O[--Oi] = p; p = P[p]; continue; }
                    int q = adj[p][EI[p]++];
                    if(P[q] == -1){ P[q] = p; p = q; }
                }
            }
        }
        E.reverseEdges();
        auto adj = E.getAdjacencyArray();
        std::vector<int> sep = {0}, csr(n), vis(n,0);
        int p1 = 0, p2 = 0;
        for(int s : O) if(!vis[s]){
            csr[p2++] = s; vis[s] = 1;
            for(; p1<p2; p1++){
                int v = csr[p1];
                for(auto e : adj[v]) if(!vis[e]){ vis[e] = 1; csr[p2++] = e; }
            }
            sep.push_back(p2);
        }
        induce = CsrArray<int>::FromRaw(std::move(csr), std::move(sep));
        componentNum = induce.size();
    }

    int numComponents() const noexcept { return componentNum; }
    const CsrArray<int>& getCsr() const noexcept { return induce; }
    std::vector<int> getMapping() const {
        std::vector<int> res(m_n);
        for(int i=0; i<numComponents(); i++) for(int v : induce[i]) res[v] = i;
        return res;
    }
};

} // namespace nachia

namespace nachia {

struct DsuFast{
private:
    std::vector<int> w;
public:
    DsuFast(int n = 0) : w(n, -1) {}
    int leader(int u){
        if(w[u] < 0) return u;
        return w[u] = leader(w[u]);
    }
    int operator[](int u){ return leader(u); }
    int merge(int u, int v){
        u = leader(u);
        v = leader(v);
        if(u == v) return u;
        if(-w[u] < -w[v]) std::swap(u, v);
        w[u] += w[v];
        w[v] = u;
        return u;
    }
    int size(int u){ return -w[leader(u)]; }
    bool same(int u, int v){ return leader(u) == leader(v); }
};

} // namespace nachia

namespace nachia {

CsrArray<int> IncrementalSccOffline(Graph graph){
    int n = graph.numVertices();
    int m = graph.numEdges();
    std::vector<int> sep(m+1), neko, ya;
    using Cc = StronglyConnectedComponents;
    {
        auto cc = Cc(graph).getMapping();
        for(int i=0; i<m; i++){
            if(cc[graph[i].from] != cc[graph[i].to]) continue;
            ya.push_back(i);
        }
    }
    auto dfs = [&](auto& dfs, int n, int x, std::vector<int> ya) -> void {
        int m = int(ya.size());
        if(x == m) return;
        if(x + 1 == m){
            auto dsu = nachia::DsuFast(n);
            for(int e=0; e<m; e++){
                auto [u,v] = graph[ya[e]];
                if(dsu.same(u,v)) continue;
                dsu.merge(u,v);
                sep[ya[x]+1] += 1; neko.push_back(ya[e]);
            }
            return;
        }
        std::vector<int> mp(n, -1);
        int nn = 0;
        for(int e=0; e<m; e++){
            auto& [u,v] = graph[ya[e]];
            if(mp[u] == -1) mp[u] = nn++;
            if(mp[v] == -1) mp[v] = nn++;
            u = mp[u];
            v = mp[v];
        }
        int y = (x + m) / 2;
        Graph tg(nn, false);
        for(int e=0; e<y; e++) tg.addEdge(graph[ya[e]].from, graph[ya[e]].to);
        auto cc = Cc(std::move(tg)).getMapping();
        std::vector<int> yal, yar;
        for(int e=0; e<x; e++){
            auto [u,v] = graph[ya[e]];
            (cc[u] == cc[v] ? yal : yar).push_back(ya[e]);
        }
        int xl = int(yal.size());
        for(int e=x; e<y; e++){
            auto [u,v] = graph[ya[e]];
            (cc[u] == cc[v] ? yal : yar).push_back(ya[e]);
        }
        int xr = int(yar.size());
        for(int e=y; e<m; e++){
            auto [u,v] = graph[ya[e]];
            if(cc[u] != cc[v]) yar.push_back(ya[e]);
        }
        dfs(dfs, nn, xl, std::move(yal));
        for(int e : yar){
            auto& [u,v] = graph[e];
            u = cc[u]; v = cc[v];
        }
        dfs(dfs, nn, xr, std::move(yar));
    };
    dfs(dfs, n, 0, std::move(ya));
    for(int i=0; i<m; i++) sep[i+1] += sep[i];
    return CsrArray<int>::FromRaw(std::move(neko), std::move(sep));
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
    constexpr int MOD = 998244353;
    int N, M; cin >> N >> M;
    std::vector<int> X(N);
    for(int i=0; i<N; i++) cin >> X[i];
    auto g = nachia::Graph::Input(cin, N, false, M, 0);
    int ans = 0;
    auto res = nachia::IncrementalSccOffline(g);
    auto dsu = nachia::DsuFast(N);
    for(int i=0; i<M; i++){
        for(int e : res[i]){
            int u = dsu[g[e].from];
            int v = dsu[g[e].to];
            int w = dsu.merge(u,v);
            ans = ((long long)X[u] * X[v] + ans) % MOD;
            X[w] = X[u] + X[v];
            if(X[w] >= MOD) X[w] -= MOD;
        }
        cout << ans << '\n';
    }
    return 0;
}
