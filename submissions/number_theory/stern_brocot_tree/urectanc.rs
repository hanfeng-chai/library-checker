use urectanc::{fast_io, rational::Rational, stern_brocot_tree::SternBrocotTree};

const ENCODE_PATH: &[u8] = "ENCODE_PATH".as_bytes();
const DECODE_PATH: &[u8] = "DECODE_PATH".as_bytes();
const LCA: &[u8] = "LCA".as_bytes();
const ANCESTOR: &[u8] = "ANCESTOR".as_bytes();
const RANGE: &[u8] = "RANGE".as_bytes();

fn main() {
    let mut input = fast_io::stdin();
    let mut output = fast_io::stdout();

    let t: usize = input.val();
    for _ in 0..t {
        let problem = input.bytes();
        match problem {
            ENCODE_PATH => {
                let r: Rational<u32> = Rational::new(input.val(), input.val());
                let path = r.to_continued_fraction();
                let len = path.len() - (path[0] == 0) as usize;
                output.write(len);
                for (i, &cnt) in path.iter().enumerate() {
                    if cnt > 0 {
                        let dir = if i % 2 == 0 { "R" } else { "L" };
                        output.write(dir);
                        output.write(cnt);
                    }
                }
                output.writeln("");
            }
            DECODE_PATH => {
                let k: usize = input.val();
                let mut sbt = SternBrocotTree::<u32>::root();
                for _ in 0..k {
                    let c = input.bytes()[0];
                    let n: u32 = input.val();
                    sbt = match c {
                        b'L' => sbt.nth_left(n),
                        b'R' => sbt.nth_right(n),
                        _ => unreachable!(),
                    };
                }
                let x = sbt.val();
                output.write(x.num());
                output.writeln(x.denom());
            }
            LCA => {
                let p = Rational::<u32>::new(input.val(), input.val()).to_continued_fraction();
                let q = Rational::<u32>::new(input.val(), input.val()).to_continued_fraction();
                let mut node = SternBrocotTree::root();
                for (i, (l, r)) in std::iter::zip(p, q).enumerate() {
                    let m = l.min(r);
                    node = if i % 2 == 0 {
                        node.nth_right(m)
                    } else {
                        node.nth_left(m)
                    };
                    if l != r {
                        break;
                    }
                }

                let lca = node.val();
                output.write(lca.num());
                output.writeln(lca.denom());
            }
            ANCESTOR => {
                let k: u32 = input.val();
                let r: Rational<u32> = Rational::new(input.val(), input.val());
                let path = r.to_continued_fraction();
                if path.iter().sum::<u32>() < k {
                    output.writeln(-1);
                    continue;
                }
                let truncated = path
                    .iter()
                    .scan(k, |acc, &cnt| {
                        (*acc > 0).then(|| {
                            let m = cnt.min(*acc);
                            *acc -= m;
                            m
                        })
                    })
                    .collect::<Vec<_>>();
                let ancestor = SternBrocotTree::from(truncated).val();
                output.write(ancestor.num());
                output.writeln(ancestor.denom());
            }
            RANGE => {
                let r: Rational<u32> = Rational::new(input.val(), input.val());
                let sbt = SternBrocotTree::new(r);
                let lo = sbt.lower_bound();
                let hi = sbt.upper_bound();
                output.write(lo.num());
                output.write(lo.denom());
                output.write(hi.num());
                output.writeln(hi.denom());
            }
            _ => unreachable!(),
        }
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
    pub mod rational {
        use crate::urectanc::num_traits;
        use std::fmt::Debug;
        use num_traits::PrimitiveInteger;
        #[derive(Clone, Copy)]
        pub struct Rational<I> {
            num: I,
            denom: I,
        }
        impl<I: PrimitiveInteger> Rational<I> {
            pub fn new(num: I, denom: I) -> Self {
                Self { num, denom }
            }
            pub fn zero() -> Self {
                Self::new(I::zero(), I::one())
            }
            pub fn one() -> Self {
                Self::new(I::one(), I::one())
            }
            pub fn inf() -> Self {
                Self::new(I::one(), I::zero())
            }
            pub fn num(&self) -> I {
                self.num
            }
            pub fn denom(&self) -> I {
                self.denom
            }
            pub fn to_continued_fraction(&self) -> Vec<I> {
                assert!(self.denom() != I::zero());
                let (mut x, mut y) = (self.num(), self.denom());
                let mut res = Vec::new();
                while x > I::zero() && y > I::zero() {
                    let (q, r) = (x / y, x % y);
                    res.push(if r == I::zero() { q - I::one() } else { q });
                    (x, y) = (y, r);
                }
                res
            }
        }
        impl<T: PrimitiveInteger> Ord for Rational<T> {
            fn cmp(&self, other: &Self) -> std::cmp::Ordering {
                (self.num * other.denom).cmp(&(other.num * self.denom))
            }
        }
        impl<T: PrimitiveInteger> PartialOrd for Rational<T> {
            fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
                Some(self.cmp(other))
            }
        }
        impl<T: PrimitiveInteger> PartialEq for Rational<T> {
            fn eq(&self, other: &Self) -> bool {
                self.cmp(other).is_eq()
            }
        }
        impl<T: PrimitiveInteger> Eq for Rational<T> {}
        impl<T: PrimitiveInteger> Debug for Rational<T> {
            fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
                write!(f, "{:?}/{:?}", self.num, self.denom)
            }
        }
    }
    pub mod stern_brocot_tree {
        use crate::urectanc::{num_traits, rational};
        use num_traits::PrimitiveInteger;
        use rational::Rational;
        #[derive(Clone, Copy)]
        pub struct SternBrocotTree<T> {
            left: Rational<T>,
            right: Rational<T>,
        }
        impl<I: PrimitiveInteger> SternBrocotTree<I> {
            pub fn new(r: Rational<I>) -> Self {
                assert!(r.num() >= I::one() && r.denom() >= I::one());
                let path = r.to_continued_fraction();
                Self::from(path)
            }
            pub fn root() -> Self {
                Self {
                    left: Rational::zero(),
                    right: Rational::inf(),
                }
            }
            pub fn val(&self) -> Rational<I> {
                Rational::new(
                    self.left.num() + self.right.num(),
                    self.left.denom() + self.right.denom(),
                )
            }
            pub fn lower_bound(&self) -> Rational<I> {
                self.left
            }
            pub fn upper_bound(&self) -> Rational<I> {
                self.right
            }
            pub fn path(&self) -> Vec<I> {
                self.val().to_continued_fraction()
            }
            pub fn nth_left(&self, n: I) -> Self {
                assert!(n >= I::zero());
                let right = Rational::new(
                    self.right.num() + self.left.num() * n,
                    self.right.denom() + self.left.denom() * n,
                );
                Self { left: self.left, right }
            }
            pub fn nth_right(&self, n: I) -> Self {
                assert!(n >= I::zero());
                let left = Rational::new(
                    self.left.num() + self.right.num() * n,
                    self.left.denom() + self.right.denom() * n,
                );
                Self { left, right: self.right }
            }
            pub fn binary_search(f: impl Fn(Rational<I>) -> bool, n: I) -> Self {
                assert!(n > I::zero());
                let go = |node: &Self, d: I, to_left: bool| {
                    if to_left { node.nth_left(d) } else { node.nth_right(d) }
                };
                let over = |node: &Self, to_left: bool| {
                    let v = node.val();
                    v.num() > n || v.denom() > n || f(v) == to_left
                };
                let mut node = Self::root();
                let mut to_left = over(&node, false);
                loop {
                    let (mut ok, mut ng) = (I::zero(), I::one());
                    while !over(&go(&node, ng, to_left), to_left) {
                        (ok, ng) = (ng, ng + ng);
                    }
                    while ng - ok > I::one() {
                        let mid = ok.midpoint(ng);
                        if over(&go(&node, mid, to_left), to_left) {
                            ng = mid;
                        } else {
                            ok = mid;
                        }
                    }
                    node = go(&node, ng, to_left);
                    let v = node.val();
                    if v.num() > n || v.denom() > n {
                        return node;
                    }
                    to_left ^= true;
                }
            }
        }
        impl<I, T> From<T> for SternBrocotTree<I>
        where
            T: AsRef<[I]>,
            I: PrimitiveInteger,
        {
            fn from(value: T) -> Self {
                let path = value.as_ref();
                let mut node = Self::root();
                for (i, &a) in path.iter().enumerate() {
                    node = if i % 2 == 0 { node.nth_right(a) } else { node.nth_left(a) };
                }
                node
            }
        }
    }
    pub mod num_traits {
        use std::{
            fmt::Debug,
            ops::{
                Add, AddAssign, Div, DivAssign, Mul, MulAssign, Rem, RemAssign, Sub,
                SubAssign,
            },
        };
        pub trait PrimitiveInteger: 'static + Copy + Ord + Debug + Add<
                Output = Self,
            > + Sub<
                Output = Self,
            > + Mul<
                Output = Self,
            > + Div<
                Output = Self,
            > + Rem<
                Output = Self,
            > + AddAssign + SubAssign + MulAssign + DivAssign + RemAssign {
            fn midpoint(self, rhs: Self) -> Self;
            fn rem_euclid(self, rhs: Self) -> Self;
            fn zero() -> Self;
            fn one() -> Self;
            fn min_value() -> Self;
            fn max_value() -> Self;
        }
        macro_rules! impl_primitive_integer {
            ($($ty:ty),*) => {
                $(impl PrimitiveInteger for $ty { fn midpoint(self, rhs : Self) -> Self {
                self.midpoint(rhs) } fn rem_euclid(self, rhs : Self) -> Self { self
                .rem_euclid(rhs) } fn zero() -> Self { 0 } fn one() -> Self { 1 } fn
                min_value() -> Self { Self::MIN } fn max_value() -> Self { Self::MAX }
                })*
            };
        }
        impl_primitive_integer!(
            i8, i16, i32, i64, i128, isize, u8, u16, u32, u64, u128, usize
        );
    }
}
