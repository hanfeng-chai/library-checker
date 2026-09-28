#include <toy/io.hpp>

struct SplitMixHash {
    std::size_t operator()(toy::u64 value) const {
        value += 0x9e3779b97f4a7c15ULL;
        value = (value ^ (value >> 30)) * 0xbf58476d1ce4e5b9ULL;
        value = (value ^ (value >> 27)) * 0x94d049bb133111ebULL;
        return value ^ (value >> 31);
    }
};

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int queries = input.read_uniform<7, toy::u32>();
    std::unordered_map<toy::u64, toy::u64, SplitMixHash> map;
    map.reserve(queries);
    while (queries--) {
        int type = input.read_fixed<1, toy::u32>();
        toy::u64 key = input.read_uniform<19, toy::u64>();
        if (type == 0)
            map[key] = input.read_uniform<19, toy::u64>();
        else {
            auto iterator = map.find(key);
            output.writeln(iterator == map.end() ? 0 : iterator->second);
        }
    }
}
