fn main() {
    input! {
        m: usize,
        n: usize,
        a: [M; m],
        b: [M; 1 << n],
    }
    use util::*;
    println!(
        "{}",
        polynomial_composite_set_power_series(&a, &b)
            .iter()
            .join(" ")
    );
}

impl Ring for M {}

pub fn polynomial_composite_set_power_series<T>(f: &[T], g: &[T]) -> Vec<T>
where
    T: Ring + Copy,
{
    let size = g.len().next_power_of_two();
    assert!(size > 0 && g.len() == size);
    let n = size.trailing_zeros() as usize;
    let mut f = Vec::from(f);
    let mut dp = vec![vec![T::zero()]; n + 1];
    for dp in dp.iter_mut() {
        dp[0] = f.iter().rfold(T::zero(), |s, f| s * g[0] + *f);
        f = f
            .iter()
            .skip(1)
            .scan(T::zero(), |s, f| {
                *s = *s + T::one();
                Some(*s * *f)
            })
            .collect()
    }
    if n == 0 {
        return dp.pop().unwrap();
    }
    for i in 0..(n - 1) {
        let k = i;
        let lift = |a: &[T]| -> Vec<T> {
            let mut res = vec![T::zero(); (k + 1) << k];
            for (i, (res, a)) in res.chunks_exact_mut(k + 1).zip(a.iter()).enumerate() {
                res[i.count_ones() as usize] = *a;
            }
            res
        };
        let zeta = |a: &mut [T]| {
            for i in (1..(1 << k)).step_by(2) {
                let r = (k + 1) * (i + 1);
                let a = &mut a[..r];
                let up = (i + 1).trailing_zeros();
                for j in 0..up {
                    let w = (k + 1) << j;
                    let a = &mut a[(r - 2 * w)..];
                    let (a, b) = a.split_at_mut(w);
                    for (b, a) in b.iter_mut().zip(a.iter()) {
                        *b = *b + *a;
                    }
                }
            }
        };
        let izeta = |a: &mut [T]| {
            for i in (1..(1 << k)).step_by(2) {
                let r = (k + 1) * (i + 1);
                let a = &mut a[..r];
                let up = (i + 1).trailing_zeros();
                for j in 0..up {
                    let w = (k + 1) << j;
                    let a = &mut a[(r - 2 * w)..];
                    let (a, b) = a.split_at_mut(w);
                    for (b, a) in b.iter_mut().zip(a.iter()) {
                        *b = *b - *a;
                    }
                }
            }
        };
        let g = &g[(1 << i)..(2 << i)];
        let mut gz = lift(g);
        zeta(&mut gz);
        let gz = gz;
        for j in 1..dp.len() {
            let mut h = lift(&dp[j]);
            zeta(&mut h);
            for (i, (a, b)) in h
                .chunks_exact_mut(k + 1)
                .zip(gz.chunks_exact(k + 1))
                .enumerate()
            {
                let mut buf = [T::zero(); 21];
                let buf = &mut buf[..=k];
                let cnt = i.count_ones() as usize;
                let x = &mut a[..=cnt];
                let y = &b[..=cnt];
                for (i, x) in x.iter().enumerate() {
                    let g = cnt - i;
                    for (buf, y) in buf[(i + g)..].iter_mut().zip(y[g..].iter()) {
                        *buf = *buf + *x * *y;
                    }
                }
                a.copy_from_slice(buf);
            }
            izeta(&mut h);
            dp[j - 1].extend(h.chunks_exact(k + 1).enumerate().map(|(x, h)| h[x.count_ones() as usize]));
        }
        dp.pop();
    }
    let a = subset_convolution(&g[(1 << (n - 1))..], &dp[1]);
    dp[0].iter().chain(a.iter()).cloned().collect()
}

pub fn subset_convolution<T>(a: &[T], b: &[T]) -> Vec<T>
where
    T: Ring + Copy,
{
    let size = a.len().next_power_of_two();
    assert!(size > 0 && a.len() == size && b.len() == size);
    let n = size.trailing_zeros() as usize;
    let mut x = vec![T::zero(); (n + 1) << n];
    let mut y = vec![T::zero(); (n + 1) << n];
    for (x, a) in [(&mut x, a), (&mut y, b)].iter_mut() {
        for (i, (x, a)) in x.chunks_exact_mut(n + 1).zip(a.iter()).enumerate() {
            x[i.count_ones() as usize] = *a;
        }
    }
    #[target_feature(enable = "avx2")]
    unsafe fn rec<T>(x: &mut [T], y: &mut [T], len: usize, cnt: usize)
    where
        T: Ring + Copy,
    {
        if x.len() == len {
            let mut buf = [T::zero(); 21];
            let buf = &mut buf[..len];
            let a = &x[..=cnt];
            let b = &y[..=cnt];
            for (i, x) in a.iter().enumerate() {
                let g = cnt - i;
                for (buf, y) in buf[(i + g)..].iter_mut().zip(b[g..].iter()) {
                    *buf = *buf + *x * *y;
                }
            }
            x.copy_from_slice(buf);
            return;
        }
        let m = x.len() / 2;
        let (a, b) = x.split_at_mut(m);
        let (c, d) = y.split_at_mut(m);
        let ba = b.iter_mut().zip(a.iter());
        let dc = d.iter_mut().zip(c.iter());
        for ((b, a), (d, c)) in ba.zip(dc) {
            *b = *b + *a;
            *d = *d + *c;
        }
        rec(a, c, len, cnt);
        rec(b, d, len, cnt + 1);
        for (b, a) in b.iter_mut().zip(a.iter()) {
            *b = *b - *a;
        }
    }
    unsafe {
        rec(&mut x, &mut y, n + 1, 0);
    }
    x.chunks_exact(n + 1)
        .enumerate()
        .map(|(i, x)| x[i.count_ones() as usize])
        .collect()
}

pub trait Ring: Zero + One + Sub<Output = Self> {}

mod util {
    pub trait Join {
        fn join(self, sep: &str) -> String;
    }

    impl<T, I> Join for I
    where
        I: Iterator<Item = T>,
        T: std::fmt::Display,
    {
        fn join(self, sep: &str) -> String {
            let mut s = String::new();
            use std::fmt::*;
            for (i, v) in self.enumerate() {
                if i > 0 {
                    write!(&mut s, "{}", sep).ok();
                }
                write!(&mut s, "{}", v).ok();
            }
            s
        }
    }
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

// ms を返す
fn measure<F>(f: F) -> u128
where
    F: FnOnce(),
{
    use std::time::*;
    let now = Instant::now();
    f();
    now.elapsed().as_millis()
}
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

pub trait Zero: Sized + Add<Self, Output = Self> {
    fn zero() -> Self;
    fn is_zero(&self) -> bool;
}

pub trait One: Sized + Mul<Self, Output = Self> {
    fn one() -> Self;
}

impl<T: Modulo> Zero for ModInt<T> {
    fn zero() -> Self {
        Self::zero()
    }
    fn is_zero(&self) -> bool {
        self.is_zero()
    }
}

impl<T: Modulo> One for ModInt<T> {
    fn one() -> Self {
        Self::one()
    }
}

const PRIME: u32 = 998_244_353;

type S = ConstantModulo<PRIME>;
type M = ModInt<S>;

pub const fn pow_mod(mut r: u32, mut n: u32, m: u32) -> u32 {
    let mut t = 1;
    while n > 0 {
        if n & 1 == 1 {
            t = (t as u64 * r as u64 % m as u64) as u32;
        }
        r = (r as u64 * r as u64 % m as u64) as u32;
        n >>= 1;
    }
    t
}

pub const fn primitive_root(p: u32) -> u32 {
    let mut m = p - 1;
    let mut f = [1; 30];
    let mut k = 0;
    let mut d = 2;
    while d * d <= m {
        if m % d == 0 {
            f[k] = d;
            k += 1;
        }
        while m % d == 0 {
            m /= d;
        }
        d += 1;
    }
    if m > 1 {
        f[k] = m;
        k += 1;
    }
    let mut g = 1;
    while g < p {
        let mut ok = true;
        let mut i = 0;
        while i < k {
            ok &= pow_mod(g, (p - 1) / f[i], p) > 1;
            i += 1;
        }
        if ok {
            break;
        }
        g += 1;
    }
    g
}

pub const fn is_prime(n: u32) -> bool {
    if n <= 1 {
        return false;
    }
    let mut d = 2;
    while d * d <= n {
        if n % d == 0 {
            return false;
        }
        d += 1;
    }
    true
}

use std::marker::*;
use std::ops::*;

pub trait Modulo {
    fn modulo() -> u32;
    fn build(v: u32) -> u32;
    fn reduce(v: u64) -> u32;
}

pub struct ConstantModulo<const M: u32>;

impl<const M: u32> ConstantModulo<{ M }> {
    const ORDER: usize = (M - 1).trailing_zeros() as usize;
    const PRIMITIVE_ROOT: u32 = primitive_root(M);
    const ZETA: u32 = pow_mod(Self::PRIMITIVE_ROOT, (M - 1) >> Self::ORDER, M);
    const REM: u32 = {
        let mut t = 1u32;
        let mut s = !M + 1;
        let mut n = !0u32 >> 2;
        while n > 0 {
            if n & 1 == 1 {
                t = t.wrapping_mul(s);
            }
            s = s.wrapping_mul(s);
            n >>= 1;
        }
        t
    };
    const INI: u64 = ((1u128 << 64) % M as u128) as u64;
    const IS_PRIME: () = assert!(is_prime(M));
}

impl<const M: u32> Modulo for ConstantModulo<{ M }> {
    fn modulo() -> u32 {
        M
    }
    fn build(v: u32) -> u32 {
        Self::reduce(v as u64 * Self::INI)
    }
    fn reduce(x: u64) -> u32 {
        debug_assert!(x < (Self::modulo() as u64) << 32);
        let b = (x as u32 * Self::REM) as u64;
        let t = x + b * M as u64;
        let mut c = (t >> 32) as u32;
        if c >= M {
            c -= M;
        }
        c as u32
    }
}

pub trait NTTFriendly {
    fn order() -> usize;
    fn zeta() -> u32;
}

impl<const M: u32> NTTFriendly for ConstantModulo<{ M }> {
    fn order() -> usize {
        Self::ORDER
    }
    fn zeta() -> u32 {
        Self::ZETA
    }
}

pub struct ModInt<T>(u32, PhantomData<fn() -> T>);

impl<T> Clone for ModInt<T> {
    fn clone(&self) -> Self {
        Self::build(self.0)
    }
}

impl<T> Copy for ModInt<T> {}

impl<T: Modulo> Add for ModInt<T> {
    type Output = Self;
    fn add(self, rhs: Self) -> Self::Output {
        let mut v = self.0 + rhs.0;
        if v >= T::modulo() {
            v -= T::modulo();
        }
        Self::build(v)
    }
}

impl<T: Modulo> Sub for ModInt<T> {
    type Output = Self;
    fn sub(self, rhs: Self) -> Self::Output {
        let mut v = self.0 - rhs.0;
        if self.0 < rhs.0 {
            v += T::modulo();
        }
        Self::build(v)
    }
}

impl<T: Modulo> Mul for ModInt<T> {
    type Output = Self;
    fn mul(self, rhs: Self) -> Self::Output {
        Self::build(T::reduce(self.0 as u64 * rhs.0 as u64))
    }
}

impl<T: Modulo> AddAssign for ModInt<T> {
    fn add_assign(&mut self, rhs: Self) {
        *self = *self + rhs;
    }
}

impl<T: Modulo> SubAssign for ModInt<T> {
    fn sub_assign(&mut self, rhs: Self) {
        *self = *self - rhs;
    }
}

impl<T: Modulo> MulAssign for ModInt<T> {
    fn mul_assign(&mut self, rhs: Self) {
        *self = *self * rhs;
    }
}

impl<T: Modulo> Neg for ModInt<T> {
    type Output = Self;
    fn neg(self) -> Self::Output {
        if self.is_zero() {
            Self::zero()
        } else {
            Self::new_unchecked(T::modulo() - self.0)
        }
    }
}

impl<T: Modulo> std::fmt::Display for ModInt<T> {
    fn fmt<'a>(&self, f: &mut std::fmt::Formatter<'a>) -> std::fmt::Result {
        write!(f, "{}", self.get())
    }
}

impl<T: Modulo> std::fmt::Debug for ModInt<T> {
    fn fmt<'a>(&self, f: &mut std::fmt::Formatter<'a>) -> std::fmt::Result {
        write!(f, "{}", self.get())
    }
}

impl<T: Modulo> std::str::FromStr for ModInt<T> {
    type Err = std::num::ParseIntError;
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let val = s.parse::<u32>()?;
        Ok(ModInt::new(val))
    }
}

impl<T: Modulo> From<usize> for ModInt<T> {
    fn from(v: usize) -> Self {
        Self::new_unchecked((v % T::modulo() as usize) as u32)
    }
}

impl<T> ModInt<T> {
    fn build(v: u32) -> Self {
        ModInt(v, PhantomData)
    }
    pub fn is_zero(&self) -> bool {
        self.0 == 0
    }
}

impl<T: Modulo> ModInt<T> {
    pub fn new_unchecked(v: u32) -> Self {
        Self::build(T::build(v))
    }
    pub fn new(v: u32) -> Self {
        Self::new_unchecked(v % T::modulo())
    }
    pub fn zero() -> Self {
        Self::new_unchecked(0)
    }
    pub fn one() -> Self {
        Self::new_unchecked(1)
    }
    pub fn get(&self) -> u32 {
        T::reduce(self.0 as u64)
    }
    pub fn pow(&self, mut n: u64) -> Self {
        let mut t = Self::one();
        let mut r = *self;
        while n > 0 {
            if n & 1 == 1 {
                t *= r;
            }
            r *= r;
            n >>= 1;
        }
        t
    }
    pub fn inv(&self) -> Self {
        assert!(!self.is_zero());
        self.pow((T::modulo() - 2) as u64)
    }
}
