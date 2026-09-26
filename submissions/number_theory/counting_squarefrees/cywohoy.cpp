#pragma clang optimize on
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize ("Ofast")

#include <bits/stdc++.h>
#define i32 int
#define i64 long long
#define i128 __int128_t
const i32 PRE = 12000000;

short mu[PRE+30];
std::bitset<PRE+30> sieve;

std::unordered_map<i32,short> mp;

i32 primes[PRE];

i32 S(i64 x) {
    if (x < PRE) return mu[x];
    if (mp.find(x) != mp.end()) return mp[x];
    i64 res = 1; 
    i64 m = sqrt(x);
    res += m * S(m) - x;
    for (i64 a = 2; a <= m; a++) {
        i64 t = x / a;
        res -= (mu[a] - mu[a-1]) * t;
        res -= S(t);
    }
    return mp[x] = res;
}

i64 Get(i64 n) {
    i64 c = 0;

    for (i64 i = 1; i <= n;) {
        i64 s = (i64)sqrt(n / i);
        while (s*s*i > n) s--;
        i64 j = n / (s * s);
        c += S(s) * (j - i + 1);

        i = j + 1;
    }

    return c;
}

i32 pc, i1;
i64 target, mid, val, diff;

i32 main(void) {
    mu[0] = 0;
    mu[1] = 1;
    for (i64 i = 2; i < PRE; i++) {
        if (!sieve[i]) {
            primes[pc++] = i;
            mu[i] = -1;
        }
        
        for (i64 a = 0; a < pc; a++) {
            i32 p = primes[a];
            if (i*p >= PRE) break;
            sieve[i*p] = 1;
            if (i%p == 0) {
                mu[i*p] = 0;
                break;
            }
            
            mu[i*p] = -mu[i];
        }
        
        mu[i] += mu[i-1];
    }


    scanf("%lld", &target);
    printf("%lld", Get(target));
}