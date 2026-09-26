// Bundled at 2025/12/25 21:14:18 +09:00
// Author: Haar

pub mod main {
    use super::*;
    #[allow(unused_imports)]
    use haar_lib::{get, input, io::fastio::*, iter::join_str::*, output};
    #[allow(unused_imports)]
    use std::cell::{Cell, RefCell};
    #[allow(unused_imports)]
    use std::cmp::{max, min, Reverse};
    #[allow(unused_imports)]
    use std::collections::{BTreeMap, BTreeSet, BinaryHeap, HashMap, HashSet, VecDeque};
    #[allow(unused_imports)]
    use std::io::Write;
    #[allow(unused_imports)]
    use std::mem::swap;
    #[allow(unused_imports)]
    use std::rc::Rc;
    pub struct Problem {
        io: FastIO,
    }
    use haar_lib::graph::{matrix_tree::*, *};
    use haar_lib::math::prime_mod::*;
    impl Problem {
        pub fn init() -> Self {
            Self { io: FastIO::new() }
        }
        pub fn main(&mut self) {
            let n = self.io.read_usize();
            let m = self.io.read_usize();
            let r = self.io.read_usize();
            let mut g = Graph::<Directed, _>::new(n);
            for _ in 0..m {
                let u = self.io.read_usize();
                let v = self.io.read_usize();
                g.add(Edge::new(v, u, (), ()));
            }
            let ans = count_directed_spanning_tree::<Prime<998244353>>(&g, r);
            self.io.writeln(ans);
        }
    }
}
fn main() {
    std::thread::Builder::new()
        .spawn(|| main::Problem::init().main())
        .unwrap()
        .join()
        .unwrap()
}
use crate as haar_lib;
pub mod graph {
    pub mod matrix_tree {
        use crate::graph::*;
        use crate::linalg::mod_p::determinant::*;
        use crate::math::prime_mod::PrimeMod;
        use crate::num::const_modint::*;
        pub fn count_directed_spanning_tree<P: PrimeMod>(
            g: &Graph<Directed, impl EdgeTrait>,
            root: usize,
        ) -> ConstModInt<P> {
            let modulo = ConstModIntBuilder::<P>::new();
            let n = g.len();
            let mut lap = vec![vec![modulo.from_u64(0); n - 1]; n - 1];
            for e in g.nodes_iter().flatten() {
                let mut from = e.from();
                let mut to = e.to();
                if from == root {
                    continue;
                }
                if from > root {
                    from -= 1;
                }
                if to == root {
                    lap[from][from] += modulo.from_u64(1);
                    continue;
                }
                if to > root {
                    to -= 1;
                }
                lap[from][from] += modulo.from_u64(1);
                lap[from][to] -= modulo.from_u64(1);
            }
            determinant(lap)
        }
    }
    use std::marker::PhantomData;
    pub trait EdgeTrait {
        type Weight;
        fn from(&self) -> usize;
        fn to(&self) -> usize;
        fn weight(&self) -> Self::Weight;
        fn rev(self) -> Self;
    }
    #[derive(Debug, Clone, Hash, PartialEq, Eq)]
    pub struct Edge<T, I> {
        pub from: usize,
        pub to: usize,
        pub weight: T,
        pub index: I,
    }
    impl<T, I> Edge<T, I> {
        pub fn new(from: usize, to: usize, weight: T, index: I) -> Self {
            Self {
                from,
                to,
                weight,
                index,
            }
        }
    }
    impl<T: Clone, I> EdgeTrait for Edge<T, I> {
        type Weight = T;
        #[inline]
        fn from(&self) -> usize {
            self.from
        }
        #[inline]
        fn to(&self) -> usize {
            self.to
        }
        #[inline]
        fn weight(&self) -> Self::Weight {
            self.weight.clone()
        }
        fn rev(mut self) -> Self {
            std::mem::swap(&mut self.from, &mut self.to);
            self
        }
    }
    pub trait Direction {}
    #[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash, Default)]
    pub struct Directed;
    #[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash, Default)]
    pub struct Undirected;
    impl Direction for Directed {}
    impl Direction for Undirected {}
    #[derive(Clone, Debug)]
    pub struct GraphNode<E> {
        pub edges: Vec<E>,
    }
    impl<E: EdgeTrait> GraphNode<E> {
        pub fn neighbors(&self) -> impl DoubleEndedIterator<Item = &E> {
            self.edges.iter()
        }
        pub fn neighbors_size(&self) -> usize {
            self.edges.len()
        }
    }
    impl<E: EdgeTrait> IntoIterator for GraphNode<E> {
        type Item = E;
        type IntoIter = std::vec::IntoIter<Self::Item>;
        fn into_iter(self) -> Self::IntoIter {
            self.edges.into_iter()
        }
    }
    impl<'a, E: EdgeTrait> IntoIterator for &'a GraphNode<E> {
        type Item = &'a E;
        type IntoIter = std::slice::Iter<'a, E>;
        fn into_iter(self) -> Self::IntoIter {
            self.edges.iter()
        }
    }
    #[derive(Debug, Clone)]
    pub struct Graph<D, E> {
        nodes: Vec<GraphNode<E>>,
        __phantom: PhantomData<D>,
    }
    impl<D: Direction, E: EdgeTrait + Clone> Graph<D, E> {
        pub fn new(size: usize) -> Self {
            Self {
                nodes: vec![GraphNode { edges: vec![] }; size],
                __phantom: PhantomData,
            }
        }
    }
    impl<E: EdgeTrait + Clone> Graph<Directed, E> {
        pub fn add(&mut self, e: E) {
            self.nodes[e.from()].edges.push(e);
        }
    }
    impl<E: EdgeTrait + Clone> Extend<E> for Graph<Directed, E> {
        fn extend<T: IntoIterator<Item = E>>(&mut self, iter: T) {
            iter.into_iter().for_each(|e| self.add(e));
        }
    }
    impl<E: EdgeTrait + Clone> Graph<Undirected, E> {
        pub fn add(&mut self, e: E) {
            self.nodes[e.from()].edges.push(e.clone());
            self.nodes[e.to()].edges.push(e.rev());
        }
    }
    impl<E: EdgeTrait + Clone> Extend<E> for Graph<Undirected, E> {
        fn extend<T: IntoIterator<Item = E>>(&mut self, iter: T) {
            iter.into_iter().for_each(|e| self.add(e));
        }
    }
    impl<D, E> Graph<D, E> {
        pub fn nodes_iter(&self) -> impl Iterator<Item = &GraphNode<E>> {
            self.nodes.iter()
        }
        pub fn node_of(&self, i: usize) -> &GraphNode<E> {
            &self.nodes[i]
        }
        pub fn len(&self) -> usize {
            self.nodes.len()
        }
        pub fn is_empty(&self) -> bool {
            self.nodes.is_empty()
        }
    }
}
pub mod io {
    pub mod fastio {
        use std::fmt::Display;
        use std::io::{Read, Write};
        pub struct FastIO {
            in_bytes: Vec<u8>,
            in_cur: usize,
            out_buf: std::io::BufWriter<std::io::Stdout>,
        }
        impl FastIO {
            pub fn new() -> Self {
                let mut s = vec![];
                std::io::stdin().read_to_end(&mut s).unwrap();
                let cout = std::io::stdout();
                Self {
                    in_bytes: s,
                    in_cur: 0,
                    out_buf: std::io::BufWriter::new(cout),
                }
            }
            #[inline]
            pub fn getc(&mut self) -> Option<u8> {
                let c = *self.in_bytes.get(self.in_cur)?;
                self.in_cur += 1;
                Some(c)
            }
            #[inline]
            pub fn peek(&self) -> Option<u8> {
                Some(*self.in_bytes.get(self.in_cur)?)
            }
            #[inline]
            pub fn skip(&mut self) {
                while self.peek().is_some_and(|c| c.is_ascii_whitespace()) {
                    self.in_cur += 1;
                }
            }
            pub fn read_u64(&mut self) -> u64 {
                self.skip();
                let mut ret: u64 = 0;
                while self.peek().is_some_and(|c| c.is_ascii_digit()) {
                    ret = ret * 10 + (self.in_bytes[self.in_cur] - b'0') as u64;
                    self.in_cur += 1;
                }
                ret
            }
            pub fn read_u32(&mut self) -> u32 {
                self.read_u64() as u32
            }
            pub fn read_usize(&mut self) -> usize {
                self.read_u64() as usize
            }
            pub fn read_i64(&mut self) -> i64 {
                self.skip();
                let mut ret: i64 = 0;
                let minus = if self.peek() == Some(b'-') {
                    self.in_cur += 1;
                    true
                } else {
                    false
                };
                while self.peek().is_some_and(|c| c.is_ascii_digit()) {
                    ret = ret * 10 + (self.in_bytes[self.in_cur] - b'0') as i64;
                    self.in_cur += 1;
                }
                if minus {
                    ret = -ret;
                }
                ret
            }
            pub fn read_i32(&mut self) -> i32 {
                self.read_i64() as i32
            }
            pub fn read_isize(&mut self) -> isize {
                self.read_i64() as isize
            }
            pub fn read_f64(&mut self) -> f64 {
                self.read_chars()
                    .into_iter()
                    .collect::<String>()
                    .parse()
                    .unwrap()
            }
            pub fn read_chars(&mut self) -> Vec<char> {
                self.read_bytes().into_iter().map(Into::into).collect()
            }
            pub fn read_string(&mut self) -> String {
                self.read_chars().into_iter().collect()
            }
            pub fn read_bytes(&mut self) -> Vec<u8> {
                self.skip();
                let mut ret = vec![];
                while self.peek().is_some_and(|c| c.is_ascii_graphic()) {
                    ret.push(self.in_bytes[self.in_cur]);
                    self.in_cur += 1;
                }
                ret
            }
            pub fn read_char(&mut self) -> char {
                self.skip();
                self.getc().unwrap().into()
            }
            pub fn writeln_u32(&mut self, mut n: u32) {
                if n == 0 {
                    self.out_buf.write_all(b"0\n").unwrap();
                    return;
                }
                let mut buf = [b' '; 10];
                let mut i = 9;
                while n > 0 {
                    buf[i] = (n % 10) as u8 + b'0';
                    n /= 10;
                    i -= 1;
                }
                self.out_buf.write_all(&buf).unwrap();
                self.out_buf.write_all(b"\n").unwrap();
            }
            pub fn write<T: Display>(&mut self, s: T) {
                self.out_buf.write_all(s.to_string().as_bytes()).unwrap();
            }
            pub fn write_rev<T: Display>(&mut self, s: T) {
                let mut s = s.to_string().as_bytes().to_vec();
                s.reverse();
                self.out_buf.write_all(&s).unwrap();
            }
            pub fn writeln<T: Display>(&mut self, s: T) {
                self.write(s);
                self.out_buf.write_all(b"\n").unwrap();
            }
            pub fn writeln_rev<T: Display>(&mut self, s: T) {
                self.write_rev(s);
                self.out_buf.write_all(b"\n").unwrap();
            }
            pub fn write_bytes(&mut self, s: &[u8]) {
                self.out_buf.write_all(s).unwrap();
            }
            pub fn writeln_intersperse<I>(&mut self, it: I)
            where
                I: IntoIterator,
                I::Item: Display,
            {
                let mut it = it.into_iter();
                if let Some(s) = it.next() {
                    self.write(s);
                }
                for s in it {
                    self.write_bytes(b" ");
                    self.write(s);
                }
                self.write_bytes(b"\n");
            }
        }
        impl Drop for FastIO {
            fn drop(&mut self) {
                self.out_buf.flush().unwrap();
            }
        }
    }
}
pub mod iter {
    pub mod join_str {
        pub trait JoinStr: Iterator {
            fn join_str(self, s: &str) -> String
            where
                Self: Sized,
                Self::Item: ToString,
            {
                self.map(|x| x.to_string()).collect::<Vec<_>>().join(s)
            }
        }
        impl<I> JoinStr for I where I: Iterator + ?Sized {}
    }
}
pub mod linalg {
    pub mod mod_p {
        pub mod determinant {
            use crate::num::{ff::FFElem, one_zero::*};
            pub fn determinant<T>(mut a: Vec<Vec<T>>) -> T
            where
                T: FFElem + Copy + Zero + One,
            {
                let n = a.len();
                assert!(a.iter().all(|r| r.len() == n));
                let mut s = 0;
                for i in 0..n {
                    if a[i][i].value() == 0 {
                        if let Some(j) = (i + 1..n).find(|&j| a[j][i].value() != 0) {
                            a.swap(i, j);
                            s ^= 1;
                        } else {
                            return T::zero();
                        }
                    }
                    let d = a[i][i].inv();
                    let ai = a.swap_remove(i);
                    for aj in a.iter_mut().skip(i) {
                        let t = aj[i] * d;
                        for (x, y) in aj.iter_mut().zip(ai.iter()) {
                            *x -= *y * t;
                        }
                    }
                    a.push(ai);
                    a.swap(i, n - 1);
                }
                let mut ret = T::one();
                for (i, a) in a.into_iter().enumerate() {
                    ret *= a[i];
                }
                if s == 1 {
                    ret = -ret;
                }
                ret
            }
        }
    }
}
pub mod macros {
    pub mod convert {
        #[macro_export]
        #[doc(hidden)]
        macro_rules! impl_from {
    ($(#[$meta:meta])* [ $($t:tt)* ]; $from:ty => $into:ty, $f:expr) => {
        impl<$($t)*> From<$from> for $into {
            $(#[$meta])*
            fn from(value: $from) -> Self {
                $f(value)
            }
        }
    };
    ($(#[$meta:meta])* $from:ty => $into:ty, $f:expr) => {
        impl_from!($(#[$meta])* []; $from => $into, $f);
    };
}
        #[macro_export]
        #[doc(hidden)]
        macro_rules! impl_try_from {
    ($(#[$meta:meta])* [ $($t:tt)* ]; $from:ty => $into:ty, type Error = $error:ty, $f:expr) => {
        impl<$($t)*> TryFrom<$from> for $into {
            type Error = $error;
            $(#[$meta])*
            fn try_from(value: $from) -> Result<Self, Self::Error> {
                $f(value)
            }
        }
    };
    ($(#[$meta:meta])* $from:ty => $into:ty, type Error = $error:ty, $f:expr) => {
        impl_try_from!($(#[$meta])* []; $from => $into, type Error = $error, $f);
    };
}
    }
    pub mod for_loop {
        #[macro_export]
        macro_rules! for_loop {
            ($init:stmt;  $end:expr; $update:stmt; $b:block) => {
                #[allow(redundant_semicolons)]
                {
                    $init;
                    while $end {
                        $b;
                        $update;
                    }
                }
            };
        }
    }
    pub mod impl_one_zero {
        #[macro_export]
        #[doc(hidden)]
        macro_rules! impl_one_zero {
    ([$($bound:tt)*]; $t:ty; zero: $e:expr; $($rest:tt)*) => {
        impl <$($bound)*> Zero for $t { fn zero() -> Self { $e } }
        impl_one_zero!([$($bound)*]; $t; $($rest)*);
    };
    ([$($bound:tt)*]; $t:ty; one: $e:expr; $($rest:tt)*) => {
        impl <$($bound)*> One for $t { fn one() -> Self { $e } }
        impl_one_zero!([$($bound)*]; $t; $($rest)*);
    };
    ([$($bound:tt)*]; $t:ty;) => {};
    ($t:ty; $($rest:tt)*) => {impl_one_zero!([]; $t; $($rest)*);};
}
    }
    pub mod impl_ops {
        #[macro_export]
        #[doc(hidden)]
        macro_rules! impl_ops {
    (@inner, $(#[$meta:meta])* [$($bound:tt)*]; $tr:ty, $rhs:ty, $a:ty, $f:expr, $fn:tt) => {
        impl <$($bound)*> $tr for $a {
            type Output = Self;
            $(#[$meta])*
            fn $fn(self, rhs: $rhs) -> Self::Output {
                $f(self, rhs)
            }
        }
    };
    (@inner_assign, $(#[$meta:meta])* [$($bound:tt)*]; $tr:ty, $rhs:ty, $a:ty, $f:expr, $fn:tt) => {
        impl <$($bound)*> $tr for $a {
            $(#[$meta])*
            fn $fn(&mut self, rhs: $rhs) {
                $f(self, rhs)
            }
        }
    };
    ($(#[$meta:meta])* [$($bound:tt)*]; $trait:ident for $a:ty, $f:expr) => {
        impl_ops!(@when $(#[$meta])* [$($bound)*]; $trait, Self, $a, $f);
    };
    ($(#[$meta:meta])* [$($bound:tt)*]; $trait:ident <$rhs:ty> for $a:ty, $f:expr) => {
        impl_ops!(@when $(#[$meta])* [$($bound)*]; $trait, $rhs, $a, $f);
    };
    ($(#[$meta:meta])* $trait:ident for $a:ty, $f:expr) => {
        impl_ops!(@when $(#[$meta])* []; $trait, Self, $a, $f);
    };
    ($(#[$meta:meta])* $trait:ident <$rhs:ty> for $a:ty, $f:expr) => {
        impl_ops!(@when $(#[$meta])* []; $trait, $rhs, $a, $f);
    };
    (@when $(#[$meta:meta])* [$($bound:tt)*]; Add, $rhs:ty, $a:ty, $f:expr) => {
        impl_ops!(@inner, $(#[$meta])* [$($bound)*]; std::ops::Add<$rhs>, $rhs, $a, $f, add);
    };
    (@when $(#[$meta:meta])* [$($bound:tt)*]; Sub, $rhs:ty, $a:ty, $f:expr) => {
        impl_ops!(@inner, $(#[$meta])* [$($bound)*]; std::ops::Sub<$rhs>, $rhs, $a, $f, sub);
    };
    (@when $(#[$meta:meta])* [$($bound:tt)*]; Mul, $rhs:ty, $a:ty, $f:expr) => {
        impl_ops!(@inner, $(#[$meta])* [$($bound)*]; std::ops::Mul<$rhs>, $rhs, $a, $f, mul);
    };
    (@when $(#[$meta:meta])* [$($bound:tt)*]; Div, $rhs:ty, $a:ty, $f:expr) => {
        impl_ops!(@inner, $(#[$meta])* [$($bound)*]; std::ops::Div<$rhs>, $rhs, $a, $f, div);
    };
    (@when $(#[$meta:meta])* [$($bound:tt)*]; Rem, $rhs:ty, $a:ty, $f:expr) => {
        impl_ops!(@inner, $(#[$meta])* [$($bound)*]; std::ops::Rem<$rhs>, $rhs, $a, $f, rem);
    };
    (@when $(#[$meta:meta])* [$($bound:tt)*]; AddAssign, $rhs:ty, $a:ty, $f:expr) => {
        impl_ops!(@inner_assign, $(#[$meta])* [$($bound)*]; std::ops::AddAssign<$rhs>, $rhs, $a, $f, add_assign);
    };
    (@when $(#[$meta:meta])* [$($bound:tt)*]; SubAssign, $rhs:ty, $a:ty, $f:expr) => {
        impl_ops!(@inner_assign, $(#[$meta])* [$($bound)*]; std::ops::SubAssign<$rhs>, $rhs, $a, $f, sub_assign);
    };
    (@when $(#[$meta:meta])* [$($bound:tt)*]; MulAssign, $rhs:ty, $a:ty, $f:expr) => {
        impl_ops!(@inner_assign, $(#[$meta])* [$($bound)*]; std::ops::MulAssign<$rhs>, $rhs, $a, $f, mul_assign);
    };
    (@when $(#[$meta:meta])* [$($bound:tt)*]; DivAssign, $rhs:ty, $a:ty, $f:expr) => {
        impl_ops!(@inner_assign, $(#[$meta])* [$($bound)*]; std::ops::DivAssign<$rhs>, $rhs, $a, $f, div_assign);
    };
    (@when $(#[$meta:meta])* [$($bound:tt)*]; RemAssign, $rhs:ty, $a:ty, $f:expr) => {
        impl_ops!(@inner_assign, $(#[$meta])* [$($bound)*]; std::ops::RemAssign<$rhs>, $rhs, $a, $f, rem_assign);
    };
    (@when $(#[$meta:meta])* [$($bound:tt)*]; Neg, $rhs:ty, $a:ty, $f:expr) => {
        impl <$($bound)*> std::ops::Neg for $a {
            type Output = Self;
            $(#[$meta])*
            fn neg(self) -> Self::Output {
                $f(self)
            }
        }
    }
}
    }
    pub mod io {
        #[macro_export]
        macro_rules! get {
    ( $in:expr; [$a:tt $(as $to:ty)*; $num:expr] ) => {
        (0..$num).map(|_| get!($in; $a $(as $to)*)).collect::<Vec<_>>()
    };
    ( $in:expr; ($($type:tt $(as $to:ty)*),*) ) => {
        ($(get!($in; $type $(as $to)*)),*)
    };
    ( $in:expr; i8 ) => { $in.read_i64() as i8 };
    ( $in:expr; i16 ) => { $in.read_i64() as i16 };
    ( $in:expr; i32 ) => { $in.read_i64() as i32 };
    ( $in:expr; i64 ) => { $in.read_i64() };
    ( $in:expr; isize ) => { $in.read_i64() as isize };
    ( $in:expr; u8 ) => { $in.read_u64() as u8 };
    ( $in:expr; u16 ) => { $in.read_u64() as u16 };
    ( $in:expr; u32 ) => { $in.read_u64() as u32 };
    ( $in:expr; u64 ) => { $in.read_u64() };
    ( $in:expr; usize ) => { $in.read_u64() as usize };
    ( $in:expr; char) => { $in.read_char() };
    ( $in:expr; [u8]) => { $in.read_bytes() };
    ( $in:expr; [char] ) => { $in.read_chars() };
    ( $in:expr; String ) => { $in.read_string() };
    ( $in:expr; $from:tt as $to:ty ) => { <$to>::from(get!($in; $from)) };
}
        #[macro_export]
        macro_rules! input {
    ( @inner $in:expr, $name:pat, $type:tt ) => {
        let $name = get!($in; $type);
    };
    ( @inner $in:expr, $name:pat, $type:tt as $to:ty ) => {
        let $name = get!($in; $type as $to);
    };
    ( $in:expr; $( $names:pat = $type:tt $(as $to:ty)? ),* ) => {
        $(input!(@inner $in, $names, $type $(as $to)?);)*
    }
}
        #[macro_export]
        macro_rules! output {
    ( @one $io:expr, $a:expr ) => {
        $io.write($a);
    };
    ( $io:expr; $a:expr, $($rest:expr),* ) => {
        output!(@one $io, $a);
        $(
            $io.write(" ");
            $io.write($rest);
        )*
        $io.writeln("");
    };
}
    }
    pub mod trait_alias {
        #[macro_export]
        macro_rules! trait_alias {
    ($(#[$meta:meta])* $name:ident: $($t:tt)+) => {
        $(#[$meta])*
        pub trait $name : $($t)+ {}
        impl<T: $($t)+> $name for T {}
    };
}
    }
}
pub mod math {
    pub mod mod_ops {
        pub mod pow {
            #[inline]
            pub const fn mod_pow(mut x: u64, mut p: u64, m: u64) -> u64 {
                let mut ret = 1;
                while p > 0 {
                    if (p & 1) != 0 {
                        ret *= x;
                        ret %= m;
                    }
                    x *= x;
                    x %= m;
                    p >>= 1;
                }
                ret
            }
        }
    }
    pub mod primitive_root {
        use crate::for_loop;
        use crate::math::mod_ops::pow::*;
        pub const fn primitive_root(p: u32) -> u32 {
            let mut pf = [0; 32];
            let mut n = p - 1;
            let mut j = 0;
            let mut i = 2;
            while i * i <= n {
                if n % i == 0 {
                    while n % i == 0 {
                        n /= i;
                    }
                    pf[j] = i;
                    j += 1;
                }
                i += 1
            }
            if n != 1 {
                pf[j] = n;
            }
            for_loop ! (               let mut g = 2; g <= p; g += 1; {        let mut ok = true;        for_loop!(let mut i = 0; i < pf.len(); i += 1; {            let f = pf[i];            if f == 0 {                break;            }            if mod_pow(g as u64, (p as u64 - 1) / f as u64, p as u64) == 1 {                ok = false;                break;            }        });        if ok {            return g;        }    } );
            panic!("No primitive roots.");
        }
    }
    pub mod prime_mod {
        use crate::math::primitive_root::primitive_root;
        pub trait PrimeMod: Sized + Copy + Clone + PartialEq + Default {
            const PRIME_NUM: u32;
            const PRIM_ROOT: u32;
        }
        #[derive(Copy, Clone, PartialEq, Eq, Debug, Default, PartialOrd, Ord, Hash)]
        pub struct Prime<const P: u32>;
        impl<const P: u32> PrimeMod for Prime<P> {
            const PRIME_NUM: u32 = P;
            const PRIM_ROOT: u32 = primitive_root(P);
        }
    }
}
pub mod num {
    pub mod const_modint {
        use crate::impl_from;
        use crate::impl_one_zero;
        use crate::impl_ops;
        use crate::math::prime_mod::PrimeMod;
        pub use crate::num::ff::*;
        use crate::num::one_zero::*;
        use std::marker::PhantomData;
        use std::{
            fmt,
            fmt::{Debug, Display, Formatter},
        };
        const B: u32 = 32;
        const R: u64 = 1 << B;
        const MASK: u64 = R - 1;
        struct Calc<P: PrimeMod>(PhantomData<P>);
        impl<P: PrimeMod> Calc<P> {
            const R2: u64 = R % P::PRIME_NUM as u64 * R % P::PRIME_NUM as u64;
            const M: u64 = {
                assert!(P::PRIME_NUM % 2 == 1);
                let mut ret = 0;
                let mut r = R;
                let mut i = 1;
                let mut t = 0;
                while r > 1 {
                    if t % 2 == 0 {
                        t += P::PRIME_NUM;
                        ret += i;
                    }
                    t >>= 1;
                    r >>= 1;
                    i <<= 1;
                }
                ret
            };
            const fn reduce(value: u64) -> u32 {
                let mut ret = (((((value & MASK) * Self::M) & MASK) * P::PRIME_NUM as u64 + value)
                    >> B) as u32;
                if ret >= P::PRIME_NUM {
                    ret -= P::PRIME_NUM;
                }
                ret
            }
            const fn make(value: u32) -> u32 {
                Self::reduce(value as u64 * Self::R2)
            }
        }
        #[derive(Copy, Clone, Default, PartialEq, Eq)]
        pub struct ConstModIntBuilder<P: PrimeMod>(PhantomData<P>);
        impl<P: PrimeMod> ConstModIntBuilder<P> {
            pub fn new() -> Self {
                Self(PhantomData)
            }
        }
        impl<P: PrimeMod> FF for ConstModIntBuilder<P> {
            type Element = ConstModInt<P>;
            fn from_u64(&self, mut value: u64) -> Self::Element {
                if value >= P::PRIME_NUM as u64 {
                    value %= P::PRIME_NUM as u64;
                }
                let value = Calc::<P>::make(value as u32);
                ConstModInt(value, PhantomData)
            }
            fn from_i64(&self, mut value: i64) -> Self::Element {
                value %= P::PRIME_NUM as i64;
                if value < 0 {
                    value += P::PRIME_NUM as i64;
                }
                let value = Calc::<P>::make(value as u32);
                ConstModInt(value, PhantomData)
            }
            fn modulo(&self) -> u32 {
                P::PRIME_NUM
            }
        }
        #[derive(Copy, Clone, PartialEq, Eq, Default, Hash)]
        pub struct ConstModInt<P: PrimeMod>(u32, PhantomData<P>);
        impl<P: PrimeMod> FFElem for ConstModInt<P> {
            #[inline]
            fn value(self) -> u32 {
                Calc::<P>::reduce(self.0 as u64)
            }
            #[inline]
            fn modulo(self) -> u32 {
                P::PRIME_NUM
            }
            fn pow(self, p: u64) -> Self {
                self._pow(p)
            }
        }
        impl<P: PrimeMod> ConstModInt<P> {
            pub const fn new(n: u32) -> Self {
                let value = if n < P::PRIME_NUM {
                    n
                } else {
                    n % P::PRIME_NUM
                };
                Self(Calc::<P>::make(value), PhantomData)
            }
            pub const fn _add(self, y: Self) -> Self {
                let mut a = self.0 + y.0;
                if a >= P::PRIME_NUM {
                    a -= P::PRIME_NUM;
                }
                Self(a, PhantomData)
            }
            pub const fn _sub(self, y: Self) -> Self {
                let a = if self.0 < y.0 {
                    self.0 + P::PRIME_NUM - y.0
                } else {
                    self.0 - y.0
                };
                Self(a, PhantomData)
            }
            pub const fn _mul(self, y: Self) -> Self {
                Self(Calc::<P>::reduce(self.0 as u64 * y.0 as u64), PhantomData)
            }
            pub const fn _pow(self, mut p: u64) -> Self {
                let mut ret = Self(Calc::<P>::make(1), PhantomData);
                let mut a = self;
                while p > 0 {
                    if (p & 1) != 0 {
                        ret = ret._mul(a);
                    }
                    a = a._mul(a);
                    p >>= 1;
                }
                ret
            }
            pub const fn _inv(self) -> Self {
                self._pow(P::PRIME_NUM as u64 - 2)
            }
        }
        impl<P: PrimeMod> Display for ConstModInt<P> {
            fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
                write!(f, "{}", self.value())
            }
        }
        impl<P: PrimeMod> Debug for ConstModInt<P> {
            fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
                write!(f, "{} (mod {})", self.value(), P::PRIME_NUM)
            }
        }
        impl_ops!([P: PrimeMod]; Add for ConstModInt<P>, |x: Self, y: Self| x._add(y));
        impl_ops!([P: PrimeMod]; Sub for ConstModInt<P>, |x: Self, y: Self| x._sub(y));
        impl_ops!([P: PrimeMod]; Mul for ConstModInt<P>, |x: Self, y: Self| x._mul(y));
        impl_ops!([P: PrimeMod]; Div for ConstModInt<P>, |x: Self, y: Self| x * y.inv());
        impl_ops!([P: PrimeMod]; AddAssign for ConstModInt<P>, |x: &mut Self, y| *x = *x + y);
        impl_ops!([P: PrimeMod]; SubAssign for ConstModInt<P>, |x: &mut Self, y| *x = *x - y);
        impl_ops!([P: PrimeMod]; MulAssign for ConstModInt<P>, |x: &mut Self, y| *x = *x * y);
        impl_ops!([P: PrimeMod]; DivAssign for ConstModInt<P>, |x: &mut Self, y| *x = *x / y);
        impl_ops!([P: PrimeMod]; Neg for ConstModInt<P>, |x: Self| Self(0, PhantomData) - x);
        impl_from!([P: PrimeMod]; ConstModInt<P> => u32, |value: ConstModInt<P>| value.value());
        impl_from!([P: PrimeMod]; usize => ConstModInt<P>, |value| ConstModIntBuilder::new().from_u64(value as u64));
        impl_from!([P: PrimeMod]; u64 => ConstModInt<P>, |value| ConstModIntBuilder::new().from_u64(value));
        impl_from!([P: PrimeMod]; u32 => ConstModInt<P>, |value| ConstModIntBuilder::new().from_u64(value as u64));
        impl_from!([P: PrimeMod]; isize => ConstModInt<P>, |value| ConstModIntBuilder::new().from_i64(value as i64));
        impl_from!([P: PrimeMod]; i64 => ConstModInt<P>, |value| ConstModIntBuilder::new().from_i64(value));
        impl_from!([P: PrimeMod]; i32 => ConstModInt<P>, |value| ConstModIntBuilder::new().from_i64(value as i64));
        impl_one_zero!([P: PrimeMod]; ConstModInt<P>; one: Self::new(1); zero: Self(0, PhantomData););
    }
    pub mod ff {
        use crate::num::arithmetic::Arithmetic;
        use std::ops::Neg;
        #[allow(clippy::wrong_self_convention)]
        pub trait FF: Clone {
            type Element: FFElem;
            fn from_u64(&self, a: u64) -> Self::Element;
            fn from_i64(&self, a: i64) -> Self::Element;
            fn frac(&self, a: i64, b: i64) -> Self::Element {
                self.from_i64(a) / self.from_i64(b)
            }
            fn modulo(&self) -> u32;
        }
        pub trait FFElem: Sized + Copy + Neg<Output = Self> + PartialEq + Arithmetic {
            fn value(self) -> u32;
            fn modulo(self) -> u32;
            fn pow(self, p: u64) -> Self;
            fn inv(self) -> Self {
                self.pow(self.modulo() as u64 - 2)
            }
        }
    }
    pub mod arithmetic {
        use crate::trait_alias;
        use std::ops::{Add, AddAssign, Div, DivAssign, Mul, MulAssign, Sub, SubAssign};
        trait_alias!(
            #[doc=" 四則演算ができる型"]
            Arithmetic:
            Sized
                + Add<Output = Self>
                + Sub<Output = Self>
                + Mul<Output = Self>
                + Div<Output = Self>
                + AddAssign
                + SubAssign
                + MulAssign
                + DivAssign
        );
    }
    pub mod one_zero {
        pub trait Zero {
            fn zero() -> Self;
        }
        pub trait One {
            fn one() -> Self;
        }
        macro_rules! impl_one_zero {
    ($($t:tt),*) => {
        $(
            impl Zero for $t { fn zero() -> Self { 0 as $t } }
            impl One for $t { fn one() -> Self { 1 as $t } }
            impl Zero for &$t { fn zero() -> Self { &(0 as $t) } }
            impl One for &$t { fn one() -> Self { &(1 as $t) } }
        )*
    }
}
        use std::num::Wrapping;
        macro_rules! impl_one_zero_wrap {
    ($($t:tt),*) => {
        $(
            impl Zero for Wrapping<$t> { fn zero() -> Self { Wrapping($t::zero()) }}
            impl One for Wrapping<$t> { fn one() -> Self { Wrapping($t::one()) }}
        )*
    }
}
        impl_one_zero!(u8, u16, u32, u64, u128, usize, i8, i16, i32, i64, i128, isize, f32, f64);
        impl_one_zero_wrap!(u8, u16, u32, u64, u128, usize, i8, i16, i32, i64, i128, isize);
    }
}
