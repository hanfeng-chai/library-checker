#include <bits/extc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <toy/io.hpp>

using OrderedSet =
    __gnu_pbds::tree<int, __gnu_pbds::null_type, std::less<int>, __gnu_pbds::rb_tree_tag,
                     __gnu_pbds::tree_order_statistics_node_update>;

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int queries = input.read_uniform<6, toy::u32>();
    OrderedSet set;
    while (n--) set.insert(input.read_uniform<10, int>());
    input.skip_spaces();
    while (queries--) {
        int type = input.read_fixed<1, toy::u32>();
        int value = input.read_uniform<10, int>();
        if (type == 0)
            set.insert(value);
        else if (type == 1)
            set.erase(value);
        else if (type == 2) {
            auto iterator = set.find_by_order(value - 1);
            if (iterator == set.end())
                output.write("-1\n");
            else
                output.writeln((toy::u64)*iterator);
        } else if (type == 3) {
            output.writeln((toy::u64)set.order_of_key(value + 1));
        } else if (type == 4) {
            auto iterator = set.upper_bound(value);
            if (iterator == set.begin())
                output.write("-1\n");
            else
                output.writeln((toy::u64) * --iterator);
        } else {
            auto iterator = set.lower_bound(value);
            if (iterator == set.end())
                output.write("-1\n");
            else
                output.writeln((toy::u64)*iterator);
        }
    }
}
