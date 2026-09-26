#define PROBLEM "https://judge.yosupo.jp/problem/min_plus_convolution_concave_arbitrary"
#include <vector>
#include <algorithm>

namespace nachia{

template<class Func, class Eval>
struct LiChaoTreeFlexible{
private:
    int xn;
    int N;
    std::vector<Func> V;
    std::vector<bool> visited;
    Func InfFunc;
    Eval ev;

    bool cmpat(Func fl, Func fr, int p){
        if(p >= xn) p = xn-1;
        return ev(fl, p) < ev(fr, p);
    }
public:
    
    LiChaoTreeFlexible(int n, Func inf, Eval eval)
        : xn(n)
        , InfFunc(std::move(inf))
        , ev(std::move(eval))
    {
        N = 1;
        while(N < n) N *= 2;
        V.assign(N*2, InfFunc);
        visited.assign(N*2, false);
    }
    
    void addSegment(int l, int r, Func f){
        if(l >= r) return;
        auto dfs = [&](int i,Func f,int a,int b,auto& dfs) -> void {
            visited[i] = true;
            if(i >= (int)V.size()) return;
            if(r <= a || b <= l) return;
            int m = (a+b)/2;
            if(!(l <= a && b <= r)){
                dfs(i*2,f,a,m,dfs);
                dfs(i*2+1,f,m,b,dfs);
                return;
            }
            if(cmpat(f, V[i], m)) std::swap(V[i],f);
            if(a + 1 == b) return;
            bool lessf_l = cmpat(f, V[i], a);
            bool lessf_r = cmpat(f, V[i], b-1);
            if(!lessf_l && !lessf_r) return;
            if(lessf_l) dfs(i*2,f,a,m,dfs);
            else dfs(i*2+1,f,m,b,dfs);
        };
        dfs(1,f,0,N,dfs);
    }
    
    void addLine(Func f){
        addSegment(0,N,f);
    }
    
    Func minFunc(int p){
        int i = 1;
        Func res = InfFunc;
        int l = 0, r = N;
        while(i < (int)V.size()){
            if(!visited[i]) break;
            if(cmpat(V[i], res, p)) res = V[i];
            int m = (l+r)/2;
            if(p < m){ i = i*2; r = m; }
            else{ i = i*2+1; l = m; }
        }
        return res;
    }
};

}
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

std::vector<int> MinPlusConvolution_AIsConcave(std::vector<int> A, std::vector<int> B){
    using namespace std;
    int N = (int)A.size();
    int M = (int)B.size();
    long long INF = 1001001001001;
    std::vector<int> C(N+M-1, 2002002002);
    for(int s=0; s<M; s+=N+1){
        int n = (M-s <= N) ? M-s : (N+1);
        auto ds1 = nachia::LiChaoTreeFlexible(N, 0,
            [&](int a, int b) -> long long {
                if(b < a) return -INF - a;
                return A[b-a] + B[s+a];
            });
        for(int i=0; i<N; i++){
            if(i+s<M) ds1.addLine(i);
            int k = ds1.minFunc(i);
            int fk = A[i-k] + B[s+k];
            if(fk < C[s+i]) C[s+i] = fk;
        }
        auto ds2 = nachia::LiChaoTreeFlexible(n-1, n-1,
            [&](int a, int b) -> long long {
                b += 1;
                if(a < b) return -INF + a;
                return A[N-1-(a-b)] + B[s+a];
            });
        for(int i=n-1; i>=1; i--){
            ds2.addLine(i);
            int k = ds2.minFunc(i-1);
            int p = s + (N-1) + i;
            int fk = A[p-s-k] + B[s+k];
            if(fk < C[p]) C[p] = fk;
        }
    }
    return C;
}

int main(){
    using nachia::cin;
    using nachia::cout;
    int N, M; cin >> N >> M;
    std::vector<int> A(N), B(M);
    for(auto& a : A) cin >> a;
    for(auto& b : B) cin >> b;
    auto C = MinPlusConvolution_AIsConcave(std::move(A), std::move(B));
    for(int i=0; i<N+M-1; i++){
        if(i) cout << " ";
        cout << C[i];
    } cout << "\n";
    return 0;
}
