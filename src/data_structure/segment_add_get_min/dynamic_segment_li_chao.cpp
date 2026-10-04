#include <toy/io.hpp>
#include <toy/li_chao.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    toy::DynamicLiChaoTree tree(n + query_count);
    while (n--) {
        int64_t left = input.read_uniform<10, int64_t>();
        int64_t right = input.read_uniform<10, int64_t>();
        int64_t slope = input.read_uniform<10, int64_t>();
        int64_t intercept = input.read_uniform<19, int64_t>();
        tree.add_segment(left, right, {slope, intercept});
    }
    while (query_count--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        int64_t first = input.read_uniform<10, int64_t>();
        if (!type) {
            int64_t right = input.read_uniform<10, int64_t>();
            int64_t slope = input.read_uniform<10, int64_t>();
            int64_t intercept = input.read_uniform<19, int64_t>();
            tree.add_segment(first, right, {slope, intercept});
        } else {
            int64_t answer = tree.minimum(first);
            if (answer == std::numeric_limits<int64_t>::max())
                output.write("INFINITY\n");
            else
                output.writeln_i64(answer);
        }
    }
}
