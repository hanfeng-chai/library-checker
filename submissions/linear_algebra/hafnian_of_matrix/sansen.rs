fn main() {
    input! {
        n: usize,
        a: [[M; n]; n],
    }
    let ans = hafnian(n, a);
    println!("{}", ans);
}

fn hafnian(n: usize, a: Vec<Vec<M>>) -> M {
    assert!(n > 0 && n % 2 == 0);
    assert!(a.len() == n);
    assert!(a.iter().all(|a| a.len() == n));
    let n = n / 2;
    let mut cycle = vec![M::zero(); 1 << n];
    for n in (0..n).rev() {
        cycle[1 << n] = a[2 * n][2 * n + 1];
        let mut dp = vec![M::zero(); (2 * n) << n];
        let pos = |last: usize, bit: usize| last + bit * (2 * n);
        for (i, &a) in a[2 * n].iter().enumerate().take(2 * n) {
            dp[pos(i ^ 1, 1 << (i / 2))] += a;
        }
        for bit in 0..(1 << n) {
            let mut sum = M::zero();
            for s in BitOne(bit) {
                for last in 0..2 {
                    let last = 2 * s + last;
                    let w = dp[pos(last, bit)];
                    sum += w * a[last][2 * n + 1];
                    for t in BitOne(!bit & ((1 << n) - 1)) {
                        let bit = bit | (1 << t);
                        for i in 0..2 {
                            dp[pos(2 * t + i ^ 1, bit)] += w * a[last][2 * t + i];
                        }
                    }
                }
            }
            cycle[bit | (1 << n)] += sum;
        }
    }
    let mut dp = Vec::from(&cycle[(1 << (n - 1))..(1 << n)]);
    for n in (0..(n - 1)).rev() {
        let (a, b) = dp.split_at(1 << n);
        let c = subset_convolution(a, &cycle[(1 << n)..(2 << n)]);
        dp = b.iter().zip(c).map(|p| *p.0 + p.1).collect();
    }
    dp[0]
}

struct BitOne(usize);

impl Iterator for BitOne {
    type Item = usize;
    fn next(&mut self) -> Option<Self::Item> {
        if self.0 == 0 {
            None
        } else {
            let x = self.0.trailing_zeros() as usize;
            self.0 ^= 1 << x;
            Some(x)
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
// ---------- begin modint ----------
use std::marker::*;
use std::ops::*;

pub trait Modulo {
    fn modulo() -> u32;
}

pub struct ConstantModulo<const M: u32>;

impl<const M: u32> Modulo for ConstantModulo<{ M }> {
    fn modulo() -> u32 {
        M
    }
}

pub struct ModInt<T>(u32, PhantomData<T>);

impl<T> Clone for ModInt<T> {
    fn clone(&self) -> Self {
        Self::new_unchecked(self.0)
    }
}

impl<T> Copy for ModInt<T> {}

impl<T: Modulo> Add for ModInt<T> {
    type Output = ModInt<T>;
    fn add(self, rhs: Self) -> Self::Output {
        let mut v = self.0 + rhs.0;
        if v >= T::modulo() {
            v -= T::modulo();
        }
        Self::new_unchecked(v)
    }
}

impl<T: Modulo> AddAssign for ModInt<T> {
    fn add_assign(&mut self, rhs: Self) {
        *self = *self + rhs;
    }
}

impl<T: Modulo> Sub for ModInt<T> {
    type Output = ModInt<T>;
    fn sub(self, rhs: Self) -> Self::Output {
        let mut v = self.0 - rhs.0;
        if self.0 < rhs.0 {
            v += T::modulo();
        }
        Self::new_unchecked(v)
    }
}

impl<T: Modulo> SubAssign for ModInt<T> {
    fn sub_assign(&mut self, rhs: Self) {
        *self = *self - rhs;
    }
}

impl<T: Modulo> Mul for ModInt<T> {
    type Output = ModInt<T>;
    fn mul(self, rhs: Self) -> Self::Output {
        let v = self.0 as u64 * rhs.0 as u64 % T::modulo() as u64;
        Self::new_unchecked(v as u32)
    }
}

impl<T: Modulo> MulAssign for ModInt<T> {
    fn mul_assign(&mut self, rhs: Self) {
        *self = *self * rhs;
    }
}

impl<T: Modulo> Neg for ModInt<T> {
    type Output = ModInt<T>;
    fn neg(self) -> Self::Output {
        if self.is_zero() {
            Self::zero()
        } else {
            Self::new_unchecked(T::modulo() - self.0)
        }
    }
}

impl<T> std::fmt::Display for ModInt<T> {
    fn fmt<'a>(&self, f: &mut std::fmt::Formatter<'a>) -> std::fmt::Result {
        write!(f, "{}", self.0)
    }
}

impl<T> std::fmt::Debug for ModInt<T> {
    fn fmt<'a>(&self, f: &mut std::fmt::Formatter<'a>) -> std::fmt::Result {
        write!(f, "{}", self.0)
    }
}

impl<T> Default for ModInt<T> {
    fn default() -> Self {
        Self::zero()
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
    fn from(val: usize) -> ModInt<T> {
        ModInt::new_unchecked((val % T::modulo() as usize) as u32)
    }
}

impl<T: Modulo> From<u64> for ModInt<T> {
    fn from(val: u64) -> ModInt<T> {
        ModInt::new_unchecked((val % T::modulo() as u64) as u32)
    }
}

impl<T: Modulo> From<i64> for ModInt<T> {
    fn from(val: i64) -> ModInt<T> {
        let mut v = ((val % T::modulo() as i64) + T::modulo() as i64) as u32;
        if v >= T::modulo() {
            v -= T::modulo();
        }
        ModInt::new_unchecked(v)
    }
}

impl<T> ModInt<T> {
    pub fn new_unchecked(n: u32) -> Self {
        ModInt(n, PhantomData)
    }
    pub fn zero() -> Self {
        ModInt::new_unchecked(0)
    }
    pub fn one() -> Self {
        ModInt::new_unchecked(1)
    }
    pub fn is_zero(&self) -> bool {
        self.0 == 0
    }
}

impl<T: Modulo> ModInt<T> {
    pub fn new(d: u32) -> Self {
        ModInt::new_unchecked(d % T::modulo())
    }
    pub fn pow(&self, mut n: u64) -> Self {
        let mut t = Self::one();
        let mut s = *self;
        while n > 0 {
            if n & 1 == 1 {
                t *= s;
            }
            s *= s;
            n >>= 1;
        }
        t
    }
    pub fn inv(&self) -> Self {
        assert!(!self.is_zero());
        self.pow(T::modulo() as u64 - 2)
    }
    pub fn fact(n: usize) -> Self {
        (1..=n).fold(Self::one(), |s, a| s * Self::from(a))
    }
    pub fn perm(n: usize, k: usize) -> Self {
        if k > n {
            return Self::zero();
        }
        ((n - k + 1)..=n).fold(Self::one(), |s, a| s * Self::from(a))
    }
    pub fn binom(n: usize, k: usize) -> Self {
        if k > n {
            return Self::zero();
        }
        let k = k.min(n - k);
        let mut nu = Self::one();
        let mut de = Self::one();
        for i in 0..k {
            nu *= Self::from(n - i);
            de *= Self::from(i + 1);
        }
        nu * de.inv()
    }
}
// ---------- end modint ----------
// ---------- begin precalc ----------
pub struct Precalc<T> {
    fact: Vec<ModInt<T>>,
    ifact: Vec<ModInt<T>>,
    inv: Vec<ModInt<T>>,
}

impl<T: Modulo> Precalc<T> {
    pub fn new(n: usize) -> Precalc<T> {
        let mut inv = vec![ModInt::one(); n + 1];
        let mut fact = vec![ModInt::one(); n + 1];
        let mut ifact = vec![ModInt::one(); n + 1];
        for i in 2..=n {
            fact[i] = fact[i - 1] * ModInt::new_unchecked(i as u32);
        }
        ifact[n] = fact[n].inv();
        if n > 0 {
            inv[n] = ifact[n] * fact[n - 1];
        }
        for i in (1..n).rev() {
            ifact[i] = ifact[i + 1] * ModInt::new_unchecked((i + 1) as u32);
            inv[i] = ifact[i] * fact[i - 1];
        }
        Precalc { fact, ifact, inv }
    }
    pub fn inv(&self, n: usize) -> ModInt<T> {
        assert!(n > 0);
        self.inv[n]
    }
    pub fn fact(&self, n: usize) -> ModInt<T> {
        self.fact[n]
    }
    pub fn ifact(&self, n: usize) -> ModInt<T> {
        self.ifact[n]
    }
    pub fn perm(&self, n: usize, k: usize) -> ModInt<T> {
        if k > n {
            return ModInt::zero();
        }
        self.fact[n] * self.ifact[n - k]
    }
    pub fn binom(&self, n: usize, k: usize) -> ModInt<T> {
        if k > n {
            return ModInt::zero();
        }
        self.fact[n] * self.ifact[k] * self.ifact[n - k]
    }
}
// ---------- end precalc ----------

type M = ModInt<ConstantModulo<998_244_353>>;

pub trait SubsetConvValue:
    Copy + std::ops::Add<Output = Self> + std::ops::Mul<Output = Self> + std::ops::Sub<Output = Self>
{
    fn zero() -> Self;
}

pub fn subset_transform<T, F>(a: &mut [T], f: F)
where
    T: SubsetConvValue,
    F: Fn(&mut [T]),
{
    let size = a.len().next_power_of_two();
    assert!(a.len() == size);
    let n = size.trailing_zeros() as usize;
    let mut x = vec![T::zero(); (n + 1) << n];
    for (i, a) in a.iter().enumerate() {
        x[i * (n + 1) + i.count_ones() as usize] = *a;
    }
    fn rec<T, F>(x: &mut [T], n: usize, f: &F)
    where
        T: SubsetConvValue,
        F: Fn(&mut [T]),
    {
        if x.len() == n + 1 {
            f(x);
            return;
        }
        let m = x.len() / 2;
        let (a, b) = x.split_at_mut(m);
        for (b, a) in b.iter_mut().zip(a.iter()) {
            *b = *b + *a;
        }
        rec(a, n, f);
        rec(b, n, f);
        for (b, a) in b.iter_mut().zip(a.iter()) {
            *b = *b - *a;
        }
    }
    rec(&mut x, n, &f);
    for (i, (res, x)) in a.iter_mut().zip(x.chunks_exact(n + 1)).enumerate() {
        *res = x[i.count_ones() as usize];
    }
}

unsafe fn subset_rec<T: SubsetConvValue>(x: &mut [T], y: &mut [T], n: usize) {
    if x.len() == n + 1 {
        for i in (0..=n).rev() {
            x[i] = x[..=i]
                .iter()
                .zip(y[..=i].iter().rev())
                .fold(T::zero(), |s, p| s + *p.0 * *p.1);
        }
        return;
    }
    let m = x.len() / 2;
    let (a, b) = x.split_at_mut(m);
    let (c, d) = y.split_at_mut(m);
    let x = b.iter_mut().zip(a.iter());
    let y = d.iter_mut().zip(c.iter());
    for ((b, a), (d, c)) in x.zip(y) {
        *b = *b + *a;
        *d = *d + *c;
    }
    subset_rec(a, c, n);
    subset_rec(b, d, n);
    for (b, a) in b.iter_mut().zip(a.iter()) {
        *b = *b - *a;
    }
}

pub fn subset_convolution<T: SubsetConvValue>(a: &[T], b: &[T]) -> Vec<T> {
    let size = a.len().next_power_of_two();
    assert!(a.len() == size && b.len() == size);
    let n = size.trailing_zeros() as usize;
    let mut x = vec![T::zero(); (n + 1) << n];
    let mut y = vec![T::zero(); (n + 1) << n];
    for (x, a) in [(&mut x, &a), (&mut y, &b)].iter_mut() {
        for (i, a) in a.iter().enumerate() {
            x[i * (n + 1) + i.count_ones() as usize] = *a;
        }
    }
    unsafe {
        subset_rec(&mut x, &mut y, n);
    }
    let mut res = vec![T::zero(); 1 << n];
    for (i, (res, x)) in res.iter_mut().zip(x.chunks_exact(n + 1)).enumerate() {
        *res = x[i.count_ones() as usize];
    }
    res
}

impl SubsetConvValue for M {
    fn zero() -> Self {
        M::zero()
    }
}
