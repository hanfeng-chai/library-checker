#include <vector>
#include <limits>

namespace soryuusi{

using namespace std;

struct PermutationTree{
  struct StaticRMQ{
    static constexpr int b = 3;
    static constexpr int B = 1 << b;
    static constexpr int inf = numeric_limits<int>::max();
    int n, m, h;
    vector<int> data;
    vector<int> left_prod;
    vector<int> right_prod;
    vector<vector<int>> sparse_table;
    StaticRMQ(){}
    StaticRMQ(vector<int> v){
      data = v;
      n = data.size();
      if(n <= B) return;
      m = ((n-1) >> b) + 1;
      left_prod = vector<int>(n+1,inf);
      right_prod = vector<int>(n+1,inf);
      h = 31-__builtin_clz(m);
      sparse_table = vector<vector<int>>(h+1, vector<int>(m+1,inf));
      int temp = inf;
      for(int i = 0; i < n; i++){
        left_prod[i] = temp;
        temp = min(temp, data[i]);
        if(!(~i&(B-1))) {
          sparse_table[0][i >> b] = temp;
          temp = inf;
        }
      }
      left_prod[n] = temp;
      if(n&(B-1)){
        sparse_table[0][m-1] = temp;
        temp = inf;
      }
      for(int i = (n&~(B-1))-1; i >= 0; i--){
        temp = min(temp, data[i]);
        right_prod[i] = temp;
        if(!(i&(B-1))) {
          temp = inf;
        }
      }
      sparse_table[0][m] = sparse_table[0][m-1];
      for(int h0 = 1; h0 <= h; h0++){
        int i = 1 << h0;
        for(int j = i; j <= m; j += i << 1){
          temp = inf;
          for(int k = j-1; k >= j - i; k--){
            temp = min(sparse_table[0][k], temp);
            sparse_table[h0][k] = temp;
          }
          temp = inf;
          for(int k = j; k < min(j + i,m+1); k++){
            sparse_table[h0][k] = temp;
            temp = min(temp, sparse_table[0][k]);
          }
        }
      }
      for(int i = 1; i <= m; i += 2) sparse_table[0][i] = inf;
    }
    int naive_prod(int l, int r){
      int res = inf;
      for(int i = l; i < r; i++) res = min(res,data[i]);
      return res;
    }
    int prod(int l, int r){
      if(l >= r) return inf;
      if(r-l <= 8) return naive_prod(l,r);
      int l1 = (l >> b) + 1;
      int r1 = r >> b;
      if(l1 > r1) return naive_prod(l,r);
      if(l1 == r1) return min(right_prod[l], left_prod[r]);
      int mid = 31-__builtin_clz(l1^r1);
      return  min(min(right_prod[l],min(sparse_table[mid][l1], sparse_table[mid][r1])), left_prod[r]);
    }
  };
  enum NodeType {
    Prime,
    Dec,
    Inc,
    One
  };
  struct Node {
    int p, l, r;
    NodeType type;
  };
  vector<Node> tree;

  PermutationTree();
  template<class INT> PermutationTree(const vector<INT>& p) {build(p);}
  template<class INT> void build(vector<INT> p) {
    tree.clear();
    int n = p.size();
    vector<int> ip(n);
    for(int i = 0; i < n; ++i) ip[p[i]] = i;
    StaticRMQ Q(move(ip));
    struct Data{int l, d, u;};
    vector<Data> st;
    vector<int> ids(n);
    for(int i = 0; i < n; ++i) {
      tree.emplace_back(-1,i,i+1,One);
      ids[i] = i;
    }
    int k = n;
    for(int r = 1; r <= n; ++r){
      int y = r - 1;
      int d = p[y], u = p[y] + 1;
      int pd = p[y], pu = p[y] + 1;
      while(st.size()){
        auto& [x, xd, xu] = st.back();
        if(xd > pd) xd = pd;
        if(xu < pu) xu = pu;
        if(r-x == xu-xd) {
          if(tree[ids[x]].r != y){
            tree.emplace_back(-1,x,r,Prime);
            for(int z = x; z < r; z = tree[ids[z]].r) tree[ids[z]].p = k;
            ids[x] = k++;
          }else {
            NodeType t = p[x] < p[y] ? Inc : Dec;
            if(tree[ids[x]].type == t) tree[ids[x]].r = r, tree[ids[y]].p = ids[x];
            else{
              tree.emplace_back(-1,x,r,t);
              tree[ids[x]].p = k, tree[ids[y]].p = k;
              ids[x] = k++;
            }
          }
          y = x, d = xd, u = xu;
        }
        else if(Q.prod(xd,xu) >= x) break;
        pd = xd, pu = xu;
        st.pop_back();
      }
      st.emplace_back(y,d,u);
    }
  }
};

} //namespace soryuusi

#include <iostream>

int main(){
  std::cin.tie(nullptr), std::ios_base::sync_with_stdio(false);
  int n;
  std::cin >> n;
  std::vector<int> p(n);
  for(int i = 0; i < n; ++i) std::cin >> p[i];
  soryuusi::PermutationTree PT(p);
  auto& tree = PT.tree;
  std::cout << tree.size() << "\n";
  for(auto [p,l,r,type] : PT.tree) {
    std::cout << p << " " << l << " " << r-1 << (type ? " linear\n" : " prime\n");
  }
}