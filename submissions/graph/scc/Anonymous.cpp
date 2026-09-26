#include <algorithm>
#include <cerrno>
#include <charconv>
#include <concepts>
#include <cstddef>
#include <cstdlib>
#include <iterator>
#include <span>
#include <unistd.h>
#include <utility>

struct adjacency_list_node {
    int next, vertex;
};

struct stack_node {
    int low_link, parent_or_vertex;
    void update_low_link(int low_link) { this->low_link = std::min(this->low_link, low_link); }
};

template <auto &buffer, std::size_t digit_count> class input {
    static constexpr char *buffer_begin = std::begin(buffer);
    static constexpr const char *buffer_end = std::end(buffer);
    static_assert(buffer_begin + digit_count < buffer_end);
    const char *cursor = buffer_begin;
    char *input_end = buffer_begin;

  public:
    int read() {
        skip_whitespace();
        if (cursor >= buffer_end - digit_count) {
            input_end = std::copy(cursor, static_cast<const char *>(input_end), buffer_begin);
            cursor = buffer_begin;
        }
        while (input_end <= cursor + digit_count && refill()) {}
        int value;
        cursor = std::from_chars(cursor, input_end, value).ptr;
        return value;
    }

  private:
    void skip_whitespace() {
        do {
            while (cursor != input_end) {
                if (*cursor <= ' ') {
                    ++cursor;
                } else {
                    return;
                }
            }
            cursor = input_end = buffer_begin;
        } while (refill());
    }

    bool refill() {
        for (;;) {
            const auto read_count = ::read(STDIN_FILENO, input_end, buffer_end - input_end);
            if (read_count > 0) {
                input_end += read_count;
                return true;
            } else if (read_count == 0) {
                return false;
            } else if (errno != EINTR) {
                std::abort();
            }
        }
    }
};

template <auto &buffer> class output {
    static constexpr char *output_begin = std::begin(buffer);
    static constexpr char *output_end = std::end(buffer);
    char *cursor = output_begin;

  public:
    ~output() {
        auto *buf = output_begin;
        for (auto nbyte = cursor - buf; nbyte;) {
            const auto count = ::write(STDOUT_FILENO, buf, nbyte);
            if (count > 0) {
                buf += count;
                nbyte -= count;
            } else if (count != -1 || errno != EINTR) {
                std::abort();
            }
        }
    }

    template <typename T> void write(T) = delete;
    void write(char value) { *cursor++ = value; }
    void write(bool value) { *cursor++ = '0' + value; }
    void write(std::integral auto value) { cursor = std::to_chars(cursor, output_end, value).ptr; }
};

template <int *adjacency_list, adjacency_list_node *adjacency_list_nodes, int *link,
          stack_node *stack>
class tarjan {
    int vertex_count, back = 1, popped = vertex_count + 1;

  public:
    explicit tarjan(int vertex_count) : vertex_count(vertex_count) {}

    void visit() {
        for (int vertex = 0; vertex != vertex_count; ++vertex) {
            if (!try_push_stack(vertex, popped)) {
                enter_component();
                visit(vertex);
                exit_component();
            }
        }
    }

  private:
    void visit(int vertex) {
        const int vertex_link = link[vertex];
        for (int &edge = adjacency_list[vertex]; edge; edge = adjacency_list_nodes[edge].next) {
            const int child = adjacency_list_nodes[edge].vertex;
            if (const int low_link = try_push_stack(child, vertex)) {
                stack[vertex_link].update_low_link(low_link);
            } else {
                __attribute__((musttail)) return visit(child);
            }
        }

        const int low_link = stack[vertex_link].low_link;
        const int parent = std::exchange(stack[vertex_link].parent_or_vertex, vertex);
        if (low_link == vertex_link) {
            enter_strong_component();
            while (back != vertex_link) {
                const int vertex = stack[--back].parent_or_vertex;
                link[vertex] = popped;
                yield_vertex(vertex);
            }
            exit_strong_component();
        }
        if (parent != popped) {
            stack[link[parent]].update_low_link(low_link);
            __attribute__((musttail)) return visit(parent);
        }
    }

    int try_push_stack(int vertex, int parent) {
        int &low_link = link[vertex];
        if (low_link) {
            return low_link;
        }
        low_link = back++;
        stack[low_link] = {low_link, parent};
        return 0;
    }

  protected:
    virtual void enter_component() {}
    virtual void enter_strong_component() {}
    virtual void yield_vertex(int) {}
    virtual void exit_strong_component() {}
    virtual void exit_component() {}
};

template <int *sorted_end, auto... args> class topological_sort : public tarjan<args...> {
    int component_count = 0, *sorted_begin = sorted_end, *component_end;

  public:
    using tarjan<args...>::tarjan;

    struct iterator {
        using iterator_concept = std::input_iterator_tag;
        using value_type = std::span<const int>;
        using difference_type = std::ptrdiff_t;

        int *vertex;

        value_type operator*() const { return {vertex + 1, static_cast<std::size_t>(*vertex)}; }
        iterator &operator++() {
            vertex += *vertex + 1;
            return *this;
        }
        void operator++(int) { ++*this; }
        bool operator==(const iterator &) const = default;
    };

    iterator begin() const { return {sorted_begin}; }
    iterator end() const { return {sorted_end}; }
    std::size_t size() const { return component_count; }

  private:
    void enter_strong_component() override { component_end = sorted_begin; }
    void yield_vertex(int vertex) override { *--sorted_begin = vertex; }
    void exit_strong_component() override {
        *--sorted_begin = component_end - sorted_begin;
        ++component_count;
    }
};

int main() {
    constexpr std::size_t vertex_count_max = 500'000, edge_count_max = vertex_count_max;
    constexpr std::size_t digit_count = 6;
    constexpr std::size_t output_size_max = (digit_count + sizeof('\n')) +
                                            (sizeof('1') + sizeof('\n')) * vertex_count_max +
                                            (sizeof(' ') * vertex_count_max + 2'888'890);

    static constinit union {
        struct {
            int adjacency_list[vertex_count_max];
            adjacency_list_node adjacency_list_nodes[edge_count_max + 1];
        } graph;
        static_assert(sizeof(graph) == 6'000'008);
        char output[output_size_max];
        static_assert(sizeof(output) == 4'388'897);
    } graph_storage{};

    static constinit union {
        struct {
            stack_node stack[vertex_count_max + 1];
            int link[vertex_count_max];
        } tarjan;
        static_assert(sizeof(tarjan) == 6'000'008);
        char input[sizeof(tarjan)];
    } work_storage{};

    constexpr auto *adjacency_list = graph_storage.graph.adjacency_list;
    constexpr auto *adjacency_list_nodes = graph_storage.graph.adjacency_list_nodes;
    constexpr auto *link = work_storage.tarjan.link;
    constexpr auto *stack = work_storage.tarjan.stack;

    input<work_storage.input, digit_count> input;
    const int vertex_count = input.read(), edge_count = input.read();
    for (int edge = 1; edge <= edge_count; ++edge) {
        const int source = input.read(), target = input.read();
        adjacency_list_nodes[edge] = {std::exchange(adjacency_list[source], edge), target};
    }
    std::fill_n(link, vertex_count, 0);

    topological_sort<link, adjacency_list, adjacency_list_nodes, link, stack> sorted(vertex_count);
    sorted.visit();

    output<graph_storage.output> output;
    output.write(sorted.size());
    output.write('\n');
    for (const auto component : sorted) {
        output.write(component.size());
        for (const int vertex : component) {
            output.write(' ');
            output.write(vertex);
        }
        output.write('\n');
    }
}
