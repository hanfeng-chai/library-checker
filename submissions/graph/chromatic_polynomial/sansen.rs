fn main() {
    input! {
        n: usize,
        m: usize,
        e: [(usize, usize); m],
    }
    use util::*;
    let f = chromatic_polynomial(n, &e);
    println!("{}", f.iter().join(" "));
}

type M = ModInt<998244353>;

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

fn chromatic_polynomial(n: usize, e: &[(usize, usize)]) -> Vec<M> {
    let mut g = vec![vec![0; n]; n];
    for &(a, b) in e.iter() {
        g[a][b] = 1;
        g[b][a] = 1;
    }
    let mut ban = vec![false; 1 << n];
    for (i, g) in g.iter().enumerate() {
        for (j, g) in g.iter().enumerate() {
            ban[1 << i | 1 << j] |= *g == 1;
        }
    }
    bitwise_transform(&mut ban, |a, b| *b |= *a);
    let mut dp = vec![vec![M::zero(); 1 << (n - 1)]; 2];
    for (dp, ban) in dp[1].iter_mut().zip(ban[(1 << (n - 1))..].iter()) {
        if !*ban {
            *dp = M::one();
        }
    }
    for i in (0..(n - 1)).rev() {
        let c = ban[(1 << i)..(2 << i)]
            .iter()
            .map(|p| if *p { M::zero() } else { M::one() })
            .collect::<Vec<_>>();
        let mut next = vec![vec![M::zero(); 1 << i]; dp.len() + 1];
        for (j, dp) in dp.iter().enumerate().skip(1) {
            let (a, b) = dp.split_at(1 << i);
            let v = subset_convolution(&c, a);
            next[j + 1].add_assign(&v);
            next[j].add_assign(b);
        }
        dp = next;
    }
    let mut ans = vec![];
    for (i, dp) in dp.iter().enumerate().rev() {
        ans = ans.mul(&[-M::from(i), M::one()]).add(&[dp[0]]);
    }
    ans
}

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
    for i in 0..n {
        let g = &g[(1 << i)..(2 << i)];
        for j in 1..dp.len() {
            let h = subset_convolution(&dp[j], g);
            dp[j - 1].extend(h);
        }
        dp.pop();
    }
    dp.pop().unwrap()
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

// ---------- begin bitwise transform ----------
pub fn bitwise_transform<T, F>(a: &mut [T], mut f: F)
where
    F: FnMut(&mut T, &mut T),
{
    let n = a.len().trailing_zeros() as usize;
    assert!(a.len() == 1 << n);
    for i in 0..n {
        for a in a.chunks_exact_mut(2 << i) {
            let (x, y) = a.split_at_mut(1 << i);
            for (x, y) in x.iter_mut().zip(y) {
                f(x, y);
            }
        }
    }
}
// ---------- end bitwise transform ----------
use std::ops::*;

// ---------- begin trait ----------
pub trait Zero: Sized + Add<Self, Output = Self> {
    fn zero() -> Self;
    fn is_zero(&self) -> bool;
}

pub trait One: Sized + Mul<Self, Output = Self> {
    fn one() -> Self;
    fn is_one(&self) -> bool;
}

pub trait SemiRing: Zero + One {}

pub trait Ring: SemiRing + Sub<Output = Self> + Neg<Output = Self> {}

pub trait Field: Ring + Div<Output = Self> {}

impl<T> SemiRing for T where T: Zero + One {}

impl<T> Ring for T where T: SemiRing + Sub<Output = Self> + Neg<Output = Self> {}

impl<T> Field for T where T: Ring + Div<Output = Self> {}
// ---------- end trait ----------
// ---------- begin modint ----------
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

#[derive(Clone, Copy, PartialEq, Eq)]
pub struct ModInt<const M: u32>(u32);

impl<const M: u32> ModInt<{ M }> {
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
    const PRIMITIVE_ROOT: u32 = primitive_root(M);
    const ORDER: usize = 1 << (M - 1).trailing_zeros();
    const fn reduce(x: u64) -> u32 {
        let _ = Self::IS_PRIME;
        let b = (x as u32 * Self::REM) as u64;
        let t = x + b * M as u64;
        let mut c = (t >> 32) as u32;
        if c >= M {
            c -= M;
        }
        c as u32
    }
    const fn multiply(a: u32, b: u32) -> u32 {
        Self::reduce(a as u64 * b as u64)
    }
    pub const fn new(v: u32) -> Self {
        assert!(v < M);
        Self(Self::reduce(v as u64 * Self::INI))
    }
    pub const fn const_mul(&self, rhs: Self) -> Self {
        Self(Self::multiply(self.0, rhs.0))
    }
    pub const fn pow(&self, mut n: u64) -> Self {
        let mut t = Self::new(1);
        let mut r = *self;
        while n > 0 {
            if n & 1 == 1 {
                t = t.const_mul(r);
            }
            r = r.const_mul(r);
            n >>= 1;
        }
        t
    }
    pub const fn inv(&self) -> Self {
        assert!(self.0 != 0);
        self.pow(M as u64 - 2)
    }
    pub const fn get(&self) -> u32 {
        Self::reduce(self.0 as u64)
    }
    pub const fn zero() -> Self {
        Self::new(0)
    }
    pub const fn one() -> Self {
        Self::new(1)
    }
}

impl<const M: u32> Add for ModInt<{ M }> {
    type Output = Self;
    fn add(self, rhs: Self) -> Self::Output {
        let mut v = self.0 + rhs.0;
        if v >= M {
            v -= M;
        }
        Self(v)
    }
}

impl<const M: u32> Sub for ModInt<{ M }> {
    type Output = Self;
    fn sub(self, rhs: Self) -> Self::Output {
        let mut v = self.0 - rhs.0;
        if self.0 < rhs.0 {
            v += M;
        }
        Self(v)
    }
}

impl<const M: u32> Mul for ModInt<{ M }> {
    type Output = Self;
    fn mul(self, rhs: Self) -> Self::Output {
        self.const_mul(rhs)
    }
}

impl<const M: u32> Div for ModInt<{ M }> {
    type Output = Self;
    fn div(self, rhs: Self) -> Self::Output {
        self * rhs.inv()
    }
}

impl<const M: u32> AddAssign for ModInt<{ M }> {
    fn add_assign(&mut self, rhs: Self) {
        *self = *self + rhs;
    }
}

impl<const M: u32> SubAssign for ModInt<{ M }> {
    fn sub_assign(&mut self, rhs: Self) {
        *self = *self - rhs;
    }
}

impl<const M: u32> MulAssign for ModInt<{ M }> {
    fn mul_assign(&mut self, rhs: Self) {
        *self = *self * rhs;
    }
}

impl<const M: u32> DivAssign for ModInt<{ M }> {
    fn div_assign(&mut self, rhs: Self) {
        *self = *self / rhs;
    }
}

impl<const M: u32> Neg for ModInt<{ M }> {
    type Output = Self;
    fn neg(self) -> Self::Output {
        if self.0 == 0 {
            self
        } else {
            Self(M - self.0)
        }
    }
}

impl<const M: u32> std::fmt::Display for ModInt<{ M }> {
    fn fmt<'a>(&self, f: &mut std::fmt::Formatter<'a>) -> std::fmt::Result {
        write!(f, "{}", self.get())
    }
}

impl<const M: u32> std::fmt::Debug for ModInt<{ M }> {
    fn fmt<'a>(&self, f: &mut std::fmt::Formatter<'a>) -> std::fmt::Result {
        write!(f, "{}", self.get())
    }
}

impl<const M: u32> std::str::FromStr for ModInt<{ M }> {
    type Err = std::num::ParseIntError;
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let val = s.parse::<u32>()?;
        Ok(ModInt::new(val))
    }
}

impl<const M: u32> From<usize> for ModInt<{ M }> {
    fn from(val: usize) -> ModInt<{ M }> {
        ModInt::new((val % M as usize) as u32)
    }
}
// ---------- end modint ----------
// ---------- begin precalc ----------
pub struct Precalc<const MOD: u32> {
    fact: Vec<ModInt<MOD>>,
    ifact: Vec<ModInt<MOD>>,
    inv: Vec<ModInt<MOD>>,
}

impl<const MOD: u32> Precalc<MOD> {
    pub fn new(size: usize) -> Self {
        let mut fact = vec![ModInt::one(); size + 1];
        let mut ifact = vec![ModInt::one(); size + 1];
        let mut inv = vec![ModInt::one(); size + 1];
        for i in 2..=size {
            fact[i] = fact[i - 1] * ModInt::from(i);
        }
        ifact[size] = fact[size].inv();
        for i in (2..=size).rev() {
            inv[i] = ifact[i] * fact[i - 1];
            ifact[i - 1] = ifact[i] * ModInt::from(i);
        }
        Self { fact, ifact, inv }
    }
    pub fn fact(&self, n: usize) -> ModInt<MOD> {
        self.fact[n]
    }
    pub fn ifact(&self, n: usize) -> ModInt<MOD> {
        self.ifact[n]
    }
    pub fn inv(&self, n: usize) -> ModInt<MOD> {
        assert!(0 < n);
        self.inv[n]
    }
    pub fn perm(&self, n: usize, k: usize) -> ModInt<MOD> {
        if k > n {
            return ModInt::zero();
        }
        self.fact[n] * self.ifact[n - k]
    }
    pub fn binom(&self, n: usize, k: usize) -> ModInt<MOD> {
        if n < k {
            return ModInt::zero();
        }
        self.fact[n] * self.ifact[k] * self.ifact[n - k]
    }
}
// ---------- end precalc ----------

impl<const M: u32> Zero for ModInt<{ M }> {
    fn zero() -> Self {
        Self::zero()
    }
    fn is_zero(&self) -> bool {
        self.0 == 0
    }
}

impl<const M: u32> One for ModInt<{ M }> {
    fn one() -> Self {
        Self::one()
    }
    fn is_one(&self) -> bool {
        self.get() == 1
    }
}

// ---------- begin array op ----------

struct NTTPrecalc<const M: u32> {
    sum_e: [ModInt<{ M }>; 30],
    sum_ie: [ModInt<{ M }>; 30],
}

impl<const M: u32> NTTPrecalc<{ M }> {
    const fn new() -> Self {
        let cnt2 = (M - 1).trailing_zeros() as usize;
        let root = ModInt::new(ModInt::<{ M }>::PRIMITIVE_ROOT);
        let zeta = root.pow((M - 1) as u64 >> cnt2);
        let mut es = [ModInt::zero(); 30];
        let mut ies = [ModInt::zero(); 30];
        let mut sum_e = [ModInt::zero(); 30];
        let mut sum_ie = [ModInt::zero(); 30];
        let mut e = zeta;
        let mut ie = e.inv();
        let mut i = cnt2;
        while i >= 2 {
            es[i - 2] = e;
            ies[i - 2] = ie;
            e = e.const_mul(e);
            ie = ie.const_mul(ie);
            i -= 1;
        }
        let mut now = ModInt::one();
        let mut inow = ModInt::one();
        let mut i = 0;
        while i < cnt2 - 1 {
            sum_e[i] = es[i].const_mul(now);
            sum_ie[i] = ies[i].const_mul(inow);
            now = ies[i].const_mul(now);
            inow = es[i].const_mul(inow);
            i += 1;
        }
        Self { sum_e, sum_ie }
    }
}

struct NTTPrecalcHelper<const MOD: u32>;
impl<const MOD: u32> NTTPrecalcHelper<MOD> {
    const A: NTTPrecalc<MOD> = NTTPrecalc::new();
}

pub trait ArrayAdd {
    type Item;
    fn add(&self, rhs: &[Self::Item]) -> Vec<Self::Item>;
}

impl<T> ArrayAdd for [T]
where
    T: Zero + Copy,
{
    type Item = T;
    fn add(&self, rhs: &[Self::Item]) -> Vec<Self::Item> {
        let mut c = vec![T::zero(); self.len().max(rhs.len())];
        c[..self.len()].copy_from_slice(self);
        c.add_assign(rhs);
        c
    }
}

pub trait ArrayAddAssign {
    type Item;
    fn add_assign(&mut self, rhs: &[Self::Item]);
}

impl<T> ArrayAddAssign for [T]
where
    T: Add<Output = T> + Copy,
{
    type Item = T;
    fn add_assign(&mut self, rhs: &[Self::Item]) {
        assert!(self.len() >= rhs.len());
        self.iter_mut().zip(rhs).for_each(|(x, a)| *x = *x + *a);
    }
}

impl<T> ArrayAddAssign for Vec<T>
where
    T: Zero + Add<Output = T> + Copy,
{
    type Item = T;
    fn add_assign(&mut self, rhs: &[Self::Item]) {
        if self.len() < rhs.len() {
            self.resize(rhs.len(), T::zero());
        }
        self.as_mut_slice().add_assign(rhs);
    }
}

pub trait ArraySub {
    type Item;
    fn sub(&self, rhs: &[Self::Item]) -> Vec<Self::Item>;
}

impl<T> ArraySub for [T]
where
    T: Zero + Sub<Output = T> + Copy,
{
    type Item = T;
    fn sub(&self, rhs: &[Self::Item]) -> Vec<Self::Item> {
        let mut c = vec![T::zero(); self.len().max(rhs.len())];
        c[..self.len()].copy_from_slice(self);
        c.sub_assign(rhs);
        c
    }
}

pub trait ArraySubAssign {
    type Item;
    fn sub_assign(&mut self, rhs: &[Self::Item]);
}

impl<T> ArraySubAssign for [T]
where
    T: Sub<Output = T> + Copy,
{
    type Item = T;
    fn sub_assign(&mut self, rhs: &[Self::Item]) {
        assert!(self.len() >= rhs.len());
        self.iter_mut().zip(rhs).for_each(|(x, a)| *x = *x - *a);
    }
}

impl<T> ArraySubAssign for Vec<T>
where
    T: Zero + Sub<Output = T> + Copy,
{
    type Item = T;
    fn sub_assign(&mut self, rhs: &[Self::Item]) {
        if self.len() < rhs.len() {
            self.resize(rhs.len(), T::zero());
        }
        self.as_mut_slice().sub_assign(rhs);
    }
}

pub trait ArrayDot {
    type Item;
    fn dot(&self, rhs: &[Self::Item]) -> Vec<Self::Item>;
}

impl<T> ArrayDot for [T]
where
    T: Mul<Output = T> + Copy,
{
    type Item = T;
    fn dot(&self, rhs: &[Self::Item]) -> Vec<Self::Item> {
        assert!(self.len() == rhs.len());
        self.iter().zip(rhs).map(|p| *p.0 * *p.1).collect()
    }
}

pub trait ArrayDotAssign {
    type Item;
    fn dot_assign(&mut self, rhs: &[Self::Item]);
}

impl<T> ArrayDotAssign for [T]
where
    T: MulAssign + Copy,
{
    type Item = T;
    fn dot_assign(&mut self, rhs: &[Self::Item]) {
        assert!(self.len() == rhs.len());
        self.iter_mut().zip(rhs).for_each(|(x, a)| *x *= *a);
    }
}

pub trait ArrayMul {
    type Item;
    fn mul(&self, rhs: &[Self::Item]) -> Vec<Self::Item>;
}

impl<T> ArrayMul for [T]
where
    T: Zero + One + Copy,
{
    type Item = T;
    fn mul(&self, rhs: &[Self::Item]) -> Vec<Self::Item> {
        if self.is_empty() || rhs.is_empty() {
            return vec![];
        }
        let mut res = vec![T::zero(); self.len() + rhs.len() - 1];
        for (i, a) in self.iter().enumerate() {
            for (res, b) in res[i..].iter_mut().zip(rhs.iter()) {
                *res = *res + *a * *b;
            }
        }
        res
    }
}

// transform でlen=1を指定すればNTTになる
pub trait ArrayConvolution {
    type Item;
    fn transform(&mut self, len: usize);
    fn ntt(&mut self);
    fn intt(&mut self);
    fn inverse_transform(&mut self, len: usize);
    fn convolution(&self, rhs: &[Self::Item]) -> Vec<Self::Item>;
}

impl<const M: u32> ArrayConvolution for [ModInt<{ M }>] {
    type Item = ModInt<{ M }>;
    fn transform(&mut self, len: usize) {
        let f = self;
        let n = f.len();
        let k = (n / len).trailing_zeros() as usize;
        assert!(len << k == n);
        assert!(k <= ModInt::<{ M }>::ORDER);
        let pre = &NTTPrecalcHelper::<{ M }>::A;
        for ph in 1..=k {
            let p = len << (k - ph);
            let mut now = ModInt::one();
            for (i, f) in f.chunks_exact_mut(2 * p).enumerate() {
                let (x, y) = f.split_at_mut(p);
                for (x, y) in x.iter_mut().zip(y.iter_mut()) {
                    let l = *x;
                    let r = *y * now;
                    *x = l + r;
                    *y = l - r;
                }
                now *= pre.sum_e[(!i).trailing_zeros() as usize];
            }
        }
    }
    fn ntt(&mut self) {
        self.transform(1);
    }
    fn intt(&mut self) {
        self.inverse_transform(1);
    }
    fn inverse_transform(&mut self, len: usize) {
        let f = self;
        let n = f.len();
        let k = (n / len).trailing_zeros() as usize;
        assert!(len << k == n);
        assert!(k <= ModInt::<{ M }>::ORDER);
        let pre = &NTTPrecalcHelper::<{ M }>::A;
        for ph in (1..=k).rev() {
            let p = len << (k - ph);
            let mut inow = ModInt::one();
            for (i, f) in f.chunks_exact_mut(2 * p).enumerate() {
                let (x, y) = f.split_at_mut(p);
                for (x, y) in x.iter_mut().zip(y.iter_mut()) {
                    let l = *x;
                    let r = *y;
                    *x = l + r;
                    *y = (l - r) * inow;
                }
                inow *= pre.sum_ie[(!i).trailing_zeros() as usize];
            }
        }
        let ik = ModInt::new(2).inv().pow(k as u64);
        for f in f.iter_mut() {
            *f *= ik;
        }
    }
    fn convolution(&self, rhs: &[Self::Item]) -> Vec<Self::Item> {
        if self.len().min(rhs.len()) <= 32 {
            return self.mul(rhs);
        }
        const PARAM: usize = 10;
        let size = self.len() + rhs.len() - 1;
        let mut k = 0;
        while (size + (1 << k) - 1) >> k > PARAM {
            k += 1;
        }
        let len = (size + (1 << k) - 1) >> k;
        let mut f = vec![ModInt::zero(); len << k];
        let mut g = vec![ModInt::zero(); len << k];
        f[..self.len()].copy_from_slice(self);
        g[..rhs.len()].copy_from_slice(rhs);
        f.transform(len);
        g.transform(len);
        let mut buf = [ModInt::zero(); 2 * PARAM - 1];
        let buf = &mut buf[..(2 * len - 1)];
        let pre = &NTTPrecalcHelper::<{ M }>::A;
        let mut now = ModInt::one();
        for (i, (f, g)) in f
            .chunks_exact_mut(2 * len)
            .zip(g.chunks_exact(2 * len))
            .enumerate()
        {
            let mut r = now;
            for (f, g) in f.chunks_exact_mut(len).zip(g.chunks_exact(len)) {
                buf.fill(ModInt::zero());
                for (i, f) in f.iter().enumerate() {
                    for (buf, g) in buf[i..].iter_mut().zip(g.iter()) {
                        *buf = *buf + *f * *g;
                    }
                }
                f.copy_from_slice(&buf[..len]);
                for (f, buf) in f.iter_mut().zip(buf[len..].iter()) {
                    *f = *f + r * *buf;
                }
                r = -r;
            }
            now *= pre.sum_e[(!i).trailing_zeros() as usize];
        }
        f.inverse_transform(len);
        f.truncate(self.len() + rhs.len() - 1);
        f
    }
}
// ---------- end array op ----------
