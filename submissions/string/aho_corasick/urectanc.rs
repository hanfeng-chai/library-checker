use urectanc::{aho_corasick::Builder, fast_io};

fn main() {
    let mut input = fast_io::stdin();
    let mut output = fast_io::stdout();

    let n: usize = input.val();
    let mut words = (0..n).map(|_| input.bytes().to_vec()).collect::<Vec<_>>();
    words.iter_mut().flatten().for_each(|c| *c -= b'a');

    let mut builder = Builder::<26>::new();
    let v = words
        .iter()
        .map(|word| builder.insert(word))
        .collect::<Vec<_>>();
    let aho = builder.build();

    let m = aho.size();
    output.writeln(m);
    for i in 1..m {
        let p = aho[i].prefix().unwrap();
        let s = aho[i].suffix();
        output.write(p);
        output.writeln(s);
    }

    for &v in &v {
        output.write(v);
    }
    output.writeln("");
}


pub mod urectanc {
    pub mod aho_corasick {
        mod small_set {
            pub type Mask = u32;
            pub struct SmallSet<T> {
                mask: Mask,
                inner: Vec<T>,
            }
            impl<T: Copy> SmallSet<T> {
                pub fn new() -> Self {
                    Self { mask: 0, inner: Vec::new() }
                }
                pub fn get(&self, i: u8) -> Option<T> {
                    (self.mask >> i & 1 == 1).then(|| self.inner[self.pos(i)])
                }
                pub fn insert(&mut self, i: u8, v: T) {
                    self.inner.insert(self.pos(i), v);
                    self.mask |= 1 << i;
                }
                fn pos(&self, i: u8) -> usize {
                    (self.mask & ((1 << i) - 1)).count_ones() as usize
                }
                pub fn iter(&self) -> Iter<'_, T> {
                    Iter { set: self, mask: self.mask }
                }
            }
            impl<'a, T: Copy> IntoIterator for &'a SmallSet<T> {
                type Item = <Iter<'a, T> as Iterator>::Item;
                type IntoIter = Iter<'a, T>;
                fn into_iter(self) -> Self::IntoIter {
                    self.iter()
                }
            }
            impl<T: Copy> Default for SmallSet<T> {
                fn default() -> Self {
                    Self::new()
                }
            }
            pub struct Iter<'a, T> {
                set: &'a SmallSet<T>,
                mask: u32,
            }
            impl<'a, T: Copy> Iterator for Iter<'a, T> {
                type Item = (u8, T);
                fn next(&mut self) -> Option<Self::Item> {
                    (self.mask != 0)
                        .then(|| {
                            let i = self.mask.trailing_zeros() as u8;
                            self.mask &= self.mask - 1;
                            (i, self.set.get(i).unwrap())
                        })
                }
            }
        }
        use std::ops::Index;
        use small_set::{Mask, SmallSet};
        pub struct AhoCorasick {
            nodes: Vec<Node>,
        }
        impl AhoCorasick {
            pub fn size(&self) -> usize {
                self.nodes.len()
            }
            pub fn root(&self) -> usize {
                0
            }
            pub fn trans(&self, mut id: usize, c: u8) -> usize {
                loop {
                    if let Some(next) = self.nodes[id].trans(c) {
                        return next;
                    }
                    if id == self.root() {
                        return self.root();
                    }
                    id = self[id].suffix();
                }
            }
        }
        impl Index<usize> for AhoCorasick {
            type Output = Node;
            fn index(&self, index: usize) -> &Self::Output {
                &self.nodes[index]
            }
        }
        pub struct Builder<const K: u8> {
            nodes: Vec<Node>,
        }
        impl<const K: u8> Builder<K> {
            #[allow(clippy::new_without_default)]
            pub fn new() -> Self {
                assert!(K as usize <= std::mem::size_of::< Mask > () * 8);
                let root = Node::new(!0);
                Self { nodes: vec![root] }
            }
            pub fn insert(&mut self, s: &[u8]) -> usize {
                let mut current = 0;
                for &c in s {
                    current = self
                        .nodes[current]
                        .trans(c)
                        .unwrap_or_else(|| {
                            let next = self.nodes.len();
                            self.nodes[current].next.insert(c, next as u32);
                            self.nodes.push(Node::new(current));
                            next
                        });
                }
                current
            }
            pub fn build(self) -> AhoCorasick {
                let mut nodes = self.nodes;
                let root = 0;
                let mut que = std::collections::VecDeque::from([root]);
                while let Some(current) = que.pop_front() {
                    let current_node = std::mem::take(&mut nodes[current]);
                    for (c, next) in &current_node.next {
                        let next = next as usize;
                        let mut link = current_node.link as usize;
                        while link != root && nodes[link].trans(c).is_none() {
                            link = nodes[link].link as usize;
                        }
                        nodes[next].link = nodes[link].trans(c).unwrap_or(root) as u32;
                        que.push_back(next);
                    }
                    nodes[current] = current_node;
                }
                AhoCorasick { nodes }
            }
        }
        #[derive(Default)]
        pub struct Node {
            parent: u32,
            next: SmallSet<u32>,
            link: u32,
        }
        impl Node {
            pub fn new(parent: usize) -> Self {
                Self {
                    parent: parent as u32,
                    next: SmallSet::new(),
                    link: 0,
                }
            }
            pub fn prefix(&self) -> Option<usize> {
                Some(self.parent as usize).filter(|&p| p != !0)
            }
            pub fn suffix(&self) -> usize {
                self.link as usize
            }
            pub fn trans(&self, c: u8) -> Option<usize> {
                self.next.get(c).map(|next| next as usize)
            }
            pub fn next(&self) -> impl Iterator<Item = (u8, usize)> {
                self.next.iter().map(|(c, to)| (c, to as usize))
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
}
