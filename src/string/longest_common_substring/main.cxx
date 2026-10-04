#include <toy/common_substring.h>
#include <toy/io.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    auto s = in.token(), t = in.token();
    for (u32 x : common_substring(s, t)) out.write(x, ' ');
    out.put('\n');
}
