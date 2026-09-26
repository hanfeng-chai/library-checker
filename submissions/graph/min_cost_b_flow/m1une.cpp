// BEGIN: verify/graph/flow/min_cost_b_flow.test.cpp
#line 1 "verify::graph::flow::min_cost_b_flow.test.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/min_cost_b_flow"

// BEGIN: ../../../graph/flow/bounded_min_cost_flow.hpp
#line 3 "..::..::..::graph::flow::bounded_min_cost_flow.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <optional>
#include <utility>
#include <vector>

namespace m1une {
namespace flow {

template <class Cap, class Cost, class TotalCost = Cost>
struct BoundedMinCostFlow {
    static_assert(std::numeric_limits<Cap>::is_integer);
    static_assert(std::numeric_limits<Cap>::is_signed);

    struct Edge {
        int from;
        int to;
        Cap lower;
        Cap upper;
        Cost cost;
    };

    struct ResultEdge {
        int from;
        int to;
        Cap lower;
        Cap upper;
        Cap flow;
        Cost cost;
    };

    struct Result {
        std::vector<ResultEdge> edges;
        std::vector<Cap> balance;
        std::vector<Cost> potential;
        TotalCost cost;

        ResultEdge get_edge(int i) const {
            assert(0 <= i && i < int(edges.size()));
            return edges[i];
        }

        Cap flow(int i) const {
            assert(0 <= i && i < int(edges.size()));
            return edges[i].flow;
        }
    };

   private:
    struct NetworkEdge {
        int to;
        Cap cap;
        Cost cost;
    };

    struct NetworkSimplexSolver {
        struct Parent {
            int vertex;
            int edge;
            Cap up;
            Cap down;
        };

        int n;
        std::vector<NetworkEdge> edges;
        std::vector<Cap> excess;
        std::vector<Cost> potential;

        NetworkSimplexSolver(int vertex_count, const std::vector<Cap>& balance)
            : n(vertex_count), excess(balance) {}

        void reserve_edges(int edge_count) {
            edges.reserve(2 * (edge_count + n));
        }

        int add_edge(int from, int to, Cap lower, Cap upper, Cost cost) {
            int id = int(edges.size()) / 2;
            edges.push_back(NetworkEdge{to, upper - lower, cost});
            edges.push_back(NetworkEdge{from, Cap(0), -cost});
            excess[from] -= lower;
            excess[to] += lower;
            return id;
        }

        bool solve() {
            const int original_edge_count = int(edges.size());
            potential.assign(n + 1, Cost(0));

            Cost artificial_cost = Cost(1);
            for (int edge = 0; edge < original_edge_count; edge += 2) {
                artificial_cost += edges[edge].cost < Cost(0)
                    ? -edges[edge].cost : edges[edge].cost;
            }

            std::vector<Parent> parent(n);
            edges.reserve(original_edge_count + 2 * n);
            for (int vertex = 0; vertex < n; vertex++) {
                if (excess[vertex] >= Cap(0)) {
                    edges.push_back(NetworkEdge{n, Cap(0), artificial_cost});
                    edges.push_back(NetworkEdge{vertex, excess[vertex], -artificial_cost});
                    potential[vertex] = -artificial_cost;
                } else {
                    edges.push_back(NetworkEdge{n, -excess[vertex], -artificial_cost});
                    edges.push_back(NetworkEdge{vertex, Cap(0), artificial_cost});
                    potential[vertex] = artificial_cost;
                }
                int edge = int(edges.size()) - 2;
                parent[vertex] = Parent{
                    n, edge, edges[edge].cap, edges[edge ^ 1].cap
                };
            }

            std::vector<int> depth(n + 1, 1);
            depth[n] = 0;
            std::vector<int> next(2 * (n + 1));
            std::vector<int> previous(2 * (n + 1));
            auto connect = [&](int first, int second) {
                next[first] = second;
                previous[second] = first;
            };
            for (int vertex = 0; vertex <= n; vertex++) {
                connect(2 * vertex, 2 * vertex + 1);
            }
            for (int vertex = 0; vertex < n; vertex++) {
                connect(2 * vertex + 1, next[2 * n]);
                connect(2 * n, 2 * vertex);
            }

            auto push_flow = [&](int entering_edge) {
                const int first = edges[entering_edge ^ 1].to;
                const int second = edges[entering_edge].to;
                const Cost cycle_cost =
                    edges[entering_edge].cost
                    + potential[first] - potential[second];

                Cap amount = edges[entering_edge].cap;
                bool leave_first_side = true;
                int leaving_vertex = second;

                int first_ancestor = first;
                int second_ancestor = second;
                auto move_first_up = [&] {
                    if (parent[first_ancestor].down < amount) {
                        amount = parent[first_ancestor].down;
                        leaving_vertex = first_ancestor;
                        leave_first_side = true;
                    }
                    first_ancestor = parent[first_ancestor].vertex;
                };
                auto move_second_up = [&] {
                    if (parent[second_ancestor].up <= amount) {
                        amount = parent[second_ancestor].up;
                        leaving_vertex = second_ancestor;
                        leave_first_side = false;
                    }
                    second_ancestor = parent[second_ancestor].vertex;
                };
                if (depth[first_ancestor] >= depth[second_ancestor]) {
                    int difference = depth[first_ancestor] - depth[second_ancestor];
                    for (int i = 0; i < difference; i++) move_first_up();
                } else {
                    int difference = depth[second_ancestor] - depth[first_ancestor];
                    for (int i = 0; i < difference; i++) move_second_up();
                }
                while (first_ancestor != second_ancestor) {
                    move_first_up();
                    move_second_up();
                }
                const int ancestor = first_ancestor;

                if (amount != Cap(0)) {
                    int vertex = first;
                    while (vertex != ancestor) {
                        parent[vertex].up += amount;
                        parent[vertex].down -= amount;
                        vertex = parent[vertex].vertex;
                    }
                    vertex = second;
                    while (vertex != ancestor) {
                        parent[vertex].up -= amount;
                        parent[vertex].down += amount;
                        vertex = parent[vertex].vertex;
                    }
                }

                int vertex = first;
                int new_parent = second;
                std::pair<Cap, Cap> parent_capacities{
                    edges[entering_edge].cap - amount,
                    edges[entering_edge ^ 1].cap + amount
                };
                Cost potential_difference = -cycle_cost;
                if (!leave_first_side) {
                    std::swap(vertex, new_parent);
                    std::swap(parent_capacities.first, parent_capacities.second);
                    potential_difference = -potential_difference;
                }
                int parent_edge = entering_edge ^ (leave_first_side ? 0 : 1);

                while (new_parent != leaving_vertex) {
                    int new_depth = depth[new_parent];
                    int tour_index = 2 * vertex;
                    while (tour_index != 2 * vertex + 1) {
                        if ((tour_index & 1) == 0) {
                            new_depth++;
                            potential[tour_index / 2] += potential_difference;
                            depth[tour_index / 2] = new_depth;
                        } else {
                            new_depth--;
                        }
                        tour_index = next[tour_index];
                    }

                    connect(previous[2 * vertex], next[2 * vertex + 1]);
                    connect(2 * vertex + 1, next[2 * new_parent]);
                    connect(2 * new_parent, 2 * vertex);

                    std::swap(parent[vertex].edge, parent_edge);
                    parent_edge ^= 1;
                    std::swap(parent[vertex].up, parent_capacities.first);
                    std::swap(parent[vertex].down, parent_capacities.second);
                    std::swap(parent_capacities.first, parent_capacities.second);

                    int old_parent = parent[vertex].vertex;
                    parent[vertex].vertex = new_parent;
                    new_parent = vertex;
                    vertex = old_parent;
                }
                edges[parent_edge].cap = parent_capacities.first;
                edges[parent_edge ^ 1].cap = parent_capacities.second;
            };

            const int candidate_limit = std::max(
                int(0.2 * std::sqrt(double(original_edge_count))), 10
            );
            const int minor_limit = std::max(candidate_limit / 10, 3);
            std::vector<int> candidates;
            candidates.reserve(candidate_limit);

            auto minor_pivot = [&] {
                Cost best_cost = Cost(0);
                int best_edge = -1;
                int index = 0;
                while (index < int(candidates.size())) {
                    int edge = candidates[index];
                    if (edges[edge].cap == Cap(0)) {
                        candidates[index] = candidates.back();
                        candidates.pop_back();
                        continue;
                    }
                    Cost reduced_cost =
                        edges[edge].cost
                        + potential[edges[edge ^ 1].to]
                        - potential[edges[edge].to];
                    if (reduced_cost >= Cost(0)) {
                        candidates[index] = candidates.back();
                        candidates.pop_back();
                        continue;
                    }
                    if (reduced_cost < best_cost) {
                        best_cost = reduced_cost;
                        best_edge = edge;
                    }
                    index++;
                }
                if (best_edge == -1) return false;
                push_flow(best_edge);
                return true;
            };

            int edge = 0;
            while (true) {
                for (int iteration = 0; iteration < minor_limit; iteration++) {
                    if (!minor_pivot()) break;
                }

                Cost best_cost = Cost(0);
                int best_edge = -1;
                candidates.clear();
                for (int scanned = 0; scanned < int(edges.size()); scanned++) {
                    if (edges[edge].cap != Cap(0)) {
                        Cost reduced_cost =
                            edges[edge].cost
                            + potential[edges[edge ^ 1].to]
                            - potential[edges[edge].to];
                        if (reduced_cost < Cost(0)) {
                            if (reduced_cost < best_cost) {
                                best_cost = reduced_cost;
                                best_edge = edge;
                            }
                            candidates.push_back(edge);
                            if (int(candidates.size()) == candidate_limit) break;
                        }
                    }
                    edge++;
                    if (edge == int(edges.size())) edge = 0;
                }
                if (candidates.empty()) break;
                push_flow(best_edge);
            }

            for (int vertex = 0; vertex < n; vertex++) {
                edges[parent[vertex].edge].cap = parent[vertex].up;
                edges[parent[vertex].edge ^ 1].cap = parent[vertex].down;
            }

            bool feasible = true;
            for (int vertex = 0; vertex < n; vertex++) {
                int artificial_edge = original_edge_count + 2 * vertex;
                if (
                    (excess[vertex] >= Cap(0)
                        && edges[artificial_edge ^ 1].cap != Cap(0))
                    || (excess[vertex] < Cap(0)
                        && edges[artificial_edge].cap != Cap(0))
                ) {
                    feasible = false;
                    break;
                }
            }
            potential.pop_back();
            return feasible;
        }

        Cap edge_flow(int edge_id, Cap lower) const {
            return lower + edges[2 * edge_id + 1].cap;
        }
    };

    int _n;
    std::vector<Edge> _edges;
    std::vector<Cap> _balance;

   public:
    BoundedMinCostFlow() : BoundedMinCostFlow(0) {}

    explicit BoundedMinCostFlow(int n) : _n(n), _balance(n, Cap(0)) {
        assert(0 <= n);
    }

    int size() const {
        return _n;
    }

    int edge_count() const {
        return int(_edges.size());
    }

    void reserve_edges(int edge_count) {
        assert(0 <= edge_count);
        _edges.reserve(edge_count);
    }

    int add_edge(int from, int to, Cap lower, Cap upper, Cost cost) {
        assert(0 <= from && from < _n);
        assert(0 <= to && to < _n);
        assert(lower <= upper);
        int id = int(_edges.size());
        _edges.push_back(Edge{from, to, lower, upper, cost});
        return id;
    }

    Edge get_edge(int i) const {
        assert(0 <= i && i < int(_edges.size()));
        return _edges[i];
    }

    std::vector<Edge> edges() const {
        return _edges;
    }

    void set_balance(int v, Cap b) {
        assert(0 <= v && v < _n);
        _balance[v] = b;
    }

    void add_balance(int v, Cap b) {
        assert(0 <= v && v < _n);
        _balance[v] += b;
    }

    void add_supply(int v, Cap supply) {
        assert(Cap(0) <= supply);
        add_balance(v, supply);
    }

    void add_demand(int v, Cap demand) {
        assert(Cap(0) <= demand);
        add_balance(v, -demand);
    }

    Cap balance(int v) const {
        assert(0 <= v && v < _n);
        return _balance[v];
    }

    const std::vector<Cap>& balances() const {
        return _balance;
    }

    std::optional<Result> min_cost_flow() const {
        return min_cost_flow(_balance);
    }

    std::optional<Result> min_cost_flow(const std::vector<Cap>& balance) const {
        assert(int(balance.size()) == _n);
        Cap balance_sum = Cap(0);
        for (Cap value : balance) balance_sum += value;
        if (balance_sum != Cap(0)) return std::nullopt;

        NetworkSimplexSolver solver(_n, balance);
        solver.reserve_edges(int(_edges.size()));
        for (const auto& edge : _edges) {
            solver.add_edge(edge.from, edge.to, edge.lower, edge.upper, edge.cost);
        }
        if (!solver.solve()) return std::nullopt;

        Result result;
        result.balance = balance;
        result.cost = TotalCost(0);
        result.edges.reserve(_edges.size());
        for (int i = 0; i < int(_edges.size()); i++) {
            const auto& e = _edges[i];
            Cap flow = solver.edge_flow(i, e.lower);
            result.cost += TotalCost(flow) * TotalCost(e.cost);
            result.edges.push_back(ResultEdge{e.from, e.to, e.lower, e.upper, flow, e.cost});
        }
        result.potential = std::move(solver.potential);
        return result;
    }

    std::optional<Result> min_cost_st_flow(int s, int t, Cap flow_value) const {
        assert(0 <= s && s < _n);
        assert(0 <= t && t < _n);
        assert(s != t);
        std::vector<Cap> balance = _balance;
        balance[s] += flow_value;
        balance[t] -= flow_value;
        return min_cost_flow(balance);
    }
};

template <class Cap, class Cost, class TotalCost = Cost>
using BMinCostFlow = BoundedMinCostFlow<Cap, Cost, TotalCost>;

}  // namespace flow
}  // namespace m1une

// END: ../../../graph/flow/bounded_min_cost_flow.hpp
#line 4 "verify::graph::flow::min_cost_b_flow.test.cpp"
// BEGIN: ../../../utilities/int128.hpp
#line 3 "..::..::..::utilities::int128.hpp"

#include <algorithm>
#include <cctype>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>

namespace m1une {
namespace utilities {

using i128 = __int128_t;
using u128 = __uint128_t;

inline std::string to_string(u128 x) {
    if (x == 0) {
        return "0";
    }
    std::string s;
    while (x > 0) {
        s.push_back(static_cast<char>('0' + x % 10));
        x /= 10;
    }
    std::reverse(s.begin(), s.end());
    return s;
}

inline std::string to_string(i128 x) {
    if (x < 0) {
        u128 magnitude = static_cast<u128>(-(x + 1)) + 1;
        return "-" + to_string(magnitude);
    }
    return to_string(static_cast<u128>(x));
}

inline u128 parse_uint128(const std::string& s) {
    if (s.empty()) {
        throw std::invalid_argument("empty string");
    }
    u128 value = 0;
    for (char c : s) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            throw std::invalid_argument("invalid unsigned __int128 literal");
        }
        value = value * 10 + static_cast<unsigned>(c - '0');
    }
    return value;
}

inline i128 parse_int128(const std::string& s) {
    if (s.empty()) {
        throw std::invalid_argument("empty string");
    }
    bool negative = s[0] == '-';
    std::size_t pos = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    if (pos == s.size()) {
        throw std::invalid_argument("invalid __int128 literal");
    }

    i128 value = 0;
    for (; pos < s.size(); ++pos) {
        char c = s[pos];
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            throw std::invalid_argument("invalid __int128 literal");
        }
        int digit = c - '0';
        value = value * 10 + (negative ? -digit : digit);
    }
    return value;
}

}  // namespace utilities
}  // namespace m1une

inline std::ostream& operator<<(std::ostream& os, __uint128_t x) {
    return os << m1une::utilities::to_string(x);
}

inline std::ostream& operator<<(std::ostream& os, __int128_t x) {
    return os << m1une::utilities::to_string(x);
}

inline std::istream& operator>>(std::istream& is, __uint128_t& x) {
    std::string s;
    is >> s;
    if (is) {
        x = m1une::utilities::parse_uint128(s);
    }
    return is;
}

inline std::istream& operator>>(std::istream& is, __int128_t& x) {
    std::string s;
    is >> s;
    if (is) {
        x = m1une::utilities::parse_int128(s);
    }
    return is;
}

// END: ../../../utilities/int128.hpp
#line 5 "verify::graph::flow::min_cost_b_flow.test.cpp"

#include <cassert>
// BEGIN: ../../../utilities/fast_io.hpp
#line 3 "..::..::..::utilities::fast_io.hpp"

#include <array>
#include <charconv>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>

namespace m1une {
namespace utilities {
namespace internal {

// Detect std::begin(x), std::end(x).
template <class T, class = void>
struct is_range : std::false_type {};

template <class T>
struct is_range<T, std::void_t<
    decltype(std::begin(std::declval<T&>())),
    decltype(std::end(std::declval<T&>()))
>> : std::true_type {};

template <class T>
inline constexpr bool is_range_v = is_range<T>::value;

template <class T>
using range_reference_t = decltype(*std::begin(std::declval<T&>()));

template <class T>
using range_value_t = std::remove_cv_t<std::remove_reference_t<range_reference_t<T>>>;

template <class T, class = void>
struct range_stored_value {
    using type = range_value_t<T>;
};

template <class T>
struct range_stored_value<T, std::void_t<typename std::remove_cv_t<std::remove_reference_t<T>>::value_type>> {
    using type = typename std::remove_cv_t<std::remove_reference_t<T>>::value_type;
};

template <class T>
using range_stored_value_t = typename range_stored_value<T>::type;

// Treat strings and C strings as scalar output objects, not as ranges.
template <class T>
struct is_char_array : std::false_type {};

template <class T, std::size_t N>
struct is_char_array<T[N]>
    : std::bool_constant<std::is_same_v<std::remove_cv_t<T>, char>> {};

template <class T>
struct is_string_like
    : std::bool_constant<
          std::is_same_v<std::decay_t<T>, std::string>
          || std::is_same_v<std::decay_t<T>, const char*>
          || std::is_same_v<std::decay_t<T>, char*>
          || is_char_array<std::remove_reference_t<T>>::value
      > {};

template <class T>
inline constexpr bool is_string_like_v = is_string_like<T>::value;

// ModInt-like type: x.val() is printable, and x can be assigned from long long.
template <class T, class = void>
struct has_val_method : std::false_type {};

template <class T>
struct has_val_method<T, std::void_t<decltype(std::declval<const T&>().val())>>
    : std::true_type {};

template <class T>
inline constexpr bool has_val_method_v = has_val_method<T>::value;

template <class T, class = void>
struct has_static_mod_raw : std::false_type {};

template <class T>
struct has_static_mod_raw<
    T, std::void_t<decltype(T::mod()), decltype(T::raw(std::declval<uint32_t>()))>>
    : std::true_type {};

template <class T>
inline constexpr bool has_static_mod_raw_v = has_static_mod_raw<T>::value;

// libstdc++ before GCC 16 does not classify __int128 as an integral type in
// strict ISO modes such as -std=c++23. Keep the fast-I/O interface independent
// of that implementation detail.
template <class T>
inline constexpr bool is_integral_v =
    std::is_integral_v<T>
    || std::is_same_v<std::remove_cv_t<T>, __int128_t>
    || std::is_same_v<std::remove_cv_t<T>, __uint128_t>;

template <class T>
inline constexpr bool is_signed_v =
    std::is_signed_v<T>
    || std::is_same_v<std::remove_cv_t<T>, __int128_t>;

template <class T>
struct make_unsigned {
    using type = std::make_unsigned_t<T>;
};

template <>
struct make_unsigned<__int128_t> {
    using type = __uint128_t;
};

template <>
struct make_unsigned<__uint128_t> {
    using type = __uint128_t;
};

template <class T>
using make_unsigned_t = typename make_unsigned<std::remove_cv_t<T>>::type;

}  // namespace internal

struct FastInput {
    static constexpr int buffer_size = 1 << 20;

   private:
    std::FILE* _stream;
    char _buffer[buffer_size];
    int _position;
    int _length;

    bool prepare_number() {
        if (_length - _position >= 64) return true;
        const int remaining = _length - _position;
        if (remaining > 0) std::memmove(_buffer, _buffer + _position, remaining);
        const int added = int(std::fread(_buffer + remaining, 1, buffer_size - remaining, _stream));
        _position = 0;
        _length = remaining + added;
        if (_length < buffer_size) _buffer[_length] = '\0';
        return _length != 0;
    }

   public:
    explicit FastInput(std::FILE* stream = stdin)
        : _stream(stream), _position(0), _length(0) {}

    FastInput(const FastInput&) = delete;
    FastInput& operator=(const FastInput&) = delete;

    int read_char_raw() {
        if (_position == _length) {
            _length = int(std::fread(_buffer, 1, buffer_size, _stream));
            _position = 0;
            if (_length == 0) return EOF;
        }
        return _buffer[_position++];
    }

    bool skip_spaces() {
        int c = read_char_raw();
        while (c != EOF && c <= ' ') c = read_char_raw();
        if (c == EOF) return false;
        --_position;
        return true;
    }

    bool read(char& value) {
        if (!skip_spaces()) return false;
        value = char(read_char_raw());
        return true;
    }

    bool read(std::string& value) {
        if (!skip_spaces()) return false;
        value.clear();
        int c = read_char_raw();
        while (c != EOF && c > ' ') {
            value.push_back(char(c));
            c = read_char_raw();
        }
        return true;
    }

    bool read(bool& value) {
        int x;
        if (!read(x)) return false;
        value = x != 0;
        return true;
    }

    template <class T>
    std::enable_if_t<
        internal::is_integral_v<T>
            && !std::is_same_v<std::remove_cv_t<T>, bool>
            && !std::is_same_v<std::remove_cv_t<T>, char>,
        bool
    >
    read(T& value) {
        if (!prepare_number()) return false;
        int c = static_cast<unsigned char>(_buffer[_position++]);
        while (c <= ' ') c = static_cast<unsigned char>(_buffer[_position++]);

        bool negative = false;
        if (c == '-') {
            negative = true;
            c = static_cast<unsigned char>(_buffer[_position++]);
        }

        if constexpr (internal::is_signed_v<T>) {
            T result = 0;
            while ('0' <= c && c <= '9') {
                const int first = c - '0';
                const int second = static_cast<unsigned char>(_buffer[_position]) - '0';
                if (0 <= second && second <= 9) {
                    result = negative ? result * 100 - (first * 10 + second)
                                      : result * 100 + (first * 10 + second);
                    ++_position;
                } else {
                    result = negative ? result * 10 - first : result * 10 + first;
                }
                c = static_cast<unsigned char>(_buffer[_position++]);
            }
            value = result;
        } else {
            T result = 0;
            while ('0' <= c && c <= '9') {
                const unsigned first = unsigned(c - '0');
                const int second = static_cast<unsigned char>(_buffer[_position]) - '0';
                if (0 <= second && second <= 9) {
                    result = result * 100 + T(first * 10 + unsigned(second));
                    ++_position;
                } else {
                    result = result * 10 + T(first);
                }
                c = static_cast<unsigned char>(_buffer[_position++]);
            }
            value = negative ? T(0) - result : result;
        }
        if (_position > _length) _position = _length;
        return true;
    }

    template <class T>
    std::enable_if_t<std::is_floating_point_v<T>, bool>
    read(T& value) {
        if (!skip_spaces()) return false;
        int c = read_char_raw();
        bool negative = false;
        if (c == '-' || c == '+') {
            negative = c == '-';
            c = read_char_raw();
        }

        long double result = 0;
        while ('0' <= c && c <= '9') {
            result = result * 10 + (c - '0');
            c = read_char_raw();
        }
        if (c == '.') {
            long double place = 0.1L;
            c = read_char_raw();
            while ('0' <= c && c <= '9') {
                result += (c - '0') * place;
                place *= 0.1L;
                c = read_char_raw();
            }
        }
        if (c == 'e' || c == 'E') {
            c = read_char_raw();
            bool exponent_negative = false;
            if (c == '-' || c == '+') {
                exponent_negative = c == '-';
                c = read_char_raw();
            }
            int exponent = 0;
            while ('0' <= c && c <= '9') {
                exponent = exponent * 10 + (c - '0');
                c = read_char_raw();
            }
            long double scale = 1;
            long double power = 10;
            while (exponent > 0) {
                if (exponent & 1) scale *= power;
                power *= power;
                exponent >>= 1;
            }
            result = exponent_negative ? result / scale : result * scale;
        }
        value = static_cast<T>(negative ? -result : result);
        return true;
    }

    template <class T>
    std::enable_if_t<
        internal::has_val_method_v<T>
            && !internal::is_integral_v<T>
            && !internal::is_range_v<T>,
        bool
    >
    read(T& value) {
        long long x;
        if (!read(x)) return false;
        if constexpr (internal::has_static_mod_raw_v<T>) {
            if (x >= 0 && uint64_t(x) < uint64_t(T::mod())) {
                value = T::raw(uint32_t(x));
            } else {
                value = T(x);
            }
        } else {
            value = T(x);
        }
        return true;
    }

    template <class Range>
    std::enable_if_t<
        internal::is_range_v<Range>
            && !internal::is_string_like_v<Range>,
        bool
    >
    read(Range& range) {
        using StoredValue = internal::range_stored_value_t<Range>;
        constexpr bool nested = internal::is_range_v<StoredValue>
                                && !internal::is_string_like_v<StoredValue>;

        for (auto&& value : range) {
            if constexpr (std::is_same_v<StoredValue, bool> && !nested) {
                bool x;
                if (!read(x)) return false;
                value = x;
            } else {
                if (!read(value)) return false;
            }
        }
        return true;
    }

    template <class First, class Second, class... Rest>
    bool read(First& first, Second& second, Rest&... rest) {
        if (!read(first)) return false;
        return read(second, rest...);
    }

    template <class T>
    FastInput& operator>>(T& value) {
        if (!read(value)) std::abort();
        return *this;
    }
};

struct FastOutput {
    static constexpr int buffer_size = 1 << 20;

   private:
    inline static const auto digit_quads = [] {
        std::array<char, 40000> result{};
        for (int i = 0; i < 10000; i++) {
            int value = i;
            for (int j = 3; j >= 0; j--) {
                result[4 * i + j] = char('0' + value % 10);
                value /= 10;
            }
        }
        return result;
    }();

    std::FILE* _stream;
    char _buffer[buffer_size];
    int _position;
    int _precision;
    std::chars_format _float_format;

   public:
    explicit FastOutput(std::FILE* stream = stdout)
        : _stream(stream),
          _position(0),
          _precision(6),
          _float_format(std::chars_format::general) {}

    FastOutput(const FastOutput&) = delete;
    FastOutput& operator=(const FastOutput&) = delete;

    ~FastOutput() {
        flush();
    }

    void flush() {
        if (_position == 0) return;
        std::fwrite(_buffer, 1, _position, _stream);
        _position = 0;
    }

    void write_char(char c) {
        if (_position == buffer_size) flush();
        _buffer[_position++] = c;
    }

    void write(const char* s) {
        while (*s != '\0') write_char(*s++);
    }

    void write(const std::string& s) {
        for (char c : s) write_char(c);
    }

    void write(char c) {
        write_char(c);
    }

    void write(bool value) {
        write_char(value ? '1' : '0');
    }

    template <class T>
    std::enable_if_t<std::is_floating_point_v<T>>
    write(T value) {
        char digits[128];
        auto [end, error] = std::to_chars(
            digits,
            digits + sizeof(digits),
            value,
            _float_format,
            _precision
        );
        if (error != std::errc()) std::abort();
        for (const char* pointer = digits; pointer != end; pointer++) {
            write_char(*pointer);
        }
    }

    template <class T>
    std::enable_if_t<
        internal::is_integral_v<T>
            && !std::is_same_v<std::remove_cv_t<T>, bool>
            && !std::is_same_v<std::remove_cv_t<T>, char>
    >
    write(T value) {
        using Raw = std::remove_cv_t<T>;
        using Unsigned = internal::make_unsigned_t<Raw>;

        Unsigned magnitude;
        if constexpr (internal::is_signed_v<Raw>) {
            if (value < 0) {
                write_char('-');
                magnitude = Unsigned(0) - Unsigned(value);
            } else {
                magnitude = Unsigned(value);
            }
        } else {
            magnitude = value;
        }

        if (magnitude == 0) {
            write_char('0');
            return;
        }

        unsigned chunks[16];
        int count = 0;
        while (magnitude >= 10000) {
            const Unsigned quotient = magnitude / 10000;
            chunks[count++] = unsigned(magnitude - quotient * 10000);
            magnitude = quotient;
        }
        if (_position > buffer_size - 64) flush();
        const unsigned leading = unsigned(magnitude);
        const char* first = digit_quads.data() + 4 * leading;
        int skip = leading < 10 ? 3 : leading < 100 ? 2 : leading < 1000 ? 1 : 0;
        for (; skip < 4; skip++) _buffer[_position++] = first[skip];
        while (count--) {
            const char* digits = digit_quads.data() + 4 * chunks[count];
            std::memcpy(_buffer + _position, digits, 4);
            _position += 4;
        }
    }

    template <class T>
    std::enable_if_t<
        internal::has_val_method_v<T>
            && !internal::is_integral_v<T>
            && !internal::is_range_v<T>
    >
    write(const T& value) {
        write(value.val());
    }

    template <class Range>
    std::enable_if_t<
        internal::is_range_v<Range>
            && !internal::is_string_like_v<Range>
    >
    write(const Range& range) {
        using StoredValue = internal::range_stored_value_t<const Range>;
        constexpr bool nested = internal::is_range_v<StoredValue>
                                && !internal::is_string_like_v<StoredValue>;

        bool first = true;
        for (const auto& value : range) {
            if (!first) write_char(nested ? '\n' : ' ');
            first = false;
            if constexpr (std::is_same_v<StoredValue, bool> && !nested) {
                write(static_cast<bool>(value));
            } else {
                write(value);
            }
        }
    }

    template <class First, class... Rest>
    void print(const First& first, const Rest&... rest) {
        write(first);
        ((write_char(' '), write(rest)), ...);
    }

    void println() {
        write_char('\n');
    }

    void set_precision(int precision) {
        _precision = precision;
    }

    void set_fixed(int precision = 6) {
        _float_format = std::chars_format::fixed;
        _precision = precision;
    }

    void set_general(int precision = 6) {
        _float_format = std::chars_format::general;
        _precision = precision;
    }

    template <class... Args>
    void println(const Args&... args) {
        print(args...);
        write_char('\n');
    }

    template <class T>
    FastOutput& operator<<(const T& value) {
        write(value);
        return *this;
    }
};

}  // namespace utilities
}  // namespace m1une

// END: ../../../utilities/fast_io.hpp
#line 8 "verify::graph::flow::min_cost_b_flow.test.cpp"

int main() {
    m1une::utilities::FastInput fast_input;
    m1une::utilities::FastOutput fast_output;

    using Flow = long long;
    using Cost = long long;
    using TotalCost = __int128_t;
    using Solver = m1une::flow::BoundedMinCostFlow<Flow, Cost, TotalCost>;

    int vertex_count, edge_count;
    fast_input >> vertex_count >> edge_count;
    Solver solver(vertex_count);
    solver.reserve_edges(edge_count);
    for (int vertex = 0; vertex < vertex_count; vertex++) {
        Flow balance;
        fast_input >> balance;
        solver.set_balance(vertex, balance);
    }
    for (int edge = 0; edge < edge_count; edge++) {
        int from, to;
        Flow lower, upper;
        long long cost;
        fast_input >> from >> to >> lower >> upper >> cost;
        solver.add_edge(from, to, lower, upper, cost);
    }

    auto result = solver.min_cost_flow();
    if (!result.has_value()) {
        fast_output << "infeasible\n";
        return 0;
    }

    assert(int(result->potential.size()) == vertex_count);
    for (const auto& edge : result->edges) {
        Cost reduced_cost =
            edge.cost + result->potential[edge.from] - result->potential[edge.to];
        if (edge.flow < edge.upper) assert(Cost(0) <= reduced_cost);
        if (edge.lower < edge.flow) assert(reduced_cost <= Cost(0));
    }

    fast_output << result->cost << '\n';
    for (Cost potential : result->potential) fast_output << potential << '\n';
    for (const auto& edge : result->edges) fast_output << edge.flow << '\n';
}
// END: verify/graph/flow/min_cost_b_flow.test.cpp
