#include <toy/io.hpp>
#include <toy/persistent.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int query_count = input.read_uniform<6, toy::u32>();
    toy::PersistentQueue<toy::u32, 20> queue(query_count);
    using Version = decltype(queue)::Version;
    std::vector<Version> versions(query_count + 1);
    for (int query = 0; query < query_count; ++query) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        int base = input.read_uniform<6, int>() + 1;
        if (type == 0) {
            toy::u32 value = input.read_uniform<10, toy::u32>();
            versions[query + 1] = queue.push(versions[base], value);
        } else {
            auto [version, value] = queue.pop(versions[base]);
            versions[query + 1] = version;
            output.write_padded_u32(value);
        }
    }
}
