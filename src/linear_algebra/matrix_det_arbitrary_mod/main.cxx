#include <toy/io_batch.h>
#include <toy/matrix_arbitrary.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 3>();
    u32 modulus = in.read<u32, 10>();
    Buffer<u32> a(n * n);
    read_bulk9(in, std::span(a.p, a.n));
    out.write(determinant_euclid(n, std::span(a.p, a.n), modulus));
}
