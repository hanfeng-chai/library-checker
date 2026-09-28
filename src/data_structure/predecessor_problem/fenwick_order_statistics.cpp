#include <toy/ds.hpp>
#include <toy/io.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<8, toy::u32>();
    int queries = input.read_uniform<7, toy::u32>();
    toy::FenwickTree<int> tree(n);
    std::vector<bool> present(n);
    std::string_view initial = input.read_token(n);
    for (int i = 0; i < n; ++i)
        if (initial[i] == '1') present[i] = true, tree.add(i, 1);
    while (queries--) {
        int type = input.read_fixed<1, toy::u32>();
        int key = input.read_uniform<7, toy::u32>();
        if (type == 0 && !present[key]) present[key] = true, tree.add(key, 1);
        else if (type == 1 && present[key]) present[key] = false, tree.add(key, -1);
        else if (type == 2)
            output.writeln_fixed<1>((toy::u64)present[key]);
        else if (type == 3) {
            int before = tree.prefix_sum(key);
            int total = tree.prefix_sum(n);
            int answer = before == total ? -1 : tree.lower_bound(before + 1);
            if (answer < 0) output.write("-1\n");
            else output.writeln((toy::u64)answer);
        } else if (type == 4) {
            int count = tree.prefix_sum(key + 1);
            int answer = count ? tree.lower_bound(count) : -1;
            if (answer < 0) output.write("-1\n");
            else output.writeln((toy::u64)answer);
        }
    }
}
