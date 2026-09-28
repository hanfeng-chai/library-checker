#include <toy/ds.hpp>
#include <toy/io.hpp>

struct Query {
    int type;
    int value;
};

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<int> initial(n);
    std::vector<int> coordinates;
    coordinates.reserve(n + query_count);
    for (int& value : initial) {
        value = input.read_uniform<10, int>();
        coordinates.push_back(value);
    }
    input.skip_spaces();
    std::vector<Query> queries(query_count);
    for (auto& query : queries) {
        query.type = input.read_fixed<1, toy::u32>();
        query.value = input.read_uniform<10, int>();
        if (query.type != 2) coordinates.push_back(query.value);
    }
    std::sort(coordinates.begin(), coordinates.end());
    coordinates.erase(std::unique(coordinates.begin(), coordinates.end()), coordinates.end());
    toy::FenwickTree<int> tree((int)coordinates.size());
    std::vector<unsigned char> present(coordinates.size());
    for (int value : initial) {
        int index = std::lower_bound(coordinates.begin(), coordinates.end(), value) -
                    coordinates.begin();
        present[index] = 1;
        tree.add(index, 1);
    }
    for (Query query : queries) {
        int index = std::lower_bound(coordinates.begin(), coordinates.end(), query.value) -
                    coordinates.begin();
        if (query.type == 0) {
            if (!present[index]) present[index] = 1, tree.add(index, 1);
        } else if (query.type == 1) {
            if (present[index]) present[index] = 0, tree.add(index, -1);
        } else if (query.type == 2) {
            int total = tree.prefix_sum(coordinates.size());
            if (query.value > total) output.write("-1\n");
            else output.writeln((toy::u64)coordinates[tree.lower_bound(query.value)]);
        } else if (query.type == 3) {
            int end = std::upper_bound(coordinates.begin(), coordinates.end(), query.value) -
                      coordinates.begin();
            output.writeln((toy::u64)tree.prefix_sum(end));
        } else if (query.type == 4) {
            int end = std::upper_bound(coordinates.begin(), coordinates.end(), query.value) -
                      coordinates.begin();
            int count = tree.prefix_sum(end);
            if (!count) output.write("-1\n");
            else output.writeln((toy::u64)coordinates[tree.lower_bound(count)]);
        } else {
            int count = tree.prefix_sum(index);
            int total = tree.prefix_sum(coordinates.size());
            if (count == total) output.write("-1\n");
            else output.writeln((toy::u64)coordinates[tree.lower_bound(count + 1)]);
        }
    }
}
