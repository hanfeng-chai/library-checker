#include <bits/stdc++.h>
#include <bit>
#include <sys/mman.h>
#include <sys/stat.h>
using u64 = uint64_t;
using u32 = uint32_t;

static constexpr auto LUT1 = [] {
    std::array<std::array<char, 4>, 10000> res;
    for (int i = 0; i < 10000; ++i) {
        res[i][0] = '0' + i / 1000;
        res[i][1] = '0' + i / 100 % 10;
        res[i][2] = '0' + i / 10 % 10;
        res[i][3] = '0' + i % 10;
        if (i < 1000) res[i][0] = ' ';
        if (i < 100) res[i][1] = ' ';
        if (i < 10) res[i][2] = ' ';
        if (i < 1) res[i][3] = ' ';
    }
    return res;
}();

static constexpr auto LUT2 = [] {
    std::array<std::array<char, 4>, 10000> res;
    for (int i = 0; i < 10000; ++i) {
        res[i][0] = '0' + i / 1000;
        res[i][1] = '0' + i / 100 % 10;
        res[i][2] = '0' + i / 10 % 10;
        res[i][3] = '0' + i % 10;
    }
    return res;
}();

constexpr bool all_digit(u64 x) {
    x ^= 0x3030303030303030;
    x &= 0xf0f0f0f0f0f0f0f0;
    return !x;
}

template <typename K, typename V, u32 N> struct HashMap {
    K key[N];
    V val[N];
    std::bitset<N> use;
    static constexpr u32 shift = 64 - std::__lg(N);
    static constexpr u64 r = 11995408973635179863ULL;
    V& operator[](const K& i) {
        u32 hash = i * r >> shift;
        for (;;) {
            if (use[hash] == 0) {
                key[hash] = i;
                use[hash] = 1;
                return val[hash];
            }
            if (key[hash] == i) return val[hash];
            (++hash) &= (N - 1);
        }
    }
};

HashMap<u64, u64, 1 << 20> hash_map;

constexpr unsigned int N = 1 << 20;

class OpenAddrHashMap {
private:
    std::vector<u64> keys;
    std::vector<u64> values;
    std::bitset<N> used;
    static constexpr u64 P = 11995408973635179863ULL;
    static constexpr int SHIFT = 64 - (std::bit_width(N) - 1);

public:
    OpenAddrHashMap(): keys(N), values(N) {
    }

    void set(u64 key, u64 value) {
        u64 h = key * P >> SHIFT;
        while (used[h] && keys[h] != key) {
            if (++h >= N) h = 0;
        }
        keys[h] = key;
        values[h] = value;
        used.set(h);
    }

    u64 get(u64 key) {
        u64 h = key * P >> SHIFT;
        while (used[h] && keys[h] != key) {
            if (++h >= N) h = 0;
        }
        return values[h];
    }
};

int main() {
    struct stat st;
    fstat(0, &st);
    char* I = (char*)mmap(nullptr, st.st_size + 64, PROT_READ,
                          MAP_PRIVATE | MAP_POPULATE, 0, 0);
    const auto ii = [&]() {
        u64 x{};
        union {
            char ch[16];
            u64 d[2];
        };
        memcpy(ch, I, 16);
        u64 a = d[0], b = d[1];
        if (all_digit(a)) {
            a ^= 0x3030303030303030;
            a = (a * 10 + (a >> 8)) & 0x00ff00ff00ff00ff;
            a = (a * 100 + (a >> 16)) & 0x0000ffff0000ffff;
            a = (a * 10000 + (a >> 32)) & 0x00000000ffffffff;
            x = a, I += 8;
            if (all_digit(b)) {
                b ^= 0x3030303030303030;
                b = (b * 10 + (b >> 8)) & 0x00ff00ff00ff00ff;
                b = (b * 100 + (b >> 16)) & 0x0000ffff0000ffff;
                b = (b * 10000 + (b >> 32)) & 0x00000000ffffffff;
                x = a * 100000000 + b, I += 8;
            }
        }
        for (; *I > 47; ++I) x = x * 10 + *I - 48;
        ++I;
        return x;
    };
    const auto is_query = [&]() {
        char value = *I;
        I += 2;
        return value == '1';
    };
    char bufO[1 << 19], *ptrO = bufO, *endO = bufO + sizeof(bufO);
    auto flush = [&]() {
        write(1, bufO, ptrO - bufO);
        ptrO = bufO;
    };
    const auto print1 = [&](u64 x) { memcpy(ptrO, &LUT1[x], 4), ptrO += 4; };
    const auto print2 = [&](u64 x) { memcpy(ptrO, &LUT2[x], 4), ptrO += 4; };
    const auto oo = [&](u64 x) {
        if (endO - ptrO < 40) flush();
        if (x > 999'9999'9999'9999) {
            print1(x / 10000 / 10000 / 10000 / 10000);
            print2(x / 10000 / 10000 / 10000 % 10000);
            print2(x / 10000 / 10000 % 10000);
            print2(x / 10000 % 10000);
            print2(x % 10000);
        } else if (x > 999'9999'9999) {
            print1(x / 10000 / 10000 / 10000);
            print2(x / 10000 / 10000 % 10000);
            print2(x / 10000 % 10000);
            print2(x % 10000);
        } else if (x > 999'9999) {
            print1(x / 10000 / 10000);
            print2(x / 10000 % 10000);
            print2(x % 10000);
        } else if (x > 999) {
            print1(x / 10000);
            print2(x % 10000);
        } else if (x > 0) {
            print1(x);
        } else {
            *ptrO = ' ', ++ptrO;
            *ptrO = '0', ++ptrO;
        }
    };
	OpenAddrHashMap hm;
    for (int i = ii(); i > 0; --i) {
        if (is_query()) {
			oo(hm.get(ii()));
        } else {
            auto k = ii();
			hm.set(k, ii());
        }
    }
    flush();
}