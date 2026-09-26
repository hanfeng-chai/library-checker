#include <bits/stdc++.h>
#ifdef LOCAL
    #include "../debug.hpp"
#else 
    #define dbg(...)
#endif // LOCAL

using namespace std;

#define all(v) begin(v), end(v)
#define rall(v) rbegin(v), rend(v)
#define compact(v) v.erase(unique(all(v)), end(v))
#define sz(v) (v).size()

using ll = long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using tpl = tuple<int,int,int>;

template<class T> bool minimize (T& a, const T& b) { return (a > b ? a = b, 1 : 0); }
template<class T> bool maximize (T& a, const T& b) { return (a < b ? a = b, 1 : 0); }

template<int MOD>
struct ModInt {
    static const int mod = MOD;
    int v;
    ModInt (int a = 0) : v(a) {refine();}

    void refine() {
        if (abs(v) >= mod) v %= mod;
        if (v < 0) v += mod;
    }

    const ModInt& operator+= (const ModInt &o) {
        if ((v += o.v) >= mod) v -= mod;
        return *this;
    }

    const ModInt& operator-= (const ModInt &o) {
        if ((v -= o.v) < 0) v += mod;
        return *this;
    }

    const ModInt& operator*= (const ModInt &o) {
        v = 1LL * v * o.v % MOD;
        return *this;
    }

    const ModInt& operator/= (const ModInt &o) {
        return operator*=(o.inv());
    }

    ModInt pwr (int k) const {
        ModInt ans(1), a = *this;
        for (; k; k >>= 1, a *= a)
            if (k & 1) ans *= a;
        return ans;
    }

    ModInt inv() const { return pwr(mod - 2); }

    friend ModInt operator+ (ModInt a, const ModInt &b) { return a += b; }
    friend ModInt operator- (ModInt a, const ModInt &b) { return a -= b; }
    friend ModInt operator* (ModInt a, const ModInt &b) { return a *= b; }
    friend ModInt operator/ (ModInt a, const ModInt &b) { return a /= b; }

    friend istream& operator>> (istream &i, ModInt &m) { return i >> m.v, m.refine(), i; }
    friend ostream& operator<< (ostream &o, const ModInt &m) { return o << m.v; }
};
using mint = ModInt<998'244'353>;

template<class T>
struct Matrix : vector<T> {
    int n, m;
    Matrix (int n, int m) : vector<T>(n * m), n(n), m(m) {}
    Matrix (initializer_list<T> init, int row) : vector<T>(all(init)), n(row), m(init.size() / m) {}

    T* operator[] (int i) { return this->data() + i * m; }
    const T* operator[] (int i) const { return this->data() + i * m; }

    void swapRow (int i, int j) {
        swap_ranges(this->begin() + i * m, this->begin() + (i + 1) * m, this->begin() + j * m);
    }

    friend ostream& operator<< (ostream &o, const Matrix &m) {
        o << "{";
        for (int i = 0; i < m.n; i++) {
            o << "{";
            for (int j = 0; j < m.m; j++) o << m[i][j] << ", ";
            o << "}, ";
        }
        return o << "}";
    }
};

void gaussEliminate (Matrix<mint> &v, int n, int m, vector<int> &pc) {
    // transfer data
    Matrix<unsigned long long> mtr(n, m);
    vector<int> hold(n), pivCol(n, -1);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) mtr[i][j] = v[i][j].v;

    auto normalize = [&] (unsigned long long &x) {
        if (x >= mint::mod) x %= mint::mod;
    };
    auto normalizeRow = [&] (int i) {
        for (int j = 0; j < m; j++) normalize(mtr[i][j]);
        hold[i] = 0;
    };
    
    // forward + backward elimination combined
    for (int i = 0, pCol = 0; i < n && pCol < m; pCol++) {
        // bring up pivot
        for (int j = i; j < n; j++) {
            normalize(mtr[j][pCol]);
            if (mtr[j][pCol]) {
                mtr.swapRow(i, j), swap(hold[i], hold[j]);
                break;
            }
        }
        if (!mtr[i][pCol]) continue; // skip to next column of the same row
        
        // normalize this row
        normalizeRow(i);
        if (mtr[i][pCol] != 1) {
            mint factor = mint(1) / mint(mtr[i][pCol]);
            for (int j = pCol; j < m; j++) mtr[i][j] *= factor.v;
            normalizeRow(i);
        }
        
        // eliminate the pCol-th entry for every other row
        for (int k = 0; k < n; k++) {
            normalize(mtr[k][pCol]);
            if (k == i || !mtr[k][pCol]) continue;
            int scalar = mint::mod - mtr[k][pCol];
            for (int j = pCol; j < m; j++) mtr[k][j] += mtr[i][j] * scalar;
            if (++hold[k] == 16) normalizeRow(k);
        }
        pivCol[i++] = pCol;
    }

    // transfer data back
    for (int i = 0; i < n; i++) {
        normalizeRow(i);
        for (int j = 0; j < m; j++) v[i][j] = mtr[i][j];
    }
    swap(pivCol, pc);
}

bool solveSOE (Matrix<mint> &aug, int n, int m, Matrix<mint> &oneSol, Matrix<mint> &nullspace) {
    vector<int> pivotCol(n);
    vector<bool> pivot(m - 1);
    gaussEliminate(aug, n, m, pivotCol);

    int rank = 0;
    for (int i = 0; i < n; i++) {
        if (pivotCol[i] != -1 && pivotCol[i] + 1 < m)
            pivot[pivotCol[i]] = true, rank++;
        else if (pivotCol[i] + 1 == m) {
            pivotCol[i] = -1;
            if (aug[i][m - 1].v) return false;
        }
    }
    
    oneSol = Matrix<mint>(m - 1, 1);
    nullspace = Matrix<mint>(m - 1 - rank, m - 1);

    for (int i = 0; i < n; i++) {
        if (pivotCol[i] == -1) continue;
        oneSol[pivotCol[i]][0] = aug[i][m - 1];
        for (int j = 0, counter = 0; j < m - 1; j++) {
            if (!pivot[j]) nullspace[counter++][pivotCol[i]] = mint(0) - aug[i][j];
        }
    }
    for (int j = 0, counter = 0; j < m - 1; j++)
        if (!pivot[j]) nullspace[counter++][j] = 1;
    return true;
}

void testcase() {
    int N, M; cin >> N >> M;
    Matrix<mint> vec(N, M + 1), nullspace(0, 0);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++) cin >> vec[i][j];
    for (int i = 0; i < N; i++) cin >> vec[i][M];

    Matrix<mint> oneSol(0, 0);
    if (!solveSOE(vec, N, M + 1, oneSol, nullspace)) return cout << -1 << "\n", void();

    cout << nullspace.n << "\n";
    for (int i = 0; i < oneSol.size(); i++) cout << oneSol[i][0] << " ";
    cout << "\n";
    for (int i = 0; i < nullspace.n; i++) {
        for (int j = 0; j < nullspace.m; j++) cout << nullspace[i][j] << " ";
        cout << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
  
    int TC = 1;
    while (TC--) {
        testcase();
    }

    return 0;
}

/*
2 3
1 2 3
4 5 6
50 122

2 2
1 1
1 1
1 2

3 3
1 0 2
0 1 1
1 1 3
3 1 4

6 6
499122177 332748118 748683265 598946612 166374059 713031681
332748118 748683265 598946612 166374059 713031681 873463809
748683265 598946612 166374059 713031681 873463809 776412275
598946612 166374059 713031681 873463809 776412275 299473306
166374059 713031681 873463809 776412275 299473306 635246407
713031681 873463809 776412275 299473306 635246407 582309206
908402107 282743739 726407896 277197937 313497732 729432879

3 2
1 0
0 1
0 0
1 2 0
*/