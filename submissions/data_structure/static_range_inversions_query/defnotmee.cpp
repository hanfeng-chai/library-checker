#include<bits/stdc++.h>
#define all(x) x.begin(), x.end()
#define ff first
#define ss second
#define O_O
using namespace std;
template <typename T>
using bstring = basic_string<T>;
template <typename T>
using matrix = vector<vector<T>>;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef long long ll;
typedef pair<int,int> pii;
typedef pair<ll,ll> pll;
const ll INFL = 4e18+25;
const int INF = 1e9+42;
const double EPS = 1e-7;
const int MOD = 998244353;
const int RANDOM = chrono::high_resolution_clock::now().time_since_epoch().count();
const int MAXN = 1e5+65;

const int LOGLEN = 8;
const int LEN = 1<<LOGLEN;



struct decomp{
    int v[MAXN] = {};
    int block[MAXN] = {};

    inline int query(int id){
        return v[id] + block[id>>LOGLEN];
    }

    inline int update(int r){
        int br = r>>LOGLEN;
        int ret = 0;

        for(int i = 0; i < br; i++)
            block[i]++;

        for(int i = br*LEN; i <= r; i++)
            v[i]++;
        
        return ret;
    }
};


struct query{
    int lp, rp, sign, addrange, qid;
};

struct mo{
    int ql, qr, qid;

    bool operator<(const mo& b) const {
        if((ql>>LOGLEN+1) == (b.ql>>LOGLEN+1))
            return ((ql>>LOGLEN+1)&1) ? qr > b.qr : qr < b.qr;
        return ql < b.ql;
    }
};



int main(){
    
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, q;
    cin >> n >> q;

    decomp dc;

    vector<int> v(n);
    vector<array<int,2>> aux(n);
    for(int i = 0; i < n; i++){
        cin >> v[i];
        aux[i] = {v[i],i};
    }

    sort(all(aux));
    for(int i = 0; i < n; i++)
        v[aux[i][1]] = i;


    // Inversions on [l,r] = inversions on [0,r] (pinv[r]) - inversions between [0,l-1] and [l,r]
    vector<ll> pinv(n);

    vector<mo> o(q);

    for(int i = 0; i < q; i++)
        cin >> o[i].ql >> o[i].qr, o[i].qr--, o[i].qid = i;

    sort(all(o));

    // Let g(i, x) := number of j in [0,i] such that v[j] >= x
    // sweep[i][j] = {lx, rx, sign, addrange, qid}
    // -> for all lp <= p <= rp, qans[qid] += sign * ((i-p+1)*addrange - g(i,v[p]))
    matrix<query> sweep(n);

    int l = 0, r = 0;
    
    auto addg =[&](int r, int lp, int rp, int sign, int addrange, int qid){
        if(r != -1)
            sweep[r].emplace_back(lp,rp,sign,addrange,qid);
    };

    for(auto&& [ql,qr,qid] : o){

        if(r < qr){
            // qans[i] += -g(l-1,x)
            addg(l-1, r+1, qr, 1, 0, qid);
            r = qr;
        }

        if(l > ql){
            // qans[i] += r-l+1-g(r,x)
            addg(r, ql, l-1, 1, 1, qid);

            l = ql;
        }

        if(r > qr){
            // qans[i] -= g(l-1,x)
            addg(l-1, qr+1, r, -1, 0, qid);            
            r = qr;
        }

        if(l < ql){
            // qans[i] += -(r-l+1-g(r,x))
            addg(r, l, ql-1, -1, 1, qid);
            l = ql;
        }
    }
    vector<ll> qans(q);
    vector<ll> delta(q);

    for(int i = 0; i < n; i++){
        pinv[i] = dc.query(v[i]);
        dc.update(v[i]);
        for(auto& [lp, rp, sign, addrange, qid] : sweep[i]){
            // cerr << lp << ' ' << rp << ' ' << sign << ' ' << addrange << ' ' << qid << ":\n";
            for(int p = lp; p <= rp; p++){
                // cerr << "delta[" << qid << "] += " << sign * ((i-p+1)*addrange) << " + " << sign*(- dc.query(v[p])) << '\n';
                delta[qid] += sign * ((i-p+1)*addrange - dc.query(v[p]));
            }
        }
    }

    for(int i = 1; i < n; i++)
        pinv[i]+=pinv[i-1];

    ll prev = 0;
    for(auto&& [ql, qr, qid] : o){
        prev+=delta[qid];
        qans[qid] = prev+pinv[qr]-pinv[max(0,ql-1)];
    }

    for(int i = 0; i < q; i++){
        cout << qans[i] << '\n';
    }
    
    return 0;

}