#include <iostream>

#include <gmp.h>

using namespace std;

static_assert(sizeof(unsigned long) == 8);

int main() {
    ios_base::sync_with_stdio(0), cin.tie(0);

    mpz_t a, b, q, r;

    mpz_inits(a, b, q, r, NULL);

    int t;
    cin >> t;
    while (t--) {
        string sa, sb;
        cin >> sa >> sb;
        if (sa.size() <= 16 && sb.size() <= 16) {
            unsigned long ua = stoul(sa, nullptr, 16),
                          ub = stoul(sb, nullptr, 16);
            auto uq = ua / ub, ur = ua % ub;
            gmp_printf("%lX %lX\n", uq, ur);
        } else if (sa.size() <= 16) {
            gmp_printf("0 %s\n", sa.c_str());
        } else if (sb.size() <= 16) {
            mpz_set_str(a, sa.c_str(), 16);
            unsigned long ub = stoul(sb, nullptr, 16);
            auto ur = mpz_tdiv_q_ui(q, a, ub);
            gmp_printf("%ZX %lX\n", q, ur);
        } else {
            mpz_set_str(a, sa.c_str(), 16);
            mpz_set_str(b, sb.c_str(), 16);
            mpz_tdiv_qr(q, r, a, b);
            gmp_printf("%ZX %ZX\n", q, r);
        }
    }

    mpz_clears(a, b, q, r, NULL);
}
