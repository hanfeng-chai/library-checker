#include <array>
#include <charconv>
#include <chrono>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
#include <version>

#if defined(__SIZEOF_INT128__) && !defined(ATL_NO_INT128)
#define ATL_HAS_INT128 1
#else
#define ATL_HAS_INT128 0
#endif

namespace atl {

	using int32 = std::int32_t;
	using int64 = std::int64_t;

	using uint32 = std::uint32_t;
	using uint64 = std::uint64_t;

#if ATL_HAS_INT128
	using int128 = __int128_t;
	using uint128 = __uint128_t;
#endif

}

namespace atl::traits {

	template<typename Tp, typename... Types>
	struct is_one_of : public std::disjunction<std::is_same<Tp, Types>...> {};

}

namespace atl::traits {

	template<typename T>
	struct is_signed_integral
		: public is_one_of<std::remove_cv_t<T>, signed char, signed short, signed int, signed long, signed long long
#if ATL_HAS_INT128
						   ,
						   int128
#endif
						   > {
	};

	template<typename T>
	constexpr bool is_signed_integral_v = is_signed_integral<T>::value;

	template<typename T>
	struct is_unsigned_integral
		: public is_one_of<std::remove_cv_t<T>, unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long
#if ATL_HAS_INT128
						   ,
						   uint128
#endif
						   > {
	};

	template<typename T>
	constexpr bool is_unsigned_integral_v = is_unsigned_integral<T>::value;

	template<typename T>
	struct is_integral : public std::bool_constant<is_signed_integral_v<T> || is_unsigned_integral_v<T>> {};

	template<typename T>
	constexpr bool is_integral_v = is_integral<T>::value;

	namespace detail {

		template<typename From, typename To>
		struct copy_cv {
			using type = To;
		};

		template<typename From, typename To>
		struct copy_cv<From const, To> {
			using type = To const;
		};

		template<typename From, typename To>
		struct copy_cv<From volatile, To> {
			using type = To volatile;
		};

		template<typename From, typename To>
		struct copy_cv<From const volatile, To> {
			using type = To const volatile;
		};

		template<typename From, typename To>
		using copy_cv_t = typename copy_cv<From, To>::type;
	}

#if ATL_HAS_INT128

#endif

	template<typename T>
	struct make_unsigned : std::make_unsigned<T> {};

#if ATL_HAS_INT128
	template<typename T>
		requires std::is_same_v<std::remove_cv_t<T>, int128>
	struct make_unsigned<T> {
		using type = detail::copy_cv_t<T, uint128>;
	};

	template<typename T>
		requires std::is_same_v<std::remove_cv_t<T>, uint128>
	struct make_unsigned<T> {
		using type = T;
	};
#endif

	template<typename T>
		requires std::is_enum_v<T>
	struct make_unsigned<T>;

	template<typename T>
	using make_unsigned_t = typename make_unsigned<T>::type;
}

namespace atl {

	template<typename T>
	concept integral = traits::is_integral_v<T>;

	template<typename T>
	concept signed_integral = traits::is_signed_integral_v<T>;

	template<typename T>
	concept unsigned_integral = traits::is_unsigned_integral_v<T>;
}

namespace atl::io {

	template<typename T>
	concept readable = atl::integral<T> || std::same_as<T, bool> || std::same_as<T, char>
					|| std::same_as<T, std::string>
#if defined(__cpp_lib_to_chars)
					|| std::floating_point<T>
#endif
		;

	class reader {
		static constexpr std::size_t cap = 1 << 18;
		std::FILE *src_;
		char *owned_ = nullptr;
		char const *cur_ = nullptr;
		char const *end_ = nullptr;
		bool mmapped_ = false;

		void refill() {
			if (mmapped_) return;
			std::size_t rem = std::size_t(end_ - cur_);
			if (rem && cur_ != owned_) std::memmove(owned_, cur_, rem);
			std::size_t got = std::fread(owned_ + rem, 1, cap - rem, src_);
			cur_ = owned_;
			end_ = owned_ + rem + got;
		}

		void ensure() {
			if (!mmapped_ && std::size_t(end_ - cur_) < 64) refill();
		}

		void skip_ws() {
			for (;;) {
				while (cur_ != end_ && static_cast<unsigned char>(*cur_) <= ' ') ++cur_;
				if (cur_ != end_ || mmapped_) return;
				refill();
				if (cur_ == end_) return;
			}
		}

		template<atl::unsigned_integral U>
		U parse_uint() {
			U x = 0;
			while (end_ - cur_ >= 8) {
				std::uint64_t v;
				std::memcpy(&v, cur_, 8);
				v ^= 0x3030303030303030ull;
				if (v & 0xf0f0f0f0f0f0f0f0ull) break;
				v = (v * 10 + (v >> 8)) & 0x00ff00ff00ff00ffull;
				v = (v * 100 + (v >> 16)) & 0x0000ffff0000ffffull;
				v = (v * 10000 + (v >> 32)) & 0x00000000ffffffffull;
				x = x * 100'000'000u + U(v);
				cur_ += 8;
			}
			if (end_ - cur_ >= 4) {
				std::uint32_t v;
				std::memcpy(&v, cur_, 4);
				v ^= 0x30303030u;
				if (!(v & 0xf0f0f0f0u)) {
					v = (v * 10 + (v >> 8)) & 0x00ff00ffu;
					v = (v * 100 + (v >> 16)) & 0x0000ffffu;
					x = x * 10000u + U(v);
					cur_ += 4;
				}
			}
			while (cur_ != end_) {
				unsigned c = static_cast<unsigned char>(*cur_);
				if (c - '0' > 9u) break;
				x = x * 10 + (c - '0');
				++cur_;
			}
			return x;
		}

	public:
		explicit reader(std::FILE *src = stdin) : src_(src) {
			owned_ = new char[cap];
			cur_ = end_ = owned_;
		}

		reader(reader const &) = delete;
		reader &operator=(reader const &) = delete;

		~reader() {
			delete[] owned_;
		}

		template<atl::unsigned_integral U>
		U read_uint() {
			skip_ws();
			ensure();
			return parse_uint<U>();
		}

		template<atl::signed_integral T>
		T read_int() {
			skip_ws();
			ensure();
			bool neg = (cur_ != end_ && *cur_ == '-');
			if (neg) ++cur_;
			using U = atl::traits::make_unsigned_t<T>;
			U v = parse_uint<U>();
			return neg ? T(U(0) - v) : T(v);
		}

		std::string read_token() {
			skip_ws();
			std::string s;
			for (;;) {
				char const *p = cur_;
				while (p != end_ && static_cast<unsigned char>(*p) > ' ') ++p;
				s.append(cur_, p);
				cur_ = p;
				if (p != end_ || mmapped_) break;
				refill();
				if (cur_ == end_) break;
			}
			return s;
		}

#if defined(__cpp_lib_to_chars)

		template<std::floating_point F>
		F read_float() {
			skip_ws();
			ensure();
			char const *p = cur_;
			while (p != end_ && static_cast<unsigned char>(*p) > ' ') ++p;
			F val = 0;
			if (p != end_ || mmapped_) {
				std::from_chars(cur_, p, val);
				cur_ = p;
			} else {
				std::string tok = read_token();
				std::from_chars(tok.data(), tok.data() + tok.size(), val);
			}
			return val;
		}
#endif

		template<readable T>
		T read_one() {
			if constexpr (atl::unsigned_integral<T>)
				return read_uint<T>();
			else if constexpr (atl::signed_integral<T>)
				return read_int<T>();
			else if constexpr (std::is_same_v<T, bool>)
				return read_uint<uint32>() != 0;
			else if constexpr (std::is_same_v<T, char>) {
				skip_ws();
				ensure();
				return cur_ != end_ ? *cur_++ : char(0);
			} else if constexpr (std::is_same_v<T, std::string>)
				return read_token();
#if defined(__cpp_lib_to_chars)
			else if constexpr (std::is_floating_point_v<T>)
				return read_float<T>();
#endif
			else
				;
		}
	};

	inline reader in;

	template<readable T>
	T read() {
		return in.read_one<T>();
	}

}

namespace atl::io {
	namespace detail {

		inline constexpr std::array<char, 200> two_digits = [] {
			std::array<char, 200> t{};
			for (int32 i = 0; i < 100; ++i) {
				t[2 * i] = char('0' + i / 10);
				t[2 * i + 1] = char('0' + i % 10);
			}
			return t;
		}();

	}

	class writer {
		static constexpr std::size_t cap = 1 << 18;
		std::FILE *sink_;
		char *buf_;
		char *cur_;
		char *end_;

		template<atl::unsigned_integral U>
		void put_uint(U x) {
			char tmp[44];
			char *p = tmp + sizeof(tmp);

			while (x >= 100) {
				uint32 d = uint32(x % 100) * 2;
				x /= 100;
				*--p = detail::two_digits[d + 1];
				*--p = detail::two_digits[d];
			}
			if (x >= 10) {
				uint32 d = uint32(x) * 2;
				*--p = detail::two_digits[d + 1];
				*--p = detail::two_digits[d];
			} else {
				*--p = char('0' + uint32(x));
			}

			put(std::string_view(p, std::size_t(tmp + sizeof(tmp) - p)));
		}

	public:
		explicit writer(std::FILE *sink = stdout) : sink_(sink), buf_(new char[cap]), cur_(buf_), end_(buf_ + cap) {}

		writer(writer const &) = delete;
		writer &operator=(writer const &) = delete;

		~writer() {
			flush();
			delete[] buf_;
		}

		void flush() {
			if (cur_ != buf_) {
				std::fwrite(buf_, 1, std::size_t(cur_ - buf_), sink_);
				cur_ = buf_;
			}
		}

		void put(char c) {
			if (cur_ == end_) flush();
			*cur_++ = c;
		}

		void put(bool b) { put(char('0' + b)); }

		void put(std::string_view s) {
			if (s.size() >= cap) {
				flush();
				std::fwrite(s.data(), 1, s.size(), sink_);
				return;
			}
			if (std::size_t(end_ - cur_) < s.size()) flush();
			std::memcpy(cur_, s.data(), s.size());
			cur_ += s.size();
		}

		void put(char const *s) { put(std::string_view(s)); }

		void put(std::string const &s) { put(std::string_view(s)); }

		template<atl::unsigned_integral T>
		void put(T x) {
			put_uint(x);
		}

		template<atl::signed_integral T>
		void put(T x) {
			using U = atl::traits::make_unsigned_t<T>;
			if (x < 0) {
				put('-');
				put_uint(U(U(0) - U(x)));
			} else {
				put_uint(U(x));
			}
		}

#if defined(__cpp_lib_to_chars)

		template<std::floating_point F>
		void put(F x) {
			char tmp[32];
			auto res = std::to_chars(tmp, tmp + sizeof(tmp), x);
			put(std::string_view(tmp, std::size_t(res.ptr - tmp)));
		}

#endif
	};

	template<typename T>
	concept writable = requires(writer &w, T const &x) { w.put(x); };

	inline writer out;

	inline void write() {}

	template<writable T, writable... Ts>
	void write(T const &x, Ts const &...xs) {
		out.put(x);
		((out.put(' '), out.put(xs)), ...);
	}

	template<writable... Ts>
	void writeln(Ts const &...xs) {
		write(xs...);
		out.put('\n');
	}

#if defined(__cpp_lib_to_chars)

#endif

}

namespace atl {
	namespace detail {

		constexpr uint64 splitmix64(uint64 x) noexcept {
			x += 0x9e3779b97f4a7c15ULL;
			x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
			x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
			return x ^ (x >> 31);
		}

		inline uint64 &hash_seed_ref() {
			static uint64 seed = [] {
				uint64 s = uint64(std::chrono::steady_clock::now().time_since_epoch().count());
				std::random_device rd;
				s ^= (uint64(rd()) << 32) ^ rd();
				return splitmix64(s);
			}();
			return seed;
		}

		inline uint64 hash_seed() noexcept { return hash_seed_ref(); }

	}

}

namespace atl::detail {

	template<typename Node>
	concept sibling_node = std::default_initializable<Node> && requires(Node &n) {
		{ n.symbol } -> std::same_as<uint32 &>;
		{ n.left } -> std::same_as<uint32 &>;
		{ n.right } -> std::same_as<uint32 &>;
	};

	template<sibling_node Node>
	class hashed_child_pool {
		std::vector<Node> nodes_;
		uint64 seed_;

		uint32 create(uint32 sym) {
			nodes_.push_back(Node{});
			uint32 const i = uint32(nodes_.size() - 1);
			nodes_[i].symbol = sym;
			nodes_[i].left = nil;
			nodes_[i].right = nil;
			return i;
		}

	public:
		static constexpr uint32 nil = uint32(-1);

		hashed_child_pool() : seed_(hash_seed()) {}

		uint64 order(uint32 sym) const { return splitmix64(uint64(sym) + seed_); }

		Node &operator[](uint32 i) { return nodes_[i]; }

		Node const &operator[](uint32 i) const { return nodes_[i]; }

		uint32 size() const { return uint32(nodes_.size()); }

		void reserve(std::size_t n) { nodes_.reserve(n); }

		uint32 add() {
			nodes_.push_back(Node{});
			return uint32(nodes_.size() - 1);
		}

		uint32 find(uint32 root, uint32 sym) const {
			uint32 cur = root;
			if (cur == nil) return nil;
			uint64 const k = order(sym);
			for (;;) {
				uint32 const c = nodes_[cur].symbol;
				if (c == sym) return cur;
				cur = k < order(c) ? nodes_[cur].left : nodes_[cur].right;
				if (cur == nil) return nil;
			}
		}

		std::pair<uint32, uint32> emplace(uint32 root, uint32 sym) {
			if (root == nil) {
				uint32 const n = create(sym);
				return {n, n};
			}
			uint64 const k = order(sym);
			uint32 cur = root;
			for (;;) {
				uint32 const c = nodes_[cur].symbol;
				if (c == sym) return {cur, root};
				if (k < order(c)) {
					if (nodes_[cur].left == nil) {
						uint32 const n = create(sym);
						nodes_[cur].left = n;
						return {n, root};
					}
					cur = nodes_[cur].left;
				} else {
					if (nodes_[cur].right == nil) {
						uint32 const n = create(sym);
						nodes_[cur].right = n;
						return {n, root};
					}
					cur = nodes_[cur].right;
				}
			}
		}
	};
}

#ifdef ATL_ASSERT
#define atl_assert(condition)                                                                             \
	do {                                                                                                  \
		if (!(condition)) {                                                                               \
			std::fprintf(stderr, "atl_assert: (%s), file %s, line %d\n", #condition, __FILE__, __LINE__); \
			std::abort();                                                                                 \
		}                                                                                                 \
	} while (0)
#else
#define atl_assert(condition) (void)0
#endif

namespace atl {

	template<std::size_t Alphabet>
	class eertree {
		static constexpr uint32 nil = uint32(-1);

		struct node {
			uint32 symbol = 0;
			uint32 left = nil, right = nil;
			uint32 children = nil;
			uint32 parent = nil;
			uint32 link = nil;
			int32 length = 0;
			uint32 suffix_depth = 0;
			uint32 hits = 0;
		};

		detail::hashed_child_pool<node> pool_;
		std::vector<uint32> word_;
		uint32 last_ = 1;

		uint32 extensible(uint32 v, uint32 sym) const {
			auto const pos = uint32(word_.size()) - 1;
			for (;;) {
				int64 const mirror = int64(pos) - pool_[v].length - 1;
				if (mirror >= 0 && word_[uint32(mirror)] == sym) return v;
				v = pool_[v].link;
			}
		}

	public:
		using node_id = uint32;

		eertree() {
			pool_.add();
			pool_.add();
			pool_[0].length = -1;
			pool_[0].link = 0;
			pool_[1].length = 0;
			pool_[1].link = 0;
		}

		node_id even_root() const { return 1; }

		template<std::integral Sym>
		node_id extend(Sym sym) {
			atl_assert(uint64(sym) < uint64(Alphabet));
			uint32 const c = uint32(sym);
			word_.push_back(c);
			uint32 const x = extensible(last_, c);
			if (uint32 const existing = pool_.find(pool_[x].children, c); existing != nil) {
				last_ = existing;
			} else {
				uint32 const slink =
					pool_[x].length == -1
						? even_root()
						: pool_.find(pool_[extensible(pool_[x].link, c)].children, c);
				auto const [y, root] = pool_.emplace(pool_[x].children, c);
				pool_[x].children = root;
				pool_[y].parent = x;
				pool_[y].link = slink;
				pool_[y].length = pool_[x].length + 2;
				pool_[y].suffix_depth = pool_[slink].suffix_depth + 1;
				last_ = y;
			}
			++pool_[last_].hits;
			return last_;
		}

		node_id suffix_link(node_id v) const {
			atl_assert(v != 0 && v < pool_.size());
			return pool_[v].link;
		}

		node_id parent(node_id v) const {
			atl_assert(v >= 2 && v < pool_.size());
			return pool_[v].parent;
		}

		std::size_t palindrome_count() const { return pool_.size() - 2; }

		std::size_t node_count() const { return pool_.size(); }

		void reserve(std::size_t symbols) { pool_.reserve(symbols + 2); }
	};
}

using namespace atl;
namespace io = atl::io;

int main() {
	auto const s = io::read<std::string>();
	eertree<26> t;
	t.reserve(s.size());
	std::vector<uint32> at(s.size());
	for (std::size_t i = 0; i < s.size(); ++i) at[i] = t.extend(s[i] - 'a');

	auto const number = [](uint32 v) { return int64(v) - 1; };
	io::writeln(t.palindrome_count());
	for (uint32 v = 2; v < t.node_count(); ++v) io::writeln(number(t.parent(v)), number(t.suffix_link(v)));
	for (std::size_t i = 0; i < s.size(); ++i) {
		if (i > 0) io::write(' ');
		io::write(number(at[i]));
	}
	io::writeln();
}
