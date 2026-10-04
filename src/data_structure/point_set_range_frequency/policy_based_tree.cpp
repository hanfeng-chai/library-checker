#include <bits/extc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <toy/io.hpp>

using OrderedPairs = __gnu_pbds::tree<std::pair<toy::u32, int>, __gnu_pbds::null_type,
                                      std::less<std::pair<toy::u32, int>>, __gnu_pbds::rb_tree_tag,
                                      __gnu_pbds::tree_order_statistics_node_update>;

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<toy::u32> values(n);
    OrderedPairs positions;
    for (int i = 0; i < n; ++i) {
        values[i] = input.read_uniform<10, toy::u32>();
        positions.insert({values[i], i});
    }
    if (query_count && n == 0) input.skip_spaces();

    while (query_count--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        int first = input.read_uniform<6, toy::u32>();
        int second = input.read_uniform<10, toy::u32>();
        if (type == 0) {
            positions.erase({values[first], first});
            values[first] = second;
            positions.insert({values[first], first});
        } else {
            toy::u32 value = input.read_uniform<10, toy::u32>();
            int frequency =
                positions.order_of_key({value, second}) - positions.order_of_key({value, first});
            output.write_padded_u32(frequency);
        }
    }
}
