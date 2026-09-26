use urectanc::{compressed_sparse_row::CSRArray, fast_io};

fn main() {
    let mut input = fast_io::stdin();
    let mut output = fast_io::stdout();

    let n: usize = input.val();
    let m: usize = input.val();
    let mut edges = Vec::with_capacity(2 * m);
    for i in 0..m {
        let u: usize = input.val();
        let v: usize = input.val();
        if u == v {
            output.writeln(1);
            output.writeln(u);
            output.writeln(i);
            return;
        }
        edges.push((u, (v as u32, i as u32)));
        edges.push((v, (u as u32, i as u32)));
    }
    let graph = CSRArray::new(n, &edges);

    let mut stack = vec![];
    let mut seen = vec![false; n];
    for r in 0..n {
        if seen[r] {
            continue;
        }
        seen[r] = true;
        stack.push((r as u32, 0u32, !0u32));

        while let Some((v, i, pe)) = stack.pop() {
            if i == 0 {
                seen[v as usize] = true;
            }

            if let Some(&(u, e)) = graph[v as usize].get(i as usize) {
                if e == pe {
                    stack.push((v, i + 1, pe));
                    continue;
                }

                if !seen[u as usize] {
                    stack.push((v, i + 1, pe));
                    stack.push((u, 0, e));
                    continue;
                }

                let j = stack.iter().rposition(|&t| t.0 == u).unwrap();
                output.writeln(stack.len() - j + 1);
                for &(v, _, _) in &stack[j..] {
                    output.write(v);
                }
                output.writeln(v);
                for &(_, _, e) in &stack[j + 1..] {
                    output.write(e);
                }
                output.write(pe);
                output.writeln(e);
                return;
            }
        }
    }
    output.writeln(-1);
}


pub mod urectanc {
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
                T: Copy,
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
                let mut csr: Vec<T> = Vec::with_capacity(m);
                {
                    let csr = csr.spare_capacity_mut();
                    for &(k, v) in items.iter().rev() {
                        index[k] -= 1;
                        csr[index[k]].write(v);
                    }
                }
                unsafe {
                    csr.set_len(m);
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
                Row { csr: self, index: 0 }
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
