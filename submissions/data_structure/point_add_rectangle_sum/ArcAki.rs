#![allow(clippy::print_literal, clippy::needless_doctest_main)]
#[allow(unused)]
mod __mini_proconio {
    use std::any::type_name;
    use std::fmt::Debug;
    use std::io::{self, BufRead, BufReader, Read, Stdin};
    use std::iter::Peekable;
    use std::ptr::NonNull;
    use std::str::{FromStr, SplitWhitespace};
    use std::sync::{Mutex, OnceLock};

    pub(super) struct Tokens {
        tokens: Peekable<SplitWhitespace<'static>>,
        _ctx: CurrentContext,
    }
    impl Tokens {
        pub(super) fn next_token(&mut self) -> Option<&str> { self.tokens.next() }
        pub(super) fn is_empty(&mut self) -> bool { self.tokens.peek().is_none() }
    }
    impl From<String> for Tokens {
        fn from(s: String) -> Self {
            let ctx = CurrentContext::from(s);
            let it = unsafe { ctx.0.as_ref().split_whitespace().peekable() };
            Self { tokens: it, _ctx: ctx }
        }
    }
    unsafe impl Send for Tokens {}
    unsafe impl Sync for Tokens {}
    struct CurrentContext(NonNull<str>);
    impl From<String> for CurrentContext {
        fn from(s: String) -> Self {
            let b = s.into_boxed_str();
            Self(NonNull::new(Box::leak(b)).unwrap())
        }
    }
    impl Drop for CurrentContext {
        fn drop(&mut self) {
            unsafe { drop(Box::from_raw(self.0.as_mut())) };
        }
    }

    pub mod source {
        use super::*;
        pub mod line {
            use super::*;
            pub struct LineSource<R: BufRead> {
                tokens: Tokens,
                reader: R,
            }
            impl<R: BufRead> LineSource<R> {
                pub fn new(reader: R) -> Self {
                    Self { tokens: "".to_owned().into(), reader }
                }
                fn prepare(&mut self) {
                    while self.tokens.is_empty() {
                        let mut line = String::new();
                        let n = self.reader.read_line(&mut line)
                            .expect("failed to read line");
                        if n == 0 { return; }
                        self.tokens = line.into();
                    }
                }
            }
            impl<R: BufRead> Source<R> for LineSource<R> {
                fn next_token(&mut self) -> Option<&str> {
                    self.prepare();
                    self.tokens.next_token()
                }
                fn is_empty(&mut self) -> bool {
                    self.prepare();
                    self.tokens.is_empty()
                }
            }
            impl<'a> From<&'a str> for LineSource<BufReader<&'a [u8]>> {
                fn from(s: &'a str) -> Self {
                    Self::new(BufReader::new(s.as_bytes()))
                }
            }
        }

        pub mod once {
            use super::*;
            use std::marker::PhantomData;
            pub struct OnceSource<R: BufRead> {
                tokens: Tokens,
                _read: PhantomData<R>,
            }
            impl<R: BufRead> OnceSource<R> {
                pub fn new(mut r: R) -> Self {
                    let mut s = String::new();
                    r.read_to_string(&mut s).expect("failed to read");
                    Self { tokens: s.into(), _read: PhantomData }
                }
            }
            impl<R: BufRead> Source<R> for OnceSource<R> {
                fn next_token(&mut self) -> Option<&str> { self.tokens.next_token() }
                fn is_empty(&mut self) -> bool { self.tokens.is_empty() }
            }
            impl<'a> From<&'a str> for OnceSource<BufReader<&'a [u8]>> {
                fn from(s: &'a str) -> Self { Self::new(BufReader::new(s.as_bytes())) }
            }
        }

        pub mod auto {
            #[cfg(debug_assertions)]
            pub use super::line::LineSource as AutoSource;
            #[cfg(not(debug_assertions))]
            pub use super::once::OnceSource as AutoSource;
        }

        pub trait Source<R: BufRead> {
            fn next_token(&mut self) -> Option<&str>;
            #[allow(clippy::wrong_self_convention)]
            fn is_empty(&mut self) -> bool;
            fn next_token_unwrap(&mut self) -> &str {
                self.next_token().expect(
                    "no more tokens (input exhausted or format mismatch)")
            }
        }

        pub trait Readable {
            type Output;
            fn read<R: BufRead, S: Source<R>>(source: &mut S) -> Self::Output;
        }
        impl<T: FromStr> Readable for T where T::Err: Debug {
            type Output = T;
            fn read<R: BufRead, S: Source<R>>(source: &mut S) -> T {
                let tk = source.next_token_unwrap();
                match tk.parse() {
                    Ok(v) => v,
                    Err(e) => panic!(
                        "failed to parse `{}` as {}: {:?}",
                        tk, type_name::<T>(), e
                    ),
                }
            }
        }

        pub trait RuntimeReadable {
            type Output;
            fn read<R: BufRead, S: Source<R>>(self, source: &mut S) -> Self::Output;
        }
    }

    pub mod marker {
        use super::source::{Readable, Source};
        use std::io::BufRead;

        pub enum Chars {}
        impl Readable for Chars {
            type Output = Vec<char>;
            fn read<R: BufRead, S: Source<R>>(s: &mut S) -> Vec<char> {
                s.next_token_unwrap().chars().collect()
            }
        }

        pub enum Bytes {}
        impl Readable for Bytes {
            type Output = Vec<u8>;
            fn read<R: BufRead, S: Source<R>>(s: &mut S) -> Vec<u8> {
                s.next_token_unwrap().bytes().collect()
            }
        }

        pub enum Usize1 {}
        impl Readable for Usize1 {
            type Output = usize;
            fn read<R: BufRead, S: Source<R>>(s: &mut S) -> usize {
                usize::read(s).checked_sub(1)
                    .expect("attempted to read 0 as Usize1")
            }
        }
    }

    pub use source::{Readable as __Readable, RuntimeReadable};
    pub mod source_pub { pub use super::source::{self, auto::AutoSource, line::LineSource}; }

    pub enum StdinSource<R: BufRead> {
            Normal(source::auto::AutoSource<R>),
            Interactive(source::line::LineSource<R>),
            Unknown(source::line::LineSource<R>),
        }
    impl<R: BufRead> source::Source<R> for StdinSource<R> {
        fn next_token(&mut self) -> Option<&str> {
            match self {
                Self::Normal(s) => s.next_token(),
                Self::Interactive(s) | Self::Unknown(s) => s.next_token(),
            }
        }
        fn is_empty(&mut self) -> bool {
            match self {
                Self::Normal(s) => s.is_empty(),
                Self::Interactive(s) | Self::Unknown(s) => s.is_empty(),
            }
        }
    }

    pub static STDIN_SOURCE: OnceLock<Mutex<StdinSource<BufReader<Stdin>>>> = OnceLock::new();

    #[doc(hidden)]
    pub fn __acquire_global_stdin_lock() -> std::sync::MutexGuard<'static, StdinSource<BufReader<Stdin>>> {
        STDIN_SOURCE.get_or_init(|| {
            Mutex::new(StdinSource::Unknown(source::line::LineSource::new(
                BufReader::new(io::stdin()),
            )))
        }).lock().expect("failed to lock stdin")
    }

    #[macro_export]
    macro_rules! input {
        (@from [$src:expr] @rest) => {};
        (@from [$src:expr] @rest mut $($rest:tt)*) => {
            $crate::input! { @from [$src] @mut [mut] @rest $($rest)* }
        };
        (@from [$src:expr] @rest $($rest:tt)*) => {
            $crate::input! { @from [$src] @mut [] @rest $($rest)* }
        };
        (@from [$src:expr] @mut [$($m:tt)?] @rest $var:tt: $($rest:tt)*) => {
            $crate::input! { @from [$src] @mut [$($m)*] @var $var @kind [] @rest $($rest)* }
        };
        (@from [$src:expr] @mut [$($m:tt)?] @var $var:tt @kind [$($kind:tt)*] @rest) => {
            let $($m)* $var = $crate::read_value!(@source [$src] @kind [$($kind)*]);
        };
        (@from [$src:expr] @mut [$($m:tt)?] @var $var:tt @kind [$($kind:tt)*] @rest, $($rest:tt)*) => {
            $crate::input!(@from [$src] @mut [$($m)*] @var $var @kind [$($kind)*] @rest);
            $crate::input!(@from [$src] @rest $($rest)*);
        };
        (@from [$src:expr] @mut [$($m:tt)?] @var $var:tt @kind [] @rest [$($tt:tt)*] $($rest:tt)*) => {
            $crate::input!(@from [$src] @mut [$($m)*] @var $var @kind [[$($tt)*]] @rest $($rest)*);
        };
        (@from [$src:expr] @mut [$($m:tt)?] @var $var:tt @kind [] @rest ($($tt:tt)*) $($rest:tt)*) => {
            $crate::input!(@from [$src] @mut [$($m)*] @var $var @kind [($($tt)*)] @rest $($rest)*);
        };
        (@from [$src:expr] @mut [$($m:tt)?] @var $var:tt @kind [] @rest $ty:ty, $($rest:tt)*) => {
            $crate::input!(@from [$src] @mut [$($m)*] @var $var @kind [$ty] @rest, $($rest)*);
        };
        (@from [$src:expr] @mut [$($m:tt)?] @var $var:tt @kind [] @rest $ty:ty) => {
            $crate::input!(@from [$src] @mut [$($m)*] @var $var @kind [$ty] @rest);
        };
        (@from [$src:expr] @mut [$($m:tt)?] @var $var:tt @kind [] @rest with $($rest:tt)*) => {
            let $($m)* $var = $crate::read_value!(@source [$src] @dyn_kind [$($rest)*]);
        };
        (@from $($tt:tt)*) => {
            compile_error!("input! parse error (mini proconio)");
        };

        (from $src:expr, $($rest:tt)*) => {
            #[allow(unused_variables, unused_mut)]
            let mut __s = $src;
            $crate::input! { @from [&mut __s] @rest $($rest)* }
        };
        ($($rest:tt)*) => {
            let mut __lock = $crate::__mini_proconio::__acquire_global_stdin_lock();
            $crate::input! { @from [&mut *__lock] @rest $($rest)* }
            drop(__lock);
        };
    }

    #[macro_export]
    macro_rules! read_value {
        (@source [$src:expr] @kind [[$($kind:tt)*]]) => {
            $crate::read_value!(@array @source [$src] @kind [] @rest $($kind)*)
        };
        (@array @source [$src:expr] @kind [$($kind:tt)*] @rest) => {{
            let len = <usize as $crate::__mini_proconio::__Readable>::read($src);
            $crate::read_value!(@source [$src] @kind [[$($kind)*; len]])
        }};
        (@array @source [$src:expr] @kind [$($kind:tt)*] @rest ; $($rest:tt)*) => {
            $crate::read_value!(@array @source [$src] @kind [$($kind)*] @len [$($rest)*])
        };
        (@array @source [$src:expr] @kind [$($kind:tt)*] @rest $tt:tt $($rest:tt)*) => {
            $crate::read_value!(@array @source [$src] @kind [$($kind)* $tt] @rest $($rest)*)
        };
        (@array @source [$src:expr] @kind [$($kind:tt)*] @len [$($len:tt)*]) => {{
            let len = $($len)*;
            (0..len).map(|_| $crate::read_value!(@source [$src] @kind [$($kind)*])).collect::<Vec<_>>()
        }};

        (@source [$src:expr] @kind [($($kinds:tt)*)]) => {
            $crate::read_value!(@tuple @source [$src] @kinds [] @current [] @rest $($kinds)*)
        };
        (@tuple @source [$src:expr] @kinds [$([$($kind:tt)*])*] @current [] @rest) => {
            ( $($crate::read_value!(@source [$src] @kind [$($kind)*]),)* )
        };
        (@tuple @source [$src:expr] @kinds [$($kinds:tt)*] @current [$($curr:tt)*] @rest) => {
            $crate::read_value!(@tuple @source [$src] @kinds [$($kinds)* [$($curr)*]] @current [] @rest)
        };
        (@tuple @source [$src:expr] @kinds [$($kinds:tt)*] @current [$($curr:tt)*] @rest, $($rest:tt)*) => {
            $crate::read_value!(@tuple @source [$src] @kinds [$($kinds)* [$($curr)*]] @current [] @rest $($rest)*)
        };
        (@tuple @source [$src:expr] @kinds [$($kinds:tt)*] @current [$($curr:tt)*] @rest $tt:tt $($rest:tt)*) => {
            $crate::read_value!(@tuple @source [$src] @kinds [$($kinds)*] @current [$($curr)* $tt] @rest $($rest)*)
        };

        (@source [$src:expr] @kind [$ty:ty]) => {
            <$ty as $crate::__mini_proconio::__Readable>::read($src)
        };

        (@source [$src:expr] @dyn_kind [$dyn:expr]) => {
            $crate::__mini_proconio::RuntimeReadable::read($dyn, $src)
        };

        (from $src:expr, with $($rest:tt)*) => {{
            #[allow(unused_mut)]
            let mut __s = $src;
            $crate::read_value!(@source [&mut __s] @dyn_kind [$($rest)*])
        }};
        (from $src:expr, $($rest:tt)*) => {{
            #[allow(unused_mut)]
            let mut __s = $src;
            $crate::read_value!(@source [&mut __s] @kind [$($rest)*])
        }};
        ($($rest:tt)*) => {{
            let mut __lock = $crate::__mini_proconio::STDIN_SOURCE
                .get_or_init(|| {
                    std::sync::Mutex::new($crate::__mini_proconio::StdinSource::Normal(
                        $crate::__mini_proconio::source_pub::auto::AutoSource::new(
                            std::io::BufReader::new(std::io::stdin())
                        ),
                    ))
                })
                .lock()
                .expect("failed to lock stdin");
            let __res = $crate::read_value!(from &mut *__lock, $($rest)*);
            drop(__lock);
            __res
        }};
    }

    #[macro_export]
    macro_rules! input_interactive {
        ($($rest:tt)*) => {
            let mut __lock = $crate::__mini_proconio::__acquire_global_stdin_lock();
            $crate::input! { from &mut *__lock, $($rest)* }
            drop(__lock);
        };
    }
    #[macro_export]
    macro_rules! read_value_interactive {
        ($($rest:tt)*) => {{
            let mut __lock = $crate::__mini_proconio::__acquire_global_stdin_lock();
            let __res = $crate::read_value!(from &mut *__lock, $($rest)*);
            drop(__lock);
            __res
        }};
    }
}
#[allow(unused_imports)]
use __mini_proconio as proconio_;
#[allow(unused_imports)]
use proconio_::marker::*;

pub type ModInt1000000007 = StaticModInt<Mod1000000007>;
pub type ModInt998244353 = StaticModInt<Mod998244353>;

#[derive(Copy, Clone, Debug, Eq, PartialEq)]
#[repr(transparent)]
pub struct StaticModInt<M> {
    val: u32,
    phantom: PhantomData<fn() -> M>,
}

pub trait Modulus: 'static + Copy + Eq {
    const VALUE: u32;
}

#[derive(Copy, Clone, Ord, PartialOrd, Eq, PartialEq, Debug)]
pub enum Mod1000000007 {}
impl Modulus for Mod1000000007 {
    const VALUE: u32 = 1_000_000_007;
}

#[derive(Copy, Clone, Ord, PartialOrd, Eq, PartialEq, Debug)]
pub enum Mod998244353 {}
impl Modulus for Mod998244353 {
    const VALUE: u32 = 998_244_353;
}

impl<M: Modulus> StaticModInt<M> {
    #[inline(always)]
    pub fn modulus() -> u32 {
        M::VALUE
    }

    #[inline]
    pub fn new<T: Into<i128>>(v: T) -> Self {
        let m = M::VALUE as i128;
        let mut x = v.into() % m;
        if x < 0 { x += m; }
        Self::raw(x as u32)
    }

    #[inline]
    pub fn raw(val: u32) -> Self {
        Self { val, phantom: PhantomData }
    }

    #[inline]
    pub fn val(self) -> u32 {
        self.val
    }

    #[inline]
    pub fn pow(self, mut n: u64) -> Self {
        let mut x = self;
        let mut r = if Self::modulus() > 1 { Self::raw(1) } else { Self::raw(0) };
        while n > 0 {
            if n & 1 == 1 { r *= x; }
            x *= x;
            n >>= 1;
        }
        r
    }

    #[inline]
    pub fn inv(self) -> Self {
        assert!(self.val() != 0, "attempt to divide by zero");
        self.pow((M::VALUE as u64) - 2)
    }
}

impl<M: Modulus> Neg for StaticModInt<M> {
    type Output = Self;
    #[inline]
    fn neg(self) -> Self {
        if self.val == 0 { self } else { Self::raw(M::VALUE - self.val) }
    }
}

impl<M: Modulus> Add for StaticModInt<M> {
    type Output = Self;
    #[inline]
    fn add(self, rhs: Self) -> Self {
        let mut v = self.val + rhs.val;
        let m = M::VALUE;
        if v >= m { v -= m; }
        Self::raw(v)
    }
}

impl<M: Modulus> Sub for StaticModInt<M> {
    type Output = Self;
    #[inline]
    fn sub(self, rhs: Self) -> Self {
        let m = M::VALUE;
        let mut v = self.val.wrapping_sub(rhs.val);
        if v >= m { v = v.wrapping_add(m); }
        Self::raw(v)
    }
}

impl<M: Modulus> Mul for StaticModInt<M> {
    type Output = Self;
    #[inline]
    fn mul(self, rhs: Self) -> Self {
        let m = M::VALUE as u64;
        let v = (self.val as u64) * (rhs.val as u64) % m;
        Self::raw(v as u32)
    }
}

impl<M: Modulus> Div for StaticModInt<M> {
    type Output = Self;
    #[inline]
    fn div(self, rhs: Self) -> Self {
        self * rhs.inv()
    }
}

impl<M: Modulus> AddAssign for StaticModInt<M> {
    #[inline]
    fn add_assign(&mut self, rhs: Self) { *self = *self + rhs; }
}
impl<M: Modulus> SubAssign for StaticModInt<M> {
    #[inline]
    fn sub_assign(&mut self, rhs: Self) { *self = *self - rhs; }
}
impl<M: Modulus> MulAssign for StaticModInt<M> {
    #[inline]
    fn mul_assign(&mut self, rhs: Self) { *self = *self * rhs; }
}
impl<M: Modulus> DivAssign for StaticModInt<M> {
    #[inline]
    fn div_assign(&mut self, rhs: Self) { *self = *self / rhs; }
}
impl<M: Modulus> From<i64> for StaticModInt<M> {
    #[inline]
    fn from(v: i64) -> Self { Self::new(v) }
}
impl<M: Modulus> From<u64> for StaticModInt<M> {
    #[inline]
    fn from(v: u64) -> Self { Self::new(v) }
}
impl<M: Modulus> From<usize> for StaticModInt<M> {
    #[inline]
    fn from(v: usize) -> Self { Self::new(v as u64) }
}

impl<M: Modulus> FromStr for StaticModInt<M> {
    type Err = <i128 as FromStr>::Err;

    #[inline]
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let v = s.parse::<i128>()?;
        Ok(Self::new(v))
    }
}

macro_rules! impl_ops_with_prim {
    ($($t:ty),* $(,)?) => {
        $(
            impl<M: Modulus> Add<$t> for StaticModInt<M> {
                type Output = Self;
                #[inline] fn add(self, rhs: $t) -> Self { self + Self::new(rhs as i128) }
            }
            impl<M: Modulus> Sub<$t> for StaticModInt<M> {
                type Output = Self;
                #[inline] fn sub(self, rhs: $t) -> Self { self - Self::new(rhs as i128) }
            }
            impl<M: Modulus> Mul<$t> for StaticModInt<M> {
                type Output = Self;
                #[inline] fn mul(self, rhs: $t) -> Self { self * Self::new(rhs as i128) }
            }
            impl<M: Modulus> Div<$t> for StaticModInt<M> {
                type Output = Self;
                #[inline] fn div(self, rhs: $t) -> Self { self / Self::new(rhs as i128) }
            }

            impl<M: Modulus> AddAssign<$t> for StaticModInt<M> {
                #[inline] fn add_assign(&mut self, rhs: $t) { *self = *self + rhs; }
            }
            impl<M: Modulus> SubAssign<$t> for StaticModInt<M> {
                #[inline] fn sub_assign(&mut self, rhs: $t) { *self = *self - rhs; }
            }
            impl<M: Modulus> MulAssign<$t> for StaticModInt<M> {
                #[inline] fn mul_assign(&mut self, rhs: $t) { *self = *self * rhs; }
            }
            impl<M: Modulus> DivAssign<$t> for StaticModInt<M> {
                #[inline] fn div_assign(&mut self, rhs: $t) { *self = *self / rhs; }
            }
        )*
    }
}

impl_ops_with_prim!(i32, i64, u32, usize);

#[allow(unused_imports)]
use std::{
    convert::{Infallible, TryFrom, TryInto as _}, fmt::{self, Debug, Display, Formatter,},
    fs::File, hash::{Hash, Hasher, BuildHasherDefault}, iter::{Product, Sum}, marker::PhantomData,
    ops::{Add, AddAssign, Sub, SubAssign, Div, DivAssign, Mul, MulAssign, Neg, RangeBounds},
    str::FromStr, sync::{atomic::{self, AtomicU32, AtomicU64}, Once}, ptr::null_mut,
    collections::{*, btree_set::Range, btree_map::Range as BTreeRange}, mem::swap,
    cmp::{self, Reverse, Ordering, Eq, PartialEq, PartialOrd},
    thread::LocalKey, f64::consts::PI, time::Instant, cell::RefCell,
    io::{self, stdin, Read, read_to_string, BufWriter, BufReader, stdout, Write},
};
pub trait SortD{ fn sort_d(&mut self); }
impl<T: Ord> SortD for Vec<T>{ fn sort_d(&mut self) {
    self.sort_by(|u, v| v.cmp(&u));
} }
pub trait Mx{fn max(&self, rhs: Self)->Self;}
impl Mx for f64{ fn max(&self, rhs: Self)->Self{if *self < rhs{ rhs } else { *self } }}
pub trait Mi{ fn min(&self, rhs: Self)->Self; }
impl Mi for f64{ fn min(&self, rhs: Self)->Self{ if *self > rhs{ rhs } else { *self } } }
#[inline]
pub fn gcd(mut a: i64, mut b: i64)->i64{ if b==0{return a;}(a,b)=(a.abs(),b.abs());while b!=0{ let c = a;a = b;b = c%b; }a }
#[inline]
pub fn factorial_i64(n: usize)->(Vec<i64>, Vec<i64>){ let mut res = vec![1; n+1];let mut inv = vec![1; n+1];for i in 0..n{ res[i+1] = (res[i]*(i+1)as i64)%MOD; }inv[n] = mod_inverse(res[n], MOD);for i in (0..n).rev(){ inv[i] = inv[i+1]*(i+1) as i64%MOD; }(res, inv) }
#[inline]
pub fn floor(a:i64, b:i64)->i64{let res=(a%b+b)%b;(a-res)/b}
#[inline]
pub fn extended_gcd(a:i64,b:i64)->(i64,i64,i64)
{if b==0{(a,1,0)}else{let(g,x,y)=extended_gcd(b,a%b);(g,y,x-floor(a,b)*y)}}
#[inline]
pub fn mod_inverse(a:i64,m:i64)->i64{let(_,x,_) =extended_gcd(a,m);(x%m+m)%m}
#[inline]
pub fn comb(a: i64, b: i64, f: &Vec<(i64, i64)>)->i64{
    if a<b{return 0;}else if b==0 || a==b{ return 1; }
    else{let x=f[a as usize].0;
        let y=f[(a-b) as usize].1;let z=f[b as usize].1;return((x*y)%MOD)*z%MOD;}}
#[inline]
pub fn factorial(x: i64)->Vec<(i64, i64)>{
    let mut f=vec![(1i64,1i64),(1, 1)];let mut z = 1i64;
    let mut inv = vec![0; x as usize+10];inv[1] = 1;
    for i in 2..x+1{z=(z*i)%MOD;
        let w=(MOD-inv[(MOD%i)as usize]*(MOD/i)%MOD)%MOD;
        inv[i as usize] = w;
        f.push((z, (f[i as usize-1].1*w)%MOD));}return f;}
//pub fn fast_mod_pow(x: i64,p: usize, m: i64)->i64{let mut res=1;let mut t=x;let mut z=p;while z > 0{if z%2==1{res = (res*t)%m;}t = (t*t)%m;z /= 2; }res}
pub fn vec_out<T>(v: Vec<T>) where T: ToString{ println!("{}", v.iter().map(|x| x.to_string()).collect::<Vec<_>>().join(" ")); }
pub fn one_add_out(v: Vec<usize>){println!("{}", v.iter().map(|x| (x+1).to_string()).collect::<Vec<_>>().join(" "));}
pub trait Chmax{
    fn chmax(&mut self, rhs: Self);
}
impl Chmax for i64{
    fn chmax(&mut self, rhs: Self) {
        *self = (*self).max(rhs)
    }
}
impl Chmax for i32{
    fn chmax(&mut self, rhs: Self) {
        *self = (*self).max(rhs)
    }
}
impl Chmax for f64{
    fn chmax(&mut self, rhs: Self) {
        *self = (*self).max(rhs)
    }
}
impl Chmax for usize{
    fn chmax(&mut self, rhs: Self) {
        *self = (*self).max(rhs)
    }
}
pub trait Chmin {
    fn chmin(&mut self, rhs: Self);
}
impl Chmin for i64{
    fn chmin(&mut self, rhs: Self) {
        *self = (*self).min(rhs)
    }
}
impl Chmin for i32{
    fn chmin(&mut self, rhs: Self) {
        *self = (*self).min(rhs)
    }
}
impl Chmin for f64{
    fn chmin(&mut self, rhs: Self) {
        *self = (*self).min(rhs)
    }
}
impl Chmin for usize{
    fn chmin(&mut self, rhs: Self) {
        *self = (*self).min(rhs)
    }
}
#[derive(Debug, Clone)]
pub struct Counter<T: Ord>{
    c: usize,
    map: BTreeMap<T, usize>,
}

impl<T: Copy+Ord> Counter<T>{
    pub fn new()->Self{
        Counter{
            c: 0,
            map: BTreeMap::new(),
        }
    }

    #[inline(always)]
    pub fn range<R>(&self, range: R)->BTreeRange<'_, T, usize> where R: RangeBounds<T>{
        self.map.range(range)
    }

    #[inline(always)]
    pub fn mi(&self)->Option<T>{
        if let Some((x, _)) = self.range(..).next(){
            Some(*x)
        } else {
            None
        }
    }

    #[inline(always)]
    pub fn mx(&self)->Option<T>{
        if let Some((x, _)) = self.range(..).next_back(){
            Some(*x)
        } else {
            None
        }
    }

    #[inline(always)]
    pub fn one_add(&mut self, x: T){
        *self.map.entry(x).or_insert(0) += 1;
        self.c += 1;
    }

    #[inline(always)]
    pub fn one_sub(&mut self, x: T){
        if !self.map.contains_key(&x){return}
        let e = self.map.entry(x).or_insert(0);
        *e = e.saturating_sub(1);
        if self.map[&x] <= 0{
            self.map.remove(&x);
        }
        self.c = self.c.saturating_sub(1);
    }

    #[inline(always)]
    pub fn one_update(&mut self, x: T, y: T){
        self.one_sub(x);
        self.one_add(y);
    }

    #[inline(always)]
    pub fn del(&mut self, x: T){
        self.c = self.c.saturating_sub(*self.map.get(&x).unwrap_or(&0));
        self.map.remove(&x);
    }

    #[inline(always)]
    pub fn add(&mut self, x: T, c: usize){
        *self.map.entry(x).or_insert(0) += c;
        self.c += c;
    }

    #[inline(always)]
    pub fn sub(&mut self, x: T, c: usize){
        let e = self.map.entry(x).or_insert(0);
        *e = e.saturating_sub(c);
        if self.map[&x] == 0{
            self.map.remove(&x);
        }
        self.c = self.c.saturating_sub(c);
    }

    #[inline(always)]
    pub fn include(&self, x: T)->bool{
        self.map.contains_key(&x)
    }

    #[inline(always)]
    pub fn cnt(&self, x: T)->usize{
        *self.map.get(&x).unwrap_or(&0)
    }

    #[inline(always)]
    pub fn is_empty(&self)->bool{
        self.map.is_empty()
    }

    #[inline(always)]
    pub fn len(&self)->usize{
        self.map.len()
    }

    #[inline(always)]
    pub fn clear(&mut self){
        self.map.clear();
        self.c = 0;
    }

    #[inline(always)]
    pub fn merge(&mut self, rhs: &mut Counter<T>){
        if self.len() < rhs.len(){
            swap(self, rhs);
        }
        for (&k, &v) in rhs.map.iter(){
            self.add(k, v);
        }
        rhs.clear();
    }
}

pub mod fxhash {
    use std::hash::BuildHasherDefault;

    const K: u64 = 0x517c_c1b7_2722_0a95;

    #[derive(Default)]
    pub struct FxHasher {
        pub hash: u64,
    }
    impl FxHasher {
        #[inline(always)]
        fn mix_u64(mut h: u64, x: u64) -> u64 {
            // 軽量2ラウンド（1ラウンドより偏りが出にくい）
            h = h.rotate_left(5) ^ x;
            h = h.wrapping_mul(K);

            let x2 = x ^ (x >> 33) ^ (x << 11);
            h = h.rotate_left(5) ^ x2;
            h = h.wrapping_mul(K);

            h
        }
        #[inline(always)]
        fn write_u64_impl(&mut self, x: u64) {
            self.hash = Self::mix_u64(self.hash, x);
        }
    }
    impl std::hash::Hasher for FxHasher {
        #[inline(always)]
        fn finish(&self) -> u64 {
            self.hash
        }
        #[inline(always)]
        fn write(&mut self, bytes: &[u8]) {
            let mut h = self.hash;
            for &b in bytes {
                h = h.rotate_left(5) ^ (b as u64);
                h = h.wrapping_mul(K);
            }
            self.hash = h;
        }
        #[inline(always)]
        fn write_u64(&mut self, i: u64) { self.write_u64_impl(i); }
        #[inline(always)]
        fn write_u32(&mut self, i: u32) { self.write_u64_impl(i as u64); }
        #[inline(always)]
        fn write_u16(&mut self, i: u16) { self.write_u64_impl(i as u64); }
        #[inline(always)]
        fn write_u8 (&mut self, i: u8 ) { self.write_u64_impl(i as u64); }
        #[inline(always)]
        fn write_usize(&mut self, i: usize) { self.write_u64_impl(i as u64); }
        #[inline(always)]
        fn write_i64(&mut self, i: i64) { self.write_u64_impl(i as u64); }
        #[inline(always)]
        fn write_i32(&mut self, i: i32) { self.write_u64_impl(i as u64); }
        #[inline(always)]
        fn write_i16(&mut self, i: i16) { self.write_u64_impl(i as u64); }
        #[inline(always)]
        fn write_i8 (&mut self, i: i8 ) { self.write_u64_impl(i as u64); }
        #[inline(always)]
        fn write_isize(&mut self, i: isize) { self.write_u64_impl(i as u64); }
    }

    pub type FxBuildHasher = BuildHasherDefault<FxHasher>;
    pub type FxMap<K, V> = std::collections::HashMap<K, V, FxBuildHasher>;
    pub type FxSet<K> = std::collections::HashSet<K, FxBuildHasher>;
}
#[allow(unused_imports)]
use fxhash::{FxSet, FxMap, FxBuildHasher};
#[allow(dead_code)]
const INF: i64 = 1<<60;
#[allow(dead_code)]
const I: i32 = 1<<30;
#[allow(dead_code)]
const MOD: i64 = 998244353;
#[allow(dead_code)]
const D: [(usize, usize); 4] = [(1, 0), (0, 1), (!0, 0), (0, !0)];
#[allow(dead_code)]
const D2: [(usize, usize); 8] = [(1, 0), (1, 1), (0, 1), (!0, 1), (!0, 0), (!0, !0), (0, !0), (1, !0)];
#[allow(dead_code)]
type MI = ModInt998244353;

// [0, r) の半開区間
pub struct BIT<T>
where
    T: Copy
        + std::ops::Add<Output = T>
        + std::ops::Sub<Output = T>
        + PartialOrd,
{
    n: usize,
    vec: Vec<T>,
    zero: T,
}

impl<T> BIT<T>
where
    T: Copy
        + std::ops::Add<Output = T>
        + std::ops::Sub<Output = T>
        + PartialOrd,
{
    pub fn new(n: usize, zero: T) -> Self {
        let k = n.max(1);
        let base = vec![zero; k + 1];
        BIT { n: k, vec: base, zero }
    }

    #[inline]
    pub fn add(&mut self, mut idx: usize, x: T) {
        idx += 1;
        while idx <= self.n {
            self.vec[idx] = self.vec[idx] + x;
            idx += idx & (!idx + 1);
        }
    }

    #[inline]
    pub fn g(&self, mut r: usize) -> T {
        let mut res = self.zero;
        while r > 0 {
            res = res + self.vec[r];
            r -= r & (!r + 1);
        }
        res
    }

    #[inline]
    pub fn prod(&self, l: usize, r: usize) -> T {
        self.g(r) - self.g(l)
    }
}

#[derive(Clone, Copy, Debug)]
pub enum PointAddRectangleSumQuery {
    Add{
        x: i32, y: i32, w: i64,
    }, 
    Query{
        lx: i32, ly: i32, rx: i32, ry: i32,
    }
}

#[derive(Clone)]
pub struct PointAddRectangleSum {
    yn: usize, 
    data: Vec<PointAddRectangleSumQuery>,
}

impl PointAddRectangleSum {
    pub fn new()->Self{
        PointAddRectangleSum { yn: 0, data: Vec::new() }
    }

    #[inline]
    pub fn push_add(&mut self, x: i32, y: i32, w: i64){
        self.data.push(PointAddRectangleSumQuery::Add{x, y, w});
        self.yn+=1;
    }

    // [lx, rx)×[ly, ry) なので注意
    #[inline]
    pub fn push_query(&mut self, lx: i32, ly: i32, rx: i32, ry: i32,){
        self.data.push(PointAddRectangleSumQuery::Query {lx, ly, rx, ry});
        self.yn+=2;
    }

    pub fn solve(&mut self)->Vec<i64>{
        let q = self.data.len();
        let mut ans = vec![0; q];
        let mut ys = Vec::with_capacity(self.yn);
        for x in self.data.iter(){
            match *x {
                PointAddRectangleSumQuery::Add { y, .. } => {
                    ys.push(y);
                }
                PointAddRectangleSumQuery::Query { ly, ry, .. } => {
                    ys.push(ly); ys.push(ry);
                }
            }
        }
        ys.sort_unstable();ys.dedup();
        for x in self.data.iter_mut(){
            match x {
                PointAddRectangleSumQuery::Add { y, .. } => {
                    *y = ys.binary_search(&*y).unwrap()as i32;
                }
                PointAddRectangleSumQuery::Query { ly, ry, .. } => {
                    *ly = ys.binary_search(&*ly).unwrap()as i32;
                    *ry = ys.binary_search(&*ry).unwrap()as i32;
                }
            }
        }
        let mut bit = BIT::new(ys.len(), 0);
        let mut seq = Vec::new();
        self.dfs(0, q, &mut bit, &mut seq, &mut ans);
        let mut res = Vec::new();
        for (i,&x) in self.data.iter().enumerate(){
            if matches!(x, PointAddRectangleSumQuery::Query{..}){
                res.push(ans[i]);
            }
        }
        self.data.clear();
        res
    }

    #[inline]
    fn dfs(&self, l: usize, r: usize, bit: &mut BIT<i64>, seq: &mut Vec<(i32, i32, i32, i64)>, ans: &mut Vec<i64>){
        if l+1==r {return;}
        let m = (l+r)>>1;
        let mut emp1 =true;
        for i in l..m {
            if let PointAddRectangleSumQuery::Add { x, y, w} = self.data[i] {
                seq.push((x, I, y, w));
                emp1=false;
            }
        }
        if emp1{seq.clear();}
        let mut emp2=true;
        for i in m..r {
            if let PointAddRectangleSumQuery::Query { lx, ly, rx, ry } = self.data[i]{
                if !emp1{
                    seq.push((lx, ly, ry, i as i64));
                    seq.push((rx, ly, ry, -(i as i64+1)));
                }
                emp2=false;
            }
        }
        if emp1||emp2{seq.clear();}
        seq.sort_unstable_by_key(|w| (w.0,w.1));
        for &(_, l, r, w) in seq.iter(){
            if l==I {
                bit.add(r as usize, w);
            } else if w < 0{
                ans[(-w-1) as usize] += bit.prod(l as usize, r as usize);
            } else {
                ans[w as usize] -= bit.prod(l as usize, r as usize);
            }
        }
        for &(_, l, r, w) in seq.iter(){
            if l==I {
                bit.add(r as usize, -w);
            }
        }
        seq.clear();
        if !emp1{self.dfs(l, m, bit, seq, ans)};
        if !emp2{self.dfs(m, r, bit, seq, ans)};
    }
}

const MULTI: bool = false;
pub fn solve(){
    let mut o = BufWriter::new(stdout().lock());
    input!{
        n: usize, q: usize,
    }
    let mut pars = PointAddRectangleSum::new();
    for _ in 0..n {
        input!{
            u: i32, v: i32, w: i64,
        }
        pars.push_add(u, v, w);
    }
    for _ in 0..q {
        input!{
            t: u8,
        }
        if t==0 {
            input!{
                x: i32, y: i32, w: i64,
            }
            pars.push_add(x, y, w);
        } else {
            input!{
                l: i32, d: i32, r: i32, u: i32,
            }
            pars.push_query(l, d, r, u);
        }
    }
    for x in pars.solve(){
        writeln!(o, "{}", x).ok();
    }
}

fn main() {
    if MULTI{
        input!{
            t: usize,
        }
        for _ in 0..t{
            solve();
        }
    } else {
        solve();
    }
}
