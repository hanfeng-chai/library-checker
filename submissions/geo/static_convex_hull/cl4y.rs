use std::io::{Read, Write};

type Point = (i32, i32);

#[inline(always)]
fn cross(o: Point, a: Point, b: Point) -> i64 {
    let ax = a.0 as i64 - o.0 as i64;
    let ay = a.1 as i64 - o.1 as i64;
    let bx = b.0 as i64 - o.0 as i64;
    let by = b.1 as i64 - o.1 as i64;

    ax * by - ay * bx
}

fn convex_hull(p: &[Point], hull: &mut Vec<Point>) {
    hull.clear();

    let n = p.len();

    if n <= 2 {
        hull.extend_from_slice(p);
        return;
    }

    // 凸包构造过程中最多使用约 2n 个位置。
    hull.reserve(2 * n);

    let dst = hull.as_mut_ptr();
    let mut top = 0usize;

    unsafe {
        // 下凸壳：从左往右。
        for &v in p {
            while top >= 2
                && cross(
                    *dst.add(top - 2),
                    *dst.add(top - 1),
                    v,
                ) <= 0
            {
                top -= 1;
            }

            dst.add(top).write(v);
            top += 1;
        }

        let lower_size = top;

        // 上凸壳：从右往左。
        for i in (0..n - 1).rev() {
            let v = *p.get_unchecked(i);

            while top > lower_size
                && cross(
                    *dst.add(top - 2),
                    *dst.add(top - 1),
                    v,
                ) <= 0
            {
                top -= 1;
            }

            dst.add(top).write(v);
            top += 1;
        }

        // 最后一个点和第一个点重复。
        hull.set_len(top - 1);
    }
}

struct Input {
    cursor: *const u8,
}

impl Input {
    fn stdin() -> Self {
        #[cfg(unix)]
        {
            use std::ffi::{c_int, c_void};
            use std::os::fd::FromRawFd;

            const PROT_READ: c_int = 1;
            const MAP_PRIVATE: c_int = 2;

            #[link(name = "c")]
            unsafe extern "C" {
                fn mmap(
                    addr: *mut c_void,
                    len: usize,
                    prot: c_int,
                    flags: c_int,
                    fd: c_int,
                    offset: isize,
                ) -> *mut c_void;
            }

            let mut stdin = unsafe {
                std::fs::File::from_raw_fd(0)
            };

            if let Ok(meta) = stdin.metadata() {
                if meta.is_file() {
                    let len = meta.len() as usize;

                    if len != 0 {
                        let ptr = unsafe {
                            mmap(
                                std::ptr::null_mut(),
                                len,
                                PROT_READ,
                                MAP_PRIVATE,
                                0,
                                0,
                            )
                        };

                        if ptr as isize != -1 {
                            return Self {
                                cursor: ptr as *const u8,
                            };
                        }
                    }
                }
            }

            // stdin 是管道时，直接读入内存。
            let mut buf = Vec::new();
            stdin.read_to_end(&mut buf).unwrap();

            // 防止快速读取时越过末尾。
            buf.extend_from_slice(&[b'\n'; 16]);

            let ptr = Box::leak(buf.into_boxed_slice()).as_ptr();
            Self { cursor: ptr }
        }

        #[cfg(not(unix))]
        {
            let mut buf = Vec::new();
            std::io::stdin().read_to_end(&mut buf).unwrap();

            buf.extend_from_slice(&[b'\n'; 16]);

            let ptr = Box::leak(buf.into_boxed_slice()).as_ptr();
            Self { cursor: ptr }
        }
    }

    #[inline(always)]
    unsafe fn peek_u8(&self) -> u8 {
        unsafe {
            *self.cursor
        }
    }

    #[inline(always)]
    unsafe fn next_u8(&mut self) -> u8 {
        unsafe {
            let value = *self.cursor;
            self.cursor = self.cursor.add(1);
            value
        }
    }

    #[inline(always)]
    unsafe fn skip_whitespace(&mut self) {
        unsafe {
            while (*self.cursor).is_ascii_whitespace() {
                self.cursor = self.cursor.add(1);
            }
        }
    }

    /*
        尝试一次读取连续的 8 个数字。

        例如当前输入为：
            12345678

        那么可以通过若干次并行乘法直接得到 12345678，
        避免逐位执行 value = value * 10 + digit。
    */
    #[inline(always)]
    unsafe fn parse_8digits(&mut self) -> Option<u64> {
        unsafe {
            let mut value =
                std::ptr::read_unaligned(self.cursor as *const u64)
                    ^ 0x3030303030303030;

            // 任意一个字节不是 0..9。
            if value & 0xf0f0f0f0f0f0f0f0 != 0 {
                return None;
            }

            self.cursor = self.cursor.add(8);

            value = value.wrapping_mul((10 << 8) + 1)
                >> 8
                & 0x00ff00ff00ff00ff;

            value = value.wrapping_mul((100 << 16) + 1)
                >> 16
                & 0x0000ffff0000ffff;

            value = value.wrapping_mul((10000u64 << 32) + 1)
                >> 32;

            Some(value)
        }
    }

    #[inline(always)]
    unsafe fn parse_remaining_digits(&mut self, mut value: u64) -> u64 {
        unsafe {
            loop {
                let c = self.next_u8();

                if c.is_ascii_whitespace() {
                    break;
                }

                value = value * 10 + (c - b'0') as u64;
            }

            value
        }
    }

    #[inline(always)]
    fn next_usize(&mut self) -> usize {
        unsafe {
            self.skip_whitespace();

            let value = self.parse_8digits().unwrap_or(0);
            self.parse_remaining_digits(value) as usize
        }
    }

    #[inline(always)]
    fn next_i32(&mut self) -> i32 {
        unsafe {
            self.skip_whitespace();

            let negative = self.peek_u8() == b'-';

            if negative {
                self.cursor = self.cursor.add(1);
            }

            let value = self.parse_8digits().unwrap_or(0);
            let value = self.parse_remaining_digits(value) as i32;

            if negative {
                -value
            } else {
                value
            }
        }
    }
}

const OUTPUT_BUFFER_SIZE: usize = 1 << 18;
const MIN_OUTPUT_CAPACITY: usize = 50;

struct Output<W: Write> {
    buf: [u8; OUTPUT_BUFFER_SIZE],
    pos: usize,
    out: W,
}

impl<W: Write> Output<W> {
    fn new(out: W) -> Self {
        Self {
            buf: [0; OUTPUT_BUFFER_SIZE],
            pos: 0,
            out,
        }
    }

    #[cold]
    fn flush(&mut self) {
        self.out
            .write_all(&self.buf[..self.pos])
            .unwrap();

        self.pos = 0;
    }

    #[inline(always)]
    fn ensure_capacity(&mut self) {
        if OUTPUT_BUFFER_SIZE - self.pos < MIN_OUTPUT_CAPACITY {
            self.flush();
        }
    }

    #[inline(always)]
    unsafe fn write_byte(&mut self, c: u8) {
        unsafe {
            *self.buf.get_unchecked_mut(self.pos) = c;
            self.pos += 1;
        }
    }

    /*
        TABLE 中存储 0000 到 9999 的字符表示。

        输出一个四位数时可以直接复制 4 个字节，
        不需要逐位除以 10。
    */
    #[inline(always)]
    unsafe fn write_4digits<const LEADING_ZERO: bool>(
        &mut self,
        value: usize,
    ) {
        static TABLE: [u8; 40_000] = {
            let mut table = [b'0'; 40_000];
            let mut i = 0usize;

            while i < 10_000 {
                table[i * 4] += (i / 1000) as u8;
                table[i * 4 + 1] += (i / 100 % 10) as u8;
                table[i * 4 + 2] += (i / 10 % 10) as u8;
                table[i * 4 + 3] += (i % 10) as u8;
                i += 1;
            }

            table
        };

        let skip = if LEADING_ZERO {
            0
        } else {
            (value < 10) as usize
                + (value < 100) as usize
                + (value < 1000) as usize
        };

        unsafe {
            let src = TABLE.as_ptr().add(value * 4 + skip);
            let dst = self.buf.as_mut_ptr().add(self.pos);

            std::ptr::copy_nonoverlapping(
                src,
                dst,
                4 - skip,
            );

            self.pos += 4 - skip;
        }
    }

    #[inline(always)]
    unsafe fn write_u32_unchecked(&mut self, value: u32) {
        unsafe {
            if value >= 100_000_000 {
                self.write_4digits::<false>(
                    (value / 100_000_000) as usize,
                );

                self.write_4digits::<true>(
                    (value / 10_000 % 10_000) as usize,
                );

                self.write_4digits::<true>(
                    (value % 10_000) as usize,
                );
            } else if value >= 10_000 {
                self.write_4digits::<false>(
                    (value / 10_000) as usize,
                );

                self.write_4digits::<true>(
                    (value % 10_000) as usize,
                );
            } else {
                self.write_4digits::<false>(value as usize);
            }
        }
    }

    #[inline(always)]
    fn write_i32(&mut self, value: i32, end: u8) {
        self.ensure_capacity();

        unsafe {
            if value < 0 {
                self.write_byte(b'-');
            }

            self.write_u32_unchecked(value.unsigned_abs());
            self.write_byte(end);
        }
    }

    #[inline(always)]
    fn writeln_usize(&mut self, value: usize) {
        self.ensure_capacity();

        unsafe {
            self.write_u32_unchecked(value as u32);
            self.write_byte(b'\n');
        }
    }
}

impl<W: Write> Drop for Output<W> {
    fn drop(&mut self) {
        if self.pos != 0 {
            self.flush();
        }
    }
}

fn main() {
    let mut input = Input::stdin();
    let stdout = std::io::stdout();
    let mut output = Output::new(stdout.lock());

    let t = input.next_usize();

    // 所有测试复用这两个数组。
    let mut points: Vec<Point> = Vec::new();
    let mut hull: Vec<Point> = Vec::new();

    for _ in 0..t {
        let n = input.next_usize();

        points.clear();

        /*
            因为 clear 后 len = 0，所以 reserve(n)
            保证容量至少为 n。
        */
        points.reserve(n);

        unsafe {
            let dst = points.as_mut_ptr();

            for i in 0..n {
                let x = input.next_i32();
                let y = input.next_i32();

                dst.add(i).write((x, y));
            }

            points.set_len(n);
        }

        points.sort_unstable();
        points.dedup();

        convex_hull(&points, &mut hull);

        output.writeln_usize(hull.len());

        for &(x, y) in &hull {
            output.write_i32(x, b' ');
            output.write_i32(y, b'\n');
        }
    }
}