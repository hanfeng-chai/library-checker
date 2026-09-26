#pragma GCC optimize("Ofast")
#pragma GCC target("avx2")
// #pragma GCC target("avx512f,avx512bw,avx512vbmi2,popcnt")

// #include <immintrin.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<unistd.h>
#include<bits/stdc++.h>
using namespace std;

#define ll long long
#define ull unsigned long long
#define uint8_t unsigned char
#define uint64_t unsigned long long
typedef unsigned long uintptr_t;
#define rep1(a) for (int _ = 0; _ < (a); ++_)
#define rep2(i, a) for (int i = 0; i < (a); ++i)
#define rep3(i, a, b) for (int i = a; i < (b); ++i)
#define rrep1(a) for (int i = (a)-1; i >= (0); --i)
#define rrep2(i, a) for (int i = (a)-1; i >= (0); --i)
#define rrep3(i, a, b) for (int i = (b)-1; i >= (a); --i)
#define overload3(a, b, c, d, ...) d
#define rep(...) overload3(__VA_ARGS__, rep3, rep2, rep1)(__VA_ARGS__)
#define rrep(...) overload3(__VA_ARGS__, rrep3, rrep2, rrep1)(__VA_ARGS__)
struct stat st;
static char* rp;
static char W[40];
static char* wp = W+40;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#define io() {fstat(0,&st);rp=(char*)mmap(0,st.st_size,1,1,0,0);}
#define rd(x) {(x)=*rp++&15;while(*rp>47)(x)=(x)*10+(*rp++&15);++rp;}
#define wt(x) {*--wp=(x%10)+48;x/=10;while(x){*--wp=(x%10)+48;x/=10;}}

template<typename T>
struct RadixHeap{
    vector<T> v0;
    vector<pair<ull,T>> v[64];
    ull last, size;
    uint64_t mask;
    RadixHeap():last(0),size(0),mask(0){};

    int bsr(ull x){
        return 63 - __builtin_clzll(x);
    }
    void push(ull x, const T& y){
        ++size;
        if(x==last){
            v0.push_back(y);
        }
        else{
            int idx = bsr(x^last);
            v[idx].emplace_back(x,y);
            mask |= 1ULL << idx;
        }
    }
    pair<ull,T> pop(){
        if(v0.empty()){
            int h = __builtin_ctzll(mask);
            mask &= mask-1;
            if(v[h].size()==1){
                --size;
                auto top = v[h][0];
                last = top.first;
                v[h].pop_back();
                return top;
            }
            last = UINT64_MAX;
            for(const auto&[x,y]:v[h]) last=min(last,x);
            for(const auto&[x,y]:v[h]){
                if(x==last){
                    v0.push_back(y);
                } else{
                    int idx = bsr(x^last);
                    v[idx].emplace_back(x,y);
                    mask |= 1ULL<<idx;
                }
            }
            v[h].clear();
        }
        -- size;
        auto top = v0.back();
        v0.pop_back();
        return {last,top};
    }
};

int cnt[500002];
int uvw[500000][3];
int ed[500000][2];

ull dist[500001];
int prv[500000];

bool initialized = false;
alignas(64) u8 str_beg[500000];
alignas(64) char str[500000][8];

void str_init(){
    initialized = true;
    for(int i=0;i<500000;i+=10)for(int j=0;j<10;++j)str[i+j][7]=j+'0';
    for(int i=0;i<500000;i+=100)for(int j=0;j<10;++j)for(int k=0;k<10;++k)str[i+10*j+k][6]=j+'0';
    for(int i=0;i<500000;i+=1000)for(int j=0;j<10;++j)for(int k=0;k<100;++k)str[i+100*j+k][5]=j+'0';
    for(int i=0;i<500000;i+=10000)for(int j=0;j<10;++j)for(int k=0;k<1000;++k)str[i+1000*j+k][4]=j+'0';
    for(int i=0;i<500000;i+=100000)for(int j=0;j<10;++j)for(int k=0;k<10000;++k)str[i+10000*j+k][3]=j+'0';
    for(int j=0;j<5;++j)for(int k=0;k<100000;++k)str[j*100000+k][2]=j+'0';
}

void str_append(int x, string& out){
    if(!initialized) out += to_string(x);
    else out.append(str[x]+str_beg[x], str[x]+8);
}

int main(){
    io();

    str_beg[0]=7;
    for(int beg=7,mn=1;beg>=2;--beg,mn*=10){
        int imax = min(mn*10,500000);
        for(int i=mn;i<imax;++i)str_beg[i]=beg;
    }

    for(int i=0;i<500000;++i)dist[i]=0xFFFFFFFFFFFFFFFF;
    
    int N,M,s,t; rd(N);rd(M);rd(s);rd(t);
    rep(i,M){
        int a,b,c; rd(a);rd(b);rd(c);
        ++cnt[a];
        uvw[i][0]=a; uvw[i][1]=b; uvw[i][2]=c;
    }
    int cnt_or = cnt[0];
    for(int i=1;i<=N;++i){
        if(cnt[i]==0) dist[i]=0; // star対策
        cnt_or |= cnt[i];
        cnt[i]+=cnt[i-1];
    }

    if(cnt_or==1){
        // line対策
        str_init();

        for(int i=0;i<N;++i)ed[i][0]=-1;
        rep(i,M){
            const auto& [a,b,c]=uvw[i];
            ed[a][0]=b; ed[a][1]=c;
        }

        ull d = 0;
        string out; out.reserve(20*N);

        int cur = s;
        int m = 0;

        while(cur!=t){
            const auto& [v,w]=ed[cur];
            if(v==-1 || m > N){
                cout << "-1\n";
                return 0;
            }
            d += w;
            str_append(cur, out);
            out += ' ';
            str_append(v, out);
            out += '\n';
            ++m;
            cur = v;
        }
        cout << d << ' ' << m << '\n';
        cout << out;
        return 0;
    } else{
        dist[t]=0xFFFFFFFFFFFFFFFF;
        rep(i,M){
            const auto& [a,b,c]=uvw[i];
            ed[--cnt[a]][0]=b; ed[cnt[a]][1]=c;
        }

        RadixHeap<int> pq;
        pq.push(0,s);
        dist[s]=0;
        while(pq.size){
            auto [d,u]=pq.pop();
            if(d>dist[u])continue;
            if(u==t) break;

            for(int k = cnt[u]; k < cnt[u+1]; ++k){
                const auto& [v,w] = ed[k];
                ull nd = d + w;
                if(dist[v]>nd){
                    dist[v]=nd;
                    prv[v]=u;
                    pq.push(nd,v);
                }
            }
        }
        if(dist[t]==0xFFFFFFFFFFFFFFFF){
            cout << "-1\n";
            return 0;
        }

        int m = 0;
        int cur = t;
        while(cur!=s){
            cnt[m++]=cur;
            cur=prv[cur];
        }
        cnt[m]=s;
        string out; out.reserve(20*m + 30);
        string a, b;
        out += to_string(dist[t]) + ' ' + to_string(m) + '\n';
        if(m > 100000) str_init();
        for(int i=m-1;i>=0;--i){
            str_append(cnt[i+1], out);
            out += ' ';
            str_append(cnt[i], out);
            out += '\n';
            a.swap(b);
        }
        cout << out;
        return 0;   
    }
}