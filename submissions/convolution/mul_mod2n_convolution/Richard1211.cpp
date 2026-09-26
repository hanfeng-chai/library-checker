//  ____                __                         __     _      ___       _     _     
// /\  _`\   __        /\ \                       /\ \  /' \   /'___`\   /' \  /' \    
// \ \ \L\ \/\_\    ___\ \ \___      __     _ __  \_\ \/\_, \ /\_\ /\ \ /\_, \/\_, \   
//  \ \ ,  /\/\ \  /'___\ \  _ `\  /'__`\  /\`'__\/'_` \/_/\ \\/_/// /__\/_/\ \/_/\ \  
//   \ \ \\ \\ \ \/\ \__/\ \ \ \ \/\ \L\.\_\ \ \//\ \L\ \ \ \ \  // /_\ \  \ \ \ \ \ \ 
//    \ \_\ \_\ \_\ \____\\ \_\ \_\ \__/.\_\\ \_\\ \___,_\ \ \_\/\______/   \ \_\ \ \_\
//     \/_/\/ /\/_/\/____/ \/_/\/_/\/__/\/_/ \/_/ \/__,_ /  \/_/\/_____/     \/_/  \/_/
/**************************************************
 * Fast Algorithm Template by Richard1211.
 * Please submit with C++14 or higher version.
 * Blog on Luogu: https://www.luogu.com.cn/blog/522539/
 * Blog on Byethost: http://Richard1211.byethost5.com/
 * Blog on RBTree's: https://Richard1211.rbtr.ee/
***************************************************/
//#pragma GCC optimize(3,"Ofast","inline")
//#pragma GCC target("avx2")
//open Ofast sometimes
#include <bits/stdc++.h>
#ifdef ONLINE_JUDGE
//#include <bits/extc++.h>
#else
//#include <ext/rope>
//#include <ext/pb_ds/assoc_container.hpp>
//#include <ext/pb_ds/tree_policy.hpp>
//#include <ext/pb_ds/hash_policy.hpp>
//#include <ext/pb_ds/trie_policy.hpp>
//#include <ext/pb_ds/priority_queue.hpp>
#endif
//#ifdef __linux__
//#include <sys/mman.h>
//#include <sys/types.h>
//#include <fcntl.h>
//#include <unistd.h>
//#else
//#endif
using namespace std;
//using namespace __gnu_cxx;
//using namespace __gnu_pbds;
#define YES io.writeln("YES")
#define Yes io.writeln("Yes")
#define yes io.writeln("yes")
#define NO io.writeln("NO")
#define No io.writeln("No")
#define no io.writeln("no")
#define Poss io.writeln("Possible")
#define poss io.writeln("possible")
#define Impo io.writeln("Impossible")
#define impo io.writeln("impossible")
#define Alice io.writeln("Alice")
#define Bob io.writeln("Bob")
#define Taka io.writeln("Takahashi")
#define Aoki io.writeln("Aoki")
#define PFir io.writeln("First")
#define Pfir io.writeln("first")
#define PSec io.writeln("Second")
#define Psec io.writeln("second")
#define RYES return YES,0
#define RYes return Yes,0
#define Ryes return yes,0
#define RNO return NO,0
#define RNo return No,0
#define Rno return no,0
#define RPoss return Poss,0
#define Rposs return poss,0
#define RImpo return Impo,0
#define Rimpo return impo,0
#define RAlice return Alice,0
#define RTaka return Taka,0
#define RAoki return Aoki,0
#define RBob return Bob,0
#define RFir return PFir,0
#define Rfir return Pfir,0
#define RSec return PSec,0
#define Rsec return Psec,0
#define pf emplace_front
#define pb emplace_back
#define ep emplace
#define ppb pop_back
#define ppf pop_front
#define gp make_pair
#define gt make_tuple
#define fr first
#define sd second
#define elif else if
#define safedo(x) do{x;}while(false)
#define likely(x) __builtin_expect(!!(x),1)
#define unlikely(x) __builtin_expect(!!(x),0)
#define PCase(i) (io.write("Case #"),wrt(i),io.write(": "),0)
#define LAM(G,...) auto G=[&](__VA_ARGS__)
#define LMT(T,G,...) auto G=[&](__VA_ARGS__)->T
#define LMD(T,G,...) auto G=[&](auto self,__VA_ARGS__)->T
#define sc(x) static_cast<long long>(x)
#define scu(x) static_cast<unsigned>(x)
#define scU(x) static_cast<unsigned long long>(x)
#define scd(x) static_cast<double>(x)
#define scD(x) static_cast<long double>(x)
#define scr(x) static_cast<char>(x)
#define sci(x) static_cast<int>(x)
#define GMX(T) numeric_limits<T>::max()
#define GMN(T) numeric_limits<T>::min()
#define GIN(T) numeric_limits<T>::infinity()
#define GES(T) numeric_limits<T>::epsilon()
#define ALL1(G) G.begin(),G.end()
#define ALL2(G,x) G.begin()+x,G.end()
#define ALL3(G,x,y) G.begin()+x,G.begin()+y
#define RALL1(G) G.rbegin(),G.rend()
#define RALL2(G,x) G.rbegin()+x,G.rend()
#define RALL3(G,x,y) G.rbegin()+x,G.rbegin()+y
#define ALLA(G,x,y) G+x,G+y+1
#define SIZ(G) sc(G.size())
#define SOR(...) sort(ALL(__VA_ARGS__))
#define REV(...) reverse(ALL(__VA_ARGS__))
#define MIN(...) *min_element(ALL(__VA_ARGS__))
#define MAX(...) *max_element(ALL(__VA_ARGS__))
#define SUM(T,...) accumulate(ALL(__VA_ARGS__),static_cast<T>(0))
#define LB(x,...) lower_bound(ALL(__VA_ARGS__),x)
#define UB(x,...) upper_bound(ALL(__VA_ARGS__),x)
#define LBG(x,...) lower_bound(ALL(__VA_ARGS__),x,greater{})
#define UBG(x,...) upper_bound(ALL(__VA_ARGS__),x,greater{})
#define FL(x,...) fill(ALL(__VA_ARGS__),x)
#define IOT(x,...) iota(ALL(__VA_ARGS__),x)
#define SIZ(G) sc(G.size())
#define SORA(G,x,y) sort(ALLA(G,x,y))
#define REVA(G,x,y) reverse(ALLA(G,x,y))
#define MINA(G,x,y) *min_element(ALLA(G,x,y))
#define MAXA(G,x,y) *max_element(ALLA(G,x,y))
#define SUMA(T,G,x,y) accumulate(ALLA(G,x,y),static_cast<T>(0))
#define LBA(G,x,y,v) lower_bound(ALLA(G,x,y),v)
#define UBA(G,x,y,v) upper_bound(ALLA(G,x,y),v)
#define LBGA(G,x,y,v) lower_bound(ALLA(G,x,y),v,greater{})
#define UBGA(G,x,y,v) upper_bound(ALLA(G,x,y),v,greater{})
#define FLA(G,x,y,v) fill(ALLA(G,x,y),v)
#define IOTA(G,x,y,v) iota(ALLA(G,x,y),v)
#define MEMS(G,v) memset(G,v,sizeof(G))
#define MEMSV(G,v,s) memset(G,v,s)
#define UNQ(G,x)                                            \
sort(ALL(G,x));                                             \
G.erase(unique(ALL(G,x)),G.end());
#define UNQA(G,x,y)											\
sort(ALLA(G,x,y));											\
register long long LEN=unique(ALLA(G,x,y))-G-1;
#define container_of(ptr,type,member)({const typeof(((type*)0)->member)*__mptr=(ptr);\
										(type*)((char*)__mptr-offsetof(type,member));})
#define Forz() while(true)
#define For1(a) for(register long long i=1,i##_r=(a);i<=i##_r;++i)
#define For2(i,a) for(register long long i=1,i##_r=(a);i<=i##_r;++i)
#define For3(i,a,b) for(register long long i=(a),i##_r=(b);i<=i##_r;++i)
#define For4(i,a,b,c) for(register long long i=(a),i##_r=(b);i<=i##_r;i+=(c))
#define FoR1(a) for(register long long i=(a);i>=1;--i)
#define FoR2(i,a) for(register long long i=(a);i>=1;--i)
#define FoR3(i,a,b) for(register long long i=(a),i##_r=(b);i>=i##_r;--i)
#define FoR4(i,a,b,c) for(register long long i=(a),i##_r=(b);i>=i##_r;i-=(c))
#define For01(a) for(register long long i=0,i##_r=(a);i<i##_r;++i)
#define For02(i,a) for(register long long i=0,i##_r=(a);i<i##_r;++i)
#define For03(i,a,b) for(register long long i=(a),i##_r=(b);i<i##_r;++i)
#define For04(i,a,b,c) for(register long long i=(a),i##_r=(b);i<i##_r;i+=(c))
#define FoR01(a) for(register long long i=(a)-1;i>=0;--i)
#define FoR02(i,a) for(register long long i=(a)-1;i>=0;--i)
#define FoR03(i,a,b) for(register long long i=(a)-1,i##_r=(b);i>=i##_r;--i)
#define FoR04(i,a,b,c) for(register long long i=(a)-1,i##_r=(b);i>=i##_r;i-=(c))
#define ForN1(a) for(register long long i=1;i<=(a);++i)
#define ForN2(i,a) for(register long long i=1;i<=(a);++i)
#define ForN3(i,a,b) for(register long long i=(a);i<=(b);++i)
#define ForN4(i,a,b,c) for(register long long i=(a);i<=(b);i+=(c))
#define FoRN1(a) for(register long long i=(a);i>=1;--i)
#define FoRN2(i,a) for(register long long i=(a);i>=1;--i)
#define FoRN3(i,a,b) for(register long long i=(a);i>=(b);--i)
#define FoRN4(i,a,b,c) for(register long long i=(a);i>=(b);i-=(c))
#define ForN01(a) for(register long long i=0;i<(a);++i)
#define ForN02(i,a) for(register long long i=0;i<(a);++i)
#define ForN03(i,a,b) for(register long long i=(a);i<(b);++i)
#define ForN04(i,a,b,c) for(register long long i=(a);i<(b);i+=(c))
#define FoRN01(a) for(register long long i=(a)-1;i>=0;--i)
#define FoRN02(i,a) for(register long long i=(a)-1;i>=0;--i)
#define FoRN03(i,a,b) for(register long long i=(a)-1;i>=(b);--i)
#define FoRN04(i,a,b,c) for(register long long i=(a)-1;i>=(b);i-=(c))
#define For1E(i,a) for(auto &&i:a)
#define For2E(x,y,a) for(auto &&[x,y]:a)
#define For3E(x,y,z,a) for(auto &&[x,y,z]:a)
#define For4E(x,y,z,w,a) for(auto &&[x,y,z,w]:a)
#define For1EG(u) for(register long long i=head[u],to=edge[i].to;i;i=edge[i].nxt,to=edge[i].to)
#define For2EG(i,u) for(register long long i=head[u],to=edge[i].to;i;i=edge[i].nxt,to=edge[i].to)
#define For3EG(i,u,w) for(register long long i=head[u],to=edge[i].to,w=edge[i].w;i;i=edge[i].nxt,to=edge[i].to,w=edge[i].w)
#define For4EG(i,u,w,c) for(register long long i=head[u],to=edge[i].to,w=edge[i].w,c=edge[i].c;i;i=edge[i].nxt,to=edge[i].to,w=edge[i].w,c=edge[i].c)
#define For1EGW(u) for(register long long i=head[u],to=edge[i].to;i!=-1;i=edge[i].nxt,to=edge[i].to)
#define For2EGW(i,u) for(register long long i=head[u],to=edge[i].to;i!=-1;i=edge[i].nxt,to=edge[i].to)
#define For3EGW(i,u,x) for(register long long i=head[u],to=edge[i].to;i!=x;i=edge[i].nxt,to=edge[i].to)
#define For4EGW(i,u,w,x) for(register long long i=head[u],to=edge[i].to,w=edge[i].w;i!=x;i=edge[i].nxt,to=edge[i].to,w=edge[i].w)
#define OverLoaD(a,b,c,d,e,f,...) f
#define OverLoad(a,b,c,d,e,...) e
#define Overload(a,b,c,d,...) d
#define For(...) OverLoaD(ForTang,##__VA_ARGS__,For4,For3,For2,For1,Forz)(__VA_ARGS__)
#define FoR(...) OverLoad(__VA_ARGS__,FoR4,FoR3,FoR2,FoR1)(__VA_ARGS__)
#define For0(...) OverLoad(__VA_ARGS__,For04,For03,For02,For01)(__VA_ARGS__)
#define FoR0(...) OverLoad(__VA_ARGS__,FoR04,FoR03,FoR02,FoR01)(__VA_ARGS__)
#define ForN(...) OverLoad(__VA_ARGS__,ForN4,ForN3,ForN2,ForN1)(__VA_ARGS__)
#define FoRN(...) OverLoad(__VA_ARGS__,FoRN4,FoRN3,FoRN2,FoRN1)(__VA_ARGS__)
#define ForN0(...) OverLoad(__VA_ARGS__,ForN04,ForN03,ForN02,ForN01)(__VA_ARGS__)
#define FoRN0(...) OverLoad(__VA_ARGS__,FoRN04,FoRN03,FoRN02,FoRN01)(__VA_ARGS__)
#define ForE(...) OverLoad(__VA_ARGS__,For3E,For2E,For1E)(__VA_ARGS__)
#define ForEG(...) OverLoad(__VA_ARGS__,For4EG,For3EG,For2EG,For1EG)(__VA_ARGS__)
#define ForEGW(...) OverLoad(__VA_ARGS__,For4EGW,For3EGW,For2EGW,For1EGW)(__VA_ARGS__)
#define ALL(...) Overload(__VA_ARGS__,ALL3,ALL2,ALL1)(__VA_ARGS__)
#define RALL(...) Overload(__VA_ARGS__,RALL3,RALL2,RALL1)(__VA_ARGS__)
#define ForPR(...) for(bool flag=true;flag?exchange(flag,false):next_permutation(ALL(__VA_ARGS__));)
#define ForPRA(G,x,y) for(bool flag=true;flag?exchange(flag,false):next_permutation(G+x,G+y+1);)
#define ForSubset(msk,s)                                    \
for(register long long msk=(s);msk;msk&=msk-1)
#define Inline __inline__ __attribute__ ((always_inline))
#define Test()                                              \
register int TestCount=MultiCase?read():1;                  \
for(register int TestCase=1;TestCase<=TestCount;++TestCase)
#define Tesf() for(register int TestCase=1;TestCase<=t;++TestCase)
#define RD(...) register long long __VA_ARGS__;io.read(__VA_ARGS__)
#define RS(...) string __VA_ARGS__;io.read(__VA_ARGS__)
#define RC(...) register char __VA_ARGS__;io.read(__VA_ARGS__)
#define RI(...) register int __VA_ARGS__;io.read(__VA_ARGS__)
#define RU32(...) register unsigned __VA_ARGS__;io.read(__VA_ARGS__)
#define RU64(...) register unsigned long long __VA_ARGS__;io.read(__VA_ARGS__)
#define RF32(...) register double __VA_ARGS__;io.read(__VA_ARGS__)
#define RF64(...) register long double __VA_ARGS__;io.read(__VA_ARGS__)
#define RSP(T,...) register T __VA_ARGS__;readsp(__VA_ARGS__)
#define RV(G,n)                                             \
vector<long long>G(n+1);                                    \
For(n){                                                     \
	G[i]=read();                                            \
}
#define RVV(G,n,m)                                          \
vector<vector<long long>>G(n+1,vector<long long>(m+1));     \
For(n){                                                     \
	For(j,m){                                               \
		G[i][j]=read();                                     \
	}                                                       \
}
#ifdef ONLINE_JUDGE
//when on OJ,close the debug version
//hope code with no bugs.
//think twice,code once.
//timer won't print anything
//don't be TLE or MLE,only AC.
#if __cplusplus>202002L
#undef Tovec
#undef Debug
#undef DebugA
#undef Setmemory
#define Tovec(...) 42
#define Debug(...) 42
#define DebugA(...) 42
#define Setmemory(tmp) 0
#else
#undef Debug
#undef Setmemory
#define Debug(...) 42
#define Setmemory(tmp) 0
#endif
#if __cplusplus>201402L
#define register
#else
//becaus of using c++17 and higher version, register are not allowed to use.
#endif
#else
//open debug version.
//hope code with no bugs.
//because of using c++17, we need to undefine register.
//Debug will print out the things we need.
//Setmemory will set the end memory we need.
#define register
#define Setmemory(Tmp) (Timebot::timer.End=&Tmp,0)
#if __cplusplus>202002L
namespace pretty_print{
namespace detail{
struct sfinae_base{using yyes=char;using nno=yyes[2];};
template<typename T>struct has_const_iterator:private sfinae_base{private:
    template<typename C>static yyes&test(typename C::const_iterator*);
    template<typename C> static nno&test(...);public:
    static const bool value=sizeof(test<T>(nullptr))==sizeof(yyes);using type=T;
};
template<typename T>struct has_begin_end:private sfinae_base{private:
    template<typename C>static yyes&f(typename std::enable_if<std::is_same
    <decltype(static_cast<typename C::const_iterator(C::*)()const>(&C::begin)),
    typename C::const_iterator(C::*)()const>::value>::type*);template<typename C>static nno&f(...);
    template <typename C>static yyes&g(typename std::enable_if<std::is_same
    <decltype(static_cast<typename C::const_iterator(C::*)()const>(&C::end)),
    typename C::const_iterator(C::*)()const>::value,void>::type*);template<typename C>static nno&g(...);public:
    static bool const beg_value=sizeof(f<T>(nullptr))==sizeof(yyes);
    static bool const end_value=sizeof(g<T>(nullptr))==sizeof(yyes);};
}
template<typename TChar>struct delimiters_values{using char_type=TChar;const char_type*prefix;const char_type*delimiter;const char_type*postfix;};
template<typename T,typename TChar>struct delimiters{using type=delimiters_values<TChar>;static const type values;};
template <typename T,typename TChar=char,typename TCharTraits=::std::char_traits<TChar>,typename TDelimiters=delimiters<T,TChar>>struct print_container_helper{
    using delimiters_type=TDelimiters;
    using ostream_type=std::basic_ostream<TChar,TCharTraits>;
    template<typename U>struct printer{
        static void print_body(const U&c,ostream_type&stream){
            using std::begin;using std::end;auto it=begin(c);const auto the_end=end(c);
            if(it!=the_end){while(true){stream<<*it;if(++it==the_end)break;if(delimiters_type::values.delimiter!=NULL)stream<<delimiters_type::values.delimiter;}}
        }
    };
    print_container_helper(const T&container):container_(container){}
    inline void operator()(ostream_type&stream)const{
        if(delimiters_type::values.prefix!=NULL)stream<<delimiters_type::values.prefix;printer<T>::print_body(container_,stream);
        if(delimiters_type::values.postfix != NULL)stream<<delimiters_type::values.postfix;
    }private:const T&container_;
};
template<typename T,typename TChar,typename TCharTraits,typename TDelimiters>template<typename T1,typename T2>struct print_container_helper<T,TChar,TCharTraits,TDelimiters>::printer<std::pair<T1,T2>>{
    using ostream_type=typename print_container_helper<T,TChar,TCharTraits,TDelimiters>::ostream_type;
    static void print_body(const std::pair<T1,T2>&c,ostream_type&stream){stream<<c.first;
    if(print_container_helper<T,TChar,TCharTraits,TDelimiters>::delimiters_type::values.delimiter!=NULL)
    stream<<print_container_helper<T,TChar,TCharTraits,TDelimiters>::delimiters_type::values.delimiter;stream<<c.second;}
};
template<typename T,typename TChar,typename TCharTraits,typename TDelimiters>template<typename...Args>struct print_container_helper<T,TChar,TCharTraits,TDelimiters>::printer<std::tuple<Args...>>{
    using ostream_type=typename print_container_helper<T,TChar,TCharTraits,TDelimiters>::ostream_type;using element_type=std::tuple<Args...>;template<std::size_t I>struct Int{};
    static void print_body(const element_type&c,ostream_type&stream){tuple_print(c,stream,Int<0>());}
    static void tuple_print(const element_type&,ostream_type&,Int<sizeof...(Args)>){}
    static void tuple_print(const element_type&c,ostream_type&stream,typename std::conditional<sizeof...(Args)!=0,Int<0>,std::nullptr_t>::type){stream<<std::get<0>(c);tuple_print(c,stream,Int<1>());}
    template<std::size_t N>static void tuple_print(const element_type&c,ostream_type&stream,Int<N>){
    if(print_container_helper<T,TChar,TCharTraits,TDelimiters>::delimiters_type::values.delimiter!=NULL)
    stream<<print_container_helper<T,TChar,TCharTraits,TDelimiters>::delimiters_type::values.delimiter;stream<<std::get<N>(c);tuple_print(c,stream,Int<N+1>());}
};
template<typename T,typename TChar,typename TCharTraits,typename TDelimiters>inline std::basic_ostream<TChar,TCharTraits>&operator<<
(std::basic_ostream<TChar,TCharTraits>&stream,const print_container_helper<T,TChar,TCharTraits,TDelimiters>&helper){helper(stream);return stream;}
template<typename T>struct is_container:public std::integral_constant<bool,detail::has_const_iterator<T>::value&&detail::has_begin_end<T>::beg_value&&detail::has_begin_end<T>::end_value>{};
template<typename T,std::size_t N>struct is_container<T[N]>:std::true_type{};
template<std::size_t N>struct is_container<char[N]>:std::false_type{};
template<typename T>struct is_container<std::valarray<T>>:std::true_type{};
template<typename T1,typename T2>struct is_container<std::pair<T1,T2>>:std::true_type{};
template<typename...Args>struct is_container<std::tuple<Args...>>:std::true_type{};
template<typename T>struct delimiters<T,char>{static const delimiters_values<char>values;};
template<typename T>const delimiters_values<char>delimiters<T,char>::values={"[",", ","]"};
template<typename T>struct delimiters<T,wchar_t>{static const delimiters_values<wchar_t>values;};
template<typename T>const delimiters_values<wchar_t>delimiters<T,wchar_t>::values={ L"[", L", ", L"]"};
template<typename T,typename TComp,typename TAllocator>struct delimiters<::std::set<T,TComp,TAllocator>,char>{static const delimiters_values<char>values;};
template<typename T,typename TComp,typename TAllocator>const delimiters_values<char>delimiters<::std::set<T,TComp,TAllocator>,char>::values={"{",", ","}"};
template<typename T,typename TComp,typename TAllocator>struct delimiters<::std::set<T,TComp,TAllocator>,wchar_t>{static const delimiters_values<wchar_t>values;};
template<typename T,typename TComp,typename TAllocator>const delimiters_values<wchar_t>delimiters<::std::set<T,TComp,TAllocator>,wchar_t>::values={ L"{", L", ", L"}"};
template<typename T,typename TComp,typename TAllocator>struct delimiters<::std::multiset<T,TComp,TAllocator>,char>{static const delimiters_values<char>values;};
template<typename T,typename TComp,typename TAllocator>const delimiters_values<char>delimiters<::std::multiset<T,TComp,TAllocator>,char>::values={"{",", ","}" };
template<typename T,typename TComp,typename TAllocator>struct delimiters<::std::multiset<T,TComp,TAllocator>,wchar_t>{static const delimiters_values<wchar_t>values;};
template<typename T,typename TComp,typename TAllocator>const delimiters_values<wchar_t>delimiters<::std::multiset<T,TComp,TAllocator>,wchar_t>::values={ L"{", L", ", L"}"};
template<typename T,typename THash,typename TEqual,typename TAllocator>struct delimiters<::std::unordered_set<T,THash,TEqual,TAllocator>,char>{static const delimiters_values<char>values;};
template<typename T,typename THash,typename TEqual,typename TAllocator>const delimiters_values<char> delimiters<::std::unordered_set<T,THash,TEqual,TAllocator>,char>::values={"{",", ","}"};
template<typename T,typename THash,typename TEqual,typename TAllocator>struct delimiters<::std::unordered_set<T,THash,TEqual,TAllocator>,wchar_t>{static const delimiters_values<wchar_t>values;};
template<typename T,typename THash,typename TEqual,typename TAllocator>const delimiters_values<wchar_t>delimiters<::std::unordered_set<T,THash,TEqual,TAllocator>,wchar_t>::values={ L"{", L", ", L"}"};
template<typename T,typename THash,typename TEqual,typename TAllocator>struct delimiters<::std::unordered_multiset<T,THash,TEqual,TAllocator>,char>{static const delimiters_values<char>values;};
template<typename T,typename THash,typename TEqual,typename TAllocator>const delimiters_values<char>delimiters<::std::unordered_multiset<T,THash,TEqual,TAllocator>,char>::values={"{",", ","}"};
template<typename T,typename THash,typename TEqual,typename TAllocator>struct delimiters<::std::unordered_multiset<T,THash,TEqual,TAllocator>,wchar_t>{static const delimiters_values<wchar_t>values;};
template<typename T,typename THash,typename TEqual,typename TAllocator>const delimiters_values<wchar_t>delimiters<::std::unordered_multiset<T,THash,TEqual,TAllocator>,wchar_t>::values={ L"{", L", ", L"}" };
template<typename T1,typename T2>struct delimiters<std::pair<T1,T2>,char>{static const delimiters_values<char>values;};
template<typename T1,typename T2>const delimiters_values<char>delimiters<std::pair<T1,T2>,char>::values={"(",", ",")"};
template<typename T1,typename T2>struct delimiters<::std::pair<T1,T2>,wchar_t>{static const delimiters_values<wchar_t>values;};
template<typename T1,typename T2>const delimiters_values<wchar_t>delimiters<::std::pair<T1, T2>,wchar_t>::values={ L"(", L", ", L")"};
template<typename...Args>struct delimiters<std::tuple<Args...>,char>{static const delimiters_values<char>values;};
template<typename...Args>const delimiters_values<char>delimiters<std::tuple<Args...>,char>::values={"(",", ",")"};
template<typename...Args>struct delimiters<::std::tuple<Args...>,wchar_t>{static const delimiters_values<wchar_t>values;};
template<typename ...Args>const delimiters_values<wchar_t>delimiters<::std::tuple<Args...>,wchar_t>::values={ L"(", L", ", L")" };
struct custom_delims_base{virtual ~custom_delims_base(){}virtual std::ostream&stream(::std::ostream&)=0;virtual std::wostream&stream(::std::wostream&)=0;};
template<typename T,typename Delims>struct custom_delims_wrapper:custom_delims_base{
    custom_delims_wrapper(const T&t_):t(t_){}
    std::ostream&stream(std::ostream&s){return s<<print_container_helper<T,char,std::char_traits<char>,Delims>(t);}
    std::wostream&stream(std::wostream&s){return s<<print_container_helper<T,wchar_t,std::char_traits<wchar_t>,Delims>(t);}private:const T&t;
};
template<typename Delims>struct custom_delims{template<typename Container>custom_delims(const Container&c):base(new custom_delims_wrapper<Container,Delims>(c)){}std::unique_ptr<custom_delims_base>base;};
template<typename TChar,typename TCharTraits,typename Delims>inline std::basic_ostream<TChar,TCharTraits>&operator<<(std::basic_ostream<TChar,TCharTraits>&s,const custom_delims<Delims>&p){return p.base->stream(s);}
template<typename T>struct array_wrapper_n{
    typedef const T*const_iterator;typedef T value_type;
    array_wrapper_n(const T*const a,size_t n):_array(a),_n(n){}
    inline const_iterator begin()const{return _array;}
    inline const_iterator end()const{return _array+_n;}private:const T*const _array;size_t _n;
};
template <typename T>struct bucket_print_wrapper{
    typedef typename T::const_local_iterator const_iterator;typedef typename T::size_type size_type;
    const_iterator begin()const{return m_map.cbegin(n);}
    const_iterator end()const{return m_map.cend(n);}
    bucket_print_wrapper(const T&m,size_type bucket):m_map(m),n(bucket){}private:const T&m_map;const size_type n;
};}
template<typename T>inline pretty_print::array_wrapper_n<T>pretty_print_array(const T*const a,size_t n){return pretty_print::array_wrapper_n<T>(a,n);}
template<typename T>pretty_print::bucket_print_wrapper<T>bucket_print(const T&m,typename T::size_type n){return pretty_print::bucket_print_wrapper<T>(m,n);}
namespace std{
template<typename T,typename TChar,typename TCharTraits>
inline typename enable_if<::pretty_print::is_container<T>::value,basic_ostream<TChar,TCharTraits>&>::type operator<<
(basic_ostream<TChar,TCharTraits>&stream,const T&container){return stream<<::pretty_print::print_container_helper<T,TChar,TCharTraits>(container);}}
#ifndef INDENT_SYMBOL
#define INDENT_SYMBOL "    "
#endif
#ifndef DELIM_SYMBOL
#define DELIM_SYMBOL ", "
#endif
namespace PrettyDebug{
void prettyDebugCheck(int curLine,size_t curAddress){
    static int lastLine;static std::stack<size_t>st;bool newline=false;
    if(lastLine!=curLine)lastLine=curLine,newline=true;
    if(curAddress!=0){while(st.size()&&(curAddress>st.top()))st.pop(),newline=true;
    if(st.empty()||curAddress!=st.top())st.push(curAddress),newline=true;
    if(newline)std::cerr<<'\n';for(int i=1;i<int(st.size());i++)std::cerr<<INDENT_SYMBOL;}std::cerr<<"(Line "<<curLine<<"): ";
}
#define prettyDebugPre(line)prettyDebugCheck(line,size_t(__builtin_frame_address(0)))
#define prettyDebugDelim()std::cerr<<DELIM_SYMBOL
void prettyPrint(std::string_view,int){std::cerr<<'\n';}
template<typename T,typename...Args>void prettyPrint(std::string_view allNames,int line,const T&x,const Args&...args){
    std::string_view name;bool inBrackets=false;for(std::size_t i=0;i<allNames.size();i++){
    char c=allNames[i];if(std::string("[]{}()").find(c)!=std::string::npos)inBrackets^=1;if(c==','&&!inBrackets){name=allNames.substr(0,i);break;}}
    if(name.empty())name=allNames;else allNames=allNames.substr(name.size()+1);if(line!=-1)prettyDebugPre(line);else prettyDebugDelim();std::cerr<<name<<" = "<<x;prettyPrint(allNames,-1,args...);
}
template<typename...Args>inline void unusedDef(Args&&...){}}
template<typename Iter>auto Tovec(Iter begin,Iter end){return std::basic_string_view<std::decay_t<decltype(*begin)>>(begin,end-begin);}
#ifndef DEBUG_OFF
#define Debug(...) PrettyDebug::prettyPrint(std::string_view(#__VA_ARGS__),__LINE__,__VA_ARGS__)
#define DebugA(a,l,r) PrettyDebug::prettyPrint(#a,__LINE__,std::basic_string_view<std::decay_t<decltype(*a)>>(a+l,r-l+1))
#else
#define Debug(...) PrettyDebug::unusedDef(__VA_ARGS__)
#define DebugA(...) PrettyDebug::unusedDef(__VA_ARGS__)
#endif
#else
#define Debug(...) cerr << "Debug " << "[" << #__VA_ARGS__ << "]:", debug_out(__VA_ARGS__)
template<typename A,typename B>string to_string(pair<A,B>p);
template<typename A,typename B,typename C>string to_string(tuple<A,B,C>p);
template<typename A,typename B,typename C,typename D>string to_string(tuple<A,B,C,D>p);
string to_string(const string&s){return'"'+s+'"';}
string to_string(const char*s){return to_string((string)s);}
string to_string(bool b){return(b?"true":"false");}
string to_string(vector<bool>v){bool first=true;string res="{";for(int i=0;i<static_cast<int>(v.size());i++){if(!first){res+=", ";}first=false;res+=to_string(v[i]);}res+="}";return res;}
template<size_t N>string to_string(bitset<N>v){string res="";for(size_t i=0;i<N;i++){res+=static_cast<char>('0'+v[i]);}return res;}
template<typename A>string to_string(A v){bool first=true;string res="{";for(const auto&x:v){if(!first){res+=", ";}first=false;res+=to_string(x);}res+="}";return res;}
template<typename A,typename B>string to_string(pair<A,B>p){return"("+to_string(p.first)+", "+to_string(p.second)+")";}
template<typename A,typename B,typename C>string to_string(tuple<A,B,C>p){return"("+to_string(get<0>(p))+", "+to_string(get<1>(p))+", "+to_string(get<2>(p))+")";}
template<typename A,typename B,typename C,typename D>string to_string(tuple<A,B,C,D>p){return"("+to_string(get<0>(p))+", "+to_string(get<1>(p))+", "+to_string(get<2>(p))+", "+to_string(get<3>(p))+")";}
inline void debug_out(){cerr<<endl;}
template<typename Head,typename...Tail>void debug_out(Head H,Tail...T){cerr<<" "<<to_string(H);debug_out(T...);}
#endif
namespace Timebot{
	//timer will print the time and the memory of the code for you.
	//don't be TLE or MLE,only AC.
	struct Timer{
		bool f,*Beg,*End;clock_t Begin;Timer():Begin(clock()),Beg(&f){}~Timer(){double t=(clock()-Begin)*1000./CLOCKS_PER_SEC;t>=60000?fprintf(stderr,"Time: %.2lf min\n",t/60000.):t>=1000?fprintf(stderr,"Time: %.2lf s\n",t/1000.):fprintf(stderr,"Time: %.0lf ms\n",t);fprintf(stderr,"Memory: %.3lf MB\n",(End-Beg)/1048576.0);}
	}timer;
}
#endif
[[maybe_unused]]constexpr double pi=3.1415926535897932384626433832795,ei=2.7182818284590452353602874713527;
[[maybe_unused]]constexpr long long ZR=0,OE=1;//ignore error
[[maybe_unused]]constexpr long long NJ=10,NP3=14,NP2=20;//O(n!),O(3^n),O(2^n)
[[maybe_unused]]constexpr long long N31=110,N32=150,N33=210;//O(n^3)
[[maybe_unused]]constexpr long long N21=1010,N22=3030,N23=5050;//O(n^2)
[[maybe_unused]]constexpr long long N11=100100,N12=300300,N13=500500,N14=700700;//O(n)
[[maybe_unused]]constexpr long long NB1=1000100,NB2=3000300,NB3=5000500,NB4=7000700,NB5=10001000;//O(n)
[[maybe_unused]]constexpr long long USEMOD[]={0,20091119,11190119,121911211,998244353,19260817,1000000007,1145141};
[[maybe_unused]]constexpr __int128 PW10[]={1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000,10000000000,100000000000,1000000000000,10000000000000,100000000000000,1000000000000000,10000000000000000,100000000000000000,1000000000000000000,__int128(1000000000000000000)*10,__int128(1000000000000000000)*100,__int128(1000000000000000000)*1000,__int128(1000000000000000000)*10000,__int128(1000000000000000000)*100000,__int128(1000000000000000000)*1000000,__int128(1000000000000000000)*10000000,
__int128(1000000000000000000)*100000000,__int128(1000000000000000000)*1000000000,__int128(1000000000000000000)*10000000000,__int128(1000000000000000000)*100000000000,__int128(1000000000000000000)*1000000000000,__int128(1000000000000000000)*1000000000000,__int128(1000000000000000000)*100000000000000,__int128(1000000000000000000)*1000000000000000,__int128(1000000000000000000)*10000000000000000,__int128(1000000000000000000)*100000000000000000,__int128(1000000000000000000)*1000000000000000000};
template<class T>constexpr T Infty=0;
template<>constexpr int Infty<int> =1000000000;
template<>constexpr long long Infty<long long> =2000000000000000000;
template<>constexpr unsigned Infty<unsigned> =1000000000;
template<>constexpr unsigned long long Infty<unsigned long long> =2000000000000000000;
template<>constexpr __int128 Infty<__int128> =__int128(Infty<long long>)*Infty<long long>;
template<>constexpr long double Infty<long double> =2e18;
template<>constexpr __float128 Infty<__float128> =2e18;
template<>constexpr double Infty<double> =2e18;
template<class T>Inline T Min(register T a,register T b){return a<b?a:b;}
template<class T>Inline T Max(register T a,register T b){return a>b?a:b;}
template<class T>inline T Min(initializer_list<T>a){register T x=numeric_limits<T>::max();for(auto &&it:a){x=Min(x,it);}return x;}
template<class T>inline T Max(initializer_list<T>a){register T x=numeric_limits<T>::min();for(auto &&it:a){x=Max(x,it);}return x;}
template<class T,class Q>Inline bool Cmin(register T&a,register Q b){return a>b?(a=b,true):false;}
template<class T,class Q>Inline bool Cmax(register T&a,register Q b){return a<b?(a=b,true):false;}
template<class T,class Q>inline bool Cmin(register T&a,initializer_list<Q>b){register T x=a;for(auto &&it:b){Cmin(a,it);}return x==a?false:true;}
template<class T,class Q>inline bool Cmax(register T&a,initializer_list<Q>b){register T x=a;for(auto &&it:b){Cmax(a,it);}return x==a?false:true;}
template<class T>Inline T Abs(register T a){return a>0?a:-a;}
template<class T>inline T Floor(register T a,register T b){return a/b-(a%b&&(a^b)<0);}
template<class T>inline T Ceil(register T a,register T b){return Floor(a+b-1,b);}
template<class T>inline T Bmod(register T a,register T b){return a-b*Floor(a,b);}
template<class T>inline pair<T,T>Divmod(register T a,register T b){register T q=Floor(a,b);return gp(q,a-q*b);}
template<class T>Inline T Lowbit(register T x){return (x&-x);}
template<class T>Inline T Topbit(register T x){return 63-__builtin_clzll(x);}
template<class T>Inline void Swap(register T&a,register T&b){return a!=b?b^=a^=b^=a,void():void();}
template<class T>inline T Gcd(T a,T b){return b?Gcd(b,a%b):a;}
template<class T>inline T Lcm(T a,T b){return a*b/Gcd(a,b);}
template<class T>inline T Exgcd(T a,T b,T&x,T&y){if(b==0){x=1;y=0;return a;}T d=Exgcd(b,a%b,x,y);T t=x;x=y;y=t-a/b*y;return d;}
template<class T>inline T ExExgcd(T a,T b,T&x,T&y,T c){if(b==0){x=c;y=0;return a;}T d=ExExgcd(b,a%b,x,y,c);T t=x;x=y;y=t-a/b*y;return d;}
template<class T>inline T Ksc(T a,T b){if(a==0||b==0){return 0;}T ans=0;while(b){if(b&1){ans=a+ans;}a=a+a;b>>=1;}return ans;}
template<class T>inline T Ksc(T a,T b,T p){if(a==0||b==0){return 0;}T ans=0;while(b){if(b&1){ans=(a+ans)%p;}a=(a+a)%p;b>>=1;}return ans;}
template<class T=unsigned long long>inline T Gsc(T a,T b,T p){a%=p,b%=p;register T c=(long double)a*b/p;T x=a*b,y=c*p;register long long ans=(long long)(x%p)-(long long)(y%p);ans<0?ans+=p:0;return ans;}
template<class T>inline T Ksm(T a,T b,bool f=false){if(a==0&&b==0)return 1;if(a==0){return 0;}if(b==0){return 1;}T ans=1;while(b){if(b&1){ans=!f?a*ans:Ksc(a,ans);}a=!f?a*a:Ksc(a,a);b>>=1;}return ans;}
template<class T>inline T Ksm(T a,T b,T p,bool f=false){if(a==0&&b==0)return 1;if(a==0){return 0;}if(b==0){return 1%p;}T ans=1;while(b){if(b&1){ans=!f?a*ans%p:Ksc(a,ans,p);}a=!f?a*a%p:Ksc(a,a,p);b>>=1;}return ans;}
template<class T>inline T Inv(T x,T p){return Ksm(x,p-2,p);}
template<class T>inline vector<pair<T,T>>Factor(T x){vector<pair<T,T>>ans;for(register T i=2;i*i<=x;++i)if(x%i==0){ans.emplace_back(i,1);while((x/=i)%i==0)++ans.back().second;}x!=1?ans.emplace_back(x,1),0:0;return ans;}
template<class T>vector<T>Divisor(T x){vector<T>ans;for(register T i=1;i*i<=x;++i)if(x%i==0)ans.emplace_back(i),i*i!=x?ans.emplace_back(x/i),0:0;return ans;}
template<class T>Inline int Popcount(T x,bool f=true){return f?__builtin_popcountll(x):__builtin_popcount(x);}
template<class T>Inline int Prebit(T x,bool f=true){return f?__builtin_clzll(x):__builtin_clz(x);}
template<class T>Inline int Sufbit(T x,bool f=true){return f?__builtin_ctzll(x):__builtin_ctz(x);}
template<class T>Inline int Lowone(T x,bool f=true){return f?__builtin_ffsll(x):__builtin_ffs(x);}
template<class T>Inline int Parity(T x,bool f=true){return f?__builtin_parityll(x):__builtin_parity(x);}
template<class T>Inline bool Isdigit(T ch){return ch>='0'&&ch<='9';}
template<class T>Inline bool Isletter(T ch){return ch>='A'&&ch<='Z'||ch>='a'&&ch<='z';}
template<class T>Inline bool Isupper(T ch){return ch>='A'&&ch<='Z';}
template<class T>Inline bool Islower(T ch){return ch>='a'&&ch<='z';}
template<class T>Inline char Tolower(T ch){return ch-'A'+'a';}
template<class T>Inline char Toupper(T ch){return ch-'a'+'A';}
template<class T>inline T Sin(register T degree){return sinl(degree*(pi/180));}
template<class T>inline T Cos(register T degree){return cosl(degree*(pi/180));}
template<class T>inline T Tan(register T degree){return tanl(degree*(pi/180));}
template<class T>inline T Asin(register T degree){return asinl(degree)*180.0/pi;}
template<class T>inline T Acos(register T degree){return acosl(degree)*180.0/pi;}
template<class T>inline T Atan(register T degree){return atanl(degree)*180.0/pi;}
template<class T>inline T Gpop(stack<T>&q){register T u=q.top();q.pop();return u;}
template<class T>inline T Gpop(queue<T>&q){register T u=q.front();q.pop();return u;}
template<class T>inline T Gpop(deque<T>&q){register T u=q.front();q.pop_front();return u;}
template<class T>inline T Gpop(priority_queue<T>&q){register T u=q.top();q.pop();return u;}
template<class T>inline T Gpop(priority_queue<T,vector<T>,greater<T>>&q){register T u=q.top();q.pop();return u;}
template<class T>inline T Gpop(vector<T>&G){register T u=G.back();G.pop_back();return u;}
template<class T>inline vector<T>Presum(vector<T>&a,int ind=1){vector<T>sum(SIZ(a)+0);For (i,ind,SIZ(a)-1){sum[i]=sum[i-1]+a[i];}return sum;}
template<class T>inline vector<T>Sufsum(vector<T>&a,int ind=1){vector<T>sum(SIZ(a)+1);FoR(i,SIZ(a)-1,ind){sum[i]=sum[i+1]+a[i];}return sum;}
template<class T>inline vector<T>Rearrange(vector<T>&a,vector<T>&ind,int id=1){vector<T>p(SIZ(ind)+id);For(i,ind,SIZ(ind)-(id^1)){p[i]=a[ind[i]];}return p;}
template<class T>inline vector<long long>Argsort(vector<T>&a,int id=1){vector<long long>ind(SIZ(a));iota(ALL(ind,id),0);sort(ALL(ind,id),[&](int i,int j){return a[i]==a[j]?i<j:a[i]<a[j];});return ind;}
template<class T>inline vector<long long>TransSV(T &s,char ch,char sp='?',int ind=1){vector<long long>G(SIZ(s)+ind);For(i,ind,SIZ(s)-1){G[i]=s[i]==sp?-1:s[i]-ch;}return G;}
template<class T>inline long long Rand(T l,T r){static mt19937_64 Rd(chrono::steady_clock::now().time_since_epoch().count());return uniform_int_distribution<long long>(l,r)(Rd);}
template<class T,class F>inline T Binarysearch(T l,T r,T f,const F &check,bool D=true){T mid,ans;for(mid=(l+r)>>1,ans=f;l<=r;mid=(l+r)>>1){check(mid)?ans=mid,(D?r=mid-1:l=mid+1):(D?l=mid+1:r=mid-1);}return ans;}
template<class T,class F>inline T Binarysearch(T l,T r,T f,double eps,const F &check,bool D=true){T mid,ans;for(mid=(l+r)/2;r-l>eps;mid=(l+r)/2){check(mid)?ans=mid,(D?l=mid:r=mid):(D?r=mid:l=mid);}return ans;}
template<class T>inline void Radixsort(register const int n,T*a,T*b){int r1[0x100],r2[0x100],r3[0x100],r4[0x100];memset(r1,0,sizeof(r1));memset(r2,0,sizeof(r2));memset(r3,0,sizeof(r3));memset(r4,0,sizeof(r4));register int i,tmp_int;register T*j,*tar;for(j=a+1,tar=a+n+1;j!=tar;++j){tmp_int=*(int*)j;++r1[tmp_int&0xff];++r2[(tmp_int>>8)&0xff];++r3[(tmp_int>>16)&0xff];++r4[tmp_int>>24];}for(i=1;i<=0xff;++i){r1[i]+=r1[i-1];r2[i]+=r2[i-1];r3[i]+=r3[i-1];r4[i]+=r4[i-1];}for(j=a+n;j!=a;--j){tmp_int=*(int*)j;b[r1[tmp_int&0xff]--]=*j;}for(j=b+n;j!=b;--j){tmp_int=*(int*)j;a[r2[(tmp_int>>8)&0xff]--]=*j;}for(j=a+n;j!=a;--j){tmp_int=*(int*)j;b[r3[(tmp_int>>16)&0xff]--]=*j;}for(j=b+n;j!=b;--j){tmp_int=*(int*)j;a[r4[tmp_int>>24]--]=*j;}}
template<class T>inline void Radixsort(register const int n,T*a,T*b,bool op){size_t size_of_type=sizeof(T);size_t num_of_buc=size_of_type>>1;unsigned**r=new unsigned*[num_of_buc];register int i,k;for(i=0;i<num_of_buc;++i){r[i]=new unsigned[0x10000];memset(r[i],0,0x10000*sizeof(unsigned));}register unsigned short tmp_us;register T*j,*tar;for(k=0;k<num_of_buc;++k){for(j=a+1,tar=a+n+1;j!=tar;++j){tmp_us=*(((unsigned short*)j)+k);++r[k][tmp_us];}}for(k=0;k<num_of_buc;++k){for(i=1;i<=0xffff;++i){r[k][i]+=r[k][i-1];}}for(k=0;k<num_of_buc;k+=0x2){i=k;for(j=a+n;j!=a;--j){tmp_us=*(((unsigned short*)j)+i);b[r[i][tmp_us]--]=*j;}i|=1;if(i==num_of_buc){break;}for(j=b+n;j!=b;--j){tmp_us=*(((unsigned short*)j)+i);a[r[i][tmp_us]--]=*j;}}for(int i=0;i<num_of_buc;i++){delete[]r[i];}delete[]r;}
template<class T>inline void Radixsort(register const int n,T*a,T*b,bool op1,bool op2){Radixsort(n,a,b,true);reverse(a+1,a+n+1);reverse(upper_bound(a+1,a+n+1,(T)(-0.0)),a+n+1);}
template<int R,int L=0,int M=(L+R>>1),class T>Inline void Unroll(int i,T f){R-L==1?f(L+i):(Unroll<M,L>(i,f),Unroll<R,M>(i,f));}
inline void OF(string s){string IN=s+".in";string OUT=s+".out";freopen(IN.c_str(),"r",stdin);freopen(OUT.c_str(),"w",stdout);}
inline void CF(string s){fclose(stdin);fclose(stdout);}
struct Safe_Hash{
	static unsigned long long splitmix64(unsigned long long x){x+=0x9e3779b97f4a7c15;x=(x^(x>>30))*0xbf58476d1ce4e5b9;x=(x^(x>>27))*0x94d049bb133111eb;return x^(x>>31);}
    size_t operator()(unsigned long long x)const{static const unsigned long long FIXED_RANDOM=chrono::steady_clock::now().time_since_epoch().count();return splitmix64(x+FIXED_RANDOM);}
	template<class T>size_t operator()(const vector<T>&v)const{size_t res=0;for(auto&i:v)res=(res^operator()(i))*operator()(i+1);return res^operator()(v.size())*operator()(v.size()+1);}
	template<class T1,class T2>size_t operator()(const pair<T1,T2>&p)const{return operator()(p.first)^operator()(p.second)*operator()(p.first+2)*operator()(p.second+1);}
	template<class...T>size_t operator()(const tuple<T...>&t)const{size_t res=0;for(auto&i:t)res=(res^operator()(i))*operator()(i+1);return res;}
};
#define UseBuffer
struct IO{
#ifdef UseBuffer
	//#define fread fread_unlocked
	//#define fwrite fwrite_unlocked
    const static int BUFSIZE=1<<20;
	char buf[BUFSIZE],obuf[BUFSIZE],*p1,*p2,*pp;
	inline char getchar(){return(p1==p2&&(p2=(p1=buf)+fread(buf,1,BUFSIZE,stdin),p1==p2)?EOF:*p1++);}
	inline void putchar(char x){((pp-obuf==BUFSIZE&&(fwrite(obuf,1,BUFSIZE,stdout),pp=obuf)),*pp=x,pp++);}
	inline IO&flush(){fwrite(obuf,1,pp-obuf,stdout);fflush(stdout);return*this;}
	IO(){p1=buf,p2=buf,pp=obuf;}
	~IO(){flush();}
#else
	//remember to flush in interactive problems
    //int(*getchar)()=&::getchar_unlocked;
	//int(*putchar)(int)=&::putchar_unlocked;
	int(*getchar)()=&::getchar;
	int(*putchar)(int)=&::putchar;
	inline IO&flush(){fflush(stdout);return*this;};
#endif
	int k=2;
    string sep=" ";
	template<typename Tp,typename enable_if<is_integral<Tp>::value||is_same<Tp,__int128_t>::value>::type* =nullptr>inline int read(Tp&s){int f=1;char ch=getchar();s=0;while(!isdigit(ch)&&ch!=EOF)f=(ch=='-'?-1:1),ch=getchar();while(isdigit(ch))s=s*10+(ch^48),ch=getchar();s*=f;return ch!=EOF;}
	template<typename Tp,typename enable_if<is_floating_point<Tp>::value>::type* =nullptr>inline int read(Tp&s){int f=1;char ch=getchar();s=0;while(!isdigit(ch)&&ch!=EOF&&ch!='.')f=(ch=='-'?-1:1),ch=getchar();while(isdigit(ch))s=s*10+(ch^48),ch=getchar();if(ch==EOF)return false;if(ch=='.'){Tp eps=0.1;ch=getchar();while(isdigit(ch))s=s+(ch^48)*eps,ch=getchar(),eps/=10;}s*=f;return ch!=EOF;}
	inline int read(char&c){char ch=getchar();c=EOF;while(isspace(ch)&&ch!=EOF)ch=getchar();if(ch!=EOF)c=ch;return c!=EOF;}
	inline int read(char*c){char ch=getchar(),*s=c;while(isspace(ch)&&ch!=EOF)ch=getchar();while(!isspace(ch)&&ch!=EOF)*(c++)=ch,ch=getchar();*c='\0';return s!=c;}
	inline int read(string&s){s.clear();char ch=getchar();while(isspace(ch)&&ch!=EOF)ch=getchar();while(!isspace(ch)&&ch!=EOF)s+=ch,ch=getchar();return s.size()>0;}
	inline int getline(char*c,const char&ed='\n'){char ch=getchar(),*s=c;while(ch!=ed&&ch!=EOF)*(c++)=ch,ch=getchar();*c='\0';return s!=c;}
	inline int getline(string&s,const char&ed='\n'){s.clear();char ch=getchar();while(ch!=ed&&ch!=EOF)s+=ch,ch=getchar();return s.size()>0;}
	template<typename Tp=int>inline Tp read(){Tp x;read(x);return x;}
	template<typename Tp,typename...Ts>int read(Tp&x,Ts&...val){return read(x)&&read(val...);}
	template<typename Tp,typename enable_if<is_integral<Tp>::value>::type* =nullptr>IO&write(Tp x){if(x<0)putchar('-'),x=-x;static char sta[114];int top=0;do sta[top++]=x%10+'0',x/=10;while(x);while(top)putchar(sta[--top]);return*this;}
	inline IO&write(const string&str){for(char ch:str)putchar(ch);return*this;}
	inline IO&write(const char*str){while(*str!='\0')putchar(*(str++));return*this;}
	inline IO&write(char*str){return write((const char*)str);}
	inline IO&write(const char&ch){return putchar(ch),*this;}
	template<typename Tp,typename enable_if<is_floating_point<Tp>::value>::type* =nullptr>inline IO&write(Tp x){if(x>1e18||x<-1e18){write("[Floating point overflow]");throw;}if(x<0)putchar('-'),x=-x;const static long long pow10[]={1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000,10000000000,100000000000,1000000000000,10000000000000,100000000000000,1000000000000000,10000000000000000,100000000000000000,100000000000000000,100000000000000000};const auto&n=pow10[k];long long whole=(int)x;double tmp=(x-whole)*n;long long frac=tmp;double diff=tmp-frac;if(diff>0.5){++frac;if(frac>=n)frac=0,++whole;}else if(diff==0.5&&((frac==0U)||(frac&1U)))++frac;write(whole);if(k==0U){diff=x-(double)whole;if((!(diff<0.5)||(diff>0.5))&&(whole&1))++whole;}else{putchar('.');static char sta[20];int count=k,top=0;while(frac){sta[top++]=frac%10+'0';frac/=10,count--;}while(count--)putchar('0');while(top)putchar(sta[--top]);}return*this;}
	template<typename Tp,typename...Ts>inline IO&write(Tp x,Ts...val){write(x);write(sep);write(val...);return*this;}
	template<typename...Ts>inline IO&writeln(Ts...val){write(val...);putchar('\n');return*this;}
	inline IO&writeln(void){putchar('\n');return*this;}
	template<typename Tp>inline IO&writeWith(Tp x,const string&s=" "){write(x),write(s);return*this;}
	inline IO&setsep(const string&s){return sep=s,*this;}
	inline IO&setprec(const int&K){return k=K,*this;}
}io;
inline int rd(){return io.read<int>();}
inline long long read(){return io.read<long long>();}
inline unsigned long long Read(){return io.read<unsigned long long>();}
template<class T,size_t x>inline void readsp(array<T,x>&G){For0(x)io.read(G[i]);return void();}
template<class T,class Q>inline void readsp(pair<T,Q>&x){io.read(x.first,x.second);return void();}
template<size_t N=0,class T>inline void readtp(T &t){if constexpr(N<tuple_size<T>::value){auto &x=get<N>(t);io.read(x);readtp<N+1>(t);}return void();}
template<class...T>inline void readsp(tuple<T...>&x){readtp(x);return void();}
template<class T,class...Args>inline void readsp(T&x,Args&...args){readsp(x);readsp(args...);return void();}
template<class T>inline bool wrt(T x){return io.write(x),true;}
template<class T>inline bool Write(T x,string ch="\n"){return io.writeWith(x,ch),true;}
template<class...T>inline bool write(T...x){io.write(x...);io.putchar('\n');return true;}
template<class T,size_t x>inline bool writesp(const array<T,x>&G){For0(x)Write(G[i],i==x-1?"\n":" ");return true;}
template<class T,class Q>inline bool writesp(const pair<T,Q>&x){Write(x.first," ");write(x.second);return true;}
template<size_t N=0,class T>inline bool writetp(const T t){if constexpr(N<tuple_size<T>::value){if constexpr(N>0){io.putchar(' ');}const auto x=get<N>(t);wrt(x);writetp<N+1>(t);}return true;}
template<class...T>inline bool writesp(tuple<T...>t){writetp(t);io.putchar('\n');return true;}
template<class T>inline bool writesp(vector<T>&G){if(G.empty())return false;register int sz=SIZ(G)-1;For(sz){Write(G[i],i==sz?"\n":" ");}return true;}
template<class T,class...Args>inline bool writesp(T x,Args&...args){writesp(x);writesp(args...);return true;}
inline bool WRITE(__int128 x,char ch='\n'){if(x<0)io.putchar('-'),x=-x;static char sta[114];int top=0;do sta[top++]=x%10+'0',x/=10;while(x);while(top)io.putchar(sta[--top]);io.putchar(ch);return true;}
#if __cplusplus>=201402L
namespace Montgomery{
#define Modulus_Const
#define Modulus_Prime
//if modulus is const,open Montgomery Fast  Version.
//if modulus is not const,open Dynamic Fast Version,if modulus is not prime,open Dynamic Modulus Version.
#ifdef Modulus_Prime
	bool PR=true;
#else
	bool PR=false;
#endif
	namespace Mont32{
		inline constexpr unsigned getroot1(int MOD){unsigned iv=MOD;for(register unsigned i=0;i!=4;++i){iv*=2U-MOD*iv;}return-iv;}
		template<int MOD>struct Mont{
			private:
				unsigned v;static constexpr unsigned r1=getroot1(MOD),r2=-(unsigned long long)(MOD)%MOD;static_assert((MOD&1)==1);static_assert(-r1*MOD==1);static_assert(MOD<(1<<30));
				inline static constexpr unsigned ksmmod(unsigned x,unsigned long long y){unsigned ans=1;for(;y!=0;y>>=1,x=(unsigned long long)(x)*x%MOD){if(y&1){ans=(unsigned long long)(ans)*x%MOD;}}return ans;}
				inline static constexpr unsigned reduce(unsigned long long x){return x+(unsigned long long)(unsigned(x)*r1)*MOD>>32;}
				inline static constexpr unsigned norm(unsigned x){return x-(MOD&-(x>=MOD));}
			public:
				static constexpr unsigned primitive(){unsigned tmp[32]={},cnt=0;constexpr unsigned long long phi=MOD-1;unsigned long long m=phi;for(register unsigned long long i=2;i*i<=m;++i){if(m%i==0){tmp[cnt++]=i;while(m%i==0){m/=i;}}}if(m!=1){tmp[cnt++]=m;}for(register unsigned long long ans=2;ans!=MOD;++ans){bool flag=true;for(register unsigned i=0;i!=cnt&&flag;++i){flag&=ksmmod(ans,phi/tmp[i])!=1;}if(flag){return ans;}}return 0;}
				 Mont()=default;
				~Mont()=default;
				constexpr Mont(unsigned v):v(reduce((unsigned long long)(v)*r2)){}
				constexpr Mont(const Mont&x):v(x.v){}
				inline int getP()const{return MOD;}
				constexpr unsigned get()const{return norm(reduce(v));}
				explicit constexpr operator unsigned()const{return get();}
				explicit constexpr operator int()const{return(int)(get());}
				Mont operator-()const{Mont ans;return ans.v=(MOD<<1&-(v!=0))-v,ans;}
				Mont Inv()const{int x1=1,x3=0,a=get(),b=MOD;while(b!=0){int q=a/b;tie(x1,x3)=make_tuple(x3,x1-x3*q);tie(a,b)=make_tuple(b,a-b*q);}return Mont(x1+MOD);}
				Mont&operator+=(const Mont&x){return v+=x.v-(MOD<<1),v+=MOD<<1&-(v>>31),*this;}
				Mont&operator-=(const Mont&x){return v-=x.v,v+=MOD<<1&-(v>>31),*this;}
				Mont&operator*=(const Mont&x){return v=reduce((unsigned long long)(v)*x.v),*this;}
				Mont&operator/=(const Mont&x){return this->operator*=(x.Inv());}
				#define stO(op) friend Mont operator op(const Mont&x,const Mont&y){return Mont(x)op##=y;}
				stO(+)stO(-)stO(*)stO(/)
				#undef stO
				#define stO(op) friend bool operator op(const Mont&x,const Mont&y){return norm(x.v)op norm(y.v);}
				stO(==)stO(!=)stO(>)stO(<)stO(>=)stO(<=)
				#undef stO
				Mont&operator++(){*this+=1;return*this;}
				Mont&operator--(){*this-=1;return*this;}
				Mont operator++(int){Mont ans(*this);*this+=1;return ans;}
				Mont operator--(int){Mont ans(*this);*this-=1;return ans;}
				Mont Ksm(long long b){Mont ans=1,a=get();while(b){if(b&1){ans=ans*a;}a=a*a;b>>=1;}return ans;}
				friend istream&operator>>(istream&is,Mont&x){return is>>x.v,x.v=reduce((unsigned long long)(x.v)*r2),is;}
				friend ostream&operator<<(ostream&os,Mont&x){return os<<x.get();}
		};
	}
	namespace Mont64{
		inline constexpr unsigned long long getroot1(long long MOD){unsigned long long iv=MOD;for(register unsigned i=0;i!=5;++i){iv*=2ULL-MOD*iv;}return iv;}
		inline constexpr unsigned long long getroot2(long long MOD){unsigned long long iv=-(unsigned long long)(MOD)%MOD;for(register unsigned i=0;i!=64;++i){if(MOD<=(iv<<=1)){iv-=MOD;}}return iv;}
		template<long long MOD>struct Mont{
			private:
				unsigned long long v;
				static constexpr unsigned long long r1=getroot1(MOD);static constexpr unsigned long long r2=getroot2(MOD);static_assert((MOD&1)==1);static_assert(r1*MOD==1);static_assert(MOD<(1ULL<<63));
				inline static pair<unsigned long long,unsigned long long>mul(unsigned long long x,unsigned long long y){unsigned long long a=x>>32,b=(unsigned)(x),c=y>>32,d=(unsigned)(y),ac=a*c,bd=b*d,ad=a*d,bc=b*c;return make_pair(ac+(ad>>32)+(bc>>32)+((ad&-1U)+(bc&-1U)+(bd>>32)>>32),bd+(ad+bc<<32));}
				inline static unsigned long long mulhi(unsigned long long x,unsigned long long y){unsigned long long a=x>>32,b=(unsigned)(x),c=y>>32,d=(unsigned)(y),ac=a*c,bd=b*d,ad=a*d,bc=b*c;return ac+(ad>>32)+(bc>>32)+((ad&-1U)+(bc&-1U)+(bd>>32)>>32);}
				inline static unsigned long long reduce(const pair<unsigned long long,unsigned long long>&x){unsigned long long ans=x.first-mulhi(x.second*r1,MOD);return ans+(MOD&-(ans>>63));}
				inline static unsigned long long ksmmod(unsigned long long x,unsigned long long y){unsigned long long ans=reduce(make_pair(0,r2));for(x=reduce(mul(x,r2));y!=0;y>>=1,x=reduce(mul(x,x))){if(y&1){ans=reduce(mul(ans,x));}}return reduce(make_pair(0,ans));}
			public:
				inline static unsigned long long primitive(){unsigned long long tmp[128]={},cnt=0;constexpr unsigned long long phi=MOD-1;unsigned long long m=phi;for(register unsigned long long i=2;i*i<=m;++i){if(m%i==0){tmp[cnt++]=i;while(m%i==0){m/=i;}}}if(m!=1){tmp[cnt++]=m;}for(register unsigned long long ans=2;ans!=MOD;++ans){bool flag=true;for(register unsigned i=0;i!=cnt&&flag;++i){flag&=ksmmod(ans,phi/tmp[i])!=1;}if(flag){return ans;}}return 0;}
				 Mont()=default;
				~Mont()=default;
				Mont(unsigned long long v):v(reduce(mul(v,r2))){}
				Mont(const Mont&x):v(x.v){}
				inline long long getP()const{return MOD;}
				inline unsigned long long get()const{return reduce(make_pair(0,v));}
				explicit operator long long()const{return(long long)(get());}
				explicit operator unsigned long long()const{return get();}
				Mont Inv()const{long long x1=1,x3=0,a=get(),b=MOD;while(b!=0){long long q=a/b;tie(x1,x3)=make_tuple(x3,x1-x3*q);tie(a,b)=make_tuple(b,a-b*q);}return Mont(x1+MOD);}
				Mont operator-()const{Mont ans;return ans.v=(MOD&-(v!=0))-v,ans;}
				Mont&operator*=(const Mont&x){return v=reduce(mul(v,x.v)),*this;}
				Mont&operator+=(const Mont&x){return v+=x.v-MOD,v+=MOD&-(v>>63),*this;}
				Mont&operator-=(const Mont&x){return v-=x.v,v+=MOD&-(v>>63),*this;}
				Mont&operator/=(const Mont&x){return this->operator*=(x.Inv());}
				#define stO(op) friend Mont operator op(const Mont&x,const Mont&y){return Mont(x)op##=y;}
				stO(+)stO(-)stO(*)stO(/)
				#undef stO
				#define stO(op) friend bool operator op(const Mont&x,const Mont&y){return reduce(make_pair(0,x.v))op reduce(make_pair(0,y.v));}
				stO(==)stO(!=)stO(>)stO(<)stO(>=)stO(<=)
				#undef stO
				Mont&operator++(){*this+=1;return*this;}
				Mont&operator--(){*this-=1;return*this;}
				Mont operator++(int){Mont ans(*this);*this+=1;return ans;}
				Mont operator--(int){Mont ans(*this);*this-=1;return ans;}
				friend istream&operator>>(istream&is,Mont&x){return is>>x.v,x.v=reduce(mul(x.v,r2)),is;}
				friend ostream&operator<<(ostream&os,Mont&x){return os<<x.get();}
				Mont Ksm(long long b){Mont ans=1,a=get();while(b){if(b&1){ans=ans*a;}a=a*a;b>>=1;}return ans;}
		};
	}
	namespace DynamicMont{
		using Tp=long long;
		struct Barrett{
			bool f;long long coef,p;
			inline void Setp(long long P){coef=((__int128)1<<64)/(p=P);}
			long long operator() (const long long &x){return PR?x-(((__int128)x*coef)>>64)*p:(x>=2*p?x%p:x>=p&&x<2*p?x-p:x>=0&&x<p?x:x<0&&x>-p?x+p:x%p+p);}
		}Rec;
		template<int DM>struct Mont{
			static Tp MOD;Tp v;
			 Mont()=default;
			~Mont()=default;
			static void Setp(Tp p){MOD=p;Rec.Setp(MOD);}
			inline Tp getP(){return MOD;}
			inline Tp get(){Mont ans=*this;return ans.v;}
			template<class T>inline Tp reduce(const T &x){Tp ans=Rec(x);ans>=MOD?ans-=MOD:0;return ans<0?ans+MOD:ans;}
			template<class T>Mont(const T&x):v(reduce(x)){}
			Mont operator-()const{return Mont(v?MOD-v:0);}
			Mont&operator+=(const Mont&x){return v=reduce(v+x.v),*this;}
			Mont&operator-=(const Mont&x){return v=reduce(v-x.v),*this;}
			Mont&operator*=(const Mont&x){return v=reduce(v*x.v),*this;}
			Mont&operator/=(const Mont&x){return*this*=x.Inv();}
			#define stO(op) friend Mont operator op(const Mont&x,const Mont&y){return Mont(x)op##=y;}
			stO(+)stO(-)stO(*)stO(/)
			#undef stO
			#define stO(op) friend bool operator op(const Mont&x,const Mont&y){return x.v op y.v;}
			stO(==)stO(!=)stO(>)stO(<)stO(>=)stO(<=)
			#undef stO
			Mont&operator++(){*this+=1;return*this;}
			Mont&operator--(){*this-=1;return*this;}
			Mont operator++(int){Mont ans(*this);*this+=1;return ans;}
			Mont operator--(int){Mont ans(*this);*this-=1;return ans;}
			Mont Ksm(long long b)const{Mont ans=1,a=*this;while(b){if(b&1){ans=ans*a;}a=a*a;b>>=1;}return ans;}
			Mont Inv()const{return Ksm(MOD-2);}
		};
		template<int DM>Tp Mont<DM>::MOD;
	}
#if (defined Modulus_Const)&&(defined Modulus_Prime)
	using namespace Mont64;
	constexpr long long MD=998244353;
	using DZ=Mont<MD>;
#else
	using namespace DynamicMont;
	using DZ=Mont<-1 >;
	struct WarnDynamic{WarnDynamic(){cerr<<"Your mod is not constant, so you can only use DynamicMont, remember to set your mod !"<<endl;}}WarnDY;
#endif
	namespace Mathematics{
		template<int MOD>struct Combination{
			using CZ=Mont32::Mont<MOD>;
			int n;long long P;vector<CZ>inv,fac,ifac;
			Combination():n(0),fac{1},ifac{1},inv{0}{}
			Combination(int lim):Combination(){Init(lim);}
			inline void Init(int lim){if(lim<=n)return void();inv.resize(lim+1);fac.resize(lim+1);ifac.resize(lim+1);
			for(register int i=n+1;i<=lim;++i)fac[i]=fac[i-1]*i;ifac[lim]=fac[lim].Inv();for(register int i=lim;i>=n+1;--i)ifac[i-1]=ifac[i]*i,inv[i]=ifac[i]*fac[i-1];n=lim;return void();}
			inline int getP(){return MOD;}
			inline CZ Inv(long long x){return x>n?Init(x<<1):void(),inv[x];}
			inline CZ Fac(long long x){return x>n?Init(x<<1):void(),fac[x];}
			inline CZ Ifac(long long x){return x>n?Init(x<<1):void(),ifac[x];}
			inline CZ A(long long x,long long y){return x<y||x<0||y<0?0:Fac(x)*Ifac(y);}
			inline CZ C(long long x,long long y){return x<y||x<0||y<0?0:Fac(x)*Ifac(y)*Ifac(x-y);}
		};
		struct DCombination{
			using CZ=DynamicMont::Mont<-1>;
			int n;long long P;vector<CZ>inv,fac,ifac;
			DCombination():n(0),fac{1},ifac{1},inv{0}{}
			DCombination(long long M):DCombination(){CZ::Setp(M);P=M;}
			DCombination(int lim,long long M):DCombination(){CZ::Setp(M);P=M;Init(lim);}
			inline void Init(int lim){if(lim<=n)return void();inv.resize(lim+1);fac.resize(lim+1);ifac.resize(lim+1);
			for(register int i=n+1;i<=lim;++i)fac[i]=fac[i-1]*i;ifac[lim]=fac[lim].Inv();for(register int i=lim;i>=n+1;--i)ifac[i-1]=ifac[i]*i,inv[i]=ifac[i]*fac[i-1];n=lim;return void();}
			inline int getP(){return P;}
			inline CZ Inv(long long x){return x>n?Init(x<<1):void(),inv[x];}
			inline CZ Fac(long long x){return x>n?Init(x<<1):void(),fac[x];}
			inline CZ Ifac(long long x){return x>n?Init(x<<1):void(),ifac[x];}
			inline CZ A(long long x,long long y){return x<y||x<0||y<0?0:Fac(x)*Ifac(y);}
			inline CZ C(long long x,long long y){return x<y||x<0||y<0?0:Fac(x)*Ifac(y)*Ifac(x-y);}
		};
	}
	using namespace Mathematics;
}
#else
namespace Montgomery{
#define Modulus_Const
#define Modulus_Prime
//if modulus is const,open Montgomery Fast  Version.
//if modulus is not const,open Dynamic Fast Version,if modulus is not prime,open Dynamic Modulus Version.
#ifdef Modulus_Prime
	bool PR=true;
#else
	bool PR=false;
#endif
    namespace DynamicMont{
		using Tp=long long;
		struct Barrett{
			bool f;long long coef,p;
			inline void Setp(long long P){coef=((__int128)1<<64)/(p=P);}
			long long operator() (const long long &x){return PR?x-(((__int128)x*coef)>>64)*p:(x>=2*p?x%p:x>=p&&x<2*p?x-p:x>=0&&x<p?x:x<0&&x>-p?x+p:x%p+p);}
		}Rec;
		template<int DM>struct Mont{
			static Tp MOD;Tp v;
			 Mont()=default;
			~Mont()=default;
			static void Setp(Tp p){MOD=p;Rec.Setp(MOD);}
			inline Tp getP(){return MOD;}
			inline Tp get(){Mont ans=*this;return ans.v;}
			template<class T>inline Tp reduce(const T &x){Tp ans=Rec(x);ans>=MOD?ans-=MOD:0;return ans<0?ans+MOD:ans;}
			template<class T>Mont(const T&x):v(reduce(x)){}
			Mont operator-()const{return Mont(v?MOD-v:0);}
			Mont&operator+=(const Mont&x){return v=reduce(v+x.v),*this;}
			Mont&operator-=(const Mont&x){return v=reduce(v-x.v),*this;}
			Mont&operator*=(const Mont&x){return v=reduce(v*x.v),*this;}
			Mont&operator/=(const Mont&x){return*this*=x.Inv();}
			#define stO(op) friend Mont operator op(const Mont&x,const Mont&y){return Mont(x)op##=y;}
			stO(+)stO(-)stO(*)stO(/)
			#undef stO
			#define stO(op) friend bool operator op(const Mont&x,const Mont&y){return x.v op y.v;}
			stO(==)stO(!=)stO(>)stO(<)stO(>=)stO(<=)
			#undef stO
			Mont&operator++(){*this+=1;return*this;}
			Mont&operator--(){*this-=1;return*this;}
			Mont operator++(int){Mont ans(*this);*this+=1;return ans;}
			Mont operator--(int){Mont ans(*this);*this-=1;return ans;}
			Mont Ksm(long long b)const{Mont ans=1,a=*this;while(b){if(b&1){ans=ans*a;}a=a*a;b>>=1;}return ans;}
			Mont Inv()const{return Ksm(MOD-2);}
		};
		template<int DM>Tp Mont<DM>::MOD;
	}
    using namespace DynamicMont;
#if (defined Modulus_Const)&&(defined Modulus_Prime)
	constexpr long long MD=998244353;
	using DZ=Mont<-1>;
    struct SetMod{SetMod(){DZ::Setp(MD);}}SetMOD;
	struct WarnDynamic{WarnDynamic(){cerr<<"Your C++ version is lower than 17, so you can only use DynamicMont, remember to set your mod !"<<endl;}}WarnDY;
#else
	using DZ=Mont<-1>;
	struct WarnDynamic{WarnDynamic(){cerr<<"Your C++ version is lower than 17, so you can only use DynamicMont, remember to set your mod !"<<endl;}}WarnDY;
#endif
	namespace Mathematics{
		struct Combination{
			using CZ=DynamicMont::Mont<-1>;
			int n;long long P;vector<CZ>inv,fac,ifac;
			Combination():n(0),fac{1},ifac{1},inv{0}{}
			Combination(long long M):Combination(){CZ::Setp(M);P=M;}
			Combination(int lim,long long M):Combination(){CZ::Setp(M);P=M;Init(lim);}
			inline void Init(int lim){if(lim<=n)return void();inv.resize(lim+1);fac.resize(lim+1);ifac.resize(lim+1);
			for(register int i=n+1;i<=lim;++i)fac[i]=fac[i-1]*i;ifac[lim]=fac[lim].Inv();for(register int i=lim;i>=n+1;--i)ifac[i-1]=ifac[i]*i,inv[i]=ifac[i]*fac[i-1];n=lim;return void();}
			inline int getP(){return P;}
			inline CZ Inv(long long x){return x>n?Init(x<<1):void(),inv[x];}
			inline CZ Fac(long long x){return x>n?Init(x<<1):void(),fac[x];}
			inline CZ Ifac(long long x){return x>n?Init(x<<1):void(),ifac[x];}
			inline CZ A(long long x,long long y){return x<y||x<0||y<0?0:Fac(x)*Ifac(y);}
			inline CZ C(long long x,long long y){return x<y||x<0||y<0?0:Fac(x)*Ifac(y)*Ifac(x-y);}
		};
	}
	using namespace Mathematics;
}
#endif
using namespace Montgomery;
/*
Good Luck!
Have Fun!
CSPS RP++
NOIP RP++
  OI RP++
 NOI RP++
 CTT RP++
 CTS RP++
 IOI RP++
 ZKA RP++
 GKA RP++
 KAY RP++
 KAB RP++
 WHK RP++
Goal:
CF GM
AT 2DAN
LG Lv9
 
To be continued...
*/
//.................................................................................................................
//.................................................................................................................
//.................................................................................................................
//.................................................................................................................
//.................................................................................................................
//.......RRRRRRRRRRRRRRRRRRRR...................PPPPPPPPPPPPPPPPPPPP...............................................
//.......RRRRRRRRRRRRRRRRRRRRRR.................PPPPPPPPPPPPPPPPPPPPPP.............................................
//.......RRRRRRRRRRRRRRRRRRRRRRR................PPPPPPPPPPPPPPPPPPPPPPPP...........................................
//.......RRRR.................RRRRR.............PPPP...............PPPPP...........................................
//.......RRRR.................RRRRR.............PPPP................PPPPP..........................................
//.......RRRR.................RRRRR.............PPPP................PPPPP..........................................
//.......RRRR...............RRRRR...............PPPP...............PPPPP...........................................
//.......RRRR............RRRRRR.................PPPP.............PPPPPP............................................
//.......RRRR............RRRRRR.................PPPP............PPPPPP.............................................
//.......RRRR........RRRRRR.....................PPPP........PPPPPPP................................................
//.......RRRRRRRRRRRRRRRRRR.....................PPPPPPPPPPPPPPPPPP.................................................
//.......RRRRRRRRRRRRRRRRRR.....................PPPPPPPPPPPPPPPP...................................................
//.......RRRR..........RRRR.....................PPPPP.................................+++................+++.......
//.......RRRR...........RRRR....................PPPPP.................................+++................+++.......
//.......RRRR.............RRRR..................PPPPP.................................+++................+++.......
//.......RRRR..............RRRR.................PPPPP...........................+++++++++++++++....+++++++++++++++.
//.......RRRR...............RRRR................PPPPP...........................+++++++++++++++....+++++++++++++++.
//.......RRRR................RRRR...............PPPPP.................................+++................+++.......
//.......RRRR.................RRRR..............PPPPP.................................+++................+++.......
//.......RRRR...................RRRR............PPPPP.................................+++................+++.......
//.................................................................................................................
//.................................................................................................................
//.................................................................................................................
//.................................................................................................................
//.................................................................................................................
using tp=long long;
using i16=short;
using i64=long long;
using i128=__int128;
using u16=unsigned short;
using u32=unsigned;
using u64=unsigned long long;
using u128=unsigned __int128;
using d32=double;
using d64=long double;
using d128=__float128;
template<class T,class Q>using pr=pair<T,Q>;
template<class T=tp,size_t x=3>using ar=array<T,x>;
template<class T=tp>using vc=vector<T>;
template<class T=tp>using vvc=vector<vc<T>>;
template<class T=tp>using vvvc=vector<vvc<T>>;
template<class T=tp>using vvvvc=vector<vvvc<T>>;
template<class T=tp>using vvvvvc=vector<vvvvc<T>>;
template<class T=tp>using pqueue=priority_queue<T>;
template<class T=tp>using bqueue=priority_queue<T,vector<T>,greater<T>>;
template<class T=tp>using bs=basic_string<tp>;
template<class T=tp,class Q=tp,class R=tp>using tup=tuple<T,Q,R>;
template<class T=tp,class Q=tp,class R=tp,class S=tp>using ttup=tuple<T,Q,R,S>;
template<class T=tp,class Q=tp,class R=tp,class S=tp,class U=tp>using tttup=tuple<T,Q,R,S,U>;
#define CurClock(T) static_cast<T>(chrono::steady_clock::now().time_since_epoch().count())
#define mtset(T) multiset<T>
#define mtmap(T,S) multimap<T,S>
#define umap(T,S) unordered_map<T,S>
#define uset(T) unordered_set<T>
#define umset(T) unordered_multiset<T>
#define Treap(T) __gnu_pbds::tree<T,null_type,less<T>,rb_tree_tag,tree_order_statistics_node_update>
#define HashTable(T,Func) __gnu_pbds::gp_hash_table<T,T,Func>
#define FibHeap(T,Cmp) __gnu_pbds::priority_queue<T,Cmp,thin_heap_tag>
#define PairHeap(T,Cmp) __gnu_pbds::priority_queue<T,Cmp,pairing_heap_tag>
#define vv(T,G,a,...) vvc<T>G(a,vc<T>(__VA_ARGS__))
#define vvv(T,G,a,b,...) vvvc<T>G(a,vvc<T>(b,vc<T>(__VA_ARGS__)))
#define vvvv(T,G,a,b,c,...) vvvvc<T>G(a,vvvc<T>(b,vvc<T>(c,vc<T>(__VA_ARGS__))))
#define vvvvv(T,G,a,b,c,d,...) vvvvvc<T>G(a,vvvvc<T>(b,vvvc<T>(c,vvc<T>(d,vc<T>(__VA_ARGS__)))))
using pt=pr<tp,tp>;
using ar3=ar<tp,3>;
using ar4=ar<tp,4>;
template<class T,class Q>int operator += (vector<T>&G,Q x){return G.pb(x),0;}
template<class T,class Q>int operator += (deque<T>&q,Q x){return q.pb(x),0;}
template<class T,class Q>int operator -= (deque<T>&q,Q x){return q.pf(x),0;}
template<class T,class Q>int operator += (list<T>&q,Q x){return q.pb(x),0;}
template<class T,class Q>int operator -= (list<T>&q,Q x){return q.pf(x),0;}
#define LoadAdd(op) template<class T,class Q>int operator += (op&q,Q x){return q.ep(x),0;}
LoadAdd(queue<T>)LoadAdd(stack<T>)LoadAdd(set<T>)LoadAdd(multiset<T>)LoadAdd(unordered_set<T>)LoadAdd(unordered_multiset<T>)LoadAdd(pqueue<T>)LoadAdd(bqueue<T>)
[[maybe_unused]]constexpr long long N=1000100;bool MultiCase;
inline int SolveMain([[maybe_unused]]long long TC);
inline void InitMain();
#if 1
int main(){
//  OF("test");
	InitMain();
	Test()SolveMain(TestCase);
	return 0;
}
#endif
inline void InitMain(){
    MultiCase=false;
	
	return void();
}
#if 0
1.  close Buffer when meeting interactive problems, flush when output.
2.  check files when meeting file IO problems.
3.  check the size of vectors, and the memory.
4.  long long is slower than int, do not use it when the coef is big.
5.  delete Debug code or use "//" or "/**/" or "#if 0".
6.  initialize, especially "tp a[...]={}", it can cause weird errors.
7.  think twice, code once, especially ad-hoc, maths, dp, constructive problems.
8.  think of "[l,r]" of Binarysearch, and the "ans's" special value.
9.  make special datas and the limit data when "Duipai", try to make the strongest data you can think.
10. notice int overflow(use long long), long long overflow(use __int128)/MLE(use int or bitfield).
To be continued...
To be continued...
To be continued...
To be continued...
To be continued...
To be continued...
To be continued...
To be continued...
To be continued...
CSPJRP++
CSPSRP++
NOIPRP++
FJOIRP++
 NOIRP++
 CTTRP++
 CTSRP++
 IOIRP++
THUSC/THUWCRP++
PKUSC/PKUWCRP++
#endif
#ifndef __OY_CONVOLUTIONMULTIMODTWOPOWN__
#define __OY_CONVOLUTIONMULTIMODTWOPOWN__

#ifndef __OY_CONVOLUTION__
#define __OY_CONVOLUTION__

#ifndef __OY_CONVOLUTIONMODINT__
#define __OY_CONVOLUTIONMODINT__

#ifndef __OY_CONVOLUTIONMODINTCOMMON__
#define __OY_CONVOLUTIONMODINTCOMMON__

namespace OY{
    namespace MYCONV{
        struct has_mod_impl{
            template<class T>static auto check(T&&x)->decltype(x.get_mod(),std::true_type{});
            template<class T>static auto check(...)->std::false_type;
        };
        template<class T>class has_mod:public decltype(has_mod_impl::check<T>(std::declval<T>())){};
        template<typename mint>mint inv(int n){
            static const int mod=mint::get_mod();
            static vector<mint>dat={0,1};n>=mod?n%=mod:0;
            while(int(dat.size())<=n){
                int k=int(dat.size()),q=(mod+k-1)/k;
                dat.emplace_back(dat[k*q-mod]*mint::raw(q));
            }return dat[n];
        }
        template<typename mint>mint fact(int n){static const int mod=mint::get_mod();static vector<mint>dat={1,1};while(int(dat.size())<=n)dat.emplace_back(dat[int(dat.size())-1]*mint::raw(int(dat.size())));return dat[n];}
        template<typename mint>mint fact_inv(int n){static vector<mint>dat={1,1};if(n<0)return mint(0);while(int(dat.size())<=n)dat.emplace_back(dat[int(dat.size())-1]*inv<mint>(int(dat.size())));return dat[n];}
        template<class mint,class... Ts>mint fact_invs(Ts... xs){return(mint(1)*...*fact_inv<mint>(xs));}
        template<typename mint,class Head,class... Tail>mint multinomial(Head&&head,Tail&&... tail){return fact<mint>(head)*fact_invs<mint>(std::forward<Tail>(tail)...);}
        template<typename mint>mint C_dense(int n,int k){
            if(k<0||n<k)return 0;
            static vector<vector<mint>>C;static int H=0,W=0;
            auto calc=[&](int i,int j)->mint{if(i==0)return(j==0?mint(1):mint(0));return C[i-1][j]+(j?C[i-1][j-1]:0);};
            if(W<=k){for(long long i=0;i<=H-1;++i){C[i].resize(k+1);for(long long j=W;j<=k;++j)C[i][j]=calc(i,j);}W=k+1;}
            if(H<=n){C.resize(n+1);for(long long i=H;i<=n;++i){C[i].resize(W);for(long long j=1;j<=W-1;++j)C[i][j]=calc(i,j);}H=n+1;}return C[n][k];
        }
        template<typename mint,bool large=false,bool dense=false>mint C(long long n,long long k){
            if(k<0||n<k)return 0;
            if constexpr(dense)return C_dense<mint>(n,k);
            if constexpr(!large)return multinomial<mint>(n,k,n-k);
            k=min(k,n-k);mint x(1);for(long long i=0;i<=k-1;++i)x*=mint(n-i);return x*fact_inv<mint>(k);
        }
        template<typename mint,bool large=false>mint C_inv(long long n,long long k){if(!large)return fact_inv<mint>(n)*fact<mint>(k)*fact<mint>(n-k);return mint(1)/C<mint,1>(n,k);}
        template<typename mint,bool large=false,bool dense=false>mint C_negative(long long n,long long d){if(d<0)return mint(0);if(n==0){return(d==0?mint(1):mint(0));}return C<mint,large,dense>(n+d-1,d);}
    }
}// by Richard1211

#endif

namespace OY{
    namespace MYCONV{
        template<int mod>struct modint{
            static constexpr unsigned umod=unsigned(mod);
            /*static_assert(umod<u32(1)<<31);*/unsigned val;
            static modint raw(unsigned v){modint x;x.val=v;return x;}
            constexpr modint():val(0){}
            constexpr modint(unsigned x):val(x%umod){}
            constexpr modint(unsigned long long x):val(x%umod){}
            constexpr modint(unsigned  __int128 x):val(x%umod){}
            constexpr modint(int x):val((x%=mod)<0?x+mod:x){};
            constexpr modint(long long x):val((x%=mod)<0?x+mod:x){};
            constexpr modint(__int128  x):val((x%=mod)<0?x+mod:x){};
            bool operator<(const modint&other)const{return val<other.val;}
            modint&operator+=(const modint&p){(val+=p.val)>=umod?val-=umod:0;return*this;}
            modint&operator-=(const modint&p){(val+=umod-p.val)>=umod?val-=umod:0;return*this;}
            modint&operator*=(const modint&p){val=(unsigned long long)(val)*p.val%umod;return*this;}
            modint&operator/=(const modint&p){*this*=p.inverse();return*this;}
            modint operator-()const{return modint::raw(val?mod-val:unsigned(0));}
            modint operator+(const modint&p)const{return modint(*this)+=p;}
            modint operator-(const modint&p)const{return modint(*this)-=p;}
            modint operator*(const modint&p)const{return modint(*this)*=p;}
            modint operator/(const modint&p)const{return modint(*this)/=p;}
            bool operator==(const modint&p)const{return val==p.val;}
            bool operator!=(const modint&p)const{return val!=p.val;}
            modint inverse()const{int a=val,b=mod,u=1,v=0,t;while(b>0){t=a/b;swap(a-=t*b,b),swap(u-=t*v,v);}return modint(u);}
            modint pow(long long n)const{if(n<0)return inverse().pow(-n);modint ret(1),mul(val);while(n>0){if(n&1)ret*=mul;mul*=mul;n>>=1;}return ret;}
            static constexpr int get_mod(){return mod;}
            static constexpr pair<int,int>ntt_info(){
                if(mod==120586241)return{20,74066978};
                if(mod==167772161)return{25,17};
                if(mod==469762049)return{26,30};
                if(mod==754974721)return{24,362};
                if(mod==880803841)return{23,211};
                if(mod==943718401)return{22,663003469};
                if(mod==998244353)return{23,31};
                if(mod==1004535809)return{21,582313106};
                if(mod==1012924417)return{21,368093570};
                return{-1,-1};
            }
            static constexpr bool can_ntt(){return ntt_info().first!=-1;}
        };
        using modint107=modint<1000000007>;
        using modint998=modint<998244353>;
    }
}// by Richard1211

#endif
#ifndef __OY_CONVOLUTIONMODINV__
#define __OY_CONVOLUTIONMODINV__

namespace OY{
    namespace MYCONV{
        long long mod_inv(long long val,long long mod){
            if(mod==0)return 0;
            mod=abs(mod);val%=mod;
            val<0?val+=mod:0;void();
            long long a=val,b=mod,u=1,v=0,t;
            while(b>0){t=a/b;swap(a-=t*b,b),swap(u-=t*v,v);}
            u<0?u+=mod:0;return u;
        }
    }
}// by Richard1211

#endif
#ifndef __OY_SMALLCRT__
#define __OY_SMALLCRT__

namespace OY{
    namespace MYCONV{
        #define u32 unsigned
        #define u64 unsigned long long
        #define u128 unsigned __int128
        constexpr u32 mod_pow_constexpr(u64 a,u64 n,u32 mod){a%=mod;u64 res=1;for(long long i=0;i<=31;++i){if(n&1)res=res*a%mod;a=a*a%mod,n/=2;}return res;}
        template<typename T,u32 p0,u32 p1>T CRT2(u64 a0,u64 a1){
            //static_assert(p0<p1);
            static constexpr u64 x0_1=mod_pow_constexpr(p0,p1-2,p1);
            u64 c=(a1-a0+p1)*x0_1%p1;return a0+c*p0;
        }
        template<typename T,u32 p0,u32 p1,u32 p2>T CRT3(u64 a0,u64 a1,u64 a2){
            //static_assert(p0<p1&&p1<p2);
            static constexpr u64 x1=mod_pow_constexpr(p0,p1-2,p1);
            static constexpr u64 x2=mod_pow_constexpr((u64)(p0)*p1%p2,p2-2,p2);
            static constexpr u64 p01=(u64)(p0)*p1;
            u64 c=(a1-a0+p1)*x1%p1;u64 ans_1=a0+c*p0;c=(a2-ans_1%p2+p2)*x2%p2;return T(ans_1)+T(c)*T(p01);
        }
        template<typename T,u32 p0,u32 p1,u32 p2,u32 p3>T CRT4(u64 a0,u64 a1,u64 a2,u64 a3){
            //static_assert(p0<p1&&p1<p2&&p2<p3);
            static constexpr u64 x1=mod_pow_constexpr(p0,p1-2,p1);
            static constexpr u64 x2=mod_pow_constexpr((u64)(p0)*p1%p2,p2-2,p2);
            static constexpr u64 x3=mod_pow_constexpr((u64)(p0)*p1%p3*p2%p3,p3-2,p3);
            static constexpr u64 p01=(u64)(p0)*p1;u64 c=(a1-a0+p1)*x1%p1;
            u64 ans_1=a0+c*p0;c=(a2-ans_1%p2+p2)*x2%p2;
            u128 ans_2=ans_1+c*static_cast<u128>(p01);c=(a3-ans_2%p3+p3)*x3%p3;return T(ans_2)+T(c)*T(p01)*T(p2);
        }
        template<typename T,u32 p0,u32 p1,u32 p2,u32 p3,u32 p4>T CRT5(u64 a0,u64 a1,u64 a2,u64 a3,u64 a4){
            //static_assert(p0<p1&&p1<p2&&p2<p3&&p3<p4);
            static constexpr u64 x1=mod_pow_constexpr(p0,p1-2,p1);
            static constexpr u64 x2=mod_pow_constexpr((u64)(p0)*p1%p2,p2-2,p2);
            static constexpr u64 x3=mod_pow_constexpr((u64)(p0)*p1%p3*p2%p3,p3-2,p3);
            static constexpr u64 x4=mod_pow_constexpr((u64)(p0)*p1%p4*p2%p4*p3%p4,p4-2,p4);
            static constexpr u64 p01=(u64)(p0)*p1;
            static constexpr u64 p23=(u64)(p2)*p3;
            u64 c=(a1-a0+p1)*x1%p1;
            u64 ans_1=a0+c*p0;c=(a2-ans_1%p2+p2)*x2%p2;
            u128 ans_2=ans_1+c*static_cast<u128>(p01);c=static_cast<u64>(a3-ans_2%p3+p3)*x3%p3;
            u128 ans_3=ans_2+static_cast<u128>(c*p2)*p01;c=static_cast<u64>(a4-ans_3%p4+p4)*x4%p4;return T(ans_3)+T(c)*T(p01)*T(p23);
        }
        #undef u32
        #undef u64
        #undef u128
    }
}// by Richard1211

#endif
#ifndef __OY_CONVOLUTIONNAIVE__
#define __OY_CONVOLUTIONNAIVE__

namespace OY{
    namespace MYCONV{
        template<class T,typename enable_if<!has_mod<T>::value>::type* =nullptr>vector<T>ConvolutionNaive(const vector<T>&a,const vector<T>&b){
            int n=int(a.size()),m=int(b.size());
            if(n>m)return ConvolutionNaive<T>(b,a);
            if(n==0)return{};vector<T>ans(n+m-1);void();
            for(long long i=0;i<=n-1;++i)for(long long j=0;j<=m-1;++j)ans[i+j]+=a[i]*b[j];return ans;
        }
        template<class T,typename enable_if< has_mod<T>::value>::type* =nullptr>vector<T>ConvolutionNaive(const vector<T>&a,const vector<T>&b){
            int n=int(a.size()),m=int(b.size());
            if(n>m)return ConvolutionNaive<T>(b,a);
            if(n==0)return{};vc<T>ans(n+m-1);void();
            if(n<=16&&(T::get_mod()<(1<<30))){
                for(int k=0;k<n+m-1;++k){
                    int s=max(0,k-m+1);
                    int t=min(n,k+1);unsigned long long sm=0;
                    for(int i=s;i<t;++i)sm+=(unsigned long long)(a[i].val)*(b[k-i].val);ans[k]=sm;
                }
            }
            else{
                for(int k=0;k<n+m-1;++k){
                    int s=max(0,k-m+1);
                    int t=min(n,k+1);unsigned __int128 sm=0;
                    for(int i=s;i<t;++i)sm+=(unsigned long long)(a[i].val)*(b[k-i].val);ans[k]=T::raw(sm%T::get_mod());
                }
            }return ans;
        }
    }
}// by Richard1211

#endif
#ifndef __OY_CONVOLUTIONKARATSUBA__
#define __OY_CONVOLUTIONKARATSUBA__


namespace OY{
    namespace MYCONV{
        template<typename T>vector<T>ConvolutionKaratsuba(const vector<T>&f,const vector<T>&g){
            const int thresh=30;
            if(min(int(f.size()),int(g.size()))<=thresh)return ConvolutionNaive(f,g);
            int n=max(int(f.size()),int(g.size()));int m=Ceil(n,2);vector<T>f1,f2,g1,g2;
            int(f.size())<m?f1=f,0:0;
            int(f.size())>=m?f1={f.begin(),f.begin()+m},0:0;
            int(f.size())>=m?f2={f.begin()+m,f.end()},0:0;
            int(g.size())<m?g1=g,0:0;
            int(g.size())>=m?g1={g.begin(),g.begin()+m},0:0;
            int(g.size())>=m?g2={g.begin()+m,g.end()},0:0;
            vector<T>a=ConvolutionKaratsuba(f1,g1);
            vector<T>b=ConvolutionKaratsuba(f2,g2);
            for(int i=0;i<=int(f2.size())-1;++i)f1[i]+=f2[i];
            for(int i=0;i<=int(g2.size())-1;++i)g1[i]+=g2[i];
            vector<T>c=ConvolutionKaratsuba(f1,g1);
            vector<T>F(int(f.size())+int(g.size())-1);//assert(2*m+len(b)<=len(F));
            for(int i=0;i<=int(a.size())-1;++i)F[i]+=a[i],c[i]-=a[i];
            for(int i=0;i<=int(b.size())-1;++i)F[2*m+i]+=b[i],c[i]-=b[i];c.back()==T(0)?c.pop_back(),0:0;
            for(int i=0;i<=int(c.size())-1;++i)c[i]!=T(0)?F[m+i]+=c[i],0:0;return F;
        }
    }
}// by Richard1211

#endif
#ifndef __OY_CONVOLUTIONNTT__
#define __OY_CONVOLUTIONNTT__

#ifndef __OY_TOPBIT__
#define __OY_TOPBIT__

namespace OY{
    inline int topbit(int x){return(x==0?-1:31-__builtin_clz(x));}
    inline int topbit(unsigned x){return(x==0?-1:31-__builtin_clz(x));}
    inline int topbit(long long x){return(x==0?-1:63-__builtin_clzll(x));}
    inline int topbit(unsigned long long x){return(x==0?-1:63-__builtin_clzll(x));}
}// by Richard1211

#endif

namespace OY{
    namespace MYCONV{
        template<class mint>void NTT(vector<mint>&a,bool inverse){
            //assert(mint::can_ntt());
            const int mod=mint::get_mod();
            const int rank2=mint::ntt_info().first;
            static array<mint,30>root,iroot;
            static array<mint,30>rate2,irate2;
            static array<mint,30>rate3,irate3;
            //assert(rank2!=-1&&int(a.size())<=(1<<max(0,rank2)));
            static bool prepared=0;
            if(!prepared){
                prepared=1;
                root[rank2]=mint::ntt_info().second;
                iroot[rank2]=mint(1)/root[rank2];void();
                for(int i=rank2-1;i>=0;--i)root[i]=root[i+1]*root[i+1],iroot[i]=iroot[i+1]*iroot[i+1];mint prod=1,iprod=1;
                for(int i=0;i<=rank2-2;++i)rate2[i]=root[i+2]*prod,irate2[i]=iroot[i+2]*iprod,prod*=iroot[i+2],iprod*=root[i+2];prod=1,iprod=1;
                for(int i=0;i<=rank2-3;++i)rate3[i]=root[i+3]*prod,irate3[i]=iroot[i+3]*iprod,prod*=iroot[i+3],iprod*=root[i+3];
            }
            int n=int(a.size());
            int h=topbit(n);//assert(n==1<<h);
            if(!inverse){
                int len=0;
                while(len<h){
                    if(h-len==1){
                        int p=1<<(h-len-1);mint rot=1;
                        for(int s=0;s<(1<<len);++s){
                            int offset=s<<(h-len);
                            for(int i=0;i<=p-1;++i){
                                auto l=a[i+offset];
                                auto r=a[i+offset+p]*rot;
                                a[i+offset]=l+r;
                                a[i+offset+p]=l-r;
                            }rot*=rate2[topbit(~s&-~s)];
                        }len++;
                    }else{
                        int p=1<<(h-len-2);
                        mint rot=1,imag=root[2];
                        for(int s=0;s<(1<<len);++s){
                            mint rot2=rot*rot;
                            mint rot3=rot2*rot;
                            int offset=s<<(h-len);
                            for(int i=0;i<=p-1;++i){
                                unsigned long long mod2=(unsigned long long)(mod)*mod;
                                unsigned long long a0=a[i+offset].val;
                                unsigned long long a1=(unsigned long long)(a[i+offset+p].val)*rot.val;
                                unsigned long long a2=(unsigned long long)(a[i+offset+2*p].val)*rot2.val;
                                unsigned long long a3=(unsigned long long)(a[i+offset+3*p].val)*rot3.val;
                                unsigned long long a1na3imag=(a1+mod2-a3)%mod*imag.val;
                                unsigned long long na2=mod2-a2;
                                a[i+offset]=a0+a2+a1+a3;
                                a[i+offset+1*p]=a0+a2+(2*mod2-(a1+a3));
                                a[i+offset+2*p]=a0+na2+a1na3imag;
                                a[i+offset+3*p]=a0+na2+(mod2-a1na3imag);
                            }rot*=rate3[topbit(~s&-~s)];
                        }len+=2;
                    }
                }
            }else{
                mint coef=mint(1)/mint(int(a.size()));
                for(int i=0;i<=int(a.size())-1;++i)a[i]*=coef;int len=h;
                while(len){
                    if(len==1){
                        int p=1<<(h-len);mint irot=1;
                        for(int s=0;s<(1<<len-1);++s){
                            int offset=s<<(h-len+1);
                            for(int i=0;i<=p-1;++i){
                                unsigned long long l=a[i+offset].val;
                                unsigned long long r=a[i+offset+p].val;
                                a[i+offset]=l+r;
                                a[i+offset+p]=(mod+l-r)*irot.val;
                            }irot*=irate2[topbit(~s&-~s)];
                        }len--;
                    }else{
                        int p=1<<(h-len);
                        mint irot=1,iimag=iroot[2];
                        for(int s=0;s<(1<<len-2);++s){
                            mint irot2=irot*irot;
                            mint irot3=irot2*irot;
                            int offset=s<<(h-len+2);
                            for(int i=0;i<=p-1;++i){
                                unsigned long long a0=a[i+offset+0*p].val;
                                unsigned long long a1=a[i+offset+1*p].val;
                                unsigned long long a2=a[i+offset+2*p].val;
                                unsigned long long a3=a[i+offset+3*p].val;
                                unsigned long long x=(mod+a2-a3)*iimag.val%mod;
                                a[i+offset]=a0+a1+a2+a3;
                                a[i+offset+1*p]=(a0+mod-a1+x)*irot.val;
                                a[i+offset+2*p]=(a0+a1+2*mod-a2-a3)*irot2.val;
                                a[i+offset+3*p]=(a0+2*mod-a1-x)*irot3.val;
                            }irot*=irate3[topbit(~s&-~s)];
                        }len-=2;
                    }
                }
            }
        }
    }
}// by Richard1211

#endif

namespace OY{
    namespace MYCONV{
        template<class mint>vector<mint>ConvolutionNTT(vector<mint>a,vector<mint>b){
            if(a.empty()||b.empty())return{};
            int n=int(a.size()),m=int(b.size());int sz=1;
            while(sz<n+m-1)sz*=2;if((n+m-3)<=sz/2){
                auto a_last=a.back(),b_last=b.back();
                a.pop_back(),b.pop_back();void();void();
                auto c=Convolution(a,b);c.resize(n+m-1);c[n+m-2]=a_last*b_last;
                for(int i=0;i<=int(a.size())-1;++i)c[i+int(b.size())]+=a[i]*b_last;
                for(int i=0;i<=int(b.size())-1;++i)c[i+int(a.size())]+=b[i]*a_last;return c;
            }a.resize(sz),b.resize(sz);bool same=a==b;NTT(a,0);same?b=a,0:(NTT(b,0),0);
            for(long long i=0;i<=sz-1;++i)a[i]*=b[i];NTT(a,1);a.resize(n+m-1);return a;
        }
        template<typename mint>vector<mint>ConvolutionGarner(const vector<mint>&a,const vector<mint>&b){
            int n=int(a.size()),m=int(b.size());
            if(!n||!m)return{};
            static constexpr int p0=167772161;
            static constexpr int p1=469762049;
            static constexpr int p2=754974721;
            using mint0=modint<p0>;
            using mint1=modint<p1>;
            using mint2=modint<p2>;
            vc<mint0>a0(n),b0(m);
            vc<mint1>a1(n),b1(m);
            vc<mint2>a2(n),b2(m);
            for(int i=0;i<=n-1;++i)a0[i]=a[i].val,a1[i]=a[i].val,a2[i]=a[i].val;
            for(int i=0;i<=m-1;++i)b0[i]=b[i].val,b1[i]=b[i].val,b2[i]=b[i].val;
            auto c0=ConvolutionNTT<mint0>(a0,b0);
            auto c1=ConvolutionNTT<mint1>(a1,b1);
            auto c2=ConvolutionNTT<mint2>(a2,b2);
            vc<mint>c(int(c0.size()));
            for(int i=0;i<=n+m-2;++i)c[i]=CRT3<mint,p0,p1,p2>(c0[i].val,c1[i].val,c2[i].val);return c;
        }
        vector<long long>Convolution(vector<long long>a,vector<long long>b){
            int n=int(a.size()),m=int(b.size());
            if(!n||!m)return{};
            if(min(n,m)<=2500)return ConvolutionNaive(a,b);
            long long mi_a=*min_element(a.begin(),a.end()),
                      mi_b=*min_element(b.begin(),b.end());
            for(auto&x:a)x-=mi_a;
            for(auto&x:b)x-=mi_b;
            /*assert(*max_element(a.begin(),a.end())
                     *max_element(b.begin(),b.end())<=1000000000000000000);*/
            auto cumsum=[&](vector<long long>&A){
                int N=a.size();vector<long long>B(N+1);
                for(int i=0;i<=N-1;++i)B[i+1]=B[i]+A[i];return B;
            };auto Ac=cumsum(a),Bc=cumsum(b);vector<long long>res(n+m-1);
            for(int k=0;k<n+m-1;++k){
                int s=max(0,k-m+1);
                int t=min(n,k+1);void();
                res[k]+=(t-s)*mi_a*mi_b;
                res[k]+=mi_a*(Bc[k-s+1]-Bc[k-t+1]);
                res[k]+=mi_b*(Ac[t]-Ac[s]); void();
            }
            static constexpr u32 MOD1=1004535809;
            static constexpr u32 MOD2=1012924417;
            using mint1=modint<MOD1>;
            using mint2=modint<MOD2>;
            vc<mint1>a1(n),b1(m);
            vc<mint2>a2(n),b2(m);
            for(int i=0;i<=n-1;++i)a1[i]=a[i],a2[i]=a[i];
            for(int i=0;i<=m-1;++i)b1[i]=b[i],b2[i]=b[i];
            auto c1=ConvolutionNTT<mint1>(a1,b1);
            auto c2=ConvolutionNTT<mint2>(a2,b2);
            for(int i=0;i<=n+m-2;++i)res[i]+=CRT2<u64,MOD1,MOD2>(c1[i].val,c2[i].val);return res;
        }
        template<typename mint>vector<mint>Convolution(const vector<mint>&a,const vector<mint>&b){
            int n=int(a.size()),m=int(b.size());
            if(!n||!m)return{};
            if(mint::can_ntt())
            return min(n,m)<=50?ConvolutionKaratsuba<mint>(a,b):ConvolutionNTT(a,b);
            return min(n,m)<=200?ConvolutionKaratsuba<mint>(a,b):ConvolutionGarner(a,b);
        }
    }
}// by Richard1211

#endif

namespace OY{
    namespace MYCONV{
        template<typename mint>vector<mint>ConvolutionMultimod2pown(vector<mint>&A,vector<mint>&B){
            int N=0;
            while((1<<N)<int(A.size()))++N;
            //assert((1<<N)==int(A.size())&&(1<<N)==int(B.size()));
            int mask=(1<<N)-1;
            vector<vector<vector<mint>>>FA(N+1);
            vector<vector<vector<mint>>>FB(N+1);
            vector<vector<vector<mint>>>FC(N+1);
            auto shape=[&](int n)->pair<int,int>{
            return{N-n>=2?2:1,1<<max(N-n-2,0)};};
            for(long long n=0;n<=N;++n){
                int x=(1<<n)&mask;
                auto[H,W]=shape(n);
                FA[n].assign(H,vector<mint>(W));
                FB[n].assign(H,vector<mint>(W));
                FC[n].assign(H,vector<mint>(W));
                auto&a=FA[n],&b=FB[n];
                for(long long j=0;j<=W-1;++j){
                    a[0][j]=A[x];b[0][j]=B[x];
                    H==2?a[1][j]=A[(1<<N)-x],b[1][j]=B[(1<<N)-x]:0;x=(5*x)&mask;
                }
            }
            for(long long n=0;n<=N;++n){
                auto&a=FA[n],&b=FB[n];
                auto[H,W]=shape(n);
                for(long long i=0;i<=H-1;++i)NTT(a[i],false),NTT(b[i],false);
                if(H==2){for(long long j=0;j<=W-1;++j)tie(a[0][j],a[1][j])=make_pair(a[0][j]+a[1][j],a[0][j]-a[1][j]),
                                                      tie(b[0][j],b[1][j])=make_pair(b[0][j]+b[1][j],b[0][j]-b[1][j]);}
            }for(long long n1=0;n1<=N;++n1)
             for(long long n2=0;n2<=N;++n2){
                int n3=min(N,int(n1+n2));auto[H,W]=shape(n3);
                for(long long i=0;i<=H-1;++i)for(long long j=0;j<=W-1;++j)FC[n3][i][j]+=FA[n1][i][j]*FB[n2][i][j];
            }
            for(long long n=0;n<=N;++n){
                auto&c=FC[n];
                auto[H,W]=shape(n);
                for(long long i=0;i<=H-1;++i)NTT(c[i],true);
                if(H==2)for(long long j=0;j<=W-1;++j)tie(c[0][j],c[1][j])=make_pair(c[0][j]+c[1][j],c[0][j]-c[1][j]);
                mint coef=mint(1)/mint(H);for(long long i=0;i<=H-1;++i)for(long long j=0;j<=W-1;++j)c[i][j]*=coef;
            }vector<mint>C(1<<N);
            for(long long n=0;n<=N;++n){
                int x=(1<<n)&mask;
                auto[H,W]=shape(n);auto&c=FC[n];
                for(long long j=0;j<=W-1;++j){
                    C[x]=c[0][j];
                    H==2?C[(1<<N)-x]=c[1][j]:0;x=(5*x)&mask;
                }
            }
            return C;
        }
    }
}

#endif
using Z=OY::MYCONV::modint998;
inline signed SolveMain([[maybe_unused]]long long TC){
    RD(n);
    vc<Z>a(1<<n),b(1<<n);
    ForE(x,a)x=read();
    ForE(x,b)x=read();
    auto c=OY::MYCONV::ConvolutionMultimod2pown(a,b);
    ForE(x,c)Write(x.val,x==c.back()?"\n":" ");
	return 0;
}