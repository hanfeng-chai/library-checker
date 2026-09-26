#include <bits/stdc++.h>
using namespace std;

// 高速入力
namespace fio {
const int BUF = 1 << 16;
char ibuf[BUF];
int ip = 0, il = 0;
inline int gc() {
    if (ip == il) { il = (int)fread(ibuf, 1, BUF, stdin); ip = 0; if (il == 0) return -1; }
    return ibuf[ip++];
}
inline int readInt() {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    bool neg = false;
    if (c == '-') { neg = true; c = gc(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    return neg ? -x : x;
}
} // namespace fio

// 区間 LIS クエリ（Tiskin の seaweed + ウェーブレット行列による dominance カウント）
//   seaweed_doubling : seaweed 置換を分割統治（doubling マージ = steady-ant 相当）で O(N log N) 構築
//   Wavelet.count_lt : 区間 [l,r) 内で値 < x の個数（2 次元 dominance カウント）
//   query(l,r)       : (l,r) の LIS = count_lt(0, r, l+1) - l
//   ※ B=17 は値域 < 2^17（N <= 1e5）を前提
template <class T, class Compare = less<T>>
struct StaticRangeLIS {
   private:
    static constexpr int B = 17;
    struct Wavelet {
        struct BitVector {
            using u64 = unsigned long long;
            vector<u64> b;
            vector<int> s;
            void build(int n) { b.assign((n >> 6) + 2, 0); s.assign((n >> 6) + 2, 0); }
            void set(int i) { b[i >> 6] |= 1ULL << (i & 63); }
            void init() {
                for (int i = 0; i + 1 < (int)b.size(); i++)
                    s[i + 1] = s[i] + __builtin_popcountll(b[i]);
            }
            int rank1(int i) { return s[i >> 6] + __builtin_popcountll(b[i >> 6] & ~(~0ULL << (i & 63))); }
            int rank0(int i) { return i - rank1(i); }
        };
        int n;
        int z[B];
        BitVector data[B];
        Wavelet(vector<int> v) : n((int)v.size()) {
            vector<int> vl, vr;
            vl.reserve(n);
            vr.reserve(n);
            for (int k = B - 1; k >= 0; k--) {
                data[k].build(n);
                vl.clear();
                vr.clear();
                for (int i = 0; i < n; i++) {
                    if ((v[i] >> k) & 1) { data[k].set(i); vr.push_back(v[i]); }
                    else vl.push_back(v[i]);
                }
                data[k].init();
                z[k] = (int)vl.size();
                v.swap(vl);
                v.insert(v.end(), vr.begin(), vr.end());
            }
        }
        // [l, r) 内で値 < x の個数
        int count_lt(int l, int r, int x) {
            int res = 0;
            for (int k = B - 1; k >= 0; k--) {
                int l0 = data[k].rank0(l), r0 = data[k].rank0(r);
                if ((x >> k) & 1) { res += r0 - l0; l += z[k] - l0; r += z[k] - r0; }
                else { l = l0; r = r0; }
            }
            return res;
        }
    };

    Wavelet wv;

    // seaweed 置換を構築（doubling による unit-Monge 積）
    static vector<int> seaweed_doubling(const vector<T>& v) {
        int n = (int)v.size();
        vector<int> a(n);
        iota(a.begin(), a.end(), 0);
        sort(a.begin(), a.end(), [&](int l, int r) { return v[l] == v[r] ? l < r : Compare()(v[l], v[r]); });

        vector<int> reca(n), recb(n), res(n, -1), buff(n), ida(n), idb(n);
        vector<bool> s(n);

        // 2 つの braid を 1 つに統合（ant の掃き取り）
        auto subsolve = [&](int l, int m, int r) -> void {
            for (int i = l, j = m, k = l; i < m || j < r; ++k) {
                if (j == r || (i < m && reca[ida[i]] > reca[ida[j]])) buff[k] = ida[i++];
                else buff[k] = ida[j++];
            }
            copy(buff.begin() + l, buff.begin() + r, ida.begin() + l);
            fill(s.begin() + l, s.begin() + m, true);
            fill(s.begin() + m, s.begin() + r, false);
            int x = l;
            for (int i = l, j = m, k = l; i < m || j < r; ++k) {
                if (j == r || (i < m && recb[idb[i]] < recb[idb[j]])) {
                    if (s[idb[i]]) {
                        res[idb[i]] = recb[idb[i]];
                        s[idb[i]] = false;
                        buff[k] = idb[i++];
                    } else {
                        while (!s[ida[x]]) ++x;
                        res[ida[x]] = recb[idb[i]];
                        s[ida[x]] = false;
                        buff[k] = ida[x];
                        ++i;
                    }
                } else {
                    if (reca[idb[j]] >= reca[ida[x]]) {
                        res[idb[j]] = recb[idb[j]];
                        buff[k] = idb[j++];
                    } else {
                        s[idb[j]] = true;
                        while (!s[ida[x]]) ++x;
                        res[ida[x]] = recb[idb[j]];
                        s[ida[x]] = false;
                        buff[k] = ida[x];
                        ++j;
                    }
                }
            }
            copy(buff.begin() + l, buff.begin() + r, idb.begin() + l);
            copy(res.begin() + l, res.begin() + r, recb.begin() + l);
        };

        auto seaweed_prod = [&](int l, int r) -> void {
            iota(ida.begin() + l, ida.begin() + r, l);
            iota(idb.begin() + l, idb.begin() + r, l);
            for (int b = 1; b < r - l; b <<= 1)
                for (int i = l + b; i < r; i += 2 * b) subsolve(i - b, i, min(i + b, r));
            fill(res.begin() + l, res.begin() + r, -1);
            for (int i = l; i < r; ++i)
                if (recb[i] < r) res[recb[i]] = reca[i];
        };

        auto solve = [&](int l, int m, int r) -> void {
            for (int i = l, j = m, k = l; i < m || j < r; ++k) {
                if (j == r || (i < m && a[i] < a[j])) { ida[i] = k; buff[k] = a[i++]; }
                else { ida[j] = k; buff[k] = a[j++]; }
            }
            copy(buff.begin() + l, buff.begin() + r, a.begin() + l);
            fill(recb.begin() + l, recb.begin() + r, r);
            for (int i = l; i < m; ++i) {
                reca[ida[i]] = ~res[i] ? ida[res[i]] : -1;
                recb[ida[i]] = ida[i];
            }
            for (int i = m; i < r; ++i) {
                reca[ida[i]] = ida[i];
                if (~res[i]) recb[ida[res[i]]] = ida[i];
            }
            seaweed_prod(l, r);
        };

        for (int b = 1; b < n; b <<= 1)
            for (int i = b; i < n; i += 2 * b) solve(i - b, i, min(i + b, n));
        for (int i = 0; i < n; ++i) ++res[i];
        return res;
    }

   public:
    StaticRangeLIS(const vector<T>& v) : wv(seaweed_doubling(v)) {}
    int query(int l, int r) { return wv.count_lt(0, r, l + 1) - l; }
};

int main() {
    int N = fio::readInt(), Q = fio::readInt();
    vector<int> A(N);
    for (int i = 0; i < N; i++) A[i] = fio::readInt();

    StaticRangeLIS<int> lis(A);

    string out;
    out.reserve((size_t)Q * 7);
    while (Q--) {
        int l = fio::readInt(), r = fio::readInt();
        out += to_string(lis.query(l, r));
        out += '\n';
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
