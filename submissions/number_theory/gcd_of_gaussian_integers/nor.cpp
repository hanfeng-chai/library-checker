#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,popcnt,lzcnt")

#include <bits/stdc++.h>

template <typename EuclideanRing>
constexpr EuclideanRing gcd(EuclideanRing a, EuclideanRing b) {
    using std::swap;
    while (b != EuclideanRing{}) {
        a %= b;
        swap(a, b);
    }
    return a;
}

// template <class T, class D = void>
// struct bigger_type {};
// template <class T>
// struct bigger_type<T, std::enable_if_t<sizeof(T) <= 4, void>> {
//     using type = std::int64_t;
// };
// template <class T>
// struct bigger_type<T, std::enable_if_t<4 < sizeof(T) && sizeof(T) <= 8, void>> {
//     using type = __int128_t;
// };

template <typename T>
T div_floor(T a, T b) {
    return a / b - (a % b < 0);
}

template <typename T>
T div_ceil(T a, T b) {
    return a / b + (a % b > 0);
}

template <typename T>
T div_round(T a, T b) {
    return div_floor(2 * a + b, 2 * b);
}

template <typename T>
struct GaussianInteger {
    using S = GaussianInteger<T>;
    T x{}, y{};

    constexpr T norm() const { return x * x + y * y; }
    constexpr S operator~() const { return {x, -y}; }
    constexpr S operator-() const { return {-x, -y}; }
    constexpr S& operator+=(const S& other) {
        x += other.x, y += other.y;
        return *this;
    }
    constexpr S operator+(const S& other) const { return S{*this} += other; }
    constexpr S& operator-=(const S& other) {
        x -= other.x, y -= other.y;
        return *this;
    }
    constexpr S operator-(const S& other) const { return S{*this} -= other; }
    constexpr S& operator*=(const S& other) {
        T x_new = x * other.x - y * other.y;
        T y_new = x * other.y + y * other.x;
        x = x_new, y = y_new;
        return *this;
    }
    constexpr S operator*(const S& other) const { return S{*this} *= other; }
    constexpr S& operator/=(const S& other) {
        (*this) *= ~other;
        T d = other.norm();
        x = div_round(x, d), y = div_round(y, d);
        return *this;
    }
    constexpr S operator/(const S& other) const { return S{*this} /= other; }
    constexpr S& operator%=(const S& other) {
        (*this) -= S{*this} / other * other;
        return *this;
    }
    constexpr S operator%(const S& other) const { return S{*this} %= other; }

    friend auto operator<=>(const S&, const S&) = default;
};

struct IOPre {
    static constexpr int TEN = 10, SZ = TEN * TEN * TEN * TEN;
    std::array<char, 4 * SZ> num;
    constexpr IOPre() : num{} {
        for (int i = 0; i < SZ; i++) {
            int n = i;
            for (int j = 3; j >= 0; j--) {
                num[i * 4 + j] = static_cast<char>(n % TEN + '0');
                n /= TEN;
            }
        }
    }
};
struct IO {
#if !HAVE_DECL_FREAD_UNLOCKED
    #define fread_unlocked fread
#endif
#if !HAVE_DECL_FWRITE_UNLOCKED
    #define fwrite_unlocked fwrite
#endif
    static constexpr int SZ = 1 << 17, LEN = 32, TEN = 10, HUNDRED = TEN * TEN, THOUSAND = HUNDRED * TEN, TENTHOUSAND = THOUSAND * TEN,
                         MAGIC_MULTIPLY = 205, MAGIC_SHIFT = 11, MASK = 15, TWELVE = 12, SIXTEEN = 16;
    static constexpr IOPre io_pre = {};
    std::array<char, SZ> input_buffer, output_buffer;
    int input_ptr_left, input_ptr_right, output_ptr_right;

    IO() : input_buffer{}, output_buffer{}, input_ptr_left{}, input_ptr_right{}, output_ptr_right{} {}
    IO(const IO&) = delete;
    IO(IO&&) = delete;
    IO& operator=(const IO&) = delete;
    IO& operator=(IO&&) = delete;

    ~IO() { flush(); }

    void load() {
        memmove(std::begin(input_buffer), std::begin(input_buffer) + input_ptr_left, input_ptr_right - input_ptr_left);
        input_ptr_right = input_ptr_right - input_ptr_left +
                          static_cast<int>(fread_unlocked(std::begin(input_buffer) + input_ptr_right - input_ptr_left, 1,
                                                          SZ - input_ptr_right + input_ptr_left, stdin));
        input_ptr_left = 0;
    }

    template <std::integral T>
    operator T() {
        if (input_ptr_left + LEN > input_ptr_right) load();
        char c = 0;
        do c = input_buffer[input_ptr_left++];
        while (c < '-');
        [[maybe_unused]] bool minus = false;
        if constexpr (std::is_signed_v<T>)
            if (c == '-') minus = true, c = input_buffer[input_ptr_left++];
        int x = 0;
        while (c >= '0') x = x * TEN + (c & MASK), c = input_buffer[input_ptr_left++];
        if constexpr (std::is_signed_v<T>)
            if (minus) x = -x;
        return x;
    }

    void skip_space() {
        if (input_ptr_left + LEN > input_ptr_right) load();
        while (input_buffer[input_ptr_left] <= ' ') input_ptr_left++;
    }

    void flush() {
        fwrite_unlocked(std::begin(output_buffer), 1, output_ptr_right, stdout);
        output_ptr_right = 0;
    }

    IO& operator<<(char c) {
        if (output_ptr_right > SZ - LEN) flush();
        output_buffer[output_ptr_right++] = c;
        return *this;
    }

    template <std::integral T>
    IO& operator<<(T x) {
        if (output_ptr_right > SZ - LEN) flush();
        if (!x) {
            output_buffer[output_ptr_right++] = '0';
            return *this;
        }
        if constexpr (std::is_signed_v<T>)
            if (x < 0) output_buffer[output_ptr_right++] = '-', x = -x;
        int i = TWELVE;
        std::array<char, SIXTEEN> buf{};
        while (x >= TENTHOUSAND) {
            memcpy(std::begin(buf) + i, std::begin(io_pre.num) + (x % TENTHOUSAND) * 4, 4);
            x /= TENTHOUSAND;
            i -= 4;
        }
        if (x < HUNDRED) {
            if (x < TEN) {
                output_buffer[output_ptr_right++] = static_cast<char>('0' + x);
            } else {
                std::uint32_t q = (static_cast<std::uint32_t>(x) * MAGIC_MULTIPLY) >> MAGIC_SHIFT;
                std::uint32_t r = static_cast<std::uint32_t>(x) - q * TEN;
                output_buffer[output_ptr_right] = static_cast<char>('0' + q);
                output_buffer[output_ptr_right + 1] = static_cast<char>('0' + r);
                output_ptr_right += 2;
            }
        } else {
            if (x < THOUSAND) {
                memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(io_pre.num) + (x << 2) + 1, 3), output_ptr_right += 3;
            } else {
                memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(io_pre.num) + (x << 2), 4), output_ptr_right += 4;
            }
        }
        memcpy(std::begin(output_buffer) + output_ptr_right, std::begin(buf) + i + 4, TWELVE - i);
        output_ptr_right += TWELVE - i;
        return *this;
    }
};
IO $;

int main() {
    int t = $;
    while (t--) {
        GaussianInteger<std::int64_t> a, b;
        a.x = $, a.y = $, b.x = $, b.y = $;
        auto x = gcd(a, b);
        $ << x.x << ' ' << x.y << '\n';
    }
}