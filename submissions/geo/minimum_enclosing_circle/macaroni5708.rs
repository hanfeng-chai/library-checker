use std::io::{self, Read};

#[derive(Clone, Copy, Debug)]
struct P {
    x: f64,
    y: f64,
}

#[derive(Clone, Copy, Debug)]
struct Circle {
    c: P,
    r2: f64,
}

fn dist2(a: P, b: P) -> f64 {
    let dx = a.x - b.x;
    let dy = a.y - b.y;
    dx * dx + dy * dy
}

fn circle_from_1(a: P) -> Circle {
    Circle { c: a, r2: 0.0 }
}

fn circle_from_2(a: P, b: P) -> Circle {
    let cx = (a.x + b.x) / 2.0;
    let cy = (a.y + b.y) / 2.0;
    Circle {
        c: P { x: cx, y: cy },
        r2: dist2(P { x: cx, y: cy }, a),
    }
}

fn circle_from_3(a: P, b: P, c: P) -> Circle {
    let x1 = a.x;
    let y1 = a.y;
    let x2 = b.x;
    let y2 = b.y;
    let x3 = c.x;
    let y3 = c.y;
    let d = 2.0 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
    let eps = 1e-12;
    if d.abs() < eps {
        // ほぼ共線なら最大距離の2点の円にする
        let d12 = dist2(a, b);
        let d23 = dist2(b, c);
        let d31 = dist2(c, a);
        if d12 >= d23 && d12 >= d31 {
            return circle_from_2(a, b);
        } else if d23 >= d12 && d23 >= d31 {
            return circle_from_2(b, c);
        } else {
            return circle_from_2(c, a);
        }
    }
    let s1 = x1 * x1 + y1 * y1;
    let s2 = x2 * x2 + y2 * y2;
    let s3 = x3 * x3 + y3 * y3;
    let ux = (s1 * (y2 - y3) + s2 * (y3 - y1) + s3 * (y1 - y2)) / d;
    let uy = (s1 * (x3 - x2) + s2 * (x1 - x3) + s3 * (x2 - x1)) / d;
    let center = P { x: ux, y: uy };
    Circle {
        c: center,
        r2: dist2(center, a),
    }
}

fn is_outside(circle: &Circle, p: P) -> bool {
    let eps: f64 = 1e-7f64;
    dist2(circle.c, p) > circle.r2 + eps.max(1e-12f64 * circle.r2.abs())
}

// ===== 自作乱数生成器 (LCG) =====
struct MyRng {
    state: u64,
}
impl MyRng {
    fn new(seed: u64) -> Self {
        Self { state: seed }
    }
    fn next_u64(&mut self) -> u64 {
        // LCG: X_{n+1} = a*X_n + c (mod 2^64)
        self.state = self.state.wrapping_mul(6364136223846793005).wrapping_add(1);
        self.state
    }
    fn gen_range(&mut self, range: std::ops::Range<usize>) -> usize {
        let span = (range.end - range.start) as u64;
        (self.next_u64() % span) as usize + range.start
    }
}
fn shuffle<T>(v: &mut [T], rng: &mut MyRng) {
    let n = v.len();
    for i in (1..n).rev() {
        let j = rng.gen_range(0..i + 1);
        v.swap(i, j);
    }
}

fn read_buffer() -> Vec<i64> {
    let mut buffer = String::new();
    io::stdin()
        .read_line(&mut buffer)
        .expect("Failed to read line.");
    buffer
        .trim()
        .split_whitespace()
        .map(|s| s.parse().expect("Failed to parse."))
        .collect()
}

fn main() {
    // read input
    let n = read_buffer()[0] as usize;
    let mut pts: Vec<(P, usize)> = Vec::with_capacity(n);
    for i in 0..n {
        let xy = read_buffer();
        let x = xy[0] as f64;
        let y = xy[1] as f64;
        pts.push((P { x: x, y: y }, i));
    }

    // シャッフル (期待計算量を保証)
    let mut rng = MyRng::new(88172645463325252); // 適当なシード
    shuffle(&mut pts, &mut rng);

    // Welzl
    let mut circ = Circle {
        c: P { x: 0.0, y: 0.0 },
        r2: -1.0,
    };
    for i in 0..n {
        let (pi, _) = pts[i];
        if circ.r2 >= 0.0 && !is_outside(&circ, pi) {
            continue;
        }
        circ = circle_from_1(pi);
        for j in 0..i {
            let (pj, _) = pts[j];
            if !is_outside(&circ, pj) {
                continue;
            }
            circ = circle_from_2(pi, pj);
            for k in 0..j {
                let (pk, _) = pts[k];
                if !is_outside(&circ, pk) {
                    continue;
                }
                circ = circle_from_3(pi, pj, pk);
            }
        }
    }

    // 出力
    let mut orig = vec![P { x: 0.0, y: 0.0 }; n];
    for (p, idx) in pts.iter() {
        orig[*idx] = *p;
    }
    let mut ans = vec!['0'; n];
    let r2 = circ.r2;
    let tol = (1e-7f64).max(1e-12f64 * r2.abs());
    for i in 0..n {
        let d2 = dist2(circ.c, orig[i]);
        if (d2 - r2).abs() <= tol {
            ans[i] = '1';
        }
    }
    println!("{}", ans.into_iter().collect::<String>());
}
