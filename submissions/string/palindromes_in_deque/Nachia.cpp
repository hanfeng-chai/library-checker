#define PROBLEM "https://judge.yosupo.jp/problem/palindromes_in_deque"
#include <memory>
#include <map>
#include <cassert>
#include <vector>
#include <algorithm>
#include <iterator>

namespace nachia {

template<class Elem>
struct AmortizedDeque {
private:
    std::vector<Elem> l;
    std::vector<Elem> r;
    static void reset_halfway(std::vector<Elem>& a, std::vector<Elem>& b){
        auto m = (b.size() + 1) / 2;
        std::move(b.rend() - m, b.rend(), std::back_inserter(a));
        b.erase(b.begin(), b.begin() + m);
    }
public:
    void push_back(const Elem& x){ r.push_back(x); }
    void push_back(Elem&& x){ r.push_back(std::move(x)); }
    void push_front(const Elem& x){ l.push_back(x); }
    void push_front(Elem&& x){ l.push_back(std::move(x)); }
    Elem& operator[](std::size_t i){
        return i < l.size() ? l[l.size() - 1 - i] : r[i - l.size()];
    }
    const Elem& operator[](std::size_t i) const {
        return i < l.size() ? l[l.size() - 1 - i] : r[i - l.size()];
    }
    Elem& at(std::size_t i){
        return i < l.size() ? l.at(l.size() - 1 - i) : r.at(i - l.size());
    }
    const Elem& at(std::size_t i) const {
        return i < l.size() ? l.at(l.size() - 1 - i) : r.at(i - l.size());
    }
    std::size_t size(){ return l.size() + r.size(); }
    void pop_back(){
        if(r.empty()) reset_halfway(r, l);
        r.pop_back();
    }
    void pop_front(){
        if(l.empty()) reset_halfway(l, r);
        l.pop_back();
    }
    Elem& back(){ return r.empty() ? l.front() : r.back(); }
    const Elem& back() const { return r.empty() ? l.front() : r.back(); }
    Elem& front(){ return l.empty() ? r.front() : l.back(); }
    const Elem& front() const { return l.empty() ? r.front() : l.back(); }
    bool empty() const { return l.empty() && r.empty(); }
};

} // namespace nachia


namespace nachia{

template<class Char = int>
struct DequePalindromicTree {
private:
    struct Node {
        Node* parent;
        Node* link;
        Node* quick;
        std::map<Char, struct Node*> next;
        int len = 0;
        int cnt = 0;
        int linkcnt = 0;
    };
    struct DequeNode {
        Char ch;
        Node* presurf;
        Node* sufsurf;
    };

    AmortizedDeque<DequeNode> deq;
    std::unique_ptr<Node> Odd;
    std::unique_ptr<Node> Even;
    int numNodes;
    
    Node* backAppendable(Char c, Node* p){
        auto n = int(deq.size());
        while(true){
            if(p->len == -1 || (p->len < n && deq[n - p->len - 1].ch == c)) return p;
            auto q = p->link;
            if(q->len == -1 || deq[n - q->len - 1].ch == c) return q;
            p = p->quick;
        }
    }
    Node* frontAppendable(Char c, Node* p){
        auto n = int(deq.size());
        while(true){
            if(p->len == -1 || (p->len < n && deq[p->len].ch == c)) return p;
            auto q = p->link;
            if(q->len == -1 || deq[q->len].ch == c) return q;
            p = p->quick;
        }
    }
public:

    DequePalindromicTree(){
        Odd = std::make_unique<Node>();
        Odd->len = -1;
        Odd->parent = Odd->quick = Odd->link = Odd.get();
        Even = std::make_unique<Node>(*Odd);
        Even->len = 0;
        numNodes = 0;
    }

    ~DequePalindromicTree(){
        while(deq.size()) popBack();
    }

    void pushBack(Char ch){
        auto par = deq.empty() ? Odd.get() : backAppendable(ch, deq.back().sufsurf);
        auto n = int(deq.size());
        Node* v = nullptr;
        Node* w = Even.get();
        auto vit = par->next.find(ch);
        if(vit == par->next.end()){
            v = new Node();
            v->cnt = 0;
            v->linkcnt = 0;
            v->parent = par;
            v->len = par->len + 2;
            if(par != Odd.get()) w = backAppendable(ch, par->link)->next[ch];
            v->link = w;
            w->linkcnt += 1;
            deq.push_back(DequeNode{ ch, Even.get(), Even.get() });
            n += 1;
            if(w->link != Odd.get() && deq[n - w->len - 1].ch == deq[n - w->link->len - 1].ch){
                v->quick = w->quick;
            } else {
                v->quick = w->link;
            }
            par->next.insert(std::make_pair(ch, v));
            numNodes += 1;
        } else {
            deq.push_back(DequeNode{ ch, Even.get(), Even.get() });
            n += 1;
            v = vit->second;
            w = v->link;
        }
        deq[n-1].sufsurf = v;
        deq[n-v->len].presurf = v;
        if(w->len >= 1 && deq[n - v->len + w->len - 1].sufsurf == w){
            deq[n - v->len + w->len - 1].sufsurf = Even.get();
        }
        v->cnt += 1;
    }

    void pushFront(Char ch){
        auto par = deq.empty() ? Odd.get() : frontAppendable(ch, deq.front().presurf);
        auto n = int(deq.size());
        Node* v = nullptr;
        Node* w = Even.get();
        auto vit = par->next.find(ch);
        if(vit == par->next.end()){
            v = new Node();
            v->cnt = 0;
            v->linkcnt = 0;
            v->parent = par;
            v->len = par->len + 2;
            if(par != Odd.get()) w = frontAppendable(ch, par->link)->next[ch];
            v->link = w;
            w->linkcnt += 1;
            deq.push_front(DequeNode{ ch, Even.get(), Even.get() });
            n += 1;
            if(w->link != Odd.get() && deq[w->len].ch == deq[w->link->len].ch){
                v->quick = w->quick;
            } else {
                v->quick = w->link;
            }
            par->next.insert(std::make_pair(ch, v));
            numNodes += 1;
        } else {
            deq.push_front(DequeNode{ ch, Even.get(), Even.get() });
            n += 1;
            v = vit->second;
            w = v->link;
        }
        deq[0].presurf = v;
        deq[v->len - 1].sufsurf = v;
        if(w->len >= 1 && deq[v->len - w->len].presurf == w){
            deq[v->len - w->len].presurf = Even.get();
        }
        v->cnt += 1;
    }

    void popBack(){
        assert(stringLength() != 0);
        auto v = deq.back().sufsurf;
        int backChar = deq.back().ch;
        auto w = v->link;
        if(v->len >= 2 && deq[deq.size() - v->len + w->len - 1].sufsurf->len < w->len){
            deq[deq.size() - v->len + w->len - 1].sufsurf = w;
            deq[deq.size() - v->len].presurf = w;
        } else {
            deq[deq.size() - v->len].presurf = Even.get();
        }
        v->cnt -= 1;
        if(v->linkcnt == 0 && v->cnt == 0){
            v->parent->next.erase(backChar);
            w->linkcnt -= 1;
            delete v;
            numNodes -= 1;
        }
        deq.pop_back();
    }

    void popFront(){
        assert(stringLength() != 0);
        auto v = deq.front().presurf;
        int backChar = deq.front().ch;
        auto w = v->link;
        if(v->len >= 2 && deq[v->len - w->len].presurf->len < w->len){
            deq[v->len - w->len].presurf = w;
            deq[v->len - 1].sufsurf = w;
        } else {
            deq[v->len - 1].sufsurf = Even.get();
        }
        v->cnt -= 1;
        if(v->linkcnt == 0 && v->cnt == 0){
            v->parent->next.erase(backChar);
            w->linkcnt -= 1;
            delete v;
            numNodes -= 1;
        }
        deq.pop_front();
    }

    int stringLength(){ return int(deq.size()); }

    int numDistinctPalindromes(){ return numNodes; }
    int longestSuffixPalindrome(){ return stringLength() == 0 ? 0 : deq.back().sufsurf->len; }
    int longestPrefixPalindrome(){ return stringLength() == 0 ? 0 : deq.front().presurf->len; }
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
    int Q; cin >> Q;
    auto pal = nachia::DequePalindromicTree<int>();
    for(int qi=0; qi<Q; qi++){
        char t; cin >> t;
        if(t == '0'){
            char c; cin >> c;
            pal.pushFront(c);
        } else if(t == '1') {
            char c; cin >> c;
            pal.pushBack(c);
        } else if(t == '2') {
            pal.popFront();
        } else if(t == '3') {
            pal.popBack();
        }
        int x0 = pal.numDistinctPalindromes();
        int x1 = pal.longestPrefixPalindrome();
        int x2 = pal.longestSuffixPalindrome();
        cout << x0 << ' ' << x1 << ' ' << x2 << '\n';
    }
    return 0;
}
