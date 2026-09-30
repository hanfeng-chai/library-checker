#include <toy/io.h>
using namespace toy;

int main() {
    Reader in;
    Writer out;
    int n = in.read();
    i128 low = in.read<i128>();
    in.read<i128>();
    auto sum = [&]<class T>() {
        u64 s = 0;
        for (int i = 0; i < n; ++i) s += in.read<T>();
        return s;
    };
    out.write(low < 0 ? sum.operator()<i64>() : sum.operator()<u64>());
}
