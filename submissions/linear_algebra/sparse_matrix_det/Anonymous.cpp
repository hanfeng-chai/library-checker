#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,popcnt,lzcnt,tune=native")
#include <bits/stdc++.h>
using namespace std;
#ifndef yoshi_likes_e4
#define endl '\n'
#endif
#define problem ""
#define multitest 0
#define debug(x) cerr << #x << " = " << x << endl;
constexpr int MOD = 998244353;
int64_t inv_(int64_t x)
{
    int64_t v = MOD;
    int64_t x1 = 1, x2 = 0;
    while (x != 1)
    {
        int64_t q = v / x;
        int64_t r = v - q * x, X = x2 - q * x1;
        v = x;
        x = r;
        x2 = x1;
        x1 = X;
    }
    return (x1 % MOD) + (x1 < 0) * MOD;
}
struct Modint
{
    uint32_t R;
    Modint() {}
    Modint(int c) { R = c; }
    Modint add(Modint c) { return min(R + c.R, R + c.R - MOD); }
    Modint sub(Modint c) { return min(R - c.R, R - c.R + MOD); }
    Modint mul(Modint c) { return (size_t)R * c.R % MOD; }
    Modint inv() { return inv_(R); }
};
typedef array<unsigned long long, 47> AsArray;
const int B = 64;
bool getbit(AsArray &x, int b) { return (x[b / B] >> (b % B)) & 1; }
int stor[9000000];
int cnt[3000];
Modint *RAW[3000];
bitset<3000> Nz[3000];
int64_t __ref()
{
    int h = 0, k = 0;
    int64_t sign = 1;
    int i_stack[3000];
    while (h < 3000 && k < 3000)
    {
        int cur_last = 1e9, idx = 3000;
        for (int i = h; i < 3000; i++)
        {
            AsArray &ar2 = *reinterpret_cast<AsArray *>(&Nz[i]);
            if (getbit(ar2, k))
                if (cnt[i] < cur_last)
                {
                    cur_last = cnt[i];
                    idx = i;
                }
        }
        if (idx != 3000)
        {
            swap(RAW[h], RAW[idx]);
            swap(Nz[h], Nz[idx]);
            swap(cnt[h], cnt[idx]);
            const Modint ixhk = RAW[h][k].inv();
            if (idx != h)
            {
                sign *= (MOD - 1);
                sign %= MOD;
            }
            int i_ptr = 0;
            AsArray &ar2 = *reinterpret_cast<AsArray *>(&Nz[h]);
            int idx = 0;
            for (auto x : ar2)
            {
                while (x)
                {
                    int idx2 = __builtin_ctzll(x) + idx * 64;
                    if (idx2 < 3000)
                        i_stack[i_ptr++] = idx2;
                    x -= (x & -x);
                }
                idx++;
            }
            for (int i = h + 1; i < 3000; i++)
            {
                if (RAW[i][k].R)
                {
                    AsArray &ar = *reinterpret_cast<AsArray *>(&Nz[i]);
                    Modint coeff = RAW[i][k].mul(ixhk);
                    for (int k = 0; k < i_ptr; k++)
                    {
                        auto j = i_stack[k];
                        cnt[i] -= getbit(ar, j);
                        RAW[i][j] = RAW[i][j].sub(coeff.mul(RAW[h][j]));
                        if (RAW[i][j].R) [[likely]]
                            Nz[i].set(j, 1), cnt[i]++;
                        else
                            Nz[i].set(j, 0);
                    }
                }
            }
            if(h<10)
            cerr << RAW[h][h].R << endl;
            h++;
        }
        k++;
    }
    return sign;
}
void init() {}
void Yoshi()
{
    int N, K;
    cin >> N >> K;
    for (int i = 0; i < 3000; i++)
        RAW[i] = (Modint *)(stor + 3000 * i);
    while (K--)
    {
        int a, b, c;
        cin >> a >> b >> c;
        RAW[a][b] = c;
        Nz[a].set(b, 1);
        cnt[a]++;
    }
    vector<pair<int, int>> st;
    for (int i = 0; i < N; i++)
        st.push_back({cnt[i], i});
    int64_t sgn = 1;
    for (int i = 0; i < N; i++)
    {
        int ptr = min_element(st.begin() + i, st.end()) - st.begin();
        if (ptr != i)
        {
            sgn *= MOD - 1, sgn %= MOD;
            swap(cnt[st[i].second], cnt[st[ptr].second]);
            swap(Nz[st[i].second], Nz[st[ptr].second]);
            swap(RAW[st[i].second], RAW[st[ptr].second]);
            swap(st[i], st[ptr]);
        }
    }
    int64_t sign = __ref();
    int64_t determinant = (sign * sgn) % MOD;
    for (int i = 0; i < N; i++)
    {
        if(i<10)
        cerr<<RAW[i][i].R<<endl;
        determinant *= RAW[i][i].R;
        determinant %= MOD;
    }
    cout << determinant << endl;
}
signed main()
{
#ifndef yoshi_likes_e4
    ios::sync_with_stdio(0);
    cin.tie(0);
    if (fopen(problem ".inp", "r"))
    {
        freopen(problem ".inp", "r", stdin);
        freopen(problem ".out", "w", stdout);
    }
#endif
    init();
    int t = 1;
#if multitest
    cin >> t;
#endif
    while (t--)
        Yoshi();
}
