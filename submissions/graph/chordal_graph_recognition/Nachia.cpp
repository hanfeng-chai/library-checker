#line 1 "Main.cpp"

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
#line 2 "nachia\\graph\\maximum-cardinality-search.hpp"

#line 2 "nachia\\graph\\adjacency-list.hpp"

#include <vector>
#include <utility>
#include <cassert>

namespace nachia{
    
struct AdjacencyList{
public:
    struct AdjacencyListRange{
        using iterator = typename std::vector<int>::const_iterator;
        iterator begi, endi;
        iterator begin() const { return begi; }
        iterator end() const { return endi; }
        int size() const { return (int)std::distance(begi, endi); }
        const int& operator[](int i) const { return begi[i]; }
    };
private:
    int mn;
    std::vector<int> E;
    std::vector<int> I;
public:
    AdjacencyList(int n, const std::vector<std::pair<int,int>>& edges, bool rev){
        mn = n;
        std::vector<int> buf(n+1, 0);
        for(auto [u,v] : edges){ ++buf[u]; if(rev) ++buf[v]; }
        for(int i=1; i<=n; i++) buf[i] += buf[i-1];
        E.resize(buf[n]);
        for(int i=(int)edges.size()-1; i>=0; i--){
            auto [u,v] = edges[i];
            E[--buf[u]] = v;
            if(rev) E[--buf[v]] = u;
        }
        I = std::move(buf);
    }
    AdjacencyList(const std::vector<std::vector<int>>& edges = {}){
        int n = mn = edges.size();
        std::vector<int> buf(n+1, 0);
        for(int i=0; i<n; i++) buf[i+1] = buf[i] + edges[i].size();
        E.resize(buf[n]);
        for(int i=0; i<n; i++) for(int j=0; j<(int)edges[i].size(); j++) E[buf[i]+j] = edges[i][j];
        I = std::move(buf);
    }
    static AdjacencyList from_raw(std::vector<int> targets, std::vector<int> bounds){
        AdjacencyList res;
        res.mn = bounds.size() - 1;
        res.E = std::move(targets);
        res.I = std::move(bounds);
        return res;
    }
    AdjacencyListRange operator[](int u) const {
        return AdjacencyListRange{ E.begin() + I[u], E.begin() + I[u+1] };
    }
    int num_vertices() const { return mn; }
    int size() const { return num_vertices(); }
    int num_edges() const { return E.size(); }
    AdjacencyList reversed_edges() const {
        AdjacencyList res;
        int n = res.mn = mn;
        std::vector<int> buf(n+1, 0);
        for(int v : E) ++buf[v];
        for(int i=1; i<=n; i++) buf[i] += buf[i-1];
        res.E.resize(buf[n]);
        for(int u=0; u<n; u++) for(int v : operator[](u)) res.E[--buf[v]] = u;
        res.I = std::move(buf);
        return res;
    }
    AdjacencyList permuted(const std::vector<int>& perm) const {
        int n = num_vertices(), m = num_edges();
        assert((int)perm.size() == num_vertices());
        std::vector<int> newE(m), newI(n+1);
        for(int i=0; i<n; i++) newI[perm[i]+1] = I[i+1] - I[i];
        for(int i=0; i<n; i++) newI[i+1] += newI[i];
        for(int i=0; i<n; i++){
            int c = I[i+1] - I[i];
            for(int j=0; j<c; j++) newE[newI[perm[i]] + j] = perm[E[I[i]+j]];
        }
        return from_raw(std::move(newE), std::move(newI));
    }
};

} // namespace nachia
#line 2 "nachia\\graph\\adjacency-list-indexed.hpp"

#line 5 "nachia\\graph\\adjacency-list-indexed.hpp"

namespace nachia{

struct AdjacencyListEdgeIndexed{
public:
    struct Edge { int to; int edgeidx; };
    struct AdjacencyListRange{
        using iterator = typename std::vector<Edge>::const_iterator;
        iterator begi, endi;
        iterator begin() const { return begi; }
        iterator end() const { return endi; }
        int size() const { return (int)std::distance(begi, endi); }
        const Edge& operator[](int i) const { return begi[i]; }
    };
private:
    int mn;
    std::vector<Edge> E;
    std::vector<int> I;
public:
    AdjacencyListEdgeIndexed(int n, const std::vector<std::pair<int,int>>& edges, bool rev){
        mn = n;
        std::vector<int> buf(n+1, 0);
        for(auto [u,v] : edges){ ++buf[u]; if(rev) ++buf[v]; }
        for(int i=1; i<=n; i++) buf[i] += buf[i-1];
        E.resize(buf[n]);
        for(int i=(int)edges.size()-1; i>=0; i--){
            auto [u,v] = edges[i];
            E[--buf[u]] = { v, i };
            if(rev) E[--buf[v]] = { u, i };
        }
        I = std::move(buf);
    }
    AdjacencyListEdgeIndexed() : AdjacencyListEdgeIndexed(0, {}, false) {}
    AdjacencyListRange operator[](int u) const {
        return AdjacencyListRange{ E.begin() + I[u], E.begin() + I[u+1] };
    }
    int num_vertices() const { return mn; }
    int size() const { return num_vertices(); }
    int num_edges() const { return E.size(); }
    AdjacencyListEdgeIndexed reversed_edges() const {
        AdjacencyListEdgeIndexed res;
        int n = res.mn = mn;
        std::vector<int> buf(n+1, 0);
        for(auto [v,i] : E) ++buf[v];
        for(int i=1; i<=n; i++) buf[i] += buf[i-1];
        res.E.resize(buf[n]);
        for(int u=0; u<n; u++) for(auto [v,i] : operator[](u)) res.E[--buf[v]] = {u,i};
        res.I = std::move(buf);
        return res;
    }
};

} // namespace nachia
#line 5 "nachia\\graph\\maximum-cardinality-search.hpp"

#line 7 "nachia\\graph\\maximum-cardinality-search.hpp"
#include <algorithm>

namespace nachia{

// simple undirected graph
std::vector<int> MaximumCardinalitySearch(const AdjacencyList& adj){
    int n = adj.num_vertices();
    std::vector<int> res(n);
    std::vector<int> lp(n*2+1), rp(n*2+1);
    std::vector<int> idx(n, 0);
    for(int i=0; i<=n*2; i++) lp[i] = rp[i] = i;
    int li = n;
    auto Insert = [&](int i, int j){
        rp[lp[j]] = i;
        lp[i] = lp[j];
        lp[j] = i;
        rp[i] = j;
    };
    auto Erase = [&](int i){
        rp[lp[i]] = rp[i];
        lp[rp[i]] = lp[i];
    };
    for(int i=0; i<n; i++) Insert(i, n);
    for(int i=0; i<n; i++){
        li++;
        while(lp[li] == li) li--;
        int v = lp[li];
        idx[v] = -1;
        Erase(v);
        for(int nx : adj[v]) if(idx[nx] >= 0){ Erase(nx); Insert(nx, n+(++idx[nx])); }
        res[i] = v;
    }
    return res;
}

std::vector<int> AdjacencyQuery(
    const AdjacencyList& adj,
    const std::vector<std::pair<int,int>>& queries
){
    int n = adj.num_vertices(), q = queries.size();
    std::vector<int> res(q, 0);
    AdjacencyListEdgeIndexed qadj(n, queries, false);
    std::vector<int> buf(n, -1);
    for(int i=0; i<n; i++){
        for(int nx : adj[i]) buf[nx] = i;
        for(auto qi : qadj[i]) if(buf[qi.to] == i) res[qi.edgeidx] = 1;
    }
    return res;
}

std::vector<int> RecognizePerfectEliminationOrderling(
    const AdjacencyList& adj,
    const std::vector<int>& possiblePEO
){
    int n = adj.num_vertices();
    std::vector<int> invPEO(n);
    for(int i=0; i<n; i++) invPEO[possiblePEO[i]] = i;
    std::vector<int> pre(n, -1);
    AdjacencyList adj2 = adj.permuted(invPEO);
    for(int i=0; i<n; i++) for(int j : adj2[i]) if(j < i && pre[i] < j) pre[i] = j;
    std::vector<std::pair<int,int>> queries;
    std::vector<int> Z;
    for(int i=0; i<n; i++) if(pre[i] != -1){
        for(int j : adj2[i]) if(j < pre[i]){
            queries.push_back(std::make_pair(pre[i], j));
            Z.push_back(i);
        }
    }
    auto qres = AdjacencyQuery(adj2, queries);
    int x=-1, y=-1, z=-1;
    for(int i=0; i<(int)queries.size(); i++){
        if(!qres[i]){
            x = queries[i].second;
            y = queries[i].first;
            z = Z[i];
            break;
        }
    }
    if(z == -1) return {}; // it is PEO
    std::vector<int> dist(n, 0);
    std::vector<int> parent(n, -1);
    std::vector<int> bfs = {x,y};
    for(int inc : adj2[z]) parent[inc] = -2;
    dist[x] = -1; dist[y] = 1;
    parent[x] = parent[y] = z;
    int d = n+1, xx = -1, yy = -1;
    for(int i=0; i<(int)bfs.size(); i++){
        int p = bfs[i];
        for(int q : adj2[p]) if(q < p && parent[q] != -2){
            if(dist[p] < 0 && dist[q] > 0 && dist[q] - dist[p] < d){
                d = dist[q] - dist[p]; xx = p; yy = q;
            }
            else if(dist[p] > 0 && dist[q] < 0 && dist[p] - dist[q] < d){
                d = dist[p] - dist[q]; xx = q; yy = p;
            }
            else if(dist[q] == 0){
                dist[q] = dist[p] + ((dist[p] < 0) ? -1 : 1);
                parent[q] = p;
                bfs.push_back(q);
            }
        }
    }
    std::vector<int> res(d+1);
    int off = -dist[xx];
    res[off] = possiblePEO[z];
    do { res[dist[xx]+off] = possiblePEO[xx]; xx = parent[xx]; } while(xx != z);
    do { res[dist[yy]+off] = possiblePEO[yy]; yy = parent[yy]; } while(yy != z);
    return res;
}

} // namespace nachia
#line 4 "Main.cpp"

int main(){
    nachia::InputBufIterator iitr;
    nachia::OutputBufIterator oitr;
    int N = iitr.next_uint();
    int M = iitr.next_uint();
    std::vector<std::pair<int,int>> edges(M);
    for(int i=0; i<M; i++){
        edges[i].first = iitr.next_uint();
        edges[i].second = iitr.next_uint();
    }
    nachia::AdjacencyList adj(N, edges, true);
    auto mcs = nachia::MaximumCardinalitySearch(adj);
    auto cycle = nachia::RecognizePerfectEliminationOrderling(adj, mcs);
    std::reverse(mcs.begin(), mcs.end());
    
    if(cycle.empty()){
        oitr << "YES\n";
        for(int i=0; i<N; i++){
            if(i) oitr << ' ';
            oitr << mcs[i];
        }
        oitr << '\n';
    }
    else{
        oitr << "NO\n";
        oitr << cycle.size() << '\n';
        for(int i=0; i<(int)cycle.size(); i++){
            if(i) oitr << ' ';
            oitr << cycle[i];
        }
        oitr << '\n';
    }
    return 0;
}
