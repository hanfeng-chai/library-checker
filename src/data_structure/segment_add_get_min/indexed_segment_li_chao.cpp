#include <toy/io.hpp>
#include <toy/li_chao.hpp>
#include <toy/sort.hpp>

struct Operation {
    int left;
    int right;
    toy::CompactLine line;
};

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<Operation> operations(n + query_count);
    std::vector<toy::u64> events;
    events.reserve(2 * n + 2 * query_count);

    auto add_event = [&](int operation, toy::u32 kind, int32_t coordinate) {
        toy::u32 ordered =
            std::bit_cast<toy::u32>(coordinate) ^ 0x8000'0000U;
        toy::u32 payload = (toy::u32)operation * 4 + kind;
        events.push_back((toy::u64)payload << 32 | ordered);
    };
    auto read_segment = [&](int operation) {
        int32_t left = input.read_uniform<10, int32_t>();
        int32_t right = input.read_uniform<10, int32_t>();
        int32_t slope = input.read_uniform<10, int32_t>();
        int64_t intercept = input.read_uniform<19, int64_t>();
        operations[operation] = {left, right, {slope, intercept}};
        add_event(operation, 0, left);
        add_event(operation, 1, right);
    };

    for (int i = 0; i < n; ++i) read_segment(i);
    for (int i = n; i < n + query_count; ++i) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        if (type == 0) {
            read_segment(i);
        } else {
            int32_t x = input.read_uniform<10, int32_t>();
            operations[i] = {
                x, 0, {0, toy::CompactLine::infinity + 1}};
            add_event(i, 2, x);
        }
    }

    toy::radix_sort_u32(
        events.begin(), events.end(),
        [](toy::u64 event) { return (toy::u32)event; });
    std::vector<int32_t> coordinates;
    coordinates.reserve(query_count);
    toy::u32 previous = std::numeric_limits<toy::u32>::max();
    for (toy::u64 event : events) {
        toy::u32 ordered = (toy::u32)event;
        toy::u32 payload = event >> 32;
        Operation& operation = operations[payload / 4];
        toy::u32 kind = payload & 3;
        if (kind == 2) {
            if (ordered != previous) {
                toy::u32 bits = ordered ^ 0x8000'0000U;
                coordinates.push_back(std::bit_cast<int32_t>(bits));
                previous = ordered;
            }
            operation.left = coordinates.size() - 1;
        } else {
            int index = coordinates.size() - (ordered == previous);
            if (kind == 0)
                operation.left = index;
            else
                operation.right = index;
        }
    }

    toy::IndexedLiChaoTree tree(std::move(coordinates));
    for (const Operation& operation : operations) {
        if (operation.line.intercept <= toy::CompactLine::infinity) {
            tree.add_segment(
                operation.left, operation.right, operation.line);
        } else {
            int64_t answer = tree.minimum(operation.left);
            if (answer == toy::CompactLine::infinity)
                output.write("INFINITY\n");
            else
                output.writeln_i64(answer);
        }
    }
}
