#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

using namespace std;
using namespace __gnu_pbds;
using ll = long long;
using ull = unsigned long long;

struct custom_hash {
  static uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
  }
  size_t operator()(uint64_t x) const {
    static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
    return splitmix64(x + FIXED_RANDOM);
  }
};

// 0-based, root = 0
vector<int> rooted_tree_isomorphism(const vector<int> &par){
  custom_hash hasher; // splitmix64
  gp_hash_table<ull, int, custom_hash> mp;
  vector<int> ret(par.size());
  vector<ull> sum(par.size());
  for (int i=(int)par.size()-1;i>=0;i--){
    ret[i] = mp.insert({sum[i], (int)mp.size()}).first->second;
    if (i) sum[par[i]] += hasher(ret[i]);
  }
  return ret;
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin >> n;

    vector<int> par(n);
    for (int i=1;i<n;i++) cin >> par[i];

    auto ans = rooted_tree_isomorphism(par);
    printf("%d\n", *max_element(ans.begin(), ans.end()) + 1);
    for (auto &x:ans) printf("%d ", x);
    printf("\n");
}