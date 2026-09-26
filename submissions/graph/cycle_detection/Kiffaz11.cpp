// GOD-TIER COMPETITIVE PROGRAMMING FAST I/O TEMPLATE
// Target: GNU++20 on Linux (portable fread/fwrite fallback is included).
// Compile: g++ -std=gnu++20 -O2 -pipe -DNDEBUG solution.cpp -o solution
//
// Do not mix fastio::in/out with cin/cout/scanf/printf.
// Supported input: all 8/16/32/64-bit integers, GNU signed/unsigned
// __int128, float/double/long double, char, token strings, and whole lines.
// Supported output: the same numeric types, chars, bools, and strings.
//
// Typical use:
//   int n; long long x; std::string s;
//   fastio::in >> n >> x >> s;
//   fastio::out(n, ' ', x, ' ', s, '\n');
//   fastio::out.writeFixed(3.1415926535, 10);
//
// For an interactive problem, call fastio::out.flush() after every query.
// Integer input deliberately assumes the token fits its destination type.
// Define FASTIO_DISABLE_MMAP before this file to force the read() path.

#include <bits/stdc++.h>

#if defined(__unix__) || defined(__APPLE__)
#include <cerrno>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#define FASTIO_HAS_POSIX 1
#else
#define FASTIO_HAS_POSIX 0
#endif

#ifndef FASTIO_INPUT_BUFFER_SIZE
#define FASTIO_INPUT_BUFFER_SIZE (1u << 20)
#endif

#ifndef FASTIO_OUTPUT_BUFFER_SIZE
#define FASTIO_OUTPUT_BUFFER_SIZE (1u << 20)
#endif

namespace fastio {

#if defined(__SIZEOF_INT128__)
using i128 = __int128_t;
using u128 = __uint128_t;
#endif

namespace detail {

template <class T>
using bare_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <class T>
inline constexpr bool is_extended_integer_v = std::is_integral_v<bare_t<T>>
#if defined(__SIZEOF_INT128__)
    || std::is_same_v<bare_t<T>, i128> || std::is_same_v<bare_t<T>, u128>
#endif
    ;

template <class T>
inline constexpr bool is_fast_integer_v =
    is_extended_integer_v<T> &&
    !std::is_same_v<bare_t<T>, bool> &&
    !std::is_same_v<bare_t<T>, char> &&
    !std::is_same_v<bare_t<T>, wchar_t> &&
    !std::is_same_v<bare_t<T>, char8_t> &&
    !std::is_same_v<bare_t<T>, char16_t> &&
    !std::is_same_v<bare_t<T>, char32_t>;

template <class T>
struct unsigned_of {
    using type = std::make_unsigned_t<T>;
};

#if defined(__SIZEOF_INT128__)
template <>
struct unsigned_of<i128> {
    using type = u128;
};
template <>
struct unsigned_of<u128> {
    using type = u128;
};
#endif

template <class T>
using unsigned_of_t = typename unsigned_of<bare_t<T>>::type;

template <class T>
inline constexpr bool is_signed_integer_v = std::is_signed_v<bare_t<T>>
#if defined(__SIZEOF_INT128__)
    || std::is_same_v<bare_t<T>, i128>
#endif
    ;

inline constexpr char DIGIT_PAIRS[] =
    "00010203040506070809"
    "10111213141516171819"
    "20212223242526272829"
    "30313233343536373839"
    "40414243444546474849"
    "50515253545556575859"
    "60616263646566676869"
    "70717273747576777879"
    "80818283848586878889"
    "90919293949596979899";

}  // namespace detail

class FastInput {
    static constexpr std::size_t BUFFER_SIZE = FASTIO_INPUT_BUFFER_SIZE;

    alignas(64) char buffer_[BUFFER_SIZE];
    const char* cursor_ = buffer_;
    const char* end_ = buffer_;

#if FASTIO_HAS_POSIX
    const char* mapped_ = nullptr;
    std::size_t mapped_size_ = 0;
#endif

    bool good_ = true;

    [[gnu::always_inline]] inline bool refill() noexcept {
        if (cursor_ != end_) [[likely]] return true;

#if FASTIO_HAS_POSIX
        if (mapped_ != nullptr) return false;

        ssize_t count;
        do {
            count = ::read(STDIN_FILENO, buffer_, BUFFER_SIZE);
        } while (count < 0 && errno == EINTR);

        if (count <= 0) return false;
        cursor_ = buffer_;
        end_ = buffer_ + count;
#else
        const std::size_t count =
            std::fread(buffer_, 1, BUFFER_SIZE, stdin);
        if (count == 0) return false;
        cursor_ = buffer_;
        end_ = buffer_ + count;
#endif
        return true;
    }

    [[gnu::always_inline]] inline bool skipSpaces() noexcept {
        for (;;) {
            if (!refill()) return false;
            while (cursor_ != end_ &&
                   static_cast<unsigned char>(*cursor_) <= ' ') {
                ++cursor_;
            }
            if (cursor_ != end_) return true;
        }
    }

    template <class T>
    [[gnu::always_inline]] static inline void assignInteger(
        T& out, detail::unsigned_of_t<T> value, bool negative) noexcept {
        using U = detail::unsigned_of_t<T>;

        if constexpr (detail::is_signed_integer_v<T>) {
            if (negative) {
                const U min_magnitude =
                    static_cast<U>(std::numeric_limits<T>::max()) + U{1};
                if (value == min_magnitude) [[unlikely]] {
                    out = std::numeric_limits<T>::min();
                } else {
                    out = static_cast<T>(-static_cast<T>(value));
                }
            } else {
                out = static_cast<T>(value);
            }
        } else {
            out = negative ? U{0} - value : value;
        }
    }

public:
    FastInput() noexcept {
#if FASTIO_HAS_POSIX && !defined(FASTIO_DISABLE_MMAP)
        struct stat info {};
        const off_t offset = ::lseek(STDIN_FILENO, 0, SEEK_CUR);

        // mmap removes the userspace copy when stdin is a redirected regular
        // file. Pipes, terminals, and unsupported cases use read() instead.
        if (offset == 0 && ::fstat(STDIN_FILENO, &info) == 0 &&
            S_ISREG(info.st_mode) && info.st_size > 0) {
            void* address = ::mmap(nullptr, static_cast<std::size_t>(info.st_size),
                                   PROT_READ, MAP_PRIVATE, STDIN_FILENO, 0);
            if (address != MAP_FAILED) {
                mapped_ = static_cast<const char*>(address);
                mapped_size_ = static_cast<std::size_t>(info.st_size);
                cursor_ = mapped_;
                end_ = mapped_ + mapped_size_;
            }
        }
#endif
    }

    FastInput(const FastInput&) = delete;
    FastInput& operator=(const FastInput&) = delete;

    ~FastInput() {
#if FASTIO_HAS_POSIX
        if (mapped_ != nullptr) {
            ::munmap(const_cast<char*>(mapped_), mapped_size_);
        }
#endif
    }

    template <class T>
        requires(detail::is_fast_integer_v<T>)
    [[gnu::always_inline]] inline bool readInt(T& out) noexcept {
        using U = detail::unsigned_of_t<T>;

        if (!skipSpaces()) return good_ = false;

        bool negative = false;
        if (*cursor_ == '-' || *cursor_ == '+') {
            negative = (*cursor_ == '-');
            ++cursor_;
            if (!refill()) return good_ = false;
        }

        U value = 0;
        bool found_digit = false;

        for (;;) {
            while (cursor_ != end_) {
                const unsigned digit =
                    static_cast<unsigned char>(*cursor_) -
                    static_cast<unsigned>('0');
                if (digit > 9) {
                    ++cursor_;  // consume the token separator
                    if (!found_digit) return good_ = false;
                    assignInteger(out, value, negative);
                    return good_ = true;
                }
                found_digit = true;
                value = static_cast<U>(value * U{10} +
                                       static_cast<U>(digit));
                ++cursor_;
            }

            if (!refill()) {
                if (!found_digit) return good_ = false;
                assignInteger(out, value, negative);
                return good_ = true;
            }
        }
    }

    template <class T>
        requires(std::is_floating_point_v<T>)
    inline bool readFloat(T& out) {
        if (!skipSpaces()) return good_ = false;

        auto parse = [&out](const char* first, const char* last) noexcept {
            // Unlike formatted input, from_chars deliberately rejects a
            // leading '+', so accept it explicitly for contest input.
            if (first != last && *first == '+') ++first;
            const auto result = std::from_chars(first, last, out,
                                                std::chars_format::general);
            return result.ec == std::errc{} && result.ptr == last;
        };

        const char* token_end = cursor_;
        while (token_end != end_ &&
               static_cast<unsigned char>(*token_end) > ' ') {
            ++token_end;
        }

        // Almost every token takes this zero-copy path.
        if (token_end != end_) [[likely]] {
            const char* token_begin = cursor_;
            cursor_ = token_end + 1;
            return good_ = parse(token_begin, token_end);
        }

#if FASTIO_HAS_POSIX
        // An mmap region is the complete input, so reaching its end is EOF.
        if (mapped_ != nullptr) {
            const char* token_begin = cursor_;
            cursor_ = end_;
            return good_ = parse(token_begin, end_);
        }
#endif

        // Rare path: the token straddles a read-buffer boundary.
        std::string token(cursor_, end_);
        cursor_ = end_;
        for (;;) {
            if (!refill()) break;
            token_end = cursor_;
            while (token_end != end_ &&
                   static_cast<unsigned char>(*token_end) > ' ') {
                ++token_end;
            }
            token.append(cursor_, token_end);
            cursor_ = token_end;
            if (cursor_ != end_) {
                ++cursor_;
                break;
            }
        }
        return good_ = parse(token.data(), token.data() + token.size());
    }

    [[gnu::always_inline]] inline bool readChar(char& out) noexcept {
        if (!skipSpaces()) return good_ = false;
        out = *cursor_++;
        return good_ = true;
    }

    [[gnu::always_inline]] inline bool readRawChar(char& out) noexcept {
        if (!refill()) return good_ = false;
        out = *cursor_++;
        return good_ = true;
    }

    inline bool readToken(std::string& out) {
        out.clear();
        if (!skipSpaces()) return good_ = false;

        for (;;) {
            const char* begin = cursor_;
            while (cursor_ != end_ &&
                   static_cast<unsigned char>(*cursor_) > ' ') {
                ++cursor_;
            }
            out.append(begin, cursor_);

            if (cursor_ != end_) {
                ++cursor_;
                return good_ = true;
            }
            if (!refill()) return good_ = true;
        }
    }

    inline bool readLine(std::string& out) {
        out.clear();

        for (;;) {
            if (!refill()) return good_ = !out.empty();
            const char* begin = cursor_;
            while (cursor_ != end_ && *cursor_ != '\n') ++cursor_;
            out.append(begin, cursor_);

            if (cursor_ != end_) {
                ++cursor_;
                if (!out.empty() && out.back() == '\r') out.pop_back();
                return good_ = true;
            }
        }
    }

    template <class T>
        requires(detail::is_fast_integer_v<T>)
    [[gnu::always_inline]] inline FastInput& operator>>(T& out) noexcept {
        readInt(out);
        return *this;
    }

    template <class T>
        requires(std::is_floating_point_v<T>)
    inline FastInput& operator>>(T& out) {
        readFloat(out);
        return *this;
    }

    [[gnu::always_inline]] inline FastInput& operator>>(char& out) noexcept {
        readChar(out);
        return *this;
    }

    inline FastInput& operator>>(std::string& out) {
        readToken(out);
        return *this;
    }

    [[nodiscard]] explicit inline operator bool() const noexcept {
        return good_;
    }

    [[nodiscard]] inline bool good() const noexcept { return good_; }
};

class FastOutput {
    static constexpr std::size_t BUFFER_SIZE = FASTIO_OUTPUT_BUFFER_SIZE;

    alignas(64) char buffer_[BUFFER_SIZE];
    std::size_t position_ = 0;
    bool good_ = true;

    inline bool writeDirect(const char* data, std::size_t size) noexcept {
#if FASTIO_HAS_POSIX
        while (size != 0) {
            ssize_t count;
            do {
                count = ::write(STDOUT_FILENO, data, size);
            } while (count < 0 && errno == EINTR);

            if (count <= 0) return good_ = false;
            data += count;
            size -= static_cast<std::size_t>(count);
        }
        return true;
#else
        while (size != 0) {
            const std::size_t count = std::fwrite(data, 1, size, stdout);
            if (count == 0) return good_ = false;
            data += count;
            size -= count;
        }
        return true;
#endif
    }

public:
    FastOutput() = default;
    FastOutput(const FastOutput&) = delete;
    FastOutput& operator=(const FastOutput&) = delete;

    ~FastOutput() { flush(); }

    inline bool flush() noexcept {
        if (position_ == 0) return good_;
        const bool result = writeDirect(buffer_, position_);
        position_ = 0;
        return result;
    }

    [[gnu::always_inline]] inline void writeChar(char value) noexcept {
        if (position_ == BUFFER_SIZE) [[unlikely]] flush();
        buffer_[position_++] = value;
    }

    inline void writeRaw(const char* data, std::size_t size) noexcept {
        if (size >= BUFFER_SIZE) {
            flush();
            writeDirect(data, size);
            return;
        }

        if (position_ + size > BUFFER_SIZE) flush();
        std::memcpy(buffer_ + position_, data, size);
        position_ += size;
    }

    inline void writeString(std::string_view value) noexcept {
        writeRaw(value.data(), value.size());
    }

    template <class T>
        requires(detail::is_fast_integer_v<T>)
    [[gnu::always_inline]] inline void writeInt(T value) noexcept {
        // Write directly into the large output buffer. Current libstdc++ uses
        // a digit-pair conversion internally and supports GNU __int128 too.
        // Avoiding a temporary buffer + memcpy is a major output-side win.
        if constexpr (BUFFER_SIZE >= 64 &&
                      requires(char* first, char* last, T number) {
                          std::to_chars(first, last, number);
                      }) {
            if (position_ + 64 > BUFFER_SIZE) [[unlikely]] flush();
            const auto result = std::to_chars(buffer_ + position_,
                                              buffer_ + BUFFER_SIZE, value);
            position_ = static_cast<std::size_t>(result.ptr - buffer_);
            return;
        }

        // Fallback for standard libraries without an integer to_chars overload
        // for an extended integer type.
        using U = detail::unsigned_of_t<T>;
        U magnitude;
        bool negative = false;

        if constexpr (detail::is_signed_integer_v<T>) {
            negative = value < 0;
            const U unsigned_value = static_cast<U>(value);
            magnitude = negative ? U{0} - unsigned_value : unsigned_value;
        } else {
            magnitude = value;
        }

        // 128-bit minimum needs 40 bytes including its sign.
        char storage[64];
        char* const finish = storage + sizeof(storage);
        char* cursor = finish;

        while (magnitude >= U{100}) {
            const U quotient = magnitude / U{100};
            const unsigned remainder =
                static_cast<unsigned>(magnitude - quotient * U{100});
            cursor -= 2;
            std::memcpy(cursor, detail::DIGIT_PAIRS + remainder * 2, 2);
            magnitude = quotient;
        }

        if (magnitude < U{10}) {
            *--cursor = static_cast<char>('0' + static_cast<unsigned>(magnitude));
        } else {
            cursor -= 2;
            std::memcpy(cursor,
                        detail::DIGIT_PAIRS + static_cast<unsigned>(magnitude) * 2,
                        2);
        }

        if (negative) *--cursor = '-';
        writeRaw(cursor, static_cast<std::size_t>(finish - cursor));
    }

    template <class T>
        requires(std::is_floating_point_v<T>)
    inline void writeFloat(T value) noexcept {
        char storage[128];
        const auto result = std::to_chars(
            storage, storage + sizeof(storage), value, std::chars_format::general);
        if (result.ec == std::errc{}) {
            writeRaw(storage, static_cast<std::size_t>(result.ptr - storage));
        }
    }

    template <class T>
        requires(std::is_floating_point_v<T>)
    inline void writeFixed(T value, int precision) noexcept {
        char storage[512];
        const auto result = std::to_chars(storage, storage + sizeof(storage), value,
                                          std::chars_format::fixed, precision);
        if (result.ec == std::errc{}) {
            writeRaw(storage, static_cast<std::size_t>(result.ptr - storage));
        }
    }

    [[gnu::always_inline]] inline void writeOne(char value) noexcept {
        writeChar(value);
    }
    [[gnu::always_inline]] inline void writeOne(bool value) noexcept {
        writeChar(value ? '1' : '0');
    }
    inline void writeOne(const char* value) noexcept {
        writeRaw(value, std::strlen(value));
    }
    inline void writeOne(std::string_view value) noexcept { writeString(value); }
    inline void writeOne(const std::string& value) noexcept { writeString(value); }

    template <class T>
        requires(detail::is_fast_integer_v<T>)
    [[gnu::always_inline]] inline void writeOne(T value) noexcept {
        writeInt(value);
    }

    template <class T>
        requires(std::is_floating_point_v<T>)
    inline void writeOne(T value) noexcept {
        writeFloat(value);
    }

    template <class... Ts>
    [[gnu::always_inline]] inline void operator()(const Ts&... values) noexcept {
        (writeOne(values), ...);
    }

    [[gnu::always_inline]] inline FastOutput& operator<<(char value) noexcept {
        writeChar(value);
        return *this;
    }
    [[gnu::always_inline]] inline FastOutput& operator<<(bool value) noexcept {
        writeOne(value);
        return *this;
    }
    inline FastOutput& operator<<(const char* value) noexcept {
        writeOne(value);
        return *this;
    }
    inline FastOutput& operator<<(std::string_view value) noexcept {
        writeString(value);
        return *this;
    }
    inline FastOutput& operator<<(const std::string& value) noexcept {
        writeString(value);
        return *this;
    }

    template <class T>
        requires(detail::is_fast_integer_v<T>)
    [[gnu::always_inline]] inline FastOutput& operator<<(T value) noexcept {
        writeInt(value);
        return *this;
    }

    template <class T>
        requires(std::is_floating_point_v<T>)
    inline FastOutput& operator<<(T value) noexcept {
        writeFloat(value);
        return *this;
    }

    [[nodiscard]] inline bool good() const noexcept { return good_; }
};

// One global input buffer and one global output buffer, allocated in BSS.
inline FastInput in;
inline FastOutput out;

}  // namespace fastio

#ifndef FASTIO_NO_MAIN

#endif
static constexpr int MAXN = 500000 + 5;
static constexpr int MAXM = 500000 + 5;

static int head[MAXN], to_[MAXM], nxt[MAXM];
static unsigned char vis[MAXN];
static int pos_[MAXN];

struct Frame {
    int v, ei, in;
};
static Frame st[MAXN];

int main() {
    int n, m;
    if (!(fastio::in >> n)) return 0;
    fastio::in >> m;

    memset(head, 0xff, n * sizeof(int));
    memset(vis, 0, n);

    for (int i = 0; i < m; ++i) {
        int u, v;
        fastio::in >> u >> v;
        to_[i] = v;
        nxt[i] = head[u];
        head[u] = i;
    }

    int top = 0;
    for (int s = 0; s < n; ++s) {
        if (vis[s]) continue;

        vis[s] = 1;
        pos_[s] = 0;
        st[top++] = {s, head[s], -1};

        while (top) {
            Frame& fr = st[top - 1];

            if (fr.ei == -1) {
                vis[fr.v] = 2;
                --top;
                continue;
            }

            int e = fr.ei;
            fr.ei = nxt[e];
            int w = to_[e];

            if (vis[w] == 0) {
                pos_[w] = top;
                vis[w] = 1;
                st[top++] = {w, head[w], e};
            } else if (vis[w] == 1) {
                int p = pos_[w];

                fastio::out(top - p, '\n');
                for (int i = p + 1; i < top; ++i) {
                    fastio::out(st[i].in, '\n');
                }
                fastio::out(e, '\n');
                return 0;
            }
        }
    }

    fastio::out(-1, '\n');
    return 0;
}