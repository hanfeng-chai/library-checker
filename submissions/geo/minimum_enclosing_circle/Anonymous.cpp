/**
 * Title: Solution for Minimum Enclosing Circle Boundary Points
 * Author: botellot / ICPC BRO
 * Description: Solves the problem of identifying all points on the MEC boundary.
 * Uses Welzl's O(N) algorithm and the exact integer indicator function.
 * Complexity: O(N) expected.
 */

#include <iostream>
#include <vector>
#include <complex>
#include <variant>
#include <array>
#include <algorithm>
#include <random>
#include <chrono>
#include <string>

using namespace std;

using ftype = int64_t;
using point = complex<ftype>;

using mec = variant<
    array<point, 2>,
    array<point, 3>
>;

// Regresa < 0 si esta adentro, > 0 si esta afuera, == 0 si esta EN LA CIRCUNFERENCIA
ftype indicator(mec const& C, point z) {
    return visit([&](auto &&C) {
        point a = C[0], b = C[1];
        point I0 = (b - z) * conj(a - z);
        if constexpr (C.size() == 2) {
            return real(I0);
        } else {
            point c = C[2];
            point I2 = (a - c) * conj(b - c);
            point I1 = I0 * I2;
            return imag(I2) < 0 ? -imag(I1) : imag(I1);
        }
    }, C);
}

bool inside(mec const& C, point p) {
    return indicator(C, p) <= 0;
}

mt19937_64 gen(chrono::steady_clock::now().time_since_epoch().count());

mec enclosing_circle(vector<point> &p) {
    int n = p.size();
    if (n == 0) return array<point, 2>{point{0, 0}, point{0, 0}};
    
    shuffle(p.begin(), p.end(), gen);
    mec C = array<point, 2>{p[0], p[0]};
    
    for (int i = 0; i < n; i++) {
        if (!inside(C, p[i])) {
            C = array<point, 2>{p[i], p[0]};
            for (int j = 0; j < i; j++) {
                if (!inside(C, p[j])) {
                    C = array<point, 2>{p[i], p[j]};
                    for (int k = 0; k < j; k++) {
                        if (!inside(C, p[k])) {
                            C = array<point, 3>{p[i], p[j], p[k]};
                        }
                    }
                }
            }
        }
    }
    return C;
}

void solve() {
    int n;
    if (!(cin >> n)) return;
    
    vector<point> orig_points(n);
    for (int i = 0; i < n; i++) {
        ftype x, y;
        cin >> x >> y;
        orig_points[i] = point{x, y};
    }
    
    // Sacamos una copia para que Welzl la barajee sin perder el orden original
    vector<point> working_points = orig_points;
    mec C = enclosing_circle(working_points);
    
    // Construimos la cadena de respuesta 0/1
    string T = "";
    for (int i = 0; i < n; i++) {
        if (indicator(C, orig_points[i]) == 0) {
            T += '1'; // El punto esta en la frontera
        } else {
            T += '0'; // El punto esta adentro
        }
    }
    
    cout << T << "\n";
}

int main() {
    // Optimización de I/O estándar para la ICPC
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    solve();
    
    return 0;
}