// Yosupo-focused build/runtime contract:
// - This file intentionally keeps one x86_64 AVX2/BMI implementation. The
//   example-local .cargo/config.toml makes normal Cargo commands default to
//   x86_64-apple-darwin for Rosetta testing, while the Makefile keeps explicit
//   x86_64-unknown-linux-gnu targets for the judge-shaped Linux build.
// - The fast input path assumes stdin is a regular file: lseek gets the input
//   length, then mmap maps stdin from offset 0. Pipes are outside this path.
// - Input is trusted to satisfy the problem constraints: a pair count followed
//   by exactly 2n signed i128 decimal tokens separated by ASCII whitespace.
//   Parsing therefore spends work on delimiter discovery, not validation.
// - Output is written into an anonymous mmap buffer and flushed in large chunks;
//   the formatter may add leading spaces, so reserve 43 token bytes plus '\n'.
// - There is no target_arch cfg in this source. Target selection and CPU
//   feature flags live in Cargo/Makefile configuration. Stable Rust does not
//   allow inline(always) together with target_feature, so private AVX helpers
//   use explicit unsafe intrinsic calls under that crate-level build contract.

use std::arch::x86_64;

const POW10_8_U64: u64 = 100_000_000;
const POW10_16_U64: u64 = 10_000_000_000_000_000;
const POW10_19_U64: u64 = 10_000_000_000_000_000_000;
static DIGIT_TABLE: [[u8; 4]; 10_000] = make_digit_table();
static POSITIVE_LEADING_TABLE: [[u8; 4]; 10_000] = make_leading_table(false);
static NEGATIVE_LEADING_TABLE: [[u8; 4]; 10_000] = make_leading_table(true);

const fn make_digit_table() -> [[u8; 4]; 10_000] {
    let mut table = [[0u8; 4]; 10_000];
    let mut value = 0usize;
    while value != table.len() {
        table[value] = [
            b'0' + (value / 1000) as u8,
            b'0' + (value / 100 % 10) as u8,
            b'0' + (value / 10 % 10) as u8,
            b'0' + (value % 10) as u8,
        ];
        value += 1;
    }
    table
}

const fn make_leading_table(negative: bool) -> [[u8; 4]; 10_000] {
    let mut table = make_digit_table();
    let mut value = 0usize;
    while value != table.len() {
        if value < 1000 {
            table[value][0] = b' ';
        }
        if value < 100 {
            table[value][1] = b' ';
        }
        if value < 10 {
            table[value][2] = b' ';
        }
        if negative {
            if value == 0 {
                table[value][3] = b'-';
            } else if value < 10 {
                table[value][2] = b'-';
            } else if value < 100 {
                table[value][1] = b'-';
            } else if value < 1000 {
                table[value][0] = b'-';
            }
        }
        value += 1;
    }
    table
}

#[inline(always)]
fn is_whitespace(byte: u8) -> bool {
    // Sanitized tokens contain only '-' and digits, so any ASCII control byte or space is a separator.
    byte <= b' '
}

#[inline(always)]
fn div_rem_1e4(value: u32) -> (u32, u32) {
    let quotient = ((u64::from(value) * 0x68db8bb) >> 40) as u32;
    (quotient, value - quotient * 10_000)
}

#[inline(always)]
unsafe fn write_four_digits(mut output: *mut u8, value: u32) -> *mut u8 {
    unsafe {
        let digits = DIGIT_TABLE.as_ptr().add(value as usize).cast::<u32>();
        output
            .cast::<u32>()
            .write_unaligned(digits.read_unaligned());
        output = output.add(4);
    }
    output
}

#[inline(always)]
unsafe fn write_eight_digits(mut output: *mut u8, value: u32) -> *mut u8 {
    let (quotient, remainder) = div_rem_1e4(value);
    unsafe {
        output = write_four_digits(output, quotient);
        output = write_four_digits(output, remainder);
    }
    output
}

#[inline(always)]
fn div_rem_1e8(value: u64) -> (u32, u32) {
    let quotient = (value / POW10_8_U64) as u32;
    (quotient, (value - u64::from(quotient) * POW10_8_U64) as u32)
}

#[inline(always)]
unsafe fn write_sixteen_digits(mut output: *mut u8, value: u64) -> *mut u8 {
    let (quotient, remainder) = div_rem_1e8(value);
    unsafe {
        output = write_eight_digits(output, quotient);
        output = write_eight_digits(output, remainder);
    }
    output
}

#[inline(always)]
unsafe fn write_nineteen_digits(mut output: *mut u8, value: u64) -> *mut u8 {
    let high = (value / POW10_16_U64) as u32;
    let low = value - u64::from(high) * POW10_16_U64;
    unsafe {
        let digits = DIGIT_TABLE.as_ptr().add(high as usize).cast::<u8>();
        *output = *digits.add(1);
        *output.add(1) = *digits.add(2);
        *output.add(2) = *digits.add(3);
        output = write_sixteen_digits(output.add(3), low);
    }
    output
}

#[inline(always)]
fn div_rem_u128_by_1e19(value: u128) -> (u64, u64) {
    const M_PRIME: u128 = 0xd83c_94fb_6d2a_c34a_5663_d3c7_a0d8_65cb;
    let high = value.carrying_mul(M_PRIME, 0).1;
    let (sum, carry) = value.overflowing_add(high);
    let quotient = ((sum >> 64) as u64) + u64::from(carry);
    let remainder = (value - u128::from(quotient) * u128::from(POW10_19_U64)) as u64;
    debug_assert!(remainder < POW10_19_U64);
    (quotient, remainder)
}

static RIGHT_ALIGN_16_SHUFFLE: [[u8; 16]; 17] = {
    let mut table = [[0x80u8; 16]; 17];
    let mut length = 1usize;
    while length != table.len() {
        let mut index = 16 - length;
        let mut source = 0u8;
        while index != 16 {
            table[length][index] = source;
            index += 1;
            source += 1;
        }
        length += 1;
    }
    table
};

struct MappedInput {
    buffer: *const u8,
    cursor: *const u8,
    end: *const u8,
    ascii_zero: x86_64::__m256i,
}

impl MappedInput {
    fn from_stdin() -> std::io::Result<Self> {
        let input_length = unsafe { lseek(0, 0, SEEK_END) };
        if input_length < 0 {
            return Err(std::io::Error::last_os_error());
        }
        let input_length = input_length as usize;

        let buffer = if input_length == 0 {
            std::ptr::NonNull::<u8>::dangling().as_ptr()
        } else {
            let input_buffer = map_memory(input_length, PROT_READ, MAP_PRIVATE, 0)?;
            let _ = unsafe { madvise(input_buffer, input_length, MADV_SEQUENTIAL) };
            input_buffer.cast::<u8>()
        };

        Ok(Self {
            buffer,
            cursor: buffer,
            end: unsafe { buffer.add(input_length) },
            ascii_zero: unsafe { x86_64::_mm256_set1_epi8(b'0' as i8) },
        })
    }

    #[inline(always)]
    unsafe fn read_i128(&mut self) -> i128 {
        debug_assert!(unsafe { self.cursor.offset_from(self.buffer) >= 0 });
        debug_assert!(unsafe { self.end.offset_from(self.cursor) >= 0 });

        let mut current = self.cursor;
        let negative = unsafe { *current == b'-' };
        if negative {
            current = unsafe { current.add(1) };
        }

        // One token usually fits in the first 32-byte load. Subtracting '0'
        // makes any byte before '0' set the high bit, so movemask identifies
        // the delimiter without a separate whitespace comparison.
        let remaining = unsafe { self.end.offset_from(current) as usize };
        let magnitude = if remaining < 32 {
            let mut value = 0u128;
            while current != self.end && unsafe { *current } >= b'0' {
                value = value * 10 + u128::from(unsafe { *current } - b'0');
                current = unsafe { current.add(1) };
            }
            value
        } else {
            let digits = unsafe {
                x86_64::_mm256_sub_epi8(
                    x86_64::_mm256_loadu_si256(current.cast::<x86_64::__m256i>()),
                    self.ascii_zero,
                )
            };
            let delimiter_mask = unsafe { x86_64::_mm256_movemask_epi8(digits) } as u32;

            if delimiter_mask == 0 {
                let magnitude = parse_32_digit_chunk(parse_32_digits_from_nibbles(digits));
                current = unsafe { current.add(32) };
                let mut tail = 0u32;
                let mut place = 1u128;

                if unsafe { self.end.offset_from(current) } >= 4 {
                    let mut chunk = unsafe { current.cast::<u32>().read_unaligned() } ^ 0x3030_3030;
                    if chunk & 0xf0f0_f0f0 == 0 {
                        chunk = (chunk * 10 + (chunk >> 8)) & 0x00ff_00ff;
                        tail = (chunk * 100 + (chunk >> 16)) & 0x0000_ffff;
                        place = 10_000;
                        current = unsafe { current.add(4) };
                    }
                }

                while current != self.end && unsafe { *current } >= b'0' {
                    tail = tail * 10 + u32::from(unsafe { *current } - b'0');
                    place *= 10;
                    current = unsafe { current.add(1) };
                }

                magnitude * place + u128::from(tail)
            } else {
                let length = delimiter_mask.trailing_zeros() as usize;
                debug_assert!(length != 0);
                let value = parse_prefix_digits(digits, length);
                current = unsafe { current.add(length) };
                value
            }
        };

        if current != self.end {
            debug_assert!(is_whitespace(unsafe { *current }));
            current = unsafe { current.add(1) };
            // Official input uses one separator in the common path; keep accepting
            // repeated whitespace by scanning only after that first byte is gone.
            while current != self.end && is_whitespace(unsafe { *current }) {
                current = unsafe { current.add(1) };
            }
        }
        self.cursor = current;

        let sign_mask = 0u128.wrapping_sub(u128::from(negative));
        ((magnitude ^ sign_mask).wrapping_sub(sign_mask)) as i128
    }
}

#[inline(always)]
fn parse_prefix_digits(input: x86_64::__m256i, length: usize) -> u128 {
    debug_assert!(length < 32);

    if length <= 16 {
        let shuffle = unsafe {
            x86_64::_mm_loadu_si128(
                RIGHT_ALIGN_16_SHUFFLE[length]
                    .as_ptr()
                    .cast::<x86_64::__m128i>(),
            )
        };
        let aligned =
            unsafe { x86_64::_mm_shuffle_epi8(x86_64::_mm256_castsi256_si128(input), shuffle) };
        u128::from(parse_two_u32_lanes(parse_16_digits_from_nibbles(aligned)))
    } else {
        let mut buffer = [0u8; 64];
        unsafe {
            x86_64::_mm256_storeu_si256(
                buffer
                    .as_mut_ptr()
                    .add(32 - length)
                    .cast::<x86_64::__m256i>(),
                input,
            );
        }
        let aligned =
            unsafe { x86_64::_mm256_loadu_si256(buffer.as_ptr().cast::<x86_64::__m256i>()) };
        parse_32_digit_chunk(parse_32_digits_from_nibbles(aligned))
    }
}

#[inline(always)]
fn parse_32_digit_chunk(chunk: x86_64::__m256i) -> u128 {
    let low = unsafe { x86_64::_mm256_castsi256_si128(chunk) };
    let high = unsafe { x86_64::_mm256_extracti128_si256::<1>(chunk) };
    let low_16 = parse_two_u32_lanes(low);
    let high_16 = parse_two_u32_lanes(high);
    u128::from(low_16) * u128::from(POW10_16_U64) + u128::from(high_16)
}

#[inline(always)]
fn parse_32_digits_from_nibbles(input: x86_64::__m256i) -> x86_64::__m256i {
    // Collapse 32 ASCII digits to four base-1e8 lanes with the usual
    // maddubs/madd/pack/madd sequence; the caller combines 16-digit halves.
    unsafe {
        let pair_weights = x86_64::_mm256_set1_epi16(0x010a);
        let quad_weights = x86_64::_mm256_set1_epi32(0x0001_0064);
        let octet_weights = x86_64::_mm256_set1_epi32(0x0001_2710);
        let pairs = x86_64::_mm256_maddubs_epi16(input, pair_weights);
        let quads = x86_64::_mm256_madd_epi16(pairs, quad_weights);
        let packed = x86_64::_mm256_packus_epi32(quads, quads);
        x86_64::_mm256_madd_epi16(packed, octet_weights)
    }
}

#[inline(always)]
fn parse_16_digits_from_nibbles(input: x86_64::__m128i) -> x86_64::__m128i {
    unsafe {
        let pair_weights = x86_64::_mm_set1_epi16(0x010a);
        let quad_weights = x86_64::_mm_set1_epi32(0x0001_0064);
        let octet_weights = x86_64::_mm_set1_epi32(0x0001_2710);
        let pairs = x86_64::_mm_maddubs_epi16(input, pair_weights);
        let quads = x86_64::_mm_madd_epi16(pairs, quad_weights);
        let packed = x86_64::_mm_packus_epi32(quads, quads);
        x86_64::_mm_madd_epi16(packed, octet_weights)
    }
}

#[inline(always)]
fn parse_two_u32_lanes(chunk: x86_64::__m128i) -> u64 {
    let low = unsafe { x86_64::_mm_cvtsi128_si32(chunk) } as u64;
    let high = unsafe { x86_64::_mm_extract_epi32::<1>(chunk) } as u64;
    low * 100_000_000 + high
}

#[inline(always)]
unsafe fn write_u128_leading_digits(mut output: *mut u8, value: u128, negative: bool) -> *mut u8 {
    if value <= u128::from(u64::MAX) {
        return unsafe { write_u64_leading_digits(output, value as u64, negative) };
    }

    let (quotient, low) = div_rem_u128_by_1e19(value);
    unsafe {
        output = write_u64_leading_digits(output, quotient, negative);
        write_nineteen_digits(output, low)
    }
}

#[inline(always)]
unsafe fn write_u64_leading_digits(mut output: *mut u8, value: u64, negative: bool) -> *mut u8 {
    if value < POW10_8_U64 {
        unsafe { write_u32_leading_digits(output, value as u32, negative) }
    } else if value < POW10_16_U64 {
        let (quotient, remainder) = div_rem_1e8(value);
        unsafe {
            output = write_u32_leading_digits(output, quotient, negative);
            write_eight_digits(output, remainder)
        }
    } else {
        let quotient = (value / POW10_16_U64) as u32;
        let remainder = value - u64::from(quotient) * POW10_16_U64;
        unsafe {
            output = write_u32_leading_digits(output, quotient, negative);
            write_sixteen_digits(output, remainder)
        }
    }
}

#[inline(always)]
unsafe fn write_u32_leading_digits(mut output: *mut u8, value: u32, negative: bool) -> *mut u8 {
    if value < 10_000 {
        unsafe { write_leading_four_digits(output, value, negative) }
    } else {
        let (quotient, remainder) = div_rem_1e4(value);
        unsafe {
            output = write_leading_four_digits(output, quotient, negative);
            write_four_digits(output, remainder)
        }
    }
}

#[inline(always)]
unsafe fn write_leading_four_digits(mut output: *mut u8, value: u32, negative: bool) -> *mut u8 {
    unsafe {
        if !negative {
            return write_leading_table_digits(output, &POSITIVE_LEADING_TABLE, value);
        }
        if value >= 1000 {
            output = write_leading_table_digits(output, &NEGATIVE_LEADING_TABLE, 0);
            write_four_digits(output, value)
        } else {
            write_leading_table_digits(output, &NEGATIVE_LEADING_TABLE, value)
        }
    }
}

#[inline(always)]
unsafe fn write_leading_table_digits(
    output: *mut u8,
    table: &[[u8; 4]; 10_000],
    value: u32,
) -> *mut u8 {
    unsafe {
        let digits = table.as_ptr().add(value as usize).cast::<u32>();
        output
            .cast::<u32>()
            .write_unaligned(digits.read_unaligned());
        output.add(4)
    }
}

unsafe extern "C" {
    fn mmap(
        addr: *mut std::ffi::c_void,
        length: usize,
        prot: std::ffi::c_int,
        flags: std::ffi::c_int,
        fd: std::ffi::c_int,
        offset: isize,
    ) -> *mut std::ffi::c_void;
    fn madvise(
        addr: *mut std::ffi::c_void,
        length: usize,
        advice: std::ffi::c_int,
    ) -> std::ffi::c_int;
    fn lseek(fd: std::ffi::c_int, offset: isize, whence: std::ffi::c_int) -> isize;
    fn write(fd: std::ffi::c_int, buffer: *const std::ffi::c_void, length: usize) -> isize;
}

const PROT_READ: std::ffi::c_int = 0x01;
const PROT_WRITE: std::ffi::c_int = 0x02;
const MAP_PRIVATE: std::ffi::c_int = 0x0002;
const MADV_SEQUENTIAL: std::ffi::c_int = 2;
#[cfg(target_os = "linux")]
const MAP_ANONYMOUS: std::ffi::c_int = 0x0020;
#[cfg(target_os = "macos")]
const MAP_ANONYMOUS: std::ffi::c_int = 0x1000;
const SEEK_END: std::ffi::c_int = 2;

struct MappedOutput {
    buffer: *mut u8,
    cursor: *mut u8,
    end: *mut u8,
}

impl MappedOutput {
    const CAPACITY: usize = 1 << 17;
    const LINE_MAX_LEN: usize = 43 + 1;

    fn from_stdout() -> std::io::Result<Self> {
        let buffer = map_memory(
            Self::CAPACITY,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS,
            -1,
        )?
        .cast::<u8>();

        Ok(Self {
            buffer,
            cursor: buffer,
            end: unsafe { buffer.add(Self::CAPACITY) },
        })
    }

    fn flush(&mut self) -> std::io::Result<()> {
        let length = unsafe { self.cursor.offset_from(self.buffer) as usize };
        debug_assert!(length <= Self::CAPACITY);

        let mut written = 0usize;
        while written != length {
            let count = unsafe {
                write(
                    1,
                    self.buffer.add(written).cast::<std::ffi::c_void>(),
                    length - written,
                )
            };
            if count < 0 {
                let error = std::io::Error::last_os_error();
                if error.kind() == std::io::ErrorKind::Interrupted {
                    continue;
                }
                return Err(error);
            }
            if count == 0 {
                return Err(std::io::Error::from(std::io::ErrorKind::WriteZero));
            }
            written += count as usize;
        }

        self.cursor = self.buffer;
        Ok(())
    }

    fn write_i128_line(&mut self, value: i128) -> std::io::Result<()> {
        if unsafe { self.end.offset_from(self.cursor) as usize } < Self::LINE_MAX_LEN {
            self.flush()?;
        }

        let negative = value < 0;
        let mut cursor =
            unsafe { write_u128_leading_digits(self.cursor, value.unsigned_abs(), negative) };
        unsafe {
            *cursor = b'\n';
            cursor = cursor.add(1);
        }
        self.cursor = cursor;
        Ok(())
    }
}

fn map_memory(
    length: usize,
    prot: std::ffi::c_int,
    flags: std::ffi::c_int,
    fd: std::ffi::c_int,
) -> std::io::Result<*mut std::ffi::c_void> {
    let buffer = unsafe { mmap(std::ptr::null_mut(), length, prot, flags, fd, 0) };
    if buffer == usize::MAX as *mut std::ffi::c_void {
        Err(std::io::Error::last_os_error())
    } else {
        Ok(buffer)
    }
}

#[target_feature(enable = "avx2,bmi1,bmi2")]
unsafe fn run() -> std::io::Result<()> {
    let mut input = MappedInput::from_stdin()?;
    let mut output = MappedOutput::from_stdout()?;

    let pair_count = unsafe { input.read_i128() } as usize;
    const PAIR_COUNT_MAX: usize = 500_000;
    debug_assert!((1..=PAIR_COUNT_MAX).contains(&pair_count));

    for _ in 0..pair_count {
        let a = unsafe { input.read_i128() };
        let b = unsafe { input.read_i128() };
        const INPUT_ABS_MAX: i128 = 10_000_000_000_000_000_000_000_000_000_000_000_000;
        debug_assert!((-INPUT_ABS_MAX..=INPUT_ABS_MAX).contains(&a));
        debug_assert!((-INPUT_ABS_MAX..=INPUT_ABS_MAX).contains(&b));

        output.write_i128_line(a + b)?;
    }

    output.flush()
}

fn main() -> std::io::Result<()> {
    unsafe { run() }
}
