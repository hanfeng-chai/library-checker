use urectanc::{fast_io, modint::ModInt998244353, rerooting};

type Mint = ModInt998244353;

fn main() {
    let mut input = fast_io::stdin();
    let mut output = fast_io::stdout();

    let n: usize = input.val();
    let a: Vec<u32> = input.vec(n);
    let edges = (0..n - 1).map(|_| {
        let u: usize = input.val();
        let v: usize = input.val();
        let b = Mint::raw(input.val());
        let c = Mint::raw(input.val());
        (u, v, (b, c))
    });

    let ans = rerooting::solve(edges, R { a });
    for ans in ans {
        output.write(ans.0.val());
    }
}

struct R {
    a: Vec<u32>,
}

impl rerooting::Rerooting for R {
    type Value = (Mint, u32);
    type EdgeWeight = (Mint, Mint);

    fn identity(&self) -> Self::Value {
        (Mint::zero(), 0)
    }

    fn fold(&self, x: &Self::Value, y: &Self::Value) -> Self::Value {
        (x.0 + y.0, x.1 + y.1)
    }

    fn add_edge(&self, x: &Self::Value, w: &Self::EdgeWeight) -> Self::Value {
        (w.0 * x.0 + w.1 * x.1, x.1)
    }

    fn add_node(&self, x: &Self::Value, v: usize) -> Self::Value {
        (x.0 + self.a[v], x.1 + 1)
    }
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
            ($(($name:ident, $modulus:expr)),* $(,)?) => {
                $(#[doc = concat!("A modulus marker for `", stringify!($modulus), "`.")]
                #[derive(Clone, Copy, Debug, Default, PartialEq, Eq)] pub struct $name;
                impl Modulus for $name { const MOD : u32 = $modulus; })*
            };
        }
        define_modulus!((Mod998244353, 998244353), (Mod1000000007, 1000000007));
        #[derive(Clone, Copy, Default, PartialEq, Eq, Hash)]
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
                let Some(inv) = self.checked_inv() else {
                    panic!("the inverse does not exist")
                };
                inv
            }
            pub const fn checked_inv(self) -> Option<Self> {
                let (gcd, inv) = gcd_inv(self.val() as i64, Self::modulus() as i64);
                if gcd == 1 { Some(Self::raw(inv as u32)) } else { None }
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
                let (num, denom) = self.into_rational();
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
    pub mod rerooting {
        use crate::urectanc::compressed_sparse_row;
        use compressed_sparse_row::CsrArray;
        pub trait Rerooting {
            type Value: Copy;
            type EdgeWeight: Copy + Default;
            fn identity(&self) -> Self::Value;
            fn fold(&self, x: &Self::Value, y: &Self::Value) -> Self::Value;
            fn add_edge(&self, x: &Self::Value, w: &Self::EdgeWeight) -> Self::Value;
            fn add_node(&self, x: &Self::Value, v: usize) -> Self::Value;
        }
        pub fn solve<R: Rerooting>(
            edges: impl Iterator<Item = (usize, usize, R::EdgeWeight)>,
            op: R,
        ) -> Vec<R::Value> {
            let edges: Vec<_> = edges
                .into_iter()
                .flat_map(|(u, v, w)| [(u, (v, w)), (v, (u, w))])
                .collect();
            let n = edges.len() / 2 + 1;
            let graph = CsrArray::new(n, &edges);
            let root = 0;
            let mut bfs_order = Vec::with_capacity(n);
            bfs_order.push((root, !0));
            for i in 0..n {
                let (v, p) = bfs_order[i];
                for &(c, _) in graph[v].iter().filter(|&&(c, _)| c != p) {
                    bfs_order.push((c, v));
                }
            }
            let mut dp = vec![op.identity(); n];
            let mut res = vec![op.identity(); n];
            for &(v, p) in bfs_order.iter().rev() {
                for &(c, ref w) in graph[v].iter().filter(|&&(c, _)| c != p) {
                    dp[c] = op.add_edge(&op.add_node(&dp[c], c), w);
                    dp[v] = op.fold(&dp[v], &dp[c]);
                }
            }
            let mut prefix = vec![];
            for &(v, p) in &bfs_order {
                let mut acc = res[v];
                for &(c, _) in graph[v].iter().filter(|&&(c, _)| c != p) {
                    prefix.push(acc);
                    acc = op.fold(&acc, &dp[c]);
                }
                res[v] = op.add_node(&acc, v);
                let mut racc = op.identity();
                for &(c, ref w) in graph[v].iter().filter(|&&(c, _)| c != p).rev() {
                    let acc = prefix.pop().unwrap();
                    res[c] = op.add_edge(&op.add_node(&op.fold(&acc, &racc), v), w);
                    racc = op.fold(&dp[c], &racc);
                }
            }
            res
        }
    }
    pub mod compressed_sparse_row {
        pub struct CsrArray<T> {
            n: usize,
            index: Vec<usize>,
            csr: Vec<T>,
        }
        impl<T> std::ops::Index<usize> for CsrArray<T> {
            type Output = [T];
            fn index(&self, index: usize) -> &Self::Output {
                self.get(index).unwrap()
            }
        }
        impl<'a, T> IntoIterator for &'a CsrArray<T> {
            type Item = &'a [T];
            type IntoIter = Row<'a, T>;
            fn into_iter(self) -> Self::IntoIter {
                self.iter()
            }
        }
        impl<T> CsrArray<T> {
            pub fn new(n: usize, items: impl AsRef<[(usize, T)]>) -> Self
            where
                T: Copy + Default,
            {
                let items = items.as_ref();
                let mut index = vec![0; n + 1];
                for &(k, _) in items {
                    index[k] += 1;
                }
                for i in 0..n {
                    index[i + 1] += index[i];
                }
                let m = items.len();
                let mut csr = vec![T::default(); m];
                for &(k, v) in items.iter().rev() {
                    index[k] -= 1;
                    csr[index[k]] = v;
                }
                Self { n, index, csr }
            }
            pub fn len(&self) -> usize {
                self.n
            }
            pub fn get(&self, i: usize) -> Option<&[T]> {
                (i < self.n).then(|| &self.csr[self.index[i]..self.index[i + 1]])
            }
            pub fn iter(&'_ self) -> Row<'_, T> {
                Row { csr: self, index: 0 }
            }
        }
        pub struct Row<'a, T> {
            csr: &'a CsrArray<T>,
            index: usize,
        }
        impl<'a, T> Iterator for Row<'a, T> {
            type Item = &'a [T];
            fn next(&mut self) -> Option<Self::Item> {
                let res = self.csr.get(self.index)?;
                self.index += 1;
                Some(res)
            }
        }
    }
}
