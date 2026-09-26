
#include <bits/stdc++.h>

struct DSU {
  public:
      DSU(int n): n(n) , p(n), rank(n,1) { std::iota(p.begin(),p.end(), 0); }
      int leader(int u){ return u == p[u] ? u : p[u] = leader(p[u]); }
      bool same(int u, int v){ return leader(u) == leader(v)  ; }
      int size(int u){ return rank[leader(u)]; }
      bool merge(int u, int v){
        u = leader(u), v = leader(v) ; 
        if (u == v){
          return false; 
        } else {
          if (rank[u] > rank[v]) std::swap(u,v) ; 
          p[u] = v , rank[v] += rank[u] ; 
          return true; 
        }
      }
  private:
      std::vector<int> p, rank ; 
      int n ;
};

// Divide combine tree (a.k.a permutation tree)
struct dct {
    static constexpr int inf = std::numeric_limits < int > ::max() / 4;
    enum NodeType {
        Cut, // cut (prime)
        Dec, // linear decreasing
        Inc, // linear decreasing
        Leaf // leaf
    };
    struct Node {
        NodeType type = Leaf;
        int l = -1, r = -1, start = -1, parent = -1;
        int size() { return r - l + 1; }
    };
    struct Info { int id, Min, Max; };
    std::vector < Node > nodes;
    std::vector < int > forests = {-1};
    int root = -1;
    dct() {}
    Node & operator[](int i) { return nodes[i]; };
    explicit dct(std::vector < int > & Input){
        const int n = Input.size();
        nodes.resize(n);
        DSU uf(n);
        std::vector<bool> active(n,false);
        std::vector<Info> check_point;
        for (int i = 0; i < n; i++) {
            int v = i, label = Input[i], Min = Input[i], Max = Input[i], pos;
            active[Input[i]] = true;
            if(label > 0 && active[label-1]) uf.merge(label,label-1);
            if(label + 1 < n && active[label+1]) uf.merge(label,label+1);
            nodes[v] = {Leaf, Input[i], Input[i], i };
            while (forests.size() > 1) {
                int x = forests.back();
                for(pos = -1; not check_point.empty() && pos == -1;){
                    auto &u = check_point.back();
                    u.Min = Min = std::min(u.Min, Min), u.Max = Max = std::max(u.Max, Max);
                    if(not uf.same(nodes[u.id].l,label)) break;
                    check_point.pop_back();
                    if(i - nodes[u.id].start == Max - Min){ pos = nodes[u.id].start; }
                }
                if(pos == -1) break;
                if (nodes[x].type == Dec and nodes[v].r + 1 == nodes[x].l or nodes[x].type == Inc and nodes[x].r + 1 == nodes[v].l) {
                    nodes[v].parent = x; // make x parent of v
                    if (nodes[x].type == Dec) nodes[x].l = nodes[v].l;
                    else nodes[x].r = nodes[v].r;
                    forests.pop_back();
                    v = x;
                } else {
                    int u = nodes.size();
                    nodes.push_back(nodes[v]);
                    nodes[v].parent = u;
                    if (pos == nodes[x].start) nodes[u].type = nodes[x].r < nodes[v].r ? Inc : Dec; // new Join node
                    else nodes[u].type = Cut; // new Cut node 
                    for (int x; nodes[u].start != pos; forests.pop_back()) {
                        x = forests.back();
                        nodes[u].start = nodes[x].start, nodes[u].l = Min, nodes[u].r = Max, nodes[x].parent = u;
                    }
                    v = u;
                }
            }
            check_point.push_back(Info{v, nodes[v].l, nodes[v].r});
            forests.push_back(v);
        }
        root = forests.back();
    }
    
    auto adj_list() {
        const int N = nodes.size();
        std::vector < std::vector < int >> adj(N);
        for (int i = 0; i < N; i++)
            if (nodes[i].parent != -1) adj[nodes[i].parent].push_back(i);
        return adj;
    }
};

int read(){
    int ans=0; char c=getchar();
    while (!isdigit(c)) c=getchar();
    while (isdigit(c)) ans=ans*10+c-48,c=getchar();
    return ans;
}

int main() {
    int n = read();
    std::vector < int > p(n);
    for (int i = 0; i < n; i++) p[i] = read();
    dct tree(p);
    const int N = tree.nodes.size();
    printf("%d\n",N);
    for (int i = N-1; i >= 0; i--) {
        printf("%d %d %d %s\n",tree[i].parent == -1 ? -1 : N - 1 - tree[i].parent , tree[i].start , tree[i].start + tree[i].size() - 1 , (tree[i].type == tree.Cut ? "prime" : "linear"));
    }
}
