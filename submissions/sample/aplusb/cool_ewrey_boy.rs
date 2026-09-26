pub mod input {
    use std::fs::File;
    use std::io::{Stdin, Read};

    enum InputSourse {
        Stdin(Stdin),
        File(File),
    }

    pub struct Input {
        input: InputSourse,
        buf: Vec<u8>,
        at: usize,
        buf_read: usize,
    }

    impl Input {
        const DEFAULT_BUF_SIZE: usize = 4096;
        pub fn file(file: File) -> Self {
            Self {
                input: InputSourse::File(file),
                buf: vec![0; Self::DEFAULT_BUF_SIZE],
                at: 0,
                buf_read: 0,
            }
        }

        pub fn stdin() -> Self {
            Self {
                input: InputSourse::Stdin(std::io::stdin()),
                buf: vec![0; Self::DEFAULT_BUF_SIZE],
                at: 0,
                buf_read: 0,
            }
        }

        pub fn get(&mut self) -> Option<u8> {
            if self.refill_buffer() {
                let res = self.buf[self.at];
                self.at += 1;
                if res == b'\r' {
                    if self.refill_buffer() && self.buf[self.at] == b'\n' {
                        self.at += 1;
                    }
                    return Some(b'\n');
                }
                Some(res)
            } else {
                None
            }
        }

        pub fn read_i32(&mut self) -> i32 {
            self.skip_whitespace();
            let mut c = self.get().unwrap();
            let sgn = match c {
                b'-' => {
                    c = self.get().unwrap();
                    true
                }
                b'+' => {
                    c = self.get().unwrap();
                    false
                }
                _ => false,
            };
            let mut res = 0;
            loop {
                assert!(c.is_ascii_digit());
                res *= 10;
                let d = (c - b'0') as i32;
                if sgn {
                    res -= d;
                } else {
                    res += d;
                }
                match self.get() {
                    None => break,
                    Some(ch) => {
                        if ch.is_ascii_whitespace() {
                            break;
                        } else {
                            c = ch;
                        }
                    }
                }
            }
            res
        }

        pub fn peek(&mut self) -> Option<u8> {
            if self.refill_buffer() {
                let res = self.buf[self.at];
                Some(if res == b'\r' { b'\n' } else { res })
            } else {
                None
            }
        }

        pub fn skip_whitespace(&mut self) {
            while let Some(c) = self.peek() {
                if  !c.is_ascii_whitespace() {
                    return;
                }
                self.get();
            }
        }

        fn refill_buffer(&mut self) -> bool {
            if self.at == self.buf_read {
                self.at = 0;
                self.buf_read = match &mut self.input {
                    InputSourse::File(file) => file.read(&mut self.buf).unwrap(),
                    InputSourse::Stdin(stdin) => stdin.read(&mut self.buf).unwrap(),
                };
                self.buf_read != 0
            } else {
                true
            }
        }

    }
}
fn main() {
    let mut input = input::Input::stdin();
    let a = input.read_i32();
    let b = input.read_i32();
    println!("{}", a + b);
}
