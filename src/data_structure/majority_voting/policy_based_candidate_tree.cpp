#include <bits/extc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <toy/io.hpp>
#include <toy/range.hpp>

using OrderedPairs = __gnu_pbds::tree<std::pair<toy::u32, int>, __gnu_pbds::null_type,
                                      std::less<std::pair<toy::u32, int>>, __gnu_pbds::rb_tree_tag,
                                      __gnu_pbds::tree_order_statistics_node_update>;

namespace {
struct Vote {
    toy::u32 value = 0;
    int balance = 0;
};

struct MergeVote {
    Vote operator()(Vote left, Vote right) const {
        if (!left.balance) return right;
        if (!right.balance) return left;
        if (left.value == right.value) return {left.value, left.balance + right.balance};
        if (left.balance > right.balance) return {left.value, left.balance - right.balance};
        return {right.value, right.balance - left.balance};
    }
};
} // namespace

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<toy::u32> values(n);
    std::vector<Vote> leaves(n);
    OrderedPairs positions;
    for (int i = 0; i < n; ++i) {
        values[i] = input.read_uniform<10, toy::u32>();
        leaves[i] = {values[i], 1};
        positions.insert({values[i], i});
    }
    toy::SegmentTree candidates(leaves, Vote{}, MergeVote{});

    while (query_count--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        int first = input.read_uniform<6, toy::u32>();
        int second = input.read_uniform<10, toy::u32>();
        if (type == 0) {
            positions.erase({values[first], first});
            values[first] = second;
            positions.insert({values[first], first});
            candidates.set(first, {values[first], 1});
        } else {
            toy::u32 candidate = candidates.fold(first, second).value;
            int frequency = positions.order_of_key({candidate, second}) -
                            positions.order_of_key({candidate, first});
            if (2 * frequency > second - first)
                output.writeln((toy::u64)candidate);
            else
                output.write("-1\n");
        }
    }
}
