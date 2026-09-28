#include <toy/io.hpp>

int main() {
    toy::Reader in(toy::direct_mapping);
    toy::CompactWriter out;
    int t = in.read_uniform<6, toy::u32>();
    while (t--) {
        toy::i128 a = in.read_staged_i128();
        toy::i128 b = in.read_staged_i128();
        out.writeln(a + b);
    }
}