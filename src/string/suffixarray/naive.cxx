#include <toy/io_batch.h>
#include <toy/suffix_array.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    auto s = in.token();
    auto answer = suffix_array<char>(std::span(s.data(), s.size()), 127);
    write_bulk6(out, std::span(answer.p, answer.n));
    out.put('\n');
}
