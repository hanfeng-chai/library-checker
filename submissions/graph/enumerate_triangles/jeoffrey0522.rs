use std::io::Write;

mod fast_io {
    use std::fs::File;
    use std::io::BufWriter;
    use std::os::unix::io::FromRawFd;

    extern "C" {
        fn mmap(addr: usize, length: usize, prot: i32, flags: i32, fd: i32, offset: i64)
            -> *mut u8;
        fn fstat(fd: i32, stat: *mut usize) -> i32;
    }

    pub struct InputAtOnce {
        buf: &'static [u8],
    }

    impl InputAtOnce {
        fn skip(&mut self) {
            loop {
                match self.buf {
                    &[..=b' ', ..] => self.buf = &self.buf[1..],
                    _ => break,
                }
            }
        }

        fn u32_noskip(&mut self) -> u32 {
            let mut acc = 0;
            loop {
                match self.buf {
                    &[b'0'..=b'9', ..] => acc = acc * 10 + (self.buf[0] - b'0') as u32,
                    _ => break,
                }
                self.buf = &self.buf[1..];
            }
            acc
        }

        pub fn token(&mut self) -> &'static str {
            self.skip();
            let start = self.buf.as_ptr();
            loop {
                match self.buf {
                    &[..=b' ', ..] => break,
                    _ => self.buf = &self.buf[1..],
                }
            }
            let end = self.buf.as_ptr();
            unsafe {
                std::str::from_utf8_unchecked(std::slice::from_raw_parts(
                    start,
                    end.offset_from(start) as usize,
                ))
            }
        }

        pub fn value<T: std::str::FromStr>(&mut self) -> T
        where
            T::Err: std::fmt::Debug,
        {
            self.token().parse().unwrap()
        }

        pub fn u32(&mut self) -> u32 {
            self.skip();
            self.u32_noskip()
        }

        pub fn i32(&mut self) -> i32 {
            self.skip();
            match self.buf {
                &[b'-', ..] => {
                    self.buf = &self.buf[1..];
                    -(self.u32_noskip() as i32)
                }
                _ => self.u32_noskip() as i32,
            }
        }
    }

    pub fn stdin() -> InputAtOnce {
        let mut stat = [0; 18];
        unsafe { fstat(0, (&mut stat).as_mut_ptr()) };
        let buf = unsafe { mmap(0, stat[6], 1, 2, 0, 0) };
        let buf =
            unsafe { std::str::from_utf8_unchecked(std::slice::from_raw_parts(buf, stat[6])) };
        InputAtOnce {
            buf: buf.as_bytes(),
        }
    }

    pub fn stdout() -> BufWriter<File> {
        let stdout = unsafe { File::from_raw_fd(1) };
        BufWriter::with_capacity(1 << 16, stdout)
    }
}

fn main() {
    let mut input = fast_io::stdin();
    let mut output = fast_io::stdout();

    let n: usize = input.value();
    let m: usize = input.value();
    let xs: Vec<u64> = (0..n).map(|_| input.value()).collect();

    // order by degree
    let mut degree = vec![0u32; n];
    let mut edges = vec![];
    for _ in 0..m {
        let u = input.u32();
        let v = input.u32();
        edges.push([u, v]);
        degree[u as usize] += 1;
        degree[v as usize] += 1;
    }

    let key = |u| (degree[u as usize], u);
    let mut heads = vec![0u32; n + 1];
    for [u, v] in &mut edges {
        if !(key(*u) > key(*v)) {
            std::mem::swap(u, v);
        }
        heads[*u as usize] += 1;
    }
    for u in 0..n {
        heads[u + 1] += heads[u];
    }
    drop(degree);

    let mut links = vec![0u32; edges.len()];
    for [u, v] in edges {
        heads[u as usize] -= 1;
        links[heads[u as usize] as usize] = v;
    }
    let children = |u| &links[heads[u] as usize..heads[u + 1] as usize];

    const P: u64 = 998244353;
    let mut ans = 0u64;
    let mut marker = vec![0u64; n];
    for u in 0..n {
        for &v in children(u) {
            marker[v as usize] = xs[v as usize];
        }

        let mut acc0 = 0;
        for &v in children(u) {
            let mut acc1 = 0;
            for &w in children(v as usize) {
                if marker[w as usize] != 0 {
                    acc1 += marker[w as usize];
                }
            }
            acc0 = (acc0 + acc1 % P * xs[v as usize]) % P;
        }
        ans = (ans + xs[u as usize] * acc0) % P;

        for &v in children(u) {
            marker[v as usize] = 0;
        }
    }

    writeln!(output, "{}", ans).unwrap();
}
