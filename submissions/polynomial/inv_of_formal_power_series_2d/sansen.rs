type M = ModInt<998244353>;

fn main() {
    input! {
        h: usize,
        w: usize,
        a: [M; h * w],
    }
    let mfps = MultivariateInfo::new(vec![w, h]);
    let b = mfps.inv(&a);
    use std::io::*;
    let out = std::io::stdout();
    let mut out = std::io::BufWriter::new(out.lock());
    for b in b.chunks(w) {
        use util::*;
        writeln!(out, "{}", b.iter().join(" ")).ok();
    }
}

pub struct MultivariateInfo {
    n: Vec<usize>,
    prod: Vec<usize>,
    weight: Vec<usize>,
}

impl MultivariateInfo {
    pub fn new(mut n: Vec<usize>) -> Self {
        assert!(n.iter().all(|n| *n > 0));
        n.retain(|n| *n > 1);
        if n.is_empty() {
            n.push(1);
        }
        let k = n.len();
        let mut prod = n.clone();
        for i in 1..prod.len() {
            prod[i] *= prod[i - 1];
        }
        let mut weight = vec![0; prod[k - 1]];
        for (i, w) in weight.iter_mut().enumerate() {
            for p in prod.iter_mut() {
                *w += i / *p;
            }
            *w %= k;
        }
        Self { n, prod, weight }
    }
    pub fn conv<T>(&self, f: &[T], g: &[T]) -> Vec<T>
    where
        T: Copy + Ring + std::fmt::Debug,
        [T]: NTT,
    {
        if f.is_empty() || g.is_empty() {
            return vec![];
        }
        let prod = &self.prod;
        let weight = &self.weight;
        let k = prod.len();
        assert!(f.len() <= prod[k - 1] && g.len() <= prod[k - 1]);
        let size = (f.len() + g.len() - 1).next_power_of_two();
        let mut x = vec![T::zero(); k * size];
        let mut y = vec![T::zero(); k * size];
        for (x, f) in [(&mut x, f), (&mut y, g)].iter_mut() {
            for ((x, f), w) in x.chunks_exact_mut(k).zip(f.iter()).zip(weight.iter()) {
                x[*w] = *f;
            }
            x.transform(k);
        }
        self.dot(&mut x, &y);
        x.inverse_transform(k);
        x.truncate(prod[k - 1].min(f.len() + g.len() - 1) * k);
        x.chunks_exact(k)
            .zip(weight.iter())
            .map(|p| p.0[*p.1])
            .collect()
    }
    pub fn inv<T>(&self, f: &[T]) -> Vec<T>
    where
        T: Copy + Field,
        [T]: NTT,
    {
        let k = self.prod.len();
        let weight = &self.weight;
        assert!(f.len() <= self.prod[k - 1] && f.len() > 0 && !f[0].is_zero());
        let mut g = Vec::with_capacity(f.len());
        g.push(T::one() / f[0]);
        while g.len() < f.len() {
            let len = g.len();
            let nlen = f.len().min(2 * len);
            let size = nlen.next_power_of_two();
            let mut p = vec![T::zero(); k * size];
            let mut q = vec![T::zero(); k * size];
            for (i, w) in weight.iter().enumerate().take(nlen) {
                p[i * k + *w] = f[i];
                if i < len {
                    q[i * k + *w] = g[i];
                }
            }
            p.transform(k);
            q.transform(k);
            self.dot(&mut p, &q);
            p.inverse_transform(k);
            for (p, w) in p.chunks_exact_mut(k).zip(weight.iter()) {
                let v = p[*w];
                p.fill(T::zero());
                p[*w] = v;
            }
            p[..k * len].fill(T::zero());
            p.transform(k);
            self.dot(&mut p, &q);
            p.inverse_transform(k);
            p.truncate(nlen * k);
            g.extend(weight[len..].iter().zip(p[k * len..].chunks_exact(k)).map(|p| -p.1[*p.0]));
        }
        g
    }
    fn dot<T>(&self, f: &mut [T], g: &[T])
    where
        T: Copy + SemiRing,
    {
        let k = self.n.len();
        let mut buf = vec![T::zero(); 2 * k - 1];
        for (f, g) in f.chunks_exact_mut(k).zip(g.chunks_exact(k)) {
            buf.fill(T::zero());
            for (i, x) in f.iter().enumerate() {
                for (b, y) in buf[i..].iter_mut().zip(g.iter()) {
                    *b = *b + *x * *y;
                }
            }
            let (l, r) = buf.split_at(k);
            f.copy_from_slice(l);
            f.add_assign(r);
        }
    }
}

// [x^n] f g^i (0 <= i < k)
pub fn power_projection<T>(f: Vec<T>, g: Vec<T>, mut n: usize, k: usize) -> Vec<T>
where
    T: Ring + Copy,
    [T]: ArrayConvolution<Item = T>,
{
    let mut nu = Poly2d::new(vec![T::zero(); n + 1], 1, n + 1);
    let mut de = Poly2d::new(vec![T::zero(); 2 * (n + 1)], 2, n + 1);
    for (i, f) in f.iter().enumerate().take(n + 1) {
        nu[0][i] = *f;
    }
    de[0][0] = T::one();
    for (i, g) in g.iter().enumerate().take(n + 1) {
        de[1][i] = -*g;
    }
    while n > 0 {
        let mut p = de.clone();
        for i in 0..p.h {
            for j in (1..p.w).step_by(2) {
                p[i][j] = -p[i][j];
            }
        }
        nu = nu.conv(&p);
        nu = nu.resize(nu.h, nu.w.min(n + 1)).clip((0, 1), (n & 1, 2));
        let mut w = de.w;
        if de.w % 2 == 0 {
            de = de.resize(de.h, de.w - 1);
            p = p.resize(p.h, p.w - 1);
            w -= 1;
        }
        let a = de.a.convolution(&p.a);
        de = Poly2d::new(a, de.h + p.h - 1, w).clip((0, 1), (0, 2));
        n >>= 1;
    }
    let nu = (0..nu.h).map(|i| nu[i][0]).collect::<Vec<_>>();
    let de = (0..de.h).map(|i| de[i][0]).collect::<Vec<_>>();
    let mut ans = nu.convolution(&de.inverse_one(k));
    ans.truncate(k);
    ans
}

// f^(-1)(x) mod x^n
pub fn composition_inverse<T>(mut f: Vec<T>, n: usize) -> Vec<T>
where
    T: Field + From<usize> + Copy,
    [T]: ArrayConvolution<Item = T>,
{
    assert!(f.len() >= 2 && f[0].is_zero() && !f[1].is_zero());
    let f1inv = T::one() / f[1];
    if n <= 2 {
        let mut res = vec![T::zero(), f1inv];
        res.truncate(n);
        return res;
    }
    f.truncate(n);
    let n = n - 1;
    for f in f.iter_mut() {
        *f = *f * f1inv;
    }
    let pow = power_projection(vec![T::one()], f, n, n + 1);
    let mut g = vec![T::zero(); n];
    for i in 1..=n {
        g[n - i] = T::from(n) * pow[i] / T::from(i);
    }
    g = g.log(n);
    let v = -T::one() / T::from(n);
    for g in g.iter_mut() {
        *g = *g * v;
    }
    g = g.exp(n);
    g.insert(0, T::zero());
    let mut pow = T::one();
    for g in g.iter_mut() {
        *g = *g * pow;
        pow = pow * f1inv;
    }
    g
}

// f(g(x)) mod x^n
pub fn composition_of_fps<T>(mut f: Vec<T>, mut g: Vec<T>, n: usize) -> Vec<T>
where
    T: Field + Copy + std::fmt::Debug,
    [T]: ArrayConvolution<Item = T>,
{
    if f.is_empty() || n == 0 {
        return vec![T::zero(); n];
    }
    if g.len() > 0 && !g[0].is_zero() {
        f = f.taylor_shift(std::mem::replace(&mut g[0], T::zero()));
    }
    if g.iter().position(|g| !g.is_zero()).map_or(true, |x| x >= n) {
        let mut res = vec![T::zero(); n];
        res[0] = f[0];
        return res;
    }
    let mut memo = vec![];
    let mut de = Poly2d::new(vec![T::zero(); 2 * n], 2, n);
    de[0][0] = T::one();
    for (i, g) in g.iter().enumerate().take(n) {
        de[1][i] = -*g;
    }
    let mut deg = 1;
    while deg < n {
        let mut s = de.clone();
        for s in s.a.chunks_exact_mut(de.w) {
            for s in s[1..].iter_mut().step_by(2) {
                *s = -*s;
            }
        }
        memo.push(s.clone());
        if 2 * deg >= n {
            break;
        }
        let w = de.w;
        if w % 2 == 0 {
            de = de.resize(de.h, w - 1);
            s = s.resize(de.h, w - 1);
        }
        let a = de.a.convolution(&s.a);
        de = Poly2d::new(a, de.h + de.h - 1, de.w);
        de = de
            .resize(de.h, ((n + deg - 1) / deg).min(de.w))
            .clip((0, 1), (0, 2));
        deg <<= 1;
    }
    f.resize(n, T::zero());
    let h = f.len();
    f.reverse();
    let mut f = Poly2d::new(f, h, 1);
    while let Some(mut m) = memo.pop() {
        let mut nf = Poly2d::new(vec![T::zero(); f.h * (2 * f.w - 1)], f.h, 2 * f.w - 1);
        for i in 0..f.h {
            for j in 0..f.w {
                nf[i][2 * j] = f[i][j];
            }
        }
        if f.h != 2 * deg {
            f = m.conv(&nf);
            let ylow = (nf.h - 1).saturating_sub(deg - 1);
            let xup = f.w.min((n + deg - 1) / deg);
            f = f.resize(nf.h, xup).clip((ylow, 1), (0, 1));
        } else {
            let fw = nf.w;
            let mw = m.w;
            let w = m.w + nf.w - 1;
            nf = nf.resize(nf.h, w);
            nf.a.rotate_right(mw - 1);
            nf.a.extend((1..mw).map(|_| T::zero()));
            m = m.resize(m.h, w);
            m.a.reverse();
            m.a.rotate_left(fw - 1);
            for _ in 1..fw {
                m.a.pop();
            }
            let a = nf.a.middle_product(&m.a);
            let nw = ((n + deg - 1) / deg).min(w);
            let a = a
                .chunks(w)
                .flat_map(|a| a.iter().cloned().take(nw))
                .collect::<Vec<_>>();
            f = Poly2d::new(a, deg, nw);
        }
        deg >>= 1;
    }
    f.a.truncate(n);
    f.a.resize(n, T::zero());
    f.a
}

#[derive(Clone, Debug)]
struct Poly2d<T> {
    a: Vec<T>,
    h: usize,
    w: usize,
}

impl<T> Poly2d<T>
where
    T: Copy + Ring,
    [T]: ArrayConvolution<Item = T>,
{
    pub fn zero() -> Self {
        Self::new(vec![], 0, 0)
    }
    pub fn new(a: Vec<T>, h: usize, w: usize) -> Self {
        let mut res = vec![T::zero(); h * w];
        for (res, a) in res.chunks_exact_mut(w).zip(a.chunks(w)) {
            let l = w.min(a.len());
            res[..l].copy_from_slice(&a[..l]);
        }
        Self { a: res, h, w }
    }
    pub fn resize(&self, h: usize, w: usize) -> Self {
        if h * w == 0 {
            return Self::new(vec![], 0, 0);
        }
        let mut a = vec![T::zero(); h * w];
        let l = self.w.min(w);
        for (a, b) in a.chunks_exact_mut(w).zip(self.a.chunks(self.w)) {
            a[..l].copy_from_slice(&b[..l]);
        }
        Self::new(a, h, w)
    }
    fn conv(&self, rhs: &Self) -> Self {
        if self.is_empty() {
            return rhs.clone();
        }
        if rhs.is_empty() {
            return rhs.clone();
        }
        let nw = self.w + rhs.w - 1;
        let a = self.resize(self.h, nw);
        let b = rhs.resize(rhs.h, nw);
        Self::new(a.a.convolution(&b.a), self.h + rhs.h - 1, nw)
    }
    fn clip(&self, row: (usize, usize), col: (usize, usize)) -> Self {
        if row.0 >= self.h || col.0 >= self.w {
            return Self::zero();
        }
        let h = (self.h - row.0 + row.1 - 1) / row.1;
        let w = (self.w - col.0 + col.1 - 1) / col.1;
        let mut res = Self::new(vec![T::zero(); h * w], h, w);
        for i in 0..h {
            for j in 0..w {
                res[i][j] = self[row.0 + row.1 * i][col.0 + col.1 * j];
            }
        }
        res
    }
    fn is_empty(&self) -> bool {
        self.a.is_empty()
    }
}

impl<T> Index<usize> for Poly2d<T> {
    type Output = [T];
    fn index(&self, x: usize) -> &Self::Output {
        assert!(x < self.h);
        let l = x * self.w;
        let r = l + self.w;
        &self.a[l..r]
    }
}

impl<T> IndexMut<usize> for Poly2d<T> {
    fn index_mut(&mut self, x: usize) -> &mut Self::Output {
        assert!(x < self.h);
        let l = x * self.w;
        let r = l + self.w;
        &mut self.a[l..r]
    }
}

pub fn product_polynomial<T>(p: Vec<Vec<T>>) -> Vec<T>
where
    T: One,
    [T]: ArrayConvolution<Item = T>,
{
    if p.is_empty() {
        return vec![T::one()];
    }
    if p.iter().any(|p| p.is_empty()) {
        return vec![];
    }
    let s = p.iter().map(|p| p.len() - 1).sum::<usize>();
    let mut q = (0..=s).map(|_| vec![]).collect::<Vec<_>>();
    for p in p {
        q[p.len() - 1].push(p);
    }
    let mut memo = vec![];
    for i in 0..q.len() {
        while let Some(f) = q[i].pop() {
            if memo.is_empty() {
                memo = f;
            } else {
                let f = memo.convolution(&f);
                q[f.len() - 1].push(f);
                memo.clear();
            }
        }
    }
    memo
}

pub fn consecutive_terms_of_linear_recurrent_sequence<T>(
    a: Vec<T>,
    c: Vec<T>,
    l: usize,
    r: usize,
) -> Vec<T>
where
    T: Copy + Ring + std::fmt::Debug,
    [T]: ArrayConvolution<Item = T>,
{
    assert!(c.len() > 0 && c.len() == a.len());
    let d = c.len();
    let mut de = c;
    for c in de.iter_mut() {
        *c = -*c;
    }
    de.insert(0, T::one());
    let mut nu = a.convolution(&de);
    nu.truncate(d);
    let s = l.saturating_sub(nu.len() - 1);
    let t = r;
    let mut ans = consecutive_terms_of_inverse(de, s, t);
    ans = ans.convolution(&nu);
    ans.drain(..(l - s));
    ans.truncate(r - l);
    ans
}

// [x^i] 1 / Q (l <= i < r)
// [x^0] Q = 1 を要求
pub fn consecutive_terms_of_inverse<T>(q: Vec<T>, l: usize, r: usize) -> Vec<T>
where
    T: Copy + Ring + std::fmt::Debug,
    [T]: ArrayConvolution<Item = T>,
{
    assert!(q.len() > 0 && q[0].is_one());
    assert!(l <= r);
    if l == r {
        return vec![];
    }
    if l + 1 == r {
        return vec![kth_term_of_linearly_recurrent_sequence(
            &vec![T::one()],
            &q,
            l,
        )];
    }
    let mut de = q;
    de.truncate(r - l + 1);
    let mut f = de.clone();
    for f in f[1..].iter_mut().step_by(2) {
        *f = -*f;
    }
    let de = de
        .convolution(&f)
        .into_iter()
        .step_by(2)
        .collect::<Vec<_>>();
    let s = (l + 2).saturating_sub(f.len()) / 2;
    let t = (r + 1) / 2;
    let x = consecutive_terms_of_inverse(de, s, t);
    let mut y = vec![T::zero(); r - l + f.len() - 1];
    for (i, x) in x.into_iter().enumerate() {
        let k = y.len();
        y[k - (r - (s + i) * 2)] = x;
    }
    f.reverse();
    y.middle_product(&f)
}

// [x^i] P / Q
// [x^0] Q = 1 を要求
pub fn kth_term_of_linearly_recurrent_sequence<T>(p: &[T], q: &[T], mut k: usize) -> T
where
    T: Copy + Ring,
    [T]: ArrayConvolution<Item = T>,
{
    assert!(q.len() > 0 && q[0].is_one());
    let mut nu = Vec::from(p);
    let mut de = Vec::from(q);
    while k > 0 {
        de.truncate(k + 1);
        nu.truncate(k + 1);
        let mut f = de.clone();
        for f in f[1..].iter_mut().step_by(2) {
            *f = -*f;
        }
        nu = nu
            .convolution(&f)
            .into_iter()
            .skip(k & 1)
            .step_by(2)
            .collect();
        de = de.convolution(&f).into_iter().step_by(2).collect();
        k >>= 1;
    }
    nu[0]
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

#[derive(Clone, Copy)]
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
    const VALID: () = assert!(is_prime(M) && M % 2 == 1 && M < (1 << 30));
    const PRIMITIVE_ROOT: u32 = primitive_root(M);
    const ORDER: usize = 1 << (M - 1).trailing_zeros();
    const fn reduce(x: u64) -> u32 {
        let _ = Self::VALID;
        let b = (x as u32 * Self::REM) as u64;
        let t = x + b * M as u64;
        (t >> 32) as u32
    }
    const fn multiply(a: u32, b: u32) -> u32 {
        Self::reduce(a as u64 * b as u64)
    }
    pub const fn new(v: u32) -> Self {
        Self(Self::reduce((v % M) as u64 * Self::INI))
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
        let mut res = Self::reduce(self.0 as u64);
        if res >= M {
            res -= M;
        }
        res
    }
    pub const fn zero() -> Self {
        Self::new(0)
    }
    pub const fn one() -> Self {
        Self::new(1)
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

impl<const M: u32> Add for ModInt<{ M }> {
    type Output = Self;
    fn add(self, rhs: Self) -> Self::Output {
        let mut v = self.0 + rhs.0;
        if v >= 2 * M {
            v -= 2 * M;
        }
        Self(v)
    }
}

impl<const M: u32> Sub for ModInt<{ M }> {
    type Output = Self;
    fn sub(self, rhs: Self) -> Self::Output {
        let mut v = self.0 - rhs.0;
        if self.0 < rhs.0 {
            v += 2 * M;
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
            Self(2 * M - self.0)
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

impl<const M: u32> From<u64> for ModInt<{ M }> {
    fn from(val: u64) -> ModInt<{ M }> {
        ModInt::new((val % M as u64) as u32)
    }
}

impl<const M: u32> From<i64> for ModInt<{ M }> {
    fn from(val: i64) -> ModInt<{ M }> {
        ModInt::new(val.rem_euclid(M as i64) as u32)
    }
}

impl<const M: u32> Into<usize> for ModInt<{ M }> {
    fn into(self) -> usize {
        self.get() as usize
    }
}

impl<const M: u32> PartialEq for ModInt<{ M }> {
    fn eq(&self, rhs: &Self) -> bool {
        self.get() == rhs.get()
    }
}

impl<const M: u32> Eq for ModInt<{ M }> {}
// ---------- end modint ----------
// ---------- begin precalc ----------
pub struct Precalc<T> {
    fact: Vec<T>,
    ifact: Vec<T>,
    inv: Vec<T>,
}

impl<T> Precalc<T>
where
    T: Copy + Field,
{
    pub fn new(size: usize) -> Self {
        let mut fact = vec![T::one(); size + 1];
        let mut ifact = vec![T::one(); size + 1];
        let mut inv = vec![T::one(); size + 1];
        let mut mul = T::one();
        for i in 2..=size {
            mul = mul + T::one();
            fact[i] = fact[i - 1] * mul;
        }
        ifact[size] = T::one() / fact[size];
        for i in (2..=size).rev() {
            inv[i] = ifact[i] * fact[i - 1];
            ifact[i - 1] = ifact[i] * mul;
            mul = mul - T::one();
        }
        Self { fact, ifact, inv }
    }
    pub fn fact(&self, n: usize) -> T {
        self.fact[n]
    }
    pub fn ifact(&self, n: usize) -> T {
        self.ifact[n]
    }
    pub fn inv(&self, n: usize) -> T {
        assert!(0 < n);
        self.inv[n]
    }
    pub fn perm(&self, n: usize, k: usize) -> T {
        if k > n {
            return T::zero();
        }
        self.fact[n] * self.ifact[n - k]
    }
    pub fn binom(&self, n: usize, k: usize) -> T {
        if n < k {
            return T::zero();
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
        self.get() == 0
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

pub trait NTT {
    fn ntt(&mut self);
    fn intt(&mut self);
    fn transform(&mut self, len: usize);
    fn inverse_transform(&mut self, len: usize);
    fn dot_product_ntt(&mut self, rhs: &Self, len: usize);
}

impl<const M: u32> NTT for [ModInt<{ M }>] {
    fn ntt(&mut self) {
        self.transform(1);
    }
    fn intt(&mut self) {
        self.inverse_transform(1);
    }
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
    fn dot_product_ntt(&mut self, rhs: &Self, len: usize) {
        let mut buf = [ModInt::zero(); 20];
        let buf = &mut buf[..(2 * len - 1)];
        let pre = &NTTPrecalcHelper::<{ M }>::A;
        let mut now = ModInt::one();
        for (i, (f, g)) in self
            .chunks_exact_mut(2 * len)
            .zip(rhs.chunks_exact(2 * len))
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
    }
}

// transform でlen=1を指定すればNTTになる
pub trait ArrayConvolution {
    type Item;
    fn convolution(&self, rhs: &[Self::Item]) -> Vec<Self::Item>;
    fn middle_product(&self, a: &[Self::Item]) -> Vec<Self::Item>;
}

pub fn convolution_modulo<const MOD: u32, const A: u32>(
    a: &[ModInt<MOD>],
    b: &[ModInt<MOD>],
) -> Vec<ModInt<A>> {
    let a = a
        .iter()
        .map(|a| ModInt::<A>::new(a.get()))
        .collect::<Vec<_>>();
    let b = b
        .iter()
        .map(|a| ModInt::<A>::new(a.get()))
        .collect::<Vec<_>>();
    a.convolution(&b)
}

pub fn middle_product_modulo<const MOD: u32, const A: u32>(
    a: &[ModInt<MOD>],
    b: &[ModInt<MOD>],
) -> Vec<ModInt<A>> {
    let a = a
        .iter()
        .map(|a| ModInt::<A>::new(a.get()))
        .collect::<Vec<_>>();
    let b = b
        .iter()
        .map(|a| ModInt::<A>::new(a.get()))
        .collect::<Vec<_>>();
    a.middle_product(&b)
}

impl<const M: u32> ArrayConvolution for [ModInt<{ M }>] {
    type Item = ModInt<{ M }>;
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
        if ModInt::<{ M }>::ORDER < k {
            const A: u32 = 167772161;
            const B: u32 = 469762049;
            const C: u32 = 754974721;
            assert!(ModInt::<A>::ORDER >= k);
            assert!(ModInt::<B>::ORDER >= k);
            assert!(ModInt::<C>::ORDER >= k);
            const P: u32 = pow_mod(A, B - 2, B);
            const Q: u32 = pow_mod(A, C - 2, C);
            const R: u32 = pow_mod(B, C - 2, C);
            const QR: u32 = (Q as u64 * R as u64 % C as u64) as u32;
            const W1: u32 = A;
            let w2: u32 = (A as u64 * B as u64 % M as u64) as u32;
            let x: Vec<ModInt<A>> = convolution_modulo(self, rhs);
            let y: Vec<ModInt<B>> = convolution_modulo(self, rhs);
            let z: Vec<ModInt<C>> = convolution_modulo(self, rhs);
            let mut ans = vec![ModInt::<{ M }>::zero(); x.len()];
            for (((ans, x), y), z) in ans.iter_mut().zip(x).zip(y).zip(z) {
                let a = x.get();
                let b = ((y.get() + B - a) as u64 * P as u64 % B as u64) as u32;
                let c = (((z.get() + C - a) as u64 * QR as u64 + (C - b) as u64 * R as u64)
                    % C as u64) as u32;
                *ans = (a as u64 + b as u64 * W1 as u64 + c as u64 * w2 as u64).into();
            }
            return ans;
        }
        let len = (size + (1 << k) - 1) >> k;
        let mut f = vec![ModInt::zero(); len << k];
        let mut g = vec![ModInt::zero(); len << k];
        f[..self.len()].copy_from_slice(self);
        g[..rhs.len()].copy_from_slice(rhs);
        f.transform(len);
        g.transform(len);
        f.dot_product_ntt(&g, len);
        f.inverse_transform(len);
        f.truncate(self.len() + rhs.len() - 1);
        f
    }
    fn middle_product(&self, rhs: &[Self::Item]) -> Vec<Self::Item> {
        assert!(self.len() >= rhs.len());
        if self.len() - rhs.len() <= 32 {
            return self
                .windows(rhs.len())
                .map(|a| {
                    a.iter()
                        .zip(rhs.iter())
                        .fold(ModInt::zero(), |s, p| s + *p.0 * *p.1)
                })
                .collect();
        }
        const PARAM: usize = 10;
        let size = self.len();
        let mut k = 0;
        while (size + (1 << k) - 1) >> k > PARAM {
            k += 1;
        }
        if ModInt::<{ M }>::ORDER < k {
            const A: u32 = 167772161;
            const B: u32 = 469762049;
            const C: u32 = 754974721;
            assert!(ModInt::<A>::ORDER >= k);
            assert!(ModInt::<B>::ORDER >= k);
            assert!(ModInt::<C>::ORDER >= k);
            const P: u32 = pow_mod(A, B - 2, B);
            const Q: u32 = pow_mod(A, C - 2, C);
            const R: u32 = pow_mod(B, C - 2, C);
            const QR: u32 = (Q as u64 * R as u64 % C as u64) as u32;
            const W1: u32 = A;
            let w2: u32 = (A as u64 * B as u64 % M as u64) as u32;
            let x: Vec<ModInt<A>> = middle_product_modulo(self, rhs);
            let y: Vec<ModInt<B>> = middle_product_modulo(self, rhs);
            let z: Vec<ModInt<C>> = middle_product_modulo(self, rhs);
            let mut ans = vec![ModInt::<{ M }>::zero(); x.len()];
            for (((ans, x), y), z) in ans.iter_mut().zip(x).zip(y).zip(z) {
                let a = x.get();
                let b = ((y.get() + B - a) as u64 * P as u64 % B as u64) as u32;
                let c = (((z.get() + C - a) as u64 * QR as u64 + (C - b) as u64 * R as u64)
                    % C as u64) as u32;
                *ans = (a as u64 + b as u64 * W1 as u64 + c as u64 * w2 as u64).into();
            }
            return ans;
        }
        let len = (size + (1 << k) - 1) >> k;
        let mut f = vec![ModInt::zero(); len << k];
        let mut g = vec![ModInt::zero(); len << k];
        f[..self.len()].copy_from_slice(self);
        g[..rhs.len()].copy_from_slice(rhs);
        g[..rhs.len()].reverse();
        f.transform(len);
        g.transform(len);
        f.dot_product_ntt(&g, len);
        f.inverse_transform(len);
        (rhs.len()..=self.len()).map(|i| f[i - 1]).collect()
    }
}

pub trait PolynomialOperation {
    type Item;
    fn eval(&self, x: Self::Item) -> Self::Item;
    fn derivative(&self) -> Vec<Self::Item>;
    fn integral(&self) -> Vec<Self::Item>;
}

impl<T> PolynomialOperation for [T]
where
    T: Field + Copy,
{
    type Item = T;
    fn eval(&self, x: Self::Item) -> Self::Item {
        self.iter().rfold(T::zero(), |s, a| s * x + *a)
    }
    fn derivative(&self) -> Vec<Self::Item> {
        if self.len() <= 1 {
            return vec![];
        }
        self[1..]
            .iter()
            .scan(T::one(), |s, a| {
                let res = *a * *s;
                *s = *s + T::one();
                Some(res)
            })
            .collect()
    }
    fn integral(&self) -> Vec<Self::Item> {
        let mut inv = vec![T::one(); self.len() + 1];
        let mut val = T::zero();
        for i in 1..inv.len() {
            val = val + T::one();
            inv[i] = val * inv[i - 1];
        }
        let mut iprod = T::one() / inv[self.len()];
        for i in (1..inv.len()).rev() {
            inv[i] = iprod * inv[i - 1] * self[i - 1];
            iprod = iprod * val;
            val = val - T::one();
        }
        inv[0] = T::zero();
        inv
    }
}

pub trait RingInverse {
    type Item;
    fn inverse_one(&self, n: usize) -> Vec<Self::Item>;
}

impl<T> RingInverse for [T]
where
    T: Ring + Copy,
    [T]: ArrayConvolution<Item = T>,
{
    type Item = T;
    fn inverse_one(&self, n: usize) -> Vec<Self::Item> {
        if n == 0 {
            return vec![];
        }
        assert!(self.len() > 0 && self[0].is_one());
        let mut g = Vec::with_capacity(n);
        g.push(T::one());
        let mut f = Vec::from(self);
        f.resize(n, T::zero());
        while g.len() < n {
            let size = g.len();
            let up = (2 * size).min(n);
            let mut ig = g.clone();
            ig.push(T::zero());
            ig.reverse();
            let fg = f[..up].middle_product(&ig);
            g.extend(fg.convolution(&g)[..up - size].iter().map(|p| -*p));
        }
        g
    }
}

pub trait FPSOperation {
    type Item;
    fn inverse(&self, n: usize) -> Vec<Self::Item>;
    fn log(&self, n: usize) -> Vec<Self::Item>;
    fn exp(&self, n: usize) -> Vec<Self::Item>;
}

impl<T> FPSOperation for [T]
where
    T: Field + Copy,
    [T]: ArrayConvolution<Item = T>,
{
    type Item = T;
    fn inverse(&self, n: usize) -> Vec<Self::Item> {
        if n == 0 {
            return vec![];
        }
        assert!(self.len() > 0 && !self[0].is_zero());
        let inv = T::one() / self[0];
        let mut f = Vec::from(self);
        for f in f.iter_mut() {
            *f = *f * inv;
        }
        let mut res = f.inverse_one(n);
        for r in res.iter_mut() {
            *r = *r * inv;
        }
        res
    }
    fn log(&self, n: usize) -> Vec<Self::Item> {
        if n == 0 {
            return vec![];
        }
        assert!(self.len() > 0 && self[0].is_one());
        let mut res = self.derivative().convolution(&self.inverse(n));
        res.truncate(n - 1);
        res.integral()
    }
    fn exp(&self, n: usize) -> Vec<Self::Item> {
        if n == 0 {
            return vec![];
        }
        assert!(self.len() > 0 && self[0].is_zero());
        let mut g = Vec::with_capacity(n);
        g.push(T::one());
        while g.len() < n {
            let size = g.len();
            let up = (2 * size).min(n);
            let lg = g.log(up);
            let rhs = self[..up.min(self.len())].sub(&lg);
            let mut h = g.convolution(&rhs);
            h.resize(up, T::zero());
            g.extend(h[size..up].iter().cloned());
        }
        g
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

// ---------- taylor shift ----------
// f(x) とcを受け取って f(x+c) を返す
pub trait TaylorShift {
    type Item;
    fn taylor_shift(&self, c: Self::Item) -> Vec<Self::Item>;
}

impl<T> TaylorShift for [T]
where
    T: Copy + Field,
    [T]: ArrayConvolution<Item = T>,
{
    type Item = T;
    fn taylor_shift(&self, c: Self::Item) -> Vec<Self::Item> {
        if self.is_empty() || c.is_zero() {
            return Vec::from(self);
        }
        let mut fact = vec![T::one(); self.len()];
        let mut val = T::zero();
        for i in 1..fact.len() {
            val = val + T::one();
            fact[i] = fact[i - 1] * val;
        }
        let mut ifact = vec![T::one(); self.len()];
        ifact[self.len() - 1] = T::one() / fact[self.len() - 1];
        for i in (1..fact.len()).rev() {
            ifact[i - 1] = ifact[i] * val;
            val = val - T::one();
        }
        let mut a = Vec::from(self);
        for (a, f) in a.iter_mut().zip(fact.iter()) {
            *a = *a * *f;
        }
        a.reverse();
        let mut pow = T::one();
        for (f, i) in fact.iter_mut().zip(ifact.iter()) {
            *f = *i * pow;
            pow = pow * c;
        }
        a = a.convolution(&fact);
        a.truncate(self.len());
        a.reverse();
        for (a, i) in a.iter_mut().zip(ifact.iter()) {
            *a = *a * *i;
        }
        a
    }
}
// ---------- taylor shift ----------

// f(x) を x = a * r^i (0 <= i < m) で評価した値の列を返す
pub fn multipoint_evaluation_on_geometric_sequence<T>(f: &[T], a: T, r: T, m: usize) -> Vec<T>
where
    T: Copy + Field,
    [T]: ArrayConvolution<Item = T>,
{
    let n = f.len();
    assert!(n > 0 && m > 0);
    if r.is_zero() {
        let mut res = vec![f[0]; m];
        res[0] = f.iter().rev().fold(T::zero(), |s, f| s * a + *f);
        return res;
    }
    let ir = T::one() / r;
    let mut f = Vec::from(f);
    let mut dp = (T::one(), ir);
    for f in f.iter_mut() {
        *f = *f * dp.0;
        dp = (dp.0 * dp.1 * a, dp.1 * ir);
    }
    let mut g = vec![T::zero(); n + m - 1];
    let mut dp = (T::one(), r);
    for g in g.iter_mut() {
        *g = dp.0;
        dp = (dp.0 * dp.1, dp.1 * r);
    }
    f = g.middle_product(&f);
    let mut dp = (T::one(), ir);
    for f in f.iter_mut() {
        *f = *f * dp.0;
        dp = (dp.0 * dp.1, dp.1 * ir);
    }
    f
}

pub fn multipoint_evaluation<T>(c: Vec<T>, p: Vec<T>) -> Vec<T>
where
    T: Copy + Ring,
    [T]: ArrayConvolution<Item = T>,
{
    if p.is_empty() {
        return vec![];
    }
    let n = c.len();
    let m = p.len();
    let mut prod = vec![vec![]; 2 * m];
    for (prod, p) in prod[m..].iter_mut().zip(p.iter()) {
        *prod = vec![T::one(), -*p];
    }
    for i in (1..m).rev() {
        prod[i] = prod[2 * i].convolution(&prod[2 * i + 1]);
    }
    let inv = prod[1].inverse_one(n);
    let mut c = c;
    c.resize(n + m - 1, T::zero());
    let mut dp = vec![vec![]; 2 * m];
    dp[1] = c.middle_product(&inv);
    for i in 1..m {
        dp[2 * i] = dp[i].middle_product(&prod[2 * i + 1]);
        dp[2 * i + 1] = dp[i].middle_product(&prod[2 * i]);
    }
    dp[m..].iter().map(|dp| dp[0]).collect()
}

pub fn shift_of_sampling_points_of_polynomial<T>(f: &[T], c: T, m: usize) -> Vec<T>
where
    T: Copy + Field + From<usize> + Into<usize>,
    [T]: ArrayConvolution<Item = T>,
{
    if f.is_empty() {
        return vec![T::zero(); m];
    }
    if m == 0 {
        return vec![];
    }
    let cv: usize = c.into();
    if cv < f.len() {
        let sub = f.len() - cv;
        if m <= sub {
            return Vec::from(&f[cv..(cv + m)]);
        }
        let mut ans = Vec::from(&f[cv..]);
        ans.extend(shift_of_sampling_points_of_polynomial(
            &f,
            T::from(f.len()),
            m - sub,
        ));
        return ans;
    }
    for i in 0..m {
        if T::from(cv + i).is_zero() {
            let up = i;
            let mut ans = shift_of_sampling_points_of_polynomial(&f, c, up);
            ans.extend(&shift_of_sampling_points_of_polynomial(
                &f,
                T::zero(),
                m - up,
            ));
            return ans;
        }
    }
    let pc = Precalc::<T>::new(f.len());
    let n = f.len();
    let mut f = Vec::from(f);
    for (i, f) in f.iter_mut().enumerate() {
        *f = *f * pc.ifact(i) * pc.ifact(n - 1 - i);
        if (n - 1 - i) % 2 == 1 {
            *f = -*f;
        }
    }
    let mut prod = vec![one(); n + m - 1];
    let mut v = T::one();
    for i in 0..prod.len() {
        v = v * T::from(cv - (n - 1) + i);
        prod[i] = v;
    }
    let mut inv = vec![one(); n + m - 1];
    inv[n + m - 2] = T::one() / prod[n + m - 2];
    for i in (0..(inv.len() - 1)).rev() {
        inv[i] = inv[i + 1] * T::from(cv - (n - 1) + i + 1);
        inv[i + 1] = inv[i + 1] * prod[i];
    }
    f.reverse();
    let mut f = inv.middle_product(&f);
    let mut val = prod[n - 1];
    for (i, f) in f.iter_mut().enumerate() {
        *f = *f * val;
        val = val * inv[i] * T::from(cv + 1 + i);
    }
    f
}
