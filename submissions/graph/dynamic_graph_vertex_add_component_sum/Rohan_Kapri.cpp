
#include <algorithm>
#include <bit>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>
namespace canard::hash {
[[nodiscard]] inline std::uint64_t undirected_key(std::uint32_t u, std::uint32_t v) noexcept {
    if (u > v)
        std::swap(u, v);
    return (std::uint64_t{u} << 32) | v;
}
// Full-key comparison; deterministic hashing affects speed, never correctness.
// Back-shift deletion prevents tombstone accumulation. IDs must be nonzero.
class edge_table {
    std::vector<std::uint64_t> keys_;
    std::vector<std::uint32_t> values_;
    std::size_t size_ = 0;
    static std::uint64_t mix(std::uint64_t x) noexcept {
        x ^= x >> 30;
        x *= 0xbf58476d1ce4e5b9ULL;
        x ^= x >> 27;
        x *= 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
    std::size_t bucket(std::uint64_t k) const noexcept {
        return mix(k) & (keys_.size() - 1);
    }
    void rehash(std::size_t capacity) {
        auto keys = std::move(keys_);
        auto values = std::move(values_);
        keys_.assign(capacity, 0);
        values_.assign(capacity, 0);
        size_ = 0;
        for (std::size_t i = 0; i < values.size(); ++i)
            if (values[i])
                insert(keys[i], values[i]);
    }

  public:
    explicit edge_table(std::size_t expected = 0) {
        const auto capacity = std::bit_ceil(std::max<std::size_t>(16, 2 * expected));
        keys_.resize(capacity);
        values_.resize(capacity);
    }
    edge_table(const edge_table&) = default;
    edge_table& operator=(const edge_table&) = default;
    edge_table(edge_table&& other) noexcept
        : keys_(std::move(other.keys_)), values_(std::move(other.values_)),
          size_(std::exchange(other.size_, 0)) {}
    edge_table& operator=(edge_table&& other) noexcept {
        if (this != &other) {
            keys_ = std::move(other.keys_);
            values_ = std::move(other.values_);
            size_ = std::exchange(other.size_, 0);
        }
        return *this;
    }
    [[nodiscard]] std::size_t size() const noexcept {
        return size_;
    }
    [[nodiscard]] std::uint32_t find(std::uint64_t key) const noexcept {
        if (keys_.empty())
            return 0;
        auto i = bucket(key);
        const auto mask = keys_.size() - 1;
        while (values_[i]) {
            if (keys_[i] == key)
                return values_[i];
            i = (i + 1) & mask;
        }
        return 0;
    }
    void insert(std::uint64_t key, std::uint32_t id) {
        assert(id);
        if (keys_.empty())
            rehash(16);
        if ((size_ + 1) * 4 > keys_.size() * 3)
            rehash(keys_.size() * 2);
        auto i = bucket(key);
        const auto mask = keys_.size() - 1;
        while (values_[i]) {
            assert(keys_[i] != key);
            i = (i + 1) & mask;
        }
        keys_[i] = key;
        values_[i] = id;
        ++size_;
    }
    std::uint32_t erase(std::uint64_t key) noexcept {
        if (keys_.empty())
            return 0;
        auto hole = bucket(key);
        const auto mask = keys_.size() - 1;
        while (values_[hole] && keys_[hole] != key)
            hole = (hole + 1) & mask;
        if (!values_[hole])
            return 0;
        const auto id = values_[hole];
        --size_;
        auto next = (hole + 1) & mask;
        while (values_[next]) {
            const auto home = bucket(keys_[next]);
            if (((hole - home) & mask) < ((next - home) & mask)) {
                keys_[hole] = keys_[next];
                values_[hole] = values_[next];
                hole = next;
            }
            next = (next + 1) & mask;
        }
        values_[hole] = 0;
        return id;
    }
};
} // namespace canard::hash
#include <cassert>
#include <cstdint>
#include <span>
#include <vector>
namespace canard::offline {
struct graph_operation {
    std::uint32_t kind, first, second;
};
struct edge_lifetime {
    std::uint32_t first, last, u, v;
};
// Only this offline plan knows future deletions. Reinsertion is a new lifetime.
class component_timeline {
    std::vector<graph_operation> operations_;
    std::vector<edge_lifetime> edges_;
    std::vector<std::uint32_t> death_;

  public:
    explicit component_timeline(std::span<const graph_operation> ops)
        : operations_(ops.begin(), ops.end()), death_(ops.size(), ops.size()) {
        hash::edge_table open(std::min<std::size_t>(ops.size() / 4, 65536));
        edges_.reserve(ops.size() / 2);
        for (std::uint32_t i = 0; i < ops.size(); ++i) {
            const auto op = ops[i];
            if (op.kind > 1)
                continue;
            const auto key = hash::undirected_key(op.first, op.second);
            if (op.kind == 0) {
                assert(op.first != op.second && !open.find(key));
                edges_.push_back({i, static_cast<std::uint32_t>(ops.size()), op.first, op.second});
                open.insert(key, edges_.size());
            } else {
                const auto id = open.erase(key);
                assert(id);
                auto& e = edges_[id - 1];
                e.last = i;
                death_[e.first] = i;
            }
        }
    }
    std::span<const graph_operation> operations() const noexcept {
        return operations_;
    }
    std::span<const edge_lifetime> lifetimes() const noexcept {
        return edges_;
    }
    std::uint32_t death(std::uint32_t insertion) const noexcept {
        return death_[insertion];
    }
};
} // namespace canard::offline
// ===== include/canard/graph/expiry_am_forest.hpp =====
// ===== include/canard/graph/component_group.hpp =====
// ===== include/canard/associated_types.hpp =====
#include <cstddef>

namespace canard {

template <typename T> using coordinate_type_t = typename T::coordinate_type;
template <typename T> using point_type_t = typename T::point_type;
template <typename T> using rectangle_type_t = typename T::rectangle_type;
template <typename T> using partition_type_t = typename T::partition_type;


template <typename T> using aggregate_type_t = typename T::aggregate_type;
template <typename T> using element_type_t = typename T::element_type;
template <typename T> using configuration_type_t = typename T::configuration_type;
template <typename T> using allocator_type_t = typename T::allocator_type;
template <typename T> using size_type_t = typename T::size_type;

// Direct associated-type projections: no cv/ref removal, fallback, or conversion.
// Missing members remain substitution failures in requires-expressions.
template <typename T> using value_type_t = typename T::value_type;

template <typename T> using monoid_type_t = typename T::monoid_type;

template <typename T> using tag_type_t = typename T::tag_type;

template <typename T> using summary_type_t = typename T::summary_type;

template <typename T> using update_type_t = typename T::update_type;

template <typename T> using action_type_t = typename T::action_type;

template <typename T> using leaf_type_t = typename T::leaf_type;

template <typename T> using branch_type_t = typename T::branch_type;

template <typename T> using word_type_t = typename T::word_type;

template <typename T> using representation_type_t = typename T::representation_type;

template <typename T> using execution_type_t = typename T::execution_type;

template <typename T> using delta_type_t = typename T::delta_type;

template <typename T> using prepared_action_t = typename T::prepared_action;

template <typename T> using subtree_change_t = typename T::subtree_change;

template <typename T> using local_change_t = typename T::local_change;

template <typename T> using edge_t = typename T::edge;

template <typename T> using field_t = typename T::field;

template <typename T> using pack_t = typename T::pack;

template <typename T> using fixed_multiplier_t = typename T::fixed_multiplier;

template <typename T> using blended_multiplier_t = typename T::blended_multiplier;

template <typename T, std::size_t Extent> using interval_t = typename T::template interval<Extent>;

template <typename T, std::size_t Count> using edit_type_t = typename T::template edit_type<Count>;

} // namespace canard
#include <concepts>
#include <cstdint>
#include <type_traits>
namespace canard::graph {
// Semantic requirement: a commutative group, with remove(a,b) = a - b.
// Exact integer sums require the caller to keep all arithmetic representable.
template <typename T = std::uint64_t> struct component_sum {
    using value_type = T;
    constexpr T identity() const noexcept {
        return T{};
    }
    constexpr T combine(T a, T b) const noexcept {
        return a + b;
    }
    constexpr T remove(T a, T b) const noexcept {
        return a - b;
    }
};
template <typename G>
concept component_group = std::copy_constructible<G> &&
                          requires(const G& g, const value_type_t<G>& a, const value_type_t<G>& b) {
                              requires std::copyable<value_type_t<G>>;
                              requires std::is_trivially_copyable_v<value_type_t<G>>;
                              requires std::default_initializable<value_type_t<G>>;
                              { g.identity() } noexcept -> std::same_as<value_type_t<G>>;
                              { g.combine(a, b) } noexcept -> std::same_as<value_type_t<G>>;
                              { g.remove(a, b) } noexcept -> std::same_as<value_type_t<G>>;
                          };
} // namespace canard::graph
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <span>
#include <utility>
#include <vector>
namespace canard {
// Maximum-bottleneck counterpart of lazy AM Perch, extended with monotone
// expiry cuts and subtree sums. Transformed edges need not be original edges.
// The incremental AM paper does NOT prove an amortized bound for this extension.
template <graph::component_group Group = graph::component_sum<>> class expiry_am_forest {
  public:
    using value_type = value_type_t<Group>;
    using aggregate_type = value_type;

  private:
    struct node {
        std::uint32_t parent = 0, deadline = 0, size = 1;
        value_type sum{};
    };
    [[no_unique_address]] Group group_;
    std::vector<node> nodes_;
    std::vector<std::uint32_t> owner_;
    std::uint32_t horizon_;
    void unindex(std::uint32_t x) noexcept {
        const auto d = nodes_[x].deadline;
        if (d && d < horizon_) {
            assert(owner_[d] == x);
            owner_[d] = 0;
        }
    }
    void index(std::uint32_t x) noexcept {
        const auto d = nodes_[x].deadline;
        if (d && d < horizon_) {
            assert(!owner_[d]);
            owner_[d] = x;
        }
    }
    void rotate(std::uint32_t x) noexcept {
        auto& a = nodes_[x];
        const auto y = a.parent;
        auto& b = nodes_[y];
        b.size -= a.size;
        b.sum = group_.remove(b.sum, a.sum);
        if (a.deadline <= b.deadline)
            a.parent = b.parent;
        else {
            a.size += b.size;
            a.sum = group_.combine(a.sum, b.sum);
            unindex(x);
            unindex(y);
            std::swap(a.deadline, b.deadline);
            a.parent = b.parent;
            b.parent = x;
            index(x);
            index(y);
        }
    }
    void maintain(std::uint32_t x) noexcept {
        while (nodes_[x].parent) {
            const auto y = nodes_[x].parent;
            if (std::uint64_t{nodes_[x].size} * 3 <= std::uint64_t{nodes_[y].size} * 2)
                x = y;
            else
                rotate(x);
        }
    }
    void reroot(std::uint32_t x, std::uint32_t watch) noexcept {
        while (nodes_[x].parent && nodes_[x].parent != watch)
            rotate(x);
    }

  public:
    explicit expiry_am_forest(std::span<const value_type> values,
                              std::uint32_t horizon,
                              Group group = {})
        : group_(std::move(group)), nodes_(values.size() + 1), owner_(horizon), horizon_(horizon) {
        nodes_[0].size = 0;
        nodes_[0].sum = group_.identity();
        for (std::uint32_t i = 0; i < values.size(); ++i)
            nodes_[i + 1].sum = values[i];
    }
    void insert(std::uint32_t u, std::uint32_t v, std::uint32_t deadline) noexcept {
        ++u;
        ++v;
        assert(u != v && deadline && deadline <= horizon_);
        maintain(u);
        maintain(v);
        reroot(v, 0);
        reroot(u, v);
        if (nodes_[u].parent == v) {
            if (deadline > nodes_[u].deadline) {
                unindex(u);
                nodes_[u].deadline = deadline;
                index(u);
            }
        } else {
            assert(!nodes_[u].parent);
            if (nodes_[u].size > nodes_[v].size)
                std::swap(u, v);
            nodes_[u].parent = v;
            nodes_[u].deadline = deadline;
            index(u);
            nodes_[v].size += nodes_[u].size;
            nodes_[v].sum = group_.combine(nodes_[v].sum, nodes_[u].sum);
        }
    }
    void expire(std::uint32_t deadline) noexcept {
        assert(deadline < horizon_);
        const auto x = owner_[deadline];
        if (!x)
            return;
        const auto count = nodes_[x].size;
        const auto sum = nodes_[x].sum;
        for (auto p = nodes_[x].parent; p; p = nodes_[p].parent) {
            nodes_[p].size -= count;
            nodes_[p].sum = group_.remove(nodes_[p].sum, sum);
        }
        nodes_[x].parent = 0;
        nodes_[x].deadline = 0;
        owner_[deadline] = 0;
    }
    void add(std::uint32_t v, const value_type& delta) noexcept {
        ++v;
        maintain(v);
        for (; v; v = nodes_[v].parent)
            nodes_[v].sum = group_.combine(nodes_[v].sum, delta);
    }
    aggregate_type fold(std::uint32_t v) noexcept {
        ++v;
        maintain(v);
        while (nodes_[v].parent)
            v = nodes_[v].parent;
        return nodes_[v].sum;
    }
};
} // namespace canard
namespace canard::offline {
template <graph::component_group Group = graph::component_sum<>>
std::vector<value_type_t<Group>> expiry_component_sum(std::span<const value_type_t<Group>> initial,
                                                      const component_timeline& plan,
                                                      Group group = {}) {
    const auto ops = plan.operations();
    expiry_am_forest<Group> forest(initial, ops.size(), group);
    std::vector<value_type_t<Group>> answers;
    answers.reserve(ops.size() / 4);
    for (std::uint32_t t = 0; t < ops.size(); ++t) {
        const auto op = ops[t];
        if (op.kind == 0)
            forest.insert(op.first, op.second, plan.death(t));
        else if (op.kind == 1)
            forest.expire(t);
        else if (op.kind == 2)
            forest.add(op.first, static_cast<value_type_t<Group>>(op.second));
        else
            answers.push_back(forest.fold(op.first));
    }
    return answers;
}
} // namespace canard::offline
// ===== include/canard/io/buffered_writer.hpp =====
// ===== include/canard/io/protocols.hpp =====
// ===== include/canard/io/decimal/field.hpp =====
namespace canard::io::decimal {
struct field {
    const char* data;
    unsigned digits;
};
}

namespace canard::io::decimal {
struct u64_field {
    const char* source;
    unsigned digits;
};
}
#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>
namespace canard::io {
template <typename C>
concept unsigned64_codec = requires(const char* bytes, decimal::u64_field field) {
    requires std::same_as<value_type_t<C>, std::uint64_t>;
    { C::delimiters32(bytes) } noexcept -> std::same_as<std::uint32_t>;
    { C::decode(field) } noexcept -> std::same_as<std::uint64_t>;
    { C::decode_pair(field, field) } noexcept -> std::same_as<std::array<std::uint64_t, 2>>;
};
template <typename F, typename Word = value_type_t<F>>
concept integer_formatter = std::same_as<Word, value_type_t<F>> && requires(const F& f, char* p, Word x) {
    typename std::integral_constant<std::size_t, F::token_capacity>;
    requires (F::token_capacity > 0);
    { f(p, x) } noexcept -> std::same_as<char*>;
};
template <typename F, typename Word = value_type_t<F>>
concept paired_integer_formatter = integer_formatter<F, Word> && requires(const F& f, char* p, Word x) {
    { f.format_pair(p, x, x, char{}) } noexcept -> std::same_as<char*>;
};
} // namespace canard::io
// ===== include/canard/io/decimal_format.hpp =====
#include <array>
#include <bit>
#include <charconv>
#include <cstdint>
#include <cstring>
#include <limits>
namespace canard::io::decimal {
inline constexpr auto four_digits = [] consteval {
    std::array<std::array<char, 4>, 10'000> result{};
    for (unsigned value = 0; value < result.size(); ++value) {
        result[value] = {static_cast<char>('0' + value / 1000),
                         static_cast<char>('0' + value / 100 % 10),
                         static_cast<char>('0' + value / 10 % 10),
                         static_cast<char>('0' + value % 10)};
    }
    return result;
}();

// Writes canonical decimal plus newline; at least 16 writable bytes supplied.
// Precondition: value <= MaxValue. Default handles all uint32 values;
// a nine-digit bound preserves the original solver's one-byte leading group.
template <std::uint32_t MaxValue = std::numeric_limits<std::uint32_t>::max()>
[[nodiscard]] inline char* format_u32_line(char* cursor, char* end, std::uint32_t value) noexcept {
    // Most answers have nine digits. Emit their eight low digits with
    // two fixed-size copies; use to_chars only for a short leading group.
    auto append_four = [&cursor](unsigned group) noexcept {
        std::memcpy(cursor, four_digits[group].data(), 4);
        cursor += 4;
    };
    if (value >= 100'000'000) {
        unsigned high = value / 100'000'000;
        value %= 100'000'000;
        if constexpr (MaxValue < 1'000'000'000) {
            *cursor++ = static_cast<char>('0' + high);
        } else {
            cursor = std::to_chars(cursor, end, high).ptr;
        }
        append_four(value / 10'000);
        append_four(value % 10'000);
    } else if (value >= 10'000) {
        cursor = std::to_chars(cursor, end, value / 10'000).ptr;
        append_four(value % 10'000);
    } else {
        cursor = std::to_chars(cursor, end, value).ptr;
    }
    *cursor++ = '\n';
    return cursor;
}
} // namespace canard::io::decimal

namespace canard::io::decimal {
// At least 24 writable bytes. Canonical uint64_t decimal, no delimiter.
// Reuses the shared base-10^4 table; first-group trimming uses one word copy.
[[nodiscard]] inline char* format_u64_token(char* cursor, std::uint64_t value) noexcept {
    if constexpr (std::endian::native != std::endian::little)
        return std::to_chars(cursor, cursor + 24, value).ptr;
    auto group = [&cursor](unsigned x) {
        std::memcpy(cursor, four_digits[x].data(), 4);
        cursor += 4;
    };
    auto first = [&cursor](unsigned x) {
        const unsigned skip = 3 - (x >= 10) - (x >= 100) - (x >= 1000);
        std::uint32_t word;
        std::memcpy(&word, four_digits[x].data(), 4);
        word >>= 8 * skip;
        std::memcpy(cursor, &word, 4);
        cursor += 4 - skip;
    };
    if (value >= 10'000'000'000'000'000ULL) {
        const std::uint64_t a = value / 10'000, b = value / 100'000'000,
                            c = value / 1'000'000'000'000ULL, d = value / 10'000'000'000'000'000ULL;
        first(d);
        group(c - d * 10'000);
        group(b - c * 10'000);
        group(a - b * 10'000);
        group(value - a * 10'000);
    } else if (value >= 1'000'000'000'000ULL) {
        const std::uint64_t a = value / 10'000, b = value / 100'000'000,
                            c = value / 1'000'000'000'000ULL;
        first(c);
        group(b - c * 10'000);
        group(a - b * 10'000);
        group(value - a * 10'000);
    } else if (value >= 100'000'000) {
        const std::uint64_t a = value / 10'000, b = value / 100'000'000;
        first(b);
        group(a - b * 10'000);
        group(value - a * 10'000);
    } else if (value >= 10'000) {
        const std::uint64_t a = value / 10'000;
        first(a);
        group(value - a * 10'000);
    } else
        first(value);
    return cursor;
}

// Canonical decimal token, no delimiter. At least 16 writable bytes are required.
// The first group suppresses leading zeros; all following groups have four digits.
// Like the existing formatter, writes may touch unused bytes after the token.
[[nodiscard]] inline char* format_u32_compact(char* cursor, std::uint32_t value) noexcept {
    auto first = [&](std::uint32_t x) {
        if (x < 10) {
            *cursor++ = static_cast<char>('0' + x);
            return;
        }
        const unsigned skip = 3 - unsigned(x >= 10) - unsigned(x >= 100) - unsigned(x >= 1000);
        if constexpr (std::endian::native == std::endian::little) {
            std::uint32_t word;
            std::memcpy(&word, four_digits[x].data(), 4);
            word >>= 8 * skip;
            std::memcpy(cursor, &word, 4);
        } else {
            std::memcpy(cursor, four_digits[x].data() + skip, 4 - skip);
        }
        cursor += 4 - skip;
    };
    auto next = [&](std::uint32_t x) {
        std::memcpy(cursor, four_digits[x].data(), 4);
        cursor += 4;
    };
    if (value >= 100000000u) {
        first(value / 100000000u);
        next(value / 10000u % 10000u);
        next(value % 10000u);
    } else if (value >= 10000u) {
        first(value / 10000u);
        next(value % 10000u);
    } else
        first(value);
    return cursor;
}
} // namespace canard::io::decimal
#include <cstddef>
#include <concepts>
#include <type_traits>
#include <memory>
#include <span>
// ===== include/canard/io/output_buffer.hpp =====
// ===== include/canard/io/transfer.hpp =====
#include <cstddef>
#include <system_error>
namespace canard::io {
struct transfer_result {
    std::size_t count = 0;
    std::error_code error{};
};
}
#include <concepts>
#include <cstring>
#include <memory>
#include <span>
#include <utility>
namespace canard::io {
template <typename S>
concept byte_sink = (requires { requires S::assume_complete_writes; } &&
    requires(S& sink, const char* data, std::size_t n) { { sink.write_all(data, n) } noexcept; }) ||
    requires(S& sink, const char* data, std::size_t n) {
        { sink.write_some(data, n) } noexcept -> std::same_as<transfer_result>;
    };
// Shared byte ownership only. Decimal syntax and ISA are not part of this type.
// Explicit flush/finish reports errors; an error retains the unwritten suffix.
// Destruction attempts flush but cannot report errors: callers needing delivery
// confirmation must call finish() while the object is still alive.
template <byte_sink Sink, std::size_t Capacity = (1u << 16)>
class output_buffer {
  protected:
    static_assert(Capacity >= 64);
    static constexpr std::size_t capacity = Capacity;
    std::unique_ptr<char[]> storage_ = std::make_unique_for_overwrite<char[]>(Capacity);
    char* cursor_ = storage_.get();
    [[no_unique_address]] Sink sink_;
  public:
    explicit output_buffer(Sink sink = Sink{}) : sink_(std::move(sink)) {}
    output_buffer(const output_buffer&) = delete;
    output_buffer& operator=(const output_buffer&) = delete;
    ~output_buffer() noexcept { try { flush(); } catch (...) {} }
    void flush() {
        const auto size = static_cast<std::size_t>(cursor_ - storage_.get());
        if constexpr (requires { requires Sink::assume_complete_writes; }) {
            sink_.write_all(storage_.get(), size);
            cursor_ = storage_.get();
        } else {
            std::size_t sent = 0;
            while (sent != size) {
                const auto result = sink_.write_some(storage_.get() + sent, size - sent);
                // A sink must never report consuming bytes outside its input.
                if (result.count > size - sent)
                    std::terminate();
                sent += result.count;
                auto error = result.error;
                if (!error && result.count == 0)
                    error = std::make_error_code(std::errc::io_error);
                if (error) {
                    std::memmove(storage_.get(), storage_.get() + sent, size - sent);
                    cursor_ = storage_.get() + size - sent;
                    throw std::system_error(error, "canard output");
                }
            }
            cursor_ = storage_.get();
        }
    }
    void finish() {
        flush();
        if constexpr (requires { sink_.finish(); }) {
            if (auto error = sink_.finish())
                throw std::system_error(error, "canard output finish");
        }
    }
    [[nodiscard]] std::span<const char> pending_bytes() const & noexcept {
        return {storage_.get(), static_cast<std::size_t>(cursor_ - storage_.get())};
    }
    void pending_bytes() const && = delete;
    void write_bytes(std::span<const char> bytes) {
        while (!bytes.empty()) {
            auto room = static_cast<std::size_t>(storage_.get() + capacity - cursor_);
            if (room == 0) {
                flush();
                room = capacity;
            }
            const auto count = bytes.size() < room ? bytes.size() : room;
            std::memcpy(cursor_, bytes.data(), count);
            cursor_ += count;
            bytes = bytes.subspan(count);
        }
    }
};
}
// ===== include/canard/io/sink/stdio.hpp =====
#include <cerrno>
#include <cstdio>
#include <stdexcept>
namespace canard::io {
class stdio_sink {
    std::FILE* file_;
  public:
    static constexpr bool assume_complete_writes = false;
    explicit stdio_sink(std::FILE* file = stdout) : file_(file) {
        if (!file)
            throw std::invalid_argument("canard: null FILE handle");
    }
    transfer_result write_some(const char* data, std::size_t size) noexcept {
        for (;;) {
            errno = 0;
            const auto count = std::fwrite(data, 1, size, file_);
            if (!std::ferror(file_)) return {count, {}};
            const auto error = errno ? std::error_code(errno, std::generic_category())
                                     : std::make_error_code(std::errc::io_error);
            if (error == std::errc::interrupted) {
                std::clearerr(file_);
                if (count != 0) return {count, {}};
                continue;
            }
            return {count, error};
        }
    }
    std::error_code finish() noexcept {
        for (;;) {
            errno = 0;
            if (std::fflush(file_) == 0) return {};
            const auto error = errno ? std::error_code(errno, std::generic_category())
                                     : std::make_error_code(std::errc::io_error);
            if (error != std::errc::interrupted)
                return error;
            std::clearerr(file_);
        }
    }
};
}
namespace canard::io {
// Numeric formatting over the shared byte buffer. The default stdio sink is
// checked; trusted POSIX transport is an explicit Sink policy. Call finish()
// to observe transport failures; destructor flush is best effort.
template <std::uint32_t MaxValue = std::numeric_limits<std::uint32_t>::max(), byte_sink Sink = stdio_sink>
class buffered_writer : private output_buffer<Sink> {
    using base = output_buffer<Sink>;
    using base::capacity;
    using base::storage_;
    using base::cursor_;
  public:
    explicit buffered_writer(Sink sink = Sink{}) : base(std::move(sink)) {}
    using base::flush;
    using base::finish;
    using base::pending_bytes;
    using base::write_bytes;
    // Canonical unsigned pair plus newline. Shares the existing buffer and
    // descriptor lifetime; no problem-specific I/O stack.
    void write_pair(std::uint64_t first, std::uint64_t second) {
        char* const end = storage_.get() + capacity;
        if (end - cursor_ < 48)
            flush();
        cursor_ = decimal::format_u64_token(cursor_, first);
        *cursor_++ = ' ';
        cursor_ = decimal::format_u64_token(cursor_, second);
        *cursor_++ = '\n';
    }

    // Fixed-size decimal batches reserve buffer space once. A conversion policy
    // supplies its maximum token footprint and returns the logical token end;
    // this owner still controls the byte buffer and selected transport.
    template <typename Word, std::size_t Extent, typename Formatter>
        requires integer_formatter<Formatter, Word>
    void write_batch(std::span<const Word, Extent> values, const Formatter& formatter) {
        constexpr std::size_t stride = Formatter::token_capacity + 1;
        static_assert(Formatter::token_capacity > 0 && Formatter::token_capacity < capacity);
        if constexpr (Extent != std::dynamic_extent && Extent <= capacity / stride) {
            if (storage_.get() + capacity - cursor_ < static_cast<std::ptrdiff_t>(stride * Extent))
                flush();
            char* output = cursor_;
            if constexpr (paired_integer_formatter<Formatter, Word>) {
                std::size_t i = 0;
                for (; i + 1 < values.size(); i += 2) {
                    output = formatter.format_pair(output, values[i], values[i + 1], char(10));
                    *output++ = char(10);
                }
                if (i < values.size()) {
                    output = formatter(output, values[i]);
                    *output++ = char(10);
                }
            } else {
                for (const auto value : values) {
                    output = formatter(output, value);
                    *output++ = char(10);
                }
            }
            cursor_ = output;
        } else {
            for (const auto value : values) {
                if (storage_.get() + capacity - cursor_ < static_cast<std::ptrdiff_t>(stride))
                    flush();
                cursor_ = formatter(cursor_, value);
                *cursor_++ = char(10);
            }
        }
    }

    void write_u64(std::uint64_t value) {
        if (storage_.get() + capacity - cursor_ < 24)
            flush();
        cursor_ = decimal::format_u64_token(cursor_, value);
        *cursor_++ = '\n';
    }

    // One newline per value, in input order. A small fixed-size batch reserves
    // buffer space once and commits the cursor once; larger/dynamic spans use
    // the same bounded scalar entry point. At most 16 bytes are touched per value.
    template <std::size_t Extent>
    void write_compact_batch(std::span<const std::uint32_t, Extent> values) {
        if constexpr (Extent != std::dynamic_extent && Extent <= capacity / 16) {
            if (storage_.get() + capacity - cursor_ < static_cast<std::ptrdiff_t>(16 * Extent))
                flush();
            char* next = cursor_;
            for (const auto value : values) {
                next = decimal::format_u32_compact(next, value);
                *next++ = '\n';
            }
            cursor_ = next;
        } else {
            for (const auto value : values)
                write_compact_u32(value);
        }
    }

    // Canonical uint32 decimal through the shared base-10^4 table.
    void write_compact_u32(std::uint32_t value) {
        if (storage_.get() + capacity - cursor_ < 16)
            flush();
        cursor_ = decimal::format_u32_compact(cursor_, value);
        *cursor_++ = '\n';
    }

    void write(std::uint32_t value) {
        char* const end = storage_.get() + capacity;
        if (end - cursor_ < 16) {
            flush();
        }
        cursor_ = decimal::format_u32_line<MaxValue>(cursor_, end, value);
    }
};

} // namespace canard::io
// ===== include/canard/io/padded_file.hpp =====
// Compatibility include: trusted Linux whole-file mapping, no pipe fallback.
// ===== include/canard/io/source/linux_mapped_file.hpp =====
// ===== include/canard/io/padded_bytes_view.hpp =====
#include <cstddef>
namespace canard::io {
// Borrowed trusted storage. data[-8..size+31] must be readable. Padding is not
// automatically promised by string_view/span. The owner outlives every reader.
struct padded_bytes_view {
    const char* data;
    std::size_t size;
    static constexpr std::size_t prefix_padding = 8;
    static constexpr std::size_t suffix_padding = 32;
};
}
#if !defined(__linux__)
#error "canard Linux mapping requires Linux; use buffered_file_source on other platforms."
#endif
#include <cstddef>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
namespace canard::io {
// Trusted Linux regular file, positioned at its start; no pipe fallback or
// error checking. The descriptor remains caller-owned. Linux 4 KiB pages.
class padded_file {
    void* mapping_ = MAP_FAILED;
    std::size_t mapping_size_ = 0;
    std::size_t bytes_ = 0;
    const char* data_ = nullptr;

  public:
    explicit padded_file(int descriptor = STDIN_FILENO) noexcept {
        struct stat status;
        ::fstat(descriptor, &status);
        bytes_ = static_cast<std::size_t>(status.st_size);
        constexpr std::size_t page = 4096;
        mapping_size_ = ((bytes_ + page - 1) & ~(page - 1)) + 2 * page;
        mapping_ = ::mmap(nullptr, mapping_size_, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        ::mmap(static_cast<char*>(mapping_) + page,
               bytes_,
               PROT_READ,
               MAP_PRIVATE | MAP_FIXED,
               descriptor,
               0);
        data_ = static_cast<const char*>(mapping_) + page;
    }
    padded_file(const padded_file&) = delete;
    padded_file& operator=(const padded_file&) = delete;
    ~padded_file() {
        ::munmap(mapping_, mapping_size_);
    }
    [[nodiscard]] padded_bytes_view view() const & noexcept {
        return {data_, bytes_};
    }
    void view() const && = delete;
};
} // namespace canard::io
// ===== include/canard/io/avx2/trusted_ascii_reader.hpp =====
// ===== include/canard/io/decimal_avx2.hpp =====
// ===== include/canard/config/compiler.hpp =====
#if defined(__GNUC__) || defined(__clang__)
# define CANARD_ALWAYS_INLINE [[gnu::always_inline]]
# define CANARD_NOINLINE [[gnu::noinline]]
#elif defined(_MSC_VER)
# define CANARD_ALWAYS_INLINE __forceinline
# define CANARD_NOINLINE __declspec(noinline)
#else
# define CANARD_ALWAYS_INLINE
# define CANARD_NOINLINE
#endif
namespace canard::detail {
template <int ReadWrite = 0, int Locality = 3>
inline void prefetch(const void* address) noexcept {
#if defined(__GNUC__) || defined(__clang__)
    __builtin_prefetch(address, ReadWrite, Locality);
#else
    (void)address;
#endif
}
}
#include <array>
#include <bit>
#include <cstdint>
#include <cstring>
#include <immintrin.h>
#ifndef __AVX2__
#error "The packed decimal backend requires AVX2."
#endif
namespace canard::io::decimal {
static_assert(std::endian::native == std::endian::little);

// Borrowing field in padded ASCII storage. digits is 1..10, and the eight
// bytes preceding data+digits must be readable, even for a short field.


namespace detail {
inline constexpr int pair_weights = 0x010a;      // [10, 1]
inline constexpr int quad_weights = 0x00010064;  // [100, 1]
inline constexpr int octet_weights = 0x00012710; // [10000, 1]
inline constexpr auto nibble_mask = [] consteval {
    std::array<std::uint64_t, 11> masks{};
    for (unsigned digits = 1; digits <= 10; ++digits)
        masks[digits] = 0x0f0f'0f0f'0f0f'0f0full << (digits < 8 ? (8 - digits) * 8 : 0);
    return masks;
}();
inline constexpr std::array<unsigned, 10> ninth_digit_weight{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 100'000'000};
[[nodiscard]] inline std::uint64_t eight_byte_tail(field input) noexcept {
    std::uint64_t bytes;
    std::memcpy(&bytes, input.data + input.digits - 8, 8);
    return bytes;
}
[[nodiscard]] inline std::uint32_t ninth_digit(field input) noexcept {
    return static_cast<unsigned>(input.data[0] - '0') * ninth_digit_weight[input.digits];
}
template <unsigned MaxDigits>
[[nodiscard]] inline std::uint32_t leading_digits(field input) noexcept {
    if constexpr (MaxDigits <= 8)
        return 0;
    else if constexpr (MaxDigits == 9)
        return ninth_digit(input);
    else {
        if (input.digits <= 8)
            return 0;
        unsigned leading = static_cast<unsigned>(input.data[0] - '0');
        if (input.digits == 10)
            leading = leading * 10 + static_cast<unsigned>(input.data[1] - '0');
        return leading * 100'000'000;
    }
}
} // namespace detail

[[nodiscard]] inline std::uint32_t decode_one(field input) noexcept {
    auto digits = detail::eight_byte_tail(input) & detail::nibble_mask[input.digits];
    digits = digits * 10 + (digits >> 8);
    const auto eight = static_cast<std::uint32_t>(
        ((digits & 0x0000'00ff'0000'00ffull) * 0x000f'4240'0000'0064ull +
         ((digits >> 16) & 0x0000'00ff'0000'00ffull) * 0x0000'2710'0000'0001ull) >>
        32);
    return eight + detail::leading_digits<10>(input);
}

// ASCII -> digit pairs -> four-digit groups -> eight-digit groups.
// Four-digit groups are <=9999, so packing does not saturate and the final
// signed 16-bit multiply-add is exact. A ninth digit is added separately.
template <unsigned FirstMaxDigits = 9, unsigned SecondMaxDigits = 9>
[[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 2>
decode_pair(field first, field second) noexcept {
    static_assert(1 <= FirstMaxDigits && FirstMaxDigits <= 10);
    static_assert(1 <= SecondMaxDigits && SecondMaxDigits <= 10);
    using namespace detail;
    auto digits = _mm_set_epi64x(std::bit_cast<long long>(eight_byte_tail(second)),
                                 std::bit_cast<long long>(eight_byte_tail(first)));
    digits = _mm_and_si128(digits,
                           _mm_set_epi64x(nibble_mask[second.digits], nibble_mask[first.digits]));
    const auto pairs = _mm_maddubs_epi16(digits, _mm_set1_epi16(pair_weights));
    const auto quads = _mm_madd_epi16(pairs, _mm_set1_epi32(quad_weights));
    const auto compact = _mm_packus_epi32(quads, quads);
    const auto eights = _mm_madd_epi16(compact, _mm_set1_epi32(octet_weights));
    const auto leading_first = leading_digits<FirstMaxDigits>(first);
    const auto leading_second = leading_digits<SecondMaxDigits>(second);
    return {static_cast<std::uint32_t>(_mm_cvtsi128_si32(eights)) + leading_first,
            static_cast<std::uint32_t>(_mm_extract_epi32(eights, 1)) + leading_second};
}

template <unsigned FirstMaxDigits, unsigned SecondMaxDigits, unsigned ThirdMaxDigits>
[[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 3>
decode_triplet(field first, field second, field third) noexcept;

// FirstMaxDigits is a precondition, not a run-time validator. A known-small
// first field (e.g. an index) skips its ninth-digit calculation at compile time.
template <unsigned FirstMaxDigits = 9>
[[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 3>
decode_three(field first, field second, field third) noexcept {
    return decode_triplet<FirstMaxDigits, 9, 9>(first, second, third);
}

// The next 32 bytes must be readable and contain all requested delimiters.
// Only ASCII decimal input is supported, so signed-byte comparison suffices.
[[nodiscard]] inline std::uint32_t delimiter_mask(const char* data) noexcept {
    return static_cast<std::uint32_t>(_mm256_movemask_epi8(_mm256_cmpgt_epi8(
        _mm256_set1_epi8('0'), _mm256_loadu_si256(reinterpret_cast<const __m256i*>(data)))));
}
} // namespace canard::io::decimal

namespace canard::io::decimal {
// Three bounded unsigned fields; total ASCII record (with delimiters) <=32
// bytes. Unlike decode_three's historical interface, every bound is explicit.
template <unsigned FirstMaxDigits = 9, unsigned SecondMaxDigits = 9, unsigned ThirdMaxDigits = 9>
[[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 3>
decode_triplet(field first, field second, field third) noexcept {
    static_assert(1 <= FirstMaxDigits && FirstMaxDigits <= 10);
    static_assert(1 <= SecondMaxDigits && SecondMaxDigits <= 10);
    static_assert(1 <= ThirdMaxDigits && ThirdMaxDigits <= 10);
    using namespace detail;
    auto digits = _mm256_setr_epi64x(std::bit_cast<long long>(eight_byte_tail(first)),
                                     std::bit_cast<long long>(eight_byte_tail(second)),
                                     std::bit_cast<long long>(eight_byte_tail(third)),
                                     0);
    digits = _mm256_and_si256(
        digits,
        _mm256_setr_epi64x(
            nibble_mask[first.digits], nibble_mask[second.digits], nibble_mask[third.digits], 0));
    const auto pairs = _mm256_maddubs_epi16(digits, _mm256_set1_epi16(pair_weights));
    const auto quads = _mm256_madd_epi16(pairs, _mm256_set1_epi32(quad_weights));
    const auto compact = _mm256_packus_epi32(quads, quads);
    const auto eights = _mm256_madd_epi16(compact, _mm256_set1_epi32(octet_weights));
    return {static_cast<std::uint32_t>(_mm256_extract_epi32(eights, 0)) +
                leading_digits<FirstMaxDigits>(first),
            static_cast<std::uint32_t>(_mm256_extract_epi32(eights, 1)) +
                leading_digits<SecondMaxDigits>(second),
            static_cast<std::uint32_t>(_mm256_extract_epi32(eights, 4)) +
                leading_digits<ThirdMaxDigits>(third)};
}

// Four fields of 1..7 decimal digits. Every eight-byte tail must be readable.
// The length/decimal alphabet are trusted preconditions, not runtime checks.
[[nodiscard]] inline std::array<std::uint32_t, 4>
decode_four_short(field a, field b, field c, field d) noexcept {
    using namespace detail;
    auto x = _mm256_setr_epi64x(std::bit_cast<long long>(eight_byte_tail(a)),
                                std::bit_cast<long long>(eight_byte_tail(b)),
                                std::bit_cast<long long>(eight_byte_tail(c)),
                                std::bit_cast<long long>(eight_byte_tail(d)));
    x = _mm256_and_si256(x,
                         _mm256_setr_epi64x(nibble_mask[a.digits],
                                            nibble_mask[b.digits],
                                            nibble_mask[c.digits],
                                            nibble_mask[d.digits]));
    x = _mm256_maddubs_epi16(x, _mm256_set1_epi16(pair_weights));
    x = _mm256_madd_epi16(x, _mm256_set1_epi32(quad_weights));
    x = _mm256_packus_epi32(x, x);
    x = _mm256_madd_epi16(x, _mm256_set1_epi32(octet_weights));
    // AVX2 packing is local to each 128-bit half: fields occupy lanes 0,1,4,5.
    return {static_cast<std::uint32_t>(_mm256_extract_epi32(x, 0)),
            static_cast<std::uint32_t>(_mm256_extract_epi32(x, 1)),
            static_cast<std::uint32_t>(_mm256_extract_epi32(x, 4)),
            static_cast<std::uint32_t>(_mm256_extract_epi32(x, 5))};
}
} // namespace canard::io::decimal
// ===== include/canard/io/detail/basic_trusted_ascii_reader.hpp =====
#include <array>
#include <bit>
#include <cstring>
#include <cstdint>
namespace canard::io {
// Borrowing reader over padded bytes; lifetime is controlled by the caller.
// Packed records require exactly one delimiter between fields, <=32 bytes
// including the terminating delimiter, and the stated maximum digit counts.
// read_tagged_triplet(): one-digit tag, then three fields. read_pair(): two
// 1..9-digit fields. read_u32() also accepts repeated leading whitespace.
template <typename Codec>
class basic_trusted_ascii_reader {
    const char* cursor_;
    void skip_whitespace() {
        while (static_cast<unsigned char>(*cursor_) <= ' ') {
            ++cursor_;
        }
    }

    template <typename Word> [[nodiscard]] static constexpr bool all_digits(Word word) noexcept {
        constexpr Word zeros =
            sizeof(Word) == 8 ? static_cast<Word>(0x3030'3030'3030'3030ull) : Word{0x3030'3030u};
        constexpr Word high =
            sizeof(Word) == 8 ? static_cast<Word>(0xf0f0'f0f0'f0f0'f0f0ull) : Word{0xf0f0'f0f0u};
        constexpr Word sixes =
            sizeof(Word) == 8 ? static_cast<Word>(0x0606'0606'0606'0606ull) : Word{0x0606'0606u};
        return (word & high) == zeros && ((word + sixes) & high) == zeros;
    }

    template <typename Word> [[nodiscard]] std::uint32_t read_unsigned() {
        if constexpr (std::endian::native != std::endian::little) {
            skip_whitespace();
            std::uint32_t value = 0;
            while (*cursor_ >= '0' && *cursor_ <= '9')
                value = value * 10 + (*cursor_++ - '0');
            return value;
        }
        skip_whitespace();
        Word word;
        std::memcpy(&word, cursor_, sizeof(word));
        std::uint32_t result = 0;
        if (all_digits(word)) {
            if constexpr (sizeof(Word) == 8) {
                word ^= 0x3030'3030'3030'3030ull;
                word = (word * 10 + (word >> 8)) & 0x00ff'00ff'00ff'00ffull;
                word = (word * 100 + (word >> 16)) & 0x0000'ffff'0000'ffffull;
                word = (word * 10'000 + (word >> 32)) & 0x0000'0000'ffff'ffffull;
            } else {
                word ^= 0x3030'3030u;
                word = (word * 10 + (word >> 8)) & 0x00ff'00ffu;
                word = (word * 100 + (word >> 16)) & 0x0000'ffffu;
            }
            result = static_cast<std::uint32_t>(word);
            cursor_ += sizeof(word);
        }
        // Valid judge input contains only values fitting in uint32_t.
        for (unsigned digit; (digit = static_cast<unsigned>(*cursor_ - '0')) < 10; ++cursor_) {
            result = result * 10 + digit;
        }
        return result;
    }

  public:
    // Two pairs of 1..MaxDigits unsigned fields, each followed by one delimiter.
    // The complete four-field record fits in 32 readable bytes; MaxDigits <= 7.
    template <unsigned MaxDigits = 6>
    [[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 4>
    read_two_index_pairs() noexcept {
        static_assert(MaxDigits >= 1 && MaxDigits <= 7);
        skip_whitespace();
        auto mask = Codec::delimiter_mask(cursor_);
        const unsigned p0 = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned p1 = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned p2 = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned p3 = std::countr_zero(mask);
        const auto result = Codec::decode_four_short({cursor_, p0},
                                                       {cursor_ + p0 + 1, p1 - p0 - 1},
                                                       {cursor_ + p1 + 1, p2 - p1 - 1},
                                                       {cursor_ + p2 + 1, p3 - p2 - 1});
        cursor_ += p3 + 1;
        return result;
    }

    explicit basic_trusted_ascii_reader(padded_bytes_view input) noexcept : cursor_(input.data) {}
    // One codec delimiter mask per record; decimal reductions use packed
    // byte-to-pair, pair-to-quad, and quad-to-eight-digit multiply-adds.
    // A ninth leading digit is added separately. Field lengths are trusted.
    template <unsigned FirstMaxDigits = 9, unsigned SecondMaxDigits = 9>
    [[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 2> read_pair() {
        skip_whitespace();
        auto mask = Codec::delimiter_mask(cursor_);
        const unsigned first = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned second = std::countr_zero(mask);
        const auto [a, b] = Codec::template decode_pair<FirstMaxDigits, SecondMaxDigits>(
            {cursor_, first}, {cursor_ + first + 1, second - first - 1});
        cursor_ += second;
        ++cursor_;
        return {a, b};
    }
    template <unsigned FirstMaxDigits = 9,
              unsigned SecondMaxDigits = 9,
              unsigned ThirdMaxDigits = 9>
    [[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 3> read_triplet() {
        skip_whitespace();
        auto mask = Codec::delimiter_mask(cursor_);
        const unsigned first = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned second = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned third = std::countr_zero(mask);
        const auto result =
            Codec::template decode_triplet<FirstMaxDigits, SecondMaxDigits, ThirdMaxDigits>(
                {cursor_, first},
                {cursor_ + first + 1, second - first - 1},
                {cursor_ + second + 1, third - second - 1});
        cursor_ += third + 1;
        return result;
    }

    // Exactly one one-digit tag and two bounded decimal fields.
    template <unsigned FirstMaxDigits = 9, unsigned SecondMaxDigits = 9>
    [[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 3>
    read_tagged_pair() noexcept {
        skip_whitespace();
        const unsigned type = static_cast<unsigned>(cursor_[0] - '0');
        cursor_ += 2;
        auto mask = Codec::delimiter_mask(cursor_);
        const unsigned first = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned second = std::countr_zero(mask);
        const auto [a, b] = Codec::template decode_pair<FirstMaxDigits, SecondMaxDigits>(
            {cursor_, first}, {cursor_ + first + 1, second - first - 1});
        cursor_ += second + 1;
        return {type, a, b};
    }

    template <unsigned FirstMaxDigits = 9>
    [[nodiscard]] CANARD_ALWAYS_INLINE inline std::array<std::uint32_t, 4> read_tagged_triplet() {
        skip_whitespace();
        auto mask = Codec::delimiter_mask(cursor_);
        mask &= mask - 1;
        const unsigned second = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned third = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned fourth = std::countr_zero(mask);
        const auto type = static_cast<unsigned>(cursor_[0] - '0');
        const auto [position, a, b] =
            Codec::template decode_three<FirstMaxDigits>({cursor_ + 2, second - 2},
                                                  {cursor_ + second + 1, third - second - 1},
                                                  {cursor_ + third + 1, fourth - third - 1});
        cursor_ += fourth;
        ++cursor_;
        return {type, position, a, b};
    }

    // Scalar/SWAR parser for general uint32 values, including ten digits.
    // MaxDigits selects a 4- or 8-byte fast prefix, not a run-time validator.
    template <unsigned MaxDigits = 10> [[nodiscard]] std::uint32_t read_u32() {
        static_assert(1 <= MaxDigits && MaxDigits <= 10);
        if constexpr (MaxDigits <= 6)
            return read_unsigned<std::uint32_t>();
        else
            return read_unsigned<std::uint64_t>();
    }
    // Signed input, including ten-digit magnitudes. Successful int32 parsing
    // is a precondition; unsigned subtraction also handles INT32_MIN exactly.
    [[nodiscard]] std::int32_t read_i32() {
        skip_whitespace();
        const bool negative = *cursor_ == '-';
        cursor_ += negative;
        const auto magnitude = read_unsigned<std::uint64_t>();
        return std::bit_cast<std::int32_t>(negative ? 0u - magnitude : magnitude);
    }
    [[nodiscard]] const char* position() const noexcept {
        return cursor_;
    }
};
} // namespace canard::io
namespace canard::io::decimal {
struct ascii_avx2_codec {
    CANARD_ALWAYS_INLINE static std::uint32_t delimiter_mask(const char* p) noexcept {
        return decimal::delimiter_mask(p);
    }
    template <unsigned A = 9, unsigned B = 9>
    CANARD_ALWAYS_INLINE static auto decode_pair(field a, field b) noexcept {
        return decimal::decode_pair<A, B>(a, b);
    }
    template <unsigned A = 9, unsigned B = 9, unsigned C = 9>
    CANARD_ALWAYS_INLINE static auto decode_triplet(field a, field b, field c) noexcept {
        return decimal::decode_triplet<A, B, C>(a, b, c);
    }
    template <unsigned A = 9>
    CANARD_ALWAYS_INLINE static auto decode_three(field a, field b, field c) noexcept {
        return decimal::decode_three<A>(a, b, c);
    }
    CANARD_ALWAYS_INLINE static auto decode_four_short(field a, field b, field c, field d) noexcept {
        return decimal::decode_four_short(a, b, c, d);
    }
};
}
namespace canard::io {
using avx2_ascii_reader = basic_trusted_ascii_reader<decimal::ascii_avx2_codec>;
}
// ===== include/canard/io/sink/posix.hpp =====
#include <cerrno>
#include <unistd.h>
namespace canard::io {
// Expert sink retaining the old successful, complete-write precondition.
class trusted_posix_sink {
    int descriptor_;
  public:
    static constexpr bool assume_complete_writes = true;
    explicit trusted_posix_sink(int descriptor = STDOUT_FILENO) noexcept : descriptor_(descriptor) {}
    void write_all(const char* data, std::size_t size) const noexcept {
        ::write(descriptor_, data, size);
    }
};
class posix_sink {
    int descriptor_;
  public:
    static constexpr bool assume_complete_writes = false;
    explicit posix_sink(int descriptor = STDOUT_FILENO) noexcept : descriptor_(descriptor) {}
    transfer_result write_some(const char* data, std::size_t size) const noexcept {
        ssize_t count;
        do { count = ::write(descriptor_, data, size); } while (count < 0 && errno == EINTR);
        if (count < 0) return {0, {errno, std::generic_category()}};
        return {static_cast<std::size_t>(count), {}};
    }
};
}
using client_sink = canard::io::trusted_posix_sink;
#include <cstdint>
#include <span>
#include <vector>
int main() {
    canard::io::padded_file file;
    canard::io::avx2_ascii_reader input{file.view()};
    canard::io::buffered_writer<0xffffffffu, client_sink> output;
    const auto [n, q] = input.read_pair();
    std::vector<std::uint64_t> initial(n);
    for (auto& value : initial)
        value = input.read_u32();
    std::vector<canard::offline::graph_operation> operations;
    operations.reserve(q);
    for (unsigned i = 0; i < q; ++i) {
        const auto kind = input.read_u32<1>();
        const auto first = input.read_u32<6>();
        const auto second = kind == 3 ? 0 : input.read_u32();
        operations.push_back({kind, first, second});
    }
    const canard::offline::component_timeline timeline{operations};
    const auto answers =
        canard::offline::expiry_component_sum(std::span<const std::uint64_t>{initial}, timeline);
    for (const auto answer : answers)
        output.write_u64(answer);
}
