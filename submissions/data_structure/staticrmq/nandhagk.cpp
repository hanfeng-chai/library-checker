// Generated from canard headers and client. Edit the maintained sources.
// ===== clients/static_rmq.cpp =====
// ===== include/canard/range/blocked_sparse_table.hpp =====
// ===== include/canard/kernel/static_block_scalar.hpp =====
// ===== include/canard/algebra/idempotent.hpp =====
// ===== include/canard/algebra/monoid.hpp =====
// ===== include/canard/associated_types.hpp =====
#include <cstddef>

namespace canard {

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
#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace canard::algebra {

// These concepts check EXPRESSIONS, not the semantic laws in docs/CONTRACTS.md.
// In particular, no syntactic trait is taken as proof of commutativity.
template <typename M>
concept monoid = std::copy_constructible<M> &&
                 requires(const M& m, const value_type_t<M>& x, const value_type_t<M>& y) {
                     requires std::copyable<value_type_t<M>>;
                     { m.identity() } -> std::same_as<value_type_t<M>>;
                     { m.combine(x, y) } -> std::same_as<value_type_t<M>>;
                 };

template <typename A, typename M>
concept action_for = monoid<M> && std::copy_constructible<A> &&
                     requires(const A& a,
                              const tag_type_t<A>& newer,
                              const tag_type_t<A>& older,
                              const value_type_t<M>& x,
                              std::size_t count) {
                         requires std::copyable<tag_type_t<A>>;
                         { a.identity() } -> std::same_as<tag_type_t<A>>;
                         { a.compose(newer, older) } -> std::same_as<tag_type_t<A>>;
                         { a.map(newer, x, count) } -> std::same_as<value_type_t<M>>;
                     };

// Convenient adaptation of an ordinary stateful callable and an explicit
// identity. The callable is stored without type erasure.
template <typename T, typename Combine> struct operation_monoid {
    using value_type = T;
    [[no_unique_address]] Combine operation;
    T unit;
    [[nodiscard]] T identity() const noexcept(std::is_nothrow_copy_constructible_v<T>) {
        return unit;
    }
    [[nodiscard]] T combine(const T& x, const T& y) const
        noexcept(std::is_nothrow_invocable_r_v<T, const Combine&, const T&, const T&>) {
        return std::invoke(operation, x, y);
    }
};
template <typename Combine, typename T>
operation_monoid(Combine, T) -> operation_monoid<T, Combine>;

template <monoid M> struct reversed_monoid {
    using value_type = value_type_t<M>;
    [[no_unique_address]] M base;
    [[nodiscard]] value_type identity() const noexcept(noexcept(base.identity())) {
        return base.identity();
    }
    [[nodiscard]] value_type combine(const value_type& x, const value_type& y) const
        noexcept(noexcept(base.combine(y, x))) {
        return base.combine(y, x);
    }
};
template <typename M> reversed_monoid(M) -> reversed_monoid<M>;

// Componentwise product: a new payload composition does not need a new tree.
template <monoid Left, monoid Right> struct product_monoid {
    using value_type = std::pair<value_type_t<Left>, value_type_t<Right>>;
    [[no_unique_address]] Left left;
    [[no_unique_address]] Right right;
    [[nodiscard]] value_type identity() const
        noexcept(noexcept(left.identity()) && noexcept(right.identity()) &&
                 std::is_nothrow_move_constructible_v<decltype(left.identity())> &&
                 std::is_nothrow_move_constructible_v<decltype(right.identity())>) {
        return {left.identity(), right.identity()};
    }
    [[nodiscard]] value_type combine(const value_type& x, const value_type& y) const
        noexcept(noexcept(left.combine(x.first, y.first)) &&
                 noexcept(right.combine(x.second, y.second)) &&
                 std::is_nothrow_move_constructible_v<value_type>) {
        return {left.combine(x.first, y.first), right.combine(x.second, y.second)};
    }
};
template <typename L, typename R> product_monoid(L, R) -> product_monoid<L, R>;

template <typename Left, typename Right> struct product_action {
    using tag_type = std::pair<tag_type_t<Left>, tag_type_t<Right>>;
    [[no_unique_address]] Left left;
    [[no_unique_address]] Right right;
    [[nodiscard]] tag_type identity() const
        noexcept(noexcept(left.identity()) && noexcept(right.identity()) &&
                 std::is_nothrow_move_constructible_v<decltype(left.identity())> &&
                 std::is_nothrow_move_constructible_v<decltype(right.identity())>) {
        return {left.identity(), right.identity()};
    }
    [[nodiscard]] tag_type compose(const tag_type& newer, const tag_type& older) const
        noexcept(noexcept(left.compose(newer.first, older.first)) &&
                 noexcept(right.compose(newer.second, older.second)) &&
                 std::is_nothrow_move_constructible_v<tag_type>) {
        return {left.compose(newer.first, older.first), right.compose(newer.second, older.second)};
    }
    template <typename X, typename Y>
    [[nodiscard]] std::pair<X, Y>
    map(const tag_type& f, const std::pair<X, Y>& x, std::size_t count) const
        noexcept(noexcept(left.map(f.first, x.first, count)) &&
                 noexcept(right.map(f.second, x.second, count)) &&
                 std::is_nothrow_move_constructible_v<std::pair<X, Y>>) {
        return {left.map(f.first, x.first, count), right.map(f.second, x.second, count)};
    }
};
template <typename L, typename R> product_action(L, R) -> product_action<L, R>;

// Used only internally to erase action storage and propagation from point trees.
struct no_action {
    struct tag_type {};
    [[nodiscard]] constexpr tag_type identity() const noexcept {
        return {};
    }
    [[nodiscard]] constexpr tag_type compose(tag_type, tag_type) const noexcept {
        return {};
    }
    template <typename T>
    [[nodiscard]] constexpr T map(tag_type, const T& x, std::size_t) const
        noexcept(std::is_nothrow_copy_constructible_v<T>) {
        return x;
    }
};

} // namespace canard::algebra
// ===== include/canard/algebra/basic.hpp =====
#include <algorithm>
#include <concepts>
#include <limits>

namespace canard::algebra {

template <std::integral T> struct sum {
    using value_type = T;
    [[nodiscard]] constexpr T identity() const noexcept {
        return T{0};
    }
    [[nodiscard]] constexpr T combine(T a, T b) const noexcept {
        return a + b;
    }
};

template <std::integral T> struct minimum {
    using value_type = T;
    [[nodiscard]] constexpr T identity() const noexcept {
        return std::numeric_limits<T>::max();
    }
    [[nodiscard]] constexpr T combine(T a, T b) const noexcept {
        return std::min(a, b);
    }
};

template <std::integral T> struct maximum {
    using value_type = T;
    [[nodiscard]] constexpr T identity() const noexcept {
        return std::numeric_limits<T>::lowest();
    }
    [[nodiscard]] constexpr T combine(T a, T b) const noexcept {
        return std::max(a, b);
    }
};

template <std::integral T> struct add_to_sum {
    using tag_type = T;
    [[nodiscard]] constexpr T identity() const noexcept {
        return T{0};
    }
    // Newer is applied AFTER older. Addition happens to commute; others do not.
    [[nodiscard]] constexpr T compose(T newer, T older) const noexcept {
        return newer + older;
    }
    [[nodiscard]] constexpr T map(T f, T x, std::size_t count) const noexcept {
        return x + f * static_cast<T>(count);
    }
};

template <std::integral T> struct add_to_extremum {
    using tag_type = T;
    [[nodiscard]] constexpr T identity() const noexcept {
        return T{0};
    }
    [[nodiscard]] constexpr T compose(T newer, T older) const noexcept {
        return newer + older;
    }
    [[nodiscard]] constexpr T map(T f, T x, std::size_t count) const noexcept {
        // Length distinguishes a real INT_MAX/INT_MIN leaf from an empty span.
        return count == 0 ? x : x + f;
    }
};

} // namespace canard::algebra

namespace canard::algebra {
// An explicit semantic promise, not a law that a C++ concept can prove.
// Specialize to true only when combine(x, x) == x for every valid x.
template <typename Monoid> inline constexpr bool enable_idempotent_monoid = false;

template <std::integral T> inline constexpr bool enable_idempotent_monoid<minimum<T>> = true;

template <std::integral T> inline constexpr bool enable_idempotent_monoid<maximum<T>> = true;

template <typename Monoid>
concept idempotent_monoid = monoid<Monoid> && enable_idempotent_monoid<Monoid>;
} // namespace canard::algebra
// ===== include/canard/wide/configuration.hpp =====
#include <bit>
#include <cstddef>
#include <cstdint>
#include <limits>

namespace canard::wide {

namespace representation {
// Ordinary child summaries, with pending action tags for lazy trees.
struct ordinary {};
// Bounded signed-integer parent/child gaps and one absolute root coordinate.
// This describes an invariant and an update law, not an instruction set.
struct normalized_extremum {};
} // namespace representation
namespace execution {
// Portable C++ block operations. The compiler may still auto-vectorize them.
struct scalar {};
// Explicit AVX2 block operations; requires the matching implementation header.
struct avx2 {};
} // namespace execution

// Open, compile-time customization point. Each admitted combination supplies
// a local block kernel, never an owner or a complete tree. No silent fallback.
template <typename Representation,
          typename Execution,
          typename Monoid,
          typename Action,
          unsigned Fanout>
struct kernel_binding {};

template <unsigned Fanout = 16,
          std::size_t MaxSize = (std::size_t{1} << 30),
          typename Representation = representation::ordinary,
          typename Execution = execution::scalar>
struct configuration {
    static_assert(Fanout >= 2 && Fanout <= 64 && std::has_single_bit(Fanout));
    static_assert(MaxSize > 0 && MaxSize <= std::numeric_limits<std::uint32_t>::max());
    static constexpr unsigned fanout = Fanout;
    static constexpr std::size_t max_size = MaxSize;
    using representation_type = Representation;
    using execution_type = Execution;
};

template <typename Configuration, typename Monoid, typename Action>
concept supported_configuration = requires {
    typename kernel_binding<representation_type_t<Configuration>,
                            execution_type_t<Configuration>,
                            Monoid,
                            Action,
                            Configuration::fanout>::type;
};

template <typename Configuration, typename Monoid, typename Action>
    requires supported_configuration<Configuration, Monoid, Action>
using kernel_for_t = typename kernel_binding<representation_type_t<Configuration>,
                                             execution_type_t<Configuration>,
                                             Monoid,
                                             Action,
                                             Configuration::fanout>::type;

} // namespace canard::wide
#include <span>

namespace canard::kernel {
template <algebra::idempotent_monoid Monoid, unsigned BlockSize> struct static_block_scalar {
    using value_type = value_type_t<Monoid>;
    [[no_unique_address]] Monoid monoid;

    [[nodiscard]] value_type identity() const {
        return monoid.identity();
    }
    [[nodiscard]] value_type combine(const value_type& left, const value_type& right) const {
        return monoid.combine(left, right);
    }
    void scan(const value_type* values, value_type* prefixes, value_type* suffixes) const {
        prefixes[0] = values[0];
        for (unsigned i = 1; i < BlockSize; ++i)
            prefixes[i] = combine(prefixes[i - 1], values[i]);
        suffixes[BlockSize - 1] = values[BlockSize - 1];
        for (unsigned i = BlockSize - 1; i != 0; --i)
            suffixes[i - 1] = combine(values[i - 1], suffixes[i]);
    }
    [[nodiscard]] value_type
    fold_local(const value_type* block, unsigned first, unsigned last) const {
        auto result = block[first];
        for (unsigned i = first + 1; i < last; ++i)
            result = combine(result, block[i]);
        return result;
    }
    void combine_rows(std::span<const value_type> left,
                      const value_type* right,
                      value_type* destination) const {
        for (std::size_t i = 0; i < left.size(); ++i)
            destination[i] = combine(left[i], right[i]);
    }
};
} // namespace canard::kernel

namespace canard::static_query {
// Separate from wide-tree kernel_binding: a static index has overlapping rows,
// not child summaries or lazy tags. Unsupported combinations never fall back.
template <typename Representation, typename Execution, typename Monoid, unsigned BlockSize>
struct blocked_kernel_binding {};

template <algebra::idempotent_monoid Monoid, unsigned BlockSize>
    requires(!std::same_as<value_type_t<Monoid>, bool>)
struct blocked_kernel_binding<wide::representation::ordinary,
                              wide::execution::scalar,
                              Monoid,
                              BlockSize> {
    using type = kernel::static_block_scalar<Monoid, BlockSize>;
};

template <typename Configuration, typename Monoid>
concept supported_blocked_configuration = requires {
    typename blocked_kernel_binding<representation_type_t<Configuration>,
                                    execution_type_t<Configuration>,
                                    Monoid,
                                    Configuration::fanout>::type;
};

template <typename Configuration, typename Monoid>
    requires supported_blocked_configuration<Configuration, Monoid>
using blocked_kernel_t = typename blocked_kernel_binding<representation_type_t<Configuration>,
                                                         execution_type_t<Configuration>,
                                                         Monoid,
                                                         Configuration::fanout>::type;
} // namespace canard::static_query
// ===== include/canard/memory/sparse_table_storage.hpp =====
#include <array>
#include <bit>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <span>
#include <utility>
#include <vector>

namespace canard::memory {
// Exact-length, level-major rows. Ordinary row k has size - 2^k + 1 cells.
// An optional first row represents all size + 1 empty intervals. Its cells keep
// the supplied initial value; the owner is responsible for supplying an identity.
// Contains no operation and no cached pointers into another owner.
template <typename T, bool IncludeEmptyRow = false> class sparse_table_storage {
    static constexpr unsigned first_row = IncludeEmptyRow ? 1 : 0;
    std::vector<T> cells_;
    std::array<std::size_t, 34> offsets_{};
    std::uint32_t size_ = 0;
    unsigned height_ = 0;

  public:
    sparse_table_storage() = default;
    sparse_table_storage(std::uint32_t size, const T& initial)
        : size_(size), height_(std::bit_width(size)) {
        if constexpr (IncludeEmptyRow) {
            offsets_[1] = std::size_t{size} + 1;
        }
        for (unsigned level = 0; level < height_; ++level) {
            offsets_[first_row + level + 1] =
                offsets_[first_row + level] + size - (std::size_t{1} << level) + 1;
        }
        cells_.assign(offsets_[first_row + height_], initial);
    }
    sparse_table_storage(const sparse_table_storage&) = default;
    sparse_table_storage& operator=(const sparse_table_storage&) = default;
    sparse_table_storage(sparse_table_storage&& other) noexcept
        : cells_(std::move(other.cells_)), offsets_(other.offsets_),
          size_(std::exchange(other.size_, 0)), height_(std::exchange(other.height_, 0)) {}
    sparse_table_storage& operator=(sparse_table_storage&& other) noexcept {
        if (this != &other) {
            cells_ = std::move(other.cells_);
            offsets_ = other.offsets_;
            size_ = std::exchange(other.size_, 0);
            height_ = std::exchange(other.height_, 0);
        }
        return *this;
    }
    [[nodiscard]] unsigned height() const noexcept {
        return height_;
    }
    [[nodiscard]] std::span<T> row(unsigned level) noexcept {
        assert(level < height_);
        const unsigned physical = level + first_row;
        return {cells_.data() + offsets_[physical],
                offsets_[physical + 1] - offsets_[physical]};
    }
    [[nodiscard]] std::span<const T> row(unsigned level) const noexcept {
        assert(level < height_);
        const unsigned physical = level + first_row;
        return {cells_.data() + offsets_[physical],
                offsets_[physical + 1] - offsets_[physical]};
    }
    // level == bit_width(count): zero selects the empty-interval row, otherwise
    // selects intervals of length 2^(level - 1). Requires a populated identity row,
    // not default-constructed or moved-from storage.
    [[nodiscard]] std::span<const T> overlap_row(unsigned level) const noexcept
        requires(IncludeEmptyRow)
    {
        assert(level <= height_);
        return {cells_.data() + offsets_[level], offsets_[level + 1] - offsets_[level]};
    }
    [[nodiscard]] std::size_t storage_bytes() const noexcept {
        return cells_.size() * sizeof(T);
    }
    void swap(sparse_table_storage& other) noexcept {
        cells_.swap(other.cells_);
        std::swap(offsets_, other.offsets_);
        std::swap(size_, other.size_);
        std::swap(height_, other.height_);
    }
};
} // namespace canard::memory
#include <algorithm>
#include <cassert>
#include <format>
#include <iterator>
#include <ranges>
#include <span>
#include <type_traits>
#include <vector>

namespace canard {
struct blocked_sparse_table_description {
    std::size_t size;
    unsigned block_size;
    unsigned macro_levels;
    std::size_t storage_bytes;
};

template <algebra::idempotent_monoid Monoid, typename Configuration = wide::configuration<32>>
    requires static_query::supported_blocked_configuration<Configuration, Monoid>
class blocked_sparse_table {
  public:
    using value_type = value_type_t<Monoid>;
    using size_type = std::uint32_t;
    using monoid_type = Monoid;
    using configuration_type = Configuration;
    static constexpr unsigned block_size = Configuration::fanout;

  private:
    using kernel_type = static_query::blocked_kernel_t<Configuration, Monoid>;
    [[no_unique_address]] kernel_type kernel_;
    std::vector<value_type> values_, prefixes_, suffixes_;
    memory::sparse_table_storage<value_type, true> macro_;
    size_type size_ = 0;

    void build() {
        assert(values_.size() <= Configuration::max_size);
        size_ = static_cast<size_type>(values_.size());
        const std::size_t blocks = (std::size_t{size_} + block_size - 1) / block_size;
        const auto identity = kernel_.identity();
        values_.resize(blocks * block_size, identity);
        prefixes_.assign(values_.size(), identity);
        suffixes_.assign(values_.size(), identity);
        macro_ = memory::sparse_table_storage<value_type, true>{
            static_cast<size_type>(blocks), identity};
        if (size_ == 0)
            return;
        auto first = macro_.row(0);
        for (std::size_t block = 0; block < blocks; ++block) {
            const auto i = block * block_size;
            kernel_.scan(values_.data() + i, prefixes_.data() + i, suffixes_.data() + i);
            first[block] = suffixes_[i];
        }
        for (unsigned level = 1; level < macro_.height(); ++level) {
            auto current = macro_.row(level);
            auto previous = macro_.row(level - 1);
            const auto half = size_type{1} << (level - 1);
            kernel_.combine_rows(
                previous.first(current.size()), previous.data() + half, current.data());
        }
    }

  public:
    explicit blocked_sparse_table(std::vector<value_type>&& values,
                                  Monoid monoid = {},
                                  Configuration = {})
        : kernel_{std::move(monoid)}, values_(std::move(values)) {
        build();
    }

    template <std::ranges::input_range Range>
        requires std::constructible_from<value_type, std::ranges::range_reference_t<Range>>
    explicit blocked_sparse_table(Range&& values, Monoid monoid = {}, Configuration = {})
        : kernel_{std::move(monoid)} {
        if constexpr (std::ranges::sized_range<Range>) {
            const auto count = std::ranges::size(values);
            assert(count <= Configuration::max_size);
            values_.reserve((count + block_size - 1) / block_size * block_size);
        }
        for (auto&& value : values)
            values_.emplace_back(value);
        build();
    }
    explicit blocked_sparse_table(Monoid monoid = {}, Configuration profile = {})
        : blocked_sparse_table(std::vector<value_type>{}, std::move(monoid), profile) {}
    blocked_sparse_table(const blocked_sparse_table&) = default;
    blocked_sparse_table(blocked_sparse_table&& other) noexcept(
        std::is_nothrow_copy_constructible_v<kernel_type>)
        : kernel_(other.kernel_), values_(std::move(other.values_)),
          prefixes_(std::move(other.prefixes_)), suffixes_(std::move(other.suffixes_)),
          macro_(std::move(other.macro_)), size_(std::exchange(other.size_, 0)) {}
    void swap(blocked_sparse_table& other) noexcept
        requires std::is_nothrow_swappable_v<kernel_type>
    {
        using std::swap;
        swap(kernel_, other.kernel_);
        values_.swap(other.values_);
        prefixes_.swap(other.prefixes_);
        suffixes_.swap(other.suffixes_);
        macro_.swap(other.macro_);
        swap(size_, other.size_);
    }
    blocked_sparse_table& operator=(const blocked_sparse_table& other)
        requires std::is_nothrow_swappable_v<kernel_type>
    {
        if (this != &other) {
            blocked_sparse_table next{other};
            swap(next);
        }
        return *this;
    }
    blocked_sparse_table& operator=(blocked_sparse_table&& other)
        requires std::is_nothrow_swappable_v<kernel_type>
    {
        if (this != &other) {
            blocked_sparse_table next{std::move(other)};
            swap(next);
        }
        return *this;
    }
    [[nodiscard]] size_type size() const noexcept {
        return size_;
    }
    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }
    [[nodiscard]] std::span<const value_type> values() const noexcept {
        return {values_.data(), size_};
    }
    [[nodiscard]] const value_type& operator[](size_type position) const noexcept {
        assert(position < size_);
        return values_[position];
    }
    [[nodiscard, gnu::always_inline]] inline value_type
    fold(size_type first, size_type last) const {
        assert(first <= last && last <= size_);
        if (first == last)
            return kernel_.identity();
        --last;
        auto left = first / block_size, right = last / block_size;
        if (left == right)
            return kernel_.fold_local(values_.data() + std::size_t{left} * block_size,
                                      first % block_size,
                                      last % block_size + 1);
        // Adjacent blocks have no complete middle block. The extra macro row
        // supplies identities in that case, avoiding a second shape branch.
        // The overlapping intervals remain in left-to-right order.
        const auto count = right - left - 1;
        // count <= 2^31 - 1 because block_size >= 2 and size_type is uint32_t.
        // The sentinel low bit makes the bit-width input nonzero even when
        // the middle is empty, avoiding a zero-input branch in code generation.
        const auto level = std::bit_width((count << 1) | size_type{1}) - 1;
        const auto width = (size_type{1} << level) >> 1;
        const auto row = macro_.overlap_row(level);
        const auto middle = kernel_.combine(row[left + 1], row[right - width]);
        return kernel_.combine(kernel_.combine(suffixes_[first], middle), prefixes_[last]);
    }
    template <std::size_t Extent>
    void fold_batch(std::span<const size_type, Extent> first,
                    std::span<const size_type, Extent> last,
                    std::span<value_type, Extent> answers) const {
        assert(first.size() == last.size() && first.size() == answers.size());
        for (std::size_t i = 0; i < first.size(); ++i)
            answers[i] = fold(first[i], last[i]);
    }
    [[nodiscard]] value_type all_fold() const {
        return fold(0, size_);
    }
    [[nodiscard]] std::size_t storage_bytes() const noexcept {
        return (values_.size() + prefixes_.size() + suffixes_.size()) * sizeof(value_type) +
               macro_.storage_bytes();
    }
    [[nodiscard]] blocked_sparse_table_description description() const noexcept {
        return {size_, block_size, macro_.height(), storage_bytes()};
    }
};

template <std::ranges::input_range Range, algebra::idempotent_monoid Monoid>
blocked_sparse_table(Range&&, Monoid) -> blocked_sparse_table<Monoid>;
template <std::ranges::input_range Range, algebra::idempotent_monoid Monoid, typename Configuration>
blocked_sparse_table(Range&&, Monoid, Configuration) -> blocked_sparse_table<Monoid, Configuration>;
} // namespace canard

template <> struct std::formatter<canard::blocked_sparse_table_description, char> {
    constexpr auto parse(std::format_parse_context& context) {
        return context.begin();
    }
    template <typename Context>
    auto format(canard::blocked_sparse_table_description d, Context& context) const {
        return std::format_to(
            context.out(),
            "blocked_sparse_table(size={}, block={}, levels={}, storage={} bytes)",
            d.size,
            d.block_size,
            d.macro_levels,
            d.storage_bytes);
    }
};
// ===== include/canard/static_query/minimum_avx2.hpp =====
// ===== include/canard/simd/minimum_u32_avx2.hpp =====
// ===== include/canard/simd/avx2.hpp =====
// A deliberately small, concrete AVX2 vocabulary; not a std::simd emulation.
// No dispatch, allocation, bounds checks, or implicit scalar conversions.
#include <bit>
#include <cstdint>
#include <type_traits>
#include <immintrin.h>
#ifndef __AVX2__
#error "This backend requires AVX2 (e.g. -march=znver3 or -march=native)."
#endif
namespace canard::simd {
struct mask32x8;
namespace detail {
struct native_access;
}
class u32x8 {
    __m256i bits_;
    explicit u32x8(__m256i bits) noexcept : bits_(bits) {}
    friend struct detail::native_access;

  public:
    using value_type = std::uint32_t;
    using mask_type = mask32x8;
    static constexpr int size() noexcept {
        return 8;
    }
    static constexpr int alignment = 32;
    // Default construction, like an intrinsic register, leaves lanes uninitialized.
    u32x8() = default;
    explicit u32x8(value_type value) noexcept
        : bits_(_mm256_set1_epi32(std::bit_cast<int>(value))) {}
};
// A broadcast invariant encoded in the type, not re-checked at runtime.
// Fixed-factor arithmetic can exploit identical lanes without extra shuffles.
class broadcast_u32x8 {
    u32x8 value_;

  public:
    broadcast_u32x8() = default;
    explicit broadcast_u32x8(std::uint32_t value) noexcept : value_(value) {}
    [[nodiscard]] u32x8 vector() const noexcept {
        return value_;
    }
};
struct mask32x8 {
  private:
    __m256i bits_;
    explicit mask32x8(__m256i bits) noexcept : bits_(bits) {}
    friend struct detail::native_access;

  public:
    // Each lane is either all-zero or all-one, never an arbitrary byte mask.
    mask32x8() = default;
};
namespace detail {
struct native_access {
    static __m256i get(u32x8 v) noexcept {
        return v.bits_;
    }
    static __m256i get(mask32x8 v) noexcept {
        return v.bits_;
    }
    static u32x8 words(__m256i v) noexcept {
        return u32x8{v};
    }
    static mask32x8 mask(__m256i v) noexcept {
        return mask32x8{v};
    }
};
} // namespace detail
static_assert(sizeof(u32x8) == 32 && alignof(u32x8) == 32);
static_assert(sizeof(mask32x8) == 32);
static_assert(std::is_trivially_copyable_v<u32x8>);

// Full-width operations. Alignment and eight readable/writable words are
// preconditions. select() does NOT make an earlier memory access conditional.
[[nodiscard]] inline u32x8 load_aligned(const std::uint32_t* address) noexcept {
    return detail::native_access::words(
        _mm256_load_si256(reinterpret_cast<const __m256i*>(address)));
}
inline void store_aligned(std::uint32_t* address, u32x8 value) noexcept {
    _mm256_store_si256(reinterpret_cast<__m256i*>(address), detail::native_access::get(value));
}
[[nodiscard]] inline u32x8 select(mask32x8 mask, u32x8 when_true, u32x8 when_false) noexcept {
    using access = detail::native_access;
    return access::words(
        _mm256_blendv_epi8(access::get(when_false), access::get(when_true), access::get(mask)));
}
// Lane k is selected iff k >= first. Negative first selects every lane;
// first >= 8 selects none. Precondition: first > INT_MIN.
[[nodiscard]] inline mask32x8 lanes_from(int first) noexcept {
    return detail::native_access::mask(_mm256_cmpgt_epi32(_mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7),
                                                          _mm256_set1_epi32(first - 1)));
}
[[nodiscard]] inline u32x8 operator+(u32x8 a, u32x8 b) noexcept {
    using access = detail::native_access;
    return access::words(_mm256_add_epi32(access::get(a), access::get(b)));
}
[[nodiscard]] inline u32x8 operator-(u32x8 a, u32x8 b) noexcept {
    using access = detail::native_access;
    return access::words(_mm256_sub_epi32(access::get(a), access::get(b)));
}
[[nodiscard]] inline u32x8 min(u32x8 a, u32x8 b) noexcept {
    using access = detail::native_access;
    return access::words(_mm256_min_epu32(access::get(a), access::get(b)));
}
// Explicitly distinguish multiplication modulo 2^32 from the high half of
// the widening product. Neither of these functions is modular-field multiply.
[[nodiscard]] inline u32x8 multiply_low(u32x8 a, u32x8 b) noexcept {
    using access = detail::native_access;
    return access::words(_mm256_mullo_epi32(access::get(a), access::get(b)));
}
[[nodiscard]] inline u32x8 multiply_high(u32x8 a, u32x8 b) noexcept {
    using access = detail::native_access;
    const auto av = access::get(a), bv = access::get(b);
    const auto even = _mm256_mul_epu32(av, bv);
    const auto odd = _mm256_mul_epu32(_mm256_srli_epi64(av, 32), _mm256_srli_epi64(bv, 32));
    return access::words(_mm256_blend_epi32(_mm256_srli_epi64(even, 32), odd, 0xaa));
}
// Same high-half multiplication with the second operand known to be a
// broadcast. AVX2's odd-lane stream can reuse it without a right shift.
[[nodiscard]] inline u32x8 multiply_high(u32x8 a, broadcast_u32x8 b) noexcept {
    using access = detail::native_access;
    const auto av = access::get(a), bv = access::get(b.vector());
    const auto even = _mm256_mul_epu32(av, bv);
    const auto odd = _mm256_mul_epu32(_mm256_srli_epi64(av, 32), bv);
    return access::words(_mm256_blend_epi32(_mm256_srli_epi64(even, 32), odd, 0xaa));
}
} // namespace canard::simd
#include <cstdint>

namespace canard::simd {
// Unaligned, full-width loads: eight live readable words are required.
[[nodiscard]] inline u32x8 load_unaligned_u32(const std::uint32_t* source) noexcept {
    return detail::native_access::words(
        _mm256_loadu_si256(reinterpret_cast<const __m256i*>(source)));
}
inline void store_unaligned_u32(std::uint32_t* destination, u32x8 values) noexcept {
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(destination),
                        detail::native_access::get(values));
}
[[nodiscard]] inline u32x8 prefix_minimum(u32x8 values) noexcept {
    auto x = detail::native_access::get(values);
    x = _mm256_min_epu32(x,
                         _mm256_permutevar8x32_epi32(x, _mm256_setr_epi32(0, 0, 1, 2, 3, 4, 5, 6)));
    x = _mm256_min_epu32(x,
                         _mm256_permutevar8x32_epi32(x, _mm256_setr_epi32(0, 0, 0, 1, 2, 3, 4, 5)));
    x = _mm256_min_epu32(x,
                         _mm256_permutevar8x32_epi32(x, _mm256_setr_epi32(0, 0, 0, 0, 0, 1, 2, 3)));
    return detail::native_access::words(x);
}
[[nodiscard]] inline u32x8 suffix_minimum(u32x8 values) noexcept {
    auto x = detail::native_access::get(values);
    x = _mm256_min_epu32(x,
                         _mm256_permutevar8x32_epi32(x, _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 7)));
    x = _mm256_min_epu32(x,
                         _mm256_permutevar8x32_epi32(x, _mm256_setr_epi32(2, 3, 4, 5, 6, 7, 7, 7)));
    x = _mm256_min_epu32(x,
                         _mm256_permutevar8x32_epi32(x, _mm256_setr_epi32(4, 5, 6, 7, 7, 7, 7, 7)));
    return detail::native_access::words(x);
}
[[nodiscard]] inline std::uint32_t reduce_minimum(u32x8 values) noexcept {
    const auto x = detail::native_access::get(values);
    auto a = _mm_min_epu32(_mm256_castsi256_si128(x), _mm256_extracti128_si256(x, 1));
    a = _mm_min_epu32(a, _mm_shuffle_epi32(a, 0x4e));
    a = _mm_min_epu32(a, _mm_shuffle_epi32(a, 0xb1));
    return static_cast<std::uint32_t>(_mm_cvtsi128_si32(a));
}
} // namespace canard::simd
// ===== include/canard/simd/reduction_avx2.hpp =====
#include <cstdint>

namespace canard::simd {

// Lane k is selected exactly when first <= k < last. Negative boundaries and
// boundaries beyond lane 7 are permitted. first must be greater than INT_MIN.
[[nodiscard]] inline mask32x8 lanes_between(int first, int last) noexcept {
    using access = detail::native_access;
    return access::mask(
        _mm256_andnot_si256(access::get(lanes_from(last)), access::get(lanes_from(first))));
}

[[nodiscard]] inline u32x8 operator&(u32x8 left, u32x8 right) noexcept {
    using access = detail::native_access;
    return access::words(_mm256_and_si256(access::get(left), access::get(right)));
}

// Exact sum of eight unsigned 32-bit lanes, without 32-bit overflow.
[[nodiscard]] inline std::uint64_t reduce_add_widened(u32x8 values) noexcept {
    const auto raw = detail::native_access::get(values);
    const auto pairs = _mm256_add_epi64(_mm256_and_si256(raw, _mm256_set1_epi64x(0xffff'ffffu)),
                                        _mm256_srli_epi64(raw, 32));
    const auto halves =
        _mm_add_epi64(_mm256_castsi256_si128(pairs), _mm256_extracti128_si256(pairs, 1));
    const auto total = _mm_add_epi64(halves, _mm_srli_si128(halves, 8));
    return static_cast<std::uint64_t>(_mm_cvtsi128_si64(total));
}

// Sum an arbitrary contiguous range without reading beyond its end.
// The mathematical total must fit in uint64_t.
[[nodiscard]] inline std::uint64_t sum_widened_u32(const std::uint32_t* first,
                                                   const std::uint32_t* last) noexcept {
    auto even = _mm256_setzero_si256(), odd = _mm256_setzero_si256();
    const auto low = _mm256_set1_epi64x(0xffff'ffffu);
    for (; last - first >= 8; first += 8) {
        const auto values = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(first));
        even = _mm256_add_epi64(even, _mm256_and_si256(values, low));
        odd = _mm256_add_epi64(odd, _mm256_srli_epi64(values, 32));
    }
    const auto total = _mm256_add_epi64(even, odd);
    const auto halves =
        _mm_add_epi64(_mm256_castsi256_si128(total), _mm256_extracti128_si256(total, 1));
    std::uint64_t answer = static_cast<std::uint64_t>(
        _mm_cvtsi128_si64(_mm_add_epi64(halves, _mm_srli_si128(halves, 8))));
    for (; first != last; ++first)
        answer += *first;
    return answer;
}

} // namespace canard::simd
// ===== include/canard/simd/static_for.hpp =====
#include <cstddef>
#include <type_traits>
#include <utility>

namespace canard::simd {

namespace detail {
template <std::size_t... I, typename F>
inline void static_for_impl(std::index_sequence<I...>, F&& function) {
    (function(std::integral_constant<std::size_t, I>{}), ...);
}
} // namespace detail

template <std::size_t N, typename F> inline void static_for(F&& function) {
    detail::static_for_impl(std::make_index_sequence<N>{}, std::forward<F>(function));
}

} // namespace canard::simd

namespace canard::kernel {
template <unsigned BlockSize>
    requires(BlockSize == 8 || BlockSize == 16 || BlockSize == 32 || BlockSize == 64)
struct static_minimum_u32_avx2 : static_block_scalar<algebra::minimum<std::uint32_t>, BlockSize> {
    using value_type = std::uint32_t;
    using base = static_block_scalar<algebra::minimum<value_type>, BlockSize>;
    using pack = simd::u32x8;
    explicit static_minimum_u32_avx2(algebra::minimum<value_type> monoid) : base{monoid} {}
    void scan(const value_type* values, value_type* prefixes, value_type* suffixes) const noexcept {
        value_type before = ~value_type{}, after = ~value_type{};
        for (unsigned i = 0; i < BlockSize; i += 8) {
            const auto p =
                min(simd::prefix_minimum(simd::load_unaligned_u32(values + i)), pack{before});
            simd::store_unaligned_u32(prefixes + i, p);
            before = static_cast<value_type>(
                _mm256_extract_epi32(simd::detail::native_access::get(p), 7));
        }
        for (unsigned i = BlockSize; i != 0; i -= 8) {
            const auto s =
                min(simd::suffix_minimum(simd::load_unaligned_u32(values + i - 8)), pack{after});
            simd::store_unaligned_u32(suffixes + i - 8, s);
            after = static_cast<value_type>(
                _mm256_extract_epi32(simd::detail::native_access::get(s), 0));
        }
    }
    [[nodiscard, gnu::noinline]] value_type
    fold_local(const value_type* block, unsigned first, unsigned last) const noexcept {
        if (last == first + 1)
            return block[first];
        if (last - first <= 8) {
            const unsigned start = std::min(first, BlockSize - 8);
            const auto selected = simd::lanes_between(int(first - start), int(last - start));
            return simd::reduce_minimum(simd::select(
                selected, simd::load_unaligned_u32(block + start), pack{~value_type{}}));
        }
        if constexpr (BlockSize <= 32) {
            auto left = simd::load_unaligned_u32(block + first);
            auto right = simd::load_unaligned_u32(block + last - 8);
            if (last - first > 16) {
                left = min(left, simd::load_unaligned_u32(block + first + 8));
                right = min(right, simd::load_unaligned_u32(block + last - 16));
            }
            return simd::reduce_minimum(min(left, right));
        }
        pack best{~value_type{}};
        simd::static_for<BlockSize / 8>([&](auto chunk) {
            constexpr auto offset = 8 * chunk;
            const auto mask =
                simd::lanes_between(int(first) - int(offset), int(last) - int(offset));
            best = min(
                best,
                simd::select(mask, simd::load_unaligned_u32(block + offset), pack{~value_type{}}));
        });
        return simd::reduce_minimum(best);
    }
    void combine_rows(std::span<const value_type> left,
                      const value_type* right,
                      value_type* destination) const noexcept {
        std::size_t i = 0;
        for (; i + 8 <= left.size(); i += 8) {
            simd::store_unaligned_u32(destination + i,
                                      min(simd::load_unaligned_u32(left.data() + i),
                                          simd::load_unaligned_u32(right + i)));
        }
        for (; i < left.size(); ++i)
            destination[i] = std::min(left[i], right[i]);
    }
};
} // namespace canard::kernel

namespace canard::static_query {
template <unsigned BlockSize>
    requires(BlockSize == 8 || BlockSize == 16 || BlockSize == 32 || BlockSize == 64)
struct blocked_kernel_binding<wide::representation::ordinary,
                              wide::execution::avx2,
                              algebra::minimum<std::uint32_t>,
                              BlockSize> {
    using type = kernel::static_minimum_u32_avx2<BlockSize>;
};
} // namespace canard::static_query
// ===== include/canard/io/trusted_ascii_reader.hpp =====
// ===== include/canard/io/padded_file.hpp =====
#include <cstddef>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
namespace canard::io {
// A borrowing contract, not an owning string: data[-8..size+31] must be
// readable. The bytes outside [data,data+size) need not belong to the file.
// The trusted parsers consume known record counts rather than checking EOF.
struct padded_bytes_view {
    const char* data;
    std::size_t size;
};

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
    [[nodiscard]] padded_bytes_view view() const noexcept {
        return {data_, bytes_};
    }
};
} // namespace canard::io
// ===== include/canard/io/decimal_avx2.hpp =====
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
struct field {
    const char* data;
    unsigned digits;
};

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
[[nodiscard, gnu::always_inline]] inline std::array<std::uint32_t, 2>
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
[[nodiscard, gnu::always_inline]] inline std::array<std::uint32_t, 3>
decode_triplet(field first, field second, field third) noexcept;

// FirstMaxDigits is a precondition, not a run-time validator. A known-small
// first field (e.g. an index) skips its ninth-digit calculation at compile time.
template <unsigned FirstMaxDigits = 9>
[[nodiscard, gnu::always_inline]] inline std::array<std::uint32_t, 3>
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
[[nodiscard, gnu::always_inline]] inline std::array<std::uint32_t, 3>
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
namespace canard::io {
// Borrowing reader over padded bytes; lifetime is controlled by the caller.
// Packed records require exactly one delimiter between fields, <=32 bytes
// including the terminating delimiter, and the stated maximum digit counts.
// read_tagged_triplet(): one-digit tag, then three fields. read_pair(): two
// 1..9-digit fields. read_u32() also accepts repeated leading whitespace.
class trusted_ascii_reader {
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
        static_assert(std::endian::native == std::endian::little);
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
    [[nodiscard, gnu::always_inline]] inline std::array<std::uint32_t, 4>
    read_two_index_pairs() noexcept {
        static_assert(MaxDigits >= 1 && MaxDigits <= 7);
        skip_whitespace();
        auto mask = decimal::delimiter_mask(cursor_);
        const unsigned p0 = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned p1 = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned p2 = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned p3 = std::countr_zero(mask);
        const auto result = decimal::decode_four_short({cursor_, p0},
                                                       {cursor_ + p0 + 1, p1 - p0 - 1},
                                                       {cursor_ + p1 + 1, p2 - p1 - 1},
                                                       {cursor_ + p2 + 1, p3 - p2 - 1});
        cursor_ += p3 + 1;
        return result;
    }

    explicit trusted_ascii_reader(padded_bytes_view input) noexcept : cursor_(input.data) {}
    // One AVX2 delimiter mask per record; decimal reductions use packed
    // byte-to-pair, pair-to-quad, and quad-to-eight-digit multiply-adds.
    // A ninth leading digit is added separately. Field lengths are trusted.
    template <unsigned FirstMaxDigits = 9, unsigned SecondMaxDigits = 9>
    [[nodiscard, gnu::always_inline]] inline std::array<std::uint32_t, 2> read_pair() {
        skip_whitespace();
        auto mask = decimal::delimiter_mask(cursor_);
        const unsigned first = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned second = std::countr_zero(mask);
        const auto [a, b] = decimal::decode_pair<FirstMaxDigits, SecondMaxDigits>(
            {cursor_, first}, {cursor_ + first + 1, second - first - 1});
        cursor_ += second;
        ++cursor_;
        return {a, b};
    }
    template <unsigned FirstMaxDigits = 9,
              unsigned SecondMaxDigits = 9,
              unsigned ThirdMaxDigits = 9>
    [[nodiscard, gnu::always_inline]] inline std::array<std::uint32_t, 3> read_triplet() {
        skip_whitespace();
        auto mask = decimal::delimiter_mask(cursor_);
        const unsigned first = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned second = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned third = std::countr_zero(mask);
        const auto result =
            decimal::decode_triplet<FirstMaxDigits, SecondMaxDigits, ThirdMaxDigits>(
                {cursor_, first},
                {cursor_ + first + 1, second - first - 1},
                {cursor_ + second + 1, third - second - 1});
        cursor_ += third + 1;
        return result;
    }

    // Exactly one one-digit tag and two bounded decimal fields.
    template <unsigned FirstMaxDigits = 9, unsigned SecondMaxDigits = 9>
    [[nodiscard, gnu::always_inline]] inline std::array<std::uint32_t, 3>
    read_tagged_pair() noexcept {
        skip_whitespace();
        const unsigned type = static_cast<unsigned>(cursor_[0] - '0');
        cursor_ += 2;
        auto mask = decimal::delimiter_mask(cursor_);
        const unsigned first = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned second = std::countr_zero(mask);
        const auto [a, b] = decimal::decode_pair<FirstMaxDigits, SecondMaxDigits>(
            {cursor_, first}, {cursor_ + first + 1, second - first - 1});
        cursor_ += second + 1;
        return {type, a, b};
    }

    template <unsigned FirstMaxDigits = 9>
    [[nodiscard, gnu::always_inline]] inline std::array<std::uint32_t, 4> read_tagged_triplet() {
        skip_whitespace();
        auto mask = decimal::delimiter_mask(cursor_);
        mask &= mask - 1;
        const unsigned second = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned third = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned fourth = std::countr_zero(mask);
        const auto type = static_cast<unsigned>(cursor_[0] - '0');
        const auto [position, a, b] =
            decimal::decode_three<FirstMaxDigits>({cursor_ + 2, second - 2},
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
// ===== include/canard/io/buffered_writer.hpp =====
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
#include <memory>
#include <span>
#include <unistd.h>
namespace canard::io {
// Trusted POSIX sink: allocations and complete writes are assumed successful.
// Owns its byte buffer, borrows its descriptor. flush() is also done at destruction.
template <std::uint32_t MaxValue = std::numeric_limits<std::uint32_t>::max()>
class buffered_writer {
    static constexpr std::size_t capacity = 1u << 16;
    std::unique_ptr<char[]> storage_ = std::make_unique_for_overwrite<char[]>(capacity);
    char* cursor_ = storage_.get();

    int descriptor_;

  public:
    explicit buffered_writer(int descriptor = STDOUT_FILENO) noexcept : descriptor_(descriptor) {}
    buffered_writer(const buffered_writer&) = delete;
    buffered_writer& operator=(const buffered_writer&) = delete;
    ~buffered_writer() {
        flush();
    }

    void flush() {
        ::write(descriptor_, storage_.get(), static_cast<std::size_t>(cursor_ - storage_.get()));
        cursor_ = storage_.get();
    }

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
#include <array>
#include <cstdint>
#include <span>
#include <vector>

#ifndef CANARD_RMQ_BLOCK_SIZE
#define CANARD_RMQ_BLOCK_SIZE 16
#endif
#ifndef CANARD_RMQ_BATCH
#define CANARD_RMQ_BATCH 8
#endif
namespace {
using word = std::uint32_t;
#ifdef CANARD_RMQ_SCALAR
using execution = canard::wide::execution::scalar;
#else
using execution = canard::wide::execution::avx2;
#endif
using profile = canard::wide::
    configuration<CANARD_RMQ_BLOCK_SIZE, 500000, canard::wide::representation::ordinary, execution>;
constexpr unsigned batch_size = CANARD_RMQ_BATCH;
static_assert(batch_size >= 1 && batch_size <= 64);
} // namespace
int main() {
    canard::io::padded_file file;
    canard::io::trusted_ascii_reader input{file.view()};
    canard::io::buffered_writer<1'000'000'000> output;
    const auto [n, q] = input.read_pair<6, 6>();
    std::vector<word> initial;
    initial.reserve((n + profile::fanout - 1) / profile::fanout * profile::fanout);
    for (unsigned i = 0; i + 1 < n; i += 2) {
        const auto [a, b] = input.read_pair<10, 10>();
        initial.push_back(a);
        initial.push_back(b);
    }
    if (n & 1)
        initial.push_back(input.read_u32<10>());
    canard::blocked_sparse_table index{
        std::move(initial), canard::algebra::minimum<word>{}, profile{}};
    std::array<word, batch_size> left, right, answers;
    unsigned i = 0;
    for (; q - i >= batch_size; i += batch_size) {
        unsigned j = 0;
        for (; j + 1 < batch_size; j += 2) {
            const auto fields = input.read_two_index_pairs<6>();
            left[j] = fields[0];
            right[j] = fields[1];
            left[j + 1] = fields[2];
            right[j + 1] = fields[3];
        }
        if (j < batch_size) {
            const auto [l, r] = input.read_pair<6, 6>();
            left[j] = l;
            right[j] = r;
        }
        index.fold_batch(std::span<const word, batch_size>{left},
                         std::span<const word, batch_size>{right},
                         std::span<word, batch_size>{answers});
        output.write_compact_batch(std::span<const word, batch_size>{answers});
    }
    for (; i < q; ++i) {
        const auto [l, r] = input.read_pair<6, 6>();
        output.write_compact_u32(index.fold(l, r));
    }
}