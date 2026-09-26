use urectanc::{
    algebra::{Group, Monoid},
    fast_io::{Input, Output},
    modint::ModInt998244353,
    potentialized_union_find::PotentializedUnionFind,
};

type Mint = ModInt998244353;

fn main() {
    let mut input = Input::stdin();
    let mut output = Output::stdout();

    let n: usize = input.val();
    let q: usize = input.val();
    let mut uf = PotentializedUnionFind::<Mat>::new(n);

    for _ in 0..q {
        let t: u8 = input.val();
        let u: usize = input.val();
        let v: usize = input.val();
        if t == 0 {
            let x = Mat(
                input.val::<u32>().into(),
                input.val::<u32>().into(),
                input.val::<u32>().into(),
                input.val::<u32>().into(),
            );
            let valid = uf.merge(u, v, x);
            output.writeln(valid as u32);
        } else {
            if let Some(mat) = uf.diff(u, v) {
                output.write(mat.0.val());
                output.write(mat.1.val());
                output.write(mat.2.val());
                output.writeln(mat.3.val());
            } else {
                output.writeln(-1);
            }
        }
    }
}

#[derive(Clone, Debug, PartialEq)]
struct Mat(Mint, Mint, Mint, Mint);

impl Monoid for Mat {
    type Elem = Self;

    fn identity() -> Self::Elem {
        Mat(1.into(), 0.into(), 0.into(), 1.into())
    }

    fn op(lhs: &Self::Elem, rhs: &Self::Elem) -> Self::Elem {
        Mat(
            lhs.0 * rhs.0 + lhs.1 * rhs.2,
            lhs.0 * rhs.1 + lhs.1 * rhs.3,
            lhs.2 * rhs.0 + lhs.3 * rhs.2,
            lhs.2 * rhs.1 + lhs.3 * rhs.3,
        )
    }
}

impl Group for Mat {
    fn inv(elem: &Self::Elem) -> Self::Elem {
        Mat(elem.3, -elem.1, -elem.2, elem.0)
    }
}

pub mod urectanc {
    pub mod potentialized_union_find {
        use crate::urectanc::algebra;
        use algebra::Group;
        pub struct PotentializedUnionFind<G: Group> {
            parent_or_size: Vec<i32>,
            potential: Vec<G::Elem>,
        }
        impl<G> PotentializedUnionFind<G>
        where
            G: Group,
            G::Elem: PartialEq,
        {
            pub fn new(size: usize) -> Self {
                Self {
                    parent_or_size: vec![-1; size],
                    potential: vec![G::identity(); size],
                }
            }
            pub fn leader(&mut self, v: usize) -> (usize, G::Elem) {
                if self.parent_or_size[v] < 0 {
                    return (v, self.potential[v].clone());
                }
                let p = self.parent_or_size[v] as usize;
                let (leader, potential) = self.leader(p);
                self.parent_or_size[v] = leader as i32;
                self.potential[v] = G::op(&potential, &self.potential[v]);
                (leader, self.potential[v].clone())
            }
            pub fn size(&mut self, v: usize) -> usize {
                let leader = self.leader(v).0;
                -self.parent_or_size[leader] as usize
            }
            pub fn same(&mut self, u: usize, v: usize) -> bool {
                self.diff(u, v).is_some()
            }
            pub fn diff(&mut self, v: usize, reference: usize) -> Option<G::Elem> {
                let (v, dv) = self.leader(v);
                let (r, dr) = self.leader(reference);
                (v == r).then_some(G::op(&G::inv(&dr), &dv))
            }
            pub fn merge(&mut self, v: usize, reference: usize, mut diff: G::Elem) -> bool {
                let (mut v, dv) = self.leader(v);
                let (mut r, dr) = self.leader(reference);
                if v == r {
                    return G::op(&dr, &diff) == dv;
                }
                diff = G::op(&dr, &G::op(&diff, &G::inv(&dv)));
                if self.size(v) > self.size(r) {
                    std::mem::swap(&mut v, &mut r);
                    diff = G::inv(&diff);
                }
                self.parent_or_size[r] += std::mem::replace(&mut self.parent_or_size[v], r as i32);
                self.potential[v] = G::op(&self.potential[r], &diff);
                true
            }
        }
    }
    pub mod modint {
        use std::{
            fmt::Debug,
            ops::{Add, AddAssign, Div, DivAssign, Mul, MulAssign, Sub, SubAssign},
        };
        const fn gcd_inv(a: i64, b: i64) -> (i64, i64) {
            let a = a.rem_euclid(b);
            if a == 0 {
                return (b, 0);
            }
            let mut u = (b, 0);
            let mut v = (a, 1);
            while v.0 != 0 {
                let q = u.0.div_euclid(v.0);
                u.0 -= q * v.0;
                u.1 -= q * v.1;
                (u, v) = (v, u);
            }
            if u.1 < 0 {
                u.1 += b.div_euclid(u.0);
            }
            u
        }
        pub trait Modulus: 'static + Clone + Copy + Debug + PartialEq + Eq {
            const MOD: u32;
        }
        macro_rules! define_modulus {
            ($(($name:ident, $modulus:expr)),* $(,)?) => {
                $(#[doc = concat!("A modulus marker for `", stringify!($modulus), "`.")]
                #[derive(Clone, Copy, Debug, PartialEq, Eq)] pub struct $name; impl
                Modulus for $name { const MOD : u32 = $modulus; })*
            };
        }
        define_modulus!((Mod998244353, 998244353), (Mod1000000007, 1000000007));
        #[derive(Clone, Copy, PartialEq, Eq, Hash)]
        pub struct StaticModInt<M> {
            val: u32,
            _phantom: std::marker::PhantomData<fn() -> M>,
        }
        impl<M: Modulus> StaticModInt<M> {
            pub const fn modulus() -> u32 {
                M::MOD
            }
            pub const fn new(val: u32) -> Self {
                Self {
                    val: val.rem_euclid(Self::modulus()),
                    _phantom: std::marker::PhantomData,
                }
            }
            pub const fn raw(val: u32) -> Self {
                Self {
                    val,
                    _phantom: std::marker::PhantomData,
                }
            }
            pub const fn val(&self) -> u32 {
                self.val
            }
            pub const fn pow(self, mut exp: u64) -> Self {
                let modulus = Self::modulus() as u64;
                let mut base = self.val() as u64;
                let mut acc = 1u64;
                while exp > 0 {
                    if exp & 1 == 1 {
                        acc = acc * base % modulus;
                    }
                    base = base * base % modulus;
                    exp >>= 1;
                }
                Self::raw(acc as u32)
            }
            pub const fn inv(self) -> Self {
                let Some(inv) = self.checked_inv() else {
                    panic!("the inverse does not exist")
                };
                inv
            }
            pub const fn checked_inv(self) -> Option<Self> {
                let (gcd, inv) = gcd_inv(self.val() as i64, Self::modulus() as i64);
                if gcd == 1 {
                    Some(Self::raw(inv as u32))
                } else {
                    None
                }
            }
            fn into_rational(&self) -> (i64, i64) {
                let m = Self::modulus() as i64;
                let mut u = (m, 0i64);
                let mut v = (self.val() as i64, 1i64);
                while v.0 * v.0 * 2 > m {
                    let q = u.0.div_euclid(v.0);
                    let w = (u.0 - q * v.0, u.1 - q * v.1);
                    u = std::mem::replace(&mut v, w);
                }
                if v.1 < 0 {
                    (-v.0, -v.1)
                } else {
                    v
                }
            }
        }
        impl<M: Modulus> std::fmt::Display for StaticModInt<M> {
            fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
                write!(f, "{}", self.val)
            }
        }
        impl<M: Modulus> std::fmt::Debug for StaticModInt<M> {
            fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
                let (num, denom) = self.into_rational();
                if denom == 1 {
                    write!(f, "{num}")
                } else {
                    write!(f, "{num}/{denom}")
                }
            }
        }
        impl<M: Modulus> std::str::FromStr for StaticModInt<M> {
            type Err = std::num::ParseIntError;
            fn from_str(s: &str) -> Result<Self, Self::Err> {
                let value = s.parse::<u32>()?;
                Ok(value.into())
            }
        }
        macro_rules! impl_from_integer {
            ($($ty:tt),*) => {
                $(impl < M : Modulus > From <$ty > for StaticModInt < M > { fn from(value
                : $ty) -> StaticModInt < M > { Self::raw((value as $ty)
                .rem_euclid(Self::modulus() as $ty) as u32) } })*
            };
        }
        impl_from_integer!(u32, u64, usize, i32, i64, isize);
        impl<M: Modulus> std::ops::Neg for StaticModInt<M> {
            type Output = Self;
            fn neg(mut self) -> Self::Output {
                if self.val > 0 {
                    self.val = Self::modulus() - self.val;
                }
                self
            }
        }
        impl<M: Modulus, T: Into<StaticModInt<M>>> AddAssign<T> for StaticModInt<M> {
            fn add_assign(&mut self, rhs: T) {
                self.val += rhs.into().val;
                if self.val >= Self::modulus() {
                    self.val -= Self::modulus();
                }
            }
        }
        impl<M: Modulus, T: Into<StaticModInt<M>>> SubAssign<T> for StaticModInt<M> {
            fn sub_assign(&mut self, rhs: T) {
                self.val = self.val.wrapping_sub(rhs.into().val);
                if self.val > Self::modulus() {
                    self.val = self.val.wrapping_add(Self::modulus());
                }
            }
        }
        impl<M: Modulus, T: Into<StaticModInt<M>>> MulAssign<T> for StaticModInt<M> {
            fn mul_assign(&mut self, rhs: T) {
                self.val =
                    ((self.val as u64 * rhs.into().val as u64) % Self::modulus() as u64) as u32;
            }
        }
        impl<M: Modulus, T: Into<StaticModInt<M>>> DivAssign<T> for StaticModInt<M> {
            fn div_assign(&mut self, rhs: T) {
                *self *= rhs.into().inv();
            }
        }
        macro_rules! impl_binnary_operators {
            (
                $({ $op:ident, $op_assign:ident, $fn:ident, $fn_assign:ident }),* $(,)?
            ) => {
                $(impl < M : Modulus, T : Into < StaticModInt < M >>> std::ops::$op < T >
                for StaticModInt < M > { type Output = StaticModInt < M >; fn $fn (mut
                self, rhs : T) -> StaticModInt < M > { self.$fn_assign (rhs.into()); self
                } } impl < M : Modulus > std::ops::$op <& StaticModInt < M >> for
                StaticModInt < M > { type Output = StaticModInt < M >; fn $fn (self, rhs
                : & StaticModInt < M >) -> StaticModInt < M > { self.$fn (* rhs) } } impl
                < M : Modulus, T : Into < StaticModInt < M >>> std::ops::$op < T > for &
                StaticModInt < M > { type Output = StaticModInt < M >; fn $fn (self, rhs
                : T) -> StaticModInt < M > { (* self).$fn (rhs.into()) } } impl < M :
                Modulus > std::ops::$op <& StaticModInt < M >> for & StaticModInt < M > {
                type Output = StaticModInt < M >; fn $fn (self, rhs : & StaticModInt < M
                >) -> StaticModInt < M > { (* self).$fn (* rhs) } } impl < M : Modulus >
                std::ops::$op_assign <& StaticModInt < M >> for StaticModInt < M > { fn
                $fn_assign (& mut self, rhs : & StaticModInt < M >) { * self = self.$fn
                (* rhs); } })*
            };
        }
        impl_binnary_operators!(
            { Add, AddAssign, add, add_assign }, { Sub, SubAssign, sub, sub_assign }, {
            Mul, MulAssign, mul, mul_assign }, { Div, DivAssign, div, div_assign },
        );
        impl<M: Modulus> std::iter::Sum for StaticModInt<M> {
            fn sum<I: Iterator<Item = Self>>(iter: I) -> Self {
                iter.fold(Self::raw(0), Add::add)
            }
        }
        impl<'a, M: Modulus> std::iter::Sum<&'a StaticModInt<M>> for StaticModInt<M> {
            fn sum<I: Iterator<Item = &'a Self>>(iter: I) -> Self {
                iter.fold(Self::raw(0), Add::add)
            }
        }
        impl<M: Modulus> std::iter::Product for StaticModInt<M> {
            fn product<I: Iterator<Item = Self>>(iter: I) -> Self {
                iter.fold(Self::raw(1), Mul::mul)
            }
        }
        impl<'a, M: Modulus> std::iter::Product<&'a StaticModInt<M>> for StaticModInt<M> {
            fn product<I: Iterator<Item = &'a Self>>(iter: I) -> Self {
                iter.fold(Self::raw(1), Mul::mul)
            }
        }
        pub type ModInt998244353 = StaticModInt<Mod998244353>;
        pub type ModInt1000000007 = StaticModInt<Mod1000000007>;
    }
    pub mod fast_io {
        mod input {
            use std::{io::Read, os::fd::FromRawFd};
            mod mman {
                use std::ffi::{c_int, c_void};
                pub const PROT_READ: c_int = 1;
                pub const MAP_PRIVATE: c_int = 2;
                #[link(name = "c")]
                extern "C" {
                    pub fn mmap(
                        addr: *mut c_void,
                        len: usize,
                        prot: c_int,
                        flags: c_int,
                        fd: c_int,
                        offset: isize,
                    ) -> *mut c_void;
                }
            }
            pub struct Input {
                cursor: *const u8,
            }
            impl Input {
                pub fn new(buf: &[u8]) -> Self {
                    Self {
                        cursor: buf.as_ptr(),
                    }
                }
                pub fn stdin() -> Self {
                    use mman::*;
                    let mut stdin = unsafe { std::fs::File::from_raw_fd(0) };
                    let buf = match stdin.metadata() {
                        Ok(metadata) if metadata.is_file() => {
                            let len = metadata.len() as usize;
                            unsafe {
                                mmap(std::ptr::null_mut(), len, PROT_READ, MAP_PRIVATE, 0, 0) as _
                            }
                        }
                        _ => {
                            let mut buf = Vec::new();
                            stdin.read_to_end(&mut buf).unwrap();
                            Box::leak(buf.into_boxed_slice()).as_ptr()
                        }
                    };
                    Self { cursor: buf }
                }
                fn seek(&mut self, offset: usize) {
                    self.cursor = unsafe { self.cursor.add(offset) };
                }
                fn peek<T>(&self) -> T {
                    let ptr = self.cursor as *const T;
                    unsafe { std::ptr::read_unaligned(ptr) }
                }
                fn next<T>(&mut self) -> T {
                    let val = self.peek();
                    self.seek(std::mem::size_of::<T>());
                    val
                }
                fn skip_whitespace(&mut self) {
                    while self.peek::<u8>().is_ascii_whitespace() {
                        self.seek(1);
                    }
                }
                fn parse_neg(&mut self) -> bool {
                    let neg = self.peek::<u8>() == b'-';
                    self.seek(neg as usize);
                    neg
                }
                fn parse_digits(&mut self, mut val: u64) -> u64 {
                    loop {
                        let c = self.next::<u8>();
                        if c.is_ascii_whitespace() {
                            break;
                        }
                        val = val * 10 + (c - b'0') as u64;
                    }
                    val
                }
                fn parse_8digits(&mut self) -> Option<u64> {
                    let mut val = self.peek::<u64>() ^ 0x3030303030303030;
                    if val & 0xf0f0f0f0f0f0f0f0 != 0 {
                        return None;
                    }
                    self.seek(8);
                    val = val.wrapping_mul((10 << 8) + 1) >> 8 & 0x00ff00ff00ff00ff;
                    val = val.wrapping_mul((100 << 16) + 1) >> 16 & 0x0000ffff0000ffff;
                    val = val.wrapping_mul((10000 << 32) + 1) >> 32;
                    Some(val)
                }
                pub fn val<T: Readable>(&mut self) -> T {
                    self.skip_whitespace();
                    T::read(self)
                }
                pub fn vec<T: Readable>(&mut self, len: usize) -> Vec<T> {
                    (0..len).map(|_| self.val()).collect()
                }
                pub fn bytes(&mut self) -> &[u8] {
                    self.skip_whitespace();
                    let start = self.cursor;
                    while !self.peek::<u8>().is_ascii_whitespace() {
                        self.seek(1);
                    }
                    unsafe {
                        let len = self.cursor.offset_from(start) as usize;
                        std::slice::from_raw_parts(start, len)
                    }
                }
            }
            pub trait Readable {
                fn read(input: &mut Input) -> Self;
            }
            impl Readable for u8 {
                fn read(input: &mut Input) -> Self {
                    input.parse_digits(0) as _
                }
            }
            impl Readable for u16 {
                fn read(input: &mut Input) -> Self {
                    input.parse_digits(0) as _
                }
            }
            impl Readable for u32 {
                fn read(input: &mut Input) -> Self {
                    let val = input.parse_8digits().unwrap_or(0);
                    input.parse_digits(val) as _
                }
            }
            impl Readable for u64 {
                fn read(input: &mut Input) -> Self {
                    let val = input.parse_8digits().map_or(0, |x| {
                        input.parse_8digits().map_or(x, |y| x * 100_000_000 + y)
                    });
                    input.parse_digits(val)
                }
            }
            impl Readable for usize {
                fn read(input: &mut Input) -> Self {
                    u64::read(input) as _
                }
            }
            macro_rules! impl_readable_signed {
                ($signed:ty, $unsigned:ty) => {
                    impl Readable for $signed {
                        fn read(input: &mut Input) -> Self {
                            let neg = input.parse_neg();
                            let val = <$unsigned>::read(input) as Self;
                            if neg {
                                -val
                            } else {
                                val
                            }
                        }
                    }
                };
            }
            impl_readable_signed!(i8, u8);
            impl_readable_signed!(i16, u16);
            impl_readable_signed!(i32, u32);
            impl_readable_signed!(i64, u64);
            impl_readable_signed!(isize, usize);
        }
        mod output {
            use std::io::Write;
            const BUF_SIZE: usize = 1 << 18;
            const MIN_WRITE_CAPACITY: usize = 50;
            pub struct Output<W: Write> {
                buf: [u8; BUF_SIZE],
                pos: usize,
                inner: W,
            }
            impl Output<std::io::StdoutLock<'static>> {
                pub fn stdout() -> Self {
                    Self::new(std::io::stdout().lock())
                }
            }
            impl<W: Write> Drop for Output<W> {
                fn drop(&mut self) {
                    self.flush();
                }
            }
            impl<W: Write> Output<W> {
                pub fn new(inner: W) -> Self {
                    Self {
                        buf: [0; BUF_SIZE],
                        pos: 0,
                        inner,
                    }
                }
                #[cold]
                pub fn flush(&mut self) {
                    self.inner
                        .write_all(&self.buf[..self.pos])
                        .expect("flush failed");
                    self.pos = 0;
                }
                pub fn write<T: Writable<W>>(&mut self, val: T) {
                    self.ensure_capacity();
                    unsafe {
                        T::write_unchecked(self, val);
                        self.write_byte_unchecked(b' ');
                    }
                }
                pub fn writeln<T: Writable<W>>(&mut self, val: T) {
                    self.ensure_capacity();
                    unsafe {
                        T::write_unchecked(self, val);
                        self.write_byte_unchecked(b'\n');
                    }
                }
                #[inline]
                fn spare_capacity(&self) -> usize {
                    BUF_SIZE - self.pos
                }
                fn ensure_capacity(&mut self) {
                    if self.spare_capacity() < MIN_WRITE_CAPACITY {
                        self.flush();
                    }
                }
                unsafe fn write_byte_unchecked(&mut self, byte: u8) {
                    unsafe {
                        let dst = self.buf.as_mut_ptr().add(self.pos);
                        std::ptr::write_unaligned(dst, byte);
                    }
                    self.pos += 1;
                }
                unsafe fn write_digits_unchecked<const LZ: bool>(&mut self, n: usize) {
                    static TABLE: [u8; 40_000] = {
                        let mut table = [b'0'; 40_000];
                        let mut i = 0;
                        while i < 10_000 {
                            table[4 * i + 0] += (i / 1000) as u8;
                            table[4 * i + 1] += (i / 100 % 10) as u8;
                            table[4 * i + 2] += (i / 10 % 10) as u8;
                            table[4 * i + 3] += (i % 10) as u8;
                            i += 1;
                        }
                        table
                    };
                    let offset = if LZ {
                        0
                    } else {
                        (n < 10) as usize + (n < 100) as usize + (n < 1000) as usize
                    };
                    unsafe {
                        let src = TABLE.as_ptr().add(4 * n + offset) as *const u32;
                        let dst = self.buf.as_mut_ptr().add(self.pos) as *mut u32;
                        std::ptr::write_unaligned(dst, std::ptr::read_unaligned(src));
                    }
                    self.pos += 4 - offset;
                }
            }
            pub trait Writable<W: Write> {
                unsafe fn write_unchecked(output: &mut Output<W>, val: Self);
            }
            impl<W: Write> Writable<W> for u32 {
                unsafe fn write_unchecked(output: &mut Output<W>, val: Self) {
                    unsafe {
                        if val >= 1_0000_0000 {
                            output.write_digits_unchecked::<false>((val / 10000 / 10000) as usize);
                            output.write_digits_unchecked::<true>((val / 10000 % 10000) as usize);
                            output.write_digits_unchecked::<true>((val % 10000) as usize);
                        } else if val >= 1_0000 {
                            output.write_digits_unchecked::<false>((val / 10000) as usize);
                            output.write_digits_unchecked::<true>((val % 10000) as usize);
                        } else {
                            output.write_digits_unchecked::<false>(val as usize);
                        }
                    }
                }
            }
            impl<W: Write> Writable<W> for u64 {
                unsafe fn write_unchecked(output: &mut Output<W>, val: Self) {
                    unsafe {
                        if val >= 1_0000_0000_0000_0000 {
                            output.write_digits_unchecked::<false>(
                                (val / 10000 / 10000 / 10000 / 10000) as usize,
                            );
                            output.write_digits_unchecked::<true>(
                                (val / 10000 / 10000 / 10000 % 10000) as usize,
                            );
                            output.write_digits_unchecked::<true>(
                                (val / 10000 / 10000 % 10000) as usize,
                            );
                            output.write_digits_unchecked::<true>((val / 10000 % 10000) as usize);
                            output.write_digits_unchecked::<true>((val % 10000) as usize);
                        } else if val >= 1_0000_0000_0000 {
                            output.write_digits_unchecked::<false>(
                                (val / 10000 / 10000 / 10000) as usize,
                            );
                            output.write_digits_unchecked::<true>(
                                (val / 10000 / 10000 % 10000) as usize,
                            );
                            output.write_digits_unchecked::<true>((val / 10000 % 10000) as usize);
                            output.write_digits_unchecked::<true>((val % 10000) as usize);
                        } else if val >= 1_0000_0000 {
                            output.write_digits_unchecked::<false>((val / 10000 / 10000) as usize);
                            output.write_digits_unchecked::<true>((val / 10000 % 10000) as usize);
                            output.write_digits_unchecked::<true>((val % 10000) as usize);
                        } else if val >= 1_0000 {
                            output.write_digits_unchecked::<false>((val / 10000) as usize);
                            output.write_digits_unchecked::<true>((val % 10000) as usize);
                        } else {
                            output.write_digits_unchecked::<false>(val as usize);
                        }
                    }
                }
            }
            impl<W: Write> Writable<W> for usize {
                unsafe fn write_unchecked(output: &mut Output<W>, val: Self) {
                    unsafe {
                        u64::write_unchecked(output, val as _);
                    }
                }
            }
            impl<W: Write> Writable<W> for i32 {
                unsafe fn write_unchecked(output: &mut Output<W>, val: Self) {
                    unsafe {
                        if val < 0 {
                            output.write_byte_unchecked(b'-');
                        }
                        u32::write_unchecked(output, val.abs() as _);
                    }
                }
            }
            impl<W: Write> Writable<W> for i64 {
                unsafe fn write_unchecked(output: &mut Output<W>, val: Self) {
                    unsafe {
                        if val < 0 {
                            output.write_byte_unchecked(b'-');
                        }
                        u64::write_unchecked(output, val.abs() as _);
                    }
                }
            }
        }
        #[cfg(unix)]
        pub use input::Input;
        pub use output::Output;
    }
    pub mod algebra {
        pub trait Monoid {
            type Elem: Clone;
            fn identity() -> Self::Elem;
            fn op(lhs: &Self::Elem, rhs: &Self::Elem) -> Self::Elem;
        }
        pub trait Group: Monoid {
            fn inv(elem: &Self::Elem) -> Self::Elem;
        }
        pub struct Reverse<M> {
            _phantom: std::marker::PhantomData<M>,
        }
        impl<M: Monoid> Monoid for Reverse<M> {
            type Elem = M::Elem;
            fn identity() -> Self::Elem {
                M::identity()
            }
            fn op(lhs: &Self::Elem, rhs: &Self::Elem) -> Self::Elem {
                M::op(rhs, lhs)
            }
        }
    }
}
