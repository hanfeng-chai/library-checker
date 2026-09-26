fn main() {
    //stress();
    input! {
        t: usize,
        ask: [(i64, i64, i64); t],
    }
    for (k, a, p) in ask {
        if let Some(x) = kth_root(a, k, p) {
            assert!(pow_mod(x, k, p) == a);
            println!("{}", x);
        } else {
            println!("-1");
        }
    }
}

fn stress() {
    for p in 2..1000 {
        if !is_prime(p) {
            continue;
        }
        println!("{p}");
        for k in 1..1000 {
            let mut elem = vec![false; p as usize];
            for x in 0..p {
                elem[pow_mod(x, k, p) as usize] = true;
            }
            for a in 0..p {
                if let Some(x) = kth_root(a, k, p) {
                    assert!(pow_mod(x, k, p) == a);
                } else {
                    assert!(!elem[a as usize]);
                }
            }
        }
    }
}

fn is_prime(p: i64) -> bool {
    if p <= 1 {
        return false;
    }
    if p <= 3 {
        return true;
    }
    (2..).take_while(|&k| k * k <= p).all(|k| p % k != 0)
}

// x^k = a (mod p)
// となるようなxが存在するなら一つ返す
// p: 素数
pub fn kth_root(a: i64, k: i64, p: i64) -> Option<i64> {
    if a < 0 || a >= p {
        return None;
    }
    if k == 0 {
        return if a == 1 { Some(1) } else { None };
    }
    assert!(p >= 2 && k > 0);
    if a <= 1 || k == 1 {
        return Some(a);
    }
    let (x, _, g) = ext_gcd(k, p - 1);
    if pow_mod(a, (p - 1) / g, p) != 1 {
        return None;
    }
    let x = x.rem_euclid((p - 1) / g);
    let a = pow_mod(a, x, p);
    let k = g;
    let f = factor(ext_gcd(k, (p - 1) / k).2);
    let mut ans = {
        let mut q = p - 1;
        loop {
            let g = ext_gcd(q, k).2;
            if g == 1 {
                let (x, _, _) = ext_gcd(k, q);
                q = x.rem_euclid(q);
                break;
            }
            q /= g;
        }
        pow_mod(a, q, p)
    };
    for &f in f.iter() {
        let mut q = p - 1;
        let mut r = 0;
        while q % f == 0 {
            q /= f;
            r += 1;
        }
        let mut c = 0;
        let mut k = k;
        while k % f == 0 {
            k /= f;
            c += 1;
        }
        let mut w = (0..)
            .map(|_| (rand() % (p - 1) as usize) as i64 + 1)
            .find(|i| pow_mod(*i, (p - 1) / f, p) != 1)
            .unwrap();
        w = pow_mod(w, q, p);
        let qq = mul(q, k, p - 1);
        for i in c..r {
            let a = pow_mod(a, q * f.pow(r - i - 1), p);
            let b = pow_mod(ans, qq * f.pow(r - i + c - 1), p);
            let r = pow_mod(w, qq * f.pow(r - i + c - 1), p);
            let k = discreate_log(r, mul(a, pow_mod(b, p - 2, p), p), f, p);
            ans = mul(ans, pow_mod(w, k, p), p);
            w = pow_mod(w, f, p);
        }
    }
    Some(ans)
}

// r^k = b (mod p)
// となるようなkを返す
// r^l = 1 であること、解が必ず存在することを仮定
pub fn discreate_log(r: i64, mut b: i64, l: i64, p: i64) -> i64 {
    let sq = (l as f64).sqrt().ceil() as i64;
    let mut memo = std::collections::HashMap::with_capacity(sq as usize);
    let mut q = 1;
    let ir = pow_mod(r, p - 2, p);
    for i in 0..sq {
        if b == 1 {
            return i;
        }
        memo.insert(b, i);
        b = mul(b, ir, p);
        q = mul(q, r, p);
    }
    let mut k = q;
    for i in 1.. {
        if let Some(&u) = memo.get(&k) {
            return i * sq + u;
        }
        k = mul(k, q, p);
    }
    unreachable!()
}

pub const fn ext_gcd(a: i64, b: i64) -> (i64, i64, i64) {
    if b == 0 {
        (a.signum(), 0, a.abs())
    } else {
        let q = a.div_euclid(b);
        let r = a.rem_euclid(b);
        let (x, y, g) = ext_gcd(b, r);
        (y, x - q * y, g)
    }
}

pub fn factor(mut n: i64) -> Vec<i64> {
    let mut f = vec![];
    let mut p = 2;
    while p * p <= n {
        if n % p == 0 {
            f.push(p);
        }
        while n % p == 0 {
            n /= p;
        }
        p += 1;
    }
    if n > 1 {
        f.push(n);
    }
    f
}

pub const fn mul(a: i64, b: i64, m: i64) -> i64 {
    (a as i128 * b as i128 % m as i128) as i64
}

pub const fn pow_mod(mut r: i64, mut n: i64, p: i64) -> i64 {
    r %= p;
    let mut t = 1 % p;
    while n > 0 {
        if n & 1 == 1 {
            t = mul(t, r, p);
        }
        r = mul(r, r, p);
        n >>= 1;
    }
    t
}

// ---------- begin input macro ----------
// reference: https://qiita.com/tanakh/items/0ba42c7ca36cd29d0ac8
#[macro_export]
macro_rules! input {
    (source = $s:expr, $($r:tt)*) => {
        let mut iter = $s.split_whitespace();
        input_inner!{iter, $($r)*}
    };
    ($($r:tt)*) => {
        let s = {
            use std::io::Read;
            let mut s = String::new();
            std::io::stdin().read_to_string(&mut s).unwrap();
            s
        };
        let mut iter = s.split_whitespace();
        input_inner!{iter, $($r)*}
    };
}

#[macro_export]
macro_rules! input_inner {
    ($iter:expr) => {};
    ($iter:expr, ) => {};
    ($iter:expr, $var:ident : $t:tt $($r:tt)*) => {
        let $var = read_value!($iter, $t);
        input_inner!{$iter $($r)*}
    };
}

#[macro_export]
macro_rules! read_value {
    ($iter:expr, ( $($t:tt),* )) => {
        ( $(read_value!($iter, $t)),* )
    };
    ($iter:expr, [ $t:tt ; $len:expr ]) => {
        (0..$len).map(|_| read_value!($iter, $t)).collect::<Vec<_>>()
    };
    ($iter:expr, chars) => {
        read_value!($iter, String).chars().collect::<Vec<char>>()
    };
    ($iter:expr, bytes) => {
        read_value!($iter, String).bytes().collect::<Vec<u8>>()
    };
    ($iter:expr, usize1) => {
        read_value!($iter, usize) - 1
    };
    ($iter:expr, $t:ty) => {
        $iter.next().unwrap().parse::<$t>().expect("Parse error")
    };
}
// ---------- end input macro ----------
fn rand_memory() -> usize {
    Box::into_raw(Box::new("I hope this is a random number")) as usize
}

fn rand() -> usize {
    static mut X: usize = 0;
    unsafe {
        if X == 0 {
            X = rand_memory();
        }
        X ^= X << 13;
        X ^= X >> 17;
        X ^= X << 5;
        X
    }
}

fn shuffle<T>(a: &mut [T]) {
    for i in 1..a.len() {
        let p = rand() % (i + 1);
        a.swap(i, p);
    }
}
