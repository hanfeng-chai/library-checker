fn main() {
    input! {
        n: usize,
        _m: usize,
        a: [usize; n],
        b: [usize; n],
    }
    let mut a = a;
    let mut b = b;
    for i in 1..n {
        a[i] = a[i].max(a[i - 1]);
    }
    for i in (1..n).rev() {
        b[i - 1] = b[i - 1].min(b[i]);
    }
    if a.iter().zip(b.iter()).any(|p| *p.0 >= *p.1) {
        println!("0");
        return;
    }
    let len = n + b[n - 1];
    let mut l = vec![0; len];
    let mut r = vec![std::usize::MAX; len];
    l[len - 1] = b[n - 1] - 1;
    for (i, (&a, &b)) in a.iter().zip(b.iter()).enumerate() {
        l[i + a] = a;
        r[i + b] = b;
    }
    let ans = calc(&l, &r, &[M::one()], &[M::one(), M::one()])
        .iter()
        .fold(M::zero(), |s, a| s + *a);
    println!("{:?}", ans);
}

type M = ModInt<998244353>;

// F_0 := f として
// [a_0, b_0) 次のみ残す
// F_1 := F_0 * g
// [a_1, b_1) 次のみ残す
// F_2 := F_0 * g
// [a_2, b_2) 次のみ残す
// ...
// という手順を繰り返してできる多項式を返す
fn calc(a: &[usize], b: &[usize], f: &[M], g: &[M]) -> Vec<M> {
    if g.is_empty() {
        return vec![];
    }
    let mut a = Vec::from(a);
    let mut b = Vec::from(b);
    let n = a.len();
    assert!(b.len() == n);
    for i in 1..n {
        a[i] = a[i - 1].max(a[i]);
        b[i] = b[i].min(b[i - 1].saturating_add(g.len() - 1));
    }
    for i in (1..n).rev() {
        a[i - 1] = a[i - 1].max(a[i].saturating_sub(g.len() - 1));
        b[i - 1] = b[i - 1].min(b[i]);
    }
    if a.iter().zip(b.iter()).any(|p| *p.0 >= *p.1) {
        return vec![];
    }
    type T = (usize, Vec<M>);
    let add = |a: &T, b: &T| -> T {
        if a.1.is_empty() {
            return b.clone();
        }
        if b.1.is_empty() {
            return a.clone();
        }
        let geta = a.0.min(b.0);
        let tail = (a.0 + a.1.len()).max(b.0 + b.1.len());
        let mut res = vec![M::zero(); tail - geta];
        res[(a.0 - geta)..].add_assign(&a.1);
        res[(b.0 - geta)..].add_assign(&b.1);
        (geta, res)
    };
    let mul = |a: &T, b: &T| -> T { (a.0 + b.0, a.1.convolution(&b.1)) };
    let truncate = |a: T, l: usize, r: usize| -> T {
        let (mut geta, mut po) = a;
        if geta < l {
            po.drain(..(l - geta));
            geta = l;
        }
        if geta < r {
            po.truncate(r - geta);
        } else {
            po.clear();
        }
        (geta, po)
    };
    let k = a.len().next_power_of_two().trailing_zeros() as usize;
    let mut pow = vec![(0, Vec::from(g))];
    for _ in 1..k {
        let po = pow.last().unwrap();
        let p = mul(po, po);
        pow.push(p);
    }
    let step = recurse(|rec, (poly, s, k): (T, usize, usize)| -> T {
        if poly.1.is_empty() {
            return poly;
        }
        if k == 0 {
            let res = mul(&poly, &pow[0]);
            return truncate(res, a[s + 1], b[s + 1]);
        }
        let len = 1 << k;
        let t = s + len;
        let (l, r) = (a[t], b[t]);
        let l = l.max(poly.0);
        let r = r
            .saturating_sub((g.len() - 1) << k)
            .min(poly.0 + poly.1.len());
        if l < r {
            let mut res = (0, vec![]);
            if l > 0 {
                let f = truncate(poly.clone(), 0, l);
                let f = rec((f, s, k - 1));
                res = rec((f, s + (1 << (k - 1)), k - 1));
            }
            let f = truncate(poly.clone(), l, r);
            let g = mul(&f, &pow[k]);
            res = add(&res, &g);
            if r < poly.0 + poly.1.len() {
                let f = truncate(poly, r, std::usize::MAX);
                let f = rec((f, s, k - 1));
                res = add(&res, &rec((f, s + (1 << (k - 1)), k - 1)));
            }
            res
        } else {
            rec((rec((poly, s, k - 1)), s + (1 << (k - 1)), k - 1))
        }
    });
    let mut pos = 0;
    let mut state = (0, Vec::from(f));
    state = truncate(state, a[0], b[0]);
    for i in (0..k).rev() {
        if pos + (1 << i) < a.len() {
            state = step((state, pos, i));
            pos += 1 << i;
        }
    }
    let geta = state.0;
    let mut res = state.1.clone();
    res.splice(0..0, (0..geta).map(|_| M::zero()));
    res
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
// ---------- begin trait ----------

use std::ops::*;

pub trait Zero: Sized + Add<Self, Output = Self> {
    fn zero() -> Self;
    fn is_zero(&self) -> bool;
}

pub trait One: Sized + Mul<Self, Output = Self> {
    fn one() -> Self;
    fn is_one(&self) -> bool;
}

pub trait Group: Zero + Sub<Output = Self> + Neg<Output = Self> {}
pub trait SemiRing: Zero + One {}
pub trait Ring: SemiRing + Group {}
pub trait Field: Ring + Div<Output = Self> {}

impl<T> Group for T where T: Zero + Sub<Output = Self> + Neg<Output = Self> {}
impl<T> SemiRing for T where T: Zero + One {}
impl<T> Ring for T where T: SemiRing + Group {}
impl<T> Field for T where T: Ring + Div<Output = Self> {}

pub fn zero<T: Zero>() -> T {
    T::zero()
}

pub fn one<T: One>() -> T {
    T::one()
}

pub fn pow<T: One + Clone>(mut r: T, mut n: usize) -> T {
    let mut t = one();
    while n > 0 {
        if n & 1 == 1 {
            t = t * r.clone();
        }
        r = r.clone() * r;
        n >>= 1;
    }
    t
}

pub fn pow_sum<T: SemiRing + Clone>(r: T, n: usize) -> T {
    if n == 0 {
        T::zero()
    } else if n & 1 == 1 {
        T::one() + r.clone() * pow_sum(r, n - 1)
    } else {
        let a = T::one() + r.clone();
        let b = r.clone() * r;
        a * pow_sum(b, n / 2)
    }
}
// ---------- end trait ----------
// ---------- begin recurse ----------
// reference
// https://twitter.com/noshi91/status/1393952665566994434
// https://twitter.com/shino16_cp/status/1393933468082397190
pub fn recurse<A, R, F>(f: F) -> impl Fn(A) -> R
where
    F: Fn(&dyn Fn(A) -> R, A) -> R,
{
    fn call<A, R, F>(f: &F, a: A) -> R
    where
        F: Fn(&dyn Fn(A) -> R, A) -> R,
    {
        f(&|a| call(f, a), a)
    }
    move |a| call(&f, a)
}
// ---------- end recurse ----------
