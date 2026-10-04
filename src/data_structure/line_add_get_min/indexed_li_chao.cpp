#include <toy/io.hpp>
#include <toy/li_chao.hpp>
#include <toy/sort.hpp>

struct Operation {
    int32_t first;
    int64_t second;
};

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<Operation> operations(n + query_count);
    for (int i = 0; i < n; ++i) {
        operations[i] = {input.read_uniform<10, int32_t>(), input.read_uniform<19, int64_t>()};
    }

    std::vector<toy::u64> query_coordinates;
    query_coordinates.reserve(query_count);
    for (int i = n; i < n + query_count; ++i) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        int32_t first = input.read_uniform<10, int32_t>();
        if (type == 0) {
            operations[i] = {first, input.read_uniform<19, int64_t>()};
        } else {
            operations[i] = {first, toy::CompactLine::infinity + 1};
            toy::u32 ordered = std::bit_cast<toy::u32>(first) ^ 0x8000'0000U;
            query_coordinates.push_back((toy::u64)(toy::u32)i << 32 | ordered);
        }
    }

    toy::radix_sort_u32(query_coordinates.begin(), query_coordinates.end(),
                        [](toy::u64 value) { return (toy::u32)value; });
    std::vector<int32_t> coordinates(query_coordinates.size());
    for (int index = 0; index < (int)query_coordinates.size(); ++index) {
        toy::u64 event = query_coordinates[index];
        operations[event >> 32].first = index;
        toy::u32 bits = (toy::u32)event ^ 0x8000'0000U;
        coordinates[index] = std::bit_cast<int32_t>(bits);
    }

    toy::IndexedLiChaoTree tree(std::move(coordinates));
    for (const Operation &operation : operations) {
        if (operation.second <= toy::CompactLine::infinity) {
            tree.add({operation.first, operation.second});
        } else {
            output.writeln_i64(tree.minimum(operation.first));
        }
    }
}
