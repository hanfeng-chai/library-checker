#include <toy/io.hpp>

class RecursiveDsu {
    std::vector<int> parent, rank;

  public:
    explicit RecursiveDsu(int n) : parent(n), rank(n) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int leader(int x) { return parent[x] == x ? x : parent[x] = leader(parent[x]); }

    void merge(int a, int b) {
        a = leader(a);
        b = leader(b);
        if (a == b) return;
        if (rank[a] < rank[b]) std::swap(a, b);
        parent[b] = a;
        if (rank[a] == rank[b]) ++rank[a];
    }
};

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::CompactWriter<> output;
    int n = input.read_uniform<6, toy::u32>();
    int queries = input.read_uniform<6, toy::u32>();
    RecursiveDsu dsu(n);
    while (queries--) {
        int type = input.read_fixed<1, toy::u32>();
        int u = input.read_uniform<6, toy::u32>();
        int v = input.read_uniform<6, toy::u32>();
        if (type == 0)
            dsu.merge(u, v);
        else
            output.writeln_fixed<1>((toy::u64)(dsu.leader(u) == dsu.leader(v)));
    }
}
