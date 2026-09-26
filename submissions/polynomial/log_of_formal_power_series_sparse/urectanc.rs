use urectanc::{fast_io, modint::ModInt998244353, polynomial::SparsePolynomial};

type Mint = ModInt998244353;

fn main() {
    let mut input = fast_io::stdin();
    let mut output = fast_io::stdout();

    let n: usize = input.val();
    let k: usize = input.val();
    let f: SparsePolynomial<_> = (0..k)
        .map(|_| (input.val(), Mint::raw(input.val())))
        .collect();

    let g = f.log(n).unwrap();
    for g in g {
        output.write(g.val());
    }
    output.writeln("");
}


pub mod urectanc {
    pub mod fast_io {
        mod input {
            use std::{io::Read, os::fd::FromRawFd};
            mod mman {
                use std::ffi::{c_int, c_void};
                pub const PROT_READ: c_int = 1;
                pub const MAP_PRIVATE: c_int = 2;
                #[link(name = "c")]
                unsafe extern "C" {
                    pub unsafe fn mmap(
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
                    Self { cursor: buf.as_ptr() }
                }
                pub fn stdin() -> Self {
                    use mman::*;
                    let mut stdin = unsafe { std::fs::File::from_raw_fd(0) };
                    let buf = match stdin.metadata() {
                        Ok(metadata) if metadata.is_file() => {
                            let len = metadata.len() as usize;
                            unsafe {
                                mmap(
                                    std::ptr::null_mut(),
                                    len,
                                    PROT_READ,
                                    MAP_PRIVATE,
                                    0,
                                    0,
                                ) as _
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
                    let val = input
                        .parse_8digits()
                        .map_or(
                            0,
                            |x| {
                                input.parse_8digits().map_or(x, |y| x * 100_000_000 + y)
                            },
                        );
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
                    impl Readable for $signed { fn read(input : & mut Input) -> Self {
                    let neg = input.parse_neg(); let val = <$unsigned >::read(input) as
                    Self; if neg { - val } else { val } } }
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
                    self.inner.write_all(&self.buf[..self.pos]).expect("flush failed");
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
                            table[4 * i] += (i / 1000) as u8;
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
                            output
                                .write_digits_unchecked::<
                                    false,
                                >((val / 10000 / 10000) as usize);
                            output
                                .write_digits_unchecked::<
                                    true,
                                >((val / 10000 % 10000) as usize);
                            output
                                .write_digits_unchecked::<true>((val % 10000) as usize);
                        } else if val >= 1_0000 {
                            output
                                .write_digits_unchecked::<false>((val / 10000) as usize);
                            output
                                .write_digits_unchecked::<true>((val % 10000) as usize);
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
                            output
                                .write_digits_unchecked::<
                                    false,
                                >((val / 10000 / 10000 / 10000 / 10000) as usize);
                            output
                                .write_digits_unchecked::<
                                    true,
                                >((val / 10000 / 10000 / 10000 % 10000) as usize);
                            output
                                .write_digits_unchecked::<
                                    true,
                                >((val / 10000 / 10000 % 10000) as usize);
                            output
                                .write_digits_unchecked::<
                                    true,
                                >((val / 10000 % 10000) as usize);
                            output
                                .write_digits_unchecked::<true>((val % 10000) as usize);
                        } else if val >= 1_0000_0000_0000 {
                            output
                                .write_digits_unchecked::<
                                    false,
                                >((val / 10000 / 10000 / 10000) as usize);
                            output
                                .write_digits_unchecked::<
                                    true,
                                >((val / 10000 / 10000 % 10000) as usize);
                            output
                                .write_digits_unchecked::<
                                    true,
                                >((val / 10000 % 10000) as usize);
                            output
                                .write_digits_unchecked::<true>((val % 10000) as usize);
                        } else if val >= 1_0000_0000 {
                            output
                                .write_digits_unchecked::<
                                    false,
                                >((val / 10000 / 10000) as usize);
                            output
                                .write_digits_unchecked::<
                                    true,
                                >((val / 10000 % 10000) as usize);
                            output
                                .write_digits_unchecked::<true>((val % 10000) as usize);
                        } else if val >= 1_0000 {
                            output
                                .write_digits_unchecked::<false>((val / 10000) as usize);
                            output
                                .write_digits_unchecked::<true>((val % 10000) as usize);
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
                        u32::write_unchecked(output, val.unsigned_abs());
                    }
                }
            }
            impl<W: Write> Writable<W> for i64 {
                unsafe fn write_unchecked(output: &mut Output<W>, val: Self) {
                    unsafe {
                        if val < 0 {
                            output.write_byte_unchecked(b'-');
                        }
                        u64::write_unchecked(output, val.unsigned_abs());
                    }
                }
            }
            impl<W: Write> Writable<W> for &str {
                unsafe fn write_unchecked(output: &mut Output<W>, val: Self) {
                    let len = val.len();
                    debug_assert!(len <= MIN_WRITE_CAPACITY);
                    unsafe {
                        let dst = output.buf.as_mut_ptr().add(output.pos);
                        std::ptr::copy_nonoverlapping(val.as_ptr(), dst, len);
                    }
                    output.pos += len;
                }
            }
        }
        #[cfg(unix)]
        pub use input::Input;
        pub use output::Output;
        pub fn stdin() -> Input {
            Input::stdin()
        }
        pub fn stdout() -> Output<std::io::StdoutLock<'static>> {
            Output::stdout()
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
        pub trait Modulus: 'static + Clone + Copy + Debug + Default + PartialEq + Eq {
            const MOD: u32;
        }
        macro_rules! define_modulus {
            ($name:ident, $modulus:expr) => {
                #[derive(Clone, Copy, Debug, Default, PartialEq, Eq)] pub struct $name;
                impl Modulus for $name { const MOD : u32 = const { assert!($modulus <
                (1u32 << 31)); $modulus }; }
            };
        }
        define_modulus!(Mod998244353, 998244353);
        define_modulus!(Mod1000000007, 1000000007);
        #[derive(Clone, Copy, Default, PartialEq, Eq, Hash)]
        #[repr(transparent)]
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
            pub const fn zero() -> Self {
                Self::raw(0)
            }
            pub const fn one() -> Self {
                Self::raw(1)
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
                self.checked_inv().expect("the inverse does not exist")
            }
            pub const fn checked_inv(self) -> Option<Self> {
                let (gcd, inv) = gcd_inv(self.val() as i64, Self::modulus() as i64);
                if gcd == 1 { Some(Self::raw(inv as u32)) } else { None }
            }
            fn to_rational(self) -> (i64, i64) {
                let m = Self::modulus() as i64;
                let mut u = (m, 0i64);
                let mut v = (self.val() as i64, 1i64);
                while v.0 * v.0 * 2 > m {
                    let q = u.0.div_euclid(v.0);
                    let w = (u.0 - q * v.0, u.1 - q * v.1);
                    (u, v) = (v, w);
                }
                if v.1 < 0 { (-v.0, -v.1) } else { v }
            }
        }
        impl<M: Modulus> std::fmt::Display for StaticModInt<M> {
            fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
                write!(f, "{}", self.val)
            }
        }
        impl<M: Modulus> std::fmt::Debug for StaticModInt<M> {
            fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
                let (num, denom) = self.to_rational();
                if denom == 1 { write!(f, "{num}") } else { write!(f, "{num}/{denom}") }
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
                self.val = ((self.val as u64 * rhs.into().val as u64)
                    % Self::modulus() as u64) as u32;
            }
        }
        impl<M: Modulus, T: Into<StaticModInt<M>>> DivAssign<T> for StaticModInt<M> {
            #[allow(clippy::suspicious_op_assign_impl)]
            fn div_assign(&mut self, rhs: T) {
                *self *= rhs.into().inv();
            }
        }
        macro_rules! impl_binnary_operators {
            ($op:ident, $op_assign:ident, $fn:ident, $fn_assign:ident) => {
                impl < M : Modulus, T : Into < StaticModInt < M >>> std::ops::$op < T >
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
                (* rhs); } }
            };
        }
        impl_binnary_operators!(Add, AddAssign, add, add_assign);
        impl_binnary_operators!(Sub, SubAssign, sub, sub_assign);
        impl_binnary_operators!(Mul, MulAssign, mul, mul_assign);
        impl_binnary_operators!(Div, DivAssign, div, div_assign);
        impl<M: Modulus> std::iter::Sum for StaticModInt<M> {
            fn sum<I: Iterator<Item = Self>>(iter: I) -> Self {
                iter.fold(Self::zero(), Add::add)
            }
        }
        impl<'a, M: Modulus> std::iter::Sum<&'a StaticModInt<M>> for StaticModInt<M> {
            fn sum<I: Iterator<Item = &'a Self>>(iter: I) -> Self {
                iter.fold(Self::zero(), Add::add)
            }
        }
        impl<M: Modulus> std::iter::Product for StaticModInt<M> {
            fn product<I: Iterator<Item = Self>>(iter: I) -> Self {
                iter.fold(Self::one(), Mul::mul)
            }
        }
        impl<'a, M: Modulus> std::iter::Product<&'a StaticModInt<M>>
        for StaticModInt<M> {
            fn product<I: Iterator<Item = &'a Self>>(iter: I) -> Self {
                iter.fold(Self::one(), Mul::mul)
            }
        }
        pub type ModInt998244353 = StaticModInt<Mod998244353>;
        pub type ModInt1000000007 = StaticModInt<Mod1000000007>;
    }
    pub mod polynomial {
        use crate::urectanc::{modint, number_theoretic_transform};
        use std::{
            collections::VecDeque, iter::{Product, Sum},
            ops::{
                Add, AddAssign, Index, IndexMut, Mul, MulAssign, Shl, Shr, Sub, SubAssign,
            },
        };
        use modint::{Modulus, StaticModInt};
        use number_theoretic_transform::{
            NTTFriendly, NumberTheoreticTransform, convolve,
        };
        pub use sparse::SparsePolynomial;
        mod sparse {
            use crate::urectanc::modint;
            use modint::{Modulus, StaticModInt};
            use super::{Polynomial, modinv_table};
            pub struct SparsePolynomial<M> {
                coeff: Vec<(usize, StaticModInt<M>)>,
            }
            impl<M: Modulus> SparsePolynomial<M> {
                pub fn inv(self, precision: usize) -> Option<Polynomial<M>> {
                    let c0 = self
                        .coeff
                        .get(0)
                        .and_then(|&(i, c)| (i == 0).then_some(c))?;
                    let scale = c0.inv();
                    if precision == 0 {
                        return Some(Polynomial::zero().prefix(precision));
                    }
                    let mut g = vec![Default::default(); precision];
                    g[0] = scale;
                    for i in 1..precision {
                        let mut gi = StaticModInt::<M>::zero();
                        for &(j, fj) in self.coeff.iter().take_while(|&&(j, _)| j <= i) {
                            gi -= fj * g[i - j];
                        }
                        g[i] = gi * scale;
                    }
                    Some(g.into())
                }
                pub fn log(self, precision: usize) -> Option<Polynomial<M>> {
                    self.coeff
                        .get(0)
                        .and_then(|&(i, c)| (i == 0 && c == 1.into()).then_some(()))?;
                    if precision == 0 {
                        return Some(Polynomial::zero().prefix(precision));
                    }
                    let inv = modinv_table::<M>(precision);
                    let mut g = vec![Default::default(); precision];
                    for &(i, fi) in self
                        .coeff[1..]
                        .iter()
                        .take_while(|&&(i, _)| i < precision)
                    {
                        g[i] = fi;
                    }
                    for i in 1..precision {
                        let mut gi = g[i] * i;
                        for &(j, fj) in self
                            .coeff[1..]
                            .iter()
                            .take_while(|&&(j, _)| j < i)
                        {
                            gi -= fj * g[i - j] * (i - j);
                        }
                        g[i] = gi * inv[i];
                    }
                    Some(g.into())
                }
                pub fn exp(self, precision: usize) -> Option<Polynomial<M>> {
                    if self.coeff.is_empty() {
                        return Some(Polynomial::one().prefix(precision));
                    }
                    self.coeff.get(0).and_then(|&(i, _)| (i > 0).then_some(()))?;
                    let inv = modinv_table::<M>(precision);
                    let mut g = vec![Default::default(); precision];
                    g[0] = StaticModInt::<M>::one();
                    for i in 1..precision {
                        let mut ci = StaticModInt::<M>::zero();
                        for &(j, fj) in self.coeff.iter().take_while(|&&(j, _)| j <= i) {
                            ci += fj * g[i - j] * j;
                        }
                        g[i] = ci * inv[i];
                    }
                    Some(g.into())
                }
                pub fn pow(mut self, exp: usize, precision: usize) -> Polynomial<M> {
                    if exp == 0 {
                        return Polynomial::one().prefix(precision);
                    }
                    let Some(&(shift, f0)) = self.coeff.first() else {
                        return Polynomial::zero().prefix(precision);
                    };
                    self.coeff.iter_mut().for_each(|(i, _)| *i -= shift);
                    let offset = shift.saturating_mul(exp).min(precision);
                    let precision = precision - offset;
                    if precision == 0 {
                        return Polynomial::zero().prefix(offset + precision);
                    }
                    let inv = modinv_table::<M>(precision);
                    let mut g = vec![Default::default(); precision];
                    g[0] = f0.pow(exp as _);
                    let scale = f0.inv();
                    let e1 = StaticModInt::<M>::one() + exp;
                    for i in 1..precision {
                        let mut gi = StaticModInt::<M>::zero();
                        for &(j, fj) in self
                            .coeff[1..]
                            .iter()
                            .take_while(|&&(j, _)| j <= i)
                        {
                            gi += fj * g[i - j] * (e1 * j - i);
                        }
                        g[i] = gi * inv[i] * scale;
                    }
                    std::iter::repeat_n(StaticModInt::<M>::zero(), offset)
                        .chain(g)
                        .collect()
                }
            }
            impl<M: Modulus> From<Polynomial<M>> for SparsePolynomial<M> {
                fn from(f: Polynomial<M>) -> Self {
                    f.into_iter().enumerate().collect()
                }
            }
            impl<M: Modulus> FromIterator<(usize, StaticModInt<M>)>
            for SparsePolynomial<M> {
                fn from_iter<T: IntoIterator<Item = (usize, StaticModInt<M>)>>(
                    iter: T,
                ) -> Self {
                    let mut coeff: Vec<_> = iter
                        .into_iter()
                        .filter(|&(_, c)| c != StaticModInt::<M>::zero())
                        .collect();
                    coeff.sort_unstable_by_key(|&(i, _)| i);
                    Self { coeff }
                }
            }
            impl<M: Modulus> From<SparsePolynomial<M>> for Polynomial<M> {
                fn from(f: SparsePolynomial<M>) -> Self {
                    let Some(deg) = f.coeff.iter().map(|&(i, _)| i).max() else {
                        return Polynomial::zero();
                    };
                    let mut coeff = vec![StaticModInt::< M >::zero(); deg + 1];
                    for &(i, c) in &f.coeff {
                        coeff[i] = c;
                    }
                    coeff.into()
                }
            }
        }
        #[derive(Clone)]
        pub struct Polynomial<M> {
            coeff: Vec<StaticModInt<M>>,
        }
        impl<M: Modulus> Polynomial<M> {
            pub fn zero() -> Self {
                vec![].into()
            }
            pub fn one() -> Self {
                vec![1.into()].into()
            }
            pub fn deg(&self) -> usize {
                self.coeff.len()
            }
            pub fn iter(&self) -> std::slice::Iter<'_, StaticModInt<M>> {
                self.coeff.iter()
            }
            pub fn iter_mut(&mut self) -> std::slice::IterMut<'_, StaticModInt<M>> {
                self.coeff.iter_mut()
            }
            pub fn prefix(&self, len: usize) -> Self {
                self.coeff
                    .iter()
                    .copied()
                    .chain(std::iter::repeat(0.into()))
                    .take(len)
                    .collect()
            }
            pub fn derivative(&self) -> Self {
                self.coeff.iter().enumerate().skip(1).map(|(i, x)| x * i).collect()
            }
            pub fn integral(&self) -> Self {
                if self.deg() == 0 {
                    return Self::zero();
                }
                let mut inv = modinv_table::<M>(self.deg());
                inv[1..].iter_mut().zip(&self.coeff).for_each(|(a, b)| *a *= b);
                inv.into()
            }
        }
        impl<M: NTTFriendly> Polynomial<M> {
            pub fn inv(&self, precision: usize) -> Option<Self> {
                (self[0] != 0.into()).then_some(())?;
                let mut inv = Self::from(vec![self[0].inv()]);
                while inv.deg() < precision {
                    self.refine_inv(&mut inv);
                }
                inv.coeff.truncate(precision);
                Some(inv)
            }
            fn refine_inv(&self, inv: &mut Self) {
                let n = inv.deg();
                let mut f = self.prefix(2 * n);
                let mut g = inv.prefix(2 * n);
                f.coeff.ntt();
                g.coeff.ntt();
                f.coeff.hadamard(&g.coeff);
                f.coeff.intt();
                f.coeff[..n].fill(0.into());
                f.coeff.ntt();
                f.coeff.hadamard(&g.coeff);
                f.coeff.intt();
                inv.coeff.extend(f.coeff[n..].iter().map(|&f| -f));
            }
            pub fn log(&self, precision: usize) -> Option<Self> {
                (self.deg() > 0 && self[0] == 1.into()).then_some(())?;
                let inv = self.inv(precision)?;
                Some((self.derivative() * inv).integral().prefix(precision))
            }
            pub fn exp(&self, precision: usize) -> Option<Self> {
                if self.deg() == 0 {
                    return Some(Self::one());
                }
                (self[0] == 0.into()).then_some(())?;
                let modinv = modinv_table::<M>(precision);
                let mut exp = Self::one();
                let mut exp_inv = Self::one();
                while exp.deg() < precision {
                    let n = exp.deg();
                    let mut f = exp.prefix(2 * n);
                    let mut g = exp_inv.prefix(2 * n);
                    f.coeff.ntt();
                    g.coeff.ntt();
                    let mut h = self.prefix(n);
                    h.coeff.iter_mut().enumerate().for_each(|(i, w)| *w *= i);
                    h.coeff.ntt();
                    h.coeff[..].hadamard(&f.coeff[..n]);
                    h.coeff.intt();
                    let mut s = exp.prefix(2 * n);
                    s.coeff[..n].iter_mut().enumerate().for_each(|(i, w)| *w *= i);
                    s -= h;
                    s.coeff.ntt();
                    s.coeff.hadamard(&g.coeff);
                    s.coeff.intt();
                    let mut u = self.prefix(2 * n);
                    u.coeff[..n].fill(0.into());
                    u.iter_mut()
                        .zip(&modinv)
                        .skip(n)
                        .zip(s.coeff)
                        .for_each(|((u, inv), s)| *u -= s * inv);
                    u.coeff.ntt();
                    u.coeff.hadamard(&f.coeff);
                    u.coeff.intt();
                    exp.coeff.extend_from_slice(&u.coeff[n..]);
                    if exp.deg() < precision {
                        exp.refine_inv(&mut exp_inv);
                    }
                }
                exp.coeff.truncate(precision);
                Some(exp)
            }
            pub fn pow(&self, exp: usize, precision: usize) -> Self {
                if exp == 0 {
                    return Self::one().prefix(precision);
                }
                let Some(shift) = self.iter().position(|&c| c != 0.into()) else {
                    return Self::zero().prefix(precision);
                };
                let c = self.coeff[shift];
                let scale = c.inv();
                let f: Polynomial<_> = self
                    .iter()
                    .skip(shift)
                    .map(|a| a * scale)
                    .collect();
                let mut f = f.log(precision).unwrap();
                f.iter_mut().for_each(|a| *a *= exp);
                let pow = f.exp(precision).unwrap();
                let scale = c.pow(exp as u64);
                std::iter::repeat_n(0.into(), shift.saturating_mul(exp))
                    .chain(pow.into_iter().map(|a| a * scale))
                    .take(precision)
                    .collect()
            }
            pub fn taylor_shift(mut self, shift: impl Into<StaticModInt<M>>) -> Self {
                let n = self.deg();
                let shift = shift.into();
                let mut coeff = vec![StaticModInt::< M >::one(); n];
                for i in 1..n {
                    coeff[i] = coeff[i - 1] * i;
                }
                self.coeff.hadamard(&coeff);
                self.coeff.reverse();
                coeff[n - 1] = coeff[n - 1].inv();
                for i in (1..n).rev() {
                    coeff[i - 1] = coeff[i] * i;
                }
                let exp = coeff
                    .iter()
                    .scan(
                        StaticModInt::one(),
                        |c, &finv| {
                            let a = *c * finv;
                            *c *= &shift;
                            Some(a)
                        },
                    )
                    .collect();
                self = (self * exp).prefix(n);
                self.coeff.reverse();
                self.coeff.hadamard(&coeff);
                self
            }
            pub fn bostan_mori(
                num: &Self,
                denom: &Self,
                mut k: usize,
            ) -> StaticModInt<M> {
                assert!(num.deg() < denom.deg());
                let ntt_len = (2 * denom.deg() - 1).next_power_of_two();
                let mut p = num.prefix(ntt_len);
                let mut q = denom.prefix(ntt_len);
                while k > 0 {
                    p.coeff.ntt();
                    q.coeff.ntt();
                    let mut r = q.clone();
                    for i in (0..ntt_len).step_by(2) {
                        r.coeff.swap(i, i + 1);
                    }
                    p.coeff.hadamard(&r.coeff);
                    q.coeff.hadamard(&r.coeff);
                    p.coeff.intt();
                    q.coeff.intt();
                    p = p.into_iter().skip(k & 1).step_by(2).collect();
                    q = q.into_iter().step_by(2).collect();
                    p.coeff.resize(ntt_len, 0.into());
                    q.coeff.resize(ntt_len, 0.into());
                    k >>= 1;
                }
                p[0] / q[0]
            }
        }
        impl<M, T> From<T> for Polynomial<M>
        where
            M: Modulus,
            T: AsRef<[StaticModInt<M>]>,
        {
            fn from(value: T) -> Self {
                Self {
                    coeff: value.as_ref().to_owned(),
                }
            }
        }
        impl<M, S> FromIterator<S> for Polynomial<M>
        where
            M: Modulus,
            S: Into<StaticModInt<M>>,
        {
            fn from_iter<T: IntoIterator<Item = S>>(iter: T) -> Self {
                Self::from(iter.into_iter().map(Into::into).collect::<Vec<_>>())
            }
        }
        impl<M: Modulus> IntoIterator for Polynomial<M> {
            type Item = StaticModInt<M>;
            type IntoIter = std::vec::IntoIter<Self::Item>;
            fn into_iter(self) -> Self::IntoIter {
                self.coeff.into_iter()
            }
        }
        impl<'a, M: Modulus> IntoIterator for &'a Polynomial<M> {
            type Item = &'a StaticModInt<M>;
            type IntoIter = std::slice::Iter<'a, StaticModInt<M>>;
            fn into_iter(self) -> Self::IntoIter {
                self.coeff.iter()
            }
        }
        impl<'a, M: Modulus> IntoIterator for &'a mut Polynomial<M> {
            type Item = &'a mut StaticModInt<M>;
            type IntoIter = std::slice::IterMut<'a, StaticModInt<M>>;
            fn into_iter(self) -> Self::IntoIter {
                self.coeff.iter_mut()
            }
        }
        impl<M> Index<usize> for Polynomial<M> {
            type Output = StaticModInt<M>;
            fn index(&self, index: usize) -> &Self::Output {
                &self.coeff[index]
            }
        }
        impl<M> IndexMut<usize> for Polynomial<M> {
            fn index_mut(&mut self, index: usize) -> &mut Self::Output {
                &mut self.coeff[index]
            }
        }
        impl<M: Modulus> std::fmt::Debug for Polynomial<M> {
            fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
                f.debug_list().entries(&self.coeff).finish()
            }
        }
        impl<M: Modulus> Add for Polynomial<M> {
            type Output = Polynomial<M>;
            fn add(mut self, rhs: Self) -> Self::Output {
                self += rhs;
                self
            }
        }
        impl<M: Modulus> AddAssign for Polynomial<M> {
            fn add_assign(&mut self, mut rhs: Self) {
                if self.deg() < rhs.deg() {
                    std::mem::swap(self, &mut rhs);
                }
                self.iter_mut().zip(rhs).for_each(|(l, r)| *l += r);
            }
        }
        impl<M: Modulus> Sub for Polynomial<M> {
            type Output = Polynomial<M>;
            fn sub(mut self, rhs: Self) -> Self::Output {
                self -= rhs;
                self
            }
        }
        impl<M: Modulus> SubAssign for Polynomial<M> {
            fn sub_assign(&mut self, mut rhs: Self) {
                if self.deg() < rhs.deg() {
                    std::mem::swap(self, &mut rhs);
                    self.iter_mut().for_each(|l| *l = -*l);
                    *self += rhs;
                } else {
                    self.iter_mut().zip(rhs).for_each(|(l, r)| *l -= r);
                }
            }
        }
        impl<M: NTTFriendly> Mul for Polynomial<M> {
            type Output = Polynomial<M>;
            fn mul(self, rhs: Self) -> Self::Output {
                &self * &rhs
            }
        }
        impl<M: NTTFriendly> Mul for &Polynomial<M> {
            type Output = Polynomial<M>;
            fn mul(self, rhs: Self) -> Self::Output {
                convolve(&self.coeff, &rhs.coeff).into()
            }
        }
        impl<M: NTTFriendly> MulAssign for Polynomial<M> {
            fn mul_assign(&mut self, rhs: Self) {
                self.coeff = convolve(&self.coeff, &rhs.coeff);
            }
        }
        impl<M: Modulus> Shl<usize> for Polynomial<M> {
            type Output = Self;
            #[allow(clippy::suspicious_arithmetic_impl)]
            fn shl(mut self, rhs: usize) -> Self::Output {
                self.coeff.resize(self.deg() + rhs, 0.into());
                self.coeff.rotate_right(rhs);
                self
            }
        }
        impl<M: Modulus> Shr<usize> for Polynomial<M> {
            type Output = Self;
            fn shr(mut self, rhs: usize) -> Self::Output {
                self.coeff.rotate_left(rhs);
                self.coeff.truncate(self.deg().saturating_sub(rhs));
                self
            }
        }
        impl<M: Modulus> Sum for Polynomial<M> {
            fn sum<I: Iterator<Item = Self>>(iter: I) -> Self {
                iter.fold(Self::zero(), |acc, item| acc + item)
            }
        }
        impl<M: NTTFriendly> Product for Polynomial<M> {
            fn product<I: Iterator<Item = Self>>(iter: I) -> Self {
                let mut que: VecDeque<_> = iter.collect();
                while que.len() > 1 {
                    let f = que.pop_front().unwrap();
                    let g = que.pop_front().unwrap();
                    que.push_back(f * g);
                }
                que.pop_front().unwrap_or(Self::one())
            }
        }
        pub fn berlekamp_massey<M: Modulus>(a: &[StaticModInt<M>]) -> Polynomial<M> {
            let mut b = vec![- StaticModInt::raw(1)];
            let mut c = vec![- StaticModInt::raw(1)];
            let mut y = StaticModInt::raw(1);
            let mut shift = 0;
            for i in 0..a.len() {
                shift += 1;
                let x = a[..=i]
                    .iter()
                    .rev()
                    .zip(&c)
                    .map(|(a, c)| a * c)
                    .sum::<StaticModInt<M>>();
                if x == 0.into() {
                    continue;
                }
                let r = x / y;
                if c.len() < b.len() + shift {
                    let old_c = c.clone();
                    c.resize(b.len() + shift, 0.into());
                    c[shift..]
                        .iter_mut()
                        .zip(std::mem::replace(&mut b, old_c))
                        .for_each(|(c, b)| *c -= r * b);
                    y = x;
                    shift = 0;
                } else {
                    c[shift..].iter_mut().zip(&b).for_each(|(c, b)| *c -= r * b);
                }
            }
            c.into_iter().skip(1).collect()
        }
        pub(crate) fn modinv_table<M: Modulus>(n: usize) -> Vec<StaticModInt<M>> {
            let mut inv = vec![StaticModInt::raw(0); n + 1];
            if n > 0 {
                inv[1] = 1.into();
            }
            let m = M::MOD as usize;
            for i in 2..=n {
                inv[i] = -inv[m % i] * (m / i);
            }
            inv
        }
    }
    pub mod number_theoretic_transform {
        use crate::urectanc::{modint, montgomery};
        use modint::{Mod998244353, Modulus, StaticModInt};
        use montgomery::{Montgomery, MontgomeryModInt};
        #[cfg(target_arch = "x86_64")]
        use std::arch::x86_64::__m256i;
        use std::mem::transmute;
        #[cfg(target_arch = "x86_64")]
        mod simd {
            use crate::urectanc::montgomery;
            use montgomery::simd::*;
            use std::arch::x86_64::{
                _mm256_add_epi32, _mm256_blend_epi32, _mm256_permute2x128_si256,
                _mm256_permutevar8x32_epi32, _mm256_setr_epi32, _mm256_shuffle_epi32,
                _mm256_sub_epi32,
            };
            use super::{ButterflyCache, ModInt, NTTFriendly};
            #[target_feature(enable = "avx2")]
            pub unsafe fn transform_avx2<M: NTTFriendly>(f: &mut [ModInt<M>]) {
                let &ButterflyCache { ref rate1, w1230, ref rate1230, .. } = &M::BUTTERFLY_CACHE;
                let n = f.len();
                assert!(n.is_power_of_two() && n >= 8);
                let log = n.trailing_zeros();
                for k in (3..log).rev() {
                    let block_size = 2 << k;
                    let offset = block_size >> 1;
                    transform_block::<M, true, true>(&mut f[..], offset, ModInt::one());
                    let mut w = rate1[0];
                    for (i, block) in f.chunks_exact_mut(block_size).enumerate().skip(1)
                    {
                        transform_block::<M, true, false>(block, offset, w);
                        w = w * rate1[i.trailing_ones() as usize];
                    }
                }
                let mut wx8 = w1230;
                for (i, chunk) in f.chunks_exact_mut(8).enumerate() {
                    let w0x8 = _mm256_permutevar8x32_epi32(
                        wx8,
                        _mm256_setr_epi32(7, 0, 7, 1, 7, 2, 7, 3),
                    );
                    let w1x8 = _mm256_permutevar8x32_epi32(
                        wx8,
                        _mm256_setr_epi32(7, 7, 4, 4, 7, 7, 5, 5),
                    );
                    let w2x8 = _mm256_permutevar8x32_epi32(
                        wx8,
                        _mm256_setr_epi32(7, 7, 7, 7, 6, 6, 6, 6),
                    );
                    let head = chunk.as_mut_ptr().cast();
                    let mut vec = unsafe { load(head) };
                    vec = mul2::<M>(vec, w2x8);
                    let a_negb = _mm256_blend_epi32::<
                        0b11110000,
                    >(vec, _mm256_sub_epi32(M::N2X8, vec));
                    let b_a = _mm256_permute2x128_si256::<1>(vec, vec);
                    vec = _mm256_add_epi32(a_negb, b_a);
                    vec = mul2::<M>(vec, w1x8);
                    let a_negb = _mm256_blend_epi32::<
                        0b11001100,
                    >(vec, _mm256_sub_epi32(M::N2X8, vec));
                    let b_a = _mm256_shuffle_epi32::<0b01_00_11_10>(vec);
                    vec = _mm256_add_epi32(a_negb, b_a);
                    vec = mul2::<M>(vec, w0x8);
                    let a_negb = _mm256_blend_epi32::<
                        0b10101010,
                    >(vec, _mm256_sub_epi32(M::N2X8, vec));
                    let b_a = _mm256_shuffle_epi32::<0b10_11_00_01>(vec);
                    vec = add2::<M>(a_negb, b_a);
                    unsafe { store(head, vec) };
                    wx8 = normalize::<
                        M,
                    >(mul2::<M>(wx8, rate1230[i.trailing_ones() as usize]));
                }
            }
            #[target_feature(enable = "avx2")]
            pub unsafe fn inverse_transform_avx2<M: NTTFriendly>(f: &mut [ModInt<M>]) {
                let &ButterflyCache { ref irate1, iw1230, ref irate1230, .. } = &M::BUTTERFLY_CACHE;
                let n = f.len();
                assert!(n.is_power_of_two() && n >= 8);
                let mut wx8 = iw1230;
                for (i, chunk) in f.chunks_exact_mut(8).enumerate() {
                    let w0x8 = _mm256_permutevar8x32_epi32(
                        wx8,
                        _mm256_setr_epi32(7, 0, 7, 1, 7, 2, 7, 3),
                    );
                    let w1x8 = _mm256_permutevar8x32_epi32(
                        wx8,
                        _mm256_setr_epi32(7, 7, 4, 4, 7, 7, 5, 5),
                    );
                    let w2x8 = _mm256_permutevar8x32_epi32(
                        wx8,
                        _mm256_setr_epi32(7, 7, 7, 7, 6, 6, 6, 6),
                    );
                    let head = chunk.as_mut_ptr().cast();
                    let mut vec = unsafe { load(head) };
                    let a_negb = _mm256_blend_epi32::<
                        0b10101010,
                    >(vec, _mm256_sub_epi32(M::N2X8, vec));
                    let b_a = _mm256_shuffle_epi32::<0b10_11_00_01>(vec);
                    vec = mul2::<M>(_mm256_add_epi32(a_negb, b_a), w0x8);
                    let a_negb = _mm256_blend_epi32::<
                        0b11001100,
                    >(vec, _mm256_sub_epi32(M::N2X8, vec));
                    let b_a = _mm256_shuffle_epi32::<0b01_00_11_10>(vec);
                    vec = mul2::<M>(_mm256_add_epi32(a_negb, b_a), w1x8);
                    let a_negb = _mm256_blend_epi32::<
                        0b11110000,
                    >(vec, _mm256_sub_epi32(M::N2X8, vec));
                    let b_a = _mm256_permute2x128_si256::<1>(vec, vec);
                    vec = mul2::<M>(_mm256_add_epi32(a_negb, b_a), w2x8);
                    unsafe { store(head, vec) };
                    wx8 = normalize::<
                        M,
                    >(mul2::<M>(wx8, irate1230[i.trailing_ones() as usize]));
                }
                let log = n.trailing_zeros();
                for k in 3..log {
                    let block_size = 2 << k;
                    let offset = block_size >> 1;
                    transform_block::<M, false, true>(&mut f[..], offset, ModInt::one());
                    let mut w = irate1[0];
                    for (i, block) in f.chunks_exact_mut(block_size).enumerate().skip(1)
                    {
                        transform_block::<M, false, false>(block, offset, w);
                        w = w * irate1[i.trailing_ones() as usize];
                    }
                }
                let inv_n = ModInt::<M>::new(n as u32).inv();
                let inv_nx8 = broadcast(inv_n.val);
                let ptr = f.as_mut_ptr();
                for i in (0..n).step_by(8) {
                    let ptr = unsafe { ptr.add(i) };
                    let mut a = unsafe { load(ptr.cast()) };
                    a = normalize::<M>(mul2::<M>(a, inv_nx8));
                    unsafe { store(ptr.cast(), a) };
                }
            }
            #[inline]
            #[target_feature(enable = "avx2")]
            fn transform_block<M: NTTFriendly, const FORWARD: bool, const TRIVIAL: bool>(
                block: &mut [ModInt<M>],
                offset: usize,
                w: ModInt<M>,
            ) {
                let head: *mut u32 = block.as_mut_ptr().cast();
                let wx8 = broadcast(w.val);
                for i in (0..offset).step_by(8) {
                    let (ptr_a, ptr_b) = unsafe { (head.add(i), head.add(offset + i)) };
                    let (mut a, mut b) = unsafe { (load(ptr_a), load(ptr_b)) };
                    if FORWARD && !TRIVIAL {
                        b = mul2::<M>(b, wx8);
                    }
                    (a, b) = (add2::<M>(a, b), sub2::<M>(a, b));
                    if !FORWARD && !TRIVIAL {
                        b = mul2::<M>(b, wx8);
                    }
                    unsafe {
                        store(ptr_a, a);
                        store(ptr_b, b);
                    }
                }
            }
        }
        pub trait NTTFriendly: Montgomery {
            const PRIMITIVE_ROOT: u32;
            const BUTTERFLY_CACHE: ButterflyCache<Self> = ButterflyCache::new();
        }
        impl NTTFriendly for Mod998244353 {
            const PRIMITIVE_ROOT: u32 = 3;
        }
        macro_rules! define_ntt_friendly_modulus {
            ($name:ident, $modulus:expr, $primitive_root:expr) => {
                #[derive(Clone, Copy, Debug, Default, PartialEq, Eq)] struct $name; impl
                Modulus for $name { const MOD : u32 = const { assert!($modulus < (1u32 <<
                31)); $modulus }; } impl Montgomery for $name {} impl NTTFriendly for
                $name { const PRIMITIVE_ROOT : u32 = $primitive_root; }
            };
        }
        define_ntt_friendly_modulus!(Mod167772161, 167772161, 3);
        define_ntt_friendly_modulus!(Mod469762049, 469762049, 3);
        define_ntt_friendly_modulus!(Mod754974721, 754974721, 11);
        pub trait NumberTheoreticTransform<M, T: ?Sized> {
            fn ntt(&mut self);
            fn intt(&mut self);
            fn hadamard(&mut self, rhs: &T);
        }
        impl<M, T: ?Sized> NumberTheoreticTransform<M, T> for T
        where
            T: AsRef<[StaticModInt<M>]> + AsMut<[StaticModInt<M>]>,
            M: NTTFriendly,
        {
            fn ntt(&mut self) {
                let f = unsafe {
                    transmute::<&mut [StaticModInt<M>], &mut [ModInt<M>]>(self.as_mut())
                };
                assert!(f.len() <= (1 << (M::MOD - 1).trailing_zeros()));
                transform::<M>(f);
            }
            fn intt(&mut self) {
                let f = unsafe {
                    transmute::<&mut [StaticModInt<M>], &mut [ModInt<M>]>(self.as_mut())
                };
                assert!(f.len() <= (1 << (M::MOD - 1).trailing_zeros()));
                inverse_transform::<M>(f);
            }
            fn hadamard(&mut self, rhs: &T) {
                std::iter::zip(self.as_mut(), rhs.as_ref()).for_each(|(l, r)| *l *= r);
            }
        }
        pub fn convolve<M: NTTFriendly>(
            lhs: impl AsRef<[StaticModInt<M>]>,
            rhs: impl AsRef<[StaticModInt<M>]>,
        ) -> Vec<StaticModInt<M>> {
            let mut lhs = lhs.as_ref().to_owned();
            let mut rhs = rhs.as_ref().to_owned();
            let new_len = lhs.len() + rhs.len() - 1;
            let ntt_len = new_len.next_power_of_two();
            lhs.resize(ntt_len, 0.into());
            rhs.resize(ntt_len, 0.into());
            lhs.ntt();
            rhs.ntt();
            lhs.hadamard(&rhs);
            lhs.intt();
            lhs.truncate(new_len);
            lhs
        }
        pub fn convolve_mod_arbitrary(
            lhs: impl AsRef<[u32]>,
            rhs: impl AsRef<[u32]>,
            modulus: u32,
        ) -> Vec<u32> {
            assert!(modulus <= 2663300486);
            const M1: u64 = Mod167772161::MOD as _;
            const M2: u64 = Mod469762049::MOD as _;
            let m = modulus as u64;
            let m1m2 = (M1 * M2) % m;
            let inv_m1 = StaticModInt::<Mod469762049>::from(M1).inv();
            let inv_m1m2 = StaticModInt::<Mod754974721>::from(M1 * M2).inv();
            let (lhs, rhs) = (lhs.as_ref(), rhs.as_ref());
            let r1 = convolve::<
                Mod167772161,
            >(
                lhs.iter().copied().map(Into::into).collect::<Vec<_>>(),
                rhs.iter().copied().map(Into::into).collect::<Vec<_>>(),
            );
            let r2 = convolve::<
                Mod469762049,
            >(
                lhs.iter().copied().map(Into::into).collect::<Vec<_>>(),
                rhs.iter().copied().map(Into::into).collect::<Vec<_>>(),
            );
            let r3 = convolve::<
                Mod754974721,
            >(
                lhs.iter().copied().map(Into::into).collect::<Vec<_>>(),
                rhs.iter().copied().map(Into::into).collect::<Vec<_>>(),
            );
            r1.iter()
                .zip(&r2)
                .zip(&r3)
                .map(|((r1, r2), r3)| {
                    let c1 = r1.val() as u64;
                    let c2 = ((r2 - c1) * inv_m1).val() as u64;
                    let c3 = ((r3 - c1 - c2 * M1) * inv_m1m2).val() as u64;
                    ((c1 + c2 * M1 + c3 * m1m2) % m) as u32
                })
                .collect()
        }
        type ModInt<M> = MontgomeryModInt<M>;
        pub struct ButterflyCache<M: NTTFriendly> {
            imag: ModInt<M>,
            iimag: ModInt<M>,
            rate1: [ModInt<M>; 30],
            irate1: [ModInt<M>; 30],
            rate2: [ModInt<M>; 30],
            irate2: [ModInt<M>; 30],
            #[cfg(target_arch = "x86_64")]
            w1230: __m256i,
            #[cfg(target_arch = "x86_64")]
            iw1230: __m256i,
            #[cfg(target_arch = "x86_64")]
            rate1230: [__m256i; 30],
            #[cfg(target_arch = "x86_64")]
            irate1230: [__m256i; 30],
        }
        impl<M: NTTFriendly> ButterflyCache<M> {
            #[allow(clippy::new_without_default)]
            pub const fn new() -> Self {
                let lg = (M::MOD - 1).trailing_zeros() as usize;
                let mut r = ModInt::<M>::new(M::PRIMITIVE_ROOT).pow((M::MOD - 1) >> lg);
                let mut ir = r.inv();
                let mut root = [ModInt::<M>::zero(); 30];
                let mut iroot = [ModInt::<M>::zero(); 30];
                let mut rate1 = [ModInt::<M>::zero(); 30];
                let mut irate1 = [ModInt::<M>::zero(); 30];
                let mut rate2 = [ModInt::<M>::zero(); 30];
                let mut irate2 = [ModInt::<M>::zero(); 30];
                let mut i = lg;
                while i > 0 {
                    i -= 1;
                    root[i] = r.normalize();
                    iroot[i] = ir.normalize();
                    r = r.mul2(r);
                    ir = ir.mul2(ir);
                }
                let one = ModInt::<M>::one();
                let mut rate1230 = [[ModInt::<M>::zero(); 8]; 30];
                let mut irate1230 = [[ModInt::<M>::zero(); 8]; 30];
                let mut acc = ModInt::<M>::one();
                let mut iacc = ModInt::<M>::one();
                let mut i = 0;
                while i < lg - 1 {
                    let r3 = root[i + 3].mul2(iacc).normalize();
                    let ir3 = iroot[i + 3].mul2(acc).normalize();
                    let r2 = r3.mul2(r3).normalize();
                    let ir2 = ir3.mul2(ir3).normalize();
                    let r1 = r2.mul2(r2).normalize();
                    let ir1 = ir2.mul2(ir2).normalize();
                    rate1[i] = r1;
                    irate1[i] = ir1;
                    rate2[i] = r2;
                    irate2[i] = ir2;
                    rate1230[i] = [r3, r3, r3, r3, r2, r2, r1, one];
                    irate1230[i] = [ir3, ir3, ir3, ir3, ir2, ir2, ir1, one];
                    acc = root[i + 3].mul2(acc);
                    iacc = iroot[i + 3].mul2(iacc);
                    i += 1;
                }
                let (deg90, deg45, deg135) = (
                    root[1],
                    root[2],
                    root[1].mul2(root[2]).normalize(),
                );
                let (deg270, deg315, deg225) = (
                    iroot[1],
                    iroot[2],
                    iroot[1].mul2(iroot[2]).normalize(),
                );
                let w1230 = [one, deg90, deg45, deg135, one, deg90, one, one];
                let iw1230 = [one, deg270, deg315, deg225, one, deg270, one, one];
                Self {
                    imag: deg90,
                    iimag: deg270,
                    rate1,
                    irate1,
                    rate2,
                    irate2,
                    #[cfg(target_arch = "x86_64")]
                    w1230: unsafe { transmute::<[ModInt<M>; 8], __m256i>(w1230) },
                    #[cfg(target_arch = "x86_64")]
                    iw1230: unsafe { transmute::<[ModInt<M>; 8], __m256i>(iw1230) },
                    #[cfg(target_arch = "x86_64")]
                    rate1230: unsafe {
                        transmute::<[[ModInt<M>; 8]; 30], [__m256i; 30]>(rate1230)
                    },
                    #[cfg(target_arch = "x86_64")]
                    irate1230: unsafe {
                        transmute::<[[ModInt<M>; 8]; 30], [__m256i; 30]>(irate1230)
                    },
                }
            }
        }
        fn transform<M: NTTFriendly>(f: &mut [ModInt<M>]) {
            if f.len() >= 8 && is_x86_feature_detected!("avx") {
                unsafe { simd::transform_avx2(f) };
                return;
            }
            let &ButterflyCache { imag, ref rate2, .. } = &M::BUTTERFLY_CACHE;
            let n = f.len();
            assert!(n.is_power_of_two());
            let log = n.trailing_zeros();
            if log % 2 == 1 {
                let (b0, b1) = f.split_at_mut(n / 2);
                for (x0, x1) in std::iter::zip(b0, b1) {
                    (*x0, *x1) = (*x0 + *x1, *x0 - *x1);
                }
            }
            for k in (0..log).step_by(2).rev() {
                let block_size = 1 << k;
                let mut w = ModInt::<M>::one();
                for (i, chunk) in f.chunks_exact_mut(4 * block_size).enumerate() {
                    let w2 = w * w;
                    let w3 = w2 * w;
                    let (b01, b23) = chunk.split_at_mut(2 * block_size);
                    let (b0, b1) = b01.split_at_mut(block_size);
                    let (b2, b3) = b23.split_at_mut(block_size);
                    for (((x0, x1), x2), x3) in b0.iter_mut().zip(b1).zip(b2).zip(b3) {
                        let (y0, y1, y2, y3) = (*x0, *x1 * w, *x2 * w2, *x3 * w3);
                        let (z0, z1, z2, z3) = (
                            y0 + y2,
                            y1 + y3,
                            y0 - y2,
                            (y1 - y3) * imag,
                        );
                        (*x0, *x1, *x2, *x3) = (z0 + z1, z0 - z1, z2 + z3, z2 - z3);
                    }
                    w = w * rate2[i.trailing_ones() as usize];
                }
            }
        }
        fn inverse_transform<M: NTTFriendly>(f: &mut [ModInt<M>]) {
            if f.len() >= 8 && is_x86_feature_detected!("avx") {
                unsafe { simd::inverse_transform_avx2(f) };
                return;
            }
            let &ButterflyCache { iimag, ref irate2, .. } = &M::BUTTERFLY_CACHE;
            let n = f.len();
            assert!(n.is_power_of_two());
            let log = n.trailing_zeros();
            for k in (0..log).step_by(2) {
                let block_size = 1 << k;
                let mut w = ModInt::<M>::one();
                for (i, chunk) in f.chunks_exact_mut(4 * block_size).enumerate() {
                    let (b01, b23) = chunk.split_at_mut(2 * block_size);
                    let (b0, b1) = b01.split_at_mut(block_size);
                    let (b2, b3) = b23.split_at_mut(block_size);
                    let w2 = w * w;
                    let w3 = w2 * w;
                    for (((x0, x1), x2), x3) in b0.iter_mut().zip(b1).zip(b2).zip(b3) {
                        let (y0, y1, y2, y3) = (*x0, *x1, *x2, *x3);
                        let (z0, z1, z2, z3) = (
                            y0 + y1,
                            y0 - y1,
                            y2 + y3,
                            (y2 - y3) * iimag,
                        );
                        (*x0, *x1, *x2, *x3) = (
                            z0 + z2,
                            (z1 + z3) * w,
                            (z0 - z2) * w2,
                            (z1 - z3) * w3,
                        );
                    }
                    w = w * irate2[i.trailing_ones() as usize];
                }
            }
            if log % 2 == 1 {
                let (b0, b1) = f.split_at_mut(n / 2);
                for (x0, x1) in std::iter::zip(b0, b1) {
                    (*x0, *x1) = (*x0 + *x1, *x0 - *x1);
                }
            }
            let inv_n = ModInt::<M>::new(n as u32).inv();
            f.iter_mut().for_each(|x| *x = (*x * inv_n).normalize());
        }
    }
    pub mod montgomery {
        use crate::urectanc::modint;
        #[cfg(target_arch = "x86_64")]
        use std::arch::x86_64::__m256i;
        use std::ops::{Add, Mul, Sub};
        use modint::{Mod998244353, Modulus};
        #[cfg(target_arch = "x86_64")]
        pub mod simd {
            use std::arch::x86_64::{
                __m256i, _mm256_add_epi32, _mm256_add_epi64, _mm256_bsrli_epi128,
                _mm256_loadu_si256, _mm256_min_epu32, _mm256_mul_epu32, _mm256_or_si256,
                _mm256_set1_epi32, _mm256_storeu_si256, _mm256_sub_epi32,
            };
            use super::Montgomery;
            #[inline]
            #[target_feature(enable = "avx2")]
            pub unsafe fn load(ptr: *const u32) -> __m256i {
                unsafe { _mm256_loadu_si256(ptr as _) }
            }
            #[inline]
            #[target_feature(enable = "avx2")]
            pub unsafe fn store(dst: *mut u32, val: __m256i) {
                unsafe { _mm256_storeu_si256(dst.cast(), val) };
            }
            #[inline]
            #[target_feature(enable = "avx2")]
            pub fn broadcast(val: u32) -> __m256i {
                _mm256_set1_epi32(val as _)
            }
            #[inline]
            #[target_feature(enable = "avx2")]
            pub fn normalize<M: Montgomery>(val: __m256i) -> __m256i {
                _mm256_min_epu32(val, _mm256_sub_epi32(val, M::NX8))
            }
            #[inline]
            #[target_feature(enable = "avx2")]
            pub fn normalize2<M: Montgomery>(val: __m256i) -> __m256i {
                _mm256_min_epu32(val, _mm256_sub_epi32(val, M::N2X8))
            }
            #[inline]
            #[target_feature(enable = "avx2")]
            pub fn add2<M: Montgomery>(lhs: __m256i, rhs: __m256i) -> __m256i {
                normalize2::<M>(_mm256_add_epi32(lhs, rhs))
            }
            #[inline]
            #[target_feature(enable = "avx2")]
            pub fn sub2<M: Montgomery>(lhs: __m256i, rhs: __m256i) -> __m256i {
                normalize2::<M>(_mm256_sub_epi32(_mm256_add_epi32(lhs, M::N2X8), rhs))
            }
            #[inline]
            #[target_feature(enable = "avx2")]
            pub fn mul2<M: Montgomery>(lhs: __m256i, rhs: __m256i) -> __m256i {
                let lhs1 = _mm256_bsrli_epi128(lhs, 4);
                let rhs1 = _mm256_bsrli_epi128(rhs, 4);
                let t0 = _mm256_mul_epu32(lhs, rhs);
                let t1 = _mm256_mul_epu32(lhs1, rhs1);
                let m0 = _mm256_mul_epu32(t0, M::N_PRIMEX8);
                let m1 = _mm256_mul_epu32(t1, M::N_PRIMEX8);
                let res0 = _mm256_add_epi64(t0, _mm256_mul_epu32(m0, M::NX8));
                let res1 = _mm256_add_epi64(t1, _mm256_mul_epu32(m1, M::NX8));
                _mm256_or_si256(_mm256_bsrli_epi128(res0, 4), res1)
            }
        }
        pub trait Montgomery: Modulus {
            const N: u32 = {
                assert!(Self::MOD & 1 == 1);
                assert!(Self::MOD < 1 << 30);
                Self::MOD
            };
            const N2: u32 = Self::N * 2;
            const N_PRIME: u32 = {
                let mut m = 1u32;
                m = m.wrapping_mul(2 + m.wrapping_mul(Self::N));
                m = m.wrapping_mul(2 + m.wrapping_mul(Self::N));
                m = m.wrapping_mul(2 + m.wrapping_mul(Self::N));
                m = m.wrapping_mul(2 + m.wrapping_mul(Self::N));
                m = m.wrapping_mul(2 + m.wrapping_mul(Self::N));
                assert!(Self::N.wrapping_mul(m) == ! 0);
                m
            };
            const R: u32 = ((1u64 << 32) % Self::N as u64) as u32;
            const RR: u32 = ((Self::R as u64 * Self::R as u64) % Self::N as u64) as u32;
            #[cfg(target_arch = "x86_64")]
            const NX8: __m256i = unsafe { std::mem::transmute::<_, _>([Self::N; 8]) };
            #[cfg(target_arch = "x86_64")]
            const N2X8: __m256i = unsafe { std::mem::transmute::<_, _>([Self::N2; 8]) };
            #[cfg(target_arch = "x86_64")]
            const N_PRIMEX8: __m256i = unsafe {
                std::mem::transmute::<_, _>([Self::N_PRIME; 8])
            };
        }
        impl Montgomery for Mod998244353 {}
        #[derive(Clone, Copy)]
        #[repr(transparent)]
        pub struct MontgomeryModInt<M: Montgomery> {
            pub val: u32,
            _phantom: std::marker::PhantomData<M>,
        }
        impl<M: Montgomery> MontgomeryModInt<M> {
            pub const fn new(val: u32) -> Self {
                Self::raw(val).mul2(Self::raw(M::RR))
            }
            pub const fn raw(val: u32) -> Self {
                Self {
                    val,
                    _phantom: std::marker::PhantomData,
                }
            }
            pub const fn zero() -> Self {
                Self::raw(0)
            }
            pub const fn one() -> Self {
                Self::raw(M::R)
            }
            pub const fn normalize(self) -> Self {
                let (sub, borrow) = self.val.overflowing_sub(M::N);
                Self::raw(if borrow { self.val } else { sub })
            }
            pub const fn normalize2(self) -> Self {
                let (sub, borrow) = self.val.overflowing_sub(M::N2);
                Self::raw(if borrow { self.val } else { sub })
            }
            pub const fn add2(self, rhs: Self) -> Self {
                Self::raw(self.val + rhs.val).normalize2()
            }
            pub const fn sub2(self, rhs: Self) -> Self {
                Self::raw(self.val + M::N2 - rhs.val).normalize2()
            }
            pub const fn mul2(self, rhs: Self) -> Self {
                let t = self.val as u64 * rhs.val as u64;
                let m = (t as u32).wrapping_mul(M::N_PRIME);
                Self::raw(((t + (m as u64 * M::N as u64)) >> 32) as u32)
            }
            pub const fn pow(self, mut e: u32) -> Self {
                let mut result = Self::one();
                let mut base = self;
                while e > 0 {
                    if e & 1 == 1 {
                        result = result.mul2(base);
                    }
                    base = base.mul2(base);
                    e >>= 1;
                }
                result.normalize()
            }
            pub const fn inv(self) -> Self {
                self.pow(M::N - 2)
            }
        }
        impl<M: Montgomery> Add for MontgomeryModInt<M> {
            type Output = Self;
            fn add(self, rhs: Self) -> Self::Output {
                self.add2(rhs)
            }
        }
        impl<M: Montgomery> Sub for MontgomeryModInt<M> {
            type Output = Self;
            fn sub(self, rhs: Self) -> Self::Output {
                self.sub2(rhs)
            }
        }
        impl<M: Montgomery> Mul for MontgomeryModInt<M> {
            type Output = Self;
            fn mul(self, rhs: Self) -> Self::Output {
                self.mul2(rhs)
            }
        }
    }
}
