#include <toy/ds.hpp>
#include <toy/io.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<8, toy::u32>();
    int queries = input.read_uniform<7, toy::u32>();
    static toy::BoundedPredecessorSet<10'000'000> set;
    std::string_view initial = input.read_token(n);
    set.assign(initial);
    while (queries--) {
        int type = input.read_fixed<1, toy::u32>();
        int key = input.read_uniform<7, toy::u32>();
        if (type == 0) set.insert(key);
        else if (type == 1) set.erase(key);
        else if (type == 2) output.writeln_fixed<1>((toy::u64)set.contains(key));
        else {
            int answer = type == 3 ? set.successor(key) : set.predecessor(key);
            if (answer < 0) output.write("-1\n");
            else output.writeln((toy::u64)answer);
        }
    }
}
