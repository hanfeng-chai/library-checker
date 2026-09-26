// Generated from the shared canard headers; edit clients/ and include/.
// C++23. Trusted regular-file input. Compile with the judge flags.

// ===== clients/range_affine_range_sum.cpp =====
// Ordinary child summaries and lazy tags, with AVX2 block execution.

// ===== include/canard/range/wide_lazy_segment_tree.hpp =====


// ===== include/canard/range/detail/wide_owner.hpp =====


// ===== include/canard/kernel/wide_scalar.hpp =====


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
template<class M>
concept monoid = std::copy_constructible<M> && requires(
    const M& m, const typename M::value_type& x, const typename M::value_type& y) {
    requires std::copyable<typename M::value_type>;
    { m.identity() } -> std::same_as<typename M::value_type>;
    { m.combine(x, y) } -> std::same_as<typename M::value_type>;
};

template<class A, class M>
concept action_for = monoid<M> && std::copy_constructible<A> && requires(
    const A& a, const typename A::tag_type& newer, const typename A::tag_type& older,
    const typename M::value_type& x, std::size_t count) {
    requires std::copyable<typename A::tag_type>;
    { a.identity() } -> std::same_as<typename A::tag_type>;
    { a.compose(newer, older) } -> std::same_as<typename A::tag_type>;
    { a.map(newer, x, count) } -> std::same_as<typename M::value_type>;
};

// Convenient adaptation of an ordinary stateful callable and an explicit
// identity. The callable is stored without type erasure.
template<class T, class Combine>
struct operation_monoid {
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
template<class Combine, class T>
operation_monoid(Combine, T) -> operation_monoid<T, Combine>;

template<monoid M>
struct reversed_monoid {
    using value_type = typename M::value_type;
    [[no_unique_address]] M base;
    [[nodiscard]] value_type identity() const noexcept(noexcept(base.identity())) {
        return base.identity();
    }
    [[nodiscard]] value_type combine(const value_type& x, const value_type& y) const
        noexcept(noexcept(base.combine(y, x))) { return base.combine(y, x); }
};
template<class M> reversed_monoid(M) -> reversed_monoid<M>;

// Componentwise product: a new payload composition does not need a new tree.
template<monoid Left, monoid Right>
struct product_monoid {
    using value_type = std::pair<typename Left::value_type, typename Right::value_type>;
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
template<class L, class R> product_monoid(L, R) -> product_monoid<L, R>;

template<class Left, class Right>
struct product_action {
    using tag_type = std::pair<typename Left::tag_type, typename Right::tag_type>;
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
        return {left.compose(newer.first, older.first),
                right.compose(newer.second, older.second)};
    }
    template<class X, class Y>
    [[nodiscard]] std::pair<X, Y> map(const tag_type& f, const std::pair<X, Y>& x,
                                     std::size_t count) const
        noexcept(noexcept(left.map(f.first, x.first, count)) &&
                 noexcept(right.map(f.second, x.second, count)) &&
                 std::is_nothrow_move_constructible_v<std::pair<X, Y>>) {
        return {left.map(f.first, x.first, count), right.map(f.second, x.second, count)};
    }
};
template<class L, class R> product_action(L, R) -> product_action<L, R>;

// Used only internally to erase action storage and propagation from point trees.
struct no_action {
    struct tag_type {};
    [[nodiscard]] constexpr tag_type identity() const noexcept { return {}; }
    [[nodiscard]] constexpr tag_type compose(tag_type, tag_type) const noexcept { return {}; }
    template<class T>
    [[nodiscard]] constexpr T map(tag_type, const T& x, std::size_t) const
        noexcept(std::is_nothrow_copy_constructible_v<T>) { return x; }
};

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
}
namespace execution {
// Portable C++ block operations. The compiler may still auto-vectorize them.
struct scalar {};
// Explicit AVX2 block operations; requires the matching implementation header.
struct avx2 {};
}

// Open, compile-time customization point. Each admitted combination supplies
// a local block kernel, never an owner or a complete tree. No silent fallback.
template<class Representation, class Execution, class Monoid, class Action,
         unsigned Fanout>
struct kernel_binding {};

template<unsigned Fanout = 16, std::size_t MaxSize = (std::size_t{1} << 30),
         class Representation = representation::ordinary,
         class Execution = execution::scalar>
struct configuration {
    static_assert(Fanout >= 2 && Fanout <= 64 && std::has_single_bit(Fanout));
    static_assert(MaxSize > 0 && MaxSize <= std::numeric_limits<std::uint32_t>::max());
    static constexpr unsigned fanout = Fanout;
    static constexpr std::size_t max_size = MaxSize;
    using representation_type = Representation;
    using execution_type = Execution;
};

template<class Configuration, class Monoid, class Action>
concept supported_configuration = requires {
    typename kernel_binding<typename Configuration::representation_type,
        typename Configuration::execution_type, Monoid, Action,
        Configuration::fanout>::type;
};

template<class Configuration, class Monoid, class Action>
    requires supported_configuration<Configuration, Monoid, Action>
using kernel_for_t = typename kernel_binding<
    typename Configuration::representation_type,
    typename Configuration::execution_type, Monoid, Action,
    Configuration::fanout>::type;

} // namespace canard::wide
#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace canard::kernel {
namespace detail {
template<std::size_t N, class T>
[[nodiscard]] constexpr std::array<T, N> repeat(const T& x) {
    return [&]<std::size_t... I>(std::index_sequence<I...>) {
        return std::array<T, N>{(static_cast<void>(I), x)...};
    }(std::make_index_sequence<N>{});
}
template<class Action, unsigned B, bool Lazy>
struct tag_storage {};
template<class Action, unsigned B>
struct tag_storage<Action, B, true> {
    std::array<typename Action::tag_type, B> tags;
    std::uint64_t dirty = 0;
};
} // namespace detail

// Portable reference block implementation. It is generic in the scalar
// algebra; only this block layer knows the physical array representation.
// Other backends can replace fold/transform/repair without replacing traversal.
template<algebra::monoid Monoid, class Action, unsigned Fanout>
    requires algebra::action_for<Action, Monoid>
struct wide_scalar {
    using value_type = typename Monoid::value_type;
    using summary_type = value_type;
    using update_type = typename Action::tag_type;
    using action_type = update_type;
    using prepared_action = action_type;
    static constexpr bool has_lazy = !std::same_as<Action, algebra::no_action>;
    static constexpr unsigned fanout = Fanout;

    struct alignas(64) leaf_type {
        std::array<summary_type, Fanout> values;
    };
    struct alignas(64) branch_type {
        std::array<summary_type, Fanout> values;
        [[no_unique_address]] detail::tag_storage<Action, Fanout, has_lazy> lazy;
    };

    // Generic repair returns a replacement subtotal. There is no subtraction,
    // group operation, delta-combination law, or equality test assumed here.
    struct local_change {};
    struct subtree_change { summary_type value; };

    [[no_unique_address]] Monoid monoid;
    [[no_unique_address]] Action action;

    static constexpr bool nothrow_mutation =
        std::is_nothrow_copy_constructible_v<value_type> &&
        std::is_nothrow_copy_assignable_v<value_type> &&
        std::is_nothrow_move_constructible_v<value_type> &&
        std::is_nothrow_move_assignable_v<value_type> &&
        std::is_nothrow_copy_constructible_v<action_type> &&
        std::is_nothrow_copy_assignable_v<action_type> &&
        std::is_nothrow_move_constructible_v<action_type> &&
        std::is_nothrow_move_assignable_v<action_type> &&
        noexcept(std::declval<const Monoid&>().identity()) &&
        noexcept(std::declval<const Monoid&>().combine(
            std::declval<const value_type&>(), std::declval<const value_type&>())) &&
        noexcept(std::declval<const Action&>().identity()) &&
        noexcept(std::declval<const Action&>().compose(
            std::declval<const action_type&>(), std::declval<const action_type&>())) &&
        noexcept(std::declval<const Action&>().map(
            std::declval<const action_type&>(), std::declval<const value_type&>(), std::size_t{}));

    [[nodiscard]] leaf_type empty_leaf() const {
        return {detail::repeat<Fanout>(monoid.identity())};
    }
    [[nodiscard]] branch_type empty_branch() const {
        if constexpr (has_lazy)
            return {detail::repeat<Fanout>(monoid.identity()),
                    {detail::repeat<Fanout>(action.identity()), 0}};
        else return {detail::repeat<Fanout>(monoid.identity()), {}};
    }
    [[nodiscard]] summary_type identity() const noexcept(nothrow_mutation) { return monoid.identity(); }
    [[nodiscard]] summary_type combine(const summary_type& x, const summary_type& y) const
        noexcept(nothrow_mutation) { return monoid.combine(x, y); }
    [[nodiscard]] summary_type import_value(const value_type& x) const
        noexcept(nothrow_mutation) { return x; }
    [[nodiscard]] value_type export_value(const summary_type& x) const
        noexcept(nothrow_mutation) { return x; }

    template<class Block>
    [[nodiscard]] const summary_type& slot(const Block& block, unsigned i) const noexcept {
        return block.values[i];
    }
    template<class Block>
    void write_slot(Block& block, unsigned i, const summary_type& x) const
        noexcept(nothrow_mutation) { block.values[i] = x; }

    template<unsigned Active = Fanout, class Block>
    [[nodiscard]] summary_type fold(const Block& block, unsigned first, unsigned last) const
        noexcept(nothrow_mutation) {
        static_assert(Active <= Fanout);
        auto answer = identity();
        for (auto i = first; i < last; ++i) answer = combine(answer, slot(block, i));
        return answer;
    }

    [[nodiscard]] action_type identity_action() const noexcept(nothrow_mutation) { return action.identity(); }
    [[nodiscard]] prepared_action prepare(const update_type& update) const
        noexcept(nothrow_mutation) { return update; }
    [[nodiscard]] prepared_action prepare_internal(const action_type& update) const
        noexcept(nothrow_mutation) { return update; }
    [[nodiscard]] action_type compose(const action_type& newer, const action_type& older) const
        noexcept(nothrow_mutation) { return action.compose(newer, older); }
    [[nodiscard]] summary_type map(const action_type& f, const summary_type& x, std::size_t count) const
        noexcept(nothrow_mutation) { return action.map(f, x, count); }

    [[nodiscard]] bool needs_materialization(const branch_type& node, unsigned child) const noexcept {
        if constexpr (has_lazy) return (node.lazy.dirty >> child) & 1;
        else return false;
    }
    [[nodiscard]] action_type child_frame(const branch_type& node, unsigned child) const
        noexcept(nothrow_mutation) {
        if constexpr (has_lazy) return node.lazy.tags[child];
        else return {};
    }
    void clear_frame(branch_type& node, unsigned child) const noexcept(nothrow_mutation) {
        if constexpr (has_lazy) {
            node.lazy.tags[child] = action.identity();
            node.lazy.dirty &= ~(std::uint64_t{1} << child);
        }
    }

    [[nodiscard]] local_change begin_changes() const noexcept { return {}; }
    void account_local(local_change&, local_change) const noexcept {}
    [[nodiscard]] local_change repair_slot(branch_type& node, unsigned child, const subtree_change& change) const
        noexcept(nothrow_mutation) { write_slot(node, child, change.value); return {}; }

    template<bool ReturnChange, class Block>
    [[nodiscard]] subtree_change finish(const Block& node, local_change) const
        noexcept(nothrow_mutation) {
        if constexpr (ReturnChange) return {fold(node, 0, Fanout)};
        else return {identity()};
    }

    // Each addressed slot is an entire child with exactly child_length real
    // leaves. Traversal never sends a padded slot here.
    template<bool ReturnChange, unsigned Active = Fanout, class Block>
    local_change apply_slots(Block& node, unsigned first, unsigned last,
                             const prepared_action& f, std::size_t child_length) const
        noexcept(nothrow_mutation) {
        static_assert(Active <= Fanout);
        for (auto i = first; i < last; ++i) apply_slot(node, i, f, child_length);
        return {};
    }

    // A carried action exists only on a geometrically complete subtree.
    // All Fanout slots here represent real children. Apply incoming outside
    // the complete update interval and (update o incoming) inside it.
    template<class Block>
    local_change apply_carried(Block& node, unsigned first, unsigned last,
                               const prepared_action& update, const action_type& incoming,
                               std::size_t child_length) const noexcept(nothrow_mutation) {
        const auto after = compose(update, incoming);
        for (unsigned i = 0; i < Fanout; ++i)
            apply_slot(node, i, first <= i && i < last ? after : incoming, child_length);
        return {};
    }

    [[nodiscard]] subtree_change replace(leaf_type& node, unsigned child,
                                         const summary_type& value) const noexcept(nothrow_mutation) {
        write_slot(node, child, value);
        return finish<true>(node, {});
    }

private:
    template<class Block>
    void apply_slot(Block& node, unsigned child, const action_type& f,
                    std::size_t length) const noexcept(nothrow_mutation) {
        node.values[child] = map(f, node.values[child], length);
        if constexpr (has_lazy && std::same_as<Block, branch_type>) {
            node.lazy.tags[child] = compose(f, node.lazy.tags[child]);
            node.lazy.dirty |= std::uint64_t{1} << child;
        }
    }
};

} // namespace canard::kernel

namespace canard::wide {
template<algebra::monoid Monoid, class Action, unsigned Fanout>
    requires algebra::action_for<Action, Monoid>
struct kernel_binding<representation::ordinary, execution::scalar,
                      Monoid, Action, Fanout> {
    using type = canard::kernel::wide_scalar<Monoid, Action, Fanout>;
};
} // namespace canard::wide

// ===== include/canard/range/detail/wide_engine.hpp =====


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
template<class Configuration = wide::configuration<>>
class wide_layout {
public:
    using size_type = std::size_t;
    static constexpr unsigned fanout = Configuration::fanout;
    static constexpr unsigned radix_bits = std::countr_zero(fanout);
    static constexpr unsigned max_height = std::max(1u,
        (static_cast<unsigned>(std::bit_width(Configuration::max_size - 1)) + radix_bits - 1) / radix_bits);

    template<unsigned Level>
    static constexpr size_type child_capacity = size_type{1} << (Level * radix_bits);
    template<unsigned Level>
    static constexpr unsigned active_entries = static_cast<unsigned>(std::min<size_type>(fanout,
        (Configuration::max_size + child_capacity<Level> - 1) / child_capacity<Level>));

private:
    size_type size_ = 0;
    unsigned height_ = 0;
    std::array<size_type, max_height> counts_{};
    std::array<size_type, max_height> offsets_{}; // branch-array offsets; level 0 separate.
    size_type branch_count_ = 0;

public:
    constexpr wide_layout() noexcept = default;
    explicit constexpr wide_layout(size_type n) noexcept : size_(n) {
        // Precondition: n <= Configuration::max_size. Zero is a normal size.
        if (n == 0) return;
        auto count = (n + fanout - 1) / fanout;
        counts_[0] = count;
        height_ = 1;
        while (count > 1) {
            count = (count + fanout - 1) / fanout;
            counts_[height_] = count;
            offsets_[height_] = branch_count_;
            branch_count_ += count;
            ++height_;
        }
    }
    [[nodiscard]] constexpr size_type size() const noexcept { return size_; }
    [[nodiscard]] constexpr unsigned height() const noexcept { return height_; }
    [[nodiscard]] constexpr size_type blocks(unsigned level) const noexcept { return counts_[level]; }
    [[nodiscard]] constexpr size_type offset(unsigned level) const noexcept { return offsets_[level]; }
    [[nodiscard]] constexpr size_type leaf_blocks() const noexcept { return counts_[0]; }
    [[nodiscard]] constexpr size_type branch_blocks() const noexcept { return branch_count_; }
    [[nodiscard]] constexpr size_type child_span(unsigned level) const noexcept {
        return size_type{1} << (level * radix_bits);
    }
    [[nodiscard]] constexpr unsigned valid_children(unsigned level, size_type block) const noexcept {
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
template<class Layout, class Visitor>
void visit_wide_cover_impl(const Layout& layout, unsigned level, std::size_t block,
                           std::size_t first, std::size_t last, Visitor& visit) {
    const auto span = layout.child_span(level);
    const auto base = block * Layout::fanout * span;
    const auto l = first / span;
    const auto r = (last - 1) / span;
    if (level == 0) {
        visit(wide_cover_packet{level, block, unsigned(l), unsigned(r + 1),
                               base + first, base + last});
        return;
    }
    const auto full_first = (first + span - 1) / span;
    const auto full_last = last / span;
    if (l == r && (first % span || last % span)) {
        visit_wide_cover_impl(layout, level - 1, block * Layout::fanout + l,
                             first % span, last - l * span, visit);
        return;
    }
    if (first % span)
        visit_wide_cover_impl(layout, level - 1, block * Layout::fanout + l,
                             first % span, span, visit);
    if (full_first < full_last)
        visit(wide_cover_packet{level, block, unsigned(full_first), unsigned(full_last),
                               base + full_first * span, base + full_last * span});
    if (last % span)
        visit_wide_cover_impl(layout, level - 1, block * Layout::fanout + r,
                             0, last % span, visit);
}
} // namespace detail

// Increasing logical order; empty intervals produce no visits. This service
// can visit heavyweight external payloads without imposing a monoid on them.
template<class Layout, class Visitor>
void visit_cover(const Layout& layout, std::size_t first, std::size_t last, Visitor&& visitor) {
    if (first == last) return;
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

// Two typed, contiguous level-major arrays. Leaves do not pay for branch tags.
// Cached level pointers are derived state, rebased after every ownership
// change. Copies/moves never retain another owner's pointers. Ordinary tree
// operations perform no allocation.
template<class Kernel, class Configuration, class Allocator = std::allocator<std::byte>>
class wide_storage {
public:
    using layout_type = structural::wide_layout<Configuration>;
    using leaf_type = typename Kernel::leaf_type;
    using branch_type = typename Kernel::branch_type;
    using allocator_type = Allocator;
    using traits = std::allocator_traits<Allocator>;
    template<class T> using rebound = typename traits::template rebind_alloc<T>;

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

    template<class Iterator>
    void build(Iterator first, std::size_t count, Kernel& kernel) {
        layout_ = layout_type{count};
        leaves_.assign(layout_.leaf_blocks(), kernel.empty_leaf());
        branches_.assign(layout_.branch_blocks(), kernel.empty_branch());
        if (count == 0) return;
        constexpr auto B = Configuration::fanout;
        for (std::size_t i = 0; i < count; ++i, ++first)
            kernel.write_slot(leaves_[i / B], unsigned(i % B),
                              kernel.import_value(static_cast<typename Kernel::value_type>(*first)));
        const auto finalize = [&]<class Block>(Block& block, bool root) {
            if constexpr (requires { kernel.build_summary(block, root); })
                return kernel.build_summary(block, root);
            else return kernel.fold(block, 0, B);
        };
        for (unsigned level = 1; level < layout_.height(); ++level) {
            for (std::size_t child = 0; child < layout_.blocks(level - 1); ++child) {
                auto value = level == 1
                    ? finalize(leaves_[child], false)
                    : finalize(branches_[layout_.offset(level - 1) + child], false);
                kernel.write_slot(branches_[layout_.offset(level) + child / B],
                                  unsigned(child % B), value);
            }
        }
        // Ordinary summaries need no root bookkeeping. A coordinate-frame
        // representation can normalize the root and retain its external frame.
        if constexpr (requires { kernel.build_summary(leaves_[0], true); }) {
            if (layout_.height() == 1) finalize(leaves_[0], true);
            else finalize(branches_[layout_.offset(layout_.height() - 1)], true);
        }
        rebase();
    }

public:
    explicit wide_storage(const Allocator& allocator = {})
        : leaves_(rebound<leaf_type>{allocator}), branches_(rebound<branch_type>{allocator}) {}

    template<std::ranges::input_range R>
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
            using V = typename Kernel::value_type;
            std::vector<V, rebound<V>> staging{rebound<V>{allocator}};
            for (auto&& value : values) staging.emplace_back(value);
            build(staging.begin(), staging.size(), kernel);
        }
    }

    wide_storage(const wide_storage& other)
        : wide_storage(other, traits::select_on_container_copy_construction(other.get_allocator())) {}
    wide_storage(const wide_storage& other, const Allocator& allocator)
        : layout_(other.layout_), leaves_(other.leaves_, rebound<leaf_type>{allocator}),
          branches_(other.branches_, rebound<branch_type>{allocator}) { rebase(); }
    wide_storage(wide_storage&& other) noexcept
        : layout_(std::exchange(other.layout_, {})), leaves_(std::move(other.leaves_)),
          branches_(std::move(other.branches_)) { rebase(); other.rebase(); }

    wide_storage& operator=(const wide_storage& other) {
        if (this == &other) return *this;
        const auto allocator = [&] {
            if constexpr (traits::propagate_on_container_copy_assignment::value)
                return other.get_allocator();
            else return get_allocator();
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
        } else swap(next);
        return *this;
    }
    wide_storage& operator=(wide_storage&& other) {
        if (this == &other) return *this;
        if constexpr (traits::propagate_on_container_move_assignment::value) {
            leaves_ = std::move(other.leaves_);
            branches_ = std::move(other.branches_);
            layout_ = std::exchange(other.layout_, {});
            rebase(); other.rebase();
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
    void swap(wide_storage& other)
        noexcept(noexcept(leaves_.swap(other.leaves_)) && noexcept(branches_.swap(other.branches_))) {
        // Standard precondition: equal resources unless allocator swap propagates.
        using std::swap;
        swap(layout_, other.layout_);
        leaves_.swap(other.leaves_);
        branches_.swap(other.branches_);
        rebase(); other.rebase();
    }
    void clear() noexcept {
        leaves_.clear(); branches_.clear(); layout_ = {}; rebase();
    }
    [[nodiscard]] Allocator get_allocator() const { return Allocator{leaves_.get_allocator()}; }
    [[nodiscard]] const layout_type& layout() const noexcept { return layout_; }
    [[nodiscard]] std::size_t storage_bytes() const noexcept {
        return leaves_.size() * sizeof(leaf_type) + branches_.size() * sizeof(branch_type);
    }
    [[nodiscard]] std::size_t allocated_bytes() const noexcept {
        return leaves_.capacity() * sizeof(leaf_type) + branches_.capacity() * sizeof(branch_type);
    }
    // Visit a block without type erasure. Homogeneous representations use one
    // level-pointer lookup; heterogeneous ones retain their distinct types.
    template<class Function>
    decltype(auto) visit_node(unsigned level, std::size_t index, Function&& function) {
        if constexpr (std::same_as<leaf_type, branch_type>) {
            return std::forward<Function>(function)(levels_[level][index]);
        } else {
            if (level == 0) return std::forward<Function>(function)(leaves_[index]);
            return std::forward<Function>(function)(levels_[level][index]);
        }
    }
    template<unsigned Level>
    [[nodiscard]] decltype(auto) node(std::size_t index) noexcept {
        if constexpr (Level == 0) return (leaves_[index]);
        else return (levels_[Level][index]);
    }
    template<unsigned Level>
    [[nodiscard]] decltype(auto) node(std::size_t index) const noexcept {
        if constexpr (Level == 0) return (leaves_[index]);
        else return std::as_const(levels_[Level][index]);
    }
    [[nodiscard]] const branch_type& branch(unsigned level, std::size_t index) const noexcept {
        return levels_[level][index];
    }
    [[nodiscard]] const leaf_type& leaf(std::size_t index) const noexcept { return leaves_[index]; }
};

} // namespace canard::memory

// ===== include/canard/kernel/wide_block_edit.hpp =====

#include <array>
#include <cstddef>

namespace canard::kernel {

// An edge change is interpreted by the block policy: it may be a replacement
// summary, an additive difference, or a change of coordinates.
template<class Change>
struct child_change {
    unsigned slot;
    Change change;
};

// One local update packet. Apply the prepared action to [first,last), and
// install BoundaryCount already-computed child changes outside that interval.
// Boundary slots are distinct and real. The packet describes work, not a
// run-time enumeration of how the block happened to be modified.
template<class Change, std::size_t BoundaryCount>
struct block_edit {
    unsigned first;
    unsigned last;
    std::array<child_change<Change>, BoundaryCount> boundaries;
};

} // namespace canard::kernel

// ===== include/canard/range/detail/wide_boundary_fold.hpp =====

#include <cstddef>
#include <utility>

namespace canard::detail {

// Shared two-boundary traversal. Reader owns the meaning of a block and of
// moving an observation through a parent edge. This file knows no tags, SIMD,
// sums, minima, fields, inverses, or payload layout.
template<class Layout, class Reader>
class wide_boundary_fold {
    using value_type = typename Reader::summary_type;
    static constexpr auto B = Layout::fanout;
    const Layout& layout_;
    const Reader& read_;

    template<unsigned Level>
    [[nodiscard]] value_type finish(std::size_t index, value_type value,
                                    std::size_t count) const {
        if constexpr (Reader::needs_lift && Level < Layout::max_height) {
            if (Level < layout_.height()) {
                value = read_.template lift<Level>(index / B, unsigned(index % B), value, count);
                return finish<Level + 1>(index / B, std::move(value), count);
            }
        }
        return value;
    }

    template<unsigned Level>
    [[nodiscard]] value_type join(std::size_t left, std::size_t right,
            value_type suffix, value_type prefix, std::size_t suffix_count,
            std::size_t prefix_count, std::size_t total_count) const {
        const auto l = unsigned(left % B), r = unsigned(right % B);
        if constexpr (Reader::needs_lift) {
            suffix = read_.template lift<Level>(left / B, l, suffix, suffix_count);
            prefix = read_.template lift<Level>(right / B, r, prefix, prefix_count);
        }
        if (left / B == right / B) {
            // No commutativity: suffix, middle, prefix are in that order.
            auto middle = read_.template fold_slots<Level>(left / B, l + 1, r);
            auto total = read_.combine(read_.combine(suffix, middle), prefix);
            return finish<Level + 1>(left / B, std::move(total), total_count);
        }
        if constexpr (Level + 1 < Layout::max_height) {
            suffix = read_.combine(suffix, read_.template fold_slots<Level>(left / B, l + 1, B));
            prefix = read_.combine(read_.template fold_slots<Level>(right / B, 0, r), prefix);
            constexpr auto span = Layout::template child_capacity<Level>;
            return join<Level + 1>(left / B, right / B, std::move(suffix), std::move(prefix),
                                  suffix_count + (B - l - 1) * span,
                                  prefix_count + r * span, total_count);
        } else std::unreachable();
    }

public:
    wide_boundary_fold(const Layout& layout, const Reader& reader) noexcept
        : layout_(layout), read_(reader) {}

    [[nodiscard]] value_type operator()(std::size_t first, std::size_t last) const {
        if (first == last) return read_.identity();
        const auto left = first / B, right = (last - 1) / B;
        const auto l = unsigned(first % B), r = unsigned((last - 1) % B) + 1;
        if (left == right)
            return finish<1>(left, read_.template fold_slots<0>(left, l, r), last - first);
        if constexpr (Layout::max_height > 1) {
            auto suffix = read_.template fold_slots<0>(left, l, B);
            auto prefix = read_.template fold_slots<0>(right, 0, r);
            return join<1>(left, right, std::move(suffix), std::move(prefix), B - l, r, last - first);
        } else std::unreachable();
    }
};

} // namespace canard::detail

// ===== include/canard/range/detail/wide_upward_update.hpp =====

#include <array>
#include <cstddef>
#include <utility>

namespace canard::detail {

// Upward-only execution law. Block edits commute with ancestor frames; each
// edited subtree supplies a change certificate for its parent. Neither the
// scheduler nor its access adapter assumes a numeric interpretation of it.
// Unlike lazy descent, this schedule does not need level-specialized child
// lengths. A loop keeps the same hot block code and registers across levels.
template<class Layout, class Access>
class wide_upward_update {
    using delta_type = typename Access::delta_type;
    using action_type = typename Access::action_type;
    using edge = typename Access::edge;
    template<std::size_t Count>
    using edit = typename Access::template edit_type<Count>;
    static constexpr auto B = Layout::fanout;
    const Layout& layout_;
    Access access_;

    void ascend(std::size_t child, unsigned level, delta_type change) const {
        for (; level < layout_.height() && !access_.is_identity(change); ++level) {
            change = access_.repair(level, child / B, unsigned(child % B), change);
            child /= B;
        }
        access_.commit_root(change);
    }
public:
    wide_upward_update(const Layout& layout, Access access) noexcept
        : layout_(layout), access_(std::move(access)) {}

    void operator()(std::size_t first, std::size_t last, action_type update) const {
        auto left = first / B, right = (last - 1) / B;
        auto l = unsigned(first % B), r = unsigned((last - 1) % B) + 1;
        if (left == right) {
            const auto change = access_.edit_block(0, left, edit<0>{l, r, {}}, update);
            ascend(left, 1, change);
            return;
        }
        auto [dl, dr] = access_.edit_pair(0, left, edit<0>{l, B, {}},
                                        right, edit<0>{0, r, {}}, update);
        for (unsigned level = 1; ; ++level) {
            l = unsigned(left % B); r = unsigned(right % B);
            left /= B; right /= B;
            if (left == right) {
                const auto change = access_.edit_block(level, left,
                    edit<2>{l + 1, r, {edge{l, dl}, edge{r, dr}}}, update);
                ascend(left, level + 1, change);
                return;
            }
            auto changes = access_.edit_pair(level,
                left, edit<1>{l + 1, B, {edge{l, dl}}},
                right, edit<1>{0, r, {edge{r, dr}}}, update);
            dl = std::move(changes.first); dr = std::move(changes.second);
        }
    }
};
} // namespace canard::detail
#include <algorithm>
#include <concepts>
#include <cstddef>
#include <functional>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <utility>
#include <vector>

namespace canard::detail {

// ONE block-summary engine for point trees and lazy trees. The kernel
// supplies a scalar reference or a packed/SIMD block implementation. Optional
// delta repair is local to the kernel; generic monoids return replacements.
template<class Kernel, class Configuration, class Allocator>
class wide_engine {
public:
    using kernel_type = Kernel;
    using value_type = typename Kernel::value_type;
    using summary_type = typename Kernel::summary_type;
    using update_type = typename Kernel::update_type;
    using action_type = typename Kernel::action_type;
    using prepared_action = typename Kernel::prepared_action;
    using subtree_change = typename Kernel::subtree_change;
    using allocator_type = Allocator;
    using layout_type = structural::wide_layout<Configuration>;
    using storage_type = memory::wide_storage<Kernel, Configuration, Allocator>;
    static constexpr unsigned fanout = Configuration::fanout;
    static constexpr bool has_lazy = Kernel::has_lazy;
    static constexpr bool nothrow_mutation = Kernel::nothrow_mutation;
    static constexpr bool needs_lift = [] {
        if constexpr (requires { Kernel::needs_lift; }) return Kernel::needs_lift;
        else return has_lazy;
    }();

private:
    static constexpr auto B = fanout;
    [[no_unique_address]] Kernel kernel_;
    storage_type storage_;

    static Kernel construction_kernel(Kernel kernel) {
        if constexpr (requires { kernel.reset_build(); }) kernel.reset_build();
        return kernel;
    }

    template<unsigned Level = 0, class Function>
    decltype(auto) with_root(Function&& function) const {
        if (height() == Level + 1) return function.template operator()<Level>();
        if constexpr (Level + 1 < layout_type::max_height)
            return with_root<Level + 1>(std::forward<Function>(function));
        else std::unreachable();
    }

    template<unsigned Level, bool ReturnChange>
    auto transform(std::size_t index, unsigned first, unsigned last,
                   const prepared_action& update) {
        return kernel_.template apply_slots<ReturnChange, layout_type::template active_entries<Level>>(
            storage_.template node<Level>(index), first, last, update,
            layout_type::template child_capacity<Level>);
    }

    template<unsigned Level>
    typename Kernel::local_change descend(std::size_t index, unsigned child, std::size_t first,
                            std::size_t last, const prepared_action& update) {
        auto& parent = storage_.template node<Level>(index);
        const auto changed = [&] {
            if (kernel_.needs_materialization(parent, child)) {
                const auto incoming = kernel_.child_frame(parent, child);
                kernel_.clear_frame(parent, child);
                return update_carried<Level - 1>(index * B + child, first, last, update, incoming);
            }
            return update_impl<Level - 1, true>(index * B + child, first, last, update);
        }();
        return kernel_.repair_slot(parent, child, changed);
    }

    template<unsigned Level>
    subtree_change update_carried(std::size_t index, std::size_t first, std::size_t last,
                                   const prepared_action& update, const action_type& incoming) {
        auto& node = storage_.template node<Level>(index);
        constexpr auto span = layout_type::template child_capacity<Level>;
        constexpr auto mask = span - 1;
        const auto full_first = unsigned((first + mask) / span);
        const auto full_last = std::max(full_first, unsigned(last / span));
        auto changes = kernel_.apply_carried(node, full_first, full_last, update, incoming, span);
        if constexpr (Level > 0) {
            const auto l = unsigned(first / span), r = unsigned((last - 1) / span);
            if (l == r) {
                if ((first & mask) || (last & mask))
                    kernel_.account_local(changes, descend<Level>(index, l, first & mask,
                                                                  last - l * span, update));
            } else {
                if (first & mask)
                    kernel_.account_local(changes, descend<Level>(index, l, first & mask, span, update));
                if (last & mask)
                    kernel_.account_local(changes, descend<Level>(index, r, 0, last & mask, update));
            }
        }
        return kernel_.template finish<true>(node, changes);
    }

    template<unsigned Level, bool ReturnChange>
    subtree_change update_impl(std::size_t index, std::size_t first, std::size_t last,
                               const prepared_action& update) {
        auto& node = storage_.template node<Level>(index);
        if constexpr (Level == 0) {
            const auto changed = transform<0, ReturnChange>(index, unsigned(first), unsigned(last), update);
            return kernel_.template finish<ReturnChange>(node, changed);
        } else {
            constexpr auto span = layout_type::template child_capacity<Level>;
            constexpr auto mask = span - 1;
            const auto l = unsigned(first / span), r = unsigned((last - 1) / span);
            auto changes = kernel_.begin_changes();
            if (l == r) {
                if ((first & mask) == 0 && (last & mask) == 0)
                    kernel_.account_local(changes, transform<Level, ReturnChange>(index, l, l + 1, update));
                else kernel_.account_local(changes, descend<Level>(index, l, first & mask,
                                                                   last - l * span, update));
            } else {
                if (first & mask)
                    kernel_.account_local(changes, descend<Level>(index, l, first & mask, span, update));
                if (last & mask)
                    kernel_.account_local(changes, descend<Level>(index, r, 0, last & mask, update));
                const auto full_first = unsigned((first + mask) / span);
                const auto full_last = unsigned(last / span);
                if (full_first < full_last)
                    kernel_.account_local(changes,
                        transform<Level, ReturnChange>(index, full_first, full_last, update));
            }
            return kernel_.template finish<ReturnChange>(node, changes);
        }
    }

    template<unsigned Level>
    void materialize_child(std::size_t index, unsigned child) {
        if constexpr (has_lazy) {
            auto& parent = storage_.template node<Level>(index);
            if (kernel_.needs_materialization(parent, child)) {
                const auto prepared = kernel_.prepare_internal(kernel_.child_frame(parent, child));
                transform<Level - 1, false>(index * B + child, 0, B, prepared);
                kernel_.clear_frame(parent, child);
            }
        }
    }

    template<unsigned Level>
    subtree_change replace_impl(std::size_t index, std::size_t position,
                                 const summary_type& value) {
        auto& node = storage_.template node<Level>(index);
        if constexpr (Level == 0) return kernel_.replace(node, unsigned(position), value);
        else {
            constexpr auto span = layout_type::template child_capacity<Level>;
            const auto child = unsigned(position / span);
            materialize_child<Level>(index, child);
            const auto local_value = [&] {
                if constexpr (requires { kernel_.descend_value(node, child, value); })
                    return kernel_.descend_value(node, child, value);
                else return value;
            }();
            const auto change = replace_impl<Level - 1>(index * B + child, position % span, local_value);
            const auto changes = kernel_.repair_slot(node, child, change);
            return kernel_.template finish<true>(node, changes);
        }
    }

    struct reader {
        using summary_type = typename Kernel::summary_type;
        static constexpr bool needs_lift = wide_engine::needs_lift;
        const wide_engine& owner;
        [[nodiscard]] summary_type identity() const { return owner.kernel_.identity(); }
        [[nodiscard]] summary_type combine(const summary_type& x, const summary_type& y) const {
            return owner.kernel_.combine(x, y);
        }
        template<unsigned Level>
        [[nodiscard]] summary_type fold_slots(std::size_t index, unsigned first, unsigned last) const {
            return owner.kernel_.template fold<layout_type::template active_entries<Level>>(
                owner.storage_.template node<Level>(index), first, last);
        }
        template<unsigned Level>
        [[nodiscard]] summary_type lift(std::size_t index, unsigned child, const summary_type& value,
                                        std::size_t count) const {
            const auto frame = owner.kernel_.child_frame(owner.storage_.template node<Level>(index), child);
            if constexpr (requires { owner.kernel_.map_nonempty(frame, value, count); })
                return owner.kernel_.map_nonempty(frame, value, count);
            else return owner.kernel_.map(frame, value, count);
        }
    };

    // Local block adapter for the upward execution law. There is no knowledge
    // of minima or translations in the boundary scheduler or this adapter.
    struct upward_access {
        using delta_type = subtree_change;
        using action_type = prepared_action;
        using edge = kernel::child_change<delta_type>;
        template<std::size_t Edits>
        using edit_type = kernel::block_edit<delta_type, Edits>;
        wide_engine& owner;
        bool is_identity(const delta_type& d) const { return owner.kernel_.change_is_identity(d); }
        void commit_root(const delta_type& d) const { owner.kernel_.commit_root(d); }
        template<std::size_t Edits>
        delta_type edit_block(unsigned level, std::size_t index,
                              const edit_type<Edits>& edit, action_type action) const {
            return owner.storage_.visit_node(level, index, [&](auto& block) {
                if constexpr (requires { owner.kernel_.template edit_block<B>(
                        block, edit, action, unsigned{}); }) {
                    return owner.kernel_.template edit_block<B>(block, edit, action,
                        owner.layout().valid_children(level, index));
                } else {
                    auto changes = owner.kernel_.begin_changes();
                    if constexpr (Edits != 0)
                        for (const auto& boundary : edit.boundaries)
                            owner.kernel_.account_local(changes,
                                owner.kernel_.repair_slot(block, boundary.slot, boundary.change));
                    if (edit.first < edit.last)
                        owner.kernel_.account_local(changes,
                            owner.kernel_.template apply_slots<true, B>(block, edit.first,
                                edit.last, action, owner.layout().child_span(level)));
                    return owner.kernel_.template finish<true>(block, changes);
                }
            });
        }
        template<std::size_t Edits>
        std::pair<delta_type, delta_type> edit_pair(unsigned level,
                std::size_t left, const edit_type<Edits>& left_edit,
                std::size_t right, const edit_type<Edits>& right_edit,
                action_type action) const {
            // Both blocks are at the same level and are distinct. The optional
            // pair hook can stage independent writes before either wide load;
            // absent that hook the same packets execute sequentially.
            return owner.storage_.visit_node(level, left, [&](auto& lhs) {
                return owner.storage_.visit_node(level, right, [&](auto& rhs) {
                    if constexpr (requires { owner.kernel_.edit_pair(lhs, left_edit,
                            rhs, right_edit, action, unsigned{}, unsigned{}); }) {
                        return owner.kernel_.edit_pair(lhs, left_edit, rhs, right_edit, action,
                            owner.layout().valid_children(level, left),
                            owner.layout().valid_children(level, right));
                    } else return std::pair{
                        edit_block(level, left, left_edit, action),
                        edit_block(level, right, right_edit, action)};
                });
            });
        }
        delta_type repair(unsigned level, std::size_t index, unsigned slot, const delta_type& d) const {
            return owner.storage_.visit_node(level, index, [&](auto& block) {
                if constexpr (requires { owner.kernel_.repair_one(block, slot, d); })
                    return owner.kernel_.repair_one(block, slot, d);
                else return owner.kernel_.template finish<true>(block, owner.kernel_.repair_slot(block, slot, d));
            });
        }
    };

    // Shared ordered search for both directions. Carries are read-only and are
    // composed as (outer o local), never in the reverse chronological order.
    template<bool Rightward, unsigned Level, class Predicate>
    std::size_t search(std::size_t index, std::size_t base, std::size_t boundary,
                       const action_type& outer, summary_type& accumulator, Predicate& predicate) const {
        constexpr auto span = layout_type::template child_capacity<Level>;
        const auto& node = storage_.template node<Level>(index);
        const auto active = layout().valid_children(Level, index);
        for (unsigned step = 0; step < active; ++step) {
            const auto slot = Rightward ? step : active - 1 - step;
            const auto first = base + slot * span;
            const auto last = std::min(size(), first + span);
            if constexpr (Rightward) { if (last <= boundary) continue; }
            else { if (first >= boundary) continue; }
            const bool full = Rightward ? first >= boundary : last <= boundary;
            if (full) {
                const auto part = kernel_.map(outer, kernel_.slot(node, slot), last - first);
                auto combined = Rightward ? kernel_.combine(accumulator, part)
                                          : kernel_.combine(part, accumulator);
                if (std::invoke(predicate, kernel_.export_value(combined))) {
                    accumulator = std::move(combined);
                    continue;
                }
                if constexpr (Level == 0) return Rightward ? first : last;
            }
            if constexpr (Level > 0) {
                const auto carry = kernel_.compose(outer, kernel_.child_frame(node, slot));
                const auto result = search<Rightward, Level - 1>(index * B + slot, first, boundary,
                                                                carry, accumulator, predicate);
                if (result != (Rightward ? size() : 0)) return result;
            }
        }
        return Rightward ? size() : 0;
    }

    template<unsigned Level, class Output>
    void write_values(std::size_t index, const action_type& outer, Output& output) const {
        const auto& node = storage_.template node<Level>(index);
        const auto active = layout().valid_children(Level, index);
        for (unsigned child = 0; child < active; ++child) {
            if constexpr (Level == 0) {
                *output = kernel_.export_value(kernel_.map(outer, kernel_.slot(node, child), 1));
                ++output;
            } else {
                const auto carry = kernel_.compose(outer, kernel_.child_frame(node, child));
                write_values<Level - 1>(index * B + child, carry, output);
            }
        }
    }

public:
    template<std::ranges::input_range R>
    wide_engine(R&& values, Kernel kernel, const Allocator& allocator = {})
        : kernel_(construction_kernel(std::move(kernel))), storage_(std::forward<R>(values), kernel_, allocator) {
        if constexpr (requires { Kernel::max_count; })
            static_assert(Configuration::max_size <= Kernel::max_count,
                          "The selected block kernel needs a smaller maximum count.");
    }
    wide_engine(const wide_engine&) = default;
    wide_engine(const wide_engine& other, const Allocator& allocator)
        : kernel_(other.kernel_), storage_(other.storage_, allocator) {}
    // Copying the (usually empty) algebra objects keeps a moved-from empty tree
    // in a valid domain. The owned O(n) arrays themselves are transferred.
    wide_engine(wide_engine&& other)
        noexcept(std::is_nothrow_copy_constructible_v<Kernel>)
        : kernel_(other.kernel_), storage_(std::move(other.storage_)) {}
    wide_engine& operator=(const wide_engine& other)
        requires(std::is_nothrow_copy_assignable_v<Kernel> &&
                 requires(storage_type& s) { s = std::as_const(s); }) {
        if (this != &other) { storage_ = other.storage_; kernel_ = other.kernel_; }
        return *this;
    }
    wide_engine& operator=(wide_engine&& other)
        requires(std::is_nothrow_copy_assignable_v<Kernel>) {
        if (this != &other) { storage_ = std::move(other.storage_); kernel_ = other.kernel_; }
        return *this;
    }
    void swap(wide_engine& other) noexcept
        requires(std::is_nothrow_swappable_v<Kernel>) {
        using std::swap;
        storage_.swap(other.storage_); swap(kernel_, other.kernel_);
    }

    [[nodiscard]] std::size_t size() const noexcept { return layout().size(); }
    [[nodiscard]] bool empty() const noexcept { return size() == 0; }
    [[nodiscard]] unsigned height() const noexcept { return layout().height(); }
    [[nodiscard]] const layout_type& layout() const noexcept { return storage_.layout(); }
    [[nodiscard]] std::size_t storage_bytes() const noexcept { return storage_.storage_bytes(); }
    [[nodiscard]] std::size_t allocated_bytes() const noexcept { return storage_.allocated_bytes(); }
    [[nodiscard]] Allocator get_allocator() const { return storage_.get_allocator(); }
    [[nodiscard]] const Kernel& block_kernel() const noexcept { return kernel_; }

    [[nodiscard]] value_type fold(std::size_t first, std::size_t last) const {
        if constexpr (requires { kernel_.root_summary(); })
            if (first == 0 && last != 0 && last == size())
                return kernel_.export_value(kernel_.root_summary());
        return kernel_.export_value(wide_boundary_fold{layout(), reader{*this}}(first, last));
    }
    [[nodiscard]] value_type get(std::size_t position) const {
        if constexpr (!needs_lift)
            return kernel_.export_value(kernel_.slot(storage_.template node<0>(position / B), unsigned(position % B)));
        else return fold(position, position + 1);
    }
    [[nodiscard]] value_type all_fold() const {
        if (empty()) return kernel_.export_value(kernel_.identity());
        if constexpr (requires { kernel_.root_summary(); })
            return kernel_.export_value(kernel_.root_summary());
        return with_root([&]<unsigned Level> {
            return kernel_.export_value(kernel_.template fold<layout_type::template active_entries<Level>>(
                storage_.template node<Level>(0), 0, layout().valid_children(Level, 0)));
        });
    }
    void set(std::size_t position, const value_type& value) noexcept requires(nothrow_mutation) {
        const auto encoded = kernel_.import_value(value);
        with_root([&]<unsigned Level> {
            const auto changed = replace_impl<Level>(0, position, encoded);
            if constexpr (requires { kernel_.commit_root(changed); })
                kernel_.commit_root(changed);
        });
    }
    [[nodiscard]] prepared_action prepare(const update_type& update) const requires(has_lazy) {
        return kernel_.prepare(update);
    }
    void apply(std::size_t first, std::size_t last, const update_type& update) noexcept
        requires(has_lazy && nothrow_mutation) {
        if (first == last) return;
        apply_prepared(first, last, kernel_.prepare(update));
    }
    void apply_prepared(std::size_t first, std::size_t last, const prepared_action& update) noexcept
        requires(has_lazy && nothrow_mutation) {
        if (first == last) return;
        if constexpr (requires { kernel_.apply_all(update); }) {
            if (first == 0 && last == size()) { kernel_.apply_all(update); return; }
        }
        if constexpr (requires { Kernel::upward_updates; }) {
            static_assert(Kernel::upward_updates);
            upward_access access{*this};
            wide_upward_update{layout(),access}(first,last,update);
        } else with_root([&]<unsigned Level> { update_impl<Level, false>(0, first, last, update); });
    }
    template<class Predicate>
    [[nodiscard]] std::size_t max_right(std::size_t first, Predicate predicate) const {
        if (first == size()) return first;
        auto accumulated = kernel_.identity();
        return with_root([&]<unsigned Level> {
            return search<true, Level>(0, 0, first, kernel_.identity_action(), accumulated, predicate);
        });
    }
    template<class Predicate>
    [[nodiscard]] std::size_t min_left(std::size_t last, Predicate predicate) const {
        if (last == 0) return 0;
        auto accumulated = kernel_.identity();
        return with_root([&]<unsigned Level> {
            return search<false, Level>(0, 0, last, kernel_.identity_action(), accumulated, predicate);
        });
    }
    template<class Output>
    Output materialize(Output output) const {
        if (!empty()) with_root([&]<unsigned Level> {
            write_values<Level>(0, kernel_.identity_action(), output);
        });
        return output;
    }
    [[nodiscard]] std::vector<value_type> snapshot() const {
        std::vector<value_type> result;
        result.reserve(size());
        materialize(std::back_inserter(result));
        return result;
    }
    template<std::ranges::input_range R>
    void rebuild(R&& values) requires(std::is_nothrow_swappable_v<Kernel>) {
        wide_engine next{std::forward<R>(values), kernel_, get_allocator()};
        swap(next);
    }
    void prefetch(std::size_t first, std::size_t last) const noexcept {
        if (first == last) return;
#if defined(__GNUC__) || defined(__clang__)
        const auto prefetch_block = [&](const auto& block) {
            if constexpr (requires { kernel_.prefetch(block); }) kernel_.prefetch(block);
            else __builtin_prefetch(&block, 0, 3);
        };
        const auto l = first / B, r = (last - 1) / B;
        prefetch_block(storage_.leaf(l));
        prefetch_block(storage_.leaf(r));
        if (height() > 1) {
            prefetch_block(storage_.branch(1, l / B));
            prefetch_block(storage_.branch(1, r / B));
        }
#endif
    }
};

} // namespace canard::detail

// ===== include/canard/range/maintained_reference.hpp =====

#include <cstddef>
#include <concepts>
#include <format>
#include <string_view>
#include <utility>

namespace canard {

// A copied proxy copies the handle. Assigning a proxy writes a VALUE, never
// silently rebinds a handle. Snapshot the RHS before starting maintenance.
template<class Owner>
class point_reference {
    Owner* owner_;
    std::size_t position_;
public:
    using value_type = typename Owner::value_type;
    point_reference(Owner& owner, std::size_t position) noexcept : owner_(&owner), position_(position) {}
    point_reference(const point_reference&) noexcept = default;
    [[nodiscard]] value_type value() const { return owner_->get(position_); }
    operator value_type() const { return value(); }
    const point_reference& operator=(const value_type& replacement) const noexcept
        requires(Owner::nothrow_mutation) {
        owner_->set(position_, replacement); return *this;
    }
    const point_reference& operator=(const point_reference& other) const
        requires(Owner::nothrow_mutation) {
        const auto replacement = other.value();
        owner_->set(position_, replacement); return *this;
    }
    template<class Other>
    const point_reference& operator=(const point_reference<Other>& other) const
        requires(Owner::nothrow_mutation && std::same_as<value_type, typename Other::value_type>) {
        const auto replacement = other.value();
        owner_->set(position_, replacement); return *this;
    }
    template<class Tag>
    void apply(const Tag& action) const
        requires requires(Owner& owner) { owner.apply(std::size_t{}, std::size_t{}, action); } {
        owner_->apply(position_, position_ + 1, action);
    }
};

template<class Owner>
class interval_reference {
    Owner* owner_;
    std::size_t first_, last_;
public:
    interval_reference(Owner& owner, std::size_t first, std::size_t last) noexcept
        : owner_(&owner), first_(first), last_(last) {}
    [[nodiscard]] auto fold() const { return owner_->fold(first_, last_); }
    template<class Tag>
    void apply(const Tag& action) const
        requires requires(Owner& owner) { owner.apply(std::size_t{}, std::size_t{}, action); } {
        owner_->apply(first_, last_, action);
    }
};

struct wide_tree_description {
    std::size_t size;
    unsigned fanout;
    unsigned height;
    std::size_t storage_bytes;
    bool lazy;
};

} // namespace canard

// One read per format operation. Numeric presentation comes from the value's
// own formatter. Public facades include this header, not an optional plugin.
template<class Owner>
    requires std::formattable<typename Owner::value_type, char>
struct std::formatter<canard::point_reference<Owner>, char>
    : std::formatter<typename Owner::value_type, char> {
    template<class Context>
    auto format(const canard::point_reference<Owner>& reference, Context& context) const {
        const auto snapshot = reference.value();
        return std::formatter<typename Owner::value_type, char>::format(snapshot, context);
    }
};

template<>
struct std::formatter<canard::wide_tree_description, char> {
    constexpr auto parse(std::format_parse_context& context) { return context.begin(); }
    template<class Context>
    auto format(canard::wide_tree_description x, Context& context) const {
        return std::format_to(context.out(), "{}(size={}, fanout={}, height={}, storage={} bytes)",
            x.lazy ? "wide_lazy_segment_tree" : "wide_segment_tree", x.size, x.fanout,
            x.height, x.storage_bytes);
    }
};
#include <cstddef>
#include <memory>

namespace canard::detail {

template<class Monoid, class Action, class Configuration, class Allocator>
class wide_owner : public wide_engine<
    wide::kernel_for_t<Configuration, Monoid, Action>,
    Configuration, Allocator> {
    using kernel = wide::kernel_for_t<Configuration, Monoid, Action>;
    using engine = wide_engine<kernel, Configuration, Allocator>;
public:
    using typename engine::value_type;
    using size_type = std::size_t;
    using engine::engine;
    wide_owner(const wide_owner&) = default;
    wide_owner(wide_owner&&) = default;
    wide_owner& operator=(const wide_owner&) = default;
    wide_owner& operator=(wide_owner&&) = default;
    wide_owner(const wide_owner& other, const Allocator& allocator) : engine(other, allocator) {}

    [[nodiscard]] point_reference<wide_owner> operator[](size_type position) & noexcept {
        return {*this, position};
    }
    [[nodiscard]] value_type operator[](size_type position) const & { return this->get(position); }
    void operator[](size_type) && = delete;
    void operator[](size_type) const && = delete;

    [[nodiscard]] interval_reference<wide_owner> operator[](size_type first, size_type last) & noexcept {
        return {*this, first, last};
    }
    void operator[](size_type, size_type) && = delete;
    void operator[](size_type, size_type) const && = delete;

    [[nodiscard]] wide_tree_description description() const noexcept {
        return {this->size(), engine::fanout, this->height(), this->storage_bytes(), engine::has_lazy};
    }
};

} // namespace canard::detail
#include <memory>
#include <iterator>
#include <ranges>
#include <span>
#include <utility>

namespace canard {

template<algebra::monoid Monoid, class Action, class Configuration = wide::configuration<>,
         class Allocator = std::allocator<std::byte>>
    requires (algebra::action_for<Action, Monoid> &&
              wide::supported_configuration<Configuration, Monoid, Action>)
class wide_lazy_segment_tree : public detail::wide_owner<Monoid, Action, Configuration, Allocator> {
    using base = detail::wide_owner<Monoid, Action, Configuration, Allocator>;
    using kernel = wide::kernel_for_t<Configuration, Monoid, Action>;
public:
    using typename base::value_type;
    using monoid_type = Monoid;
    using action_policy_type = Action;
    using configuration_type = Configuration;

    template<std::ranges::input_range R>
    explicit wide_lazy_segment_tree(R&& values, Monoid monoid = {}, Action action = {},
                                    Configuration = {}, const Allocator& allocator = {})
        : base(std::forward<R>(values), kernel{std::move(monoid), std::move(action)}, allocator) {}
    explicit wide_lazy_segment_tree(Monoid monoid = {}, Action action = {},
                                    Configuration configuration = {}, const Allocator& allocator = {})
        : wide_lazy_segment_tree(std::span<const value_type>{}, std::move(monoid), std::move(action),
                                 configuration, allocator) {}

    template<std::input_iterator Iterator, std::sentinel_for<Iterator> Sentinel>
    explicit wide_lazy_segment_tree(Iterator first, Sentinel last, Monoid monoid = {}, Action action = {},
                                    Configuration configuration = {}, const Allocator& allocator = {})
        : wide_lazy_segment_tree(std::ranges::subrange{first, last}, std::move(monoid), std::move(action),
                                 configuration, allocator) {}

    wide_lazy_segment_tree(const wide_lazy_segment_tree&) = default;
    wide_lazy_segment_tree(wide_lazy_segment_tree&&) = default;
    wide_lazy_segment_tree& operator=(const wide_lazy_segment_tree&) = default;
    wide_lazy_segment_tree& operator=(wide_lazy_segment_tree&&) = default;
    wide_lazy_segment_tree(const wide_lazy_segment_tree& other, const Allocator& allocator) : base(other, allocator) {}
    friend void swap(wide_lazy_segment_tree& a, wide_lazy_segment_tree& b) noexcept
        requires requires(base& x) { x.swap(x); } { a.base::swap(b); }
};

template<std::ranges::input_range R, algebra::monoid M, class A>
wide_lazy_segment_tree(R&&, M, A) -> wide_lazy_segment_tree<M, A>;
template<std::ranges::input_range R, algebra::monoid M, class A, class C>
wide_lazy_segment_tree(R&&, M, A, C) -> wide_lazy_segment_tree<M, A, C>;
template<std::ranges::input_range R, algebra::monoid M, class A, class C, class Alloc>
wide_lazy_segment_tree(R&&, M, A, C, Alloc) -> wide_lazy_segment_tree<M, A, C, Alloc>;


template<std::input_iterator It, std::sentinel_for<It> S, algebra::monoid M, class A>
wide_lazy_segment_tree(It, S, M, A) -> wide_lazy_segment_tree<M, A>;
template<std::input_iterator It, std::sentinel_for<It> S, algebra::monoid M, class A, class C>
wide_lazy_segment_tree(It, S, M, A, C) -> wide_lazy_segment_tree<M, A, C>;

} // namespace canard

// ===== include/canard/algebra/modular.hpp =====

#include <cstdint>
#include <format>
#include <string_view>

namespace canard::algebra {

template<std::uint32_t Modulus>
struct affine_map {
    static_assert(Modulus > 1 && Modulus < (std::uint32_t{1} << 31));
    std::uint32_t multiplier = 1;
    std::uint32_t translation = 0;
    [[nodiscard]] constexpr std::uint32_t operator()(std::uint32_t x) const noexcept {
        return (std::uint64_t(multiplier) * x + translation) % Modulus;
    }
    friend constexpr bool operator==(affine_map, affine_map) = default;
};

template<std::uint32_t Modulus>
struct modular_sum {
    static_assert(Modulus > 1 && Modulus < (std::uint32_t{1} << 31));
    using value_type = std::uint32_t;
    [[nodiscard]] constexpr value_type identity() const noexcept { return 0; }
    [[nodiscard]] constexpr value_type combine(value_type a, value_type b) const noexcept {
        const auto x = a + b;
        return x >= Modulus ? x - Modulus : x;
    }
};

template<std::uint32_t Modulus>
struct affine_composition {
    using value_type = affine_map<Modulus>;
    [[nodiscard]] constexpr value_type identity() const noexcept { return {}; }
    // Array order: combine(left, right) means apply left first, then right.
    [[nodiscard]] constexpr value_type combine(value_type left, value_type right) const noexcept {
        return {std::uint32_t(std::uint64_t(right.multiplier) * left.multiplier % Modulus),
                std::uint32_t((std::uint64_t(right.multiplier) * left.translation +
                               right.translation) % Modulus)};
    }
};

template<std::uint32_t Modulus>
struct affine_on_sum {
    using tag_type = affine_map<Modulus>;
    [[nodiscard]] constexpr tag_type identity() const noexcept { return {}; }
    [[nodiscard]] constexpr tag_type compose(tag_type newer, tag_type older) const noexcept {
        return affine_composition<Modulus>{}.combine(older, newer);
    }
    [[nodiscard]] constexpr std::uint32_t map(tag_type f, std::uint32_t x,
                                             std::size_t count) const noexcept {
        return (std::uint64_t(f.multiplier) * x +
                std::uint64_t(f.translation) * (count % Modulus)) % Modulus;
    }
};

} // namespace canard::algebra

// The public scalar's formatter is exposed by its own header. Arithmetic-only
// internal kernels do not include this header or formatting/transport support.
template<std::uint32_t P>
struct std::formatter<canard::algebra::affine_map<P>, char> : std::formatter<std::string_view, char> {
    template<class Context>
    auto format(canard::algebra::affine_map<P> f, Context& context) const {
        const auto text = std::format("({}x + {})", f.multiplier, f.translation);
        return std::formatter<std::string_view, char>::format(text, context);
    }
};

// ===== include/canard/wide/avx2.hpp =====


// ===== include/canard/kernel/wide_affine_sum_avx2.hpp =====


// ===== include/canard/kernel/affine_sum_arithmetic_avx2.hpp =====


// ===== include/canard/numeric/montgomery32_avx2.hpp =====

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
namespace detail { struct native_access; }
class u32x8 {
    __m256i bits_;
    explicit u32x8(__m256i bits) noexcept : bits_(bits) {}
    friend struct detail::native_access;
public:
    using value_type = std::uint32_t;
    using mask_type = mask32x8;
    static constexpr int size() noexcept { return 8; }
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
    [[nodiscard]] u32x8 vector() const noexcept { return value_; }
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
    static __m256i get(u32x8 v) noexcept { return v.bits_; }
    static __m256i get(mask32x8 v) noexcept { return v.bits_; }
    static u32x8 words(__m256i v) noexcept { return u32x8{v}; }
    static mask32x8 mask(__m256i v) noexcept { return mask32x8{v}; }
};
} // namespace detail
static_assert(sizeof(u32x8) == 32 && alignof(u32x8) == 32);
static_assert(sizeof(mask32x8) == 32);
static_assert(std::is_trivially_copyable_v<u32x8>);

// Full-width operations. Alignment and eight readable/writable words are
// preconditions. select() does NOT make an earlier memory access conditional.
[[nodiscard]] inline u32x8 load_aligned(const std::uint32_t* address) noexcept {
    return detail::native_access::words(_mm256_load_si256(
        reinterpret_cast<const __m256i*>(address)));
}
inline void store_aligned(std::uint32_t* address, u32x8 value) noexcept {
    _mm256_store_si256(reinterpret_cast<__m256i*>(address), detail::native_access::get(value));
}
[[nodiscard]] inline u32x8 select(mask32x8 mask, u32x8 when_true, u32x8 when_false) noexcept {
    using access = detail::native_access;
    return access::words(_mm256_blendv_epi8(access::get(when_false),
                                          access::get(when_true), access::get(mask)));
}
// Lane k is selected iff k >= first. Negative first selects every lane;
// first >= 8 selects none. Precondition: first > INT_MIN.
[[nodiscard]] inline mask32x8 lanes_from(int first) noexcept {
    return detail::native_access::mask(_mm256_cmpgt_epi32(
        _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7), _mm256_set1_epi32(first - 1)));
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

// ===== include/canard/numeric/montgomery32.hpp =====
#include <cstdint>
#include <limits>
namespace canard::numeric {
// Expert arithmetic policy. Words are deliberately raw: encode/decode mark
// representation changes; operators below preserve lazy residues in [0,2p).
// No primality assumption is needed for arithmetic or power().
template<std::uint32_t Modulus>
struct montgomery32 {
    using word_type = std::uint32_t;
    using u32 = word_type;
    using u64 = std::uint64_t;
    static constexpr u32 modulus = Modulus;
    static_assert(Modulus > 1 && (Modulus & 1) && Modulus < (u32{1} << 30),
                  "Requires odd 1 < modulus < 2^30.");
    static constexpr u32 twice_modulus = 2 * modulus;
    static constexpr u32 negative_inverse = [] consteval {
        u32 inverse = 1;
        for (int step = 0; step < 5; ++step) {
            inverse *= 2 - modulus * inverse;
        }
        return -inverse;
    }();
    static constexpr u32 one = (u64{1} << 32) % modulus;
    static constexpr u32 radix_squared = u64{one} * one % modulus;
    static_assert(4 * u64{modulus} < (u64{1} << 32));
    // The fused reduction below also needs its 64-bit numerator not to overflow.
    static_assert(8 * u64{modulus} * modulus + u64{modulus} * 0xffff'ffffu
                  < std::numeric_limits<u64>::max());

    // REDC primitive: both words lie in [0, 2*modulus). Usually both are
    // encoded, producing an encoded result. One encoded and one ordinary
    // operand deliberately produce an ordinary result (encoded scaling).
    [[nodiscard]] static constexpr u32 multiply(u32 a, u32 b) noexcept {
        const u64 product = u64{a} * b;
        const u32 correction = static_cast<u32>(product) * negative_inverse;
        return static_cast<u32>((product + u64{correction} * modulus) >> 32);
    }

    [[nodiscard]] static constexpr u32 add(u32 a, u32 b) noexcept {
        const u32 sum = a + b;
        return sum >= twice_modulus ? sum - twice_modulus : sum;
    }

    [[nodiscard]] static constexpr u32 subtract(u32 a, u32 b) noexcept {
        return a >= b ? a - b : a + twice_modulus - b;
    }

    // Reduce a*b+c*d together. Both products must have the same encoding
    // degree: either encoded*encoded or encoded*ordinary for both terms.
    // The intermediate result is <3p; subtracting 2p
    // when necessary restores the [0,2p) invariant.
    [[nodiscard]] static constexpr u32 multiply_sum(u32 a, u32 b, u32 c, u32 d) noexcept {
        const u64 sum = u64{a} * b + u64{c} * d;
        const u32 correction = static_cast<u32>(sum) * negative_inverse;
        const u32 result = static_cast<u32>((sum + u64{correction} * modulus) >> 32);
        return result >= twice_modulus ? result - twice_modulus : result;
    }

    [[nodiscard]] static constexpr u32 encode(u32 value) noexcept {
        return multiply(value, radix_squared);
    }

    [[nodiscard]] static constexpr u32 decode(u32 value) noexcept {
        const u32 result = multiply(value, 1);
        return result >= modulus ? result - modulus : result;
    }

    [[nodiscard]] static constexpr u32 power(u32 base, u32 exponent) noexcept {
        u32 result = one;
        for (; exponent != 0; exponent >>= 1, base = multiply(base, base)) {
            if ((exponent & 1u) != 0) {
                result = multiply(result, base);
            }
        }
        return result;
    }

};
// Primality is checked at compile time only by algorithms requiring inversion.
[[nodiscard]] consteval bool is_prime32(std::uint32_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (std::uint32_t d = 3; std::uint64_t{d} * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

// Optional public scalar value type; exactly one word. The hot storage kernels
// use the policy above directly rather than aliasing arrays of these objects.
template<std::uint32_t Modulus>
class montgomery_value {
    using arithmetic = montgomery32<Modulus>;
    std::uint32_t encoded_;
    struct encoded_tag {};
    constexpr montgomery_value(encoded_tag, std::uint32_t x) noexcept : encoded_(x) {}
public:
    // Precondition: canonical < Modulus. No run-time checks or division here.
    explicit constexpr montgomery_value(std::uint32_t canonical = 0) noexcept
        : encoded_(arithmetic::encode(canonical)) {}
    [[nodiscard]] constexpr std::uint32_t value() const noexcept {
        return arithmetic::decode(encoded_);
    }
    friend constexpr montgomery_value operator+(montgomery_value a, montgomery_value b) noexcept {
        return {encoded_tag{}, arithmetic::add(a.encoded_, b.encoded_)};
    }
    friend constexpr montgomery_value operator-(montgomery_value a, montgomery_value b) noexcept {
        return {encoded_tag{}, arithmetic::subtract(a.encoded_, b.encoded_)};
    }
    friend constexpr montgomery_value operator*(montgomery_value a, montgomery_value b) noexcept {
        return {encoded_tag{}, arithmetic::multiply(a.encoded_, b.encoded_)};
    }
    friend constexpr bool operator==(montgomery_value a, montgomery_value b) noexcept {
        return a.value() == b.value();
    }
};
} // namespace canard::numeric
namespace canard::numeric {
template<std::uint32_t Modulus>
struct montgomery32_avx2 {
    using scalar = montgomery32<Modulus>;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;
    using pack_type = simd::u32x8;
    static constexpr auto modulus = Modulus;
    static constexpr auto negative_inverse = scalar::negative_inverse;
    // A radix-2^32 reduction needs the complete 64-bit product for each lane.
    // AVX2 multiplies alternate 32-bit lanes, so use separate even/odd streams.
    [[nodiscard]] static inline simd::u32x8 multiply(simd::u32x8 left, simd::u32x8 right) noexcept {
        using access = simd::detail::native_access;
        const auto a = access::get(left), b = access::get(right);
        const auto even = _mm256_mul_epu32(a, b);
        const auto odd = _mm256_mul_epu32(_mm256_srli_epi64(a, 32),
                                         _mm256_srli_epi64(b, 32));
        const auto inverse = _mm256_set1_epi32(std::bit_cast<int>(negative_inverse));
        const auto prime = _mm256_set1_epi32(static_cast<int>(modulus));
        const auto even_correction = _mm256_mul_epu32(even, inverse);
        const auto odd_correction = _mm256_mul_epu32(odd, inverse);
        const auto even_sum = _mm256_add_epi64(
            even, _mm256_mul_epu32(even_correction, prime));
        const auto odd_sum = _mm256_add_epi64(
            odd, _mm256_mul_epu32(odd_correction, prime));
        return access::words(_mm256_blend_epi32(_mm256_srli_epi64(even_sum, 32), odd_sum, 0xaa));
    }

    // The factor is supplied encoded; internally Shoup-style reciprocal
    // reduction uses its canonical ordinary value. Input and output vectors
    // retain their representation (encoded or ordinary). The reciprocal is
    // prepared ONCE for all uses of this multiplier.
    class blended_multiplier;
    class fixed_multiplier {
        friend class blended_multiplier;
        simd::u32x8 factor_;
        simd::broadcast_u32x8 quotient_;
    public:
        explicit fixed_multiplier(u32 encoded) noexcept {
            const u32 value = scalar::decode(encoded);
            factor_ = simd::u32x8{value};
            quotient_ = simd::broadcast_u32x8{static_cast<u32>((u64{value} << 32) / modulus)};
        }
        [[nodiscard]] simd::u32x8 operator()(simd::u32x8 values) const noexcept {
            const auto q = simd::multiply_high(values, quotient_);
            return simd::multiply_low(values, factor_)
                 - simd::multiply_low(q, simd::u32x8{modulus});
        }
    };
    // Each lane chooses one of two already-prepared factors. Multiplication
    // retains its input representation and returns a lazy residue below 2p.
    // No divisions or reciprocal preparations occur inside the vector loop.
    class blended_multiplier {
        simd::u32x8 factor_, quotient_;
    public:
        blended_multiplier(simd::mask32x8 selected, const fixed_multiplier& yes, const fixed_multiplier& no) noexcept
            :factor_(simd::select(selected, yes.factor_, no.factor_)),
             quotient_(simd::select(selected, yes.quotient_.vector(), no.quotient_.vector())) {}
        [[nodiscard]] simd::u32x8 operator()(simd::u32x8 values) const noexcept {
            auto q = simd::multiply_high(values, quotient_);
            return simd::multiply_low(values, factor_) - simd::multiply_low(q, simd::u32x8{modulus});
        }
    };
    [[nodiscard]] static simd::u32x8 add(simd::u32x8 a, simd::u32x8 b) noexcept {
        const auto sum = a + b;
        return simd::min(sum, sum - simd::u32x8{scalar::twice_modulus});
    }
};
} // namespace canard::numeric

// ===== include/canard/simd/reduction_avx2.hpp =====

#include <cstdint>

namespace canard::simd {

// Lane k is selected exactly when first <= k < last. Negative boundaries and
// boundaries beyond lane 7 are permitted. first must be greater than INT_MIN.
[[nodiscard]] inline mask32x8 lanes_between(int first, int last) noexcept {
    using access = detail::native_access;
    return access::mask(_mm256_andnot_si256(
        access::get(lanes_from(last)), access::get(lanes_from(first))));
}

[[nodiscard]] inline u32x8 operator&(u32x8 left, u32x8 right) noexcept {
    using access = detail::native_access;
    return access::words(_mm256_and_si256(access::get(left), access::get(right)));
}

// Exact sum of eight unsigned 32-bit lanes, without 32-bit overflow.
[[nodiscard]] inline std::uint64_t reduce_add_widened(u32x8 values) noexcept {
    const auto raw = detail::native_access::get(values);
    const auto pairs = _mm256_add_epi64(
        _mm256_and_si256(raw, _mm256_set1_epi64x(0xffff'ffffu)),
        _mm256_srli_epi64(raw, 32));
    const auto halves = _mm_add_epi64(
        _mm256_castsi256_si128(pairs), _mm256_extracti128_si256(pairs, 1));
    const auto total = _mm_add_epi64(halves, _mm_srli_si128(halves, 8));
    return static_cast<std::uint64_t>(_mm_cvtsi128_si64(total));
}

} // namespace canard::simd
#include <array>
#include <cstddef>
#include <cstdint>

namespace canard::algebra {

// Input-facing coefficients: x -> multiplier * x + translation, modulo p.
// Both coefficients must be canonical residues, but multiplier may be zero.
struct affine_update {
    std::uint32_t multiplier;
    std::uint32_t translation;
};

// Same action in Montgomery representation. The field is part of the type.
template<class Field>
struct encoded_affine_update {
    using word_type = typename Field::word_type;
    word_type multiplier;
    word_type translation;
};

// Ordinary residues, not encoded ones. Each lane may lie in [0, 2p).
template<std::size_t Fanout>
struct alignas(Fanout >= 16 ? 64 : 32) affine_sum_leaf {
    std::array<std::uint32_t, Fanout> sums;
};

// Each slot summarizes one child. sums[] already includes that slot's lazy
// action; the child's own block does not include it until the action is pushed.
// Lazy coefficients are encoded. Only a clean multiplier has the high bit set.
template<std::size_t Fanout>
struct alignas(Fanout >= 16 ? 64 : 32) affine_sum_branch {
    std::array<std::uint32_t, Fanout> sums;
    std::array<std::uint32_t, Fanout> lazy_multipliers;
    std::array<std::uint32_t, Fanout> lazy_translations;
};

template<class Field, std::size_t Fanout>
    requires(Fanout == 8 || Fanout == 16 || Fanout == 32)
struct affine_sum_kernel {
    using word_type = typename Field::word_type;
    using pack = simd::u32x8;
    using vectors = numeric::montgomery32_avx2<Field::modulus>;
    using leaf_type = affine_sum_leaf<Fanout>;
    using branch_type = affine_sum_branch<Fanout>;
    using action_type = encoded_affine_update<Field>;

    static constexpr word_type clean_bit = word_type{1} << 31;
    static constexpr word_type value_bits = clean_bit - 1;
    static constexpr word_type clean_multiplier = clean_bit | Field::one;

    // The expensive fixed-factor preparation is done once per update (or push),
    // then reused for all sum, multiplier, and translation vectors it touches.
    struct prepared_action {
        action_type action;
        typename vectors::fixed_multiplier scale;

        explicit prepared_action(action_type encoded) noexcept
            : action(encoded), scale(encoded.multiplier) {}
    };

    [[nodiscard]] static word_type reduce(pack values) noexcept {
        return static_cast<word_type>(simd::reduce_add_widened(values) % Field::modulus);
    }

    template<unsigned Active = Fanout>
    [[nodiscard]] static word_type sum_range(
        const word_type* sums, unsigned first, unsigned last) noexcept {
        static_assert(0 < Active && Active <= Fanout);
        pack total{0};
        for (unsigned block = 0; block < Active; block += pack::size()) {
            const auto selected = simd::lanes_between(
                static_cast<int>(first) - static_cast<int>(block),
                static_cast<int>(last) - static_cast<int>(block));
            total = accumulate(total, simd::select(
                selected, simd::load_aligned(sums + block), pack{0}));
        }
        return reduce(total);
    }

    [[nodiscard]] static bool has_pending_action(
        const branch_type& node, unsigned child) noexcept {
        return (node.lazy_multipliers[child] & clean_bit) == 0;
    }

    [[nodiscard]] static action_type pending_action(
        const branch_type& node, unsigned child) noexcept {
        return {node.lazy_multipliers[child] & value_bits,
                node.lazy_translations[child]};
    }

    static void clear_action(branch_type& node, unsigned child) noexcept {
        node.lazy_multipliers[child] = clean_multiplier;
        node.lazy_translations[child] = 0;
    }

    // REDC((bR)*ordinary_sum + (cR)*ordinary_length) is an ordinary sum.
    // Mixing representations here is intentional: neither the sum nor the
    // length needs encoding, and there is no decoding at each query level.
    [[nodiscard]] static word_type apply_to_sum(
        action_type action, word_type ordinary_sum, unsigned length) noexcept {
        return Field::multiply_sum(
            action.multiplier, ordinary_sum, action.translation, length);
    }

    template<bool ReturnDelta, unsigned Active = Fanout>
    [[gnu::always_inline]] static inline word_type apply(leaf_type& node, unsigned first, unsigned last,
                           const prepared_action& update) noexcept {
        return transform<false, ReturnDelta, Active>(node.sums.data(), nullptr, nullptr,
                                              first, last, update, 1);
    }

    template<bool ReturnDelta, unsigned Active = Fanout>
    [[gnu::always_inline]] static inline word_type apply(branch_type& node, unsigned first, unsigned last,
                           const prepared_action& update, unsigned child_length) noexcept {
        return transform<true, ReturnDelta, Active>(
            node.sums.data(), node.lazy_multipliers.data(), node.lazy_translations.data(),
            first, last, update, child_length);
    }

    // Materialize the incoming action everywhere and the new action only on
    // [first, last), using their composition for those lanes. Return only the
    // new action's delta, because the parent already includes the incoming one.
    template<bool MaintainTags>
    [[gnu::always_inline]] static inline word_type transform_carried(
        word_type* sums, word_type* multipliers, word_type* translations,
        unsigned first, unsigned last, const prepared_action& update,
        action_type incoming, unsigned child_length) noexcept {
        const auto [b, c] = update.action;
        const prepared_action before{incoming};
        if constexpr (MaintainTags) {
            if (first == last) {
                return transform<true, false>(sums, multipliers, translations,0, Fanout, before, child_length);
            }
        }
        const prepared_action after{action_type{
            Field::multiply(b, incoming.multiplier),
            Field::add(Field::multiply(b, incoming.translation), c)}};
        const pack old_offset{Field::multiply(incoming.translation, child_length)};
        const pack new_offset{Field::multiply(after.action.translation, child_length)};
        pack previous_total{0};
        for (unsigned block = 0; block<Fanout; block += pack::size()) {
            const auto selected = simd::lanes_between(int(first)-int(block), int(last)-int(block));
            const typename vectors::blended_multiplier scale{selected, after.scale, before.scale};
            auto previous = simd::load_aligned(sums+block);
            previous_total = accumulate(previous_total, simd::select(selected, previous, pack{0}));
            simd::store_aligned(sums+block, vectors::add(scale(previous), simd::select(selected, new_offset, old_offset)));
            if constexpr (MaintainTags) {
                const auto old_b = simd::load_aligned(multipliers+block) & pack{value_bits};
                const auto old_c = simd::load_aligned(translations+block);
                const auto offset = simd::select(selected, pack{after.action.translation}, pack{incoming.translation});
                simd::store_aligned(multipliers+block, scale(old_b));
                simd::store_aligned(translations+block, vectors::add(scale(old_c), offset));
            }
        }
        const unsigned length = (last-first)*child_length;
        // H = update o incoming. Delta = (H.b-G.b)*S + (H.c-G.c)*L.
        // Reusing these coefficients avoids first reducing G(S,L).
        return apply_to_sum({Field::subtract(after.action.multiplier, incoming.multiplier),
                             Field::subtract(after.action.translation, incoming.translation)},
                            reduce(previous_total), length);
    }

    [[gnu::always_inline]] static inline word_type apply_carried(leaf_type& node,
            unsigned first, unsigned last, const prepared_action& update,
            action_type incoming) noexcept {
        return transform_carried<false>(node.sums.data(), nullptr, nullptr,
            first, last, update, incoming,1);
    }
    [[gnu::always_inline]] static inline word_type apply_carried(branch_type& node,
            unsigned first, unsigned last, const prepared_action& update,
            action_type incoming, unsigned length) noexcept {
        return transform_carried<true>(node.sums.data(), node.lazy_multipliers.data(),
            node.lazy_translations.data(), first, last, update, incoming, length);
    }

private:
    // For B<=16, each lane accumulates at most two values below 2p.
    // Since 4p<2^32, raw addition is exact; reduce only after widening.
    [[nodiscard]] static pack accumulate(pack a, pack b) noexcept {
        if constexpr (Fanout <= 16) return a+b;
        else return vectors::add(a, b);
    }
    // Every memory operation is full-width and aligned. Masks select values,
    // not memory accesses: all Fanout entries must exist even in a padded node.
    template<bool MaintainTags, bool ReturnDelta, unsigned Active = Fanout>
    [[gnu::always_inline]] static inline word_type transform(
        word_type* sums, word_type* multipliers, word_type* translations,
        unsigned first, unsigned last, const prepared_action& update,
        unsigned child_length) noexcept {
        const auto [b, c] = update.action;
        const pack sum_offset{Field::multiply(c, child_length)};
        const pack tag_offset{c};
        pack previous_total{0};

        for (unsigned block = 0; block < Active; block += pack::size()) {
            const auto selected = simd::lanes_between(
                static_cast<int>(first) - static_cast<int>(block),
                static_cast<int>(last) - static_cast<int>(block));
            auto* destination = sums + block;
            const auto previous = simd::load_aligned(destination);
            if constexpr (ReturnDelta) {
                previous_total = accumulate(previous_total,
                    simd::select(selected, previous, pack{0}));
            }
            const auto next = vectors::add(update.scale(previous), sum_offset);
            simd::store_aligned(destination, simd::select(selected, next, previous));

            if constexpr (MaintainTags) {
                auto* scales = multipliers + block;
                auto* shifts = translations + block;
                const auto old_scales = simd::load_aligned(scales);
                const auto old_shifts = simd::load_aligned(shifts);
                const auto next_scales = update.scale(old_scales & pack{value_bits});
                const auto next_shifts = vectors::add(update.scale(old_shifts), tag_offset);
                simd::store_aligned(scales, simd::select(selected, next_scales, old_scales));
                simd::store_aligned(shifts, simd::select(selected, next_shifts, old_shifts));
            }
        }
        if constexpr (ReturnDelta) {
            // New total - old total = (b - 1) * old total + c * covered length.
            return apply_to_sum({Field::subtract(b, Field::one), c},
                                reduce(previous_total), (last - first) * child_length);
        } else {
            return 0;
        }
    }
};

} // namespace canard::algebra
#include <type_traits>

namespace canard::kernel {

// Block adapter, NOT a second tree. Geometry, queries, point replacement,
// lazy chronology, and boundary descent belong to the shared wide_engine.
// Internal sums are ordinary lazy residues; actions are Montgomery-encoded.
template<std::uint32_t Modulus, unsigned Fanout>
    requires (Fanout == 8 || Fanout == 16 || Fanout == 32)
struct wide_affine_sum_avx2 {
    using field = numeric::montgomery32<Modulus>;
    using arithmetic = algebra::affine_sum_kernel<field, Fanout>;
    using value_type = std::uint32_t;
    using summary_type = std::uint32_t;
    using update_type = algebra::affine_map<Modulus>;
    using action_type = typename arithmetic::action_type;
    using prepared_action = typename arithmetic::prepared_action;
    using leaf_type = typename arithmetic::leaf_type;
    using branch_type = typename arithmetic::branch_type;
    using local_change = std::uint32_t;
    using subtree_change = std::uint32_t;
    static constexpr bool has_lazy = true;
    static constexpr bool nothrow_mutation = true;
    static constexpr unsigned fanout = Fanout;
    // Montgomery's mixed-product bound also applies to real segment lengths.
    static constexpr std::size_t max_count = 2ull * Modulus - 1;

    wide_affine_sum_avx2(algebra::modular_sum<Modulus>, algebra::affine_on_sum<Modulus>) noexcept {}

    template<class Block>
    void prefetch(const Block& node) const noexcept {
        __builtin_prefetch(node.sums.data(), 0, 3);
        if constexpr (std::same_as<Block, branch_type>) {
            __builtin_prefetch(node.lazy_multipliers.data(), 0, 3);
            __builtin_prefetch(node.lazy_translations.data(), 0, 3);
        }
    }
    [[nodiscard]] leaf_type empty_leaf() const noexcept { return {}; }
    [[nodiscard]] branch_type empty_branch() const noexcept {
        branch_type node{};
        node.lazy_multipliers.fill(arithmetic::clean_multiplier);
        return node;
    }
    [[nodiscard]] summary_type identity() const noexcept { return 0; }
    [[nodiscard]] summary_type combine(summary_type x, summary_type y) const noexcept {
        return field::add(x, y);
    }
    [[nodiscard]] summary_type import_value(value_type x) const noexcept { return x; }
    [[nodiscard]] value_type export_value(summary_type x) const noexcept {
        return x >= Modulus ? x - Modulus : x;
    }
    template<class Block>
    [[nodiscard]] summary_type slot(const Block& node, unsigned child) const noexcept {
        return node.sums[child];
    }
    template<class Block>
    void write_slot(Block& node, unsigned child, summary_type value) const noexcept { node.sums[child] = value; }
    template<unsigned Active = Fanout, class Block>
    [[nodiscard]] summary_type fold(const Block& node, unsigned first, unsigned last) const noexcept {
        return arithmetic::template sum_range<Active>(node.sums.data(), first, last);
    }
    [[nodiscard]] action_type identity_action() const noexcept { return {field::one, 0}; }
    [[nodiscard]] prepared_action prepare(update_type f) const noexcept {
        return prepared_action{{field::encode(f.multiplier), field::encode(f.translation)}};
    }
    [[nodiscard]] prepared_action prepare_internal(action_type f) const noexcept { return prepared_action{f}; }
    [[nodiscard]] action_type compose(action_type newer, action_type older) const noexcept {
        return {field::multiply(newer.multiplier, older.multiplier),
                field::add(field::multiply(newer.multiplier, older.translation), newer.translation)};
    }
    [[nodiscard]] summary_type map(action_type f, summary_type x, std::size_t count) const noexcept {
        return arithmetic::apply_to_sum(f, x, static_cast<unsigned>(count));
    }
    [[nodiscard]] bool needs_materialization(const branch_type& node, unsigned child) const noexcept {
        return arithmetic::has_pending_action(node, child);
    }
    [[nodiscard]] action_type child_frame(const branch_type& node, unsigned child) const noexcept {
        return arithmetic::pending_action(node, child);
    }
    void clear_frame(branch_type& node, unsigned child) const noexcept { arithmetic::clear_action(node, child); }

    // Additive deltas are justified by modular_sum, NOT a requirement imposed
    // on a user-defined monoid. The portable policy instead returns a subtotal.
    [[nodiscard]] local_change begin_changes() const noexcept { return 0; }
    void account_local(local_change& total, local_change change) const noexcept { total = field::add(total, change); }
    [[nodiscard]] local_change repair_slot(branch_type& node, unsigned child, subtree_change delta) const noexcept {
        node.sums[child] = field::add(node.sums[child], delta);
        return delta;
    }
    template<bool ReturnChange, class Block>
    [[nodiscard]] subtree_change finish(const Block&, local_change changes) const noexcept {
        if constexpr (ReturnChange) return changes;
        else return 0;
    }
    template<bool ReturnChange, unsigned Active = Fanout, class Block>
    local_change apply_slots(Block& node, unsigned first, unsigned last,
                             const prepared_action& update, std::size_t count) const noexcept {
        if constexpr (std::same_as<Block, leaf_type>)
            return arithmetic::template apply<ReturnChange, Active>(node, first, last, update);
        else return arithmetic::template apply<ReturnChange, Active>(
            node, first, last, update, static_cast<unsigned>(count));
    }
    template<class Block>
    local_change apply_carried(Block& node, unsigned first, unsigned last,
                               const prepared_action& update, action_type incoming,
                               std::size_t count) const noexcept {
        if constexpr (std::same_as<Block, leaf_type>)
            return arithmetic::apply_carried(node, first, last, update, incoming);
        else return arithmetic::apply_carried(node, first, last, update, incoming, static_cast<unsigned>(count));
    }
    [[nodiscard]] subtree_change replace(leaf_type& node, unsigned child, summary_type value) const noexcept {
        const auto delta = field::subtract(value, node.sums[child]);
        node.sums[child] = value;
        return delta;
    }
};

} // namespace canard::kernel

// ===== include/canard/kernel/wide_minimum_avx2.hpp =====


// ===== include/canard/algebra/basic.hpp =====

#include <algorithm>
#include <concepts>
#include <limits>

namespace canard::algebra {

template<std::integral T>
struct sum {
    using value_type = T;
    [[nodiscard]] constexpr T identity() const noexcept { return T{0}; }
    [[nodiscard]] constexpr T combine(T a, T b) const noexcept { return a + b; }
};

template<std::integral T>
struct minimum {
    using value_type = T;
    [[nodiscard]] constexpr T identity() const noexcept { return std::numeric_limits<T>::max(); }
    [[nodiscard]] constexpr T combine(T a, T b) const noexcept { return std::min(a, b); }
};

template<std::integral T>
struct maximum {
    using value_type = T;
    [[nodiscard]] constexpr T identity() const noexcept { return std::numeric_limits<T>::lowest(); }
    [[nodiscard]] constexpr T combine(T a, T b) const noexcept { return std::max(a, b); }
};

template<std::integral T>
struct add_to_sum {
    using tag_type = T;
    [[nodiscard]] constexpr T identity() const noexcept { return T{0}; }
    // Newer is applied AFTER older. Addition happens to commute; others do not.
    [[nodiscard]] constexpr T compose(T newer, T older) const noexcept { return newer + older; }
    [[nodiscard]] constexpr T map(T f, T x, std::size_t count) const noexcept {
        return x + f * static_cast<T>(count);
    }
};

template<std::integral T>
struct add_to_extremum {
    using tag_type = T;
    [[nodiscard]] constexpr T identity() const noexcept { return T{0}; }
    [[nodiscard]] constexpr T compose(T newer, T older) const noexcept { return newer + older; }
    [[nodiscard]] constexpr T map(T f, T x, std::size_t count) const noexcept {
        // Length distinguishes a real INT_MAX/INT_MIN leaf from an empty span.
        return count == 0 ? x : x + f;
    }
};

} // namespace canard::algebra

// ===== include/canard/simd/minimum_avx2.hpp =====

// ===== include/canard/simd/static_for.hpp =====
#include <cstddef>
#include <type_traits>
#include <utility>

namespace canard::simd {

namespace detail {
template<std::size_t... I, class F>
inline void static_for_impl(std::index_sequence<I...>, F&& function) {
    (function(std::integral_constant<std::size_t, I>{}), ...);
}
} // namespace detail

template<std::size_t N, class F>
inline void static_for(F&& function) {
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

template<class T>
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
template<std::size_t Extent>
    requires (Extent >= 4 && Extent % 4 == 0 && Extent <= 64)
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
template<class T> struct minimum_register;
template<> struct minimum_register<double> { using type = __m256d; };
template<> struct minimum_register<std::int64_t> { using type = __m256i; };
} // namespace detail

// A concrete four-lane backend, not an emulation of the entire std::simd API.
// The tree uses the floating specialization only for exactly representable
// integers. This backend itself has ordinary floating-point semantics.
template<minimum_scalar T>
class minimum_pack {
    using native_type = typename detail::minimum_register<T>::type;
    native_type bits_;

    explicit minimum_pack(native_type value) noexcept : bits_(value) {}

public:
    using value_type = T;
    static constexpr std::size_t size() noexcept { return 4; }
    static constexpr std::size_t alignment = 32;

    // Default construction leaves the register uninitialized.
    minimum_pack() = default;
    explicit minimum_pack(T value) noexcept {
        if constexpr (std::same_as<T, double>) bits_ = _mm256_set1_pd(value);
        else bits_ = _mm256_set1_epi64x(value);
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
        if constexpr (std::same_as<T, double>) _mm256_store_pd(address, bits_);
        else _mm256_store_si256(reinterpret_cast<__m256i*>(address), bits_);
    }

    friend minimum_pack operator+(minimum_pack a, minimum_pack b) noexcept {
        if constexpr (std::same_as<T, double>) return minimum_pack{_mm256_add_pd(a.bits_, b.bits_)};
        else return minimum_pack{_mm256_add_epi64(a.bits_, b.bits_)};
    }

    friend minimum_pack operator-(minimum_pack a, minimum_pack b) noexcept {
        if constexpr (std::same_as<T, double>) return minimum_pack{_mm256_sub_pd(a.bits_, b.bits_)};
        else return minimum_pack{_mm256_sub_epi64(a.bits_, b.bits_)};
    }

    friend mask64x4 operator==(minimum_pack a, minimum_pack b) noexcept {
        if constexpr (std::same_as<T, double>)
            return {_mm256_castpd_si256(_mm256_cmp_pd(a.bits_, b.bits_, _CMP_EQ_OQ))};
        else return {_mm256_cmpeq_epi64(a.bits_, b.bits_)};
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
            return minimum_pack{_mm256_blendv_pd(b.bits_, a.bits_,
                                                _mm256_castsi256_pd(selected.bits))};
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
            const auto pairs = _mm_min_pd(_mm256_castpd256_pd128(bits_),
                                         _mm256_extractf128_pd(bits_, 1));
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
#include <limits>

namespace canard::kernel {

namespace detail {
alignas(32) inline constexpr auto minimum_masks = [] {
    std::array<std::array<std::uint64_t, 4>, 16> table{};
    for (unsigned mask = 0; mask < 16; ++mask)
        for (unsigned lane = 0; lane < 4; ++lane)
            table[mask][lane] = (mask >> lane) & 1 ? ~std::uint64_t{0} : 0;
    return table;
}();
inline simd::mask64x4 minimum_interval(unsigned first, unsigned last, unsigned block) noexcept {
    const auto selected = (std::uint64_t{1} << last) - (std::uint64_t{1} << first);
    return simd::load_mask(minimum_masks[(selected >> block) & 15].data());
}
} // namespace detail

// Same ordinary-summary invariant as wide_scalar, but packed block operations.
// No doubles, sentinels-as-absence flags, normalization, or inverse actions.
// Padding is selected out, and real INT64_MAX leaves remain genuine values.
template<class Action, unsigned Fanout>
    requires ((Fanout == 8 || Fanout == 16 || Fanout == 32) &&
              (std::same_as<Action, algebra::no_action> ||
               std::same_as<Action, algebra::add_to_extremum<std::int64_t>>))
struct wide_minimum_avx2 : wide_scalar<algebra::minimum<std::int64_t>, Action, Fanout> {
    using base = wide_scalar<algebra::minimum<std::int64_t>, Action, Fanout>;
    using base::has_lazy;
    using typename base::leaf_type;
    using typename base::branch_type;
    using typename base::summary_type;
    using typename base::action_type;
    using typename base::prepared_action;
    using typename base::local_change;
    using typename base::subtree_change;
    using pack = simd::i64x4;

    wide_minimum_avx2(algebra::minimum<std::int64_t> monoid, Action action) noexcept : base{monoid, action} {}

    template<unsigned Active = Fanout, class Block>
    [[nodiscard]] summary_type fold(const Block& node, unsigned first, unsigned last) const noexcept {
        const pack infinity{std::numeric_limits<std::int64_t>::max()};
        auto answer = infinity;
        for (unsigned block = 0; block < Active; block += 4) {
            const auto selected = detail::minimum_interval(first, last, block);
            answer = min(answer, select(selected, pack::load(node.values.data() + block), infinity));
        }
        return answer.reduce_min();
    }
    template<bool ReturnChange, class Block>
    [[nodiscard]] subtree_change finish(const Block& node, local_change) const noexcept {
        if constexpr (ReturnChange) return {fold(node, 0, Fanout)};
        else return {this->identity()};
    }
    [[nodiscard]] subtree_change replace(leaf_type& node, unsigned child, summary_type value) const noexcept {
        node.values[child] = value;
        return finish<true>(node, {});
    }
    template<bool ReturnChange, unsigned Active = Fanout, class Block>
    local_change apply_slots(Block& node, unsigned first, unsigned last,
                             const prepared_action& update, std::size_t) const noexcept requires(has_lazy) {
        // Add zero to unselected lanes, including infinity padding. This avoids
        // even intermediate mathematical overflow on inaccessible padding.
        for (unsigned block = 0; block < Active; block += 4) {
            const auto selected = detail::minimum_interval(first, last, block);
            const auto delta = mask_zero(selected, pack{update});
            (pack::load(node.values.data() + block) + delta).store(node.values.data() + block);
            if constexpr (std::same_as<Block, branch_type>) {
                (pack::load(node.lazy.tags.data() + block) + delta).store(node.lazy.tags.data() + block);
            }
        }
        if constexpr (std::same_as<Block, branch_type>)
            node.lazy.dirty |= ((std::uint64_t{1} << last) - 1) ^ ((std::uint64_t{1} << first) - 1);
        return {};
    }
    template<class Block>
    local_change apply_carried(Block& node, unsigned first, unsigned last,
                               const prepared_action& update, const action_type& incoming,
                               std::size_t) const noexcept requires(has_lazy) {
        const pack before{incoming}, after{incoming + update};
        for (unsigned block = 0; block < Fanout; block += 4) {
            const auto selected = detail::minimum_interval(first, last, block);
            const auto delta = select(selected, after, before);
            (pack::load(node.values.data() + block) + delta).store(node.values.data() + block);
            if constexpr (std::same_as<Block, branch_type>)
                (pack::load(node.lazy.tags.data() + block) + delta).store(node.lazy.tags.data() + block);
        }
        if constexpr (std::same_as<Block, branch_type>) node.lazy.dirty = (std::uint64_t{1} << Fanout) - 1;
        return {};
    }
};

} // namespace canard::kernel

// ===== include/canard/kernel/wide_normalized_minimum_avx2.hpp =====

// ===== include/canard/kernel/wide_normalized_extremum.hpp =====

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstddef>
#include <utility>
#include <limits>

namespace canard::kernel {

// Translation-equivariant minimum in local coordinates. This is a block
// representation using the SAME traversal, not a copied normalized-min tree.
//
// Restricted numeric domain: throughout the operation history, real values,
// value differences, update sums, and temporary frame changes must have
// magnitude < 2^60. The reserved padding value 2^62 is never a real value.
// No floating representation is selected implicitly.
template<class Action, unsigned Fanout, class Arithmetic>
    requires ((Fanout == 8 || Fanout == 16 || Fanout == 32) &&
              (std::same_as<Action, algebra::no_action> ||
               std::same_as<Action, algebra::add_to_extremum<std::int64_t>>))
class wide_normalized_minimum {
public:
    using value_type = std::int64_t;
    using summary_type = std::int64_t;
    using update_type = typename Action::tag_type;
    using action_type = std::int64_t; // A parent edge is a change of coordinates.
    using prepared_action = std::int64_t;
    using subtree_change = std::int64_t; // Change in this subtree's minimum.
    // A repair certificate: no change, one edited slot, several edits, or an
    // already-normalized block. The engine never inspects these alternatives.
    struct local_change {
        bool touched = false;
        bool single = false;
        bool normalized = false;
        summary_type before = 0, after = 0, delta = 0;
    };
    struct alignas(64) leaf_type { std::array<std::int64_t, Fanout> values; };
    using branch_type = leaf_type; // No tags: a block is only its child gaps.
    using pack = typename Arithmetic::pack;
    static_assert(pack::size() == 4);
    static constexpr bool has_lazy = !std::same_as<Action, algebra::no_action>;
    static constexpr bool needs_lift = true;
    static constexpr bool upward_updates = true;
    static constexpr bool nothrow_mutation = true;
    static constexpr unsigned fanout = Fanout;
    static constexpr summary_type infinity = summary_type{1} << 62;
private:
    value_type root_minimum_ = 0;

    void subtract_frame(leaf_type& node, summary_type change) const noexcept {
        if (change == 0) return;
        const pack shift{change}, absent{infinity};
        simd::static_for<Fanout / 4>([&](auto chunk) {
            auto* destination = node.values.data() + 4 * chunk;
            const auto old = pack::load(destination);
            (old - select(old == absent, pack{0}, shift)).store(destination);
        });
    }
    [[nodiscard]] summary_type normalize(leaf_type& node) const noexcept {
        const auto change = fold(node, 0, Fanout);
        subtract_frame(node, change);
        return change;
    }
public:
    wide_normalized_minimum(algebra::minimum<value_type>, Action) noexcept {}
    void reset_build() noexcept { root_minimum_ = 0; }
    [[nodiscard]] leaf_type empty_leaf() const noexcept { leaf_type x; x.values.fill(infinity); return x; }
    [[nodiscard]] branch_type empty_branch() const noexcept { return empty_leaf(); }
    [[nodiscard]] summary_type identity() const noexcept { return infinity; }
    // After each public mutation the root is normalized, so its summary in
    // local coordinates is known without scanning any child slots.
    [[nodiscard]] summary_type root_summary() const noexcept { return 0; }
    [[nodiscard]] summary_type combine(summary_type x, summary_type y) const noexcept { return std::min(x,y); }
    [[nodiscard]] summary_type import_value(value_type x) const noexcept { return x - root_minimum_; }
    [[nodiscard]] value_type export_value(summary_type x) const noexcept {
        return x == infinity ? std::numeric_limits<value_type>::max() : x + root_minimum_;
    }
    [[nodiscard]] summary_type slot(const leaf_type& n, unsigned child) const noexcept { return n.values[child]; }
    void write_slot(leaf_type& n, unsigned child, summary_type x) const noexcept { n.values[child] = x; }
    template<unsigned Active = Fanout>
    [[nodiscard]] summary_type fold(const leaf_type& n, unsigned first, unsigned last) const noexcept {
        const pack absent{infinity}; auto best=absent;
        const typename Arithmetic::template interval<Fanout> selected{first, last};
        simd::static_for<(Active+3)/4>([&](auto chunk) {
            constexpr unsigned block = 4 * chunk;
            best = min(best, select(selected.chunk(chunk),
                                   pack::load(n.values.data()+block), absent));
        });
        return best.reduce_min();
    }
    [[nodiscard]] summary_type build_summary(leaf_type& n, bool root) noexcept {
        const auto minimum = normalize(n);
        if (root) root_minimum_ = minimum;
        return minimum;
    }
    [[nodiscard]] action_type identity_action() const noexcept { return 0; }
    [[nodiscard]] prepared_action prepare(const update_type& f) const noexcept requires(has_lazy) { return f; }
    [[nodiscard]] prepared_action prepare_internal(action_type f) const noexcept { return f; }
    [[nodiscard]] action_type compose(action_type newer, action_type older) const noexcept { return newer + older; }
    [[nodiscard]] summary_type map(action_type frame, summary_type x, std::size_t count) const noexcept {
        return count == 0 || x == infinity ? infinity : x + frame;
    }
    // Shared boundary folding only lifts nonempty, already-combined pieces.
    // Such a summary is finite in this representation, including when its
    // numeric value is zero. Empty folds still use map() above.
    [[nodiscard]] summary_type map_nonempty(action_type frame, summary_type x,
                                             std::size_t) const noexcept {
        return x + frame;
    }
    // Edge gaps have interpretation, but are NEVER a child_frame action to push.
    [[nodiscard]] bool needs_materialization(const branch_type&, unsigned) const noexcept { return false; }
    [[nodiscard]] action_type child_frame(const branch_type& n, unsigned child) const noexcept { return n.values[child]; }
    void clear_frame(branch_type&, unsigned) const noexcept {}
    [[nodiscard]] summary_type descend_value(const branch_type& n, unsigned child, summary_type x) const noexcept {
        return x - n.values[child];
    }
    void commit_root(subtree_change delta) noexcept { root_minimum_ += delta; }
    [[nodiscard]] bool change_is_identity(subtree_change delta) const noexcept { return delta == 0; }
    void apply_all(prepared_action f) noexcept { root_minimum_ += f; }

    [[nodiscard]] local_change begin_changes() const noexcept { return {}; }
    void account_local(local_change& current, local_change next) const noexcept {
        if (!next.touched) return;
        if (!current.touched || next.normalized) current = next;
        else { current.single = false; current.touched = true; }
    }
    [[nodiscard]] local_change repair_slot(branch_type& node, unsigned child, subtree_change delta) const noexcept {
        if (delta == 0) return {};
        const auto before = node.values[child];
        node.values[child] += delta;
        return {.touched=true, .single=true, .before=before, .after=node.values[child]};
    }
    // The one-edge case has no run-time repair alternatives. Keeping its
    // scalar result separate avoids returning/merging a multi-state record.
    [[nodiscard]] subtree_change repair_one(leaf_type& node, unsigned child,
                                            subtree_change delta) const noexcept {
        if (delta == 0) return 0;
        const auto before = node.values[child];
        const auto after = before + delta;
        node.values[child] = after;
        if (before != 0 && after >= 0) return 0;
        const auto change = after <= 0 ? after : fold(node, 0, Fanout);
        subtract_frame(node, change);
        return change;
    }
    template<bool ReturnChange>
    [[nodiscard]] subtree_change finish(leaf_type& node, local_change changes) noexcept {
        if (!changes.touched) return 0;
        const auto delta = [&] {
            if (changes.normalized) return changes.delta;
            // Other children still have nonnegative gaps. These facts avoid a
            // horizontal scan when only one changed child can affect the min.
            if (changes.single && changes.before != 0 && changes.after >= 0) return summary_type{0};
            if (changes.single && changes.after <= 0) {
                subtract_frame(node, changes.after);
                return changes.after;
            }
            return normalize(node);
        }();
        if constexpr (!ReturnChange) root_minimum_ += delta;
        return delta;
    }
private:
    template<unsigned Active = Fanout, bool Dense = false>
    [[nodiscard]] summary_type transform_and_normalize(leaf_type& node,
            unsigned first, unsigned last, prepared_action increment) const noexcept {
        constexpr auto chunks = (Active + 3) / 4;
        std::array<pack, chunks> values;
        const pack absent{infinity}; auto least=absent;
        const typename Arithmetic::template interval<Fanout> interval{first, last};
        simd::static_for<chunks>([&](auto chunk) {
            values[chunk] = pack::load(node.values.data()+4*chunk)
                         + mask_zero(interval.chunk(chunk),pack{increment});
            least = min(least,values[chunk]);
        });
        const auto change = least.reduce_min();
        simd::static_for<chunks>([&](auto chunk) {
            // Padding is an exact identity, not a drifting finite sentinel.
            const auto shift = [&] {
                if constexpr (Dense) return pack{change};
                else return select(values[chunk] == absent, pack{0}, pack{change});
            }();
            (values[chunk] - shift).store(node.values.data()+4*chunk);
        });
        return change;
    }
public:
    // Atomic block transaction for the shared upward scheduler. Boundary
    // repairs are disjoint from [first,last); all refer to real child slots.
    template<unsigned Active = Fanout, std::size_t Edits>
    [[nodiscard]] subtree_change edit_block(leaf_type& node,
            const block_edit<subtree_change, Edits>& edit, prepared_action increment,
            unsigned valid_slots) const noexcept {
        const auto first = edit.first, last = edit.last;
        if constexpr (Edits == 0) {
            if (last == first + 1)
                return repair_one(node, first, increment);
        }
        for (const auto& boundary : edit.boundaries) node.values[boundary.slot] += boundary.change;
        if (valid_slots == Fanout)
            return transform_and_normalize<Active, true>(node, first, last, increment);
        return transform_and_normalize<Active, false>(node, first, last, increment);
    }
    template<std::size_t Edits>
    [[nodiscard]] std::pair<subtree_change,subtree_change> edit_pair(
            leaf_type& left, const block_edit<subtree_change, Edits>& left_edit,
            leaf_type& right, const block_edit<subtree_change, Edits>& right_edit,
            prepared_action increment, unsigned left_valid, unsigned right_valid) const noexcept {
        // Stage both independent boundary writes before either wide reload.
        for (const auto& edit : left_edit.boundaries) left.values[edit.slot] += edit.change;
        for (const auto& edit : right_edit.boundaries) right.values[edit.slot] += edit.change;
        const auto dl = left_valid == Fanout
            ? transform_and_normalize<Fanout,true>(left,left_edit.first,left_edit.last,increment)
            : transform_and_normalize<Fanout,false>(left,left_edit.first,left_edit.last,increment);
        const auto dr = right_valid == Fanout
            ? transform_and_normalize<Fanout,true>(right,right_edit.first,right_edit.last,increment)
            : transform_and_normalize<Fanout,false>(right,right_edit.first,right_edit.last,increment);
        return {dl,dr};
    }
    template<bool ReturnChange, unsigned Active = Fanout>
    local_change apply_slots(leaf_type& node, unsigned first, unsigned last,
                             prepared_action increment, std::size_t) const noexcept {
        if (increment == 0 || first == last) return {};
        return {.touched=true,.normalized=true,
                .delta=transform_and_normalize<Active>(node,first,last,increment)};
    }
    local_change apply_carried(leaf_type&, unsigned, unsigned, prepared_action,
                               action_type, std::size_t) const noexcept {
        // needs_materialization is unconditionally false, so this path is unreachable.
        std::unreachable();
    }
    [[nodiscard]] subtree_change replace(leaf_type& n, unsigned child, summary_type x) const noexcept {
        n.values[child]=x;
        return normalize(n);
    }
};
// Reuse the entire normalized representation and update scheduler for maximum.
// The codec is the order-reversing isomorphism x -> -x, within the bounded domain.
template<class Action, unsigned Fanout, class Arithmetic>
struct wide_normalized_maximum : wide_normalized_minimum<Action, Fanout, Arithmetic> {
    using base=wide_normalized_minimum<Action, Fanout, Arithmetic>;
    using typename base::summary_type;
    using typename base::value_type;
    using typename base::update_type;
    using typename base::prepared_action;
    wide_normalized_maximum(algebra::maximum<value_type>,Action action) noexcept
        : base(algebra::minimum<value_type>{},action) {}
    [[nodiscard]] summary_type import_value(value_type x) const noexcept { return base::import_value(-x); }
    [[nodiscard]] value_type export_value(summary_type x) const noexcept {
        return x==base::infinity ? std::numeric_limits<value_type>::lowest() : -base::export_value(x);
    }
    [[nodiscard]] prepared_action prepare(const update_type& f) const noexcept requires(base::has_lazy) { return -f; }
};
} // namespace canard::kernel

namespace canard::kernel {
struct normalized_avx2_arithmetic {
    using pack = simd::i64x4;
    template<unsigned Extent> using interval = simd::lane_interval64<Extent>;
};
template<class Action, unsigned Fanout>
using wide_normalized_minimum_avx2 =
    wide_normalized_minimum<Action, Fanout, normalized_avx2_arithmetic>;
template<class Action, unsigned Fanout>
using wide_normalized_maximum_avx2 =
    wide_normalized_maximum<Action, Fanout, normalized_avx2_arithmetic>;
} // namespace canard::kernel

// ===== include/canard/wide/normalized_extremum.hpp =====


// ===== include/canard/simd/minimum_scalar.hpp =====

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>

namespace canard::simd {

// Portable reference execution for the same four-lane block operations. No
// ISA dependency or target attribute; scalar means source-level execution,
// not a prohibition on compiler auto-vectorization.
struct scalar_mask64x4 {
    std::array<bool, 4> lanes;
};

class scalar_i64x4 {
    std::array<std::int64_t, 4> lanes_;
public:
    using value_type = std::int64_t;
    static constexpr std::size_t size() noexcept { return 4; }
    scalar_i64x4() = default;
    explicit constexpr scalar_i64x4(value_type value) noexcept {
        lanes_.fill(value);
    }
    [[nodiscard]] static constexpr scalar_i64x4 load(const value_type* source) noexcept {
        scalar_i64x4 result;
        for (std::size_t i = 0; i < size(); ++i) result.lanes_[i] = source[i];
        return result;
    }
    constexpr void store(value_type* destination) const noexcept {
        for (std::size_t i = 0; i < size(); ++i) destination[i] = lanes_[i];
    }
    friend constexpr scalar_i64x4 operator+(scalar_i64x4 a, scalar_i64x4 b) noexcept {
        for (std::size_t i = 0; i < size(); ++i) a.lanes_[i] += b.lanes_[i];
        return a;
    }
    friend constexpr scalar_i64x4 operator-(scalar_i64x4 a, scalar_i64x4 b) noexcept {
        for (std::size_t i = 0; i < size(); ++i) a.lanes_[i] -= b.lanes_[i];
        return a;
    }
    friend constexpr scalar_mask64x4 operator==(scalar_i64x4 a, scalar_i64x4 b) noexcept {
        scalar_mask64x4 result{};
        for (std::size_t i = 0; i < size(); ++i) result.lanes[i] = a.lanes_[i] == b.lanes_[i];
        return result;
    }
    friend constexpr scalar_i64x4 min(scalar_i64x4 a, scalar_i64x4 b) noexcept {
        for (std::size_t i = 0; i < size(); ++i) a.lanes_[i] = std::min(a.lanes_[i], b.lanes_[i]);
        return a;
    }
    friend constexpr scalar_i64x4 select(scalar_mask64x4 mask,
            scalar_i64x4 when_true, scalar_i64x4 when_false) noexcept {
        for (std::size_t i = 0; i < size(); ++i)
            if (mask.lanes[i]) when_false.lanes_[i] = when_true.lanes_[i];
        return when_false;
    }
    friend constexpr scalar_i64x4 mask_zero(scalar_mask64x4 mask, scalar_i64x4 value) noexcept {
        return select(mask, value, scalar_i64x4{0});
    }
    [[nodiscard]] constexpr value_type reduce_min() const noexcept {
        return std::min(std::min(lanes_[0], lanes_[2]), std::min(lanes_[1], lanes_[3]));
    }
};

template<unsigned Extent>
class scalar_lane_interval64 {
    unsigned first_, last_;
public:
    constexpr scalar_lane_interval64(unsigned first, unsigned last) noexcept
        : first_(first), last_(last) {}
    [[nodiscard]] constexpr scalar_mask64x4 chunk(std::size_t index) const noexcept {
        scalar_mask64x4 result{};
        for (unsigned lane = 0; lane < 4; ++lane) {
            const auto position = index * 4 + lane;
            result.lanes[lane] = first_ <= position && position < last_;
        }
        return result;
    }
};

} // namespace canard::simd
#include <concepts>
#include <cstdint>

namespace canard::kernel {
struct normalized_scalar_arithmetic {
    using pack = simd::scalar_i64x4;
    template<unsigned Extent> using interval = simd::scalar_lane_interval64<Extent>;
};
} // namespace canard::kernel

namespace canard::wide {
template<class Action>
concept extremum_addition = std::same_as<Action, algebra::no_action> ||
    std::same_as<Action, algebra::add_to_extremum<std::int64_t>>;

template<class Action, unsigned Fanout>
    requires (extremum_addition<Action> && (Fanout == 8 || Fanout == 16 || Fanout == 32))
struct kernel_binding<representation::normalized_extremum, execution::scalar,
                      algebra::minimum<std::int64_t>, Action, Fanout> {
    using type = kernel::wide_normalized_minimum<Action, Fanout,
                                               kernel::normalized_scalar_arithmetic>;
};
template<class Action, unsigned Fanout>
    requires (extremum_addition<Action> && (Fanout == 8 || Fanout == 16 || Fanout == 32))
struct kernel_binding<representation::normalized_extremum, execution::scalar,
                      algebra::maximum<std::int64_t>, Action, Fanout> {
    using type = kernel::wide_normalized_maximum<Action, Fanout,
                                               kernel::normalized_scalar_arithmetic>;
};
} // namespace canard::wide

namespace canard::wide {
// Availability is a property of all axes, not of an ISA alone. In particular,
// selecting AVX2 for an unsupported algebra never changes its representation.
template<std::uint32_t P, unsigned Fanout>
    requires (P > 1 && P < (std::uint32_t{1} << 30) && P % 2 == 1 &&
              (Fanout == 8 || Fanout == 16 || Fanout == 32))
struct kernel_binding<representation::ordinary, execution::avx2,
                      algebra::modular_sum<P>, algebra::affine_on_sum<P>, Fanout> {
    using type = kernel::wide_affine_sum_avx2<P, Fanout>;
};
template<class Action, unsigned Fanout>
    requires (extremum_addition<Action> && (Fanout == 8 || Fanout == 16 || Fanout == 32))
struct kernel_binding<representation::ordinary, execution::avx2,
                      algebra::minimum<std::int64_t>, Action, Fanout> {
    using type = kernel::wide_minimum_avx2<Action, Fanout>;
};
template<class Action, unsigned Fanout>
    requires (extremum_addition<Action> && (Fanout == 8 || Fanout == 16 || Fanout == 32))
struct kernel_binding<representation::normalized_extremum, execution::avx2,
                      algebra::minimum<std::int64_t>, Action, Fanout> {
    using type = kernel::wide_normalized_minimum_avx2<Action, Fanout>;
};
template<class Action, unsigned Fanout>
    requires (extremum_addition<Action> && (Fanout == 8 || Fanout == 16 || Fanout == 32))
struct kernel_binding<representation::normalized_extremum, execution::avx2,
                      algebra::maximum<std::int64_t>, Action, Fanout> {
    using type = kernel::wide_normalized_maximum_avx2<Action, Fanout>;
};
} // namespace canard::wide

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
struct padded_bytes_view { const char* data; std::size_t size; };

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
        mapping_ = ::mmap(nullptr, mapping_size_, PROT_READ,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        ::mmap(static_cast<char*>(mapping_) + page, bytes_, PROT_READ,
               MAP_PRIVATE | MAP_FIXED, descriptor, 0);
        data_ = static_cast<const char*>(mapping_) + page;
    }
    padded_file(const padded_file&) = delete;
    padded_file& operator=(const padded_file&) = delete;
    ~padded_file() { ::munmap(mapping_, mapping_size_); }
    [[nodiscard]] padded_bytes_view view() const noexcept { return {data_, bytes_}; }
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

// Borrowing field in padded ASCII storage. digits is 1..9, and the eight
// bytes preceding data+digits must be readable, even for a short field.
struct field { const char* data; unsigned digits; };

namespace detail {
inline constexpr int pair_weights = 0x010a; // [10, 1]
inline constexpr int quad_weights = 0x00010064; // [100, 1]
inline constexpr int octet_weights = 0x00012710; // [10000, 1]
inline constexpr auto nibble_mask = [] consteval {
    std::array<std::uint64_t, 10> masks{};
    for (unsigned digits = 1; digits <= 9; ++digits)
        masks[digits] = 0x0f0f'0f0f'0f0f'0f0full << (digits < 8 ? (8 - digits) * 8 : 0);
    return masks;
}();
inline constexpr std::array<unsigned, 10> ninth_digit_weight{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 100'000'000
};
[[nodiscard]] inline std::uint64_t eight_byte_tail(field input) noexcept {
    std::uint64_t bytes;
    std::memcpy(&bytes, input.data + input.digits - 8, 8);
    return bytes;
}
[[nodiscard]] inline std::uint32_t ninth_digit(field input) noexcept {
    return static_cast<unsigned>(input.data[0] - '0') * ninth_digit_weight[input.digits];
}
} // namespace detail

[[nodiscard]] inline std::uint32_t decode_one(field input) noexcept {
    auto digits = detail::eight_byte_tail(input) & detail::nibble_mask[input.digits];
    digits = digits * 10 + (digits >> 8);
    const auto eight = static_cast<std::uint32_t>((
        (digits & 0x0000'00ff'0000'00ffull) * 0x000f'4240'0000'0064ull +
        ((digits >> 16) & 0x0000'00ff'0000'00ffull) * 0x0000'2710'0000'0001ull) >> 32);
    return eight + detail::ninth_digit(input);
}

// ASCII -> digit pairs -> four-digit groups -> eight-digit groups.
// Four-digit groups are <=9999, so packing does not saturate and the final
// signed 16-bit multiply-add is exact. A ninth digit is added separately.
template<unsigned FirstMaxDigits = 9, unsigned SecondMaxDigits = 9>
[[nodiscard, gnu::always_inline]] inline std::array<std::uint32_t, 2>
decode_pair(field first, field second) noexcept {
    static_assert(1 <= FirstMaxDigits && FirstMaxDigits <= 9);
    static_assert(1 <= SecondMaxDigits && SecondMaxDigits <= 9);
    using namespace detail;
    auto digits = _mm_set_epi64x(std::bit_cast<long long>(eight_byte_tail(second)),
                                std::bit_cast<long long>(eight_byte_tail(first)));
    digits = _mm_and_si128(digits, _mm_set_epi64x(nibble_mask[second.digits], nibble_mask[first.digits]));
    const auto pairs = _mm_maddubs_epi16(digits, _mm_set1_epi16(pair_weights));
    const auto quads = _mm_madd_epi16(pairs, _mm_set1_epi32(quad_weights));
    const auto compact = _mm_packus_epi32(quads, quads);
    const auto eights = _mm_madd_epi16(compact, _mm_set1_epi32(octet_weights));
    const auto leading_first = [&] {if constexpr (FirstMaxDigits == 9)return ninth_digit(first);else return 0u;}();
    const auto leading_second = [&] {if constexpr (SecondMaxDigits == 9)return ninth_digit(second);else return 0u;}();
    return {
        static_cast<std::uint32_t>(_mm_cvtsi128_si32(eights)) + leading_first,
        static_cast<std::uint32_t>(_mm_extract_epi32(eights, 1)) + leading_second
    };
}

// FirstMaxDigits is a precondition, not a run-time validator. A known-small
// first field (e.g. an index) skips its ninth-digit calculation at compile time.
template<unsigned FirstMaxDigits = 9>
[[nodiscard, gnu::always_inline]] inline std::array<std::uint32_t, 3>
decode_three(field first, field second, field third) noexcept {
    static_assert(1 <= FirstMaxDigits && FirstMaxDigits <= 9);
    using namespace detail;
    auto digits = _mm256_setr_epi64x(
        std::bit_cast<long long>(eight_byte_tail(first)),
        std::bit_cast<long long>(eight_byte_tail(second)),
        std::bit_cast<long long>(eight_byte_tail(third)), 0);
    digits = _mm256_and_si256(digits, _mm256_setr_epi64x(
        nibble_mask[first.digits], nibble_mask[second.digits], nibble_mask[third.digits], 0));
    const auto pairs = _mm256_maddubs_epi16(digits, _mm256_set1_epi16(pair_weights));
    const auto quads = _mm256_madd_epi16(pairs, _mm256_set1_epi32(quad_weights));
    const auto compact = _mm256_packus_epi32(quads, quads);
    const auto eights = _mm256_madd_epi16(compact, _mm256_set1_epi32(octet_weights));
    const auto leading = [&] {
        if constexpr (FirstMaxDigits == 9) return ninth_digit(first);
        else return 0u;
    }();
    // AVX2 packing operates independently in each 128-bit half: fields end
    // up in lanes 0, 1, and 4, not 0, 1, and 2.
    return {
        static_cast<std::uint32_t>(_mm256_extract_epi32(eights, 0)) + leading,
        static_cast<std::uint32_t>(_mm256_extract_epi32(eights, 1)) + ninth_digit(second),
        static_cast<std::uint32_t>(_mm256_extract_epi32(eights, 4)) + ninth_digit(third)
    };
}

// The next 32 bytes must be readable and contain all requested delimiters.
// Only ASCII decimal input is supported, so signed-byte comparison suffices.
[[nodiscard]] inline std::uint32_t delimiter_mask(const char* data) noexcept {
    return static_cast<std::uint32_t>(_mm256_movemask_epi8(_mm256_cmpgt_epi8(
        _mm256_set1_epi8('0'), _mm256_loadu_si256(reinterpret_cast<const __m256i*>(data)))));
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

    template<class Word>
    [[nodiscard]] static constexpr bool all_digits(Word word) noexcept {
        constexpr Word zeros = sizeof(Word) == 8
            ? static_cast<Word>(0x3030'3030'3030'3030ull) : Word{0x3030'3030u};
        constexpr Word high = sizeof(Word) == 8
            ? static_cast<Word>(0xf0f0'f0f0'f0f0'f0f0ull) : Word{0xf0f0'f0f0u};
        constexpr Word sixes = sizeof(Word) == 8
            ? static_cast<Word>(0x0606'0606'0606'0606ull) : Word{0x0606'0606u};
        return (word & high) == zeros && ((word + sixes) & high) == zeros;
    }

    template<class Word>
    [[nodiscard]] std::uint32_t read_unsigned() {
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
    [[nodiscard, gnu::always_inline]] inline std::array<std::uint32_t, 2> read_pair() {
        skip_whitespace();
        auto mask = decimal::delimiter_mask(cursor_);
        const unsigned first = std::countr_zero(mask);
        mask &= mask - 1;
        const unsigned second = std::countr_zero(mask);
        const auto [a, b] = decimal::decode_pair({cursor_, first}, {cursor_ + first + 1, second - first - 1});
        cursor_ += second;
        ++cursor_;
        return {a, b};
    }
    // Exactly one one-digit tag and two bounded decimal fields.
    template<unsigned FirstMaxDigits = 9, unsigned SecondMaxDigits = 9>
    [[nodiscard, gnu::always_inline]] inline std::array<std::uint32_t,3> read_tagged_pair() noexcept {
        skip_whitespace();
        const unsigned type = static_cast<unsigned>(cursor_[0]-'0');
        cursor_ += 2;
        auto mask = decimal::delimiter_mask(cursor_);
        const unsigned first = std::countr_zero(mask);
        mask &= mask-1;
        const unsigned second = std::countr_zero(mask);
        const auto [a, b] = decimal::decode_pair<FirstMaxDigits, SecondMaxDigits>(
            {cursor_, first},{cursor_+first+1, second-first-1});
        cursor_ += second+1;
        return {type, a, b};
    }

    template<unsigned FirstMaxDigits = 9>
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
        const auto [position, a, b] = decimal::decode_three<FirstMaxDigits>(
            {cursor_ + 2, second - 2},
            {cursor_ + second + 1, third - second - 1},
            {cursor_ + third + 1, fourth - third - 1});
        cursor_ += fourth;
        ++cursor_;
        return {type, position, a, b};
    }

    // Scalar/SWAR parser for general uint32 values, including ten digits.
    // MaxDigits selects a 4- or 8-byte fast prefix, not a run-time validator.
    template<unsigned MaxDigits = 10>
    [[nodiscard]] std::uint32_t read_u32() {
        static_assert(1 <= MaxDigits && MaxDigits <= 10);
        if constexpr (MaxDigits <= 6) return read_unsigned<std::uint32_t>();
        else return read_unsigned<std::uint64_t>();
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
    [[nodiscard]] const char* position() const noexcept { return cursor_; }

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
template<std::uint32_t MaxValue = std::numeric_limits<std::uint32_t>::max()>
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
#include <cstddef>
#include <memory>
#include <unistd.h>
namespace canard::io {
// Trusted POSIX sink: allocations and complete writes are assumed successful.
// Owns its byte buffer, borrows its descriptor. flush() is also done at destruction.
template<std::uint32_t MaxValue = std::numeric_limits<std::uint32_t>::max()>
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
        ::write(descriptor_, storage_.get(), static_cast<std::size_t>(cursor_-storage_.get()));
        cursor_ = storage_.get();
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
#include <cstdint>
#include <memory>
#include <span>

namespace {
constexpr std::uint32_t modulus = 998244353;
constexpr std::uint32_t query_flag = 1u << 31;
constexpr unsigned lookahead = 4;
using profile = canard::wide::configuration<16, 500000,
    canard::wide::representation::ordinary, canard::wide::execution::avx2>;
struct operation {
    std::uint32_t first_and_type, last, multiplier, translation;
    [[nodiscard]] unsigned first() const noexcept { return first_and_type & ~query_flag; }
    [[nodiscard]] bool is_query() const noexcept { return (first_and_type & query_flag) != 0; }
};
static_assert(sizeof(operation) == 16);
}

int main() {
    canard::io::padded_file file;
    canard::io::trusted_ascii_reader input{file.view()};
    canard::io::buffered_writer<modulus - 1> output;
    const auto [size, count] = input.read_pair();
    auto initial = std::make_unique_for_overwrite<std::uint32_t[]>(size);
    for (unsigned i = 0; i + 1 < size; i += 2) {
        const auto [a, b] = input.read_pair();
        initial[i] = a; initial[i + 1] = b;
    }
    if (size & 1u) initial[size - 1] = input.read_u32<9>();
    auto operations = std::make_unique_for_overwrite<operation[]>(count + lookahead);
    for (unsigned i = 0; i < count; ++i) {
        const auto [type, first, last] = input.read_tagged_pair<6, 6>();
        if (type == 0) {
            const auto [a, b] = input.read_pair();
            operations[i] = {first, last, a, b};
        } else operations[i] = {first | query_flag, last, 0, 0};
    }
    for (unsigned i = 0; i < lookahead; ++i) operations[count + i] = {query_flag, 1, 0, 0};

    canard::wide_lazy_segment_tree tree{
        std::span<const std::uint32_t>{initial.get(), size},
        canard::algebra::modular_sum<modulus>{},
        canard::algebra::affine_on_sum<modulus>{}, profile{}};
    initial.reset();
    for (unsigned i = 0; i < count; ++i) {
        const auto& future = operations[i + lookahead];
        tree.prefetch(future.first(), future.last);
        const auto& current = operations[i];
        if (current.is_query()) output.write(tree.fold(current.first(), current.last));
        else tree.apply(current.first(), current.last,
                        {.multiplier = current.multiplier, .translation = current.translation});
    }
}
