#pragma GCC optimize("Ofast")
#include <bits/stdc++.h>

template <class Cost = long long, class CostSum = std::conditional_t<(sizeof(Cost) < sizeof(long long)), long long, Cost>>
class assignment_problem {
public:
    using cost_type = Cost;
    using cost_sum_type = CostSum;

    enum class side {
        rows,
        cols,
        smaller,
    };

    struct result {
        CostSum cost{};
        std::vector<int> match;
        std::vector<int> inv;
        int pairs = 0;
        bool ok = true;
    };

private:
    enum class square_order_kind {
        none,
        monge,
        anti_monge,
    };

    static constexpr CostSum inf() {
        return std::numeric_limits<CostSum>::max() / 4;
    }

    static std::size_t matrix_size(int n, int m) {
        assert(n >= 0 && m >= 0);
        return static_cast<std::size_t>(n) * static_cast<std::size_t>(m);
    }

    template <class RawCost>
    static square_order_kind detect_square_order_kind(int n, const RawCost* raw_cost, int stride) {
        if (n <= 1) return square_order_kind::monge;

        bool monge = true;
        bool anti_monge = true;
        using check_type = std::conditional_t<std::numeric_limits<CostSum>::is_integer, __int128_t, long double>;

        // 隣接 2x2 の不等式を調べる。途中で両方が否定されたら通常ケースへ戻る
        for (int row = 0; row + 1 < n; row++) {
            const Cost* r0 = raw_cost + static_cast<std::size_t>(row) * static_cast<std::size_t>(stride);
            const Cost* r1 = raw_cost + static_cast<std::size_t>(row + 1) * static_cast<std::size_t>(stride);
            for (int col = 0; col + 1 < n; col++) {
                const check_type lhs = static_cast<check_type>(r0[col]) + static_cast<check_type>(r1[col + 1]);
                const check_type rhs = static_cast<check_type>(r0[col + 1]) + static_cast<check_type>(r1[col]);
                if (lhs > rhs) monge = false;
                if (lhs < rhs) anti_monge = false;
                if (!monge && !anti_monge) return square_order_kind::none;
            }
        }

        if (monge) return square_order_kind::monge;
        if (anti_monge) return square_order_kind::anti_monge;
        return square_order_kind::none;
    }

    static std::vector<int> make_square_order_match(int n, bool reversed) {
        std::vector<int> match(n);
        if (reversed) {
            for (int i = 0; i < n; i++) match[i] = n - 1 - i;
        } else {
            std::iota(match.begin(), match.end(), 0);
        }
        return match;
    }

    template <class RawCost>
    static std::vector<int> solve_square_lapjv_raw_min(int n, const RawCost* raw_cost, int stride) {
        if (n <= 0) return {};
        if (n == 1) return {0};

        const CostSum big = inf();
        std::vector<const RawCost*> row_ptr(n);
        for (int row = 0; row < n; row++) {
            row_ptr[row] = raw_cost + static_cast<std::size_t>(row) * static_cast<std::size_t>(stride);
        }

        std::vector<int> row_mate(n, -1), col_mate(n, -1);
        std::vector<CostSum> pi(n, CostSum{});

        // 列ごとの最小値でポテンシャルを初期化し、競合しない列を貪欲に割り当てる
        std::vector<char> transferable(n, 0);
        for (int col = 0; col < n; col++) {
            int row = 0;
            for (int r = 1; r < n; r++) {
                if (row_ptr[row][col] > row_ptr[r][col]) row = r;
            }
            pi[col] = static_cast<CostSum>(row_ptr[row][col]);
            if (row_mate[row] == -1) {
                row_mate[row] = col;
                col_mate[col] = row;
                transferable[row] = 1;
            } else {
                transferable[row] = 0;
            }
        }

        // 一意に割り当てられた行の余剰を、対応する列ポテンシャルへ移す
        for (int row = 0; row < n; row++) {
            if (!transferable[row]) continue;
            const int col = row_mate[row];
            const RawCost* row_cost = row_ptr[row];
            int best_col = -1;
            CostSum best = big;
            for (int c = 0; c < n; c++) {
                if (c == col) continue;
                const CostSum cur = static_cast<CostSum>(row_cost[c]) - pi[c];
                if (cur < best) {
                    best = cur;
                    best_col = c;
                }
            }
            if (best_col != -1) pi[col] -= best;
        }

        // 未割当行を 2 回の augmenting row reduction で減らす
        for (int it = 0; it < 2; it++) {
            for (int row = 0; row < n; row++) {
                if (row_mate[row] != -1) continue;

                const RawCost* row_cost = row_ptr[row];
                CostSum u1 = static_cast<CostSum>(row_cost[0]) - pi[0];
                CostSum u2 = big;
                int c1 = 0;
                for (int c = 0; c < n; c++) {
                    const CostSum u = static_cast<CostSum>(row_cost[c]) - pi[c];
                    if (u < u1 || (u == u1 && col_mate[c1] != -1)) {
                        u2 = u1;
                        u1 = u;
                        c1 = c;
                    } else if (u < u2) {
                        u2 = u;
                    }
                }

                if (u1 < u2) pi[c1] -= u2 - u1;
                if (const int r1 = col_mate[c1]; r1 != -1) {
                    row_mate[r1] = -1;
                    col_mate[c1] = -1;
                }
                row_mate[row] = c1;
                col_mate[c1] = row;
            }
        }

        // 残った未割当行を dense shortest augmenting path で増加する
        std::vector<int> cols(n);
        std::iota(cols.begin(), cols.end(), 0);
        std::vector<CostSum> dist(n);
        std::vector<int> pred(n);

        for (int start_row = 0; start_row < n; start_row++) {
            if (row_mate[start_row] != -1) continue;

            const RawCost* start_cost = row_ptr[start_row];
            for (int c = 0; c < n; c++) {
                dist[c] = static_cast<CostSum>(start_cost[c]) - pi[c];
                pred[c] = start_row;
            }

            int scanned = 0;
            int labeled = 0;
            int last = 0;
            int free_col = -1;

            while (true) {
                if (scanned == labeled) {
                    last = scanned;
                    CostSum best = dist[cols[scanned]];
                    for (int j = scanned; j < n; j++) {
                        const int c = cols[j];
                        if (dist[c] <= best) {
                            if (dist[c] < best) {
                                best = dist[c];
                                labeled = scanned;
                            }
                            std::swap(cols[j], cols[labeled]);
                            labeled++;
                        }
                    }
                    for (int j = scanned; j < labeled; j++) {
                        const int c = cols[j];
                        if (col_mate[c] == -1) {
                            free_col = c;
                            break;
                        }
                    }
                    if (free_col != -1) break;
                }

                const int c1 = cols[scanned];
                scanned++;
                const int r1 = col_mate[c1];
                const RawCost* row_cost = row_ptr[r1];
                const CostSum base = static_cast<CostSum>(row_cost[c1]) - pi[c1];
                for (int j = labeled; j < n; j++) {
                    const int c2 = cols[j];
                    const CostSum len = static_cast<CostSum>(row_cost[c2]) - pi[c2] - base;
                    const CostSum nd = dist[c1] + len;
                    if (nd < dist[c2]) {
                        dist[c2] = nd;
                        pred[c2] = r1;
                        if (len == CostSum{}) {
                            if (col_mate[c2] == -1) {
                                free_col = c2;
                                break;
                            }
                            std::swap(cols[j], cols[labeled]);
                            labeled++;
                        }
                    }
                }
                if (free_col != -1) break;
            }

            for (int i = 0; i < last; i++) {
                const int c = cols[i];
                pi[c] += dist[c] - dist[free_col];
            }

            int t = free_col;
            while (t != -1) {
                const int col = t;
                const int row = pred[col];
                col_mate[col] = row;
                std::swap(row_mate[row], t);
            }
        }

        return row_mate;
    }

    template <class RawCost>
    static std::vector<int> solve_square_raw_min(int n, const RawCost* raw_cost, int stride) {
        const square_order_kind order = detect_square_order_kind(n, raw_cost, stride);
        if (order == square_order_kind::monge) return make_square_order_match(n, false);
        if (order == square_order_kind::anti_monge) return make_square_order_match(n, true);
        return solve_square_lapjv_raw_min(n, raw_cost, stride);
    }

    template <class CostGetter>
    static std::vector<int> solve_rect_hungarian(int left_size, int right_size, const CostGetter& cost) {
        if (left_size <= 0) return {};
        const CostSum big = inf();

        std::vector<CostSum> u(left_size + 1, CostSum{}), v(right_size + 1, CostSum{});
        std::vector<CostSum> minv(right_size + 1);
        std::vector<int> p(right_size + 1, 0), way(right_size + 1, 0);
        std::vector<char> used(right_size + 1, 0);

        // 左側の各頂点を 1 つずつ追加し、ポテンシャル付き最短増加路を探す
        for (int i = 1; i <= left_size; i++) {
            p[0] = i;
            int j0 = 0;
            std::fill(minv.begin(), minv.end(), big);
            std::fill(used.begin(), used.end(), 0);
            std::fill(way.begin(), way.end(), 0);

            do {
                used[j0] = 1;
                const int i0 = p[j0];
                CostSum delta = big;
                int j1 = 0;
                for (int j = 1; j <= right_size; j++) {
                    if (used[j]) continue;
                    const CostSum cur = cost(i0 - 1, j - 1) - u[i0] - v[j];
                    if (cur < minv[j]) {
                        minv[j] = cur;
                        way[j] = j0;
                    }
                    if (minv[j] < delta) {
                        delta = minv[j];
                        j1 = j;
                    }
                }
                for (int j = 0; j <= right_size; j++) {
                    if (used[j]) {
                        u[p[j]] += delta;
                        v[j] -= delta;
                    } else {
                        minv[j] -= delta;
                    }
                }
                j0 = j1;
            } while (p[j0] != 0);

            // 見つけた増加路を逆向きに辿ってマッチングを更新する
            do {
                const int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while (j0 != 0);
        }

        std::vector<int> left_to_right(left_size, -1);
        for (int j = 1; j <= right_size; j++) {
            if (p[j] != 0) left_to_right[p[j] - 1] = j - 1;
        }
        return left_to_right;
    }

    template <class RawCost>
    static CostSum max_in_flat_matrix(int n, int m, const RawCost* raw_cost, int stride) {
        CostSum max_value = static_cast<CostSum>(raw_cost[0]);
        for (int row = 0; row < n; row++) {
            const RawCost* row_cost = raw_cost + static_cast<std::size_t>(row) * static_cast<std::size_t>(stride);
            for (int col = 0; col < m; col++) {
                const CostSum value = static_cast<CostSum>(row_cost[col]);
                if (max_value < value) max_value = value;
            }
        }
        return max_value;
    }

    static result build_square_result(int n, const Cost* raw_cost, int stride, std::vector<int>&& match) {
        result res;
        res.match = std::move(match);
        res.inv.assign(n, -1);
        CostSum total{};
        for (int row = 0; row < n; row++) {
            const int col = res.match[row];
            res.inv[col] = row;
            total += static_cast<CostSum>(raw_cost[static_cast<std::size_t>(row) * static_cast<std::size_t>(stride) + static_cast<std::size_t>(col)]);
        }
        res.cost = total;
        res.pairs = n;
        res.ok = true;
        return res;
    }

    static result solve_dense_flat(int n, int m, const Cost* raw_cost, side assign_side, bool maximize) {
        static_assert(std::numeric_limits<CostSum>::is_signed, "CostSum must be signed");

        result res;
        res.match.assign(n, -1);
        res.inv.assign(m, -1);

        int left_size = 0;
        int right_size = 0;
        bool left_is_rows = true;
        if (assign_side == side::rows) {
            left_size = n;
            right_size = m;
            left_is_rows = true;
        } else if (assign_side == side::cols) {
            left_size = m;
            right_size = n;
            left_is_rows = false;
        } else {
            left_is_rows = n <= m;
            left_size = left_is_rows ? n : m;
            right_size = left_is_rows ? m : n;
        }

        if (left_size > right_size) {
            res.ok = false;
            return res;
        }
        if (left_size == 0) {
            res.ok = true;
            return res;
        }

        // 正方完全割当は割当側に依存しないため、raw 配列専用実装でまとめて処理する
        if (n == m && left_size == right_size) {
            const square_order_kind order = detect_square_order_kind(n, raw_cost, m);
            if (order != square_order_kind::none) {
                const bool reversed = maximize
                    ? order == square_order_kind::monge
                    : order == square_order_kind::anti_monge;
                return build_square_result(n, raw_cost, m, make_square_order_match(n, reversed));
            }
            if (!maximize) {
                return build_square_result(n, raw_cost, m, solve_square_lapjv_raw_min(n, raw_cost, m));
            }

            const CostSum max_value = max_in_flat_matrix(n, m, raw_cost, m);
            std::vector<CostSum> transformed(matrix_size(n, m));
            for (int row = 0; row < n; row++) {
                const std::size_t base = static_cast<std::size_t>(row) * static_cast<std::size_t>(m);
                for (int col = 0; col < m; col++) {
                    transformed[base + static_cast<std::size_t>(col)] = max_value - static_cast<CostSum>(raw_cost[base + static_cast<std::size_t>(col)]);
                }
            }
            return build_square_result(n, raw_cost, m, solve_square_lapjv_raw_min(n, transformed.data(), m));
        }

        CostSum max_value{};
        if (maximize) max_value = max_in_flat_matrix(n, m, raw_cost, m);

        auto build_result = [&](const std::vector<int>& left_to_right) -> result {
            CostSum total{};
            if (left_is_rows) {
                for (int row = 0; row < left_size; row++) {
                    const int col = left_to_right[row];
                    res.match[row] = col;
                    res.inv[col] = row;
                    total += static_cast<CostSum>(raw_cost[static_cast<std::size_t>(row) * static_cast<std::size_t>(m) + static_cast<std::size_t>(col)]);
                }
            } else {
                for (int col = 0; col < left_size; col++) {
                    const int row = left_to_right[col];
                    res.match[row] = col;
                    res.inv[col] = row;
                    total += static_cast<CostSum>(raw_cost[static_cast<std::size_t>(row) * static_cast<std::size_t>(m) + static_cast<std::size_t>(col)]);
                }
            }
            res.cost = total;
            res.pairs = left_size;
            res.ok = true;
            return res;
        };

        auto solve_with = [&](const auto& get) -> result {
            return build_result(solve_rect_hungarian(left_size, right_size, get));
        };

        if (left_is_rows) {
            std::vector<const Cost*> row_ptr(n);
            for (int row = 0; row < n; row++) {
                row_ptr[row] = raw_cost + static_cast<std::size_t>(row) * static_cast<std::size_t>(m);
            }
            if (maximize) {
                auto get = [&](int left, int right) -> CostSum {
                    return max_value - static_cast<CostSum>(row_ptr[left][right]);
                };
                return solve_with(get);
            }
            auto get = [&](int left, int right) -> CostSum {
                return static_cast<CostSum>(row_ptr[left][right]);
            };
            return solve_with(get);
        }

        if (maximize) {
            auto get = [&](int left, int right) -> CostSum {
                return max_value - static_cast<CostSum>(raw_cost[static_cast<std::size_t>(right) * static_cast<std::size_t>(m) + static_cast<std::size_t>(left)]);
            };
            return solve_with(get);
        }
        auto get = [&](int left, int right) -> CostSum {
            return static_cast<CostSum>(raw_cost[static_cast<std::size_t>(right) * static_cast<std::size_t>(m) + static_cast<std::size_t>(left)]);
        };
        return solve_with(get);
    }

    static std::vector<Cost> flatten_matrix(const std::vector<std::vector<Cost>>& cost, int& n, int& m) {
        n = static_cast<int>(cost.size());
        m = n == 0 ? 0 : static_cast<int>(cost[0].size());
        std::vector<Cost> flat(static_cast<std::size_t>(n) * static_cast<std::size_t>(m));
        for (int i = 0; i < n; i++) {
            assert(static_cast<int>(cost[i].size()) == m);
            const std::size_t base = static_cast<std::size_t>(i) * static_cast<std::size_t>(m);
            for (int j = 0; j < m; j++) {
                flat[base + static_cast<std::size_t>(j)] = cost[i][j];
            }
        }
        return flat;
    }

public:
    // 空のコスト行列を作成する O(1)
    assignment_problem() = default;

    // N 行 M 列のコスト行列を 0 で作成 O(NM)
    assignment_problem(int n, int m) : n_(n), m_(m), cost_(matrix_size(n, m)) {}

    // N 行 M 列のコスト行列を init で作成 O(NM)
    assignment_problem(int n, int m, const Cost& init) : n_(n), m_(m), cost_(matrix_size(n, m), init) {}

    // 行数を返す O(1)
    int rows() const {
        return n_;
    }

    // 列数を返す O(1)
    int cols() const {
        return m_;
    }

    // row 行 col 列のコストへの参照を返す O(1)
    Cost& operator()(int row, int col) {
        assert(0 <= row && row < n_ && 0 <= col && col < m_);
        return cost_[static_cast<std::size_t>(row) * static_cast<std::size_t>(m_) + static_cast<std::size_t>(col)];
    }

    // row 行 col 列のコストへの const 参照を返す O(1)
    const Cost& operator()(int row, int col) const {
        assert(0 <= row && row < n_ && 0 <= col && col < m_);
        return cost_[static_cast<std::size_t>(row) * static_cast<std::size_t>(m_) + static_cast<std::size_t>(col)];
    }

    // row 行 col 列のコストを設定する O(1)
    void set(int row, int col, const Cost& value) {
        (*this)(row, col) = value;
    }

    // flat な内部配列への const 参照を返す O(1)
    const std::vector<Cost>& data() const {
        return cost_;
    }

    // flat な内部配列への参照を返す O(1)
    std::vector<Cost>& data() {
        return cost_;
    }

    // 指定した側を全て割り当てる最小コスト割当を求める O(K^2L)
    result solve_min(side assign_side = side::rows) const {
        return solve_dense_flat(n_, m_, cost_.data(), assign_side, false);
    }

    // 指定した側を全て割り当てる最大コスト割当を求める O(K^2L)
    result solve_max(side assign_side = side::rows) const {
        return solve_dense_flat(n_, m_, cost_.data(), assign_side, true);
    }

    // row-major flat 配列から最小コスト割当を求める O(K^2L)
    static result solve_min(int n, int m, const Cost* cost, side assign_side = side::rows) {
        return solve_dense_flat(n, m, cost, assign_side, false);
    }

    // row-major flat 配列から最大コスト割当を求める O(K^2L)
    static result solve_max(int n, int m, const Cost* cost, side assign_side = side::rows) {
        return solve_dense_flat(n, m, cost, assign_side, true);
    }

    // vector<vector<Cost>> から最小コスト割当を求める O(K^2L)
    static result solve_min(const std::vector<std::vector<Cost>>& cost, side assign_side = side::rows) {
        int n = 0;
        int m = 0;
        std::vector<Cost> flat = flatten_matrix(cost, n, m);
        return solve_dense_flat(n, m, flat.data(), assign_side, false);
    }

    // vector<vector<Cost>> から最大コスト割当を求める O(K^2L)
    static result solve_max(const std::vector<std::vector<Cost>>& cost, side assign_side = side::rows) {
        int n = 0;
        int m = 0;
        std::vector<Cost> flat = flatten_matrix(cost, n, m);
        return solve_dense_flat(n, m, flat.data(), assign_side, true);
    }

    // N x N の raw 配列から最小コスト割当のコストと match を求める Monge/anti-Monge では O(N^2)、通常 O(N^3)
    static std::pair<CostSum, std::vector<int>> min_square(int n, const Cost* cost, int stride = -1) {
        static_assert(std::numeric_limits<CostSum>::is_signed, "CostSum must be signed");
        if (stride < 0) stride = n;
        std::vector<int> match = solve_square_raw_min(n, cost, stride);
        CostSum total{};
        for (int row = 0; row < n; row++) {
            total += static_cast<CostSum>(cost[static_cast<std::size_t>(row) * static_cast<std::size_t>(stride) + static_cast<std::size_t>(match[row])]);
        }
        return {total, std::move(match)};
    }

    // N x N の flat vector から最小コスト割当のコストと match を求める Monge/anti-Monge では O(N^2)、通常 O(N^3)
    static std::pair<CostSum, std::vector<int>> min_square(int n, const std::vector<Cost>& cost) {
        assert(static_cast<std::size_t>(n) * static_cast<std::size_t>(n) <= cost.size());
        return min_square(n, cost.data(), n);
    }

private:
    int n_ = 0;
    int m_ = 0;
    std::vector<Cost> cost_;
};

class fast_scanner {
public:
    int read_int() {
        int sign = 1;
        int c = read_char();
        while (c <= ' ' && c != 0) c = read_char();
        if (c == '-') {
            sign = -1;
            c = read_char();
        }
        int x = 0;
        while (c > ' ') {
            x = x * 10 + (c - '0');
            c = read_char();
        }
        return x * sign;
    }

private:
    static constexpr int buf_size = 1 << 20;
    char buf_[buf_size]{};
    int pos_ = 0;
    int len_ = 0;

    int read_char() {
        if (pos_ == len_) {
            len_ = static_cast<int>(fread_unlocked(buf_, 1, buf_size, stdin));
            pos_ = 0;
            if (len_ == 0) return 0;
        }
        return buf_[pos_++];
    }
};

class fast_printer {
public:
    ~fast_printer() {
        flush();
    }

    void put_char(char c) {
        if (pos_ == buf_size) flush();
        buf_[pos_++] = c;
    }

    void write_long_long(long long x) {
        if (x == 0) {
            put_char('0');
            return;
        }
        if (x < 0) {
            put_char('-');
            x = -x;
        }
        char s[32];
        int n = 0;
        while (x > 0) {
            s[n++] = static_cast<char>('0' + x % 10);
            x /= 10;
        }
        while (n--) put_char(s[n]);
    }

    void write_int(int x) {
        write_long_long(x);
    }

    void flush() {
        if (pos_ > 0) {
            fwrite_unlocked(buf_, 1, pos_, stdout);
            pos_ = 0;
        }
    }

private:
    static constexpr int buf_size = 1 << 20;
    char buf_[buf_size]{};
    int pos_ = 0;
};

int main() {
    fast_scanner in;
    const int n = in.read_int();
    std::vector<int> cost(static_cast<std::size_t>(n) * static_cast<std::size_t>(n));
    for (int i = 0; i < n; i++) {
        const std::size_t base = static_cast<std::size_t>(i) * static_cast<std::size_t>(n);
        for (int j = 0; j < n; j++) {
            cost[base + static_cast<std::size_t>(j)] = in.read_int();
        }
    }

    auto ans = assignment_problem<int, long long>::min_square(n, cost.data(), n);

    fast_printer out;
    out.write_long_long(ans.first);
    out.put_char('\n');
    for (int i = 0; i < n; i++) {
        if (i) out.put_char(' ');
        out.write_int(ans.second[i]);
    }
    out.put_char('\n');
    return 0;
}
