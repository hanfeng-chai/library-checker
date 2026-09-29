#include <toy/io.h>
using namespace toy;

int main() {
    Reader in;
    Writer out;
    int t = in.read();
    while (t--) {
        i128 a = in.read<i128>(), b = in.read<i128>();
        out.write(a + b);
    }
}
