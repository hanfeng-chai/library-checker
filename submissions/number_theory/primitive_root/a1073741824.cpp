#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <cassert>
#include <functional>
#include <random>
#include <bitset>
#include <unistd.h>


using namespace std;
using ll = long long;
using lll = __int128_t;
using ull = unsigned long long;
using ld = long double;
using pii = array<int,2>;
using pll = array<ll,2>;
using plll = array<lll,2>;

#define vall(A) A.begin(), A.end()
                                    inline void print(){cout << "\n";}
                                    inline void printflush(){cout << endl;}
template<typename T, typename... U> inline void print(T obj1, U... obj2){cout << (obj1) << " "; print(obj2...);}
template<typename T, typename... U> inline void printflush(T obj1, U... obj2){cout << (obj1) << " "; printflush(obj2...);}
template<typename T> inline void vin(T& A){for (int i = 0, sz = A.size(); i < sz; i++){cin >> A[i];}}
template<typename T> inline void vout(const T& A){if (A.size() == 0ull){print();} for (int i = 0, sz = A.size(); i < sz; i++){cout << A[i] << " \n"[i == sz-1];}}
template<typename T> inline void vout2d(const T& A){if (A.size() == 0ull){print();} for (int i = 0, H = A.size(); i < H; i++){vout(A[i]);}}
template<typename T> inline void adjvin(T& A){for (int i = 1, sz = A.size(); i < sz; i++){cin >> A[i];}}
template<typename T> inline void adjvout(const T& A){for (int i = 1, sz = A.size(); i < sz; i++){cout << A[i] << " \n"[i == sz-1];}}
template<typename T> inline void adjvout2d(const T& A){if (A.size() == 0ull){print();} for (int i = 1, H = A.size(); i < H; i++){adjvout(A[i]);}}
template<typename T> inline bool btest(T K, int i){return K&(1ull<<i);}
constexpr ll pow2ll[63] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648,4294967296,8589934592,17179869184,34359738368,68719476736,137438953472,274877906944,549755813888,1099511627776,2199023255552,4398046511104,8796093022208,17592186044416,35184372088832,70368744177664,140737488355328,281474976710656,562949953421312,1125899906842624,2251799813685248,4503599627370496,9007199254740992,18014398509481984,36028797018963968,72057594037927936,144115188075855872,288230376151711744,576460752303423488,1152921504606846976,2305843009213693952,4611686018427387904};
constexpr ll pow10ll[19] = {1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000,10000000000,100000000000,1000000000000,10000000000000,100000000000000,1000000000000000,10000000000000000,100000000000000000,1000000000000000000};
constexpr ll di[4] = {0,1,0,-1};
constexpr ll di8[8] = {0,1,1,1,0,-1,-1,-1};
constexpr ll dj[4] = {1,0,-1,0};
constexpr ll dj8[8] = {1,1,0,-1,-1,-1,0,1};

#ifndef MATH_FUNCTION_HPP_
#define MATH_FUNCTION_HPP_

#include <array>
#include <cmath>
using namespace std;
using ll = long long;
using ull = unsigned long long;



/// @brief a^bをmで割った余りを返す。bに関して対数時間で計算できる
constexpr ll modpow(ll a, ull b, const ll m){
    ll t = a%m;
    ll ans = (m == 1 ? 0 : 1);
    while (b > 0){
        if (b&1){
            ans = (ans*t)%m;
        }
        b >>= 1;
        t = (t*t)%m;
    }
    return ans;
}

/// @brief a^bをmで割った余りを返す。bに関して対数時間で計算できる。mはコンパイル時に決定している必要がある
template<ll m> constexpr ll modpow(ll a, ull b){
    ll t = a%m;
    ll ans = (m == 1 ? 0 : 1);
    while (b > 0){
        if (b&1){
            ans = (ans*t)%m;
        }
        b >>= 1;
        t = (t*t)%m;
    }
    return ans;
}

/// @brief a^nを返す。bに関して線形時間で計算できる
constexpr ll powll(ll a, ull n){
    ll r = 1;
    for (ull i = 1; i <= n; i++){
        r *= a;
    }
    return r;
}

/// @brief floor(sqrt(N))を返す
constexpr ll isqrt(ull N){
    ll ret = sqrt(N);
    while (ret*ret > N){
        ret--;
    }
    while ((ret+1)*(ret+1) <= N){
        ret++;
    }
    return ret;
}

/// @brief floor(log_a(L))を返す
constexpr ll ilog(ll a, ll L){
    __int128_t t = 1;
    ll ans = 0;
    while (t <= L){
        ans++;
        t *= a;
    }
    return ans-1;
}

/// @brief 有理数のfloorを求める
constexpr inline ll floor2(ll y, ll x){
    if ((x^y) > 0){
        x = abs(x);
        y = abs(y);
        return y/x;
    }
    else if ((x^y) < 0){
        x = abs(x);
        y = abs(y);
        return -((y+x-1)/x);
    }
    else{
        return y/x;
    }
}
/// @brief 有理数のceilを求める
constexpr inline ll ceil2(ll y, ll x){
    if ((x^y) > 0){
        x = abs(x);
        y = abs(y);
        return (y+x-1)/x;
    }
    else if ((x^y) < 0){
        x = abs(x);
        y = abs(y);
        return -(y/x);
    }
    else{
        return y/x;
    }
}

/// @brief 一次不定方程式ax+by=gcd(a,b)の解を1つ見つける
/// @param a `a>=0`である必要がある
/// @param b `b>=0`である必要がある
/// @return {x,y,gcd(a,b)}
template<typename T>
constexpr array<T,3> axby1(T a, T b){
    T x = 1, y = 0;
    T z = 0, w = 1;
    T tmp = 0;
    while (b){
        T p = a/b, q = a%b;
        tmp = x - y * p; x = y; y = tmp;
        tmp = z - w * p; z = w; w = tmp;
        a = b; b = q;
    }
    return {x, z, a};
}

/// @brief 1/a mod Mを求める
template<typename T, typename U>
constexpr T inverse_mod(T a, U M){
    auto temp = axby1(a,(T)M);
    assert(temp[2] == 1);
    return (M+temp[0])%M;
}

/// @brief sqrt(a) mod Mを求める。ないなら-1が返される。
template<ll M>
constexpr ll cipolla(ll a){
    a %= M;
    if (M == 2) return a;
	if (a == 0) return 0;
    ll z = (M-1)/2;
    if (modpow<M>(a, z) != 1){return -1;}
    int b = 0;
    while (modpow<M>((b*b+M-a)%M, z) == 1){
        b++;
    }
    array<ll,2> x{1,0};
    array<ll,2> y{b, 1};
    ll w = (b*b+M-a)%M;
    z++;
    while (z){
        if (z&1){
            ll temp = x[0];
            x[0] = x[0]*y[0]%M+x[1]*y[1]%M*w%M;
            if (x[0] >= M){x[0] -= M;}
            x[1] = temp*y[1]%M+x[1]*y[0]%M;
            if (x[1] >= M){x[1] -= M;}
        }
        ll temp = y[0];
        y[0] = y[0]*y[0]%M+y[1]*y[1]%M*w%M;
        if (y[0] >= M){y[0] -= M;}
        y[1] = 2*temp*y[1]%M;
        z >>= 1;
    }
    return x[0];
}
ll cipolla(ll a, const ll M){
    a %= M;
    if (M == 2) return a;
	if (a == 0) return 0;
    ll z = (M-1)/2;
    if (modpow(a, z, M) != 1){return -1;}
    int b = 0;
    while (modpow((b*b+M-a)%M, z, M) == 1){
        b++;
    }
    array<ll,2> x{1,0};
    array<ll,2> y{b, 1};
    ll w = (b*b+M-a)%M;
    z++;
    while (z){
        if (z&1){
            ll temp = x[0];
            x[0] = x[0]*y[0]%M+x[1]*y[1]%M*w%M;
            if (x[0] >= M){x[0] -= M;}
            x[1] = temp*y[1]%M+x[1]*y[0]%M;
            if (x[1] >= M){x[1] -= M;}
        }
        ll temp = y[0];
        y[0] = y[0]*y[0]%M+y[1]*y[1]%M*w%M;
        if (y[0] >= M){y[0] -= M;}
        y[1] = 2*temp*y[1]%M;
        z >>= 1;
    }
    return x[0];
}

/// @brief x以下の最大の2冪を返す。0は0が返る。 
constexpr int lowerpow2(ull x){
    if (x == 0){return 0;}
    return 1ull<<(63-__builtin_clzll(x));
}
/// @brief x以上の最小の2冪を返す。0は0が返る。 
constexpr int upperpow2(ull x){
    if (x == 0){return 0;}
    if (x == 1){return 1;}
    return 1ull<<(64-__builtin_clzll(x-1));
}



#endif /* MATH_FUNCTION_HPP_ */
#ifndef QUOTIENTS_HPP_
#define QUOTIENTS_HPP_

#include <vector>
#include <array>
#include <cmath>
#include <algorithm>
using namespace std;
using ll = long long;

/// @brief 1<=x<=M の範囲におけるN/xの商を列挙する。 {値, 左端, 右端}の形で求まる。
vector<array<ll,3>> enumerate_quotient(ll N, ll M){
    vector<array<ll,3>> ret;
    if (N == 0){
        ret.push_back({0,1,M});
        return ret;
    }
    ll k0 = sqrtl(N)-100;
    k0 = max(0ll, k0);
    ll k0r = sqrtl(N)+100;
    while (k0r*(k0r+1) <= N){
        k0r++;
    }
    while (k0r-k0 > 1){
        ll mid = (k0+k0r)/2;
        if (mid*(mid+1) <= N){
            k0 = mid;
        }
        else{
            k0r = mid;
        }
    }
    for (ll k = k0; k >= 0; k--){
        ret.push_back({k, N/(k+1)+1, min(M, k > 0 ? N/k : M)});
        if (ret.back()[1] > ret.back()[2]){
            ret.pop_back();
            break;
        }
    }
    reverse(ret.begin(), ret.end());
    for (ll x = min(M, N/(k0+1)); x >= 1; x--){
        ret.push_back({N/x, x,x});
    }
    return ret;
}

#endif /* QUOTIENTS_HPP_ */
#ifndef PRIME_AND_DIVISORS_HPP_
#define PRIME_AND_DIVISORS_HPP_

#include <array>
#include <vector>

#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>
using namespace std;
using ll = long long;
using ulll = __uint128_t;
using pll = array<ll,2>;
using pii = array<int,2>;



/// @brief  試し割り法で正の整数Nを素因数分解する
/// @return vector<array<ll,2>>{{素因数1,個数}, {素因数2,個数}, {素因数3,個数}...}
vector<pll> p_fact(ll N){
    if (N == 1){
        return vector<pll> {{1,0}};
    }
    vector<pll> R;//戻り値用リスト

    const int M = isqrt(N);
    for (int i = 2; i <= M; i++){
        if (N % i == 0){
            ll divide_count = 0;
            while (N % i == 0){
                divide_count++;
                N /= i;
            }
            R.push_back({i,divide_count});
        }
    }
    if (N != 1){
        R.push_back({N,1});
    }
    return R;
}

/// @brief 素因数分解リストを受け取って約数関数の値を求める
/// @return 約数のK乗和
template<typename T>
ll divisor_function(const vector<array<T,2>>& vv, ll K, const ll MOD = -1){
    if (vv[0][0] == 1){
        return 1;
    }
    ll R = 1;
    if (K == 0){
        for (auto x : vv){
            R *= x[1]+1;
            if (MOD > 0){
                R %= MOD;
            }
        }
    }
    else{
        for (auto x : vv){
            ll r = powll(x[0],K);
            R *= (powll(r,x[1]+1) - 1)/(r - 1);
            if (MOD > 0){
                R %= MOD;
            }
        }
    }
    return R;
}

/// @brief 素因数分解の結果pを受け取って、約数リストを生成する。
template<typename T>
vector<T> enumerate_divisor(const vector<array<T,2>>& p){
    vector<T> d{1};
    if (p[0][0] == 1){
        return d;
    }
    for (auto &v : p){
        int t = d.size();
        ll temp = 1;
        for (int w = 0; w < v[1]; w++){
            temp *= v[0];
            for (int i = 0; i < t; i++){
                d.push_back(d[i]*temp);
            }
        }
    }
    sort(vall(d));
    return d;
}

/// @brief 線形篩
/// @attention コンストラクタに整数Nを渡すことでN以下の整数を扱うことができる。
struct LinearSieve{
    vector<int> p_list;
    vector<int> lpf;
    //Nを渡すことで1以上N以下の整数を扱うことができる
    LinearSieve(int N): lpf(N+1,-1){
        lpf[1] = 1;
        int p_list_size = 0;
        for (int i = 2; i <= N; i++){
            if (lpf[i] < 0){
                p_list.push_back(i);
                p_list_size++;
                lpf[i] = i;
            }
            for (int j = 0; j < p_list_size && p_list[j] <= lpf[i] && p_list[j]*i <= N; j++){
                lpf[p_list[j]*i] = p_list[j];
            }
        }
    }
    /// @brief xを素因数分解する。 
    vector<pii> p_fact(int x){
        if (x == 1){return {{1,0}};}
        vector<pii> r;
        do{
            if (r.empty() || lpf[x] != r.back()[0]){
                r.push_back({lpf[x], 1});
            }
            else{
                r.back()[1]++;
            }
            x /= lpf[x];
        }while(x > 1);
        return r;
    }
    /// @brief N以下の整数をすべて素因数分解した結果を取得する。
    vector<vector<pii>> p_fact_all(int N){
        vector<vector<pii>> r(N+1);
        r[1].push_back({1,0});
        for (int i = 2; i <= N; i++){
            r[i] = p_fact(i);
        }
        return r;
    }
    /// @brief N以下の整数に対するメビウス関数の値を列挙する。
    vector<int> enumerate_mobius(int N){
        vector<int> ret(N+1);
        ret[1] = 1;
        for (int i = 2; i <= N; i++){
            if (lpf[i] == i){ret[i] = -1; continue;}
            int temp = i/lpf[i];
            if (lpf[i] == lpf[temp]){
                ret[i] = 0;
            }
            else{
                ret[i] = ret[lpf[i]]*ret[temp];
            }
        }
        return ret;
    }
};

constexpr ull __MillerRabin_small[3] = {2,7,61};
constexpr ull __MillerRabin_large[7] = {2,325,9375,28178,450775,9780504,1795265022};

struct Montgomery64 {
    ull n, ni, r2;
    constexpr Montgomery64(ull n) : n(n), ni(n), r2(-ulll(n) % n){
        for (int i = 0; i < 5; ++i) ni *= 2ull - n * ni;
        ni *= -1;
    }
    constexpr ull reduce(ulll x) const {
        ull m = (ull)x * ni;
        ull res = (x + ulll(m) * n) >> 64;
        return res >= n ? res - n : res;
    }
    constexpr ull mul(ull x, ull y) const {
        return reduce(ulll(x) * y);
    }
    constexpr ull pow_not_reduced(ull a, ull b)const{
        ull res = reduce(r2);
        a = reduce(ulll(a) * r2);
        while (b) {
            if (b & 1) res = mul(res, a);
            a = mul(a, a);
            b >>= 1;
        }
        return res;
    }
};

/// @brief ミラーラビン素数判定法 
constexpr bool MillerRabin(ull N){
    if (N <= 1) return false;
    if (N == 2 || N == 3) return true;
    if (N % 2 == 0) return false;

    ull d = N - 1;
    int s = 0;
    while (d % 2 == 0){
        d >>= 1;
        s++;
    }
    Montgomery64 mg(N);
    auto check = [&](ull a){
        if (a >= N) a %= N;
        if (a == 0) return true;
        ull x = mg.pow_not_reduced(a, d);
        ull y = mg.reduce(x);
        if (y == 1 || y == N-1) return true;
        for (int r = 1; r < s; r++){
            x = mg.mul(x, x);
            if (mg.reduce(x) == N - 1) return true;
        }
        return false;
    };

    if (N < 4759123141ull){
        for (auto a : __MillerRabin_small){
            if (!check(a)) return false;
        }
    }
    else{
        for (auto a : __MillerRabin_large){
            if (!check(a)) return false;
        }
    }
    return true;
}

/// @brief ポラード・ロー法による素因数分解
constexpr vector<pll> Pollard_rho(ull N){
    assert(N>0);
    if (N == 1){return {{1,0}};}
    vector<ll> res;
    vector<ull> dq;
    dq.push_back(N);
    while (!dq.empty()){
        ull n = dq.back();
        dq.pop_back();
        bool next_loop = false;
        while (!MillerRabin(n)){
            if ((n&1) == 0){
                while ((n&1) == 0){res.push_back(2); n>>=1;}
                if (n == 1){next_loop = true; break;}
                continue;
            }
            Montgomery64 mg(n);
            int blocksize = max<int>(1, pow(n, 0.125));
            for (ull c = 1; c <= n; c++){
                ull reduced_c = mg.reduce(c*(ulll)mg.r2);
                ull y = (0b1010110101101011110011ull^c^n)%n; y = mg.reduce(y*(ulll)mg.r2);
                ull r = 1;
                ull k = 0;
                int k_mod_blocksize = 0;
                bool next_loop_inner = false;
                for (int _ = 0; _ < 63; _++){
                    ull q = mg.reduce(mg.r2);
                    ull q_old = q;
                    ull y_old = y;
                    ull x = y;
                    while (k < r){
                        k++;
                        k_mod_blocksize++;
                        y = mg.reduce(y*(ulll)y)+reduced_c; if (y >= n){ y -= n;}
                        q = mg.reduce(q*(y >= x ? (ulll)(y-x) : (ulll)(x-y)));
                        if (k_mod_blocksize == blocksize){
                            k_mod_blocksize -= blocksize;
                            ull g = gcd(mg.reduce(q),n);
                            if (g > 1){
                                if (g != n){
                                    dq.push_back(g);
                                    dq.push_back(n/g);
                                    next_loop = true; break;
                                }
                                else{
                                    y = y_old;
                                    q = q_old;
                                    for (int i = 0; i < blocksize; i++){
                                        y = mg.reduce(y*(ulll)y)+reduced_c; if (y >= n){ y -= n;}
                                        q = mg.reduce(q*(y >= x ? (ulll)(y-x) : (ulll)(x-y)));
                                        g = gcd(mg.reduce(q),n);
                                        if (g > 1){
                                            if (g != n){
                                                dq.push_back(g);
                                                dq.push_back(n/g);
                                                next_loop = true; break;
                                            }
                                            next_loop_inner = true; break;
                                        }
                                    }
                                    if (next_loop || next_loop_inner) break;
                                }
                            }
                            q_old = q;
                            y_old = y;
                        }
                    }
                    if (next_loop || next_loop_inner) break;
                    q_old = gcd(mg.reduce(q),n);
                    if (q_old > 1){
                        if (q_old != n){
                            dq.push_back(q_old);
                            dq.push_back(n/q_old);
                            next_loop = true; break;
                        }
                        next_loop_inner = true; break;
                    }
                    r <<= 1;
                }
                if (next_loop) break;
            }
            if (next_loop) break;
        }
        if (!next_loop) {
            res.push_back(n);
        }
    }
    sort(res.begin(), res.end());
    vector<pll> res2;
    for (int l = 0, r = 0, sz = res.size(); l < sz;){
        while (r < sz && res[l] == res[r]){
            r++;
        }
        res2.push_back({res[l], r-l});
        l = r;
    }
    return res2;
}

struct Xorshift64{
    ull state;
    constexpr Xorshift64(ull seed = 881726454633252252ull) : state(seed) {}
    constexpr ull next() {
        state ^= state << 13;
        state ^= state >> 7;
        state ^= state << 17;
        return state;
    }
};

/// @brief mod Pにおける原始根を1つ探す
constexpr ull primitive_root(ull P){
    assert(MillerRabin(P));
    if (P == 2){
        return 1;
    }
    Montgomery64 mg(P);
    Xorshift64 rng(0x1234567E89ABCDEF);
    auto factorized = Pollard_rho(P-1);
    while (true) {
        ull r = rng.next()%P;
        if (r == 0){continue;}
        bool ok = true;
        for (auto& q : factorized){
            if (mg.reduce(mg.pow_not_reduced(r, (P-1)/q[0])) == 1){
                ok = false;
                break;
            }
        }
        if (ok) return r;
    }
}

#endif /* PRIME_AND_DIVISORS_HPP_ */

void solve(){
    ll P;
    cin >> P;
    print(primitive_root(P));
}

int main(){
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    ll T = 1;
    cin >> T;
    while (T--){solve();}

}