#include <toy/io.hpp>

namespace {
struct Item {
    toy::u32 value;
    toy::u64 prefix;
};
}

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    int size = std::bit_ceil((unsigned)n);
    std::vector<std::vector<Item>> tree(2 * size);
    for (int i = 0; i < n; ++i)
        tree[size + i].push_back(
            {input.read_uniform<10, toy::u32>(), 0});
    for (int node = size - 1; node; --node) {
        auto& result = tree[node];
        const auto& left = tree[2 * node];
        const auto& right = tree[2 * node + 1];
        result.reserve(left.size() + right.size());
        std::merge(
            left.begin(), left.end(), right.begin(), right.end(),
            std::back_inserter(result),
            [](const Item& a, const Item& b) { return a.value < b.value; });
    }
    for (auto& bucket : tree) {
        toy::u64 sum = 0;
        for (Item& item : bucket) item.prefix = sum += item.value;
    }

    while (query_count--) {
        int left = input.read_uniform<6, toy::u32>();
        int right = input.read_uniform<6, toy::u32>();
        toy::u32 bound = input.read_uniform<10, toy::u32>();
        int count = 0;
        toy::u64 sum = 0;
        auto take = [&](int node) {
            const auto& bucket = tree[node];
            int end = std::upper_bound(
                bucket.begin(), bucket.end(), bound,
                [](toy::u32 value, const Item& item) {
                    return value < item.value;
                }) - bucket.begin();
            count += end;
            if (end) sum += bucket[end - 1].prefix;
        };
        for (left += size, right += size; left < right;
             left >>= 1, right >>= 1) {
            if (left & 1) take(left++);
            if (right & 1) take(--right);
        }
        output.write_token((toy::u64)count);
        output.write_token(sum);
        output.put('\n');
    }
}
