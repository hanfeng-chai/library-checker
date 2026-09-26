// Generated from canard headers and client. Edit the maintained sources.
// ===== clients/range_reverse_range_sum.cpp =====
// ===== include/canard/sequence/chunked_sequence.hpp =====
// ===== include/canard/sequence/chunked_lazy_sequence.hpp =====
// ===== include/canard/kernel/sequence_scalar.hpp =====
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
// ===== include/canard/kernel/sequence_leaf.hpp =====
#include <algorithm>
#include <array>
#include <cstddef>
namespace canard::kernel {
// Representation-neutral movement inside an owned, fixed-capacity leaf.
// No tags, fields, balancing, node allocation, or I/O in this component.
template <typename Word, unsigned Extent> struct sequence_leaf_operations {
    struct alignas(64) leaf_type {
        std::array<Word, Extent> values{};
    };
    static void
    copy_values(leaf_type& dst, unsigned at, const leaf_type& src, unsigned first, unsigned count) {
        std::copy_n(src.values.begin() + first, count, dst.values.begin() + at);
    }
    static void insert_value(leaf_type& leaf, unsigned at, unsigned size, const Word& value) {
        std::move_backward(
            leaf.values.begin() + at, leaf.values.begin() + size, leaf.values.begin() + size + 1);
        leaf.values[at] = value;
    }
    static void erase_value(leaf_type& leaf, unsigned at, unsigned size) {
        std::move(
            leaf.values.begin() + at + 1, leaf.values.begin() + size, leaf.values.begin() + at);
    }
    // Left=A B and right=C D become reverse(D) B and C reverse(A).
    // Reversing the enclosing whole-block interval then gives
    // A reverse(C) ... reverse(B) D. Leaves must be distinct, and both
    // replacement lengths must fit Extent. No semantic algebra is assumed.
    static void exchange_reversed_outer_parts(leaf_type& left,
                                              unsigned left_size,
                                              unsigned left_cut,
                                              leaf_type& right,
                                              unsigned right_size,
                                              unsigned right_cut) {
        const unsigned tail = right_size - right_cut;
        const unsigned replacement = tail + left_size - left_cut;
        std::array<Word, Extent> saved;
        std::copy_n(left.values.begin(), left_cut, saved.begin());
        if (tail > left_cut)
            std::move_backward(left.values.begin() + left_cut,
                               left.values.begin() + left_size,
                               left.values.begin() + replacement);
        else if (tail < left_cut)
            std::move(left.values.begin() + left_cut,
                      left.values.begin() + left_size,
                      left.values.begin() + tail);
        std::reverse_copy(right.values.begin() + right_cut,
                          right.values.begin() + right_size,
                          left.values.begin());
        std::reverse_copy(
            saved.begin(), saved.begin() + left_cut, right.values.begin() + right_cut);
    }
    // Same exchange as above, but preserve each leaf's physical orientation.
    // Neither full leaf needs to be reversed merely to edit an outer fragment.
    static void exchange_reversed_outer_parts_oriented(
        leaf_type& left, unsigned left_size, unsigned left_cut, bool left_reversed,
        leaf_type& right, unsigned right_size, unsigned right_cut, bool right_reversed) {
        const unsigned tail = right_size - right_cut;
        const unsigned inside = left_size - left_cut;
        std::array<Word, Extent> saved;
        const auto copy_fragment = [&](const Word* src, unsigned count, Word* dst) {
            if (left_reversed == right_reversed) std::reverse_copy(src, src + count, dst);
            else std::copy_n(src, count, dst);
        };
        copy_fragment(left.values.data() + (left_reversed ? inside : 0), left_cut, saved.data());
        if (!left_reversed && tail != left_cut) {
            if (tail > left_cut)
                std::move_backward(left.values.begin() + left_cut, left.values.begin() + left_size,
                                   left.values.begin() + tail + inside);
            else std::move(left.values.begin() + left_cut, left.values.begin() + left_size,
                           left.values.begin() + tail);
        }
        copy_fragment(right.values.data() + (right_reversed ? 0 : right_cut), tail,
                      left.values.data() + (left_reversed ? inside : 0));
        if (right_reversed && left_cut != tail) {
            if (left_cut > tail)
                std::move_backward(right.values.begin() + tail, right.values.begin() + right_size,
                                   right.values.begin() + left_cut + right_cut);
            else std::move(right.values.begin() + tail, right.values.begin() + right_size,
                           right.values.begin() + left_cut);
        }
        std::copy_n(saved.begin(), left_cut,
                    right.values.begin() + (right_reversed ? 0 : right_cut));
    }
    static void reverse_values(leaf_type& leaf, unsigned first, unsigned last) {
        std::reverse(leaf.values.begin() + first, leaf.values.begin() + last);
    }
};
} // namespace canard::kernel
// ===== include/canard/kernel/wide_scalar.hpp =====
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
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace canard::kernel {
namespace detail {
template <std::size_t N, typename T> [[nodiscard]] constexpr std::array<T, N> repeat(const T& x) {
    return [&]<std::size_t... I>(std::index_sequence<I...>) {
        return std::array<T, N>{(static_cast<void>(I), x)...};
    }(std::make_index_sequence<N>{});
}
template <typename Action, unsigned B, bool Lazy> struct tag_storage {};
template <typename Action, unsigned B> struct tag_storage<Action, B, true> {
    std::array<tag_type_t<Action>, B> tags;
    std::uint64_t dirty = 0;
};
} // namespace detail

// Portable reference block implementation. It is generic in the scalar
// algebra; only this block layer knows the physical array representation.
// Other backends can replace fold/transform/repair without replacing traversal.
template <algebra::monoid Monoid, typename Action, unsigned Fanout>
    requires algebra::action_for<Action, Monoid>
struct wide_scalar {
    using value_type = value_type_t<Monoid>;
    using summary_type = value_type;
    using update_type = tag_type_t<Action>;
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
    struct subtree_change {
        summary_type value;
    };

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
        noexcept(std::declval<const Monoid&>().combine(std::declval<const value_type&>(),
                                                       std::declval<const value_type&>())) &&
        noexcept(std::declval<const Action&>().identity()) &&
        noexcept(std::declval<const Action&>().compose(std::declval<const action_type&>(),
                                                       std::declval<const action_type&>())) &&
        noexcept(std::declval<const Action&>().map(
            std::declval<const action_type&>(), std::declval<const value_type&>(), std::size_t{}));

    [[nodiscard]] leaf_type empty_leaf() const {
        return {detail::repeat<Fanout>(monoid.identity())};
    }
    [[nodiscard]] branch_type empty_branch() const {
        if constexpr (has_lazy)
            return {detail::repeat<Fanout>(monoid.identity()),
                    {detail::repeat<Fanout>(action.identity()), 0}};
        else
            return {detail::repeat<Fanout>(monoid.identity()), {}};
    }
    [[nodiscard]] summary_type identity() const noexcept(nothrow_mutation) {
        return monoid.identity();
    }
    [[nodiscard]] summary_type combine(const summary_type& x, const summary_type& y) const
        noexcept(nothrow_mutation) {
        return monoid.combine(x, y);
    }
    [[nodiscard]] summary_type import_value(const value_type& x) const noexcept(nothrow_mutation) {
        return x;
    }
    [[nodiscard]] value_type export_value(const summary_type& x) const noexcept(nothrow_mutation) {
        return x;
    }

    template <typename Block>
    [[nodiscard]] const summary_type& slot(const Block& block, unsigned i) const noexcept {
        return block.values[i];
    }
    template <typename Block>
    void write_slot(Block& block, unsigned i, const summary_type& x) const
        noexcept(nothrow_mutation) {
        block.values[i] = x;
    }

    template <unsigned Active = Fanout, typename Block>
    [[nodiscard]] summary_type fold(const Block& block, unsigned first, unsigned last) const
        noexcept(nothrow_mutation) {
        static_assert(Active <= Fanout);
        auto answer = identity();
        for (auto i = first; i < last; ++i)
            answer = combine(answer, slot(block, i));
        return answer;
    }

    [[nodiscard]] action_type identity_action() const noexcept(nothrow_mutation) {
        return action.identity();
    }
    [[nodiscard]] prepared_action prepare(const update_type& update) const
        noexcept(nothrow_mutation) {
        return update;
    }
    [[nodiscard]] prepared_action prepare_internal(const action_type& update) const
        noexcept(nothrow_mutation) {
        return update;
    }
    [[nodiscard]] action_type compose(const action_type& newer, const action_type& older) const
        noexcept(nothrow_mutation) {
        return action.compose(newer, older);
    }
    [[nodiscard]] summary_type map(const action_type& f,
                                   const summary_type& x,
                                   std::size_t count) const noexcept(nothrow_mutation) {
        return action.map(f, x, count);
    }

    [[nodiscard]] bool needs_materialization(const branch_type& node,
                                             unsigned child) const noexcept {
        if constexpr (has_lazy)
            return (node.lazy.dirty >> child) & 1;
        else
            return false;
    }
    [[nodiscard]] action_type child_frame(const branch_type& node, unsigned child) const
        noexcept(nothrow_mutation) {
        if constexpr (has_lazy)
            return node.lazy.tags[child];
        else
            return {};
    }
    void clear_frame(branch_type& node, unsigned child) const noexcept(nothrow_mutation) {
        if constexpr (has_lazy) {
            node.lazy.tags[child] = action.identity();
            node.lazy.dirty &= ~(std::uint64_t{1} << child);
        }
    }

    [[nodiscard]] local_change begin_changes() const noexcept {
        return {};
    }
    void account_local(local_change&, local_change) const noexcept {}
    [[nodiscard]] local_change
    repair_slot(branch_type& node, unsigned child, const subtree_change& change) const
        noexcept(nothrow_mutation) {
        write_slot(node, child, change.value);
        return {};
    }

    template <bool ReturnChange, typename Block>
    [[nodiscard]] subtree_change finish(const Block& node, local_change) const
        noexcept(nothrow_mutation) {
        if constexpr (ReturnChange)
            return {fold(node, 0, Fanout)};
        else
            return {identity()};
    }

    // Each addressed slot is an entire child with exactly child_length real
    // leaves. Traversal never sends a padded slot here.
    template <bool ReturnChange, unsigned Active = Fanout, typename Block>
    local_change apply_slots(Block& node,
                             unsigned first,
                             unsigned last,
                             const prepared_action& f,
                             std::size_t child_length) const noexcept(nothrow_mutation) {
        static_assert(Active <= Fanout);
        for (auto i = first; i < last; ++i)
            apply_slot(node, i, f, child_length);
        return {};
    }

    // A carried action exists only on a geometrically complete subtree.
    // All Fanout slots here represent real children. Apply incoming outside
    // the complete update interval and (update o incoming) inside it.
    template <typename Block>
    local_change apply_carried(Block& node,
                               unsigned first,
                               unsigned last,
                               const prepared_action& update,
                               const action_type& incoming,
                               std::size_t child_length) const noexcept(nothrow_mutation) {
        const auto after = compose(update, incoming);
        for (unsigned i = 0; i < Fanout; ++i)
            apply_slot(node, i, first <= i && i < last ? after : incoming, child_length);
        return {};
    }

    [[nodiscard]] subtree_change
    replace(leaf_type& node, unsigned child, const summary_type& value) const
        noexcept(nothrow_mutation) {
        write_slot(node, child, value);
        return finish<true>(node, {});
    }

  private:
    template <typename Block>
    void apply_slot(Block& node, unsigned child, const action_type& f, std::size_t length) const
        noexcept(nothrow_mutation) {
        node.values[child] = map(f, node.values[child], length);
        if constexpr (has_lazy && std::same_as<Block, branch_type>) {
            node.lazy.tags[child] = compose(f, node.lazy.tags[child]);
            node.lazy.dirty |= std::uint64_t{1} << child;
        }
    }
};

} // namespace canard::kernel

namespace canard::wide {
template <algebra::monoid Monoid, typename Action, unsigned Fanout>
    requires algebra::action_for<Action, Monoid>
struct kernel_binding<representation::ordinary, execution::scalar, Monoid, Action, Fanout> {
    using type = canard::kernel::wide_scalar<Monoid, Action, Fanout>;
};
} // namespace canard::wide
// ===== include/canard/sequence/configuration.hpp =====
#include <cstddef>
#include <cstdint>
namespace canard::sequence {
template <unsigned LeafCapacity = 64,
          std::size_t MaxSize = (std::size_t{1} << 30),
          typename Representation = wide::representation::ordinary,
          typename Execution = wide::execution::scalar,
          unsigned OccupancyDivisor = 4,
          unsigned HeightTolerance = 1>
struct configuration {
    static_assert(LeafCapacity >= 8 && LeafCapacity <= 512 && std::has_single_bit(LeafCapacity));
    static_assert(MaxSize > 0 && MaxSize < (std::size_t{1} << 31));
    static constexpr unsigned leaf_capacity = LeafCapacity;
    static_assert(1 <= HeightTolerance && HeightTolerance <= 4);
    static constexpr unsigned height_tolerance = HeightTolerance;
    static_assert(OccupancyDivisor >= 2 && OccupancyDivisor <= LeafCapacity &&
                  std::has_single_bit(OccupancyDivisor));
    static constexpr unsigned occupancy_divisor = OccupancyDivisor;
    static constexpr std::size_t max_size = MaxSize;
    using representation_type = Representation;
    using execution_type = Execution;
};
template <typename Representation,
          typename Execution,
          typename Monoid,
          typename Action,
          unsigned Extent>
struct kernel_binding {};
template <typename Configuration, typename Monoid, typename Action>
concept supported_configuration = requires {
    typename kernel_binding<representation_type_t<Configuration>,
                            execution_type_t<Configuration>,
                            Monoid,
                            Action,
                            Configuration::leaf_capacity>::type;
};
template <typename Configuration, typename Monoid, typename Action>
    requires supported_configuration<Configuration, Monoid, Action>
using kernel_for_t = typename kernel_binding<representation_type_t<Configuration>,
                                             execution_type_t<Configuration>,
                                             Monoid,
                                             Action,
                                             Configuration::leaf_capacity>::type;
} // namespace canard::sequence
#include <utility>
namespace canard::kernel {
// The general case explicitly stores both orders: reversal does NOT assume
// commutativity. Monoid/action objects are the same public canard algebra.
template <algebra::monoid Monoid, typename Action, unsigned Extent>
    requires algebra::action_for<Action, Monoid>
struct sequence_scalar : sequence_leaf_operations<value_type_t<Monoid>, Extent> {
    using value_type = value_type_t<Monoid>;
    using base = sequence_leaf_operations<value_type, Extent>;
    using leaf_type = leaf_type_t<base>;
    struct summary_type {
        value_type forward, backward;
    };
    using update_type = tag_type_t<Action>;
    using action_type = update_type;
    using prepared_action = action_type;
    static constexpr bool has_lazy = !std::same_as<Action, algebra::no_action>;
    static constexpr bool nothrow_mutation = wide_scalar<Monoid, Action, 8>::nothrow_mutation;
    [[no_unique_address]] Monoid monoid;
    [[no_unique_address]] Action action;
    sequence_scalar(Monoid m, Action a) : monoid(std::move(m)), action(std::move(a)) {}
    [[nodiscard]] summary_type identity() const {
        return {monoid.identity(), monoid.identity()};
    }
    [[nodiscard]] summary_type combine(const summary_type& a, const summary_type& b) const {
        return {monoid.combine(a.forward, b.forward), monoid.combine(b.backward, a.backward)};
    }
    [[nodiscard]] summary_type reversed(summary_type a) const {
        std::swap(a.forward, a.backward);
        return a;
    }
    [[nodiscard]] value_type export_summary(const summary_type& a) const {
        return a.forward;
    }
    [[nodiscard]] value_type import_value(const value_type& a) const {
        return a;
    }
    [[nodiscard]] action_type identity_action() const {
        return action.identity();
    }
    [[nodiscard]] action_type compose(const action_type& a, const action_type& b) const {
        return action.compose(a, b);
    }
    [[nodiscard]] summary_type
    map(const action_type& f, const summary_type& s, std::size_t count) const {
        return {action.map(f, s.forward, count), action.map(f, s.backward, count)};
    }
    [[nodiscard]] prepared_action prepare(const update_type& f) const {
        return f;
    }
    [[nodiscard]] const action_type& action_of(const prepared_action& f) const {
        return f;
    }
    [[nodiscard]] summary_type summarize(const leaf_type& leaf, unsigned l, unsigned r) const {
        auto answer = identity();
        for (unsigned i = l; i < r; ++i)
            answer.forward = monoid.combine(answer.forward, leaf.values[i]);
        for (unsigned i = r; i-- > l;)
            answer.backward = monoid.combine(answer.backward, leaf.values[i]);
        return answer;
    }
    void materialize(leaf_type& leaf, unsigned size, const action_type& f) const {
        for (unsigned i = 0; i < size; ++i)
            leaf.values[i] = action.map(f, leaf.values[i], 1);
    }
    [[nodiscard]] summary_type update_leaf(leaf_type& leaf,
                                           unsigned size,
                                           unsigned l,
                                           unsigned r,
                                           const prepared_action& f,
                                           const summary_type&) const {
        for (unsigned i = l; i < r; ++i)
            leaf.values[i] = action.map(f, leaf.values[i], 1);
        return summarize(leaf, 0, size);
    }
    [[nodiscard]] summary_type after_insert(const leaf_type& leaf,
                                            unsigned size,
                                            const summary_type&,
                                            const value_type&) const {
        return summarize(leaf, 0, size);
    }
    [[nodiscard]] summary_type after_erase(const leaf_type& leaf,
                                           unsigned size,
                                           const summary_type&,
                                           const value_type&) const {
        return summarize(leaf, 0, size);
    }
    [[nodiscard]] summary_type after_set(const leaf_type& leaf,
                                         unsigned size,
                                         const summary_type&,
                                         const value_type&,
                                         const value_type&) const {
        return summarize(leaf, 0, size);
    }
};
} // namespace canard::kernel
namespace canard::sequence {
template <algebra::monoid Monoid, typename Action, unsigned Extent>
    requires algebra::action_for<Action, Monoid>
struct kernel_binding<wide::representation::ordinary,
                      wide::execution::scalar,
                      Monoid,
                      Action,
                      Extent> {
    using type = kernel::sequence_scalar<Monoid, Action, Extent>;
};
} // namespace canard::sequence
// ===== include/canard/kernel/sequence_affine_sum_common.hpp =====
// ===== include/canard/algebra/modular.hpp =====
#include <cstdint>
#include <format>
#include <string_view>

namespace canard::algebra {

template <std::uint32_t Modulus> struct affine_map {
    static_assert(Modulus > 1 && Modulus < (std::uint32_t{1} << 31));
    std::uint32_t multiplier = 1;
    std::uint32_t translation = 0;
    [[nodiscard]] constexpr std::uint32_t operator()(std::uint32_t x) const noexcept {
        return (std::uint64_t(multiplier) * x + translation) % Modulus;
    }
    friend constexpr bool operator==(affine_map, affine_map) = default;
};

template <std::uint32_t Modulus> struct modular_sum {
    static_assert(Modulus > 1 && Modulus < (std::uint32_t{1} << 31));
    using value_type = std::uint32_t;
    [[nodiscard]] constexpr value_type identity() const noexcept {
        return 0;
    }
    [[nodiscard]] constexpr value_type combine(value_type a, value_type b) const noexcept {
        const auto x = a + b;
        return x >= Modulus ? x - Modulus : x;
    }
};

template <std::uint32_t Modulus> struct affine_composition {
    using value_type = affine_map<Modulus>;
    [[nodiscard]] constexpr value_type identity() const noexcept {
        return {};
    }
    // Array order: combine(left, right) means apply left first, then right.
    [[nodiscard]] constexpr value_type combine(value_type left, value_type right) const noexcept {
        return {
            std::uint32_t(std::uint64_t(right.multiplier) * left.multiplier % Modulus),
            std::uint32_t((std::uint64_t(right.multiplier) * left.translation + right.translation) %
                          Modulus)};
    }
};

template <std::uint32_t Modulus> struct affine_on_sum {
    using tag_type = affine_map<Modulus>;
    [[nodiscard]] constexpr tag_type identity() const noexcept {
        return {};
    }
    [[nodiscard]] constexpr tag_type compose(tag_type newer, tag_type older) const noexcept {
        return affine_composition<Modulus>{}.combine(older, newer);
    }
    [[nodiscard]] constexpr std::uint32_t
    map(tag_type f, std::uint32_t x, std::size_t count) const noexcept {
        return (std::uint64_t(f.multiplier) * x +
                std::uint64_t(f.translation) * (count % Modulus)) %
               Modulus;
    }
};

} // namespace canard::algebra

// The public scalar's formatter is exposed by its own header. Arithmetic-only
// internal kernels do not include this header or formatting/transport support.
template <std::uint32_t P>
struct std::formatter<canard::algebra::affine_map<P>, char>
    : std::formatter<std::string_view, char> {
    template <typename Context>
    auto format(canard::algebra::affine_map<P> f, Context& context) const {
        const auto text = std::format("({}x + {})", f.multiplier, f.translation);
        return std::formatter<std::string_view, char>::format(text, context);
    }
};
// ===== include/canard/numeric/montgomery32.hpp =====
#include <cstdint>
#include <limits>
namespace canard::numeric {
// Expert arithmetic policy. Words are deliberately raw: encode/decode mark
// representation changes; operators below preserve lazy residues in [0,2p).
// No primality assumption is needed for arithmetic or power().
template <std::uint32_t Modulus> struct montgomery32 {
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
    static_assert(8 * u64{modulus} * modulus + u64{modulus} * 0xffff'ffffu <
                  std::numeric_limits<u64>::max());

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
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return n == 2;
    for (std::uint32_t d = 3; std::uint64_t{d} * d <= n; d += 2)
        if (n % d == 0)
            return false;
    return true;
}

// Optional public scalar value type; exactly one word. The hot storage kernels
// use the policy above directly rather than aliasing arrays of these objects.
template <std::uint32_t Modulus> class montgomery_value {
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
#include <cstdint>
namespace canard::kernel {
// Shared representation for scalar and AVX2 sequence execution. Sums/values
// are ordinary residues in [0,2p); only lazy coefficients are encoded.
template <std::uint32_t Modulus, unsigned Extent>
struct sequence_affine_sum_common : sequence_leaf_operations<std::uint32_t, Extent> {
    using field = numeric::montgomery32<Modulus>;
    using value_type = std::uint32_t;
    using summary_type = std::uint32_t;
    using base = sequence_leaf_operations<value_type, Extent>;
    using leaf_type = leaf_type_t<base>;
    using update_type = algebra::affine_map<Modulus>;
    struct action_type {
        value_type multiplier, translation;
    };
    static constexpr bool has_lazy = true;
    static constexpr bool nothrow_mutation = true;
    static constexpr std::size_t max_size = 2ull * Modulus - 1;
    sequence_affine_sum_common(algebra::modular_sum<Modulus>, algebra::affine_on_sum<Modulus>) {}
    [[nodiscard]] static summary_type identity() noexcept {
        return 0;
    }
    [[nodiscard]] static summary_type combine(summary_type a, summary_type b) noexcept {
        return field::add(a, b);
    }
    [[nodiscard]] static summary_type reversed(summary_type a) noexcept {
        return a;
    }
    [[nodiscard]] static value_type export_summary(summary_type a) noexcept {
        return a >= Modulus ? a - Modulus : a;
    }
    [[nodiscard]] static value_type import_value(value_type a) noexcept {
        return a;
    }
    [[nodiscard]] static action_type identity_action() noexcept {
        return {field::one, 0};
    }
    [[nodiscard]] static action_type encode(update_type f) noexcept {
        return {field::encode(f.multiplier), field::encode(f.translation)};
    }
    [[nodiscard]] static action_type compose(action_type a, action_type b) noexcept {
        return {field::multiply(a.multiplier, b.multiplier),
                field::add(field::multiply(a.multiplier, b.translation), a.translation)};
    }
    [[nodiscard]] static summary_type
    map(action_type f, summary_type s, std::size_t count) noexcept {
        return field::multiply_sum(f.multiplier, s, f.translation, static_cast<unsigned>(count));
    }
    [[nodiscard]] static summary_type remove_prefix_summary(summary_type total,
                                                            summary_type prefix) noexcept {
        return field::subtract(total, prefix);
    }
    [[nodiscard]] static summary_type remove_suffix_summary(summary_type total,
                                                            summary_type suffix) noexcept {
        return field::subtract(total, suffix);
    }
    [[nodiscard]] static summary_type
    after_reverse(const leaf_type&, unsigned, unsigned, unsigned, summary_type old) noexcept {
        return old;
    }
    [[nodiscard]] static summary_type
    after_insert(const leaf_type&, unsigned, summary_type old, value_type value) noexcept {
        return field::add(old, value);
    }
    [[nodiscard]] static summary_type
    after_erase(const leaf_type&, unsigned, summary_type old, value_type value) noexcept {
        return field::subtract(old, value);
    }
    [[nodiscard]] static summary_type after_set(const leaf_type&,
                                                unsigned,
                                                summary_type old,
                                                value_type before,
                                                value_type after) noexcept {
        return field::add(old, field::subtract(after, before));
    }
};
template <std::uint32_t Modulus, unsigned Extent>
struct sequence_affine_sum_scalar : sequence_affine_sum_common<Modulus, Extent> {
    using base = sequence_affine_sum_common<Modulus, Extent>;
    using field = field_t<base>;
    using leaf_type = leaf_type_t<base>;
    using summary_type = summary_type_t<base>;
    using update_type = update_type_t<base>;
    using action_type = action_type_t<base>;
    using prepared_action = action_type;
    using base::base;
    [[nodiscard]] static prepared_action prepare(update_type f) noexcept {
        return base::encode(f);
    }
    [[nodiscard]] static action_type action_of(prepared_action f) noexcept {
        return f;
    }
    [[nodiscard]] static summary_type
    summarize(const leaf_type& leaf, unsigned l, unsigned r) noexcept {
        std::uint64_t total = 0;
        for (unsigned i = l; i < r; ++i)
            total += leaf.values[i];
        return total % Modulus;
    }
    static void materialize(leaf_type& leaf, unsigned size, action_type f) noexcept {
        for (unsigned i = 0; i < size; ++i)
            leaf.values[i] = base::map(f, leaf.values[i], 1);
    }
    [[nodiscard]] static summary_type update_leaf(leaf_type& leaf,
                                                  unsigned,
                                                  unsigned l,
                                                  unsigned r,
                                                  prepared_action f,
                                                  summary_type old) noexcept {
        summary_type before = 0, after = 0;
        for (unsigned i = l; i < r; ++i) {
            before = field::add(before, leaf.values[i]);
            leaf.values[i] = base::map(f, leaf.values[i], 1);
            after = field::add(after, leaf.values[i]);
        }
        return field::add(old, field::subtract(after, before));
    }
};
} // namespace canard::kernel
namespace canard::sequence {
template <std::uint32_t P, unsigned Extent>
    requires(P > 1 && (P & 1) != 0 && P < (1u << 30))
struct kernel_binding<wide::representation::ordinary,
                      wide::execution::scalar,
                      algebra::modular_sum<P>,
                      algebra::affine_on_sum<P>,
                      Extent> {
    using type = kernel::sequence_affine_sum_scalar<P, Extent>;
};
} // namespace canard::sequence
// ===== include/canard/sequence/detail/chunked_engine.hpp =====
// ===== include/canard/memory/indexed_pool.hpp =====
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

namespace canard::memory {
// Stable handles, not stable references. Growth may relocate slots; a caller
// must not retain references across acquire/ensure_available. Index 0 is null.
// The free-list capacity always covers the slots capacity, including after a
// copy, so release performs no allocation. Released slot objects are retained
// until reuse or pool destruction.
template <typename T> class indexed_pool {
    std::vector<T> slots_;
    std::vector<std::uint32_t> free_;

  public:
    indexed_pool() {
        reserve(16);
        slots_.emplace_back();
    }
    indexed_pool(const indexed_pool& other) : slots_(other.slots_), free_(other.free_) {
        free_.reserve(slots_.capacity());
    }
    indexed_pool(indexed_pool&&) noexcept = default;
    indexed_pool& operator=(const indexed_pool& other) {
        if (this != &other) {
            indexed_pool next{other};
            swap(next);
        }
        return *this;
    }
    indexed_pool& operator=(indexed_pool&&) noexcept = default;
    void swap(indexed_pool& other) noexcept {
        slots_.swap(other.slots_);
        free_.swap(other.free_);
    }
    void reserve(std::size_t count) {
        if (count <= slots_.capacity())
            return;
        free_.reserve(count);
        slots_.reserve(count);
    }
    void ensure_available(std::size_t count) {
        const auto needed = std::max<std::size_t>(1, slots_.size()) +
                            (count > free_.size() ? count - free_.size() : 0);
        if (needed > slots_.capacity())
            reserve(std::max(needed, 2 * slots_.capacity()));
        if (slots_.empty())
            slots_.emplace_back();
    }
    [[nodiscard]] std::uint32_t acquire() {
        const auto id = acquire_retained();
        slots_[id] = T{};
        return id;
    }
    // Returned objects are initialized, but a recycled slot retains its old
    // value. A leaf owner overwrites all active positions before publishing it.
    [[nodiscard]] std::uint32_t acquire_retained() {
        if (!free_.empty()) {
            const auto id = free_.back();
            free_.pop_back();
            return id;
        }
        ensure_available(1);
        const auto id = static_cast<std::uint32_t>(slots_.size());
        slots_.emplace_back();
        return id;
    }
    void release(std::uint32_t id) noexcept {
        free_.push_back(id);
    }
    [[nodiscard]] T& operator[](std::uint32_t id) noexcept {
        return slots_[id];
    }
    [[nodiscard]] const T& operator[](std::uint32_t id) const noexcept {
        return slots_[id];
    }
    [[nodiscard]] std::size_t live_slots() const noexcept {
        return slots_.empty() ? 0 : slots_.size() - 1 - free_.size();
    }
    [[nodiscard]] std::size_t allocated_bytes() const noexcept {
        return slots_.capacity() * sizeof(T) + free_.capacity() * sizeof(std::uint32_t);
    }
};
} // namespace canard::memory
// ===== include/canard/structural/avl_rope.hpp =====
#include <algorithm>
#include <cstdint>
#include <utility>
#include <span>
namespace canard::structural {
// Leaf-oriented height-balanced split/join scheduler. MaxImbalance=1 is
// strict AVL; a fixed larger tolerance trades height for fewer rotations. Access owns metadata,
// payloads, lazy interpretation, and storage lifetime. This file knows none of them. Every tree
// passed to join obeys the selected height-balance bound; every non-leaf has two children.
template <typename Access, unsigned MaxImbalance = 1> class avl_rope {
    static_assert(1 <= MaxImbalance && MaxImbalance <= 4);
    using handle = std::uint32_t;
    Access& access_;
    handle rotate_right(handle x) {
        const auto y = access_.left(x);
        access_.push(y);
        access_.set_left(x, access_.right(y));
        access_.set_right(y, x);
        access_.repair(x);
        access_.repair(y);
        return y;
    }
    handle rotate_left(handle x) {
        const auto y = access_.right(x);
        access_.push(y);
        access_.set_right(x, access_.left(y));
        access_.set_left(y, x);
        access_.repair(x);
        access_.repair(y);
        return y;
    }

  public:
    explicit avl_rope(Access& access) : access_(access) {}
    // Precondition: the child-height difference is at most MaxImbalance+1, the node is
    // pushed, and both children are valid. The caller may retain only handles.
    [[gnu::always_inline]] handle rebalance(handle x) {
        const auto a = access_.left(x), b = access_.right(x);
        if (access_.height(a) > access_.height(b) + MaxImbalance) {
            access_.push(a);
            if (access_.height(access_.left(a)) < access_.height(access_.right(a)))
                access_.set_left(x, rotate_left(a));
            return rotate_right(x);
        }
        if (access_.height(b) > access_.height(a) + MaxImbalance) {
            access_.push(b);
            if (access_.height(access_.right(b)) < access_.height(access_.left(b)))
                access_.set_right(x, rotate_right(b));
            return rotate_left(x);
        }
        access_.repair(x);
        return x;
    }

  private:
    // Only the height-mismatched case needs a recursive descent. Keep that
    // out of the compatible-height join so callers can reuse a spare directly.
    // Precondition: both trees are nonempty and their height difference exceeds
    // MaxImbalance. ha/hb are their current heights.
    [[gnu::noinline]] handle
    join_unbalanced(handle a, handle b, handle spare, unsigned ha, unsigned hb) {
        if (ha > hb + MaxImbalance) {
            access_.push(a);
            const auto child = join(access_.right(a), b, spare);
            access_.set_right(a, child);
            return rebalance(a);
        }
        access_.push(b);
        const auto child = join(a, access_.left(b), spare);
        access_.set_left(b, child);
        return rebalance(b);
    }

  public:
    [[gnu::always_inline]] handle join(handle a, handle b, handle spare = 0) {
        if (!a || !b) {
            if (spare)
                access_.release_branch(spare);
            return a ? a : b;
        }
        const auto ha = access_.height(a), hb = access_.height(b);
        if (ha > hb + MaxImbalance || hb > ha + MaxImbalance)
            return join_unbalanced(a, b, spare, ha, hb);
        return spare ? access_.reuse_branch(spare, a, b) : access_.make_branch(a, b);
    }
    std::pair<handle, handle> split(handle x, std::uint32_t position) {
        if (position == 0)
            return {0, x};
        if (position == access_.length(x))
            return {x, 0};
        if (access_.is_leaf(x))
            return access_.split_leaf(x, position);
        access_.push(x);
        const auto a = access_.left(x), b = access_.right(x), n = access_.length(a);
        if (position == n) {
            access_.release_branch(x);
            return {a, b};
        }
        if (position < n) {
            const auto [first, second] = split(a, position);
            return {first, join(second, b, x)};
        }
        const auto [first, second] = split(b, position - n);
        return {join(a, first, x), second};
    }
    // Reconstruct a split from an already exposed root-to-leaf path.
    // All path branches must already be pushed. The nonempty replacement parts
    // occupy the old leaf's position; at least one part must be nonempty.
    // RefreshPath also repairs the initially retained spine after leaf content
    // or length changes. All other repairs are performed by join, once.
    //
    // At most one branch becomes unused: after the first separation both parts
    // are nonempty, so later branches are reused by join. Its handle stays live
    // and must be consumed as a join spare or explicitly released by the caller.
    struct split_result {
        handle first, second, spare;
    };
    template <bool RefreshPath = false>
    split_result split_from_leaf_retained(handle leaf,
                                          handle first,
                                          handle second,
                                          std::span<const handle> path) {
        handle spare = 0;
        auto child = leaf;
        for (auto i = path.size(); i--;) {
            const auto parent = path[i];
            const auto left = access_.left(parent), right = access_.right(parent);
            if (child == left) {
                if (!first) {
                    if constexpr (RefreshPath)
                        access_.repair(parent);
                    second = parent;
                } else if (!second) {
                    second = right;
                    spare = parent;
                } else
                    second = join(second, right, parent);
            } else {
                if (!second) {
                    if constexpr (RefreshPath)
                        access_.repair(parent);
                    first = parent;
                } else if (!first) {
                    first = left;
                    spare = parent;
                } else
                    first = join(left, first, parent);
            }
            child = parent;
        }
        return {first, second, spare};
    }
    template <bool RefreshPath = false>
    std::pair<handle, handle> split_from_leaf(handle leaf,
                                             handle first,
                                             handle second,
                                             std::span<const handle> path) {
        auto result = split_from_leaf_retained<RefreshPath>(leaf, first, second, path);
        if (result.spare)
            access_.release_branch(result.spare);
        return {result.first, result.second};
    }
    // Returns {remaining tree, detached boundary leaf}. No allocation.
    template <bool Front> std::pair<handle, handle> pop_leaf(handle x) {
        if (access_.is_leaf(x))
            return {0, x};
        access_.push(x);
        const auto child = Front ? access_.left(x) : access_.right(x);
        const auto [rest, leaf] = pop_leaf<Front>(child);
        if (!rest) {
            const auto other = Front ? access_.right(x) : access_.left(x);
            access_.release_branch(x);
            return {other, leaf};
        }
        if constexpr (Front)
            access_.set_left(x, rest);
        else
            access_.set_right(x, rest);
        return {rebalance(x), leaf};
    }
};
} // namespace canard::structural
// ===== include/canard/sequence/detail/rope_boundary_fold.hpp =====
#include <cstdint>

namespace canard::detail {

// One read-only branch observation in the requested orientation. A pending
// reversal changes the order and the orientation of both children, not values.
struct oriented_rope_children {
    std::uint32_t first;
    std::uint32_t second;
    unsigned first_length;
    bool reversed;
};

// Ordered two-boundary fold with no pending value actions. Reader supplies the
// summaries and the interpretation of a leaf. This scheduler knows no sums,
// inverses, packed words, mutable pointers, or SIMD instructions.
template <typename Reader> class rope_boundary_fold {
    using summary_type = summary_type_t<Reader>;
    using handle = std::uint32_t;
    Reader read_;

  public:
    explicit rope_boundary_fold(Reader reader) : read_(reader) {}

    // Precondition: x is nonempty and 0 <= first < last <= length(x).
    [[nodiscard]] summary_type operator()(handle x, unsigned first, unsigned last) const {
        bool flip = false;
        while (!read_.is_leaf(x)) {
            if (first == 0 && last == read_.length(x))
                return read_.whole(x, flip);
            const auto branch = read_.children(x, flip);
            const auto count = branch.first_length;
            flip = branch.reversed;
            if (last <= count) {
                x = branch.first;
                continue;
            }
            if (first >= count) {
                x = branch.second;
                first -= count;
                last -= count;
                continue;
            }

            handle left = branch.first, right = branch.second;
            last -= count;
            bool left_flip = flip, right_flip = flip;
            auto suffix = read_.identity(), prefix = read_.identity();
            // Interleave the independent endpoint paths. Covered right siblings
            // prepend to the left accumulator; left siblings append to the right.
            while (!read_.is_leaf(left) || !read_.is_leaf(right)) {
                if (!read_.is_leaf(left)) {
                    const auto child = read_.children(left, left_flip);
                    left_flip = child.reversed;
                    if (first < child.first_length) {
                        suffix = read_.combine(read_.whole(child.second, left_flip), suffix);
                        left = child.first;
                    } else {
                        left = child.second;
                        first -= child.first_length;
                    }
                }
                if (!read_.is_leaf(right)) {
                    const auto child = read_.children(right, right_flip);
                    right_flip = child.reversed;
                    if (last <= child.first_length) {
                        right = child.first;
                    } else {
                        prefix = read_.combine(prefix, read_.whole(child.first, right_flip));
                        right = child.second;
                        last -= child.first_length;
                    }
                }
            }
            suffix = read_.combine(
                read_.leaf_fold(left, first, read_.length(left), left_flip), suffix);
            prefix = read_.combine(prefix, read_.leaf_fold(right, 0, last, right_flip));
            return read_.combine(suffix, prefix);
        }
        return read_.leaf_fold(x, first, last, flip);
    }
};

} // namespace canard::detail
#include <algorithm>
#include <cassert>
#include <bit>
#include <cstdint>
#include <iterator>
#include <ranges>
#include <utility>
#include <vector>

namespace canard::detail {
// Online sequence owner/maintenance engine. Leaf kernels specify algebra,
// representation and execution; avl_rope schedules structural changes.
template <typename Kernel, typename Configuration> class chunked_engine {
  public:
    using value_type = value_type_t<Kernel>;
    using summary_type = summary_type_t<Kernel>;
    using update_type = update_type_t<Kernel>;
    using action_type = action_type_t<Kernel>;
    using prepared_action = prepared_action_t<Kernel>;
    using leaf_type = leaf_type_t<Kernel>;
    using size_type = std::uint32_t;
    static constexpr unsigned leaf_capacity = Configuration::leaf_capacity;
    static constexpr bool nothrow_mutation = Kernel::nothrow_mutation;
    static constexpr bool has_lazy = Kernel::has_lazy;

  private:
    using handle = std::uint32_t;
    static constexpr unsigned B = leaf_capacity;
    static constexpr unsigned height_tolerance = Configuration::height_tolerance;
    // At least one factor of two in leaf count for every tolerance+1 levels.
    // This bound also covers legacy strict-AVL configurations and tiny trees.
    static constexpr unsigned path_capacity =
        (height_tolerance + 1) * std::bit_width(Configuration::max_size) + 1;
    static constexpr unsigned minimum_occupancy = B / Configuration::occupancy_divisor;
    static constexpr unsigned reversed_flag = 1, dirty_flag = 2;
    struct node {
        handle left = 0, right = 0;
        size_type length = 0;
        handle leaf = 0;
        [[no_unique_address]] action_type action;
        summary_type summary;
        using occupancy_type =
            std::conditional_t<(minimum_occupancy < 256), std::uint8_t, std::uint16_t>;
        occupancy_type front = 0, back = 0;
        std::uint8_t height = 0, flags = 0;
    };
    [[no_unique_address]] Kernel kernel_;
    memory::indexed_pool<node> nodes_;
    memory::indexed_pool<leaf_type> leaves_;
    handle root_ = 0;

    // This is the complete topology interface. The scheduler cannot access
    // value arrays or infer the meaning of an action or a summary.
    struct topology_access {
        chunked_engine& owner;
        unsigned height(handle x) const {
            return x ? owner.nodes_[x].height : 0;
        }
        size_type length(handle x) const {
            return x ? owner.nodes_[x].length : 0;
        }
        bool is_leaf(handle x) const {
            return owner.nodes_[x].leaf != 0;
        }
        handle left(handle x) const {
            return owner.nodes_[x].left;
        }
        handle right(handle x) const {
            return owner.nodes_[x].right;
        }
        void set_left(handle x, handle child) const {
            owner.nodes_[x].left = child;
        }
        void set_right(handle x, handle child) const {
            owner.nodes_[x].right = child;
        }
        void push(handle x) const {
            owner.push_branch(x);
        }
        void repair(handle x) const {
            owner.repair_branch(x);
        }
        handle make_branch(handle a, handle b) const {
            return owner.make_branch(a, b);
        }
        handle reuse_branch(handle x, handle a, handle b) const {
            owner.nodes_[x].left = a;
            owner.nodes_[x].right = b;
            owner.repair_branch(x);
            return x;
        }
        void release_branch(handle x) const {
            owner.nodes_.release(x);
        }
        auto split_leaf(handle x, size_type k) const {
            return owner.split_leaf(x, k);
        }
    };
    using topology = structural::avl_rope<topology_access, height_tolerance>;
    [[nodiscard]] size_type length(handle x) const noexcept {
        return x ? nodes_[x].length : 0;
    }
    [[nodiscard]] unsigned height(handle x) const noexcept {
        return x ? nodes_[x].height : 0;
    }
    [[nodiscard]] bool is_leaf(handle x) const noexcept {
        return nodes_[x].leaf != 0;
    }
    [[nodiscard]] summary_type aggregate(handle x) const {
        return x ? nodes_[x].summary : kernel_.identity();
    }
    handle new_leaf() {
        const auto payload = leaves_.acquire_retained();
        const auto id = nodes_.acquire();
        auto& n = nodes_[id];
        n.leaf = payload;
        n.height = 1;
        n.action = kernel_.identity_action();
        n.summary = kernel_.identity();
        return id;
    }
    void release_leaf(handle x) noexcept {
        leaves_.release(nodes_[x].leaf);
        nodes_.release(x);
    }
    void repair_leaf(handle x, const summary_type& summary) {
        auto& n = nodes_[x];
        n.front = n.back = std::min<unsigned>(n.length, minimum_occupancy);
        n.summary = summary;
    }
    void repair_leaf(handle x) {
        const auto& n = nodes_[x];
        repair_leaf(x, kernel_.summarize(leaves_[n.leaf], 0, n.length));
    }
    [[nodiscard]] std::pair<summary_type, summary_type> partition_summary(
        const leaf_type& leaf, unsigned length, unsigned at, const summary_type& total) const {
        if constexpr (requires(const summary_type& part) {
                          kernel_.remove_prefix_summary(total, part);
                          kernel_.remove_suffix_summary(total, part);
                      }) {
            if (at <= length - at) {
                const auto first = kernel_.summarize(leaf, 0, at);
                return {first, kernel_.remove_prefix_summary(total, first)};
            }
            const auto second = kernel_.summarize(leaf, at, length);
            return {kernel_.remove_suffix_summary(total, second), second};
        } else
            return {kernel_.summarize(leaf, 0, at), kernel_.summarize(leaf, at, length)};
    }
    void repair_branch(handle x) {
        auto& n = nodes_[x];
        const auto& a = nodes_[n.left];
        const auto& b = nodes_[n.right];
        assert(n.left && n.right && n.flags == 0);
        n.length = a.length + b.length;
        n.height = 1 + std::max(a.height, b.height);
        n.front = a.front;
        n.back = b.back;
        n.summary = kernel_.combine(a.summary, b.summary);
    }
    handle make_branch(handle a, handle b) {
        const auto x = nodes_.acquire();
        auto& n = nodes_[x];
        n.left = a;
        n.right = b;
        n.action = kernel_.identity_action();
        repair_branch(x);
        return x;
    }
    void reverse_node(handle x) {
        if (!x)
            return;
        auto& n = nodes_[x];
        n.flags ^= reversed_flag;
        std::swap(n.left, n.right);
        std::swap(n.front, n.back);
        n.summary = kernel_.reversed(n.summary);
    }
    void apply_node(handle x, const action_type& action) {
        auto& n = nodes_[x];
        n.summary = kernel_.map(action, n.summary, n.length);
        if (n.flags & dirty_flag)
            n.action = kernel_.compose(action, n.action);
        else
            n.action = action;
        n.flags |= dirty_flag;
    }
    [[gnu::always_inline]] void push_branch(handle x) {
        auto& n = nodes_[x];
        assert(!n.leaf);
        if (n.flags & reversed_flag) {
            reverse_node(n.left);
            reverse_node(n.right);
        }
        if constexpr (has_lazy)
            if (n.flags & dirty_flag) {
                apply_node(n.left, n.action);
                apply_node(n.right, n.action);
            }
        n.flags = 0;
    }
    void push_leaf(handle x) {
        auto& n = nodes_[x];
        auto& data = leaves_[n.leaf];
        if constexpr (has_lazy)
            if (n.flags & dirty_flag)
                kernel_.materialize(data, n.length, n.action);
        if (n.flags & reversed_flag)
            kernel_.reverse_values(data, 0, n.length);
        n.flags = 0;
    }
    std::pair<handle, handle> split_leaf(handle x, unsigned at) {
        push_leaf(x);
        const auto old_size = nodes_[x].length;
        const auto summaries =
            partition_summary(leaves_[nodes_[x].leaf], old_size, at, nodes_[x].summary);
        const auto y = new_leaf(); // retain no references across pool growth
        kernel_.copy_values(leaves_[nodes_[y].leaf], 0, leaves_[nodes_[x].leaf], at, old_size - at);
        nodes_[x].length = at;
        nodes_[y].length = old_size - at;
        repair_leaf(x, summaries.first);
        repair_leaf(y, summaries.second);
        return {x, y};
    }
    // Repair the two edge leaves in place. Do not detach and then reinsert
    // leaves whose positions do not change: their recorded spines need only
    // one bottom-up summary repair. A merge deletes one leaf and its parent.
    handle concatenate(handle a, handle b) {
        if (!a)
            return b;
        if (!b)
            return a;
        topology_access access{*this};
        topology tree{access};
        if (nodes_[a].back >= minimum_occupancy && nodes_[b].front >= minimum_occupancy)
            return tree.join(a, b);
        std::array<handle, path_capacity> apath, bpath;
        unsigned an = 0, bn = 0;
        handle x = a, y = b;
        while (!is_leaf(x)) {
            push_branch(x);
            apath[an++] = x;
            x = nodes_[x].right;
        }
        while (!is_leaf(y)) {
            push_branch(y);
            bpath[bn++] = y;
            y = nodes_[y].left;
        }
        push_leaf(x);
        push_leaf(y);
        const unsigned nx = nodes_[x].length, ny = nodes_[y].length, total = nx + ny;
        auto& left = leaves_[nodes_[x].leaf];
        auto& right = leaves_[nodes_[y].leaf];
        if (total <= B) {
            const auto combined = kernel_.combine(nodes_[x].summary, nodes_[y].summary);
            kernel_.copy_values(left, nx, right, 0, ny);
            nodes_[x].length = total;
            nodes_[x].front = nodes_[x].back = std::min<unsigned>(total, minimum_occupancy);
            nodes_[x].summary = combined;
            release_leaf(y);
            if (bn == 0)
                b = 0;
            else {
                const auto parent = bpath[--bn];
                b = nodes_[parent].right;
                nodes_.release(parent);
                while (bn) {
                    const auto parent = bpath[--bn];
                    nodes_[parent].left = b;
                    b = tree.rebalance(parent);
                }
            }
            while (an)
                repair_branch(apath[--an]);
            return concatenate(a, b);
        }
        const unsigned target = total / 2;
        if (nx < target) {
            const unsigned moved = target - nx;
            kernel_.copy_values(left, nx, right, 0, moved);
            std::move(
                right.values.begin() + moved, right.values.begin() + ny, right.values.begin());
        } else if (nx > target) {
            const unsigned moved = nx - target;
            std::move_backward(
                right.values.begin(), right.values.begin() + ny, right.values.begin() + ny + moved);
            kernel_.copy_values(right, 0, left, target, moved);
        }
        nodes_[x].length = target;
        nodes_[y].length = total - target;
        repair_leaf(x);
        repair_leaf(y);
        while (an)
            repair_branch(apath[--an]);
        while (bn)
            repair_branch(bpath[--bn]);
        return tree.join(a, b);
    }
    handle insert_impl(handle x, unsigned position, const value_type& value) {
        if (!x) {
            x = new_leaf();
            auto& n = nodes_[x];
            n.length = n.front = n.back = 1;
            leaves_[n.leaf].values[0] = kernel_.import_value(value);
            n.summary = kernel_.summarize(leaves_[n.leaf], 0, 1);
            return x;
        }
        if (is_leaf(x)) {
            push_leaf(x);
            if (nodes_[x].length == B) {
                const auto [a, b] = split_leaf(x, B / 2);
                handle l = a, r = b;
                if (position <= B / 2)
                    l = insert_impl(a, position, value);
                else
                    r = insert_impl(b, position - B / 2, value);
                return make_branch(l, r);
            }
            auto& n = nodes_[x];
            auto& leaf = leaves_[n.leaf];
            const auto word = kernel_.import_value(value);
            kernel_.insert_value(leaf, position, n.length, word);
            ++n.length;
            n.front = n.back = std::min<unsigned>(n.length, minimum_occupancy);
            n.summary = kernel_.after_insert(leaf, n.length, n.summary, word);
            return x;
        }
        push_branch(x);
        const unsigned n = length(nodes_[x].left);
        if (position < n) {
            const auto child = insert_impl(nodes_[x].left, position, value);
            nodes_[x].left = child;
        } else {
            const auto child = insert_impl(nodes_[x].right, position - n, value);
            nodes_[x].right = child;
        }
        topology_access access{*this};
        return topology{access}.rebalance(x);
    }
    handle erase_impl(handle x, unsigned position) {
        if (is_leaf(x)) {
            push_leaf(x);
            auto& n = nodes_[x];
            auto& leaf = leaves_[n.leaf];
            const auto old = leaf.values[position];
            kernel_.erase_value(leaf, position, n.length);
            --n.length;
            if (!n.length) {
                release_leaf(x);
                return 0;
            }
            n.front = n.back = std::min<unsigned>(n.length, minimum_occupancy);
            n.summary = kernel_.after_erase(leaf, n.length, n.summary, old);
            return x;
        }
        push_branch(x);
        const unsigned n = length(nodes_[x].left);
        if (position < n) {
            const auto child = erase_impl(nodes_[x].left, position);
            nodes_[x].left = child;
        } else {
            const auto child = erase_impl(nodes_[x].right, position - n);
            nodes_[x].right = child;
        }
        const auto a = nodes_[x].left, b = nodes_[x].right;
        if (!a || !b) {
            nodes_.release(x);
            return a ? a : b;
        }
        if (nodes_[a].back < minimum_occupancy || nodes_[b].front < minimum_occupancy) {
            nodes_.release(x);
            return concatenate(a, b);
        }
        topology_access access{*this};
        topology tree{access};
        if (height(a) > height(b) + height_tolerance + 1 ||
            height(b) > height(a) + height_tolerance + 1) {
            nodes_.release(x);
            return tree.join(a, b);
        }
        return tree.rebalance(x);
    }
    [[nodiscard]] summary_type fold_oriented(handle x, unsigned l, unsigned r, bool flip) const {
        if (!flip)
            return fold_impl(x, l, r);
        return kernel_.reversed(fold_impl(x, length(x) - r, length(x) - l));
    }
    [[nodiscard]] summary_type fold_impl(handle x, unsigned l, unsigned r) const {
        const auto& n = nodes_[x];
        if (l == 0 && r == n.length)
            return n.summary;
        summary_type answer;
        const bool reverse = n.flags & reversed_flag;
        if (n.leaf) {
            answer = reverse ? kernel_.reversed(
                                   kernel_.summarize(leaves_[n.leaf], n.length - r, n.length - l))
                             : kernel_.summarize(leaves_[n.leaf], l, r);
        } else {
            const auto count = length(n.left);
            if (r <= count)
                answer = fold_oriented(n.left, l, r, reverse);
            else if (l >= count)
                answer = fold_oriented(n.right, l - count, r - count, reverse);
            else
                answer = kernel_.combine(fold_oriented(n.left, l, count, reverse),
                                         fold_oriented(n.right, 0, r - count, reverse));
        }
        if constexpr (has_lazy)
            if (n.flags & dirty_flag)
                answer = kernel_.map(n.action, answer, r - l);
        return answer;
    }
    struct plain_reader {
        using summary_type = summary_type_t<Kernel>;
        const chunked_engine& owner;

        [[nodiscard]] bool is_leaf(handle x) const noexcept {
            return owner.is_leaf(x);
        }
        [[nodiscard]] unsigned length(handle x) const noexcept {
            return owner.length(x);
        }
        [[nodiscard]] summary_type identity() const {
            return owner.kernel_.identity();
        }
        [[nodiscard]] summary_type combine(const summary_type& a, const summary_type& b) const {
            return owner.kernel_.combine(a, b);
        }
        [[nodiscard]] summary_type whole(handle x, bool flip) const {
            const auto& summary = owner.nodes_[x].summary;
            return flip ? owner.kernel_.reversed(summary) : summary;
        }
        [[nodiscard]] oriented_rope_children children(handle x, bool flip) const noexcept {
            const auto& n = owner.nodes_[x];
            const auto a = flip ? n.right : n.left;
            const auto b = flip ? n.left : n.right;
            return {a, b, owner.length(a), flip != bool(n.flags & reversed_flag)};
        }
        [[nodiscard]] summary_type
        leaf_fold(handle x, unsigned first, unsigned last, bool flip) const {
            const auto& n = owner.nodes_[x];
            const auto& leaf = owner.leaves_[n.leaf];
            flip ^= bool(n.flags & reversed_flag);
            if (flip)
                return owner.kernel_.reversed(
                    owner.kernel_.summarize(leaf, n.length - last, n.length - first));
            return owner.kernel_.summarize(leaf, first, last);
        }
    };

    void apply_impl(handle x, unsigned l, unsigned r, const prepared_action& prepared) {
        if (l == 0 && r == length(x)) {
            apply_node(x, kernel_.action_of(prepared));
            return;
        }
        if (is_leaf(x)) {
            push_leaf(x);
            auto& n = nodes_[x];
            n.summary = kernel_.update_leaf(leaves_[n.leaf], n.length, l, r, prepared, n.summary);
            return;
        }
        push_branch(x);
        const auto a = nodes_[x].left, b = nodes_[x].right, n = length(a);
        if (r <= n)
            apply_impl(a, l, r, prepared);
        else if (l >= n)
            apply_impl(b, l - n, r - n, prepared);
        else {
            apply_impl(a, l, n, prepared);
            apply_impl(b, 0, r - n, prepared);
        }
        repair_branch(x);
    }
    void set_impl(handle x, unsigned position, const value_type& value) {
        if (is_leaf(x)) {
            push_leaf(x);
            auto& n = nodes_[x];
            auto& leaf = leaves_[n.leaf];
            const auto old = leaf.values[position], word = kernel_.import_value(value);
            leaf.values[position] = word;
            n.summary = kernel_.after_set(leaf, n.length, n.summary, old, word);
            return;
        }
        push_branch(x);
        const auto a = nodes_[x].left, b = nodes_[x].right, n = length(a);
        if (position < n)
            set_impl(a, position, value);
        else
            set_impl(b, position - n, value);
        repair_branch(x);
    }
    // Reverse an interval using the two captured boundary paths. Exchange only the pieces OUTSIDE
    // the requested interval between the two boundary blocks (in reverse order), then reverse the
    // enclosing interval of whole blocks. No fragmentation or occupancy repair is necessary when
    // both replacement sizes fit.
    [[gnu::noinline]] handle
    reverse_blocks(handle x, handle a, handle b, unsigned l, unsigned r, unsigned left_length) {
        std::array<handle, path_capacity> apath, bpath;
        unsigned an = 0, bn = 0;
        handle u = a, v = b;
        unsigned i = l, j = r - left_length - 1;
        // Independent spines: expose both incrementally rather than completing
        // one dependent descent before starting the other. No lookahead beyond
        // the current operation, speculative mutation, or cached node pointers.
        const auto descend = [&](handle& at, unsigned& position, auto& path, unsigned& count) {
            push_branch(at);
            path[count++] = at;
            const auto left = nodes_[at].left, right = nodes_[at].right;
            const unsigned n = nodes_[left].length;
            const bool go_right = position >= n;
            at = go_right ? right : left;
            position -= go_right ? n : 0;
        };
        while (!is_leaf(u) && !is_leaf(v)) {
            descend(u, i, apath, an);
            descend(v, j, bpath, bn);
        }
        while (!is_leaf(u))
            descend(u, i, apath, an);
        while (!is_leaf(v))
            descend(v, j, bpath, bn);
        ++j;
        const auto nu = nodes_[u].length, nv = nodes_[v].length;
        const auto tail = nv - j, newu = tail + nu - i, newv = j + i;
        topology_access access{*this};
        topology tree{access};
        if (newu < minimum_occupancy || newv < minimum_occupancy || newu > B || newv > B) {
            const auto left_parts = i ? split_leaf(u, i) : std::pair<handle, handle>{0, u};
            const auto right_parts = j < nv ? split_leaf(v, j) : std::pair<handle, handle>{v, 0};
            const auto [first, left] =
                tree.split_from_leaf(u, left_parts.first, left_parts.second, {apath.data(), an});
            const auto [right, last] =
                tree.split_from_leaf(v, right_parts.first, right_parts.second, {bpath.data(), bn});
            reverse_node(left);
            reverse_node(right);
            const auto middle = tree.join(right, left, x);
            return concatenate(concatenate(first, middle), last);
        }
        if (i || tail) {
            if constexpr (!has_lazy && requires(leaf_type& leaf) {
                              kernel_.exchange_reversed_outer_parts_oriented(
                                  leaf, 0u, 0u, false, leaf, 0u, 0u, false);
                          }) {
                auto& left = leaves_[nodes_[u].leaf];
                auto& right = leaves_[nodes_[v].leaf];
                const bool left_flip = nodes_[u].flags & reversed_flag;
                const bool right_flip = nodes_[v].flags & reversed_flag;
                const auto parts = [&](const leaf_type& leaf, unsigned n, unsigned at,
                                       const summary_type& total, bool flip) {
                    if (!flip)
                        return partition_summary(leaf, n, at, total);
                    const auto [a, b] =
                        partition_summary(leaf, n, n - at, kernel_.reversed(total));
                    return std::pair{kernel_.reversed(b), kernel_.reversed(a)};
                };
                const auto left_summaries = parts(left, nu, i, nodes_[u].summary, left_flip);
                const auto right_summaries = parts(right, nv, j, nodes_[v].summary, right_flip);
                const auto new_left_summary =
                    kernel_.combine(kernel_.reversed(right_summaries.second), left_summaries.second);
                const auto new_right_summary =
                    kernel_.combine(right_summaries.first, kernel_.reversed(left_summaries.first));
                kernel_.exchange_reversed_outer_parts_oriented(left, nu, i, left_flip,
                                                               right, nv, j, right_flip);
                nodes_[u].length = newu;
                nodes_[v].length = newv;
                repair_leaf(u, new_left_summary);
                repair_leaf(v, new_right_summary);
            } else {
                push_leaf(u);
                push_leaf(v);
                auto& left = leaves_[nodes_[u].leaf];
                auto& right = leaves_[nodes_[v].leaf];
                const auto left_summaries = partition_summary(left, nu, i, nodes_[u].summary);
                const auto right_summaries = partition_summary(right, nv, j, nodes_[v].summary);
                const auto new_left_summary =
                    kernel_.combine(kernel_.reversed(right_summaries.second), left_summaries.second);
                const auto new_right_summary =
                    kernel_.combine(right_summaries.first, kernel_.reversed(left_summaries.first));
                kernel_.exchange_reversed_outer_parts(left, nu, i, right, nv, j);
                nodes_[u].length = newu;
                nodes_[v].length = newv;
                repair_leaf(u, new_left_summary);
                repair_leaf(v, new_right_summary);
            }
        }
        // Refresh only the retained initial spines; reconstructed ancestors are
        // repaired by join. Do not repair both complete paths first.
        const auto [first, left, spare_left] =
            tree.template split_from_leaf_retained<true>(u, 0, u, {apath.data(), an});
        const auto [right, last, spare_right] =
            tree.template split_from_leaf_retained<true>(v, v, 0, {bpath.data(), bn});
        reverse_node(left);
        reverse_node(right);
        const auto middle = tree.join(right, left, x);
        return tree.join(tree.join(first, middle, spare_left), last, spare_right);
    }
    handle reverse_impl(handle x, unsigned l, unsigned r) {
        if (l == 0 && r == length(x)) {
            reverse_node(x);
            return x;
        }
        if (is_leaf(x)) {
            push_leaf(x);
            auto& n = nodes_[x];
            auto& leaf = leaves_[n.leaf];
            kernel_.reverse_values(leaf, l, r);
            if constexpr (requires { kernel_.after_reverse(leaf, n.length, l, r, n.summary); })
                repair_leaf(x, kernel_.after_reverse(leaf, n.length, l, r, n.summary));
            else
                repair_leaf(x);
            return x;
        }
        push_branch(x);
        const auto a = nodes_[x].left, b = nodes_[x].right, n = length(a);
        topology_access access{*this};
        topology tree{access};
        if (r <= n || l >= n) {
            if (r <= n) {
                const auto child = reverse_impl(a, l, r);
                nodes_[x].left = child;
            } else {
                const auto child = reverse_impl(b, l - n, r - n);
                nodes_[x].right = child;
            }
            const auto left = nodes_[x].left, right = nodes_[x].right;
            if (nodes_[left].back < minimum_occupancy || nodes_[right].front < minimum_occupancy) {
                nodes_.release(x);
                return concatenate(left, right);
            }
            if (height(left) > height(right) + height_tolerance + 1 ||
                height(right) > height(left) + height_tolerance + 1) {
                nodes_.release(x);
                return tree.join(left, right);
            }
            return tree.rebalance(x);
        }
        return reverse_blocks(x, a, b, l, r, n);
    }
    template <typename Output>
    void materialize_impl(handle x, const action_type& outer, bool flip, Output& out) const {
        const auto& n = nodes_[x];
        const auto action = [&] {
            if constexpr (has_lazy) {
                if (n.flags & dirty_flag)
                    return kernel_.compose(outer, n.action);
            }
            return outer;
        }();
        const bool reverse = flip != bool(n.flags & reversed_flag);
        if (n.leaf) {
            for (unsigned i = 0; i < n.length; ++i) {
                const unsigned j = reverse ? n.length - 1 - i : i;
                const auto one = kernel_.summarize(leaves_[n.leaf], j, j + 1);
                *out++ = kernel_.export_summary(kernel_.map(action, one, 1));
            }
        } else {
            materialize_impl(flip ? n.right : n.left, action, reverse, out);
            materialize_impl(flip ? n.left : n.right, action, reverse, out);
        }
    }
    bool validate_impl(handle x, bool first, bool last, bool flip) const {
        const auto& n = nodes_[x];
        if (n.leaf)
            return n.length > 0 && n.length <= B && n.height == 1 &&
                   n.front == std::min<unsigned>(n.length, minimum_occupancy) &&
                   n.back == std::min<unsigned>(n.length, minimum_occupancy) &&
                   (first || last || n.length >= minimum_occupancy);
        if (!n.left || !n.right)
            return false;
        const auto& a = nodes_[n.left];
        const auto& b = nodes_[n.right];
        const bool reversed = n.flags & reversed_flag;
        if (n.length != a.length + b.length || n.height != 1 + std::max(a.height, b.height) ||
            std::abs(int(a.height) - int(b.height)) > int(height_tolerance) ||
            n.front != (reversed ? a.back : a.front) || n.back != (reversed ? b.front : b.back))
            return false;
        const bool child_flip = flip != reversed;
        return validate_impl(flip ? n.right : n.left, first, false, child_flip) &&
               validate_impl(flip ? n.left : n.right, false, last, child_flip);
    }
    handle build_balanced(const std::vector<handle>& leaves, unsigned l, unsigned r) {
        if (r - l == 1)
            return leaves[l];
        const auto m = (l + r) / 2;
        const auto a = build_balanced(leaves, l, m), b = build_balanced(leaves, m, r);
        return make_branch(a, b);
    }

  public:
    explicit chunked_engine(Kernel kernel) : kernel_(std::move(kernel)) {
        if constexpr (requires { Kernel::max_size; })
            static_assert(Configuration::max_size <= Kernel::max_size);
    }
    template <std::ranges::input_range R>
    chunked_engine(R&& values, Kernel kernel) : chunked_engine(std::move(kernel)) {
        std::vector<handle> leaves;
        if constexpr (std::ranges::sized_range<R>) {
            assert(std::ranges::size(values) <= Configuration::max_size);
            const auto count = (std::ranges::size(values) + B - 1) / B;
            nodes_.reserve(2 * count + 32);
            leaves_.reserve(count + 16);
            leaves.reserve(count);
        }
        handle current = 0;
        for (auto&& value : values) {
            if (!current || nodes_[current].length == B) {
                if (current)
                    repair_leaf(current);
                current = new_leaf();
                leaves.push_back(current);
            }
            auto& n = nodes_[current];
            leaves_[n.leaf].values[n.length++] = kernel_.import_value(value);
        }
        if (current) {
            repair_leaf(current);
            root_ = build_balanced(leaves, 0, leaves.size());
        }
        assert(size() <= Configuration::max_size);
    }
    chunked_engine(const chunked_engine&) = default;
    chunked_engine& operator=(const chunked_engine& other) {
        if (this != &other) {
            chunked_engine next{other};
            swap(next);
        }
        return *this;
    }
    chunked_engine(chunked_engine&& other) noexcept(std::is_nothrow_copy_constructible_v<Kernel>)
        : kernel_(other.kernel_), nodes_(std::move(other.nodes_)),
          leaves_(std::move(other.leaves_)), root_(std::exchange(other.root_, 0)) {}
    chunked_engine& operator=(chunked_engine&& other) {
        if (this != &other) {
            kernel_ = other.kernel_;
            nodes_ = std::move(other.nodes_);
            leaves_ = std::move(other.leaves_);
            root_ = std::exchange(other.root_, 0);
        }
        return *this;
    }
    void swap(chunked_engine& other) noexcept
        requires(std::is_nothrow_swappable_v<Kernel>)
    {
        using std::swap;
        swap(kernel_, other.kernel_);
        nodes_.swap(other.nodes_);
        leaves_.swap(other.leaves_);
        swap(root_, other.root_);
    }
    [[nodiscard]] size_type size() const noexcept {
        return length(root_);
    }
    [[nodiscard]] bool empty() const noexcept {
        return !root_;
    }
    [[nodiscard]] unsigned height() const noexcept {
        return height(root_);
    }
    [[nodiscard]] std::size_t leaf_count() const noexcept {
        return leaves_.live_slots();
    }
    [[nodiscard]] std::size_t allocated_bytes() const noexcept {
        return nodes_.allocated_bytes() + leaves_.allocated_bytes();
    }
    [[nodiscard]] value_type fold(size_type first, size_type last) const {
        assert(first <= last && last <= size());
        if (first == last)
            return kernel_.export_summary(kernel_.identity());
        if constexpr (has_lazy)
            return kernel_.export_summary(fold_impl(root_, first, last));
        else
            return kernel_.export_summary(
                rope_boundary_fold{plain_reader{*this}}(root_, first, last));
    }
    [[nodiscard]] value_type all_fold() const {
        return kernel_.export_summary(aggregate(root_));
    }
    [[nodiscard]] value_type get(size_type position) const {
        return fold(position, position + 1);
    }
    void insert(size_type position, const value_type& value) {
        assert(position <= size() && size() < Configuration::max_size);
        root_ = insert_impl(root_, position, value);
    }
    void erase(size_type position) {
        assert(position < size());
        root_ = erase_impl(root_, position);
    }
    void set(size_type position, const value_type& value) noexcept
        requires(nothrow_mutation)
    {
        assert(position < size());
        set_impl(root_, position, value);
    }
    void apply(size_type first, size_type last, const update_type& update) noexcept
        requires(has_lazy && nothrow_mutation)
    {
        assert(first <= last && last <= size());
        if (first != last)
            apply_impl(root_, first, last, kernel_.prepare(update));
    }
    void reverse(size_type first, size_type last) {
        assert(first <= last && last <= size());
        if (last - first > 1)
            root_ = reverse_impl(root_, first, last);
    }
    template <typename Output> Output materialize(Output out) const {
        if (root_)
            materialize_impl(root_, kernel_.identity_action(), false, out);
        return out;
    }
    [[nodiscard]] std::vector<value_type> snapshot() const {
        std::vector<value_type> result;
        result.reserve(size());
        materialize(std::back_inserter(result));
        return result;
    }
    [[nodiscard]] bool check_invariants() const {
        return !root_ || validate_impl(root_, true, true, false);
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
template <typename Owner> class point_reference {
    Owner* owner_;
    std::size_t position_;

  public:
    using value_type = value_type_t<Owner>;
    point_reference(Owner& owner, std::size_t position) noexcept
        : owner_(&owner), position_(position) {}
    point_reference(const point_reference&) noexcept = default;
    [[nodiscard]] value_type value() const {
        return owner_->get(position_);
    }
    operator value_type() const {
        return value();
    }
    const point_reference& operator=(const value_type& replacement) const noexcept
        requires(Owner::nothrow_mutation)
    {
        owner_->set(position_, replacement);
        return *this;
    }
    const point_reference& operator=(const point_reference& other) const
        requires(Owner::nothrow_mutation)
    {
        const auto replacement = other.value();
        owner_->set(position_, replacement);
        return *this;
    }
    template <typename Other>
    const point_reference& operator=(const point_reference<Other>& other) const
        requires(Owner::nothrow_mutation && std::same_as<value_type, value_type_t<Other>>)
    {
        const auto replacement = other.value();
        owner_->set(position_, replacement);
        return *this;
    }
    template <typename Tag>
    void apply(const Tag& action) const
        requires requires(Owner& owner) { owner.apply(std::size_t{}, std::size_t{}, action); }
    {
        owner_->apply(position_, position_ + 1, action);
    }
};

template <typename Owner> class interval_reference {
    Owner* owner_;
    std::size_t first_, last_;

  public:
    interval_reference(Owner& owner, std::size_t first, std::size_t last) noexcept
        : owner_(&owner), first_(first), last_(last) {}
    [[nodiscard]] auto fold() const {
        return owner_->fold(first_, last_);
    }
    template <typename Tag>
    void apply(const Tag& action) const
        requires requires(Owner& owner) { owner.apply(std::size_t{}, std::size_t{}, action); }
    {
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
template <typename Owner>
    requires std::formattable<canard::value_type_t<Owner>, char>
struct std::formatter<canard::point_reference<Owner>, char>
    : std::formatter<canard::value_type_t<Owner>, char> {
    template <typename Context>
    auto format(const canard::point_reference<Owner>& reference, Context& context) const {
        const auto snapshot = reference.value();
        return std::formatter<canard::value_type_t<Owner>, char>::format(snapshot, context);
    }
};

template <> struct std::formatter<canard::wide_tree_description, char> {
    constexpr auto parse(std::format_parse_context& context) {
        return context.begin();
    }
    template <typename Context>
    auto format(canard::wide_tree_description x, Context& context) const {
        return std::format_to(context.out(),
                              "{}(size={}, fanout={}, height={}, storage={} bytes)",
                              x.lazy ? "wide_lazy_segment_tree" : "wide_segment_tree",
                              x.size,
                              x.fanout,
                              x.height,
                              x.storage_bytes);
    }
};
#include <span>
namespace canard {
struct chunked_sequence_description {
    std::size_t size;
    unsigned leaf_capacity;
    unsigned height;
    std::size_t leaf_count;
    std::size_t allocated_bytes;
};
template <algebra::monoid Monoid,
          typename Action,
          typename Configuration = sequence::configuration<>>
    requires(algebra::action_for<Action, Monoid> &&
             sequence::supported_configuration<Configuration, Monoid, Action>)
class chunked_lazy_sequence
    : public detail::chunked_engine<sequence::kernel_for_t<Configuration, Monoid, Action>,
                                    Configuration> {
    using kernel = sequence::kernel_for_t<Configuration, Monoid, Action>;
    using base = detail::chunked_engine<kernel, Configuration>;

  public:
    using value_type = value_type_t<base>;
    using tag_type = tag_type_t<Action>;
    using configuration_type = Configuration;
    template <std::ranges::input_range R>
    explicit chunked_lazy_sequence(R&& values,
                                   Monoid monoid = {},
                                   Action action = {},
                                   Configuration = {})
        : base(std::forward<R>(values), kernel{std::move(monoid), std::move(action)}) {}
    explicit chunked_lazy_sequence(Monoid monoid = {}, Action action = {}, Configuration = {})
        : base(kernel{std::move(monoid), std::move(action)}) {}
    [[nodiscard]] point_reference<chunked_lazy_sequence>
    operator[](std::size_t position) & noexcept {
        return {*this, position};
    }
    [[nodiscard]] value_type operator[](std::size_t position) const& {
        return this->get(position);
    }
    void operator[](std::size_t) && = delete;
    void operator[](std::size_t) const&& = delete;
    [[nodiscard]] interval_reference<chunked_lazy_sequence>
    operator[](std::size_t first, std::size_t last) & noexcept {
        return {*this, first, last};
    }
    void operator[](std::size_t, std::size_t) && = delete;
    void operator[](std::size_t, std::size_t) const&& = delete;
    [[nodiscard]] chunked_sequence_description description() const noexcept {
        return {this->size(),
                Configuration::leaf_capacity,
                this->height(),
                this->leaf_count(),
                this->allocated_bytes()};
    }
};
template <std::ranges::input_range R, algebra::monoid M, typename A, typename C>
chunked_lazy_sequence(R&&, M, A, C) -> chunked_lazy_sequence<M, A, C>;
template <std::ranges::input_range R, algebra::monoid M, typename A>
chunked_lazy_sequence(R&&, M, A) -> chunked_lazy_sequence<M, A>;
} // namespace canard

template <> struct std::formatter<canard::chunked_sequence_description, char> {
    constexpr auto parse(std::format_parse_context& context) {
        return context.begin();
    }
    template <typename Context>
    auto format(canard::chunked_sequence_description x, Context& context) const {
        return std::format_to(context.out(),
                              "chunked_lazy_sequence(size={}, leaf_capacity={}, height={}, "
                              "leaves={}, allocated={} bytes)",
                              x.size,
                              x.leaf_capacity,
                              x.height,
                              x.leaf_count,
                              x.allocated_bytes);
    }
};
#include <utility>

namespace canard {

// No-action facade over the same owning engine, topology, storage and proxies.
// It retains insertion, erasure, replacement, reversal and immediate folds.
template <algebra::monoid Monoid, typename Configuration = sequence::configuration<>>
    requires sequence::supported_configuration<Configuration, Monoid, algebra::no_action>
class chunked_sequence : public chunked_lazy_sequence<Monoid, algebra::no_action, Configuration> {
    using base = chunked_lazy_sequence<Monoid, algebra::no_action, Configuration>;

  public:
    template <std::ranges::input_range R>
    explicit chunked_sequence(R&& values, Monoid monoid = {}, Configuration configuration = {})
        : base(std::forward<R>(values), std::move(monoid), {}, configuration) {}

    explicit chunked_sequence(Monoid monoid = {}, Configuration configuration = {})
        : base(std::move(monoid), {}, configuration) {}
};

template <std::ranges::input_range R, algebra::monoid Monoid, typename Configuration>
chunked_sequence(R&&, Monoid, Configuration) -> chunked_sequence<Monoid, Configuration>;
template <std::ranges::input_range R, algebra::monoid Monoid>
chunked_sequence(R&&, Monoid) -> chunked_sequence<Monoid>;

} // namespace canard
// ===== include/canard/sequence/compact_unsigned_sum_avx2.hpp =====
// ===== include/canard/sequence/compact_unsigned_sum.hpp =====
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
#include <cassert>
#include <limits>

namespace canard::sequence::representation {
// uint32 leaf values and uint64 sums. Every imported element must fit uint32.
// configuration::max_size < 2^31 ensures that every sum also fits uint64.
// This representation is never selected implicitly for an ordinary tree.
struct compact_u32 {};
} // namespace canard::sequence::representation

namespace canard::kernel {

template <unsigned Extent>
struct sequence_compact_sum_scalar : sequence_leaf_operations<std::uint32_t, Extent> {
    using value_type = std::uint64_t;
    using word_type = std::uint32_t;
    using summary_type = std::uint64_t;
    using base = sequence_leaf_operations<word_type, Extent>;
    using leaf_type = leaf_type_t<base>;
    using action_type = tag_type_t<algebra::no_action>;
    using update_type = action_type;
    using prepared_action = action_type;
    static constexpr bool has_lazy = false;
    static constexpr bool nothrow_mutation = true;

    sequence_compact_sum_scalar(algebra::sum<value_type>, algebra::no_action) noexcept {}

    [[nodiscard]] static constexpr summary_type identity() noexcept {
        return 0;
    }
    [[nodiscard]] static constexpr summary_type
    combine(summary_type a, summary_type b) noexcept {
        return a + b;
    }
    [[nodiscard]] static constexpr summary_type reversed(summary_type a) noexcept {
        return a;
    }
    [[nodiscard]] static constexpr value_type export_summary(summary_type a) noexcept {
        return a;
    }
    [[nodiscard]] static word_type import_value(value_type a) noexcept {
        assert(a <= std::numeric_limits<word_type>::max());
        return static_cast<word_type>(a);
    }
    [[nodiscard]] static constexpr action_type identity_action() noexcept {
        return {};
    }
    [[nodiscard]] static constexpr summary_type
    map(action_type, summary_type s, std::size_t) noexcept {
        return s;
    }
    [[nodiscard]] static constexpr summary_type
    remove_prefix_summary(summary_type total, summary_type part) noexcept {
        return total - part;
    }
    [[nodiscard]] static constexpr summary_type
    remove_suffix_summary(summary_type total, summary_type part) noexcept {
        return total - part;
    }
    [[nodiscard]] static summary_type
    summarize(const leaf_type& leaf, unsigned first, unsigned last) noexcept {
        summary_type total = 0;
        for (unsigned i = first; i < last; ++i)
            total += leaf.values[i];
        return total;
    }
    [[nodiscard]] static constexpr summary_type
    after_reverse(const leaf_type&, unsigned, unsigned, unsigned, summary_type old) noexcept {
        return old;
    }
    [[nodiscard]] static constexpr summary_type
    after_insert(const leaf_type&, unsigned, summary_type old, word_type value) noexcept {
        return old + value;
    }
    [[nodiscard]] static constexpr summary_type
    after_erase(const leaf_type&, unsigned, summary_type old, word_type value) noexcept {
        return old - value;
    }
    [[nodiscard]] static constexpr summary_type after_set(const leaf_type&,
                                                         unsigned,
                                                         summary_type old,
                                                         word_type before,
                                                         word_type after) noexcept {
        return old - before + after;
    }
};

} // namespace canard::kernel

namespace canard::sequence {

template <unsigned Extent>
struct kernel_binding<representation::compact_u32,
                      wide::execution::scalar,
                      algebra::sum<std::uint64_t>,
                      algebra::no_action,
                      Extent> {
    using type = kernel::sequence_compact_sum_scalar<Extent>;
};

} // namespace canard::sequence
// ===== include/canard/kernel/sequence_leaf_avx2.hpp =====
// ===== include/canard/simd/sequence_avx2.hpp =====
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
#include <algorithm>
namespace canard::simd {
// Copies [first,last) in reverse order to a disjoint destination of equal length.
// Unaligned addresses are supported. No padding is required: vector accesses
// stay entirely inside the ranges, with a scalar tail of at most seven words.
inline void reverse_copy_u32(const std::uint32_t* first,
                             const std::uint32_t* last,
                             std::uint32_t* output) noexcept {
    const auto order = _mm256_setr_epi32(7, 6, 5, 4, 3, 2, 1, 0);
    while (last - first >= 8) {
        last -= 8;
        const auto words = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(last));
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(output),
                           _mm256_permutevar8x32_epi32(words, order));
        output += 8;
    }
    while (last != first)
        *output++ = *--last;
}

inline void reverse_u32(std::uint32_t* first, std::uint32_t* last) noexcept {
    const auto order = _mm256_setr_epi32(7, 6, 5, 4, 3, 2, 1, 0);
    while (last - first >= 16) {
        last -= 8;
        const auto a = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(first));
        const auto b = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(last));
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(first),
                            _mm256_permutevar8x32_epi32(b, order));
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(last),
                            _mm256_permutevar8x32_epi32(a, order));
        first += 8;
    }
    std::reverse(first, last);
}
} // namespace canard::simd
#include <cstdint>
#include <cstring>
namespace canard::kernel {
// Ordered 32-bit leaf movement, independent of arithmetic and lazy actions.
template <unsigned Extent>
struct sequence_leaf_u32_avx2 : sequence_leaf_operations<std::uint32_t, Extent> {
    using base = sequence_leaf_operations<std::uint32_t, Extent>;
    using leaf_type = leaf_type_t<base>;
    // Same ordered movement contract as the scalar leaf operation. Save the
    // first prefix already reversed, vector-copy the other outer fragment,
    // and use memmove only for the potentially overlapping interior shift.
    static void exchange_reversed_outer_parts(leaf_type& left,
                                              unsigned left_size,
                                              unsigned left_cut,
                                              leaf_type& right,
                                              unsigned right_size,
                                              unsigned right_cut) noexcept {
        const unsigned tail = right_size - right_cut;
        alignas(32) std::uint32_t saved[Extent];
        simd::reverse_copy_u32(left.values.data(), left.values.data() + left_cut, saved);
        if (tail != left_cut)
            std::memmove(left.values.data() + tail,
                         left.values.data() + left_cut,
                         (left_size - left_cut) * sizeof(std::uint32_t));
        simd::reverse_copy_u32(right.values.data() + right_cut,
                              right.values.data() + right_size,
                              left.values.data());
        std::memcpy(right.values.data() + right_cut, saved, left_cut * sizeof(std::uint32_t));
    }
    static void exchange_reversed_outer_parts_oriented(
        leaf_type& left, unsigned left_size, unsigned left_cut, bool left_reversed,
        leaf_type& right, unsigned right_size, unsigned right_cut, bool right_reversed) noexcept {
        const unsigned tail = right_size - right_cut;
        const unsigned inside = left_size - left_cut;
        alignas(32) std::uint32_t saved[Extent];
        const auto copy_fragment = [&](const std::uint32_t* src, unsigned count, std::uint32_t* dst) {
            if (left_reversed == right_reversed) simd::reverse_copy_u32(src, src + count, dst);
            else std::memcpy(dst, src, count * sizeof(std::uint32_t));
        };
        copy_fragment(left.values.data() + (left_reversed ? inside : 0), left_cut, saved);
        if (!left_reversed && tail != left_cut)
            std::memmove(left.values.data() + tail, left.values.data() + left_cut,
                         inside * sizeof(std::uint32_t));
        copy_fragment(right.values.data() + (right_reversed ? 0 : right_cut), tail,
                      left.values.data() + (left_reversed ? inside : 0));
        if (right_reversed && left_cut != tail)
            std::memmove(right.values.data() + left_cut, right.values.data() + tail,
                         right_cut * sizeof(std::uint32_t));
        std::memcpy(right.values.data() + (right_reversed ? 0 : right_cut), saved,
                    left_cut * sizeof(std::uint32_t));
    }
    static void reverse_values(leaf_type& leaf, unsigned first, unsigned last) noexcept {
        simd::reverse_u32(leaf.values.data() + first, leaf.values.data() + last);
    }
};
} // namespace canard::kernel
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

namespace canard::kernel {

template <unsigned Extent>
struct sequence_compact_sum_avx2 : sequence_compact_sum_scalar<Extent> {
    using base = sequence_compact_sum_scalar<Extent>;
    using leaf_type = leaf_type_t<base>;
    using summary_type = summary_type_t<base>;
    using movement = sequence_leaf_u32_avx2<Extent>;
    using base::base;

    [[nodiscard]] static summary_type
    summarize(const leaf_type& leaf, unsigned first, unsigned last) noexcept {
        return simd::sum_widened_u32(leaf.values.data() + first, leaf.values.data() + last);
    }
    static void exchange_reversed_outer_parts(leaf_type& left,
                                              unsigned left_size,
                                              unsigned left_cut,
                                              leaf_type& right,
                                              unsigned right_size,
                                              unsigned right_cut) noexcept {
        movement::exchange_reversed_outer_parts(
            left, left_size, left_cut, right, right_size, right_cut);
    }
    static void exchange_reversed_outer_parts_oriented(leaf_type& left,
                                                       unsigned left_size,
                                                       unsigned left_cut,
                                                       bool left_reversed,
                                                       leaf_type& right,
                                                       unsigned right_size,
                                                       unsigned right_cut,
                                                       bool right_reversed) noexcept {
        movement::exchange_reversed_outer_parts_oriented(left,
                                                         left_size,
                                                         left_cut,
                                                         left_reversed,
                                                         right,
                                                         right_size,
                                                         right_cut,
                                                         right_reversed);
    }
    static void reverse_values(leaf_type& leaf, unsigned first, unsigned last) noexcept {
        movement::reverse_values(leaf, first, last);
    }
};

} // namespace canard::kernel

namespace canard::sequence {

template <unsigned Extent>
struct kernel_binding<representation::compact_u32,
                      wide::execution::avx2,
                      algebra::sum<std::uint64_t>,
                      algebra::no_action,
                      Extent> {
    using type = kernel::sequence_compact_sum_avx2<Extent>;
};

} // namespace canard::sequence
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

    void write_u64(std::uint64_t value) {
        if (storage_.get() + capacity - cursor_ < 24)
            flush();
        cursor_ = decimal::format_u64_token(cursor_, value);
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
#include <vector>

#ifndef CANARD_REVERSE_CAPACITY
#define CANARD_REVERSE_CAPACITY 512
#endif
#ifndef CANARD_REVERSE_OCCUPANCY
#define CANARD_REVERSE_OCCUPANCY 8
#endif
#ifndef CANARD_REVERSE_BALANCE
#define CANARD_REVERSE_BALANCE 4
#endif

namespace {
#ifdef CANARD_REVERSE_SCALAR
using execution = canard::wide::execution::scalar;
#else
using execution = canard::wide::execution::avx2;
#endif
using profile = canard::sequence::configuration<CANARD_REVERSE_CAPACITY,
                                               200000,
                                               canard::sequence::representation::compact_u32,
                                               execution,
                                               CANARD_REVERSE_OCCUPANCY,
                                               CANARD_REVERSE_BALANCE>;
} // namespace

int main() {
    canard::io::padded_file file;
    canard::io::trusted_ascii_reader input{file.view()};
    canard::io::buffered_writer output;
    const auto [n, q] = input.read_pair();
    std::vector<std::uint32_t> initial(n);
    for (auto& value : initial)
        value = input.read_u32<10>();

    canard::chunked_sequence sequence{
        initial, canard::algebra::sum<std::uint64_t>{}, profile{}};
    for (unsigned i = 0; i < q; ++i) {
        const auto [type, left, right] = input.read_tagged_pair<6, 6>();
        if (type == 0)
            sequence.reverse(left, right);
        else
            output.write_u64(sequence.fold(left, right));
    }
}
