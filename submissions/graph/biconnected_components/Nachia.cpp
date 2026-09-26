#line 2 "nachia\\graph\\dev\\biconnected-components.hpp"

#line 2 "nachia\\graph\\dev\\graph.hpp"
#include <vector>
#include <utility>
#include <cassert>
#line 4 "nachia\\array\\csr-array.hpp"
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
    static CsrArray Construct(int n, const std::vector<std::pair<int, Elem>>& items){
        CsrArray res;
        res.m_n = n;
        std::vector<int> buf(n+1, 0);
        for(auto& [u,v] : items){ ++buf[u]; }
        for(int i=1; i<=n; i++) buf[i] += buf[i-1];
        res.m_list.resize(buf[n]);
        for(int i=(int)items.size()-1; i>=0; i--){
            res.m_list[--buf[items[i].first]] = items[i].second;
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
#line 6 "nachia\\graph\\dev\\graph.hpp"

namespace nachia{


struct Graph {
public:
    struct Edge{ int from, to; };
    using Base = std::vector<std::pair<int, int>>;
    Graph(int n = 0, bool undirected = false) : m_n(n), m_e(), m_isUndir(undirected) {}
    Graph(int n, const std::vector<std::pair<int, int>>& edges, bool undirected = false) : m_n(n), m_isUndir(undirected){
        m_e.resize(edges.size());
        for(std::size_t i=0; i<edges.size(); i++) m_e[i] = { edges[i].first, edges[i].second };
    }
    Graph(int n, const std::vector<Edge>& edges, bool undirected = false) : m_n(n), m_e(edges), m_isUndir(undirected) {}
    Graph(int n, std::vector<Edge>&& edges, bool undirected = false) : m_n(n), m_e(edges), m_isUndir(undirected) {}
    int numVertices() const noexcept { return m_n; }
    int numEdges() const noexcept { return int(m_e.size()); }
    int addEdge(int from, int to){ m_e.push_back({ from, to }); return numEdges() - 1; }
    const Edge& operator[](int ei) const noexcept { return m_e[ei]; }
    const Edge& at(int ei) const { return m_e.at(ei); }
    void reverseEdges(){ for(auto& e : m_e) std::swap(e.from, e.to); }
    void contract(int newV, const std::vector<int>& mapping){
        assert(numVertices() == int(mapping.size()));
        for(int i=0; i<numVertices(); i++) assert(0 <= mapping[i] && mapping[i] < newV);
        for(auto& e : m_e){ e.from = mapping[e.from]; e.to = mapping[e.to]; }
    }
    std::vector<Graph> induce(int num, const std::vector<int>& mapping) const {
        int n = numVertices();
        assert(n == int(mapping.size()));
        for(int i=0; i<n; i++) assert(-1 <= mapping[i] && mapping[i] < num);
        std::vector<int> indexV(n), newV(num);
        for(int i=0; i<n; i++) if(mapping[i] >= 0) indexV[i] = ++newV[mapping[i]];
        std::vector<Graph> res; res.reserve(num);
        for(int i=0; i<num; i++) res.emplace_back(newV[i]);
        for(auto e : m_e) if(mapping[e.from] == mapping[e.to] && mapping[e.to] >= 0) res[mapping[e.to]].addEdge(indexV[e.from], indexV[e.to]);
        return res;
    }
    CsrArray<int> getEdgeIndexArray() const {
        std::vector<std::pair<int, int>> src;
        src.reserve(numEdges() * (m_isUndir ? 2 : 1));
        for(int i=0; i<numEdges(); i++){
            auto e = operator[](i);
            src.emplace_back(e.from, i);
            if(m_isUndir) src.emplace_back(e.to, i);
        }
        return CsrArray<int>::Construct(numVertices(), src);
    }
    CsrArray<int> getAdjacencyArray() const {
        std::vector<std::pair<int, int>> src;
        src.reserve(numEdges() * (m_isUndir ? 2 : 1));
        for(auto e : m_e){
            src.emplace_back(e.from, e.to);
            if(m_isUndir) src.emplace_back(e.to, e.from);
        }
        return CsrArray<int>::Construct(numVertices(), src);
    }
private:
    int m_n;
    std::vector<Edge> m_e;
    bool m_isUndir;
};

} // namespace nachia
#line 3 "nachia\\graph\\dev\\dfs-tree.hpp"

namespace nachia{

struct DfsTree{
    std::vector<int> dfsOrd;
    std::vector<int> parent;

    template<bool OutOrd>
    static DfsTree Construct(const CsrArray<int>& adj){
        DfsTree res;
        int n = adj.size();
        res.dfsOrd.reserve(n);
        std::vector<int> eid(n, 0), parent(n, -2);
        for(int s=0; s<n; s++) if(parent[s] == -2){
            int p = s;
            if(p >= n) p -= n;
            parent[p] = -1;
            while(0 <= p){
                if(eid[p] == (OutOrd ? (int)adj[p].size() : 0)) res.dfsOrd.push_back(p);
                if(eid[p] == (int)adj[p].size()){ p = parent[p]; continue; }
                int nx = adj[p][eid[p]++];
                if(parent[nx] != -2) continue;
                parent[nx] = p;
                p = nx;
            }
        }
        res.parent = std::move(parent);
        return res;
    }
    template<bool OutOrd>
    static DfsTree Construct(const Graph& g){ return Construct<OutOrd>(g.getAdjacencyArray()); }
};

} // namespace nachia
#line 6 "nachia\\graph\\dev\\biconnected-components.hpp"

#line 8 "nachia\\graph\\dev\\biconnected-components.hpp"

namespace nachia{

class BiconnectedComponents{
private:
    int mn;
    int mm;
    int mnum_bcs;
    Graph mG;
    std::vector<std::pair<int,int>> m_bcVtxPair;
public:
    BiconnectedComponents(Graph G){
        int n = mn = G.numVertices();
        int m = mm = G.numEdges();
        mG = std::move(G);
        auto adj = mG.getAdjacencyArray();

        auto dfstree = DfsTree::Construct<false>(adj);
        std::vector<int> vtxToDfsi(n), parent, low;
        for(int i=0; i<n; i++) vtxToDfsi[dfstree.dfsOrd[i]] = i;
        parent = std::move(dfstree.parent);
        low = vtxToDfsi;
        
        for(int p=0; p<n; p++) for(int e : adj[p]) low[p] = std::min(low[p], vtxToDfsi[e]);

        for(int i=n-1; i>=0; i--){
            int p = dfstree.dfsOrd[i];
            int pp = parent[p];
            if(pp >= 0) low[pp] = std::min(low[pp], low[p]);
        }
        
        int num_bcs = 0;
        std::vector<int> res(m);
        for(int p : dfstree.dfsOrd) if(parent[p] >= 0){
            int pp = parent[p];
            if(low[p] < vtxToDfsi[pp]){
                low[p] = low[pp];
                m_bcVtxPair.push_back(std::make_pair(low[p], p));
            }
            else{
                low[p] = num_bcs++;
                m_bcVtxPair.push_back(std::make_pair(low[p], pp));
                m_bcVtxPair.push_back(std::make_pair(low[p], p));
            }
        }
        for(int s=0; s<mn; s++) if(adj[s].size() == 0) m_bcVtxPair.push_back(std::make_pair(num_bcs++, s));
        mnum_bcs = num_bcs;
    }

    int getNumBcs() const { return mnum_bcs; }

    CsrArray<int> getBcVertices() const {
        return CsrArray<int>::Construct(getNumBcs(), m_bcVtxPair);
    }

    Graph getBct() const {
        int bct_n = mn + mnum_bcs;
        std::vector<std::pair<int, int>> res = m_bcVtxPair;
        for(auto& e : res) e.first += mn;
        return Graph(bct_n, std::move(res), true);
    }

    CsrArray<int> getBcEdges() const {
        auto bct = getBct().getAdjacencyArray();
        std::vector<int> bfsP(bct.size(), -1);
        std::vector<int> bfsD(bct.size(), 0);
        std::vector<int> bfs(bct.size());
        int p0 = 0, p1 = 0;
        for(int s=0; s<bct.size(); s++) if(bfsP[s] < 0){
            for(bfs[p1++]=s; p0<p1; p0++){
                int p = bfs[p0];
                for(auto e : bct[p]) if(bfsP[p] != e){
                    bfsP[e] = p;
                    bfsD[e] = bfsD[p] + 1;
                    bfs[p1++] = e;
                }
            }
        }
        std::vector<std::pair<int,int>> res(mm);
        for(int i=0; i<mm; i++){
            int u = mG[i].from, v = mG[i].to;
            res[i].first = bfsP[(bfsD[u] <= bfsD[v]) ? v : u] - mn;
            res[i].second = i;
        }
        return CsrArray<int>::Construct(mnum_bcs, res);
    }
};

} // namespace nachia
#line 2 "nachia\\misc\\fastio.hpp"

#include <cstdio>
#include <string>

namespace nachia{


const unsigned int INPUT_BUF_SIZE = 1 << 17;
const unsigned int OUTPUT_BUF_SIZE = 1 << 17;

char input_buf[INPUT_BUF_SIZE];

struct InputBufIterator{
private:
    unsigned int p = INPUT_BUF_SIZE;
public:
    using MyType = InputBufIterator;
    static bool is_whitespace(char ch){
        switch(ch){
            case ' ': case '\n': case '\r': case '\t': return true;
        }
        return false;
    }
    char seek_char(){
        if(p == INPUT_BUF_SIZE){
            size_t len = fread(input_buf, 1, INPUT_BUF_SIZE, stdin);
            if(len != INPUT_BUF_SIZE) input_buf[len] = '\0';
            p = 0;
        }
        return input_buf[p];
    }
    void skip_whitespace(){
        while(is_whitespace(seek_char())) p++;
    }
    unsigned int next_uint(){
        skip_whitespace();
        unsigned int buf = 0;
        while(true){
            char tmp = seek_char();
            if('9' < tmp || tmp < '0') break;
            buf = buf * 10 + (tmp - '0');
            p++;
        }
        return buf;
    }
    int next_int(){
        skip_whitespace();
        if(seek_char() == '-'){
            p++;
            return (int)(-next_uint());
        }
        return (int)next_uint();
    }
    unsigned long long next_ulong(){
        skip_whitespace();
        unsigned long long buf = 0;
        while(true){
            char tmp = seek_char();
            if('9' < tmp || tmp < '0') break;
            buf = buf * 10 + (tmp - '0');
            p++;
        }
        return buf;
    }
    long long next_long(){
        skip_whitespace();
        if(seek_char() == '-'){
            p++;
            return (long long)(-next_ulong());
        }
        return (long long)next_ulong();
    }
    char next_char(){
        skip_whitespace();
        char buf = seek_char();
        p++;
        return buf;
    }
    std::string next_token(){
        skip_whitespace();
        std::string buf;
        while(true){
            char ch = seek_char();
            if(is_whitespace(ch) || ch == '\0') break;
            buf.push_back(ch);
            p++;
        }
        return buf;
    }
    MyType& operator>>(unsigned int& dest){ dest = next_uint(); return *this; }
    MyType& operator>>(int& dest){ dest = next_int(); return *this; }
    MyType& operator>>(unsigned long& dest){ dest = next_ulong(); return *this; }
    MyType& operator>>(long& dest){ dest = next_long(); return *this; }
    MyType& operator>>(unsigned long long& dest){ dest = next_ulong(); return *this; }
    MyType& operator>>(long long& dest){ dest = next_long(); return *this; }
    MyType& operator>>(std::string& dest){ dest = next_token(); return *this; }
    MyType& operator>>(char& dest){ dest = next_char(); return *this; }
};


char output_buf[OUTPUT_BUF_SIZE] = {};

struct FastOutputTable{
    char dig3lz[1000][4];
    char dig3nlz[1000][4];
    FastOutputTable(){
        for(unsigned int d=0; d<1000; d++){
            unsigned int x = d;
            unsigned int i = 0;
            dig3lz[d][i++] = ('0' + x / 100 % 10);
            dig3lz[d][i++] = ('0' + x /  10 % 10);
            dig3lz[d][i++] = ('0' + x /   1 % 10);
            dig3lz[d][i++] = '\0';
        }
        for(unsigned int d=0; d<1000; d++){
            unsigned int x = d;
            unsigned int i = 0;
            if(x >= 100) dig3nlz[d][i++] = ('0' + x / 100 % 10);
            if(x >=  10) dig3nlz[d][i++] = ('0' + x /  10 % 10);
            if(x >=   1) dig3nlz[d][i++] = ('0' + x /   1 % 10);
            dig3nlz[d][i++] = '\0';
        }
    }
} fastoutput_table_inst;

struct OutputBufIterator{
    using MyType = OutputBufIterator;
    unsigned int p = 0;
    static constexpr unsigned int POW10(int d){ return (d<=0) ? 1 : (POW10(d-1)*10); }
    static constexpr unsigned long long POW10LL(int d){ return (d<=0) ? 1 : (POW10LL(d-1)*10); }
    void next_char(char c){
        output_buf[p++] = c;
        if(p == OUTPUT_BUF_SIZE){
            fwrite(output_buf, p, 1, stdout);
            p = 0;
        }
    }
    void next_eoln(){
        next_char('\n');
    }
    void next_cstr(const char* s){
        unsigned int i = 0;
        while(s[i]) next_char(s[i++]);
    }
    void next_dig9(unsigned int x){
        unsigned int y;
        y = x / POW10(6); x -= y * POW10(6);
        next_cstr(fastoutput_table_inst.dig3lz [y]);
        y = x / POW10(3); x -= y * POW10(3);
        next_cstr(fastoutput_table_inst.dig3lz [y]);
        y = x;
        next_cstr(fastoutput_table_inst.dig3lz [y]);
    }
    void next_uint(unsigned int x){
        unsigned int y = 0;
        if(x >= POW10(9)){
            y = x / POW10(9); x -= y * POW10(9);
            next_cstr(fastoutput_table_inst.dig3nlz[y]);
            next_dig9(x);
        }
        else if(x >= POW10(6)){
            y = x / POW10(6); x -= y * POW10(6);
            next_cstr(fastoutput_table_inst.dig3nlz[y]);
            y = x / POW10(3); x -= y * POW10(3);
            next_cstr(fastoutput_table_inst.dig3lz [y]);
            next_cstr(fastoutput_table_inst.dig3lz [x]);
        }
        else if(x >= POW10(3)){
            y = x / POW10(3); x -= y * POW10(3);
            next_cstr(fastoutput_table_inst.dig3nlz[y]);
            next_cstr(fastoutput_table_inst.dig3lz [x]);
        }
        else if(x >= 1){
            next_cstr(fastoutput_table_inst.dig3nlz[x]);
        }
        else{
            next_char('0');
        }
    }
    void next_int(int x){
        if(x >= 0) next_uint(x);
        else{
            next_char('-');
            next_uint((unsigned int)-x);
        }
    }
    void next_ulong(unsigned long long x){
        unsigned int y = 0;
        if(x >= POW10LL(18)){
            y = x / POW10LL(18); x -= y * POW10LL(18);
            next_uint(y);
            y = x / POW10LL(9); x -= y * POW10LL(9);
            next_dig9(y);
            next_dig9(x);
        }
        else if(x >= POW10LL(9)){
            y = x / POW10LL(9); x -= y * POW10LL(9);
            next_uint(y);
            next_dig9(x);
        }
        else{
            next_uint(x);
        }
    }
    void next_long(long long x){
        if(x >= 0) next_ulong(x);
        else{
            next_char('-');
            next_ulong((unsigned long long)-x);
        }
    }
    void write_to_file(bool flush = false){
        fwrite(output_buf, p, 1, stdout);
        if(flush) fflush(stdout);
        p = 0;
    }
    ~OutputBufIterator(){ write_to_file(); }
    MyType& operator<<(unsigned int tg){ next_uint(tg); return *this; }
    MyType& operator<<(unsigned long tg){ next_ulong(tg); return *this; }
    MyType& operator<<(unsigned long long tg){ next_ulong(tg); return *this; }
    MyType& operator<<(int tg){ next_int(tg); return *this; }
    MyType& operator<<(long tg){ next_long(tg); return *this; }
    MyType& operator<<(long long tg){ next_long(tg); return *this; }
    MyType& operator<<(const std::string& tg){ next_cstr(tg.c_str()); return *this; }
    MyType& operator<<(const char* tg){ next_cstr(tg); return *this; }
    MyType& operator<<(char tg){ next_char(tg); return *this; }
};

} // namespace nachia
#line 3 "Main.cpp"

int main(){
    nachia::InputBufIterator iitr;
    nachia::OutputBufIterator oitr;

    int n = iitr.next_uint();
    int m = iitr.next_uint();
    std::vector<std::pair<int, int>> edges(m);
    for(auto& [u,v] : edges){
        u = iitr.next_uint();
        v = iitr.next_uint();
    }

    auto bc = nachia::BiconnectedComponents(nachia::Graph(n, std::move(edges), true));
    auto bcv = bc.getBcVertices();

    oitr << bcv.size() << '\n';
    for(int i=0; i<bcv.size(); i++){
        oitr << bcv[i].size();
        for(auto v : bcv[i]) oitr << ' ' << v;
        oitr << '\n';
    }
    return 0;
}
