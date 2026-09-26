#define PROBLEM "https://judge.yosupo.jp/problem/chromatic_polynomial"
#include <vector>
#include <algorithm>

namespace nachia{

template<class E>
std::vector<E> SpsPowerProjection(int n, const std::vector<E>& A, const std::vector<E>& W, int m, bool exponential = false){
    const E a0 = A[0];

    using iter = typename std::vector<E>::iterator;
    struct {
        std::vector<int> Chi;
        void init(int n){
            Chi.resize(1<<n);
            for(int i=1; i<(1<<n); i++) Chi[i] = Chi[i-(i&-i)] + 1;
        }
        void zeta(int n, typename std::vector<E>::const_iterator a, iter r){
            int N = n + 1;
            auto z = E(0);
            for(int i=0; i<N<<n; i++) r[i] = z;
            for(int i=0; i<1<<n; i++) r[Chi[i]+i*N] = a[i];
            for(int w=1; w<=1<<n; w++) for(int d=0; !(w&(1<<d)); d++){
                int W = N * (w-(1<<d)), dd = N<<d;
                for(int i=N * (w-(2<<d)); i<W; i++) r[i+dd] += r[i];
            }
        }
        void comp(int n, iter a, iter r){
            for(int i=0; i<(1<<n); i++){
                std::copy(a, a + n, r);
                a += (n + 1);
                r += n;
            }
        }
        void conv(int n, iter a, iter b){
            int N = n + 1;
            for(int i=0; i<(1<<n); i++){
                int I=i*N;
                std::vector<E> Q(N);
                for(int ja=0; ja<=Chi[i]; ja++) for(int jb=Chi[i]-ja, x=std::min(n-ja, Chi[i]); jb<=x; jb++){
                    Q[ja+jb] += a[ja+I] * b[jb+I];
                }
                std::copy(Q.begin(), Q.end(), a + I);
            }
        }
        void mobius(int n, iter a, iter r){
            int N = n + 1;
            for(int w=1; w<=(1<<n); w++) for(int d=0; !(w&(1<<d)); d++){
                int W = N * (w-(1<<d)), dd = N<<d;
                for(int i=N * (w-(2<<d)); i<W; i++) a[i+dd] -= a[i];
            }
            for(int i=0; i<(1<<n); i++) r[i] = a[Chi[i]+i*N];
        }
    } rz; rz.init(n);

    std::vector<std::vector<E>> zet(n);
    for(int d=0; d<n; d++) zet[d].resize((d+1)<<d);
    for(int i=0; i<n; i++) rz.zeta(i, A.begin() + (1<<i), zet[i].begin());
    
    std::vector<E> res(1<<n);
    std::vector<E> zet2(n << (n-1));
    std::vector<E> p(n+1);
    for(int i=0; i<(1<<n); i++) res[i] = W[i];

    for(int d=n-1; d>=0; d--){
        p[n-1-d] = res[0];
        std::vector<E> buf(1 << d);
        std::vector<E> buf2(1 << d);
        for(int e=d; e>=0; e--){
            std::reverse(res.begin() + (1 << e), res.begin() + (2 << e));
            rz.zeta(e, res.begin() + (1 << e), zet2.begin());
            rz.conv(e, zet2.begin(), zet[e].begin());
            rz.mobius(e, zet2.begin(), buf2.begin());
            std::reverse(buf2.begin(), buf2.begin() + (1 << e));
            for(int i=0; i<(1<<e); i++) buf[i] += buf2[i];
        }
        std::swap(res, buf);
    }
    p[n] = res[0];
    if(!exponential){
        E f = E(1);
        for(int i=1; i<=n; i++) p[i] *= (f *= E(i));
    }
    
    if(a0.val() != 0){
        std::vector<E> comb(n+1, E(1));
        std::vector<E> ans(m);
        E c = 1;
        for(int d=0; d<m; d++){
            for(int i=0; i<=n && i+d<m; i++) ans[i+d] += comb[i] * c * p[i];
            for(int i=0; i<n; i++) comb[i+1] += comb[i];
            c *= a0;
        }
        return ans;
    }
    p.resize(m);
    return p;
}

} // namespace nachia
#include <utility>
#include <cassert>

namespace nachia{

template<class Modint>
std::vector<Modint> SetCoverPolynomial(int n, const std::vector<Modint>& table){
    assert(int(table.size()) >= (1 << n));
    int nn = 1 << (n-1);
    auto table2 = std::vector<Modint>(table.begin(), table.begin() + nn);
    table2[0] -= Modint(1);
    std::vector<Modint> weight(nn, 0);
    for(int i=0; i<nn; i++) weight[nn-1-i] = table[nn+i];
    auto powerproj = SpsPowerProjection(n-1, table2, weight, n, true);
    powerproj.insert(powerproj.begin(), Modint(0));
    std::vector<Modint> prod(n+2);
    std::vector<Modint> res(n+1);
    prod[0] = 1;
    for(int i=0; i<=n; i++){
        Modint I = Modint(i);
        for(int j=0; j<=i; j++){ res[j] += prod[j] * powerproj[i]; }
        for(int j=i; j>=0; j--){ prod[j+1] += prod[j]; prod[j] *= -I; }
    }
    return res;
}

template<class Modint>
std::vector<Modint> ChromaticPolynomial(std::vector<std::vector<int>> adjacency_matrix){
    int n = adjacency_matrix.size();
    int nn = 1 << n;
    std::vector<Modint> en(nn,1);
    for(int u=0; u<n; u++) for(int v=0; v<n; v++){
        if(adjacency_matrix[u][v]) en[(1<<u)|(1<<v)] = 0;
    }
    for(int d=0; d<n; d++) for(int i=0; i<nn; i++) if(i&(1<<d)) en[i] *= en[i-(1<<d)];
    return SetCoverPolynomial(n, std::move(en));
}

} // namespace nachia

namespace nachia{

// ax + by = gcd(a,b)
// return ( x, - )
std::pair<long long, long long> ExtGcd(long long a, long long b){
    long long x = 1, y = 0;
    while(b){
        long long u = a / b;
        std::swap(a-=b*u, b);
        std::swap(x-=y*u, y);
    }
    return std::make_pair(x, a);
}

} // namespace nachia

namespace nachia{

template<unsigned int MOD>
struct StaticModint{
private:
    using u64 = unsigned long long;
    unsigned int x;
public:

    using my_type = StaticModint;
    template< class Elem >
    static Elem safe_mod(Elem x){
        if(x < 0){
            if(0 <= x+MOD) return x + MOD;
            return MOD - ((-(x+MOD)-1) % MOD + 1);
        }
        return x % MOD;
    }

    StaticModint() : x(0){}
    StaticModint(const my_type& a) : x(a.x){}
    StaticModint& operator=(const my_type&) = default;
    template< class Elem >
    StaticModint(Elem v) : x(safe_mod(v)){}
    unsigned int operator*() const noexcept { return x; }
    my_type& operator+=(const my_type& r) noexcept { auto t = x + r.x; if(t >= MOD) t -= MOD; x = t; return *this; }
    my_type operator+(const my_type& r) const noexcept { my_type res = *this; return res += r; }
    my_type& operator-=(const my_type& r) noexcept { auto t = x + MOD - r.x; if(t >= MOD) t -= MOD; x = t; return *this; }
    my_type operator-(const my_type& r) const noexcept { my_type res = *this; return res -= r; }
    my_type operator-() const noexcept { my_type res = *this; res.x = ((res.x == 0) ? 0 : (MOD - res.x)); return res; }
    my_type& operator*=(const my_type& r)noexcept { x = (u64)x * r.x % MOD; return *this; }
    my_type operator*(const my_type& r) const noexcept { my_type res = *this; return res *= r; }
    my_type pow(unsigned long long i) const noexcept {
        my_type a = *this, res = 1;
        while(i){ if(i & 1){ res *= a; } a *= a; i >>= 1; }
        return res;
    }
    my_type inv() const { return my_type(ExtGcd(x, MOD).first); }
    unsigned int val() const noexcept { return x; }
    static constexpr unsigned int mod() { return MOD; }
    static my_type raw(unsigned int val) noexcept { auto res = my_type(); res.x = val; return res; }
    my_type& operator/=(const my_type& r){ return operator*=(r.inv()); }
    my_type operator/(const my_type& r) const { return operator*(r.inv()); }
};

} // namespace nachia
#include <cstdio>

int main(){
    using Modint = nachia::StaticModint<998244353>;
    int N, M; scanf("%d%d", &N, &M);
    std::vector<std::vector<int>> adj(N, std::vector<int>(N));
    for(int i=0; i<M; i++){
        int u,v; scanf("%d%d", &u, &v);
        adj[u][v] = 1;
    }
    auto ans = nachia::ChromaticPolynomial<Modint>(adj);
    for(int i=0; i<=N; i++){
        if(i) printf(" ");
        printf("%u", ans[i].val());
    } printf("\n");
    return 0;
}
