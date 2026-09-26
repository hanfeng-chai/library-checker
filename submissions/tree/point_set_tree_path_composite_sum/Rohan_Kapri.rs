use urectanc::{
    algebra::{Monoid, Reverse},
    fast_io,
    heavy_light_decomposition::HeavyLightDecomposition,
    modint::ModInt998244353,
    static_top_tree::{StaticTopTree, TreeDP},
};

type Mint = ModInt998244353;

fn main() {
    let mut input = fast_io::stdin();
    let mut output = fast_io::stdout();

    let n: usize = input.val();
    let q: usize = input.val();
    let a: Vec<_> = (0..n).map(|_| Mint::raw(input.val())).collect();
    let edges: Vec<(usize, usize, (Mint, Mint))> = (0..n - 1)
        .map(|_| {
            (
                input.val(),
                input.val(),
                (Mint::raw(input.val()), Mint::raw(input.val())),
            )
        })
        .collect();

    let hld = HeavyLightDecomposition::from_edges(edges.iter().map(|&(u, v, _)| (u, v)), 0);
    let edge_index = |u: usize, v: usize| if hld.index(u) < hld.index(v) { v } else { u };
    let mut vertex = a
        .iter()
        .map(|&a| (a, (Mint::one(), Mint::zero())))
        .collect::<Vec<_>>();
    for &(u, v, w) in &edges {
        vertex[edge_index(u, v)].1 = w;
    }

    let mut dp = StaticTopTree::<PointSetTreePathCompositeSum>::new(&hld, vertex.clone());

    for _ in 0..q {
        let op: u8 = input.val();
        let i: usize = input.val();
        if op == 0 {
            let x = Mint::raw(input.val());
            vertex[i].0 = x;
            dp.update(i, vertex[i]);
        } else {
            let y = Mint::raw(input.val());
            let z = Mint::raw(input.val());
            let (u, v, _) = edges[i];
            let i = edge_index(u, v);
            vertex[i].1 = (y, z);
            dp.update(i, vertex[i]);
        }
        let r: usize = input.val();
        let ans = dp.prod(r).reverse.sum;
        output.writeln(ans.val());
    }
}

struct PointSetTreePathCompositeSum;

impl TreeDP for PointSetTreePathCompositeSum {
    type Path = ReversiblePath;
    type Point = Point;
    type Vertex = (Mint, (Mint, Mint));

    type PathMonoid = ReversiblePath;
    type PointMonoid = Point;

    fn add_edge(path: &<Self::Path as Monoid>::Elem) -> <Self::Point as Monoid>::Elem {
        Point {
            cnt: path.forward.cnt,
            sum: path.forward.sum,
        }
    }

    fn add_vertex(
        point: &<Self::Point as Monoid>::Elem,
        vertex: &Self::Vertex,
    ) -> <Self::Path as Monoid>::Elem {
        let &(a, f) = vertex;
        ReversiblePath {
            forward: Path {
                f,
                cnt: point.cnt + 1,
                sum: f.0 * (point.sum + a) + f.1 * (point.cnt + 1),
            },
            reverse: Path {
                f,
                cnt: point.cnt + 1,
                sum: point.sum + a,
            },
        }
    }
}

#[derive(Clone)]
struct ReversiblePath {
    forward: Path,
    reverse: Path,
}

impl Monoid for ReversiblePath {
    type Elem = Self;

    #[inline(always)]
    fn identity() -> Self::Elem {
        Self {
            forward: Path::identity(),
            reverse: Path::identity(),
        }
    }

    #[inline(always)]
    fn op(lhs: &Self::Elem, rhs: &Self::Elem) -> Self::Elem {
        Self {
            forward: Path::op(&lhs.forward, &rhs.forward),
            reverse: Reverse::<Path>::op(&lhs.reverse, &rhs.reverse),
        }
    }
}

#[derive(Clone)]
struct Path {
    f: (Mint, Mint),
    cnt: u32,
    sum: Mint,
}

impl Monoid for Path {
    type Elem = Self;

    #[inline(always)]
    fn identity() -> Self::Elem {
        Self {
            f: (Mint::one(), Mint::zero()),
            cnt: 0,
            sum: Mint::zero(),
        }
    }

    #[inline(always)]
    fn op(lhs: &Self::Elem, rhs: &Self::Elem) -> Self::Elem {
        let f = (lhs.f.0 * rhs.f.0, lhs.f.0 * rhs.f.1 + lhs.f.1);
        Self {
            f,
            cnt: lhs.cnt + rhs.cnt,
            sum: lhs.sum + lhs.f.0 * rhs.sum + lhs.f.1 * rhs.cnt,
        }
    }
}

#[derive(Clone)]
struct Point {
    cnt: u32,
    sum: Mint,
}

impl Monoid for Point {
    type Elem = Self;

    #[inline(always)]
    fn identity() -> Self::Elem {
        Self {
            cnt: 0,
            sum: Mint::zero(),
        }
    }

    #[inline(always)]
    fn op(lhs: &Self::Elem, rhs: &Self::Elem) -> Self::Elem {
        Self {
            cnt: lhs.cnt + rhs.cnt,
            sum: lhs.sum + rhs.sum,
        }
    }
}

pub mod urectanc {
    pub mod algebra {
        pub trait Monoid {
            type Elem: Clone;
            fn identity() -> Self::Elem;
            fn op(lhs: &Self::Elem, rhs: &Self::Elem) -> Self::Elem;
        }
        pub trait Group: Monoid {
            fn inv(elem: &Self::Elem) -> Self::Elem;
        }
        pub trait MapMonoid: Monoid {
            type Map: Clone;
            fn identity_map() -> Self::Map;
            fn apply(x: &Self::Elem, f: &Self::Map) -> Self::Elem;
            fn compose(f: &Self::Map, g: &Self::Map) -> Self::Map;
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
        pub struct Reversible<M: Monoid> {
            _phantom: std::marker::PhantomData<M>,
        }
        impl<M: Monoid> Monoid for Reversible<M> {
            type Elem = (M::Elem, M::Elem);
            #[inline]
            fn identity() -> Self::Elem {
                (M::identity(), M::identity())
            }
            #[inline]
            fn op(lhs: &Self::Elem, rhs: &Self::Elem) -> Self::Elem {
                (M::op(&lhs.0, &rhs.0), M::op(&rhs.1, &lhs.1))
            }
        }
        pub struct MonoidArray<M, const N: usize> {
            _phantom: std::marker::PhantomData<M>,
        }
        impl<M: Monoid, const N: usize> Monoid for MonoidArray<M, N> {
            type Elem = [M::Elem; N];
            #[inline]
            fn identity() -> Self::Elem {
                std::array::from_fn(|_| M::identity())
            }
            #[inline]
            fn op(lhs: &Self::Elem, rhs: &Self::Elem) -> Self::Elem {
                std::array::from_fn(|i| M::op(&lhs[i], &rhs[i]))
            }
        }
    }
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
                            if neg { -val } else { val }
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
    pub mod heavy_light_decomposition {
        use crate::urectanc::compressed_sparse_row;
        use compressed_sparse_row::CSRArray;
        const PARENT_FLAG: usize = 1 << 30;
        pub struct HeavyLightDecomposition {
            root: usize,
            head_or_parent: Vec<usize>,
            index: Vec<usize>,
            pre_order: Vec<usize>,
            subtree_size: Vec<usize>,
        }
        impl HeavyLightDecomposition {
            pub fn from_edges<I>(edges: I, root: usize) -> Self
            where
                I: ExactSizeIterator<Item = (usize, usize)>,
            {
                let n = edges.len() + 1;
                let mut adj = vec![0; n];
                let mut deg = vec![0; n];
                for (u, v) in edges {
                    adj[u] ^= v;
                    adj[v] ^= u;
                    deg[u] += 1;
                    deg[v] += 1;
                }
                deg[root] = 0;
                let mut subtree_size = vec![1; n];
                let mut order = Vec::with_capacity(n);
                for mut v in 0..n {
                    while deg[v] == 1 {
                        let p = adj[v];
                        adj[p] ^= v;
                        deg[v] = 0;
                        deg[p] -= 1;
                        subtree_size[p] += subtree_size[v];
                        order.push(v);
                        v = p;
                    }
                }
                order.push(root);
                let mut count = vec![0; n + 1];
                for &i in &subtree_size {
                    count[i] += 1;
                }
                for i in (0..n).rev() {
                    count[i] += count[i + 1];
                }
                for v in 0..n {
                    let size = subtree_size[v];
                    count[size] -= 1;
                    order[count[size]] = v;
                }
                let mut head_or_parent = adj;
                head_or_parent[root] = !0;
                let mut index = vec![0; n];
                let mut offset = vec![1; n];
                for &v in order.iter().skip(1) {
                    let p = head_or_parent[v];
                    if offset[p] == 1 {
                        let head = head_or_parent[p];
                        head_or_parent[v] = if head & PARENT_FLAG == 0 { head } else { p };
                    } else {
                        head_or_parent[v] |= PARENT_FLAG;
                    }
                    index[v] = index[p] + offset[p];
                    offset[p] += subtree_size[v];
                }
                for (v, &i) in index.iter().enumerate() {
                    order[i] = v;
                }
                Self {
                    root,
                    head_or_parent,
                    index,
                    pre_order: order,
                    subtree_size,
                }
            }
            pub fn root(&self) -> usize {
                self.root
            }
            pub fn size(&self) -> usize {
                self.pre_order.len()
            }
            pub fn graph(&self) -> CSRArray<usize> {
                let edges: Vec<_> = self
                    .pre_order
                    .iter()
                    .skip(1)
                    .map(|&v| (self.parent(v).unwrap(), v))
                    .collect();
                CSRArray::new(self.size(), edges)
            }
            pub fn subtree_size(&self, v: usize) -> usize {
                self.subtree_size[v]
            }
            pub fn pre_order(&self) -> &'_ [usize] {
                &self.pre_order
            }
            pub fn heavy_path(&self, v: usize) -> &'_ [usize] {
                self.pre_order[self.index(v)..]
                    .chunk_by(|_, &u| self.head(u) == v)
                    .next()
                    .unwrap()
            }
            fn head(&self, v: usize) -> usize {
                let head = self.head_or_parent[v];
                if head & PARENT_FLAG == 0 { head } else { v }
            }
            pub fn parent(&self, v: usize) -> Option<usize> {
                let head = self.head_or_parent[v];
                if head == !0 {
                    None
                } else if head & PARENT_FLAG == 0 {
                    Some(self.pre_order[self.index(v) - 1])
                } else {
                    Some(head ^ PARENT_FLAG)
                }
            }
            pub fn index(&self, v: usize) -> usize {
                self.index[v]
            }
            pub fn edge_index(&self, u: usize, v: usize) -> usize {
                self.index(u).max(self.index(v))
            }
            pub fn subtree_range(&self, v: usize) -> (usize, usize) {
                let l = self.index(v);
                (l, l + self.subtree_size[v])
            }
            pub fn is_ancestor(&self, ancestor: usize, descendant: usize) -> bool {
                let (l, r) = self.subtree_range(ancestor);
                (l..r).contains(&self.index(descendant))
            }
            pub fn la(&self, v: usize, mut d: usize) -> Option<usize> {
                let mut la = Some(v);
                while let Some(v) = la {
                    let head = self.head(v);
                    if self.index(v) - self.index(head) >= d {
                        return Some(self.pre_order[self.index(v) - d]);
                    }
                    d -= self.index(v) - self.index(head) + 1;
                    la = self.parent(head);
                }
                la
            }
            pub fn lca(&self, mut u: usize, mut v: usize) -> usize {
                if self.index(u) > self.index(v) {
                    std::mem::swap(&mut u, &mut v);
                }
                if self.is_ancestor(u, v) {
                    return u;
                }
                while self.index(u) < self.index(v) {
                    v = self.parent(self.head(v)).unwrap();
                }
                v
            }
            pub fn dist(&self, u: usize, v: usize) -> usize {
                self.path_edges(u, v).map(|(l, r, _)| r - l).sum()
            }
            pub fn path_vertices(
                &self,
                u: usize,
                v: usize,
            ) -> impl Iterator<Item = (usize, usize, bool)> + '_ {
                self.path(u, v)
                    .map(|(u, v, topdown, _)| (self.index(u), self.index(v) + 1, topdown))
            }
            pub fn path_edges(
                &self,
                u: usize,
                v: usize,
            ) -> impl Iterator<Item = (usize, usize, bool)> + '_ {
                self.path(u, v).map(|(u, v, topdown, last)| {
                    (
                        self.index(u) + usize::from(last),
                        self.index(v) + 1,
                        topdown,
                    )
                })
            }
            fn path(&self, u: usize, v: usize) -> PathSegments<'_> {
                PathSegments {
                    hld: self,
                    u,
                    v,
                    exhausted: false,
                }
            }
        }
        pub struct PathSegments<'a> {
            hld: &'a HeavyLightDecomposition,
            u: usize,
            v: usize,
            exhausted: bool,
        }
        impl Iterator for PathSegments<'_> {
            type Item = (usize, usize, bool, bool);
            fn next(&mut self) -> Option<Self::Item> {
                if self.exhausted {
                    return None;
                }
                let Self { hld, u, v, .. } = *self;
                if hld.head(u) == hld.head(v) {
                    self.exhausted = true;
                    if hld.index(u) < hld.index(v) {
                        Some((u, v, true, true))
                    } else {
                        Some((v, u, false, true))
                    }
                } else {
                    if hld.index(u) < hld.index(v) {
                        let head = hld.head(v);
                        self.v = hld.parent(head).unwrap();
                        Some((head, v, true, false))
                    } else {
                        let head = hld.head(u);
                        self.u = hld.parent(head).unwrap();
                        Some((head, u, false, false))
                    }
                }
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
        pub trait Modulus: 'static + Clone + Copy + Debug + Default + PartialEq + Eq {
            const MOD: u32;
        }
        macro_rules! define_modulus {
            ($name:ident, $modulus:expr) => {
                #[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
                pub struct $name;
                impl Modulus for $name {
                    const MOD: u32 = const {
                        assert!($modulus < (1u32 << 31));
                        $modulus
                    };
                }
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
                if gcd == 1 {
                    Some(Self::raw(inv as u32))
                } else {
                    None
                }
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
            #[allow(clippy::suspicious_op_assign_impl)]
            fn div_assign(&mut self, rhs: T) {
                *self *= rhs.into().inv();
            }
        }
        macro_rules! impl_binnary_operators {
            ($op:ident, $op_assign:ident, $fn:ident, $fn_assign:ident) => {
                impl<M: Modulus, T: Into<StaticModInt<M>>> std::ops::$op<T> for StaticModInt<M> {
                    type Output = StaticModInt<M>;
                    fn $fn(mut self, rhs: T) -> StaticModInt<M> {
                        self.$fn_assign(rhs.into());
                        self
                    }
                }
                impl<M: Modulus> std::ops::$op<&StaticModInt<M>> for StaticModInt<M> {
                    type Output = StaticModInt<M>;
                    fn $fn(self, rhs: &StaticModInt<M>) -> StaticModInt<M> {
                        self.$fn(*rhs)
                    }
                }
                impl<M: Modulus, T: Into<StaticModInt<M>>> std::ops::$op<T> for &StaticModInt<M> {
                    type Output = StaticModInt<M>;
                    fn $fn(self, rhs: T) -> StaticModInt<M> {
                        (*self).$fn(rhs.into())
                    }
                }
                impl<M: Modulus> std::ops::$op<&StaticModInt<M>> for &StaticModInt<M> {
                    type Output = StaticModInt<M>;
                    fn $fn(self, rhs: &StaticModInt<M>) -> StaticModInt<M> {
                        (*self).$fn(*rhs)
                    }
                }
                impl<M: Modulus> std::ops::$op_assign<&StaticModInt<M>> for StaticModInt<M> {
                    fn $fn_assign(&mut self, rhs: &StaticModInt<M>) {
                        *self = self.$fn(*rhs);
                    }
                }
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
        impl<'a, M: Modulus> std::iter::Product<&'a StaticModInt<M>> for StaticModInt<M> {
            fn product<I: Iterator<Item = &'a Self>>(iter: I) -> Self {
                iter.fold(Self::one(), Mul::mul)
            }
        }
        pub type ModInt998244353 = StaticModInt<Mod998244353>;
        pub type ModInt1000000007 = StaticModInt<Mod1000000007>;
    }
    pub mod static_top_tree {
        use crate::urectanc::{algebra, heavy_light_decomposition};
        use algebra::Monoid;
        use heavy_light_decomposition::HeavyLightDecomposition;
        mod builder {
            use super::*;
            use crate::urectanc::heavy_light_decomposition;
            use heavy_light_decomposition::HeavyLightDecomposition;
            use std::collections::BinaryHeap;
            pub struct Builder<DP: TreeDP> {
                height: Vec<u32>,
                links: Vec<(NodeIndex, Link)>,
                path_tree: Vec<PathNode<DP>>,
                point_tree: Vec<PointNode<DP>>,
                vertex: Vec<VertexNode<DP>>,
            }
            impl<DP: TreeDP> Builder<DP> {
                pub fn new(vertex: Vec<DP::Vertex>) -> Self {
                    let n = vertex.len();
                    Self {
                        height: vec![0; n],
                        links: Vec::with_capacity(2 * n),
                        vertex: vertex.into_iter().map(VertexNode::new).collect(),
                        path_tree: vec![],
                        point_tree: vec![],
                    }
                }
                #[cold]
                #[inline(never)]
                pub fn build(mut self, hld: &HeavyLightDecomposition) -> StaticTopTree<DP> {
                    let child = hld.graph();
                    for &v in hld.pre_order().iter().rev() {
                        let mut heap: BinaryHeap<_> = child[v]
                            .iter()
                            .skip(1)
                            .map(|&u| {
                                let path = self.build_path_tree(hld.heavy_path(u));
                                Cluster::new(path.index, DP::add_edge(&path.value), path.height)
                            })
                            .collect();
                        for _ in 0..heap.len().saturating_sub(1) {
                            let l = heap.pop().unwrap();
                            let r = heap.pop().unwrap();
                            let i = self.point_tree.len();
                            self.links.push((l.index, Link::Point(i as u32 * 2)));
                            self.links.push((r.index, Link::Point(i as u32 * 2 + 1)));
                            heap.push(Cluster::new(
                                NodeIndex::Point(i),
                                DP::rake(&l.value, &r.value),
                                l.height.max(r.height) + 1,
                            ));
                            self.point_tree.push(PointNode::new(l.value, r.value));
                        }
                        if let Some(root) = heap.pop() {
                            self.links.push((root.index, Link::Vertex(v as u32)));
                            self.vertex[v].light_child = root.value;
                            self.height[v] = root.height;
                        }
                    }
                    let path = self.build_path_tree(hld.heavy_path(hld.root())).value;
                    for &(index, link) in &self.links {
                        match index {
                            NodeIndex::Path(i) => self.path_tree[i].link = link,
                            NodeIndex::Point(i) => self.point_tree[i].link = link,
                            NodeIndex::Vertex(i) => self.vertex[i].link = link,
                        }
                    }
                    StaticTopTree {
                        path_tree: self.path_tree,
                        point_tree: self.point_tree,
                        vertex: self.vertex,
                        dp: path,
                    }
                }
                #[cold]
                #[inline(never)]
                fn build_path_tree(&mut self, path: &[usize]) -> Cluster<DP::Path> {
                    let mut path_cluster =
                        PathTreeBuilder::<DP>::new(&mut self.path_tree, &mut self.links);
                    for &v in path {
                        path_cluster.add(
                            NodeIndex::Vertex(v),
                            DP::add_vertex(&self.vertex[v].light_child, &self.vertex[v].vertex),
                            self.height[v],
                        );
                    }
                    path_cluster.finish().unwrap()
                }
            }
            struct PathTreeBuilder<'a, DP: TreeDP> {
                tree: &'a mut Vec<PathNode<DP>>,
                links: &'a mut Vec<(NodeIndex, Link)>,
                stack: Vec<Cluster<DP::Path>>,
            }
            impl<'a, DP: TreeDP> PathTreeBuilder<'a, DP> {
                fn new(
                    tree: &'a mut Vec<PathNode<DP>>,
                    links: &'a mut Vec<(NodeIndex, Link)>,
                ) -> Self {
                    Self {
                        tree,
                        links,
                        stack: Vec::new(),
                    }
                }
                fn add(&mut self, index: NodeIndex, value: DP::Path, height: u32) {
                    self.stack.push(Cluster::new(index, value, height));
                    loop {
                        if let Some([l, m, r]) = self.stack.last_chunk::<3>()
                            && (l == m || l >= r)
                        {
                            let last = self.stack.pop().unwrap();
                            self.merge_last_two();
                            self.stack.push(last);
                        } else if let Some([l, r]) = self.stack.last_chunk::<2>()
                            && l >= r
                        {
                            self.merge_last_two();
                        } else {
                            break;
                        }
                    }
                }
                fn finish(mut self) -> Option<Cluster<DP::Path>> {
                    for _ in 0..self.stack.len().saturating_sub(1) {
                        self.merge_last_two();
                    }
                    self.stack.pop()
                }
                fn merge_last_two(&mut self) {
                    let r = self.stack.pop().unwrap();
                    let l = self.stack.pop().unwrap();
                    let i = self.tree.len();
                    self.links.push((l.index, Link::Path(i as u32 * 2)));
                    self.links.push((r.index, Link::Path(i as u32 * 2 + 1)));
                    self.stack.push(Cluster::new(
                        NodeIndex::Path(i),
                        DP::compress(&l.value, &r.value),
                        l.height.max(r.height) + 1,
                    ));
                    self.tree.push(PathNode::new(l.value, r.value));
                }
            }
            struct Cluster<T> {
                index: NodeIndex,
                value: T,
                height: u32,
            }
            impl<T> Cluster<T> {
                fn new(index: NodeIndex, value: T, height: u32) -> Self {
                    Self {
                        index,
                        value,
                        height,
                    }
                }
            }
            impl<T> PartialOrd for Cluster<T> {
                fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
                    Some(self.cmp(other))
                }
            }
            impl<T> Ord for Cluster<T> {
                fn cmp(&self, other: &Self) -> std::cmp::Ordering {
                    other.height.cmp(&self.height)
                }
            }
            impl<T> PartialEq for Cluster<T> {
                fn eq(&self, other: &Self) -> bool {
                    self.height == other.height
                }
            }
            impl<T> Eq for Cluster<T> {}
            #[derive(Clone, Copy)]
            enum NodeIndex {
                Path(usize),
                Point(usize),
                Vertex(usize),
            }
        }
        pub trait TreeDP {
            type Path: Clone;
            type Point: Clone;
            type Vertex;
            type PathMonoid: Monoid<Elem = Self::Path>;
            type PointMonoid: Monoid<Elem = Self::Point>;
            fn add_edge(path: &Self::Path) -> Self::Point;
            fn add_vertex(point: &Self::Point, vertex: &Self::Vertex) -> Self::Path;
            fn compress(parent: &Self::Path, child: &Self::Path) -> Self::Path {
                Self::PathMonoid::op(parent, child)
            }
            fn rake(lhs: &Self::Point, rhs: &Self::Point) -> Self::Point {
                Self::PointMonoid::op(lhs, rhs)
            }
        }
        pub struct StaticTopTree<DP: TreeDP> {
            path_tree: Vec<PathNode<DP>>,
            point_tree: Vec<PointNode<DP>>,
            vertex: Vec<VertexNode<DP>>,
            dp: DP::Path,
        }
        impl<DP: TreeDP> StaticTopTree<DP> {
            pub fn new(hld: &HeavyLightDecomposition, vertex: Vec<DP::Vertex>) -> Self {
                let builder = builder::Builder::<DP>::new(vertex);
                builder.build(hld)
            }
            #[inline(always)]
            pub fn update(&mut self, v: usize, vertex: DP::Vertex) {
                self.vertex[v].vertex = vertex;
                let mut path = self.vertex[v].val();
                let mut link = self.vertex[v].link;
                while link != Link::Root {
                    while let Link::Path(p) = link {
                        let (i, j) = (p as usize >> 1, p as usize & 1);
                        self.path_tree[i].child[j] = path;
                        path = self.path_tree[i].val();
                        link = self.path_tree[i].link;
                    }
                    let mut point = DP::add_edge(&path);
                    while let Link::Point(p) = link {
                        let (i, j) = (p as usize >> 1, p as usize & 1);
                        self.point_tree[i].child[j] = point;
                        point = self.point_tree[i].val();
                        link = self.point_tree[i].link;
                    }
                    if let Link::Vertex(u) = link {
                        let u = u as usize;
                        self.vertex[u].light_child = point;
                        path = self.vertex[u].val();
                        link = self.vertex[u].link;
                    }
                }
                self.dp = path;
            }
            pub fn dp(&self) -> DP::Point {
                DP::add_edge(&self.dp)
            }
            #[inline(always)]
            pub fn prod(&self, mut v: usize) -> DP::Path {
                let mut link = self.vertex[v].link;
                let mut point = self.vertex[v].light_child.clone();
                let mut path = DP::PathMonoid::identity();
                loop {
                    let mut above = DP::PathMonoid::identity();
                    let mut below = DP::PathMonoid::identity();
                    while let Link::Path(p) = link {
                        let (i, j) = (p as usize >> 1, p as usize & 1);
                        if j == 0 {
                            below = DP::compress(&below, &self.path_tree[i].child[1]);
                        } else {
                            above = DP::compress(&self.path_tree[i].child[0], &above);
                        }
                        link = self.path_tree[i].link;
                    }
                    point = DP::rake(&point, &DP::add_edge(&below));
                    path = DP::compress(
                        &above,
                        &DP::compress(&DP::add_vertex(&point, &self.vertex[v].vertex), &path),
                    );
                    if link == Link::Root {
                        return path;
                    }
                    point = DP::PointMonoid::identity();
                    while let Link::Point(p) = link {
                        let (i, j) = (p as usize >> 1, p as usize & 1);
                        point = if j == 0 {
                            DP::rake(&point, &self.point_tree[i].child[1])
                        } else {
                            DP::rake(&self.point_tree[i].child[0], &point)
                        };
                        link = self.point_tree[i].link;
                    }
                    if let Link::Vertex(u) = link {
                        v = u as usize;
                        link = self.vertex[v].link;
                    }
                }
            }
        }
        struct PathNode<DP: TreeDP> {
            link: Link,
            child: [DP::Path; 2],
        }
        impl<DP: TreeDP> PathNode<DP> {
            fn new(left: DP::Path, right: DP::Path) -> Self {
                Self {
                    link: Link::Root,
                    child: [left, right],
                }
            }
            fn val(&self) -> DP::Path {
                DP::compress(&self.child[0], &self.child[1])
            }
        }
        struct PointNode<DP: TreeDP> {
            link: Link,
            child: [DP::Point; 2],
        }
        impl<DP: TreeDP> PointNode<DP> {
            fn new(left: DP::Point, right: DP::Point) -> Self {
                Self {
                    link: Link::Root,
                    child: [left, right],
                }
            }
            fn val(&self) -> DP::Point {
                DP::rake(&self.child[0], &self.child[1])
            }
        }
        struct VertexNode<DP: TreeDP> {
            link: Link,
            vertex: DP::Vertex,
            light_child: DP::Point,
        }
        impl<DP: TreeDP> VertexNode<DP> {
            fn new(vertex: DP::Vertex) -> Self {
                Self {
                    link: Link::Root,
                    vertex,
                    light_child: DP::PointMonoid::identity(),
                }
            }
            fn val(&self) -> DP::Path {
                DP::add_vertex(&self.light_child, &self.vertex)
            }
        }
        #[derive(Clone, Copy, PartialEq)]
        enum Link {
            Path(u32),
            Point(u32),
            Vertex(u32),
            Root,
        }
    }
    pub mod compressed_sparse_row {
        use std::fmt::Debug;
        pub struct CSRArray<T> {
            n: usize,
            index: Vec<usize>,
            csr: Vec<T>,
        }
        impl<T> CSRArray<T> {
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
            pub fn is_empty(&self) -> bool {
                self.n == 0
            }
            pub fn get(&self, i: usize) -> Option<&[T]> {
                (i < self.n).then(|| &self.csr[self.index[i]..self.index[i + 1]])
            }
            pub fn iter(&'_ self) -> Row<'_, T> {
                Row {
                    csr: self,
                    index: 0,
                }
            }
        }
        impl<T> std::ops::Index<usize> for CSRArray<T> {
            type Output = [T];
            fn index(&self, index: usize) -> &Self::Output {
                self.get(index).unwrap()
            }
        }
        impl<'a, T> IntoIterator for &'a CSRArray<T> {
            type Item = &'a [T];
            type IntoIter = Row<'a, T>;
            fn into_iter(self) -> Self::IntoIter {
                self.iter()
            }
        }
        impl<T: Debug> Debug for CSRArray<T> {
            fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
                let mut f = f.debug_list();
                for row in self {
                    f.entry(&row);
                }
                f.finish()
            }
        }
        pub struct Row<'a, T> {
            csr: &'a CSRArray<T>,
            index: usize,
        }
        impl<'a, T> Iterator for Row<'a, T> {
            type Item = &'a [T];
            fn next(&mut self) -> Option<Self::Item> {
                let row = self.csr.get(self.index)?;
                self.index += 1;
                Some(row)
            }
        }
    }
}
