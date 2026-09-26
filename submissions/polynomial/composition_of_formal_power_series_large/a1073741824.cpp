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
template<typename T> inline void vin(T& A){for (int i = 0, sz = A.size(); i < sz; i++){cin >> A[i];}}
template<typename T> inline void vout(const T& A){for (int i = 0, sz = A.size(); i < sz; i++){cout << A[i] << " \n"[i == sz-1];}}
template<typename T> inline void vout2d(const T& A){for (int i = 0, H = A.size(); i < H; i++){vout(A[i]);}}
template<typename T> inline void adjvin(T& A){for (int i = 1, sz = A.size(); i < sz; i++){cin >> A[i];}}
template<typename T> inline void adjvout(const T& A){for (int i = 1, sz = A.size(); i < sz; i++){cout << A[i] << " \n"[i == sz-1];}}
template<typename T> inline void adjvout2d(const T& A){for (int i = 1, H = A.size(); i < H; i++){adjvout(A[i]);}}
template<typename T> inline bool btest(T K, int i){return K&(1ull<<i);}
template<typename T> void print(T object){cout << (object) << "\n";}
template<typename T, typename U> void print(T object1, U object2){cout << (object1) << " " << (object2) << "\n";}
template<typename T, typename U, typename V> void print(T object1, U object2, V object3){cout << (object1) << " " << (object2) << " " << (object3) << "\n";}
template<typename T, typename U, typename V, typename W> void print(T object1, U object2, V object3, W object4){cout << (object1) << " " << (object2) << " " << (object3) << " " << (object4) << "\n";}
const vector<ull> pow2ll{1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648,4294967296,8589934592,17179869184,34359738368,68719476736,137438953472,274877906944,549755813888,1099511627776,2199023255552,4398046511104,8796093022208,17592186044416,35184372088832,70368744177664,140737488355328,281474976710656,562949953421312,1125899906842624,2251799813685248,4503599627370496,9007199254740992,18014398509481984,36028797018963968,72057594037927936,144115188075855872,288230376151711744,576460752303423488,1152921504606846976,2305843009213693952,4611686018427387904, 9223372036854775808ull};
const vector<ull> pow10ll{1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000,10000000000,100000000000,1000000000000,10000000000000,100000000000000,1000000000000000,10000000000000000,100000000000000000,1000000000000000000, 10000000000000000000ull};
const vector<ll> di{0,1,0,-1};
const vector<ll> di8{0,1,1,1,0,-1,-1,-1};
const vector<ll> dj{1,0,-1,0};
const vector<ll> dj8{1,1,0,-1,-1,-1,0,1};

#ifndef INV_TABLE_HPP_
#define INV_TABLE_HPP_

#include <vector>
#include <cassert>
using uint = unsigned;
using ull = unsigned long long;
using ll = long long;
using namespace std;

/// @brief mod M上での階乗,逆元テーブルを保持する構造体
/// @tparam M 
template<uint M>
struct mod_table{
    vector<uint> invmodlist;
    vector<uint> factorialmodlist;
    vector<uint> factorialmodinvlist;
    uint N_MAX;
    constexpr mod_table(const uint N_MAX__){
        N_MAX = max(1u, N_MAX__);
        invmodlist = vector<uint>(N_MAX+1);
        factorialmodlist = vector<uint>(N_MAX+1);
        factorialmodinvlist = vector<uint>(N_MAX+1);
        invmodlist[1] = 1;
        for (uint i = 2; i <= N_MAX; i++){
            invmodlist[i] = (M-M/i)*(ull)invmodlist[M%i]%M;
        }
        factorialmodinvlist[0] = 1;
        factorialmodlist[0] = 1;
        for (ull i = 1; i <= N_MAX; i++){
            factorialmodinvlist[i] = (invmodlist[i]*(ull)factorialmodinvlist[i-1])%M;
            factorialmodlist[i] = (factorialmodlist[i-1]*i)%M;
        }
    }
    ll inverse_mod(ll x){
        assert(0 <= x and x < N_MAX);
        return invmodlist[x];
    }
    ll factorialmod(ll x){
        assert(0 <= x and x < N_MAX);
        return factorialmodlist[x];
    }
    ll factorialmodinv(ll x){
        assert(0 <= x and x < N_MAX);
        return factorialmodinvlist[x];
    }
};

#endif /* INV_TABLE_HPP_ */
#ifndef FPS__HPP_
#define FPS__HPP_

#include <iostream>
#include <iterator>
#include <vector>
#include <cassert>
#ifndef MODINT__HPP_
#define MODINT__HPP_

#include <iostream>
#ifndef MATH_FUNCTION_HPP_
#define MATH_FUNCTION_HPP_

#include <array>
#include <cmath>
#include <cassert>
using namespace std;
using ll = long long;
using ld = long double;
using ull = unsigned long long;
using ulll = __uint128_t;
#ifdef BOOST_VERSION
using bll = boost::multiprecision::cpp_int;
#endif


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

/// @brief a^nを返す。bに関して対数時間で計算できる。
template<typename T> constexpr T powll(T a, T n){
    T t = a;
    T ans = 1;
    while (n > 0){
        if (n%2){
            ans *= t;
        }
        n >>= 1;
        t *= t;
    }
    return ans;
}
/// @brief a^nを返す。bに関して対数時間で計算できる
template<> constexpr ll powll(ll a, ll n){
    ll t = a;
    ll ans = 1;
    while (n > 0){
        if (n&1){
            ans *= t;
        }
        n >>= 1;
        t *= t;
    }
    return ans;
}


/// @brief floor(sqrt(N))を返す。1.5×10^19まで対応
constexpr ll isqrt(ull N){
    assert(N <= 15000000000000000000ull);
    ull ret = sqrt(N);
    while (ret*ret > N){
        ret--;
    }
    while ((ret+1)*(ret+1) <= N){
        ret++;
    }
    return ret;
}
#ifdef BOOST_VERSION
/// @brief floor(sqrt(N))を返す。多倍長整数に対応
constexpr bll isqrt_large(bll N){
    bll ret = sqrt(N);
    while (ret*ret > N){
        ret--;
    }
    while ((ret+1)*(ret+1) <= N){
        ret++;
    }
    return ret;
}
#endif

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

/// @brief 有理数のfloorを求める。 floor(y/x)
template<typename T> constexpr inline T floor2(T y, T x){
    if ((x^y) > 0){
        x = x > 0 ? x : -x;
        y = y > 0 ? y : -y;
        return y/x;
    }
    else if ((x^y) < 0){
        x = x > 0 ? x : -x;
        y = y > 0 ? y : -y;
        return -((y+x-1)/x);
    }
    else{
        return y/x;
    }
}
/// @brief 有理数のceilを求める。 ceil(y/x)
template<typename T> constexpr inline T ceil2(T y, T x){
    if ((x^y) > 0){
        x = x > 0 ? x : -x;
        y = y > 0 ? y : -y;
        return (y+x-1)/x;
    }
    else if ((x^y) < 0){
        x = x > 0 ? x : -x;
        y = y > 0 ? y : -y;
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
inline constexpr ll cipolla(ll a){
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
inline constexpr ll cipolla(ll a, const ll M){
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
constexpr ull lowerpow2(ull x){
    if (x == 0){return 0;}
    return 1ull<<(63-__builtin_clzll(x));
}
/// @brief x以上の最小の2冪を返す。0は0が返る。 
constexpr ull upperpow2(ull x){
    if (x == 0){return 0;}
    if (x == 1){return 1;}
    return 1ull<<(64-__builtin_clzll(x-1));
}
/// @brief xのpopcountを求める 
constexpr int popcount(ull x){
    return __builtin_popcountll(x);
}
/// @brief xのbit lengthを求める。
constexpr int bit_length(ull x){
    if (x == 0){return 0;}
    return 64-__builtin_clzll(x);
}




#endif /* MATH_FUNCTION_HPP_ */

using ll = long long;
using uint = unsigned int;
using ull = unsigned long long;
using namespace std;

template <uint M>
struct constant_modint {
    uint val;
    constant_modint() : val(0) {}
    template <class T> constant_modint(T x) {
        ll y = (ll)(x % (ll)M);
        if (y < 0) y += M;
        val = y;
    }

    constant_modint& operator+=(const constant_modint& rhs) {
        val += rhs.val;
        if (val >= M) val -= M;
        return *this;
    }
    constant_modint& operator-=(const constant_modint& rhs) {
        if (val < rhs.val) val += M;
        val -= rhs.val;
        return *this;
    }
    constant_modint& operator*=(const constant_modint& rhs) {
        val = (ull)val * rhs.val % M;
        return *this;
    }
    constant_modint& operator/=(const constant_modint& rhs) {
        return *this *= rhs.inv();
    }

    constant_modint operator+() const { return *this; }
    constant_modint operator-() const { return constant_modint(0) - *this; }

    friend constant_modint operator+(const constant_modint& lhs, const constant_modint& rhs) { return constant_modint(lhs) += rhs; }
    friend constant_modint operator-(const constant_modint& lhs, const constant_modint& rhs) { return constant_modint(lhs) -= rhs; }
    friend constant_modint operator*(const constant_modint& lhs, const constant_modint& rhs) { return constant_modint(lhs) *= rhs; }
    friend constant_modint operator/(const constant_modint& lhs, const constant_modint& rhs) { return constant_modint(lhs) /= rhs; }

    friend bool operator==(const constant_modint& lhs, const constant_modint& rhs) { return lhs.val == rhs.val; }
    friend bool operator!=(const constant_modint& lhs, const constant_modint& rhs) { return lhs.val != rhs.val; }

    constant_modint pow(ull n) const {
        constant_modint res = 1, a = *this;
        while (n) {
            if (n&1) res *= a;
            a *= a;
            n >>= 1;
        }
        return res;
    }
    constant_modint inv() const {
        return inverse_mod((ll)val, M);
    }

    friend std::ostream& operator<<(std::ostream& os, const constant_modint& m) {
        return os << m.val;
    }
    friend std::istream& operator>>(std::istream& is, constant_modint& m) {
        ll x;
        is >> x;
        m = constant_modint(x);
        return is;
    }
};

template <int id = -1>
struct dynamic_modint {
    uint val;
    static uint& mod() {
        static uint M = 998244353;
        return M;
    }
    static void set_mod(uint m) {
        mod() = m;
    }

    dynamic_modint() : val(0) {}
    template <class T> dynamic_modint(T x) {
        ll y = x % (ll)mod();
        if (y < 0) y += mod();
        val = (uint)y;
    }

    dynamic_modint& operator+=(const dynamic_modint& rhs) {
        val += rhs.val;
        if (val >= mod()) val -= mod();
        return *this;
    }
    dynamic_modint& operator-=(const dynamic_modint& rhs) {
        if (val < rhs.val) val += mod();
        val -= rhs.val;
        return *this;
    }
    dynamic_modint& operator*=(const dynamic_modint& rhs) {
        val = (ull)val * rhs.val % mod();
        return *this;
    }
    dynamic_modint& operator/=(const dynamic_modint& rhs) {
        return *this *= rhs.inv();
    }

    dynamic_modint operator+() const { return *this; }
    dynamic_modint operator-() const { return dynamic_modint(0) - *this; }

    friend dynamic_modint operator+(const dynamic_modint& lhs, const dynamic_modint& rhs) { return dynamic_modint(lhs) += rhs; }
    friend dynamic_modint operator-(const dynamic_modint& lhs, const dynamic_modint& rhs) { return dynamic_modint(lhs) -= rhs; }
    friend dynamic_modint operator*(const dynamic_modint& lhs, const dynamic_modint& rhs) { return dynamic_modint(lhs) *= rhs; }
    friend dynamic_modint operator/(const dynamic_modint& lhs, const dynamic_modint& rhs) { return dynamic_modint(lhs) /= rhs; }

    friend bool operator==(const dynamic_modint& lhs, const dynamic_modint& rhs) { return lhs.val == rhs.val; }
    friend bool operator!=(const dynamic_modint& lhs, const dynamic_modint& rhs) { return lhs.val != rhs.val; }

    dynamic_modint pow(ull n) const {
        dynamic_modint res = 1, a = *this;
        while (n) {
            if (n & 1) res *= a;
            a *= a;
            n >>= 1;
        }
        return res;
    }
    dynamic_modint inv() const {
        ll a = val, b = mod(), u = 1, v = 0;
        while (b) {
            ll t = a / b;
            a -= t * b; swap(a, b);
            u -= t * v; swap(u, v);
        }
        return dynamic_modint(u);
    }

    friend std::ostream& operator<<(std::ostream& os, const dynamic_modint& m) {
        return os << m.val;
    }
    friend std::istream& operator>>(std::istream& is, dynamic_modint& m) {
        ll x;
        is >> x;
        m = dynamic_modint(x);
        return is;
    }
};

#endif /* MODINT__HPP_ */

using namespace std;
using ll = long long;
using uint = unsigned;


template<uint T>
struct FormalPowerSeries{
    using mint = constant_modint<T>;
    vector<mint> f;
    uint sz;
    FormalPowerSeries(){
        sz = 0;
    }
    /// @brief 指定したサイズのfpsを作成する。
    FormalPowerSeries(uint _init_sz){
        f.resize(_init_sz, 0);
        sz = _init_sz;
    }
    FormalPowerSeries(uint _init_sz, mint _init_val){
        f.resize(_init_sz, _init_val);
        sz = _init_sz;
    }
    template<typename U> FormalPowerSeries(const vector<U>& _init){
        f.assign(_init.begin(), _init.end());
        sz = f.size();
    }
    template<typename U> FormalPowerSeries(initializer_list<U> _init) : FormalPowerSeries(vector<U>(_init)){}
    uint size() const {return sz;}
    void resize(uint _new_size){
        if (sz == _new_size){return;}
        f.resize(_new_size, 0);
        sz = _new_size;
    }
    inline mint& operator[](uint deg) {return f[deg];}
    inline const mint& operator[](uint deg) const {return f[deg];}
    template<typename U> operator vector<U>() const {
        vector<U> res(sz);
        for (uint i = 0; i < sz; i++){
            res[i] = f[i].val;
        }
        return res;
    }

    void differential(){
        if (sz == 0){return;}
        for (uint i = 0; i < sz-1; i++){
            f[i].val = (i+1)*(ull)f[i+1].val%T;
        }
        f.pop_back();
        sz--;
    }
    void integral(const mod_table<T>& mtable){
        if (sz == 0){return;}
        sz++;
        f.push_back(0);
        for (uint i = sz; i > 0; i--){
            f[i].val = mtable.invmodlist[i]*(ull)f[i-1].val%T;
        }
    }
};

#endif /* FPS__HPP_ */
#ifndef FPS_OPERATION_HPP_
#define FPS_OPERATION_HPP_

#include <vector>
#include <algorithm>
#include <immintrin.h>



#ifndef PRIME_AND_DIVISORS_HPP_
#define PRIME_AND_DIVISORS_HPP_

#include <array>
#include <vector>

#include <algorithm>
#include <cassert>
#include <numeric>
#include <iostream>
using namespace std;
using ll = long long;
using ulll = __uint128_t;
using pll = array<ll,2>;
using pull = array<ull,2>;
using pii = array<int,2>;



/// @brief 試し割り法で正の整数Nを素因数分解する
/// @return vector<array<ll,2>>{{素因数1,個数}, {素因数2,個数}, {素因数3,個数}...}
template<typename T> constexpr vector<array<T,2>> trial_division(T N){
    assert(N > 0);
    if (N == 1){
        return vector<array<T,2>> {{1,0}};
    }
    vector<array<T,2>> R;//戻り値用リスト

    const T M = isqrt(static_cast<ull>(N));
    for (T i = 2; i <= M; i++){
        if (N % i == 0){
            T divide_count = 0;
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
template<typename T> constexpr T divisor_function(const vector<array<T,2>>& vv, T K, const T MOD = -1){
    if (vv[0][0] == 1){
        return 1;
    }
    T R = 1;
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
template<typename T> constexpr vector<T> enumerate_divisor(const vector<array<T,2>>& p){
    vector<T> d{1};
    if (p[0][0] == 1){
        return d;
    }
    for (auto &v : p){
        T t = d.size();
        T temp = 1;
        for (T w = 0; w < v[1]; w++){
            temp *= v[0];
            for (T i = 0; i < t; i++){
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
    vector<pii> factorize(int x){
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
    vector<vector<pii>> factorize_all(int N){
        vector<vector<pii>> r(N+1);
        r[1].push_back({1,0});
        for (int i = 2; i <= N; i++){
            r[i] = factorize(i);
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


///@brief ミラーラビン素数判定法
template<typename T> constexpr bool MillerRabin(T N){
    if (N <= 1) return false;
    if (N == 2 || N == 3) return true;
    if (N % 2 == 0) return false;

    T d = N - 1;
    T s = 0;
    while (d % 2 == 0){
        d >>= 1;
        s++;
    }
    auto modpow_T = [&](T a,T b,T m){
        T t = a%m;
        T ans = (m == 1 ? 0 : 1);
        while (b > 0){
            if (b&1){
                ans = (ans*t)%m;
            }
            b >>= 1;
            t = (t*t)%m;
        }
        return ans;
    };
    auto check = [&](T a){
        if (a >= N) a %= N;
        if (a == 0) return true;
        T x = modpow_T(a, d, N);
        if (x == 1 || x == N-1) return true;
        for (int r = 1; r < s; r++){
            x = x*x%N;
            if (x == N - 1) return true;
        }
        return false;
    };

    if (N < 4759123141){
        for (auto a : __MillerRabin_small){
            if (!check(static_cast<T>(a))) return false;
        }
    }
    else if (N < 1000000000000000000){
        for (auto a : __MillerRabin_large){
            if (!check(static_cast<T>(a))) return false;
        }
    }
    else{
        for (auto a : __MillerRabin_large){
            if (!check(static_cast<T>(a))) return false;
        }
        //50個の整数でテスト
        for (T a = 638245987265732141; a < 638245987265732191; a++){
            if (!check(a)) return false;
        }
    }
    return true;
}

/// @brief ミラーラビン素数判定法のull範囲限定版
template<> constexpr bool MillerRabin(ull N){
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
template<typename T> constexpr vector<array<T, 2>> Pollard_rho(T N){
    assert(N > 0);
    if (N == 1){return {{1,0}};}
    vector<T> res;
    vector<T> dq;
    dq.push_back(N);
    while (!dq.empty()){
        T n = dq.back();
        dq.pop_back();
        bool next_loop = false;
        while (!MillerRabin<T>(n)){
            if ((n&1) == 0){
                while ((n&1) == 0){res.push_back(2); n>>=1;}
                if (n == 1){next_loop = true; break;}
                continue;
            }
            T blocksize = sqrt(sqrt(sqrt(n)));
            for (T c = 1; c <= n; c++){
                T y = (0b1010110101101011110011ull^c^n)%n;
                T r = 1;
                T k = 0;
                T k_mod_blocksize = 0;
                bool next_loop_inner = false;
                for (int _ = 0;;){
                    T q = 1;
                    T q_old = q;
                    T y_old = y;
                    T x = y;
                    while (k < r){
                        k++;
                        k_mod_blocksize++;
                        y = y*y%n+c; if (y >= n){ y -= n;}
                        q = q*(y >= x ? (y-x) : (x-y))%n;
                        if (k_mod_blocksize == blocksize){
                            k_mod_blocksize -= blocksize;
                            T g = gcd(q,n);
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
                                        y = y*y%n+c; if (y >= n){ y -= n;}
                                        q = q*(y >= x ? (y-x) : (x-y))%n;
                                        g = gcd(q,n);
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
                    q_old = gcd(q,n);
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
    vector<array<T,2>> res2;
    for (ull l = 0, r = 0, sz = res.size(); l < sz;){
        while (r < sz && res[l] == res[r]){
            r++;
        }
        res2.push_back({res[l], r-l});
        l = r;
    }
    return res2;
}

/// @brief ポラード・ロー法による素因数分解のull範囲限定版
template<> constexpr vector<pull> Pollard_rho(ull N){
    assert(N > 0);
    if (N == 1){return {{1,0}};}
    vector<ull> res;
    vector<ull> dq;
    dq.push_back(N);
    while (!dq.empty()){
        ull n = dq.back();
        dq.pop_back();
        bool next_loop = false;
        while (!MillerRabin<ull>(n)){
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
    vector<pull> res2;
    for (int l = 0, r = 0, sz = res.size(); l < sz;){
        while (r < sz && res[l] == res[r]){
            r++;
        }
        res2.push_back({res[l], static_cast<ull>(r-l)});
        l = r;
    }
    return res2;
}


/// @brief 素因数分解を行う。`N`の大きさによって、試し割り法とポラード・ロー法を使い分けてくれる。
template<typename T> constexpr vector<array<T, 2>> factorize(T N){
    if (N < 640000){
        return trial_division<T>(N);
    }
    if (N < 1000000000000000000){
        return Pollard_rho<T>(N);
    }
    return Pollard_rho<T>(N);
}
template<> constexpr vector<pull> factorize(ull N){
    if (N < 640000ull){
        return trial_division<ull>(N);
    }
    return Pollard_rho<ull>(N);
}
template<> constexpr vector<pll> factorize(ll N){
    auto res = factorize<ull>(N);
    vector<pll> res2(res.size());
    for (int i = 0, sz = res.size(); i < sz; i++){
        res2[i][0] = res[i][0];
        res2[i][1] = res[i][1];
    }
    return res2;
}
template<> constexpr vector<pii> factorize(int N){
    auto res = factorize<ull>(N);
    vector<pii> res2(res.size());
    for (int i = 0, sz = res.size(); i < sz; i++){
        res2[i][0] = res[i][0];
        res2[i][1] = res[i][1];
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

/// @brief mod Pにおける原始根を1つ探す。Pはull範囲内の素数でなければならない。
constexpr ull primitive_root(ull P){
    assert(MillerRabin<ull>(P));
    if (P == 2){
        return 1;
    }
    Montgomery64 mg(P);
    Xorshift64 rng(0x1234567E89ABCDEF);
    auto factorized = factorize<ull>(P-1);
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
using namespace std;
using ll = long long;
using uint = unsigned;
using ull = unsigned long long;

template<typename T> inline bool btest_for_fps(T K, int i){return K&(1ull<<i);}


/// @brief mod M上での形式的冪級数の計算を行う構造体
template<uint M>
struct fps_operator{
    uint sum_e[30];
    uint sum_ie[30];
    uint log_max_length;
    uint last_powroot;

    // --- モンゴメリ乗算用の定数と関数 ---
    static constexpr uint get_r() {
        uint res = M;
        for (int i = 0; i < 4; ++i) res *= 2 - M * res;
        return res;
    }
    static constexpr uint R = get_r();
    static constexpr uint R2 = -ull(M) % M;
    static constexpr uint NEG_INV = 0 - R;

    static inline constexpr uint reduce(ull x) {
        uint res = (x + ull(uint(x) * NEG_INV) * M) >> 32;
        return res >= M ? res - M : res;
    }
    static inline constexpr uint to_montgomery(uint x) {
        return reduce(ull(x) * R2);
    }
    static inline constexpr uint from_montgomery(uint x) {
        return reduce(x);
    }
    static inline constexpr uint montgomery_mul(uint x, uint y) {
        return reduce(ull(x) * y);
    }
    static inline constexpr uint mod_add(uint x, uint y) {
        return x + y >= M ? x + y - M : x + y;
    }
    static inline constexpr uint mod_sub(uint x, uint y) {
        return x < y ? x + M - y : x - y;
    }

    // --- AVX-512 / AVX2 SIMD Intrinsics ---
#if defined(__AVX512F__) && defined(__AVX512DQ__)
    static inline __m512i montgomery_mul_simd(__m512i a, __m512i b, __m512i mod, __m512i neg_inv) {
        __m512i mul0 = _mm512_mul_epu32(a, b);
        __m512i mul1 = _mm512_mul_epu32(_mm512_srli_epi64(a, 32), _mm512_srli_epi64(b, 32));
        __m512i q0 = _mm512_mul_epu32(_mm512_mullo_epi32(mul0, neg_inv), mod);
        __m512i q1 = _mm512_mul_epu32(_mm512_mullo_epi32(mul1, neg_inv), mod);
        __m512i res0 = _mm512_srli_epi64(_mm512_add_epi64(mul0, q0), 32);
        __m512i res1 = _mm512_srli_epi64(_mm512_add_epi64(mul1, q1), 32);
        __m512i res = _mm512_or_si512(res0, _mm512_slli_epi64(res1, 32));
        __mmask16 cmp = _mm512_cmpge_epu32_mask(res, mod);
        return _mm512_mask_sub_epi32(res, cmp, res, mod);
    }
    static inline __m512i mod_add_simd(__m512i a, __m512i b, __m512i mod) {
        __m512i res = _mm512_add_epi32(a, b);
        __mmask16 cmp = _mm512_cmpge_epu32_mask(res, mod);
        return _mm512_mask_sub_epi32(res, cmp, res, mod);
    }
    static inline __m512i mod_sub_simd(__m512i a, __m512i b, __m512i mod) {
        __mmask16 cmp = _mm512_cmplt_epu32_mask(a, b);
        __m512i res = _mm512_sub_epi32(a, b);
        return _mm512_mask_add_epi32(res, cmp, res, mod);
    }
#elif defined(__AVX2__)
    // AVX2用 モンゴメリ乗算・剰余加減算
    static inline __m256i montgomery_mul_simd(__m256i a, __m256i b, __m256i mod, __m256i neg_inv) {
        __m256i mul0 = _mm256_mul_epu32(a, b);
        __m256i mul1 = _mm256_mul_epu32(_mm256_srli_epi64(a, 32), _mm256_srli_epi64(b, 32));
        __m256i q0 = _mm256_mul_epu32(_mm256_mullo_epi32(mul0, neg_inv), mod);
        __m256i q1 = _mm256_mul_epu32(_mm256_mullo_epi32(mul1, neg_inv), mod);
        __m256i res0 = _mm256_srli_epi64(_mm256_add_epi64(mul0, q0), 32);
        __m256i res1 = _mm256_srli_epi64(_mm256_add_epi64(mul1, q1), 32);
        
        __m256i res = _mm256_blend_epi32(res0, _mm256_slli_epi64(res1, 32), 0xAA);
        __m256i diff = _mm256_sub_epi32(res, mod);
        // diff < 0 なら -1 (全ビット1)、そうでないなら 0 となるマスク
        __m256i mask = _mm256_srai_epi32(diff, 31);
        return _mm256_add_epi32(diff, _mm256_and_si256(mask, mod));
    }
    static inline __m256i mod_add_simd(__m256i a, __m256i b, __m256i mod) {
        __m256i diff = _mm256_sub_epi32(_mm256_add_epi32(a, b), mod);
        __m256i mask = _mm256_srai_epi32(diff, 31);
        return _mm256_add_epi32(diff, _mm256_and_si256(mask, mod));
    }
    static inline __m256i mod_sub_simd(__m256i a, __m256i b, __m256i mod) {
        __m256i diff = _mm256_sub_epi32(a, b);
        __m256i mask = _mm256_srai_epi32(diff, 31);
        return _mm256_add_epi32(diff, _mm256_and_si256(mask, mod));
    }
#endif


    constexpr fps_operator(){
        vector<ll> powroot{1};
        vector<ll> powrootinv;
        while (powroot.back() >= 0){
            powroot.push_back(cipolla<M>(powroot.back()));
            if (powroot.back() == powroot[powroot.size()-2]){
                powroot.back() = M-powroot.back();
            }
        }
        powroot.pop_back();
        log_max_length = powroot.size()-1;
        last_powroot = powroot.back();
        for (auto v : powroot){
            powrootinv.push_back(inverse_mod<ll,ll>(v,M));
        }
        int cnt2 = powroot.size()-1;
        uint now = reduce(R2);
        for (int i = 0; i <= cnt2-2; i++){
            sum_e[i] = montgomery_mul(to_montgomery(powroot[i+2]), now);
            now = montgomery_mul(now, to_montgomery(powrootinv[i+2]));
        }
        uint inow = reduce(R2);
        for (int i = 0; i <= cnt2-2; i++){
            sum_ie[i] = montgomery_mul(to_montgomery(powrootinv[i+2]), inow);
            inow = montgomery_mul(inow, to_montgomery(powroot[i+2]));
        }
        for (int i = cnt2-1; i < 30; i++){
            sum_e[i] = 0;
            sum_ie[i] = 0;
        }
    }

    void inplaceDFT(FormalPowerSeries<M>& F) const {
        F.resize(upperpow2(F.sz));
        int n = F.sz;
        if (n == 0) return;
        int h = __builtin_ctz(n);
        
        for (int ph = 1; ph <= h; ph++) {
            int w = 1 << (ph - 1), p = 1 << (h - ph);
            uint now = to_montgomery(1);
            for (int s = 0; s < w; s++) {
                int offset = s << (h - ph + 1);
#if defined(__AVX512F__) && defined(__AVX512DQ__)
                if (p >= 16) {
                    __m512i vmod = _mm512_set1_epi32(M);
                    __m512i vneg_inv = _mm512_set1_epi32(NEG_INV);
                    __m512i vnow = _mm512_set1_epi32(now);
                    for (int i = 0; i < p; i += 16) {
                        __m512i l = _mm512_loadu_si512((__m512i*)&F[i + offset].val);
                        __m512i r = _mm512_loadu_si512((__m512i*)&F[i + offset + p].val);
                        __m512i r_now = montgomery_mul_simd(r, vnow, vmod, vneg_inv);
                        _mm512_storeu_si512((__m512i*)&F[i + offset].val, mod_add_simd(l, r_now, vmod));
                        _mm512_storeu_si512((__m512i*)&F[i + offset + p].val, mod_sub_simd(l, r_now, vmod));
                    }
                } else
#elif defined(__AVX2__)
                if (p >= 8) {
                    __m256i vmod = _mm256_set1_epi32(M);
                    __m256i vneg_inv = _mm256_set1_epi32(NEG_INV);
                    __m256i vnow = _mm256_set1_epi32(now);
                    for (int i = 0; i < p; i += 8) {
                        __m256i l = _mm256_loadu_si256((__m256i*)&F[i + offset].val);
                        __m256i r = _mm256_loadu_si256((__m256i*)&F[i + offset + p].val);
                        __m256i r_now = montgomery_mul_simd(r, vnow, vmod, vneg_inv);
                        _mm256_storeu_si256((__m256i*)&F[i + offset].val, mod_add_simd(l, r_now, vmod));
                        _mm256_storeu_si256((__m256i*)&F[i + offset + p].val, mod_sub_simd(l, r_now, vmod));
                    }
                } else
#endif
                {
                    for (int i = 0; i < p; i++) {
                        uint l = F[i + offset].val;
                        uint r = montgomery_mul(F[i + offset + p].val, now);
                        F[i + offset].val = mod_add(l, r);
                        F[i + offset + p].val = mod_sub(l, r);
                    }
                }
                now = montgomery_mul(now, sum_e[__builtin_ctz(~s)]);
            }
        }
    }
    void inplaceIDFT(FormalPowerSeries<M>& F) const {
        F.resize(upperpow2(F.sz));
        int n = F.sz;
        if (n == 0) return;
        int h = __builtin_ctz(n);
        
        for (int ph = h; ph >= 1; ph--) {
            int w = 1 << (ph - 1), p = 1 << (h - ph);
            uint inow = to_montgomery(1);
            for (int s = 0; s < w; s++) {
                int offset = s << (h - ph + 1);
#if defined(__AVX512F__) && defined(__AVX512DQ__)
                if (p >= 16) {
                    __m512i vmod = _mm512_set1_epi32(M);
                    __m512i vneg_inv = _mm512_set1_epi32(NEG_INV);
                    __m512i vinow = _mm512_set1_epi32(inow);
                    for (int i = 0; i < p; i += 16) {
                        __m512i l = _mm512_loadu_si512((__m512i*)&F[i + offset].val);
                        __m512i r = _mm512_loadu_si512((__m512i*)&F[i + offset + p].val);
                        _mm512_storeu_si512((__m512i*)&F[i + offset].val, mod_add_simd(l, r, vmod));
                        _mm512_storeu_si512((__m512i*)&F[i + offset + p].val, montgomery_mul_simd(mod_sub_simd(l, r, vmod), vinow, vmod, vneg_inv));
                    }
                } else
#elif defined(__AVX2__)
                if (p >= 8) {
                    __m256i vmod = _mm256_set1_epi32(M);
                    __m256i vneg_inv = _mm256_set1_epi32(NEG_INV);
                    __m256i vinow = _mm256_set1_epi32(inow);
                    for (int i = 0; i < p; i += 8) {
                        __m256i l = _mm256_loadu_si256((__m256i*)&F[i + offset].val);
                        __m256i r = _mm256_loadu_si256((__m256i*)&F[i + offset + p].val);
                        _mm256_storeu_si256((__m256i*)&F[i + offset].val, mod_add_simd(l, r, vmod));
                        _mm256_storeu_si256((__m256i*)&F[i + offset + p].val, montgomery_mul_simd(mod_sub_simd(l, r, vmod), vinow, vmod, vneg_inv));
                    }
                } else
#endif
                {
                    for (int i = 0; i < p; i++) {
                        uint l = F[i + offset].val;
                        uint r = F[i + offset + p].val;
                        F[i + offset].val = mod_add(l, r);
                        F[i + offset + p].val = montgomery_mul(mod_sub(l, r), inow);
                    }
                }
                inow = montgomery_mul(inow, sum_ie[__builtin_ctz(~s)]);
            }
        }
    }
    void inplaceDFT_T(FormalPowerSeries<M>& F) const {
        F.resize(upperpow2(F.sz));
        int n = F.sz;
        if (n == 0) return;
        int h = __builtin_ctz(n);
        
        for (int ph = h; ph >= 1; ph--) {
            int w = 1 << (ph - 1), p = 1 << (h - ph);
            uint32_t now = to_montgomery(1);
            for (int s = 0; s < w; s++) {
                int offset = s << (h - ph + 1);
#if defined(__AVX512F__) && defined(__AVX512DQ__)
                if (p >= 16) {
                    __m512i vmod = _mm512_set1_epi32(M);
                    __m512i vneg_inv = _mm512_set1_epi32(NEG_INV);
                    __m512i vnow = _mm512_set1_epi32(now);
                    for (int i = 0; i < p; i += 16) {
                        __m512i l = _mm512_loadu_si512((__m512i*)&F[i + offset].val);
                        __m512i r = _mm512_loadu_si512((__m512i*)&F[i + offset + p].val);
                        _mm512_storeu_si512((__m512i*)&F[i + offset].val, mod_add_simd(l, r, vmod));
                        _mm512_storeu_si512((__m512i*)&F[i + offset + p].val, montgomery_mul_simd(mod_sub_simd(l, r, vmod), vnow, vmod, vneg_inv));
                    }
                } else
#elif defined(__AVX2__)
                if (p >= 8) {
                    __m256i vmod = _mm256_set1_epi32(M);
                    __m256i vneg_inv = _mm256_set1_epi32(NEG_INV);
                    __m256i vnow = _mm256_set1_epi32(now);
                    for (int i = 0; i < p; i += 8) {
                        __m256i l = _mm256_loadu_si256((__m256i*)&F[i + offset].val);
                        __m256i r = _mm256_loadu_si256((__m256i*)&F[i + offset + p].val);
                        _mm256_storeu_si256((__m256i*)&F[i + offset].val, mod_add_simd(l, r, vmod));
                        _mm256_storeu_si256((__m256i*)&F[i + offset + p].val, montgomery_mul_simd(mod_sub_simd(l, r, vmod), vnow, vmod, vneg_inv));
                    }
                } else
#endif
                {
                    for (int i = 0; i < p; i++) {
                        uint32_t l = F[i + offset].val;
                        uint32_t r = F[i + offset + p].val;
                        F[i + offset].val = mod_add(l, r);
                        F[i + offset + p].val = montgomery_mul(mod_sub(l, r), now);
                    }
                }
                now = montgomery_mul(now, sum_e[__builtin_ctz(~s)]);
            }
        }
    }
    void inplaceIDFT_T(FormalPowerSeries<M>& F) const {
        F.resize(upperpow2(F.sz));
        int n = F.sz;
        if (n == 0) return;
        int h = __builtin_ctz(n);
        
        for (int ph = 1; ph <= h; ph++) {
            int w = 1 << (ph - 1), p = 1 << (h - ph);
            uint32_t inow = to_montgomery(1);
            for (int s = 0; s < w; s++) {
                int offset = s << (h - ph + 1);
#if defined(__AVX512F__) && defined(__AVX512DQ__)
                if (p >= 16) {
                    __m512i vmod = _mm512_set1_epi32(M);
                    __m512i vneg_inv = _mm512_set1_epi32(NEG_INV);
                    __m512i vinow = _mm512_set1_epi32(inow);
                    for (int i = 0; i < p; i += 16) {
                        __m512i l = _mm512_loadu_si512((__m512i*)&F[i + offset].val);
                        __m512i r = _mm512_loadu_si512((__m512i*)&F[i + offset + p].val);
                        __m512i r_inow = montgomery_mul_simd(r, vinow, vmod, vneg_inv);
                        _mm512_storeu_si512((__m512i*)&F[i + offset].val, mod_add_simd(l, r_inow, vmod));
                        _mm512_storeu_si512((__m512i*)&F[i + offset + p].val, mod_sub_simd(l, r_inow, vmod));
                    }
                } else
#elif defined(__AVX2__)
                if (p >= 8) {
                    __m256i vmod = _mm256_set1_epi32(M);
                    __m256i vneg_inv = _mm256_set1_epi32(NEG_INV);
                    __m256i vinow = _mm256_set1_epi32(inow);
                    for (int i = 0; i < p; i += 8) {
                        __m256i l = _mm256_loadu_si256((__m256i*)&F[i + offset].val);
                        __m256i r = _mm256_loadu_si256((__m256i*)&F[i + offset + p].val);
                        __m256i r_inow = montgomery_mul_simd(r, vinow, vmod, vneg_inv);
                        _mm256_storeu_si256((__m256i*)&F[i + offset].val, mod_add_simd(l, r_inow, vmod));
                        _mm256_storeu_si256((__m256i*)&F[i + offset + p].val, mod_sub_simd(l, r_inow, vmod));
                    }
                } else
#endif
                {
                    for (int i = 0; i < p; i++) {
                        uint32_t l = F[i + offset].val;
                        uint32_t r = montgomery_mul(F[i + offset + p].val, inow);
                        F[i + offset].val = mod_add(l, r);
                        F[i + offset + p].val = mod_sub(l, r);
                    }
                }
                inow = montgomery_mul(inow, sum_ie[__builtin_ctz(~s)]);
            }
        }
    }
    FormalPowerSeries<M> convolution(FormalPowerSeries<M> F1, FormalPowerSeries<M> F2) const {
        int n = F1.size();
        int m = F2.size();
        if (n == 0 || m == 0) return FormalPowerSeries<M>(0);
        
        if (std::min(n, m) <= 60) {
            if (n < m) {
                std::swap(n, m);
                std::swap(F1.sz, F2.sz);
                std::swap(F1.f, F2.f);
            }
            FormalPowerSeries<M> ans(n + m - 1);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    ans[i + j].val = (ans[i + j].val + ull(F1[i].val) * F2[j].val) % M;
                }
            }
            return ans;
        }

        int reference_size = upperpow2(n + m - 1);
        F1.resize(reference_size);
        F2.resize(reference_size);

        for (int i = 0; i < reference_size; i++) {
            F1[i].val = to_montgomery(F1[i].val);
            F2[i].val = to_montgomery(F2[i].val);
        }

        inplaceDFT(F1);
        inplaceDFT(F2);

        int i = 0;
#if defined(__AVX512F__) && defined(__AVX512DQ__)
        __m512i vmod512 = _mm512_set1_epi32(M);
        __m512i vneg_inv512 = _mm512_set1_epi32(NEG_INV);
        for (; i + 15 < reference_size; i += 16) {
            __m512i a = _mm512_loadu_si512((__m512i*)&F1[i].val);
            __m512i b = _mm512_loadu_si512((__m512i*)&F2[i].val);
            _mm512_storeu_si512((__m512i*)&F1[i].val, montgomery_mul_simd(a, b, vmod512, vneg_inv512));
        }
#elif defined(__AVX2__)
        __m256i vmod256 = _mm256_set1_epi32(M);
        __m256i vneg_inv256 = _mm256_set1_epi32(NEG_INV);
        for (; i + 7 < reference_size; i += 8) {
            __m256i a = _mm256_loadu_si256((__m256i*)&F1[i].val);
            __m256i b = _mm256_loadu_si256((__m256i*)&F2[i].val);
            _mm256_storeu_si256((__m256i*)&F1[i].val, montgomery_mul_simd(a, b, vmod256, vneg_inv256));
        }
#endif
        for (; i < reference_size; i++) {
            F1[i].val = montgomery_mul(F1[i].val, F2[i].val);
        }

        inplaceIDFT(F1);

        uint iz_normal = modpow<M>(reference_size, M-2);
        i = 0;
#if defined(__AVX512F__) && defined(__AVX512DQ__)
        __m512i viz_normal512 = _mm512_set1_epi32(iz_normal);
        for (; i + 15 < reference_size; i += 16) {
            __m512i a = _mm512_loadu_si512((__m512i*)&F1[i].val);
            _mm512_storeu_si512((__m512i*)&F1[i].val, montgomery_mul_simd(a, viz_normal512, vmod512, vneg_inv512));
        }
#elif defined(__AVX2__)
        __m256i viz_normal256 = _mm256_set1_epi32(iz_normal);
        for (; i + 7 < reference_size; i += 8) {
            __m256i a = _mm256_loadu_si256((__m256i*)&F1[i].val);
            _mm256_storeu_si256((__m256i*)&F1[i].val, montgomery_mul_simd(a, viz_normal256, vmod256, vneg_inv256));
        }
#endif
        for (; i < reference_size; i++) {
            F1[i].val = reduce(ull(F1[i].val) * iz_normal);
        }

        F1.resize(n + m - 1);
        return F1;
    }
    FormalPowerSeries<M> add(const FormalPowerSeries<M>& F1, const FormalPowerSeries<M>& F2) const {
        FormalPowerSeries<M> ret(max(F1.sz, F2.sz));
        if (F1.sz < F2.sz){
            for (uint i = 0; i < F1.sz; i++){
                ret[i].val = F1[i].val+F2[i].val;
                if (ret[i].val >= M){
                    ret[i].val -= M;
                }
            }
            for (uint i = F1.sz; i < F2.sz; i++){
                ret[i].val = F2[i].val;
            }
        }
        else{
            for (uint i = 0; i < F2.sz; i++){
                ret[i].val = F1[i].val+F2[i].val;
                if (ret[i].val >= M){
                    ret[i].val -= M;
                }
            }
            for (uint i = F2.sz; i < F1.sz; i++){
                ret[i].val = F1[i].val;
            }
        }
        return ret;
    }
    FormalPowerSeries<M> subtract(const FormalPowerSeries<M>& F1, const FormalPowerSeries<M>& F2) const {
        FormalPowerSeries ret(max(F1.sz, F2.sz));
        if (F1.sz < F2.sz){
            for (uint i = 0; i < F1.sz; i++){
                ret[i].val = F1[i].val+M-F2[i].val;
                if (ret[i].val >= M){
                    ret[i].val -= M;
                }
            }
            for (uint i = F1.sz; i < F2.sz; i++){
                ret[i].val = F2[i].val == 0 ? 0 : M-F2[i].val;
            }
        }
        else{
            for (uint i = 0; i < F2.sz; i++){
                ret[i].val = F1[i].val+M-F2[i].val;
                if (ret[i].val >= M){
                    ret[i].val -= M;
                }
            }
            for (uint i = F2.sz; i < F1.sz; i++){
                ret[i].val = F1[i].val;
            }
        }
        return ret;
    }
    /// @brief F*G == 1 mod x^n となるGを求める F[x^0] != 0 が必要 
    FormalPowerSeries<M> inv(const FormalPowerSeries<M>& F, const int n) const {
        int sz_f = F.sz;
        FormalPowerSeries<M> res{inverse_mod<ll,ll>(F[0].val, M)};
        for(int d = 1; d < n; d<<=1){
            FormalPowerSeries<M> f(2*d), g(2*d);
            for(int j = 0; j < 2*d; j++) f[j].val = (j < sz_f ? F[j].val : 0);
            for(int j = 0; j < d; j++) g[j].val = res[j].val;
            inplaceDFT(f);
            inplaceDFT(g);
            for(int j = 0; j < 2*d; j++) f[j].val = f[j].val*(ull)g[j].val%M;
            inplaceIDFT(f);
            ull iz = inverse_mod<ll,ll>(2*d, M);
            for (int i = 0; i < 2*d; i++){
                f[i].val = f[i].val*iz%M;
            }
            for(int j = 0; j < d; j++){
                f[j].val = 0;
                if (f[j+d].val > 0) f[j+d].val = M-f[j+d].val;
            }
            inplaceDFT(f);
            for(int j = 0; j < 2*d; j++) f[j].val = f[j].val*(ull)g[j].val%M;
            inplaceIDFT(f);
            for (int i = 0; i < 2*d; i++){
                f[i].val = f[i].val*iz%M;
            }
            for(int j = 0; j < d; j++) f[j].val = res[j].val;
            res = f;
        }
        res.resize(n);
        return res;
    }
    /// @brief exp(F) mod x^n を求める F[x^0] == 0 が必要
    FormalPowerSeries<M> exp(const FormalPowerSeries<M>& F, const int n, const mod_table<M>& mtable) const {
        assert(F[0].val == 0);
        ull sz_f = F.sz;
        FormalPowerSeries<M> F0{1},f0{1},g0{1};
        for(int d = 1; d < n; d<<=1){
            auto G0 = g0;
            inplaceDFT(G0);
            FormalPowerSeries<M> Delta(d);
            for(int j = 0; j < d; j++) Delta[j].val = F0[j].val*(ull)G0[j].val%M;
            inplaceIDFT(Delta);
            ull iz = inverse_mod<ll,ll>(d, M);
            for (int i = 0; i < d; i++) Delta[i].val = Delta[i].val*iz%M;
            if (Delta[0].val == 0) Delta[0].val = M;
            Delta[0].val -= 1;
            FormalPowerSeries<M> delta(2*d);
            for(int j = 0; j < d; j++) delta[d+j].val = Delta[j].val;
            FormalPowerSeries<M> epsilon(2*d);

            FormalPowerSeries<M> DF0(d-1);
            for (ull j = 0, ulld = d; j < ulld-1; j++) DF0[j].val = f0[j+1].val*(j+1)%M;
            DF0.f.push_back(0);
            DF0.sz++;
            inplaceDFT(DF0);
            for(int j = 0; j < d; j++) DF0[j].val = DF0[j].val*(ull)G0[j].val%M;
            inplaceIDFT(DF0);
            for (int i = 0; i < d; i++) DF0[i].val = DF0[i].val*iz%M;
            for(ull j = 0, ulld = d; j < ulld-1; j++){
                epsilon[j].val += (j+1 < sz_f ? F[j+1].val : 0)*(j+1)%M;
                if (epsilon[j].val >= M) epsilon[j].val -= M;
                epsilon[j+d].val += DF0[j].val;
                if (epsilon[j+d].val >= M) epsilon[j+d].val -= M;
                epsilon[j+d].val += M-(j+1 < sz_f ? F[j+1].val : 0)*(j+1)%M;
                if (epsilon[j+d].val >= M) epsilon[j+d].val -= M;
            }
            epsilon[d-1].val += DF0[d-1].val;
            if (epsilon[d-1].val >= M) epsilon[d-1].val -= M;
            Delta = delta;
            inplaceDFT(Delta);
            FormalPowerSeries<M> DH0(d-1);
            for (ull j = 0, limit = d-1; j < limit; j++) DH0[j].val = (j+1 < sz_f ? F[j+1].val : 0)*(j+1)%M;
            DH0.resize(2*d);
            inplaceDFT(DH0);
            for(int j = 0; j < 2*d; j++) Delta[j].val = Delta[j].val*(ull)DH0[j].val%M;
            inplaceIDFT(Delta);
            iz = inverse_mod<ll,ll>(2*d, M);
            for (int i = 0; i < 2*d; i++) Delta[i].val = Delta[i].val*iz%M;
            for(int j = 0; j < d; j++){
                epsilon[j+d].val += M-Delta[j+d].val;
                if (epsilon[j+d].val >= M){
                    epsilon[j+d].val -= M;
                }
            }
        
            for (ull i = 2*d-1; i >= 1; i--){
                epsilon[i].val = epsilon[i-1].val*(ull)mtable.invmodlist[i]%M+M-(i < sz_f ? F[i].val : 0);
                if (epsilon[i].val >= M) epsilon[i].val -= M;
            }
            epsilon[0].val = (F[0].val != 0 ? M-F[0].val : 0);
            
            auto Epsilon = epsilon;
            inplaceDFT(Epsilon);
            for (int j = 0; j < d; j++){DH0[j].val = f0[j].val; DH0[j+d].val = 0;}
            inplaceDFT(DH0);
            for (int j = 0; j < 2*d; j++) Epsilon[j].val = Epsilon[j].val*(ull)DH0[j].val%M;
            inplaceIDFT(Epsilon);
            for (int i = 0; i < 2*d; i++) Epsilon[i].val = Epsilon[i].val*iz%M;
            f0.resize(2*d);
            for (int j = 0; j < d; j++){
                f0[j+d].val += M-Epsilon[j+d].val;
                if (f0[j+d].val >= M) f0[j+d].val -= M;
            }
            if(2*d >= n) break;
            
            G0.resize(2*d);
            for (int j = 0; j < d; j++) G0[j].val = g0[j].val;
            inplaceDFT(G0);
            F0 = f0;
            inplaceDFT(F0);
            FormalPowerSeries<M> T(2*d);
            for (int j = 0; j < 2*d; j++) T[j].val = F0[j].val*(ull)G0[j].val%M;
            inplaceIDFT(T);
            for (int i = 0; i < 2*d; i++) T[i].val = T[i].val*iz%M;
            for (int j = 0; j < d; j++){
                T[j].val = 0;
                if (T[j+d].val != 0) T[j+d].val = M-T[j+d].val;
            }
            inplaceDFT(T);
            for (int j = 0; j < 2*d; j++) T[j].val = T[j].val*(ull)G0[j].val%M;
            inplaceIDFT(T);
            for (int i = 0; i < 2*d; i++) T[i].val = T[i].val*iz%M;
            for (int j = 0; j < d; j++) T[j].val = g0[j].val;
            g0 = T;
        }
        f0.resize(n);
        return f0;
    }
    /// @brief log(F) mod x^n を求める F[x^0] == 1 が必要 
    FormalPowerSeries<M> log(const FormalPowerSeries<M>& F, const int n, const mod_table<M>& mtable) const {
        assert(F[0].val == 1);
        auto DF = F;
        for (ull i = 0, limit = DF.sz-1; i < limit; i++){
            DF[i].val = DF[i+1].val*(i+1)%M;
        }
        DF.f.pop_back();
        DF.sz--;
        auto Finv = inv(F, n);
        auto res = convolution(DF, Finv);
        res.resize(n);
        for (int i = n-1; i >= 1; i--){
            res[i].val = res[i-1].val*(ull)mtable.invmodlist[i]%M;
        }
        res[0].val = 0;
        return res;
    }
    /// @brief F^k mod x^n を求める
    FormalPowerSeries<M> pow(const FormalPowerSeries<M>& F, const ull k, const int n, const mod_table<M>& mtable) const {
        if (k == 0){
            FormalPowerSeries<M> ret(n,0);
            ret[0].val = 1;
            return ret;
        }
        uint lowest_deg = 0;
        ull lowest_coef = 0;
        while (lowest_deg < F.sz && F[lowest_deg].val == 0){
            lowest_deg++;
        }
        if (lowest_deg == F.sz) return FormalPowerSeries<M>(n);
        lowest_coef = F[lowest_deg].val;
        ull iz = inverse_mod<ll,ll>(lowest_coef, M);
        FormalPowerSeries<M> G(n);
        for (uint i = lowest_deg, limit = min(F.sz, lowest_deg + n); i < limit; i++){
            G[i-lowest_deg].val = F[i].val*iz%M;
        }
        auto logG = log(G, n, mtable);
        ull kmodm=k%M;
        for (int i = 0; i < n; i++){
            logG[i].val = logG[i].val*kmodm%M;
        }
        auto res = exp(logG, n, mtable);
        FormalPowerSeries<M> ret(n);
        ull t = modpow<M>(lowest_coef, k);
        int offset = lowest_deg == 0 ? 0 : k > (ull)n ? n : min(lowest_deg*k, (ull)n);
        for (int i = 0;; i++){
            if (i+offset < n){
                ret[i+offset].val = res[i].val*t%M;
            }
            else{
                break;
            }
        }
        return ret;
    }
    // f(g(x)) mod x^len(f) を求める
    FormalPowerSeries<M> composition(FormalPowerSeries<M> f, FormalPowerSeries<M> g, const mod_table<M>& mtable) const {
        f.resize(max(f.size(),g.size()));
        int N = f.size();
        if (N == 0) return FormalPowerSeries<M>(0);

        int n = 1;
        while (n < (int)f.size()) n *= 2;
        f.resize(n), g.resize(n);

        FormalPowerSeries<M> W(2*n);
        {
            // bit reverse order
            vector<int> btr(2*n);
            int log = 31-__builtin_clz(2*n);
            for (int i = 0; i < 2*n; i++){btr[i] = (btr[i >> 1] >> 1) + ((i & 1) << (log - 1));}
            int t = log_max_length;
            uint r = last_powroot;
            ull dw = modpow<M>(inverse_mod<ll,ll>(r, M), (1<<t)/(4*n));
            uint w = 1;
            for (auto i: btr) { W[i].val = w, w = w*dw%M; }
        }

        auto rec = [&](auto &rec, int n, int k, FormalPowerSeries<M> &Q) -> FormalPowerSeries<M> {
            if (n == 1) {
                reverse(f.f.begin(), f.f.end());
                FormalPowerSeries<M> p(2*k);
                for (int i = 0; i < k; i++) p[2 * i].val = f[i].val;
                return p;
            }
            Q.resize(4*n*k);
            Q[2*n*k].val = 1;
            inplaceDFT(Q);
            FormalPowerSeries<M> nxt_Q(2*n*k);
            for (int i = 0; i < 2*n*k; i++) nxt_Q[i].val = Q[2*i].val*(ull)Q[2*i+1].val%M;
            
            inplaceIDFT(nxt_Q);
            ull iz = inverse_mod<ll,ll>(nxt_Q.size(), M);
            for (uint i = 0; i < nxt_Q.size(); i++) nxt_Q[i].val = nxt_Q[i].val*iz%M;

            for (int j = 0; j < 2*k; j++) for (int i = n/2; i < n; i++) nxt_Q[n*j+i].val = 0;
            nxt_Q[0].val = 0;
            FormalPowerSeries p = rec(rec, n/2, 2*k, nxt_Q);
            for (int j = 0; j < 2*k; j++) for (int i = n/2; i < n; i++) p[n * j + i].val = 0;
            
            iz = mtable.invmodlist[p.size()];
            for (uint i = 0; i < p.size(); i++) p[i].val = p[i].val*iz%M;
            inplaceIDFT_T(p);
            
            p.resize(4*n*k);
            for (int i = 2*n*k-1; i >= 0; i--){
                p[2*i+1].val = (ull)(mtable.invmodlist[2]-1+M)%M*W[i].val%M*Q[2*i].val%M*p[i].val%M;
                p[2*i].val = (ull)mtable.invmodlist[2]*W[i].val%M*Q[2*i+1].val%M*p[i].val%M;
            }
            
            inplaceDFT_T(p);
            
            p.resize(2*n*k);
            return p;
        };

        FormalPowerSeries<M> Q(2*n);
        for(int i = 0; i < n; i++) Q[i].val = g[i].val == 0 ? 0 : M-g[i].val;
        FormalPowerSeries p = rec(rec, n, 1, Q);
        p.resize(n);
        reverse(p.f.begin(), p.f.end());
        p.resize(N);
        return p;
    }
    // f(x+a)を求める
    FormalPowerSeries<M> polynominal_taylor_shift(const FormalPowerSeries<M>& F, ll a, const mod_table<M>& mtable){
        ull b = M+a%M;
        if (b >= M){b -= M;}
        if (b == 0){return F;}
        int N = F.size();
        FormalPowerSeries<M> G(N), H(N);
        for (int i = 0; i < N; i++){
            G[i].val = F[N-1-i].val*(ull)mtable.factorialmodlist[N-1-i]%M;
            H[i].val = mtable.factorialmodinvlist[i];
        }
        ull t = b;
        for (int i = N-2; i >= 0; i--){
            G[i].val = G[i].val*t%M;
            t = t*b%M;
        }
        G = convolution(G,H);
        t = 1;
        b = inverse_mod<ll,ll>(b,M);
        for (int j = 0; j < N; j++){
            H[j].val = mtable.factorialmodinvlist[j]*t%M*G[N-1-j].val%M;
            t = t*b%M;
        }
        return H;
    }
    // fの微分を求める。
    FormalPowerSeries<M> differential(const FormalPowerSeries<M>& F){
        FormalPowerSeries<M> G = F;
        G.differential();
        return G;
    }
    // fの積分を求める。積分定数は0
    FormalPowerSeries<M> integral(const FormalPowerSeries<M>& F, const mod_table<M>& mtable){
        FormalPowerSeries<M> G = F;
        G.integral(mtable);
        return G;
    }
};

/// @brief `[x^N].val(P(x)/Q(x))` をmod Mで求める
template<ull M>
ll Bostan_Mori(const ll N, FormalPowerSeries<M> P, FormalPowerSeries<M> Q, const fps_operator<M>& op){
    assert(N >= 0);
    if (N == 0){
        return P[0].val*inverse_mod<ll,ll>(Q[0].val, M)%M;
    }
    FormalPowerSeries<M> Q_minus;
    const int maxloop = (N == 1 ? 1 : 65-__builtin_clzll(N-1));
    for (int _i_ = 0; _i_ < maxloop; _i_++){
        Q_minus.resize(Q.size());
        for (size_t i = 0; i < Q_minus.size(); i += 2){
            Q_minus[i].val = Q[i].val;
        }
        for (size_t i = 1; i < Q_minus.size(); i += 2){
            Q_minus[i].val = (M-1)*Q[i].val%M;
        }
        auto A = op.convolution(P,Q_minus);
        auto B = op.convolution(Q,Q_minus);
        Q.resize((B.size()+1)/2);
        for (size_t i = 0; i < B.size(); i += 2){
            Q[i/2].val = B[i].val;
        }

        P.resize((A.size()+!btest_for_fps(N,_i_))/2);
        for (size_t i = btest_for_fps(N,_i_); i < A.size(); i += 2){
            P[i/2].val = A[i].val;
        }
    }
    return P[0].val*inverse_mod<ll,ll>(Q[0].val, M)%M;
}


#endif /* FPS_OPERATION_HPP_ */


constexpr fps_operator<998244353> op;
mod_table<998244353> mt(1048576);
int main(){
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N;
    cin >> N;

    FormalPowerSeries<998244353> F(N),G(N);
    vin(F);
    vin(G);

    auto ans = op.composition(F,G,mt);
    vout(ans);
}