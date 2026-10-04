#include <toy/io.hpp>
#include <toy/li_chao.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    toy::DynamicLiChaoTree tree(n + query_count);
    while (n--) tree.add({input.read_uniform<10, int64_t>(), input.read_uniform<19, int64_t>()});
    while (query_count--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        int64_t first = input.read_uniform<10, int64_t>();
        if (!type)
            tree.add({first, input.read_uniform<19, int64_t>()});
        else
            output.writeln_i64(tree.minimum(first));
    }
}
