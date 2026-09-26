#define PROBLEM "https://judge.yosupo.jp/problem/three_edge_connected_components"
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

struct ThreeEdgeConnectedComponents{
public:
    ThreeEdgeConnectedComponents(Graph G = Graph(0, true)){
        solve(std::move(G));
    }
    int num2EdgeCCComponents() const noexcept { return m_num2Component; }
    int num3EdgeCCComponents() const noexcept { return m_num3Component; }
    int operator[](int w) const { return m_color3[w]; }
    CsrArray<int> get3EdgeCCVertices() const {
        return InvMapping(num3EdgeCCComponents(), m_color3);
    }
    CsrArray<int> get2EdgeCCVertices() const {
        return InvMapping(num2EdgeCCComponents(), m_color2);
    }
    std::vector<int> getVertexMapping() const { return m_color3; }
private:
    int m_num2Component;
    int m_num3Component;
    std::vector<int> m_color3;
    std::vector<int> m_color2;
    int numVertices() const { return int(m_color3.size()); }
    static CsrArray<int> InvMapping(int m, const std::vector<int>& a){
        int n = int(a.size());
        std::vector<int> sep(m+1);
        std::vector<int> val(n);
        for(int i=0; i<n; i++) sep[a[i]]++;
        for(int i=0; i<m; i++) sep[i+1] += sep[i];
        for(int i=0; i<n; i++) val[--sep[a[i]]] = i;
        return CsrArray<int>::FromRaw(std::move(val), std::move(sep));
    }
    static int assignColors(std::vector<int>& a, const std::vector<int>& idx){
        int i = 0;
        for(int v : idx) a[v] = (a[v] == -1 ? i++ : a[a[v]]);
        return i;
    }
    void solve(Graph G){
        assert(G.isUndirected());
        int n = G.numVertices();
        int m = G.numEdges();
        std::vector<int> parEdge(n, -1);
        std::vector<int> dfsin(n, -1);
        std::vector<int> dfsout(n, 0);
        std::vector<int> lowpt(n, n);
        std::vector<int> deg(n, 0);
        std::vector<int> path(n, -1);
        m_color2.assign(n, -1);
        m_color3.assign(n, -1);
        auto inc = G.getEdgeIndexArray(true);
        int dfsid = 0;
        auto absorb = [&](int x, int borderl, int borderr){
            while(path[x] >= 0){
                int y = path[x];
                if(borderl < dfsin[y] || dfsout[y] < borderr) break;
                deg[x] += deg[y] - 2; deg[y] = 0;
                path[x] = path[y];
                m_color3[y] = x;
            }
        };
        for(int s=0; s<n; s++) if(dfsin[s] == -1){
            dfsin[s] = dfsid++;
            int w = s;
            while(true){
                if(inc[w].size() <= dfsout[w]){
                    dfsout[w] = dfsid;
                    if(parEdge[w] < 0) break;
                    int u = w; w = w ^ G[parEdge[w]].xorval();
                    if(dfsin[w] >= lowpt[u]){
                        m_color2[u] = w;
                        int lowpt_u = lowpt[u];
                        if(deg[u] == 2){ deg[u] -= 2; u = path[u]; }
                        if(lowpt[w] <= lowpt_u){
                            int buf = path[w];
                            path[w] = u;
                            absorb(w, n, -1);
                            path[w] = buf;
                        } else {
                            lowpt[w] = lowpt_u;
                            absorb(w, n, -1);
                            path[w] = u;
                        }
                    } else {
                        deg[w] -= 1;
                        deg[u] -= 1;
                    }
                    continue;
                }
                int e = inc[w][dfsout[w]++];
                int u = w ^ G[e].xorval();
                if(u == w) continue;
                deg[w] += 1;
                if(e == parEdge[w]){
                    continue;
                }
                if(dfsin[u] == -1){
                    dfsin[u] = dfsid++;
                    parEdge[u] = e;
                    w = u; continue;
                }
                if(dfsin[u] < dfsin[w]){
                    if(dfsin[u] < lowpt[w]){
                        lowpt[w] = dfsin[u];
                        absorb(w, n, -1);
                    }
                } else {
                    absorb(w, dfsin[u], dfsout[u]);
                    deg[w] -= 2;
                }
            }
        }
        for(int i=0; i<n; i++) dfsout[dfsin[i]] = i;
        m_num2Component = assignColors(m_color2, dfsout);
        m_num3Component = assignColors(m_color3, dfsout);
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

int main(){
    using nachia::cin;
    using nachia::cout;
    int N, M; cin >> N >> M;
    auto graph = nachia::Graph::Input(cin, N, true, M, 0);
    auto thecc = nachia::ThreeEdgeConnectedComponents(graph);
    auto csr = thecc.get3EdgeCCVertices();
    cout << thecc.num3EdgeCCComponents() << '\n';
    for(int i=0; i<csr.size(); i++){
        cout << csr[i].size();
        for(int v : csr[i]) cout << ' ' << v;
        cout << '\n';
    }
    return 0;
}
