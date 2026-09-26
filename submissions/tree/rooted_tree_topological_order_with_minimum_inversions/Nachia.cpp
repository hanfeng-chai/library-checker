#define PROBLEM "https://judge.yosupo.jp/problem/rooted_tree_topological_order_with_minimum_inversions"
#include <vector>
#include <algorithm>
#include <queue>
#include <utility>
#include <cassert>

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
    Graph() : m_n(0), m_e(0), m_isUndir(false) {}
    explicit Graph(int n, bool undirected = false, int m = 0) : m_n(n), m_e(m), m_isUndir(undirected) {}
    explicit Graph(int n, const std::vector<std::pair<int, int>>& edges, int undirected = false) : m_n(n), m_isUndir(undirected){
        m_e.resize(edges.size());
        for(std::size_t i=0; i<edges.size(); i++) m_e[i] = { edges[i].first, edges[i].second };
    }
    template<class Cin>
    static Graph Input(Cin& cin, int n, bool undirected, int m, int offset = 0){
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

namespace nachia {

struct Dsu{
private:
    int N;
    std::vector<int> P;
    std::vector<int> H;
public:
    Dsu() : N(0) {}
    Dsu(int n) : N(n), P(n, -1), H(n) {
        for(int i=0; i<n; i++) H[i] = i;
    }
    int leader(int u){
        if(P[u] < 0) return u;
        int v = P[u];
        while(P[v] >= 0){ P[u] = P[v]; u = v; v = P[v]; }
        return P[u];
    }
    int append(int label){
        int n = P.size();
        P.push_back(-1);
        H.push_back(label);
        return n;
    }
    int append(){ return append(int(P.size())); }
    int label(int u){ return H[leader(u)]; }
    int operator[](int u){ return H[leader(u)]; }
    void merge(int u, int v, int newLabel){
        u = leader(u);
        v = leader(v);
        if(u == v){ H[u] = newLabel; return; }
        N--;
        if(-P[u] < -P[v]) std::swap(u, v);
        P[u] += P[v];
        H[P[v] = u] = newLabel;
    }
    int merge(int u, int v){ merge(u, v, u); return u; }
    int count(){ return N; }
    int size(int u){ return -P[leader(u)]; }
    bool same(int u, int v){ return leader(u) == leader(v); }
};

} // namespace nachia

namespace nachia {

template<class Weight>
std::vector<int> TreeSearchMinimumInversionWithParentArray(
    std::vector<int> par,
    std::vector<Weight> D,
    std::vector<Weight> C,
    int start
){
    struct Frac {
        Weight d;
        Weight c;
        int root;
        int label;
        bool operator<(const Frac& r) const {
            // (0,0) smallest
            if(r.c == 0 && r.d == 0) return false;
            if(c == 0 && d == 0) return true;

            // c / d increasing
            return c * r.d < d * r.c;
        }
    };
    int n = int(par.size());
    std::priority_queue<Frac> que;
    nachia::Dsu dsu(n);
    for(int v=0; v<n; v++) if(par[v] != -1){
        que.push({ D[v], C[v], v, 0 });
    }
    std::vector<int> linkA(n);
    for(int i=0; i<n; i++) linkA[i] = i;
    std::vector<int> label(n, 0);
    while(que.size()){
        auto q = que.top(); que.pop();
        int v = q.root;
        if(label[v] != q.label) continue;
        int w = dsu[par[v]];
        D[w] = D[w] + D[v];
        C[w] = C[w] + C[v];
        dsu.merge(v, w, w);
        if(par[w] != -1){
            que.push({ D[w], C[w], w, ++label[w] });
        }
        std::swap(linkA[v], linkA[w]);
    }
    std::vector<int> ans;
    ans.push_back(linkA[start]);
    while(ans.back() != start) ans.push_back(linkA[ans.back()]);
    std::reverse(ans.begin(), ans.end());
    return ans;
}

template<class Weight>
std::vector<int> TreeSearchMinimumInversion(
    const nachia::Graph& tree,
    const std::vector<Weight>& D,
    const std::vector<Weight>& C,
    int start,
    Weight zero = Weight(0)
){
    int n = tree.numVertices();
    auto adj = tree.getAdjacencyArray();
    std::vector<int> par(n, -1);
    std::vector<int> bfs;
    bfs.push_back(start);
    for(int i=0; i<int(bfs.size()); i++){
        int v = bfs[i];
        for(int w : adj[v]) if(par[v] != w){
            par[w] = v;
            bfs.push_back(w);
        }
    }
    return TreeSearchMinimumInversionWithParentArray(
        std::move(par), D, C, start
    );
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

    int N; cin >> N;
    std::vector<int> P(N, -1);
    for(int i=1; i<N; i++) cin >> P[i];
    std::vector<long long> C(N), D(N);
    for(int i=0; i<N; i++) cin >> C[i];
    for(int i=0; i<N; i++) cin >> D[i];

    auto ans = nachia::TreeSearchMinimumInversionWithParentArray(
        std::move(P), D, C, 0
    );

    long long x = 0;
    long long c = 0;
    for(int a : ans){ x += C[a]*c; c += D[a]; }

    cout << x << "\n";
    for(int i=0; i<N; i++){
        if(i) cout << " ";
        cout << ans[i];
    } cout << "\n";

    return 0;
}
