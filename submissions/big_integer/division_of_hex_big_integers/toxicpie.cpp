#include <dlfcn.h>
#include <iostream>
#include <unistd.h>
#include <unordered_map>

using namespace std;


// assumes no bad things will ever happen
namespace DLL {
unordered_map<string, void *> syms;
void *find_name(const char *name) {
    if (auto it = syms.find(name); it != syms.end()) {
        return it->second;
    }
    return syms[name] = dlsym(RTLD_DEFAULT, name);
}
template <typename R = void, typename... T> R call(const char *name, T... t) {
    auto func = reinterpret_cast<R (*)(T...)>(find_name(name));
    return func(t...);
};
}; // namespace DLL

#ifdef EVAL
constexpr const char *LIB_PATH = "/usr/lib/x86_64-linux-gnu/libgmp.so.10";
#else
constexpr const char *LIB_PATH = "libgmp.so";
#endif

static_assert(sizeof(unsigned long) == 8);
using mpz_t = char[16];

int main(int argc, char **argv) {
    if (getenv("LD_PRELOAD") == nullptr) {
        setenv("LD_PRELOAD", LIB_PATH, 1);
        execve("/proc/self/exe", argv, environ);
        exit(0);
    }

    ios_base::sync_with_stdio(0), cin.tie(0);

    mpz_t a, b, q, r;

    DLL::call("__gmpz_init", a);
    DLL::call("__gmpz_init", b);
    DLL::call("__gmpz_init", q);
    DLL::call("__gmpz_init", r);

    int t;
    cin >> t;
    while (t--) {
        string sa, sb;
        cin >> sa >> sb;
        if (sa.size() <= 16 && sb.size() <= 16) {
            unsigned long ua = stoul(sa, nullptr, 16),
                          ub = stoul(sb, nullptr, 16);
            auto uq = ua / ub, ur = ua % ub;
            DLL::call("__gmp_printf", "%lX %lX\n", uq, ur);
        } else if (sa.size() <= 16) {
            DLL::call("__gmp_printf", "0 %s\n", sa.c_str());
        } else if (sb.size() <= 16) {
            DLL::call("__gmpz_set_str", a, sa.c_str(), 16);
            unsigned long ub = stoul(sb, nullptr, 16);
            auto ur = DLL::call<unsigned long>("__gmpz_tdiv_q_ui", q, a, ub);
            DLL::call("__gmp_printf", "%ZX %lX\n", q, ur);
        } else {
            DLL::call("__gmpz_set_str", a, sa.c_str(), 16);
            DLL::call("__gmpz_set_str", b, sb.c_str(), 16);
            DLL::call("__gmpz_tdiv_qr", q, r, a, b);
            DLL::call("__gmp_printf", "%ZX %ZX\n", q, r);
        }
    }

    DLL::call("__gmpz_clear", a);
    DLL::call("__gmpz_clear", b);
    DLL::call("__gmpz_clear", q);
    DLL::call("__gmpz_clear", r);
}
