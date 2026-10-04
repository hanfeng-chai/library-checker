#include <toy/io_batch.h>
#include <toy/pfaffian.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 3>();
    n *= 2;
    Buffer<u32> a(n * n);
    read_bulk9(in, std::span(a.p, a.n));
    out.write(pfaffian(n, std::span(a.p, a.n)));
}
