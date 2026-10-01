#include <toy/io.h>
using namespace toy;

template<class T, int Digits = 1>
u64 sum(Reader& in, int n, u128 bound) {
    constexpr u128 limit = [] { u128 x = 1; for (int i = 0; i < Digits; ++i) x *= 10; return x; }();
    if constexpr (Digits < 20) if (bound >= limit) return sum<T, Digits + 1>(in, n, bound);
    u64 s = 0;
    while (n--) s += in.read<T, Digits>();
    return s;
}

int main() {
    Reader in;
    Writer out;
    int n = in.read();
    i128 low = in.read<i128>(), high = in.read<i128>();
    u128 bound = std::max(-low, high);
    out.write(low < 0 ? sum<i64>(in, n, bound) : sum<u64>(in, n, bound));
}
