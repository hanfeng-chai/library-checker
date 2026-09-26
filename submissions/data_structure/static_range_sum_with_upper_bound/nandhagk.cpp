// Generated from canard headers and client. Edit the maintained sources.
// ===== clients/static_range_sum_with_upper_bound.cpp =====
// ===== include/canard/range/bounded_prefix_count_sum.hpp =====
// ===== include/canard/associated_types.hpp =====
#include <cstddef>

namespace canard {

// Direct associated-type projections: no cv/ref removal, fallback, or conversion.
// Missing members remain substitution failures in requires-expressions.
template <typename T> using value_type_t = typename T::value_type;

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
// ===== include/canard/algebra/monoid.hpp =====
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
// ===== include/canard/kernel/wide_prefix.hpp =====
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
#include <array>
#include <cstdint>
namespace canard::kernel {
template <unsigned Extent> struct prefix_words {
    using value_type = std::uint64_t;
    struct alignas(64) leaf_type {
        std::array<value_type, Extent> values{};
    };
    using branch_type = leaf_type;
    [[nodiscard]] static leaf_type empty_leaf() noexcept {
        return {};
    }
    [[nodiscard]] static branch_type empty_branch() noexcept {
        return {};
    }
};
template <unsigned Extent, typename Execution> struct unsigned_prefix_block;
template <unsigned Extent>
struct unsigned_prefix_block<Extent, wide::execution::scalar> : prefix_words<Extent> {
    static void add_suffix(std::uint64_t* values, unsigned first, std::uint64_t delta) noexcept {
        for (unsigned i = first; i < Extent; ++i)
            values[i] += delta;
    }
};
} // namespace canard::kernel
// ===== include/canard/memory/wide_storage.hpp =====
// ===== include/canard/structural/wide_layout.hpp =====
#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <utility>

namespace canard::structural {

// Compact level-major geometry. No values, tags, arithmetic domain, or owner.
// Level zero contains blocks of leaves; a level-h slot covers B^h leaves.
template <typename Configuration = wide::configuration<>> class wide_layout {
  public:
    using size_type = std::size_t;
    static constexpr unsigned fanout = Configuration::fanout;
    static constexpr unsigned radix_bits = std::countr_zero(fanout);
    static constexpr unsigned max_height = std::max(
        1u,
        (static_cast<unsigned>(std::bit_width(Configuration::max_size - 1)) + radix_bits - 1) /
            radix_bits);

    template <unsigned Level>
    static constexpr size_type child_capacity = size_type{1} << (Level * radix_bits);
    template <unsigned Level>
    static constexpr unsigned active_entries = static_cast<unsigned>(std::min<size_type>(
        fanout, (Configuration::max_size + child_capacity<Level> - 1) / child_capacity<Level>));

  private:
    size_type size_ = 0;
    unsigned height_ = 0;
    std::array<size_type, max_height> counts_{};
    std::array<size_type, max_height> offsets_{}; // branch-array offsets; level 0 separate.
    size_type branch_count_ = 0;

  public:
    constexpr wide_layout() noexcept = default;
    explicit constexpr wide_layout(size_type n, unsigned minimum_height = 1) noexcept : size_(n) {
        // Preconditions: n <= Configuration::max_size, 1 <= minimum_height <= max_height.
        // Optional unary roots keep a fixed number of prefix levels addressable.
        // Zero is still a normal, height-zero size.
        if (n == 0)
            return;
        auto count = (n + fanout - 1) / fanout;
        counts_[0] = count;
        height_ = 1;
        while (count > 1 || height_ < minimum_height) {
            count = (count + fanout - 1) / fanout;
            counts_[height_] = count;
            offsets_[height_] = branch_count_;
            branch_count_ += count;
            ++height_;
        }
    }
    [[nodiscard]] constexpr size_type size() const noexcept {
        return size_;
    }
    [[nodiscard]] constexpr unsigned height() const noexcept {
        return height_;
    }
    [[nodiscard]] constexpr size_type blocks(unsigned level) const noexcept {
        return counts_[level];
    }
    [[nodiscard]] constexpr size_type offset(unsigned level) const noexcept {
        return offsets_[level];
    }
    [[nodiscard]] constexpr size_type leaf_blocks() const noexcept {
        return counts_[0];
    }
    [[nodiscard]] constexpr size_type branch_blocks() const noexcept {
        return branch_count_;
    }
    [[nodiscard]] constexpr size_type child_span(unsigned level) const noexcept {
        return size_type{1} << (level * radix_bits);
    }
    [[nodiscard]] constexpr unsigned valid_children(unsigned level,
                                                    size_type block) const noexcept {
        const auto children = level == 0 ? size_ : counts_[level - 1];
        return static_cast<unsigned>(std::min<size_type>(fanout, children - block * fanout));
    }
};

// A cover packet describes consecutive complete child slots, not payloads.
// Its logical interval is exactly [first, last), even at the truncated tail.
struct wide_cover_packet {
    unsigned level;
    std::size_t block;
    unsigned first_slot;
    unsigned last_slot;
    std::size_t first;
    std::size_t last;
};

namespace detail {
template <typename Layout, typename Visitor>
void visit_wide_cover_impl(const Layout& layout,
                           unsigned level,
                           std::size_t block,
                           std::size_t first,
                           std::size_t last,
                           Visitor& visit) {
    const auto span = layout.child_span(level);
    const auto base = block * Layout::fanout * span;
    const auto l = first / span;
    const auto r = (last - 1) / span;
    if (level == 0) {
        visit(wide_cover_packet{
            level, block, unsigned(l), unsigned(r + 1), base + first, base + last});
        return;
    }
    const auto full_first = (first + span - 1) / span;
    const auto full_last = last / span;
    if (l == r && (first % span || last % span)) {
        visit_wide_cover_impl(
            layout, level - 1, block * Layout::fanout + l, first % span, last - l * span, visit);
        return;
    }
    if (first % span)
        visit_wide_cover_impl(
            layout, level - 1, block * Layout::fanout + l, first % span, span, visit);
    if (full_first < full_last)
        visit(wide_cover_packet{level,
                                block,
                                unsigned(full_first),
                                unsigned(full_last),
                                base + full_first * span,
                                base + full_last * span});
    if (last % span)
        visit_wide_cover_impl(layout, level - 1, block * Layout::fanout + r, 0, last % span, visit);
}
} // namespace detail

// Increasing logical order; empty intervals produce no visits. This service
// can visit heavyweight external payloads without imposing a monoid on them.
template <typename Layout, typename Visitor>
void visit_cover(const Layout& layout, std::size_t first, std::size_t last, Visitor&& visitor) {
    if (first == last)
        return;
    detail::visit_wide_cover_impl(layout, layout.height() - 1, 0, first, last, visitor);
}

} // namespace canard::structural
#include <iterator>
#include <memory>
#include <ranges>
#include <type_traits>
#include <utility>
#include <vector>

namespace canard::memory {

// Construct identity blocks without importing a sequence of leaf values.
// Used by accumulators whose representation starts with all blocks empty.
struct empty_blocks_t {};
inline constexpr empty_blocks_t empty_blocks{};

// Two typed, contiguous level-major arrays. Leaves do not pay for branch tags.
// Cached level pointers are derived state, rebased after every ownership
// change. Copies/moves never retain another owner's pointers. Ordinary tree
// operations perform no allocation.
template <typename Kernel, typename Configuration, typename Allocator = std::allocator<std::byte>>
class wide_storage {
  public:
    using layout_type = structural::wide_layout<Configuration>;
    using leaf_type = leaf_type_t<Kernel>;
    using branch_type = branch_type_t<Kernel>;
    using allocator_type = Allocator;
    using traits = std::allocator_traits<Allocator>;
    template <typename T> using rebound = typename traits::template rebind_alloc<T>;

  private:
    layout_type layout_;
    std::vector<leaf_type, rebound<leaf_type>> leaves_;
    std::vector<branch_type, rebound<branch_type>> branches_;
    std::array<branch_type*, layout_type::max_height> levels_{};

    void rebase() noexcept {
        levels_.fill(nullptr);
        if constexpr (std::same_as<leaf_type, branch_type>)
            levels_[0] = leaves_.data();
        for (unsigned level = 1; level < layout_.height(); ++level)
            levels_[level] = branches_.data() + layout_.offset(level);
    }

    template <typename Iterator> void build(Iterator first, std::size_t count, Kernel& kernel) {
        layout_ = layout_type{count};
        leaves_.assign(layout_.leaf_blocks(), kernel.empty_leaf());
        branches_.assign(layout_.branch_blocks(), kernel.empty_branch());
        if (count == 0)
            return;
        constexpr auto B = Configuration::fanout;
        for (std::size_t i = 0; i < count; ++i, ++first)
            kernel.write_slot(leaves_[i / B],
                              unsigned(i % B),
                              kernel.import_value(static_cast<value_type_t<Kernel>>(*first)));
        const auto finalize = [&]<typename Block>(Block& block, bool root) {
            if constexpr (requires { kernel.build_summary(block, root); })
                return kernel.build_summary(block, root);
            else
                return kernel.fold(block, 0, B);
        };
        for (unsigned level = 1; level < layout_.height(); ++level) {
            for (std::size_t child = 0; child < layout_.blocks(level - 1); ++child) {
                auto value = level == 1
                                 ? finalize(leaves_[child], false)
                                 : finalize(branches_[layout_.offset(level - 1) + child], false);
                kernel.write_slot(
                    branches_[layout_.offset(level) + child / B], unsigned(child % B), value);
            }
        }
        // Ordinary summaries need no root bookkeeping. A coordinate-frame
        // representation can normalize the root and retain its external frame.
        if constexpr (requires { kernel.build_summary(leaves_[0], true); }) {
            if (layout_.height() == 1)
                finalize(leaves_[0], true);
            else
                finalize(branches_[layout_.offset(layout_.height() - 1)], true);
        }
        rebase();
    }

  public:
    explicit wide_storage(const Allocator& allocator = {})
        : leaves_(rebound<leaf_type>{allocator}), branches_(rebound<branch_type>{allocator}) {}

    wide_storage(layout_type layout,
                 empty_blocks_t,
                 const Kernel& kernel,
                 const Allocator& allocator = {})
        : layout_(layout),
          leaves_(layout.leaf_blocks(), kernel.empty_leaf(), rebound<leaf_type>{allocator}),
          branches_(
              layout.branch_blocks(), kernel.empty_branch(), rebound<branch_type>{allocator}) {
        rebase();
    }

    template <std::ranges::input_range R>
    wide_storage(R&& values, Kernel& kernel, const Allocator& allocator = {})
        : wide_storage(allocator) {
        if constexpr (std::ranges::sized_range<R>) {
            // Exactly one pass; also works for a sized single-pass range.
            build(std::ranges::begin(values), std::ranges::size(values), kernel);
        } else if constexpr (std::ranges::forward_range<R>) {
            const auto count = std::ranges::distance(values);
            build(std::ranges::begin(values), static_cast<std::size_t>(count), kernel);
        } else {
            // The owning input-only overload explicitly stages one input pass.
            using V = value_type_t<Kernel>;
            std::vector<V, rebound<V>> staging{rebound<V>{allocator}};
            for (auto&& value : values)
                staging.emplace_back(value);
            build(staging.begin(), staging.size(), kernel);
        }
    }

    wide_storage(const wide_storage& other)
        : wide_storage(other,
                       traits::select_on_container_copy_construction(other.get_allocator())) {}
    wide_storage(const wide_storage& other, const Allocator& allocator)
        : layout_(other.layout_), leaves_(other.leaves_, rebound<leaf_type>{allocator}),
          branches_(other.branches_, rebound<branch_type>{allocator}) {
        rebase();
    }
    wide_storage(wide_storage&& other) noexcept
        : layout_(std::exchange(other.layout_, {})), leaves_(std::move(other.leaves_)),
          branches_(std::move(other.branches_)) {
        rebase();
        other.rebase();
    }

    wide_storage& operator=(const wide_storage& other) {
        if (this == &other)
            return *this;
        const auto allocator = [&] {
            if constexpr (traits::propagate_on_container_copy_assignment::value)
                return other.get_allocator();
            else
                return get_allocator();
        }();
        wide_storage next{other, allocator}; // All throwing work precedes commit.
        if constexpr (traits::propagate_on_container_copy_assignment::value &&
                      !traits::propagate_on_container_swap::value) {
            // Adopting a different allocator forbids vector::swap here. Vector
            // move construction is noexcept: replace these non-const members
            // transparently after all allocation has succeeded.
            std::destroy_at(&leaves_);
            std::construct_at(&leaves_, std::move(next.leaves_));
            std::destroy_at(&branches_);
            std::construct_at(&branches_, std::move(next.branches_));
            layout_ = next.layout_;
            rebase();
        } else
            swap(next);
        return *this;
    }
    wide_storage& operator=(wide_storage&& other) {
        if (this == &other)
            return *this;
        if constexpr (traits::propagate_on_container_move_assignment::value) {
            leaves_ = std::move(other.leaves_);
            branches_ = std::move(other.branches_);
            layout_ = std::exchange(other.layout_, {});
            rebase();
            other.rebase();
        } else if (get_allocator() == other.get_allocator()) {
            wide_storage next{std::move(other)};
            swap(next);
        } else {
            // Unequal nonpropagating resources: stage a copy before committing.
            // This preserves both trees if allocation fails halfway through.
            wide_storage next{other, get_allocator()};
            swap(next);
            other.clear();
        }
        return *this;
    }
    void swap(wide_storage& other) noexcept(noexcept(leaves_.swap(other.leaves_)) &&
                                            noexcept(branches_.swap(other.branches_))) {
        // Standard precondition: equal resources unless allocator swap propagates.
        using std::swap;
        swap(layout_, other.layout_);
        leaves_.swap(other.leaves_);
        branches_.swap(other.branches_);
        rebase();
        other.rebase();
    }
    void clear() noexcept {
        leaves_.clear();
        branches_.clear();
        layout_ = {};
        rebase();
    }
    [[nodiscard]] Allocator get_allocator() const {
        return Allocator{leaves_.get_allocator()};
    }
    [[nodiscard]] const layout_type& layout() const noexcept {
        return layout_;
    }
    [[nodiscard]] std::size_t storage_bytes() const noexcept {
        return leaves_.size() * sizeof(leaf_type) + branches_.size() * sizeof(branch_type);
    }
    [[nodiscard]] std::size_t allocated_bytes() const noexcept {
        return leaves_.capacity() * sizeof(leaf_type) + branches_.capacity() * sizeof(branch_type);
    }
    // Visit a block without type erasure. Homogeneous representations use one
    // level-pointer lookup; heterogeneous ones retain their distinct types.
    template <typename Function>
    decltype(auto) visit_node(unsigned level, std::size_t index, Function&& function) {
        if constexpr (std::same_as<leaf_type, branch_type>) {
            return std::forward<Function>(function)(levels_[level][index]);
        } else {
            if (level == 0)
                return std::forward<Function>(function)(leaves_[index]);
            return std::forward<Function>(function)(levels_[level][index]);
        }
    }
    template <unsigned Level> [[nodiscard]] decltype(auto) node(std::size_t index) noexcept {
        if constexpr (Level == 0)
            return (leaves_[index]);
        else
            return (levels_[Level][index]);
    }
    template <unsigned Level> [[nodiscard]] decltype(auto) node(std::size_t index) const noexcept {
        if constexpr (Level == 0)
            return (leaves_[index]);
        else
            return std::as_const(levels_[Level][index]);
    }
    [[nodiscard]] const branch_type& branch(unsigned level, std::size_t index) const noexcept {
        return levels_[level][index];
    }
    [[nodiscard]] const leaf_type& leaf(std::size_t index) const noexcept {
        return leaves_[index];
    }
};

} // namespace canard::memory
#include <bit>
#include <cassert>
#include <limits>
#include <type_traits>

namespace canard::wide::representation {
struct packed_count_sum_prefix {};
} // namespace canard::wide::representation

namespace canard {
// A reusable bounded insert-only prefix index, not an unrestricted point-add
// tree. Every position may be inserted at most once; values are <= MaxValue.
// Internal packed prefixes cover at most 2^16 positions. Coarse prefixes use
// full-width sum and count words. Scalar/AVX2 share storage and all traversal.
template <std::uint32_t MaxValue = 1'000'000'000,
          typename Configuration =
              wide::configuration<16, 500000, wide::representation::packed_count_sum_prefix>>
    requires(Configuration::fanout == 16 &&
             std::same_as<representation_type_t<Configuration>,
                          wide::representation::packed_count_sum_prefix> &&
             (std::same_as<execution_type_t<Configuration>, wide::execution::scalar> ||
              std::same_as<execution_type_t<Configuration>, wide::execution::avx2>))
class bounded_prefix_count_sum {
    using execution = execution_type_t<Configuration>;
    using block_kernel = kernel::unsigned_prefix_block<16, execution>;
    using storage_type = memory::wide_storage<block_kernel, Configuration>;
    using layout_type = structural::wide_layout<Configuration>;
    static constexpr unsigned local_levels = std::min(4u, layout_type::max_height);
    static constexpr unsigned chunk_bits = 4 * local_levels;
    static constexpr std::uint64_t chunk_size = std::uint64_t{1} << chunk_bits;
    static constexpr unsigned sum_bits =
        std::max(1u, unsigned(std::bit_width(std::uint64_t{MaxValue} * chunk_size)));
    static constexpr std::uint64_t sum_mask = (std::uint64_t{1} << sum_bits) - 1;
    static_assert(sum_bits + chunk_bits < 64, "Packed chunk count and sum must fit in 64 bits.");
    static constexpr unsigned coarse_extent = std::max(
        4u, std::bit_ceil(unsigned((Configuration::max_size + chunk_size - 1) / chunk_size)));
    static_assert(coarse_extent <= 64);
    using coarse_kernel = kernel::unsigned_prefix_block<coarse_extent, execution>;
    storage_type storage_;
    leaf_type_t<coarse_kernel> coarse_sums_{}, coarse_counts_{};
    // Inclusive leaf prefixes allow end-1 addressing. No one-past-the-final-
    // block load is needed even when n lies exactly on a radix boundary.
    [[nodiscard]] std::uint64_t local_prefix(unsigned position) const noexcept {
        auto answer = storage_.template node<0>(position >> 4).values[position & 15];
        if constexpr (local_levels >= 2)
            answer += storage_.template node<1>(position >> 8).values[(position >> 4) & 15];
        if constexpr (local_levels >= 3)
            answer += storage_.template node<2>(position >> 12).values[(position >> 8) & 15];
        if constexpr (local_levels >= 4)
            answer += storage_.template node<3>(position >> 16).values[(position >> 12) & 15];
        return answer;
    }
    [[nodiscard]] static layout_type layout(std::size_t n) {
        assert(n <= Configuration::max_size);
        return layout_type{n, local_levels};
    }

  public:
    using value_type = std::uint32_t;
    using monoid_type =
        algebra::product_monoid<algebra::sum<std::uint64_t>, algebra::sum<std::uint32_t>>;
    using summary_type = value_type_t<monoid_type>; // {sum, count}
    using configuration_type = Configuration;
    explicit bounded_prefix_count_sum(std::size_t n, Configuration = {})
        : storage_(layout(n), memory::empty_blocks, block_kernel{}) {}
    [[nodiscard]] std::size_t size() const noexcept {
        return storage_.layout().size();
    }
    [[nodiscard]] std::size_t storage_bytes() const noexcept {
        return storage_.storage_bytes() + sizeof(coarse_sums_) + sizeof(coarse_counts_);
    }
    void insert(unsigned position, value_type value) noexcept {
        assert(position < size() && value <= MaxValue);
        const auto encoded = (std::uint64_t{1} << sum_bits) + value;
        block_kernel::add_suffix(
            storage_.template node<0>(position >> 4).values.data(), position & 15, encoded);
        if constexpr (local_levels >= 2)
            block_kernel::add_suffix(storage_.template node<1>(position >> 8).values.data(),
                                     ((position >> 4) & 15) + 1,
                                     encoded);
        if constexpr (local_levels >= 3)
            block_kernel::add_suffix(storage_.template node<2>(position >> 12).values.data(),
                                     ((position >> 8) & 15) + 1,
                                     encoded);
        if constexpr (local_levels >= 4)
            block_kernel::add_suffix(storage_.template node<3>(position >> 16).values.data(),
                                     ((position >> 12) & 15) + 1,
                                     encoded);
        coarse_kernel::add_suffix(coarse_sums_.values.data(), (position >> chunk_bits) + 1, value);
        coarse_kernel::add_suffix(coarse_counts_.values.data(), (position >> chunk_bits) + 1, 1);
    }
    [[nodiscard]] summary_type prefix(unsigned end) const noexcept {
        assert(end <= size());
        if (!end)
            return {};
        const auto position = end - 1;
        const auto local = local_prefix(position);
        return {(local & sum_mask) + coarse_sums_.values[position >> chunk_bits],
                std::uint32_t((local >> sum_bits) + coarse_counts_.values[position >> chunk_bits])};
    }
    [[nodiscard]] summary_type fold(unsigned first, unsigned last) const noexcept {
        assert(first <= last && last <= size());
        const auto left = prefix(first), right = prefix(last);
        return {right.first - left.first, right.second - left.second};
    }
};
} // namespace canard
// ===== include/canard/kernel/wide_prefix_avx2.hpp =====
// ===== include/canard/simd/minimum_avx2.hpp =====
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
#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <immintrin.h>
#include <type_traits>
#include <utility>

#ifndef __AVX2__
#error "This backend requires AVX2 (e.g. -march=znver3 or -march=native)."
#endif

namespace canard::simd {

template <typename T>
concept minimum_scalar = std::same_as<T, double> || std::same_as<T, std::int64_t>;

// Each mask lane is all-zero or all-one. select() operates on registers; it
// never suppresses an earlier full-width load or makes invalid memory safe.
struct mask64x4 {
    __m256i bits;
};

[[nodiscard]] inline mask64x4 operator^(mask64x4 a, mask64x4 b) noexcept {
    return {_mm256_xor_si256(a.bits, b.bits)};
}

[[nodiscard]] inline mask64x4 load_mask(const std::uint64_t* address) noexcept {
    return {_mm256_load_si256(reinterpret_cast<const __m256i*>(address))};
}

// A half-open interval of selected lanes across an aligned sequence of packs.
// Preparing the two mask-row addresses once avoids serial variable shifts in
// every chunk. The table and accessors are independent of any tree or algebra.
template <std::size_t Extent>
    requires(Extent >= 4 && Extent % 4 == 0 && Extent <= 64)
class lane_interval64 {
    alignas(64) inline static constexpr auto suffixes = [] {
        std::array<std::array<std::uint64_t, Extent>, Extent + 1> rows{};
        for (std::size_t first = 0; first < Extent; ++first)
            for (std::size_t lane = first; lane < Extent; ++lane)
                rows[first][lane] = ~std::uint64_t{};
        return rows;
    }();
    const std::uint64_t* first_;
    const std::uint64_t* last_;

  public:
    // 0 <= first <= last <= Extent. Selection is not a masked-memory access.
    lane_interval64(unsigned first, unsigned last) noexcept
        : first_(suffixes[first].data()), last_(suffixes[last].data()) {}
    [[nodiscard]] mask64x4 chunk(std::size_t index) const noexcept {
        return load_mask(first_ + 4 * index) ^ load_mask(last_ + 4 * index);
    }
};

namespace detail {
template <typename T> struct minimum_register;
template <> struct minimum_register<double> {
    using type = __m256d;
};
template <> struct minimum_register<std::int64_t> {
    using type = __m256i;
};
} // namespace detail

// A concrete four-lane backend, not an emulation of the entire std::simd API.
// The tree uses the floating specialization only for exactly representable
// integers. This backend itself has ordinary floating-point semantics.
template <minimum_scalar T> class minimum_pack {
    using native_type = typename detail::minimum_register<T>::type;
    native_type bits_;

    explicit minimum_pack(native_type value) noexcept : bits_(value) {}

  public:
    using value_type = T;
    static constexpr std::size_t size() noexcept {
        return 4;
    }
    static constexpr std::size_t alignment = 32;

    // Default construction leaves the register uninitialized.
    minimum_pack() = default;
    explicit minimum_pack(T value) noexcept {
        if constexpr (std::same_as<T, double>)
            bits_ = _mm256_set1_pd(value);
        else
            bits_ = _mm256_set1_epi64x(value);
    }

    // Four readable/writable elements and 32-byte alignment are preconditions.
    [[nodiscard]] static minimum_pack load(const T* address) noexcept {
        if constexpr (std::same_as<T, double>) {
            return minimum_pack{_mm256_load_pd(address)};
        } else {
            return minimum_pack{_mm256_load_si256(reinterpret_cast<const __m256i*>(address))};
        }
    }

    void store(T* address) const noexcept {
        if constexpr (std::same_as<T, double>)
            _mm256_store_pd(address, bits_);
        else
            _mm256_store_si256(reinterpret_cast<__m256i*>(address), bits_);
    }

    friend minimum_pack operator+(minimum_pack a, minimum_pack b) noexcept {
        if constexpr (std::same_as<T, double>)
            return minimum_pack{_mm256_add_pd(a.bits_, b.bits_)};
        else
            return minimum_pack{_mm256_add_epi64(a.bits_, b.bits_)};
    }

    friend minimum_pack operator-(minimum_pack a, minimum_pack b) noexcept {
        if constexpr (std::same_as<T, double>)
            return minimum_pack{_mm256_sub_pd(a.bits_, b.bits_)};
        else
            return minimum_pack{_mm256_sub_epi64(a.bits_, b.bits_)};
    }

    friend mask64x4 operator==(minimum_pack a, minimum_pack b) noexcept {
        if constexpr (std::same_as<T, double>)
            return {_mm256_castpd_si256(_mm256_cmp_pd(a.bits_, b.bits_, _CMP_EQ_OQ))};
        else
            return {_mm256_cmpeq_epi64(a.bits_, b.bits_)};
    }

    friend minimum_pack min(minimum_pack a, minimum_pack b) noexcept {
        if constexpr (std::same_as<T, double>) {
            return minimum_pack{_mm256_min_pd(a.bits_, b.bits_)};
        } else {
            const auto choose_b = _mm256_cmpgt_epi64(a.bits_, b.bits_);
            return minimum_pack{_mm256_blendv_epi8(a.bits_, b.bits_, choose_b)};
        }
    }

    friend minimum_pack select(mask64x4 selected, minimum_pack a, minimum_pack b) noexcept {
        if constexpr (std::same_as<T, double>) {
            return minimum_pack{
                _mm256_blendv_pd(b.bits_, a.bits_, _mm256_castsi256_pd(selected.bits))};
        } else {
            return minimum_pack{_mm256_blendv_epi8(b.bits_, a.bits_, selected.bits)};
        }
    }

    friend minimum_pack mask_zero(mask64x4 selected, minimum_pack value) noexcept {
        if constexpr (std::same_as<T, double>) {
            return minimum_pack{_mm256_and_pd(value.bits_, _mm256_castsi256_pd(selected.bits))};
        } else {
            return minimum_pack{_mm256_and_si256(value.bits_, selected.bits)};
        }
    }

    [[nodiscard]] T reduce_min() const noexcept {
        if constexpr (std::same_as<T, double>) {
            const auto pairs =
                _mm_min_pd(_mm256_castpd256_pd128(bits_), _mm256_extractf128_pd(bits_, 1));
            return _mm_cvtsd_f64(_mm_min_sd(pairs, _mm_unpackhi_pd(pairs, pairs)));
        } else {
            const auto low = _mm256_castsi256_si128(bits_);
            const auto high = _mm256_extracti128_si256(bits_, 1);
            const auto pairs = _mm_blendv_epi8(low, high, _mm_cmpgt_epi64(low, high));
            const auto swapped = _mm_unpackhi_epi64(pairs, pairs);
            return _mm_cvtsi128_si64(
                _mm_blendv_epi8(pairs, swapped, _mm_cmpgt_epi64(pairs, swapped)));
        }
    }
};

using f64x4 = minimum_pack<double>;
using i64x4 = minimum_pack<std::int64_t>;
static_assert(sizeof(f64x4) == 32 && alignof(f64x4) == 32);
static_assert(sizeof(i64x4) == 32 && alignof(i64x4) == 32);

} // namespace canard::simd
#include <bit>
namespace canard::kernel {
template <unsigned Extent>
    requires(Extent >= 4 && Extent <= 64 && Extent % 4 == 0)
struct unsigned_prefix_block<Extent, wide::execution::avx2> : prefix_words<Extent> {
    static void add_suffix(std::uint64_t* values, unsigned first, std::uint64_t delta) noexcept {
        const simd::lane_interval64<Extent> selected{first, Extent};
        const simd::i64x4 increment{std::bit_cast<std::int64_t>(delta)};
        for (unsigned k = 0; k < Extent; k += 4) {
            // Access through the corresponding signed type is permitted;
            // AVX2's packed add is modulo 2^64, without scalar signed overflow.
            auto* p = reinterpret_cast<std::int64_t*>(values + k);
            (simd::i64x4::load(p) + mask_zero(selected.chunk(k / 4), increment)).store(p);
        }
    }
};
} // namespace canard::kernel
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
} // namespace canard::io::decimal
#include <cstddef>
#include <memory>
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

    void write(std::uint32_t value) {
        char* const end = storage_.get() + capacity;
        if (end - cursor_ < 16) {
            flush();
        }
        cursor_ = decimal::format_u32_line<MaxValue>(cursor_, end, value);
    }
};

} // namespace canard::io
// ===== include/canard/offline/radix_sort_u32.hpp =====
// Two stable 15-bit passes, as in the supplied baseline.

#include <bits/extc++.h>

namespace canard::offline {

template <unsigned KeyBits = 32,
          unsigned RadixBits = 16,
          std::random_access_iterator Iterator,
          typename Key>
void radix_sort_u32(Iterator first, Iterator last, Key key) {
    static_assert(1 <= RadixBits && RadixBits <= 16);
    static_assert(RadixBits < KeyBits && KeyBits <= 2 * RadixBits);
    constexpr uint32_t bucket_count = uint32_t(1) << RadixBits;
    constexpr uint32_t mask = bucket_count - 1;
    using Value = std::iter_value_t<Iterator>;

    std::size_t count = last - first;
    if (count < bucket_count / 8) {
        std::stable_sort(first, last, [&](const Value& left, const Value& right) {
            return key(left) < key(right);
        });
        return;
    }

    std::array<uint32_t, bucket_count> low_count{}, high_count{};
    for (const Value& value : std::ranges::subrange(first, last)) {
        uint32_t current = key(value);
        if constexpr (KeyBits < 32)
            assert(current < (uint32_t(1) << KeyBits));
        ++low_count[current & mask];
        ++high_count[(current >> RadixBits) & mask];
    }
    for (uint32_t i = 1; i < bucket_count; ++i) {
        low_count[i] += low_count[i - 1];
        high_count[i] += high_count[i - 1];
    }

    std::vector<Value> temporary(count);
    for (std::size_t i = count; i--;) {
        uint32_t current = key(first[i]);
        temporary[--low_count[current & mask]] = std::move(first[i]);
    }
    for (std::size_t i = count; i--;) {
        uint32_t current = key(temporary[i]);
        first[--high_count[(current >> RadixBits) & mask]] = std::move(temporary[i]);
    }
}

} // namespace canard::offline
#include <vector>
namespace {
struct item {
    std::uint32_t value, position;
};
struct query {
    std::uint32_t first, last, bound, index;
};
using profile = canard::wide::configuration<16,
                                            500000,
                                            canard::wide::representation::packed_count_sum_prefix,
                                            canard::wide::execution::avx2>;
using index_type = canard::bounded_prefix_count_sum<1'000'000'000, profile>;
} // namespace
int main() {
    canard::io::padded_file file;
    canard::io::trusted_ascii_reader input{file.view()};
    canard::io::buffered_writer output;
    const auto [n, q] = input.read_pair();
    std::vector<item> items(n);
    for (unsigned i = 0; i + 1 < n; i += 2) {
        const auto [a, b] = input.read_pair<10, 10>();
        items[i] = {a, i};
        items[i + 1] = {b, i + 1};
    }
    if (n & 1)
        items.back() = {input.read_u32<10>(), n - 1};
    std::vector<query> queries(q);
    for (unsigned i = 0; i < q; ++i) {
        const auto [l, r, bound] = input.read_triplet<6, 6, 10>();
        queries[i] = {l, r, bound, i};
    }
    canard::offline::radix_sort_u32<30, 15>(
        items.begin(), items.end(), [](const item& x) { return x.value; });
    canard::offline::radix_sort_u32<30, 15>(
        queries.begin(), queries.end(), [](const query& x) { return x.bound; });
    index_type tree{n};
    std::vector<canard::summary_type_t<index_type>> answers(q);
    unsigned inserted = 0;
    for (const auto& current : queries) {
        while (inserted < n && items[inserted].value <= current.bound) {
            tree.insert(items[inserted].position, items[inserted].value);
            ++inserted;
        }
        answers[current.index] = tree.fold(current.first, current.last);
    }
    for (const auto [sum, count] : answers)
        output.write_pair(count, sum);
}
