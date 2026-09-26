#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#pragma GCC optimize("-fgcse")
#pragma GCC optimize("-fgcse-lm")
#pragma GCC optimize("-fipa-sra")
#pragma GCC optimize("-ftree-pre")
#pragma GCC optimize("-ftree-vrp")
#pragma GCC optimize("-fpeephole2")
#pragma GCC optimize("-ffast-math")
#pragma GCC optimize("-fsched-spec")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("-falign-jumps")
#pragma GCC optimize("-falign-loops")
#pragma GCC optimize("-falign-labels")
#pragma GCC optimize("-fdevirtualize")
#pragma GCC optimize("-fcaller-saves")
#pragma GCC optimize("-fcrossjumping")
#pragma GCC optimize("-fthread-jumps")
#pragma GCC optimize("-funroll-loops")
#pragma GCC optimize("-fwhole-program")
#pragma GCC optimize("-freorder-blocks")
#pragma GCC optimize("-fschedule-insns")
#pragma GCC optimize("inline-functions")
#pragma GCC optimize("-ftree-tail-merge")
#pragma GCC optimize("-fschedule-insns2")
#pragma GCC optimize("-fstrict-aliasing")
#pragma GCC optimize("-fstrict-overflow")
#pragma GCC optimize("-falign-functions")
#pragma GCC optimize("-fcse-skip-blocks")
#pragma GCC optimize("-fcse-follow-jumps")
#pragma GCC optimize("-fsched-interblock")
#pragma GCC optimize("-fpartial-inlining")
#pragma GCC optimize("no-stack-protector")
#pragma GCC optimize("-freorder-functions")
#pragma GCC optimize("-findirect-inlining")
#pragma GCC optimize("-fhoist-adjacent-loads")
#pragma GCC optimize("-frerun-cse-after-loop")
#pragma GCC optimize("inline-small-functions")
#pragma GCC optimize("-finline-small-functions")
#pragma GCC optimize("-ftree-switch-conversion")
#pragma GCC optimize("-foptimize-sibling-calls")
#pragma GCC optimize("-fexpensive-optimizations")
#pragma GCC optimize("-funsafe-loop-optimizations")
#pragma GCC optimize("inline-functions-called-once")
#pragma GCC optimize("-fdelete-null-pointer-checks")
// test loading GMP from toxicpie

#include <cassert>
#include <iomanip>
#include <string.h>
#include <sstream>
#include <string>
#include <unordered_map>

#include <unistd.h>

using namespace std;

// resolves a library at runtime
// you must have ld load the library before using this,
// otherwise it will die and blow up everything
struct DLL 
{
    char *base;
    unordered_map<string, size_t> syms;
    DLL(const char *file) 
    {
        // obtain symbols of requested library
        // objdump -T <elf> | awk '{if($4 == ".text") print $1,$7}'
        char buf[1024];
        string command = "objdump -T " + string(file) +
                         " | awk '{if($4 == \".text\") print $7,$1}'";
        auto cmd = popen(command.c_str(), "r");
        while (fgets(buf, 1024, cmd)) 
        {
            istringstream ss(buf);
            string tmp;
            ss >> tmp, ss >> hex >> syms[tmp];
        }
        pclose(cmd);

        // obtain base address of requested library
        // awk '{if(index($6, <elf>) != 0 && $3 == "00000000") print
        // substr($1, 1, index($1, "-") - 1)}' /proc/<pid>/maps
        string maps_file = "/proc/" + to_string(getpid()) + "/maps";
        command = "awk '{if(index($6, \"" + string(file) +
                  "\") != 0 && $3 == \"00000000\") print"
                  " substr($1, 1, index($1, \"-\") - 1)}' " +
                  maps_file;
        cmd = popen(command.c_str(), "r");
        while (fgets(buf, 1024, cmd)) 
            base = reinterpret_cast<char *>(stoul(buf, 0, 16));
        
        pclose(cmd);
    }
    template <typename R = void, typename... T>
    constexpr R call(const char *name, T... t) 
    {
        auto off = syms[string(name)];
        assert(off != 0);
        return ((R(*)(T...))(base + off))(t...);
    };
};

constexpr const char *LIB_PATH = "/usr/lib/x86_64-linux-gnu/libgmp.so.10";

using mpz_t = char[16];

DLL *gmp;

struct fastIO
{
    static const int BUFF_SZ = 1 << 18;
    char inbuf[BUFF_SZ], outbuf[BUFF_SZ];
    fastIO()
    {
        setvbuf(stdin, inbuf, _IOFBF, BUFF_SZ);
        setvbuf(stdout, outbuf, _IOFBF, BUFF_SZ);
    }
} IO;

int t;
char sa[2000010], sb[2000010], sq[2000010], sr[2000010];

int main(int argc, char **argv) 
{
    if (getenv("LD_PRELOAD") == nullptr) 
    {
        setenv("LD_PRELOAD", LIB_PATH, 1);
        execve("/proc/self/exe", argv, environ);
        exit(0);
    }

    gmp = new DLL(LIB_PATH);

    mpz_t a, b, q, r;

    gmp->call("__gmpz_init", a);
    gmp->call("__gmpz_init", b);
    gmp->call("__gmpz_init", q);
    gmp->call("__gmpz_init", r);

    scanf("%d", &t);
    while (t--) 
    {
        scanf("%s%s", sa, sb);
        gmp->call("__gmpz_set_str", a, sa, 16);
        gmp->call("__gmpz_set_str", b, sb, 16);
        gmp->call("__gmpz_tdiv_qr", q, r, a, b);
        gmp->call("__gmpz_get_str", sq, -16, q);
        gmp->call("__gmpz_get_str", sr, -16, r);
        printf("%s %s\n", sq, sr);
    }

    gmp->call("__gmpz_clear", a);
    gmp->call("__gmpz_clear", b);
    gmp->call("__gmpz_clear", q);
    gmp->call("__gmpz_clear", r);
}
