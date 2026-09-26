// Generated from canard headers and client. Edit the maintained sources.
// ===== clients/range_set_range_composite.cpp =====
// ===== include/canard/range/wide_assignment_tree.hpp =====
// ===== include/canard/assignment/configuration.hpp =====
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
// ===== include/canard/kernel/wide_assignment_scalar.hpp =====
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
#include <array>
#include <bit>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace canard::kernel {

// Reference block policy for repeated-value assignments over an ordered monoid.
// It imposes no commutativity, subtraction, inverse, equality, or primality law.
template <algebra::monoid Monoid, unsigned Fanout> class wide_assignment_scalar {
  public:
    using monoid_type = Monoid;
    using value_type = value_type_t<Monoid>;
    using summary_type = value_type;
    static constexpr bool nothrow_mutation =
        std::is_default_constructible_v<summary_type> &&
        std::is_nothrow_copy_constructible_v<summary_type> &&
        std::is_nothrow_copy_assignable_v<summary_type> &&
        std::is_nothrow_move_constructible_v<summary_type> &&
        std::is_nothrow_move_assignable_v<summary_type> &&
        noexcept(std::declval<const Monoid&>().identity()) &&
        noexcept(std::declval<const Monoid&>().combine(
            std::declval<const summary_type&>(), std::declval<const summary_type&>()));

    struct alignas(64) leaf_type {
        std::array<summary_type, Fanout> values;
    };
    struct alignas(64) branch_type {
        std::array<summary_type, Fanout> values;
        std::array<std::uint32_t, Fanout> tags{};
    };

  private:
    [[no_unique_address]] Monoid monoid_;

  public:
    explicit wide_assignment_scalar(Monoid monoid) : monoid_(std::move(monoid)) {}
    [[nodiscard]] const Monoid& monoid() const noexcept {
        return monoid_;
    }
    [[nodiscard]] leaf_type empty_leaf() const {
        return {detail::repeat<Fanout>(identity())};
    }
    [[nodiscard]] branch_type empty_branch() const {
        return {detail::repeat<Fanout>(identity()), {}};
    }
    [[nodiscard]] summary_type identity() const noexcept(nothrow_mutation) {
        return monoid_.identity();
    }
    [[nodiscard]] summary_type combine(const summary_type& left, const summary_type& right) const
        noexcept(nothrow_mutation) {
        return monoid_.combine(left, right);
    }
    [[nodiscard]] summary_type import_value(const value_type& value) const
        noexcept(nothrow_mutation) {
        return value;
    }
    [[nodiscard]] value_type export_value(const summary_type& value) const
        noexcept(nothrow_mutation) {
        return value;
    }
    template <typename Block>
    [[nodiscard]] const summary_type& slot(const Block& block, unsigned index) const noexcept {
        return block.values[index];
    }
    template <typename Block>
    void write_slot(Block& block, unsigned index, const summary_type& value) const
        noexcept(nothrow_mutation) {
        block.values[index] = value;
    }
    template <typename Block>
    [[nodiscard]] summary_type fold(const Block& block, unsigned first, unsigned last) const
        noexcept(nothrow_mutation) {
        if (first == last)
            return identity();
        auto result = slot(block, first);
        for (unsigned i = first + 1; i < last; ++i)
            result = combine(result, slot(block, i));
        return result;
    }
    template <typename Block>
    void fill(Block& block, unsigned first, unsigned last, const summary_type& value,
              std::uint32_t tag) const noexcept(nothrow_mutation) {
        for (unsigned i = first; i < last; ++i) {
            block.values[i] = value;
            if constexpr (std::same_as<Block, branch_type>)
                block.tags[i] = tag;
        }
    }
    template <typename Block>
    void reset(Block& block, unsigned first, unsigned last, const summary_type& before,
               const summary_type& after, std::uint32_t old_tag, std::uint32_t new_tag) const
        noexcept(nothrow_mutation) {
        for (unsigned i = 0; i < Fanout; ++i) {
            const bool selected = first <= i && i < last;
            block.values[i] = selected ? after : before;
            if constexpr (std::same_as<Block, branch_type>)
                block.tags[i] = selected ? new_tag : old_tag;
        }
    }
    void make_powers(summary_type* output, summary_type value, unsigned count) const
        noexcept(nothrow_mutation) {
        output[0] = value;
        for (unsigned i = 1; i < count; ++i) {
            value = combine(value, value);
            output[i] = value;
        }
    }
    [[nodiscard]] summary_type repeat(const summary_type* powers, unsigned count) const
        noexcept(nothrow_mutation) {
        if (count == 0)
            return identity();
        const unsigned first = std::countr_zero(count);
        auto result = powers[first];
        count >>= first + 1;
        for (unsigned i = first + 1; count; count >>= 1, ++i) {
            if (count & 1u)
                result = combine(result, powers[i]);
        }
        return result;
    }
};

} // namespace canard::kernel
// ===== include/canard/kernel/wide_assignment_affine.hpp =====
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
#include <array>
#include <bit>
#include <concepts>
#include <cstdint>
#include <type_traits>

namespace canard::kernel {

// Shared scalar/AVX2 representation: encoded slopes and ordinary translations,
// both lazy residues in [0, 2p). A summary still denotes x -> a*x+b; encoding
// the slope makes composition and evaluation use the existing mixed REDC law.
// Odd composite moduli and zero slopes are valid. No division/inverse is used.
template <std::uint32_t Modulus, unsigned Fanout> struct wide_assignment_affine {
    using monoid_type = algebra::affine_composition<Modulus>;
    using value_type = algebra::affine_map<Modulus>;
    using field = numeric::montgomery32<Modulus>;
    struct summary_type {
        std::uint32_t multiplier;
        std::uint32_t translation;
    };
    struct alignas(64) leaf_type {
        std::array<std::uint32_t, Fanout> multipliers{};
        std::array<std::uint32_t, Fanout> translations{};
    };
    struct alignas(64) branch_type : leaf_type {
        std::array<std::uint32_t, Fanout> tags{};
    };
    static constexpr bool nothrow_mutation = true;
    [[no_unique_address]] monoid_type monoid_;

    explicit wide_assignment_affine(monoid_type monoid) noexcept : monoid_(monoid) {}
    [[nodiscard]] const monoid_type& monoid() const noexcept {
        return monoid_;
    }
    [[nodiscard]] static leaf_type empty_leaf() noexcept {
        leaf_type result;
        result.multipliers.fill(field::one);
        return result;
    }
    [[nodiscard]] static branch_type empty_branch() noexcept {
        branch_type result;
        result.multipliers.fill(field::one);
        return result;
    }
    [[nodiscard]] static summary_type identity() noexcept {
        return {field::one, 0};
    }
    [[nodiscard]] static summary_type import_value(value_type value) noexcept {
        return {field::encode(value.multiplier), value.translation};
    }
    [[nodiscard]] static value_type export_value(summary_type value) noexcept {
        return {field::decode(value.multiplier), canonical(value.translation)};
    }
    [[nodiscard]] static std::uint32_t canonical(std::uint32_t value) noexcept {
        return value >= Modulus ? value - Modulus : value;
    }
    [[nodiscard]] static summary_type combine(summary_type left, summary_type right) noexcept {
        return {field::multiply(left.multiplier, right.multiplier),
                field::add(field::multiply(left.translation, right.multiplier),
                           right.translation)};
    }
    [[nodiscard]] static std::uint32_t evaluate(summary_type value, std::uint32_t x) noexcept {
        // Preserve affine_map::operator() semantics for every uint32 argument,
        // not merely the judge's canonical arguments. REDC itself needs x<2p.
        if (x >= field::twice_modulus)
            x %= Modulus;
        return canonical(field::add(field::multiply(value.multiplier, x), value.translation));
    }
    template <typename Block>
    [[nodiscard]] static summary_type slot(const Block& block, unsigned index) noexcept {
        return {block.multipliers[index], block.translations[index]};
    }
    template <typename Block>
    static void write_slot(Block& block, unsigned index, summary_type value) noexcept {
        block.multipliers[index] = value.multiplier;
        block.translations[index] = value.translation;
    }
    template <typename Block>
    [[nodiscard]] static summary_type fold(const Block& block, unsigned first,
                                           unsigned last) noexcept {
        if (first == last)
            return identity();
        auto result = slot(block, first);
        for (unsigned i = first + 1; i < last; ++i)
            result = combine(result, slot(block, i));
        return result;
    }
    template <typename Block>
    static void fill(Block& block, unsigned first, unsigned last, summary_type value,
                     std::uint32_t tag) noexcept {
        for (unsigned i = first; i < last; ++i) {
            write_slot(block, i, value);
            if constexpr (std::same_as<Block, branch_type>)
                block.tags[i] = tag;
        }
    }
    template <typename Block>
    static void reset(Block& block, unsigned first, unsigned last, summary_type before,
                      summary_type after, std::uint32_t old_tag, std::uint32_t new_tag) noexcept {
        for (unsigned i = 0; i < Fanout; ++i) {
            const bool selected = first <= i && i < last;
            write_slot(block, i, selected ? after : before);
            if constexpr (std::same_as<Block, branch_type>)
                block.tags[i] = selected ? new_tag : old_tag;
        }
    }
    static void make_powers(summary_type* output, summary_type value, unsigned count) noexcept {
        output[0] = value;
        for (unsigned i = 1; i < count; ++i) {
            value = combine(value, value);
            output[i] = value;
        }
    }
    [[nodiscard]] static summary_type repeat(const summary_type* powers, unsigned count) noexcept {
        if (count == 0)
            return identity();
        const unsigned first = std::countr_zero(count);
        auto result = powers[first];
        count >>= first + 1;
        for (unsigned i = first + 1; count; count >>= 1, ++i) {
            if (count & 1u)
                result = combine(result, powers[i]);
        }
        return result;
    }
};

} // namespace canard::kernel
#include <concepts>
#include <cstdint>

namespace canard::assignment {

// A block-policy customization point, never a whole-tree implementation.
// Configuration reuses wide's independent representation/execution axes.
template <typename Representation, typename Execution, typename Monoid, unsigned Fanout>
struct kernel_binding {};

template <algebra::monoid Monoid, unsigned Fanout>
struct kernel_binding<wide::representation::ordinary, wide::execution::scalar, Monoid, Fanout> {
    using type = kernel::wide_assignment_scalar<Monoid, Fanout>;
};
template <std::uint32_t Modulus, unsigned Fanout>
    requires(Modulus > 1 && Modulus < (std::uint32_t{1} << 30) && Modulus % 2 == 1)
struct kernel_binding<wide::representation::ordinary, wide::execution::scalar,
                      algebra::affine_composition<Modulus>, Fanout> {
    using type = kernel::wide_assignment_affine<Modulus, Fanout>;
};
template <typename Configuration, typename Monoid>
concept supported_configuration = requires {
    typename kernel_binding<representation_type_t<Configuration>, execution_type_t<Configuration>,
                            Monoid, Configuration::fanout>::type;
};
template <typename Configuration, typename Monoid>
    requires supported_configuration<Configuration, Monoid>
using kernel_for_t = typename kernel_binding<representation_type_t<Configuration>,
                                            execution_type_t<Configuration>, Monoid,
                                            Configuration::fanout>::type;

} // namespace canard::assignment
// ===== include/canard/range/detail/wide_assignment_engine.hpp =====
// ===== include/canard/memory/traced_indexed_pool.hpp =====
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
#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace canard::memory {

// Bounded, nonmoving HANDLE space with occasional tracing of external roots.
// T has no outgoing pool references. Index zero is a private scratch object,
// never a root. Released objects retain their values until overwritten.
//
// At most root_bound distinct records may be reachable between acquisitions.
// acquire() may trace all roots, so callers must enumerate EVERY stored handle,
// including logically hidden/stale handles. No handles escape that root set.
// With 2*root_bound+1 slots, tracing adds amortized constant metadata work per
// acquisition, but an individual acquisition can take O(root_bound) time.
// This is a single-threaded facility; tracing and acquisition are not reentrant.
template <typename T>
    requires(std::default_initializable<T> &&
             std::is_nothrow_move_constructible_v<T> && std::is_nothrow_destructible_v<T>)
class traced_indexed_pool {
    indexed_pool<T> slots_;
    // 0: occupied, unmarked; 1: marked; 2: free or not yet constructed.
    std::vector<std::uint8_t> state_;
    std::uint32_t high_water_ = 0;
    std::uint32_t limit_ = 0;
    std::size_t collections_ = 0;

    template <typename Enumerate> void collect(Enumerate&& enumerate) noexcept {
        for (std::uint32_t id = 1; id <= high_water_; ++id) {
            if (state_[id] != 2)
                state_[id] = 0;
        }
        enumerate([&](std::uint32_t id) noexcept {
            if (id == 0)
                return;
            assert(id <= high_water_ && state_[id] != 2);
            state_[id] = 1;
        });
        for (std::uint32_t id = 1; id <= high_water_; ++id) {
            if (state_[id] == 0) {
                slots_.release(id);
                state_[id] = 2;
            }
        }
        ++collections_;
        assert(slots_.live_slots() < limit_);
    }

  public:
    explicit traced_indexed_pool(std::size_t root_bound) {
        constexpr auto largest = std::numeric_limits<std::uint32_t>::max() - 1;
        if (root_bound > (std::size_t{largest} - 1) / 2)
            throw std::length_error("canard: traced pool handle bound exceeded");
        limit_ = static_cast<std::uint32_t>(2 * root_bound + 1);
        slots_.reserve(std::size_t{limit_} + 1);
        state_.assign(std::size_t{limit_} + 1, 2);
        if constexpr (!std::is_nothrow_default_constructible_v<T>) {
            // A generic product may have a potentially throwing default
            // constructor even when all payload copies/operations are noexcept.
            // Construct those retained slots here, never inside a mutation.
            for (std::uint32_t i = 0; i < limit_; ++i)
                (void)slots_.acquire_retained();
            for (std::uint32_t i = 1; i <= limit_; ++i)
                slots_.release(i);
            high_water_ = limit_;
        }
    }
    traced_indexed_pool(const traced_indexed_pool& other)
        : slots_(other.slots_), state_(other.state_), high_water_(other.high_water_),
          limit_(other.limit_), collections_(other.collections_) {
        // indexed_pool copies values, not the original unused vector capacity.
        // Restore it so later acquisitions cannot allocate after a copy.
        slots_.reserve(std::size_t{limit_} + 1);
    }
    traced_indexed_pool(traced_indexed_pool&&) noexcept = default;
    traced_indexed_pool& operator=(const traced_indexed_pool& other) {
        if (this != &other) {
            traced_indexed_pool next{other};
            swap(next);
        }
        return *this;
    }
    traced_indexed_pool& operator=(traced_indexed_pool&&) noexcept = default;
    void swap(traced_indexed_pool& other) noexcept {
        using std::swap;
        slots_.swap(other.slots_);
        state_.swap(other.state_);
        swap(high_water_, other.high_water_);
        swap(limit_, other.limit_);
        swap(collections_, other.collections_);
    }

    // Enumerate(mark) must not throw or acquire/mutate this pool. It need not
    // enumerate duplicate handles only once; duplicate marks are inexpensive.
    template <typename Enumerate>
    [[nodiscard]] std::uint32_t acquire_retained(Enumerate&& enumerate) noexcept {
        if (slots_.live_slots() == limit_)
            collect(std::forward<Enumerate>(enumerate));
        const auto id = slots_.acquire_retained();
        high_water_ = std::max(high_water_, id);
        state_[id] = 0;
        return id;
    }
    [[nodiscard]] T& operator[](std::uint32_t id) noexcept {
        return slots_[id];
    }
    [[nodiscard]] const T& operator[](std::uint32_t id) const noexcept {
        return slots_[id];
    }
    [[nodiscard]] std::size_t occupied_slots() const noexcept {
        // Includes unreachable records waiting for the next collection.
        return slots_.live_slots();
    }
    [[nodiscard]] std::size_t collection_count() const noexcept {
        return collections_;
    }
    [[nodiscard]] std::size_t allocated_bytes() const noexcept {
        return slots_.allocated_bytes() + state_.capacity() * sizeof(std::uint8_t);
    }
};

} // namespace canard::memory
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
#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <ranges>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace canard::detail {

// Assignment-only scheduler over the shared wide geometry and storage. Blocks
// own child summaries and handles to immutable repeated-value power ladders.
// This file knows no affine coefficients, arithmetic domain, inverses or SIMD.
//
// Tags belong to parent slots. A nonzero tag overrides the child's old block.
// Partial mutation fuses the carried assignment and the new assignment into a
// single reset of that block, then repairs only its two boundary children.
// Read-only queries stop at a tagged boundary instead of pushing its tag.
template <typename Kernel, typename Configuration> class wide_assignment_engine {
  public:
    using kernel_type = Kernel;
    using value_type = value_type_t<Kernel>;
    using summary_type = summary_type_t<Kernel>;
    using monoid_type = monoid_type_t<Kernel>;
    using layout_type = structural::wide_layout<Configuration>;
    using storage_type = memory::wide_storage<Kernel, Configuration>;
    using size_type = std::size_t;
    static constexpr bool nothrow_mutation = Kernel::nothrow_mutation;
    static constexpr unsigned fanout = Configuration::fanout;

  private:
    static constexpr unsigned bits = std::countr_zero(fanout);
    static constexpr unsigned power_count = std::bit_width(Configuration::max_size);
    static_assert(Configuration::max_size <= (std::size_t{1} << 30));
    static_assert(nothrow_mutation, "Assignment payload operations must be nonthrowing.");
    struct power_record {
        std::array<summary_type, power_count> powers;
    };

    // Kernel must precede storage: a stateful monoid participates in construction.
    [[no_unique_address]] Kernel kernel_;
    storage_type storage_;
    memory::traced_indexed_pool<power_record> records_;

    template <std::ranges::input_range R> static decltype(auto) checked_input(R&& values) {
        if constexpr (std::ranges::sized_range<R>) {
            if (std::ranges::size(values) > Configuration::max_size)
                throw std::length_error("canard: assignment tree maximum size exceeded");
            return std::forward<R>(values);
        } else if constexpr (std::ranges::forward_range<R>) {
            if (std::ranges::distance(values) >
                static_cast<std::ptrdiff_t>(Configuration::max_size))
                throw std::length_error("canard: assignment tree maximum size exceeded");
            return std::forward<R>(values);
        } else {
            // One pass through an input-only source; owned staging is explicit.
            std::vector<value_type> staged;
            for (auto&& value : values) {
                if (staged.size() == Configuration::max_size)
                    throw std::length_error("canard: assignment tree maximum size exceeded");
                staged.emplace_back(value);
            }
            return staged;
        }
    }
    template <unsigned Level = 0, typename Function>
    decltype(auto) with_root(Function&& function) const noexcept {
        if (storage_.layout().height() == Level + 1)
            return function.template operator()<Level>();
        if constexpr (Level + 1 < layout_type::max_height)
            return with_root<Level + 1>(std::forward<Function>(function));
        else
            std::unreachable();
    }
    template <typename Mark> void trace_records(Mark&& mark) const noexcept {
        // Visit physical tags, even in blocks hidden by an ancestor assignment.
        // Those stale handles must remain valid if the old block is inspected
        // while it is being replaced. No graph traversal or payload scan occurs.
        const auto& geometry = storage_.layout();
        for (unsigned level = 1; level < geometry.height(); ++level) {
            for (size_type block = 0; block < geometry.blocks(level); ++block) {
                for (auto tag : storage_.branch(level, block).tags)
                    mark(tag);
            }
        }
    }
    [[nodiscard]] std::uint32_t prepare(unsigned first, unsigned last,
                                         const value_type& value) noexcept {
        // Without a complete leaf block, no new persistent tag can be stored.
        // Record zero is scratch; one-element/fragment assignments avoid both
        // allocating a handle and computing powers they will never use.
        const bool persistent = (first + fanout - 1) / fanout < last / fanout && height() > 1;
        const auto id = persistent ? records_.acquire_retained([&](auto mark) noexcept {
            trace_records(mark);
        }) : 0u;
        unsigned needed = 1;
        if (persistent) {
            // Tags can only represent geometrically complete child spans.
            // Start with the largest B^level no longer than the assignment,
            // capped by the largest root child. If it contains no aligned
            // complete child, dropping one radix level is enough: B >= 2.
            // A propagated tag only moves to smaller children, so no later
            // repeat can read powers above this exponent.
            const unsigned level = std::min((std::bit_width(last - first) - 1) / bits, height() - 1);
            unsigned exponent = level * bits;
            const unsigned span = 1u << exponent;
            if (((first + span - 1) >> exponent) >= (last >> exponent))
                exponent -= bits;
            needed = exponent + 1;
        }
        kernel_.make_powers(records_[id].powers.data(), kernel_.import_value(value), needed);
        return id;
    }
    [[nodiscard]] summary_type repeat(std::uint32_t tag, unsigned count) const noexcept {
        return kernel_.repeat(records_[tag].powers.data(), count);
    }
    template <unsigned Level, bool ReturnSummary = true>
    summary_type assign_impl(unsigned index, unsigned first, unsigned last, std::uint32_t tag,
                              std::uint32_t incoming = 0) noexcept {
        auto& block = storage_.template node<Level>(index);
        constexpr unsigned span = 1u << (Level * bits);
        if constexpr (Level == 0) {
            const auto& value = records_[tag].powers[0];
            if (incoming)
                kernel_.reset(block, first, last, records_[incoming].powers[0], value, 0, 0);
            else
                kernel_.fill(block, first, last, value, 0);
        } else {
            const unsigned full_first = (first + span - 1) / span;
            const unsigned full_last = last / span;
            if (incoming) {
                // An empty covered-slot interval must not read a nonexistent
                // high power from the scratch/small-assignment record.
                const auto after = full_first < full_last ? records_[tag].powers[Level * bits]
                                                         : kernel_.identity();
                kernel_.reset(block, full_first, std::max(full_first, full_last),
                              records_[incoming].powers[Level * bits], after, incoming, tag);
            }
            const unsigned left = first / span;
            const unsigned right = (last - 1) / span;
            const auto descend = [&](unsigned child, unsigned begin, unsigned end) noexcept {
                const auto changed = assign_impl<Level - 1>(
                    index * fanout + child, begin, end, tag, block.tags[child]);
                block.tags[child] = 0;
                kernel_.write_slot(block, child, changed);
            };
            if (left == right && (first % span || last % span)) {
                descend(left, first % span, last - left * span);
            } else {
                if (first % span)
                    descend(left, first % span, span);
                if (last % span)
                    descend(right, 0, last % span);
                if (!incoming && full_first < full_last)
                    kernel_.fill(block, full_first, full_last,
                                 records_[tag].powers[Level * bits], tag);
            }
        }
        // No parent consumes a root subtotal; do not reduce that block on updates.
        if constexpr (ReturnSummary)
            return kernel_.fold(block, 0, fanout);
        else
            return kernel_.identity();
    }
    template <unsigned Level>
    [[nodiscard]] summary_type query(unsigned index, unsigned first, unsigned last) const noexcept {
        const auto& block = storage_.template node<Level>(index);
        if constexpr (Level == 0) {
            return kernel_.fold(block, first, last);
        } else {
            constexpr unsigned span = 1u << (Level * bits);
            const auto boundary = [&](unsigned child, unsigned begin, unsigned end) noexcept {
                if (begin == 0 && end == span)
                    return kernel_.slot(block, child);
                if (block.tags[child])
                    return repeat(block.tags[child], end - begin);
                return query<Level - 1>(index * fanout + child, begin, end);
            };
            const unsigned left = first / span;
            const unsigned right = (last - 1) / span;
            if (left == right)
                return boundary(left, first % span, last - left * span);

            const unsigned full_first = (first + span - 1) / span;
            const unsigned full_last = last / span;
            bool has_result = full_first < full_last;
            auto result = has_result ? kernel_.fold(block, full_first, full_last)
                                     : kernel_.identity();
            if (first % span) {
                const auto part = boundary(left, first % span, span);
                result = has_result ? kernel_.combine(part, result) : part;
                has_result = true;
            }
            if (last % span) {
                const auto part = boundary(right, 0, last % span);
                result = has_result ? kernel_.combine(result, part) : part;
            }
            return result;
        }
    }
    [[nodiscard]] summary_type fold_internal(size_type first, size_type last) const noexcept {
        assert(first <= last && last <= size());
        if (first == last)
            return kernel_.identity();
        return with_root([&]<unsigned Level> noexcept {
            return query<Level>(0, static_cast<unsigned>(first), static_cast<unsigned>(last));
        });
    }

  public:
    template <std::ranges::input_range R>
        requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    explicit wide_assignment_engine(R&& values, Kernel kernel)
        : kernel_(std::move(kernel)),
          storage_(checked_input(std::forward<R>(values)), kernel_),
          records_(storage_.layout().branch_blocks() * fanout) {}

    wide_assignment_engine(const wide_assignment_engine&) = default;
    wide_assignment_engine(wide_assignment_engine&& other)
        noexcept(std::is_nothrow_copy_constructible_v<Kernel>)
        : kernel_(other.kernel_),
          storage_(std::move(other.storage_)),
          records_(std::move(other.records_)) {}
    wide_assignment_engine& operator=(const wide_assignment_engine& other)
        requires std::is_nothrow_swappable_v<Kernel>
    {
        if (this != &other) {
            wide_assignment_engine next{other};
            swap(next);
        }
        return *this;
    }
    wide_assignment_engine& operator=(wide_assignment_engine&& other)
        noexcept(std::is_nothrow_copy_constructible_v<Kernel>)
        requires std::is_nothrow_swappable_v<Kernel>
    {
        if (this != &other) {
            wide_assignment_engine next{std::move(other)};
            swap(next);
        }
        return *this;
    }
    void swap(wide_assignment_engine& other) noexcept
        requires std::is_nothrow_swappable_v<Kernel>
    {
        using std::swap;
        swap(kernel_, other.kernel_);
        storage_.swap(other.storage_);
        records_.swap(other.records_);
    }
    [[nodiscard]] size_type size() const noexcept {
        return storage_.layout().size();
    }
    [[nodiscard]] bool empty() const noexcept {
        return size() == 0;
    }
    [[nodiscard]] unsigned height() const noexcept {
        return storage_.layout().height();
    }
    [[nodiscard]] const monoid_type& monoid() const noexcept {
        return kernel_.monoid();
    }
    [[nodiscard]] size_type storage_bytes() const noexcept {
        return storage_.storage_bytes();
    }
    [[nodiscard]] size_type power_storage_bytes() const noexcept {
        return records_.allocated_bytes();
    }
    [[nodiscard]] size_type occupied_power_records() const noexcept {
        return records_.occupied_slots();
    }
    [[nodiscard]] size_type collection_count() const noexcept {
        return records_.collection_count();
    }
    [[nodiscard]] value_type fold(size_type first, size_type last) const noexcept {
        return kernel_.export_value(fold_internal(first, last));
    }
    [[nodiscard]] value_type all_fold() const noexcept {
        return fold(0, size());
    }
    [[nodiscard]] value_type get(size_type position) const noexcept {
        assert(position < size());
        return fold(position, position + 1);
    }
    // Optional fused evaluation avoids exporting/reimporting an aggregate.
    // Generic kernels instead invoke the public monoid value; no new algebraic
    // assumptions are added to fold(), and every answer is available immediately.
    template <typename... Args>
        requires std::invocable<value_type, Args...>
    decltype(auto) evaluate(size_type first, size_type last, Args&&... args) const {
        const auto result = fold_internal(first, last);
        if constexpr (requires { kernel_.evaluate(result, std::forward<Args>(args)...); })
            return kernel_.evaluate(result, std::forward<Args>(args)...);
        else
            return std::invoke(kernel_.export_value(result), std::forward<Args>(args)...);
    }
    void assign(size_type first, size_type last, const value_type& value) noexcept {
        assert(first <= last && last <= size());
        if (first == last)
            return;
        const auto l = static_cast<unsigned>(first);
        const auto r = static_cast<unsigned>(last);
        const auto tag = prepare(l, r, value);
        with_root([&]<unsigned Level> noexcept { assign_impl<Level, false>(0, l, r, tag); });
    }
    void set(size_type position, const value_type& value) noexcept {
        assert(position < size());
        assign(position, position + 1, value);
    }
    // Adapter for the existing interval_reference and point_reference API.
    // For this owner an action assigns EACH element; it does not compose onto it.
    void apply(size_type first, size_type last, const value_type& value) noexcept {
        assign(first, last, value);
    }
    template <typename Output> Output materialize(Output output) const {
        for (size_type i = 0; i < size(); ++i)
            *output++ = get(i);
        return output;
    }
    [[nodiscard]] std::vector<value_type> snapshot() const {
        std::vector<value_type> result;
        result.reserve(size());
        materialize(std::back_inserter(result));
        return result;
    }
    template <std::ranges::input_range R>
    void rebuild(R&& values) requires std::is_nothrow_swappable_v<Kernel> {
        wide_assignment_engine next{std::forward<R>(values), kernel_};
        swap(next);
    }
    void prefetch(size_type first, size_type last) const noexcept {
        assert(first <= last && last <= size());
        if (first == last)
            return;
#if defined(__GNUC__) || defined(__clang__)
        __builtin_prefetch(&storage_.leaf(first / fanout), 0, 3);
        __builtin_prefetch(&storage_.leaf((last - 1) / fanout), 0, 3);
        if (height() > 1) {
            __builtin_prefetch(&storage_.branch(1, first / fanout / fanout), 0, 3);
            __builtin_prefetch(&storage_.branch(1, (last - 1) / fanout / fanout), 0, 3);
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
#include <cstddef>
#include <format>
#include <ranges>
#include <span>
#include <type_traits>
#include <utility>

namespace canard {

struct assignment_tree_description {
    std::size_t size;
    unsigned fanout;
    unsigned height;
    std::size_t block_bytes;
    std::size_t power_bytes;
    std::size_t collections;
};

template <algebra::monoid Monoid, typename Configuration = wide::configuration<>>
    requires(assignment::supported_configuration<Configuration, Monoid> &&
             Configuration::max_size <= (std::size_t{1} << 30) &&
             assignment::kernel_for_t<Configuration, Monoid>::nothrow_mutation)
class wide_assignment_tree
    : public detail::wide_assignment_engine<assignment::kernel_for_t<Configuration, Monoid>,
                                            Configuration> {
    using kernel_type = assignment::kernel_for_t<Configuration, Monoid>;
    using base = detail::wide_assignment_engine<kernel_type, Configuration>;

  public:
    using value_type = value_type_t<Monoid>;
    using tag_type = value_type;
    using monoid_type = Monoid;
    using configuration_type = Configuration;
    using size_type = std::size_t;

    template <std::ranges::input_range R>
        requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
    explicit wide_assignment_tree(R&& values, Monoid monoid = {}, Configuration = {})
        : base(std::forward<R>(values), kernel_type{std::move(monoid)}) {}
    explicit wide_assignment_tree(Monoid monoid = {}, Configuration configuration = {})
        : wide_assignment_tree(std::span<const value_type>{}, std::move(monoid), configuration) {}
    template <std::input_iterator Iterator, std::sentinel_for<Iterator> Sentinel>
    explicit wide_assignment_tree(Iterator first, Sentinel last, Monoid monoid = {},
                                  Configuration configuration = {})
        : wide_assignment_tree(std::ranges::subrange{first, last},
                               std::move(monoid),
                               configuration) {}

    wide_assignment_tree(const wide_assignment_tree&) = default;
    wide_assignment_tree(wide_assignment_tree&&) = default;
    wide_assignment_tree& operator=(const wide_assignment_tree&) = default;
    wide_assignment_tree& operator=(wide_assignment_tree&&) = default;
    friend void swap(wide_assignment_tree& left, wide_assignment_tree& right) noexcept
        requires requires(base& value) { value.swap(value); }
    {
        left.base::swap(right);
    }
    [[nodiscard]] point_reference<wide_assignment_tree> operator[](size_type position) & noexcept {
        return {*this, position};
    }
    [[nodiscard]] value_type operator[](size_type position) const & noexcept {
        return this->get(position);
    }
    void operator[](size_type) && = delete;
    void operator[](size_type) const && = delete;
    [[nodiscard]] interval_reference<wide_assignment_tree> operator[](size_type first,
                                                                    size_type last) & noexcept {
        return {*this, first, last};
    }
    void operator[](size_type, size_type) && = delete;
    void operator[](size_type, size_type) const && = delete;
    [[nodiscard]] assignment_tree_description description() const noexcept {
        return {this->size(), Configuration::fanout, this->height(), this->storage_bytes(),
                this->power_storage_bytes(), this->collection_count()};
    }
};

template <std::ranges::input_range R, algebra::monoid M>
wide_assignment_tree(R&&, M) -> wide_assignment_tree<M>;
template <std::ranges::input_range R, algebra::monoid M, typename C>
wide_assignment_tree(R&&, M, C) -> wide_assignment_tree<M, C>;
template <std::input_iterator I, std::sentinel_for<I> S, algebra::monoid M>
wide_assignment_tree(I, S, M) -> wide_assignment_tree<M>;
template <std::input_iterator I, std::sentinel_for<I> S, algebra::monoid M, typename C>
wide_assignment_tree(I, S, M, C) -> wide_assignment_tree<M, C>;

} // namespace canard

template <> struct std::formatter<canard::assignment_tree_description, char> {
    constexpr auto parse(std::format_parse_context& context) {
        return context.begin();
    }
    template <typename Context>
    auto format(canard::assignment_tree_description value, Context& context) const {
        return std::format_to(context.out(),
                              "wide_assignment_tree(size={}, fanout={}, height={}, "
                              "blocks={} bytes, "
                              "power storage={} bytes, collections={})",
                              value.size, value.fanout, value.height, value.block_bytes,
                              value.power_bytes, value.collections);
    }
};
// ===== include/canard/assignment/avx2.hpp =====
// ===== include/canard/kernel/wide_assignment_affine_avx2.hpp =====
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
namespace canard::numeric {
template <std::uint32_t Modulus> struct montgomery32_avx2 {
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
        const auto odd = _mm256_mul_epu32(_mm256_srli_epi64(a, 32), _mm256_srli_epi64(b, 32));
        const auto inverse = _mm256_set1_epi32(std::bit_cast<int>(negative_inverse));
        const auto prime = _mm256_set1_epi32(static_cast<int>(modulus));
        const auto even_correction = _mm256_mul_epu32(even, inverse);
        const auto odd_correction = _mm256_mul_epu32(odd, inverse);
        const auto even_sum = _mm256_add_epi64(even, _mm256_mul_epu32(even_correction, prime));
        const auto odd_sum = _mm256_add_epi64(odd, _mm256_mul_epu32(odd_correction, prime));
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
            return simd::multiply_low(values, factor_) -
                   simd::multiply_low(q, simd::u32x8{modulus});
        }
    };
    // Each lane chooses one of two already-prepared factors. Multiplication
    // retains its input representation and returns a lazy residue below 2p.
    // No divisions or reciprocal preparations occur inside the vector loop.
    class blended_multiplier {
        simd::u32x8 factor_, quotient_;

      public:
        blended_multiplier(simd::mask32x8 selected,
                           const fixed_multiplier& yes,
                           const fixed_multiplier& no) noexcept
            : factor_(simd::select(selected, yes.factor_, no.factor_)),
              quotient_(simd::select(selected, yes.quotient_.vector(), no.quotient_.vector())) {}
        [[nodiscard]] simd::u32x8 operator()(simd::u32x8 values) const noexcept {
            auto q = simd::multiply_high(values, quotient_);
            return simd::multiply_low(values, factor_) -
                   simd::multiply_low(q, simd::u32x8{modulus});
        }
    };
    [[nodiscard]] static simd::u32x8 add(simd::u32x8 a, simd::u32x8 b) noexcept {
        const auto sum = a + b;
        return simd::min(sum, sum - simd::u32x8{scalar::twice_modulus});
    }
};
} // namespace canard::numeric
// ===== include/canard/numeric/montgomery32_pair_avx2.hpp =====
#include <bit>
#include <cstdint>
#include <immintrin.h>
#ifndef __AVX2__
#error "This pair arithmetic policy requires the AVX2 execution profile."
#endif

namespace canard::numeric {

// Two independent Montgomery words, each in the low half of a 64-bit lane.
// The 128-bit implementation is intentional: power recurrences have only two
// independent products. Its low/high lanes may have different encoding degrees
// provided both are scaled by the same encoded multiplier. Each word is <2p.
template <std::uint32_t Modulus> struct montgomery32_pair_avx2 {
    using field = montgomery32<Modulus>;
    using pack = __m128i;

    [[nodiscard]] static pack words(std::uint32_t first, std::uint32_t second) noexcept {
        return _mm_set_epi32(0, std::bit_cast<int>(second), 0, std::bit_cast<int>(first));
    }
    // Move a compact pair of uint32 words to/from the widened-lane form.
    // No pointer aliasing or alignment assumption is imposed on callers.
    [[nodiscard]] static pack unpack_words(std::uint64_t compact) noexcept {
        return _mm_unpacklo_epi32(_mm_cvtsi64_si128(std::bit_cast<long long>(compact)),
                                 _mm_setzero_si128());
    }
    [[nodiscard]] static std::uint64_t pack_words(pack value) noexcept {
        return static_cast<std::uint64_t>(_mm_cvtsi128_si64(
            _mm_shuffle_epi32(value, _MM_SHUFFLE(2, 0, 2, 0))));
    }

    // Eight words in consecutive 32-bit lanes, grouped into four adjacent
    // pairs. Precondition: factors[2*i] == factors[2*i+1], for i in [0,4).
    // Each input/factor is below 2p. Each output equals scalar multiply with
    // the same inputs. Both streams can reuse the unshifted factor register.
    // This is an instruction-scheduling contract, not a new residue encoding.
    [[nodiscard]] static __m256i multiply_adjacent_pairs(__m256i left,
                                                       __m256i right) noexcept {
        const auto even = _mm256_mul_epu32(left, right);
        const auto odd = _mm256_mul_epu32(_mm256_srli_epi64(left, 32), right);
        const auto inverse = _mm256_set1_epi32(std::bit_cast<int>(field::negative_inverse));
        const auto prime = _mm256_set1_epi32(Modulus);
        const auto ec = _mm256_mul_epu32(even, inverse);
        const auto oc = _mm256_mul_epu32(odd, inverse);
        const auto es = _mm256_add_epi64(even, _mm256_mul_epu32(ec, prime));
        const auto os = _mm256_add_epi64(odd, _mm256_mul_epu32(oc, prime));
        return _mm256_blend_epi32(_mm256_srli_epi64(es, 32), os, 0xaa);
    }

    [[nodiscard]] static std::uint32_t first(pack value) noexcept {
        return static_cast<std::uint32_t>(_mm_cvtsi128_si32(value));
    }
    [[nodiscard]] static std::uint32_t second(pack value) noexcept {
        return static_cast<std::uint32_t>(_mm_extract_epi32(value, 2));
    }
    [[nodiscard]] static pack broadcast_first(pack value) noexcept {
        return _mm_shuffle_epi32(value, _MM_SHUFFLE(0, 0, 0, 0));
    }
    [[nodiscard]] static pack second_only(pack value) noexcept {
        return _mm_unpackhi_epi64(_mm_setzero_si128(), value);
    }
    [[nodiscard]] static pack multiply(pack value, pack factors) noexcept {
        const auto product = _mm_mul_epu32(value, factors);
        const auto correction = _mm_mul_epu32(
            product, _mm_set1_epi32(std::bit_cast<int>(field::negative_inverse)));
        return _mm_srli_epi64(
            _mm_add_epi64(product,
                          _mm_mul_epu32(correction, _mm_set1_epi32(static_cast<int>(Modulus)))),
            32);
    }
    [[nodiscard]] static pack scale(pack value, std::uint32_t factor) noexcept {
        return multiply(value, _mm_set1_epi32(std::bit_cast<int>(factor)));
    }
    [[nodiscard]] static pack add(pack left, pack right) noexcept {
        const auto sum = _mm_add_epi32(left, right);
        return _mm_min_epu32(
            sum, _mm_sub_epi32(sum, _mm_set1_epi32(static_cast<int>(field::twice_modulus))));
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
#include <bit>
#include <cstdint>
#include <immintrin.h>

namespace canard::kernel {

// Ordered adjacent-pair reduction, not a commutative horizontal reduction.
// The scalar policy uses exactly the same representation and node arrays.
template <std::uint32_t Modulus, unsigned Fanout>
    requires(Fanout == 8 || Fanout == 16)
struct wide_assignment_affine_avx2 : wide_assignment_affine<Modulus, Fanout> {
    using base = wide_assignment_affine<Modulus, Fanout>;
    using summary_type = summary_type_t<base>;
    using branch_type = branch_type_t<base>;
    using field = field_t<base>;
    using vectors = numeric::montgomery32_avx2<Modulus>;
    using pairs = numeric::montgomery32_pair_avx2<Modulus>;
    using access = simd::detail::native_access;
    using base::base;

    [[nodiscard]] static __m256i multiply(__m256i left, __m256i right) noexcept {
        return access::get(vectors::multiply(access::words(left), access::words(right)));
    }
    [[nodiscard]] static __m256i multiply_pairs(__m256i left, __m256i right) noexcept {
        return pairs::multiply_adjacent_pairs(left, right);
    }
    [[nodiscard]] static __m256i add(__m256i left, __m256i right) noexcept {
        return access::get(vectors::add(access::words(left), access::words(right)));
    }
    [[nodiscard]] static __m256i adjacent_pairs(__m256i values) noexcept {
        const auto left = _mm256_shuffle_epi32(values, _MM_SHUFFLE(1, 0, 1, 0));
        const auto factor = _mm256_shuffle_epi32(values, _MM_SHUFFLE(2, 2, 2, 2));
        const auto shift = _mm256_blend_epi32(
            _mm256_setzero_si256(),
            _mm256_shuffle_epi32(values, _MM_SHUFFLE(3, 3, 3, 3)), 0xaa);
        return add(multiply_pairs(left, factor), shift);
    }
    [[nodiscard]] static __m256i compose_pairs(__m256i left, __m256i right) noexcept {
        const auto factor = _mm256_shuffle_epi32(right, _MM_SHUFFLE(2, 2, 0, 0));
        const auto shift = _mm256_blend_epi32(_mm256_setzero_si256(), right, 0xaa);
        return add(multiply_pairs(left, factor), shift);
    }
    [[nodiscard]] static __m256i first_stage(const std::uint32_t* multipliers,
                                           const std::uint32_t* translations,
                                           int first, int last) noexcept {
        const auto selected = access::get(simd::lanes_between(first, last));
        const auto a = _mm256_blendv_epi8(
            _mm256_set1_epi32(field::one),
            _mm256_load_si256(reinterpret_cast<const __m256i*>(multipliers)), selected);
        const auto b = _mm256_and_si256(
            _mm256_load_si256(reinterpret_cast<const __m256i*>(translations)), selected);
        const auto left = _mm256_blend_epi32(a, _mm256_slli_epi64(b, 32), 0xaa);
        const auto factor = _mm256_shuffle_epi32(a, _MM_SHUFFLE(3, 3, 1, 1));
        const auto shift = _mm256_blend_epi32(_mm256_setzero_si256(), b, 0xaa);
        return add(multiply_pairs(left, factor), shift);
    }
    [[nodiscard]] static summary_type finish_groups(__m256i groups) noexcept {
        const auto low = _mm256_castsi256_si128(groups);
        const auto high = _mm256_extracti128_si256(groups, 1);
        return base::combine(
            {static_cast<std::uint32_t>(_mm_cvtsi128_si32(low)),
             static_cast<std::uint32_t>(_mm_extract_epi32(low, 1))},
            {static_cast<std::uint32_t>(_mm_cvtsi128_si32(high)),
             static_cast<std::uint32_t>(_mm_extract_epi32(high, 1))});
    }
    [[nodiscard]] static summary_type fold_eight(const std::uint32_t* multipliers,
                                              const std::uint32_t* translations,
                                               unsigned first, unsigned last) noexcept {
        return finish_groups(adjacent_pairs(first_stage(multipliers, translations, first, last)));
    }
    [[nodiscard]] static summary_type fold_sixteen(const std::uint32_t* multipliers,
                                                const std::uint32_t* translations,
                                                 unsigned first, unsigned last) noexcept {
        const auto low = first_stage(multipliers, translations, first, last);
        const auto high = first_stage(multipliers + 8, translations + 8,
                                     int(first) - 8, int(last) - 8);
        // Each 64-bit lane holds one ordered (multiplier, translation) pair.
        // After interleaving halves, combine [01,89,45,cd] with [23,ab,67,ef].
        // Here hexadecimal digits label the sixteen original positions.
        // This computes four groups of four in one vector, not two duplicated
        // vectors. Restore logical order before the next adjacent reduction.
        const auto groups = compose_pairs(_mm256_unpacklo_epi64(low, high),
                                         _mm256_unpackhi_epi64(low, high));
        const auto ordered = _mm256_permute4x64_epi64(groups, _MM_SHUFFLE(3, 1, 2, 0));
        return finish_groups(adjacent_pairs(ordered));
    }
    template <typename Block>
    [[nodiscard]] static summary_type fold(const Block& block, unsigned first,
                                          unsigned last) noexcept {
        if (first == last)
            return base::identity();
        if (last - first == 1)
            return base::slot(block, first);
        if (last - first == 2)
            return base::combine(base::slot(block, first), base::slot(block, first + 1));
        if (last - first == 3)
            return base::combine(base::combine(base::slot(block, first),
                                              base::slot(block, first + 1)),
                                 base::slot(block, first + 2));
        if constexpr (Fanout == 8) {
            return fold_eight(block.multipliers.data(), block.translations.data(), first, last);
        } else {
            if (last <= 8)
                return fold_eight(block.multipliers.data(), block.translations.data(), first, last);
            if (first >= 8)
                return fold_eight(block.multipliers.data() + 8, block.translations.data() + 8,
                                  first - 8, last - 8);
            return fold_sixteen(block.multipliers.data(), block.translations.data(), first, last);
        }
    }
    template <typename Block>
    static void reset(Block& block, unsigned first, unsigned last, summary_type before,
                      summary_type after, std::uint32_t old_tag, std::uint32_t new_tag) noexcept {
        // No old loads: a carried assignment makes the old block irrelevant.
        // Initialize old/new subranges together, rather than writing twice.
        for (unsigned i = 0; i < Fanout; i += 8) {
            const auto selected =
                access::get(simd::lanes_between(int(first) - int(i), int(last) - int(i)));
            const auto choose = [&](std::uint32_t old_word, std::uint32_t new_word) noexcept {
                return _mm256_blendv_epi8(_mm256_set1_epi32(std::bit_cast<int>(old_word)),
                                         _mm256_set1_epi32(std::bit_cast<int>(new_word)), selected);
            };
            _mm256_store_si256(reinterpret_cast<__m256i*>(block.multipliers.data() + i),
                               choose(before.multiplier, after.multiplier));
            _mm256_store_si256(reinterpret_cast<__m256i*>(block.translations.data() + i),
                               choose(before.translation, after.translation));
            if constexpr (std::same_as<Block, branch_type>) {
                _mm256_store_si256(reinterpret_cast<__m256i*>(block.tags.data() + i),
                                   choose(old_tag, new_tag));
            }
        }
    }
    [[nodiscard]] static __m128i load_summary(const summary_type* value) noexcept {
        return pairs::unpack_words(std::bit_cast<std::uint64_t>(*value));
    }
    static void store_summary(summary_type* output, __m128i value) noexcept {
        *output = std::bit_cast<summary_type>(pairs::pack_words(value));
    }
    static void make_powers(summary_type* output, summary_type value, unsigned count) noexcept {
        output[0] = value;
        auto current = pairs::words(value.multiplier, value.translation);
        for (unsigned i = 1; i < count; ++i) {
            const auto product = pairs::multiply(current, pairs::broadcast_first(current));
            current = pairs::add(product, pairs::second_only(current));
            store_summary(output + i, current);
        }
    }
    [[nodiscard]] static summary_type repeat(const summary_type* powers, unsigned count) noexcept {
        if (count == 0)
            return base::identity();
        // Visit set bits only. The ladder entries are powers of ONE value;
        // the general monoid need not be commutative. Ascending exponents keep
        // the same composition order as the scalar reference.
        unsigned i = std::countr_zero(count);
        auto current = load_summary(powers + i);
        count &= count - 1;
        while (count) {
            i = std::countr_zero(count);
            const auto power = load_summary(powers + i);
            current = pairs::add(pairs::multiply(current, pairs::broadcast_first(power)),
                                 pairs::second_only(power));
            count &= count - 1;
        }
        return {pairs::first(current), pairs::second(current)};
    }
};

} // namespace canard::kernel

namespace canard::assignment {

template <std::uint32_t Modulus, unsigned Fanout>
    requires(Modulus > 1 && Modulus < (std::uint32_t{1} << 30) && Modulus % 2 == 1 &&
             (Fanout == 8 || Fanout == 16))
struct kernel_binding<wide::representation::ordinary, wide::execution::avx2,
                      algebra::affine_composition<Modulus>, Fanout> {
    using type = kernel::wide_assignment_affine_avx2<Modulus, Fanout>;
};

} // namespace canard::assignment
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
#include <cstdint>
#include <memory>
#include <span>

namespace {
constexpr std::uint32_t modulus = 998244353;
constexpr std::uint32_t query_flag = 1u << 31;
constexpr unsigned lookahead = 4;
using function = canard::algebra::affine_map<modulus>;
using profile = canard::wide::configuration<16, 500000,
                                           canard::wide::representation::ordinary,
                                           canard::wide::execution::avx2>;
struct operation {
    std::uint32_t first_and_type;
    std::uint32_t last;
    std::uint32_t multiplier_or_argument;
    std::uint32_t translation;
    [[nodiscard]] unsigned first() const noexcept {
        return first_and_type & ~query_flag;
    }
    [[nodiscard]] bool is_query() const noexcept {
        return (first_and_type & query_flag) != 0;
    }
};
static_assert(sizeof(operation) == 16);
} // namespace

int main() {
    canard::io::padded_file file;
    canard::io::trusted_ascii_reader input{file.view()};
    canard::io::buffered_writer<modulus - 1> output;
    const auto [size, count] = input.read_pair();
    auto initial = std::make_unique_for_overwrite<function[]>(size);
    for (unsigned i = 0; i < size; ++i) {
        const auto [a, b] = input.read_pair();
        initial[i] = {a, b};
    }
    auto operations = std::make_unique_for_overwrite<operation[]>(count + lookahead);
    for (unsigned i = 0; i < count; ++i) {
        const auto [type, first, last] = input.read_tagged_pair<6, 6>();
        if (type == 0) {
            const auto [a, b] = input.read_pair();
            operations[i] = {first, last, a, b};
        } else {
            operations[i] = {first | query_flag, last, input.read_u32<9>(), 0};
        }
    }
    for (unsigned i = 0; i < lookahead; ++i)
        operations[count + i] = {0, 0, 0, 0};

    canard::wide_assignment_tree tree{std::span<const function>{initial.get(), size},
                                      canard::algebra::affine_composition<modulus>{}, profile{}};
    initial.reset();
    // Parsing is buffered for prefetch only. Assignments and queries execute
    // in their original order; the tree itself needs no future operations.
    for (unsigned i = 0; i < count; ++i) {
        const auto& future = operations[i + lookahead];
        tree.prefetch(future.first(), future.last);
        const auto& current = operations[i];
        if (current.is_query())
            output.write(
                tree.evaluate(current.first(), current.last, current.multiplier_or_argument));
        else
            tree.assign(current.first(), current.last,
                        {.multiplier = current.multiplier_or_argument,
                         .translation = current.translation});
    }
}
