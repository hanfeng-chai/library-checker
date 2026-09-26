pub use __cargo_equip::prelude::*;

use std::cell::RefCell;

thread_local! {
    static OUTPUT: RefCell<String> = const { RefCell::new(String::new()) };
}

trait OutValue {
    fn write_out(&self, dst: &mut String);
}

macro_rules! impl_out_value_display {
    ($($ty:ty),* $(,)?) => {
        $(
            impl OutValue for $ty {
                fn write_out(&self, dst: &mut String) {
                    use std::fmt::Write as _;
                    write!(dst, "{}", self).unwrap();
                }
            }
        )*
    };
}

impl_out_value_display!(
    bool, char, i8, i16, i32, i64, i128, isize, u8, u16, u32, u64, u128, usize, f32, f64,
);

impl OutValue for str {
    fn write_out(&self, dst: &mut String) {
        dst.push_str(self);
    }
}

impl OutValue for String {
    fn write_out(&self, dst: &mut String) {
        dst.push_str(self);
    }
}

impl<T: OutValue + ?Sized> OutValue for &T {
    fn write_out(&self, dst: &mut String) {
        (*self).write_out(dst);
    }
}

fn write_separated<'a, I, T>(iter: I, dst: &mut String)
where
    I: IntoIterator<Item = &'a T>,
    T: OutValue + 'a,
{
    let mut iter = iter.into_iter();
    if let Some(first) = iter.next() {
        first.write_out(dst);
        for value in iter {
            dst.push(' ');
            value.write_out(dst);
        }
    }
}

impl<T: OutValue> OutValue for [T] {
    fn write_out(&self, dst: &mut String) {
        write_separated(self.iter(), dst);
    }
}

impl<T: OutValue, const N: usize> OutValue for [T; N] {
    fn write_out(&self, dst: &mut String) {
        self.as_slice().write_out(dst);
    }
}

impl<T: OutValue> OutValue for Vec<T> {
    fn write_out(&self, dst: &mut String) {
        self.as_slice().write_out(dst);
    }
}

impl<T: OutValue> OutValue for std::collections::VecDeque<T> {
    fn write_out(&self, dst: &mut String) {
        write_separated(self.iter(), dst);
    }
}

impl<T: OutValue> OutValue for std::collections::BTreeSet<T> {
    fn write_out(&self, dst: &mut String) {
        write_separated(self.iter(), dst);
    }
}

impl<A: OutValue, B: OutValue> OutValue for (A, B) {
    fn write_out(&self, dst: &mut String) {
        self.0.write_out(dst);
        dst.push(' ');
        self.1.write_out(dst);
    }
}

impl<A: OutValue, B: OutValue, C: OutValue> OutValue for (A, B, C) {
    fn write_out(&self, dst: &mut String) {
        self.0.write_out(dst);
        dst.push(' ');
        self.1.write_out(dst);
        dst.push(' ');
        self.2.write_out(dst);
    }
}

fn out_clear() {
    OUTPUT.with(|out| out.borrow_mut().clear());
}

fn out_take() -> String {
    OUTPUT.with(|out| std::mem::take(&mut *out.borrow_mut()))
}

fn out_write<T: OutValue + ?Sized>(value: &T) {
    OUTPUT.with(|out| value.write_out(&mut out.borrow_mut()));
}

fn out_space() {
    OUTPUT.with(|out| out.borrow_mut().push(' '));
}

fn out_newline() {
    OUTPUT.with(|out| out.borrow_mut().push('\n'));
}

fn with_output(f: impl FnOnce()) -> String {
    out_clear();
    f();
    out_take()
}

macro_rules! out {
    () => {{
        out_newline();
    }};
    ($first:expr $(, $rest:expr)* $(,)?) => {{
        out_write(&$first);
        $(
            out_space();
            out_write(&$rest);
        )*
        out_newline();
    }};
}

use proconio::input;

fn solve(a: i64, b: i64) -> String {
    with_output(|| out!(a + b))
}

fn main() {
    input! {
    a: i64,
    b: i64,
    }
    print!("{}", solve(a, b));
}

#[cfg(test)]
mod tests {
    pub use crate::__cargo_equip::prelude::*;

    use super::*;

    // ↓ Add test cases here
    const CASES: &[(&str, &str)] = &[
        ("1234 5678\n", "6912\n"),
        ("1000000000 1000000000\n", "2000000000\n"),
    ];

    #[test]
    fn test_samples() {
        for (i, &(input, expected)) in CASES.iter().enumerate() {
            let result = {
                use proconio::input;
                input! {
                from proconio::source::once::OnceSource::from(input),
                a: i64,
                b: i64,
                }
                solve(a, b)
            };
            assert_eq!(
                result.trim(),
                expected.trim(),
                "case {}\n--- input ---\n{}",
                i + 1,
                input
            );
        }
    }
}

// The following code was expanded by `cargo-equip`.

///  # Bundled libraries
/// 
///  - `registry+https://github.com/rust-lang/crates.io-index#proconio@0.4.5` licensed under `MIT OR Apache-2.0` as `crate::__cargo_equip::crates::proconio`
/// 
///  # License and Copyright Notices
/// 
///  - `registry+https://github.com/rust-lang/crates.io-index#proconio@0.4.5`
/// 
///      ```text
///      The MIT License
///      Copyright 2019 (C) statiolake <statiolake@gmail.com>
/// 
///      Permission is hereby granted, free of charge, to any person obtaining a copy of
///      this software and associated documentation files (the "Software"), to deal in
///      the Software without restriction, including without limitation the rights to
///      use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
///      the Software, and to permit persons to whom the Software is furnished to do so,
///      subject to the following conditions:
/// 
///      The above copyright notice and this permission notice shall be included in all
///      copies or substantial portions of the Software.
/// 
///      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
///      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
///      FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
///      COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
///      IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
///      CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
/// 
///      Copyright for original `input!` macro is held by Hideyuki Tanaka, 2019.  The
///      original macro is licensed under BSD 3-clause license.
/// 
///      Redistribution and use in source and binary forms, with or without
///      modification, are permitted provided that the following conditions are met:
/// 
///      1. Redistributions of source code must retain the above copyright notice, this
///         list of conditions and the following disclaimer.
/// 
///      2. Redistributions in binary form must reproduce the above copyright notice,
///         this list of conditions and the following disclaimer in the documentation
///         and/or other materials provided with the distribution.
/// 
///      3. Neither the name of the copyright holder nor the names of its contributors
///         may be used to endorse or promote products derived from this software
///         without specific prior written permission.
/// 
///      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
///      ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
///      WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
///      DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
///      FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
///      DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
///      SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
///      CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
///      OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
///      OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///      ```
#[cfg_attr(any(), rustfmt::skip)]
#[allow(unused)]
mod __cargo_equip {
    pub(crate) mod crates {
        pub mod proconio {#![allow(clippy::needless_doctest_main,clippy::print_literal)]pub use crate::__cargo_equip::macros::proconio::*;pub mod marker{use crate::__cargo_equip::crates::proconio::source::{Readable,Source};use std::io::BufRead;pub enum Chars{}impl Readable for Chars{type Output=Vec<char>;fn read<R:BufRead,S:Source<R>>(source:&mut S)->Vec<char>{source.next_token_unwrap().chars().collect()}}pub enum Bytes{}impl Readable for Bytes{type Output=Vec<u8>;fn read<R:BufRead,S:Source<R>>(source:&mut S)->Vec<u8>{source.next_token_unwrap().bytes().collect()}}pub enum Usize1{}impl Readable for Usize1{type Output=usize;fn read<R:BufRead,S:Source<R>>(source:&mut S)->usize{usize::read(source).checked_sub(1).expect("attempted to read the value 0 as a Usize1")}}pub enum Isize1{}impl Readable for Isize1{type Output=isize;fn read<R:BufRead,S:Source<R>>(source:&mut S)->isize{isize::read(source).checked_sub(1).unwrap_or_else(||{panic!(concat!("attempted to read the value {} as a Isize1:"," the value is isize::MIN and cannot be decremented"),std::isize::MIN,)})}}}pub mod source{use std::any::type_name;use std::fmt::Debug;use std::io::BufRead;use std::str::FromStr;pub mod line{use super::Source;use crate::__cargo_equip::crates::proconio::source::tokens::Tokens;use std::io::BufRead;pub struct LineSource<R:BufRead>{tokens:Tokens,reader:R,}impl<R:BufRead>LineSource<R>{pub fn new(reader:R)->LineSource<R>{LineSource{tokens:"".to_owned().into(),reader,}}fn prepare(&mut self){while self.tokens.is_empty(){let mut line=String::new();let num_bytes=self.reader.read_line(&mut line).expect("failed to get linel maybe an IO error.");if num_bytes==0{return;}self.tokens=line.into();}}}impl<R:BufRead>Source<R>for LineSource<R>{fn next_token(&mut self)->Option<&str>{self.prepare();self.tokens.next_token()}fn is_empty(&mut self)->bool{self.prepare();self.tokens.is_empty()}}use std::io::BufReader;impl<'a>From<&'a str>for LineSource<BufReader<&'a[u8]>>{fn from(s:&'a str)->LineSource<BufReader<&'a[u8]>>{LineSource::new(BufReader::new(s.as_bytes()))}}}pub mod once{use super::Source;use crate::__cargo_equip::crates::proconio::source::tokens::Tokens;use std::io::BufRead;use std::marker::PhantomData;pub struct OnceSource<R:BufRead>{tokens:Tokens,_read:PhantomData<R>,}impl<R:BufRead>OnceSource<R>{pub fn new(mut source:R)->OnceSource<R>{let mut context=String::new();source.read_to_string(&mut context).expect("failed to read from source; maybe an IO error.");OnceSource{tokens:context.into(),_read:PhantomData,}}}impl<R:BufRead>Source<R>for OnceSource<R>{fn next_token(&mut self)->Option<&str>{self.tokens.next_token()}fn is_empty(&mut self)->bool{self.tokens.is_empty()}}use std::io::BufReader;impl<'a>From<&'a str>for OnceSource<BufReader<&'a[u8]>>{fn from(s:&'a str)->OnceSource<BufReader<&'a[u8]>>{OnceSource::new(BufReader::new(s.as_bytes()))}}}mod tokens{use std::{iter::Peekable,ptr::NonNull,str::SplitWhitespace};pub(super)struct Tokens{tokens:Peekable<SplitWhitespace<'static>>,_current_context:CurrentContext,}impl Tokens{pub(super)fn next_token(&mut self)->Option<&str>{self.tokens.next()}pub(super)fn is_empty(&mut self)->bool{self.tokens.peek().is_none()}}impl From<String>for Tokens{fn from(current_context:String)->Self{let current_context=CurrentContext::from(current_context);unsafe{let tokens=current_context.0.as_ref().split_whitespace().peekable();Self{tokens,_current_context:current_context,}}}}unsafe impl Send for Tokens{}unsafe impl Sync for Tokens{}struct CurrentContext(NonNull<str>);impl From<String>for CurrentContext{fn from(s:String)->Self{let s=s.into_boxed_str();Self(NonNull::new(Box::leak(s)).unwrap())}}impl Drop for CurrentContext{fn drop(&mut self){unsafe{Box::from_raw(self.0.as_mut())};}}}pub mod auto{#[cfg(debug_assertions)]pub use super::line::LineSource as AutoSource;#[cfg(not(debug_assertions))]pub use super::once::OnceSource as AutoSource;}pub trait Source<R:BufRead>{fn next_token(&mut self)->Option<&str>;#[allow(clippy::wrong_self_convention)]fn is_empty(&mut self)->bool;fn next_token_unwrap(&mut self)->&str{self.next_token().expect(concat!("failed to get the next token; ","maybe reader reached an end of input. ","ensure that arguments for `input!` macro is correctly ","specified to match the problem input."))}}impl<R:BufRead,S:Source<R>>Source<R>for&'_ mut S{fn next_token(&mut self)->Option<&str>{(*self).next_token()}fn is_empty(&mut self)->bool{(*self).is_empty()}}pub trait Readable{type Output;fn read<R:BufRead,S:Source<R>>(source:&mut S)->Self::Output;}impl<T:FromStr>Readable for T where T::Err:Debug,{type Output=T;fn read<R:BufRead,S:Source<R>>(source:&mut S)->T{let token=source.next_token_unwrap();match token.parse(){Ok(v)=>v,Err(e)=>panic!(concat!("failed to parse the input `{input}` ","to the value of type `{ty}`: {err:?}; ","ensure that the input format is collectly specified ","and that the input value must handle specified type.",),input=token,ty=type_name::<T>(),err=e,),}}}}use crate::__cargo_equip::crates::proconio::source::{auto::AutoSource,line::LineSource};use std::io::{BufReader,Stdin};use std::sync::OnceLock;use std::{io::{self,BufRead},sync::Mutex,};pub use crate::__cargo_equip::crates::proconio::source::Readable as __Readable;pub enum StdinSource<R:BufRead>{Normal(AutoSource<R>),Interactive(LineSource<R>),Unknown(LineSource<R>),}impl<R:BufRead>source::Source<R>for StdinSource<R>{fn next_token(&mut self)->Option<&str>{match self{StdinSource::Normal(source)=>source.next_token(),StdinSource::Interactive(source)=>source.next_token(),StdinSource::Unknown(source)=>source.next_token(),}}fn is_empty(&mut self)->bool{match self{StdinSource::Normal(source)=>source.is_empty(),StdinSource::Interactive(source)=>source.is_empty(),StdinSource::Unknown(source)=>source.is_empty(),}}}pub static STDIN_SOURCE:OnceLock<Mutex<StdinSource<BufReader<Stdin>>>> =OnceLock::new();#[macro_export]macro_rules!__cargo_equip_macro_def_proconio_input{(@from[$source:expr]@rest)=>{};(@from[$source:expr]@rest mut$($rest:tt)*)=>{$crate::__cargo_equip::crates::proconio::input!{@from[$source]@mut[mut]@rest$($rest)*}};(@from[$source:expr]@rest$($rest:tt)*)=>{$crate::__cargo_equip::crates::proconio::input!{@from[$source]@mut[]@rest$($rest)*}};(@from[$source:expr]@mut[$($mut:tt)?]@rest$var:tt:$($rest:tt)*)=>{$crate::__cargo_equip::crates::proconio::input!{@from[$source]@mut[$($mut)*]@var$var@kind[]@rest$($rest)*}};(@from[$source:expr]@mut[$($mut:tt)?]@var$var:tt@kind[$($kind:tt)*]@rest)=>{let$($mut)*$var=$crate::__cargo_equip::crates::proconio::read_value!(@source[$source]@kind[$($kind)*]);};(@from[$source:expr]@mut[$($mut:tt)?]@var$var:tt@kind[$($kind:tt)*]@rest,$($rest:tt)*)=>{$crate::__cargo_equip::crates::proconio::input!(@from[$source]@mut[$($mut)*]@var$var@kind[$($kind)*]@rest);$crate::__cargo_equip::crates::proconio::input!(@from[$source]@rest$($rest)*);};(@from[$source:expr]@mut[$($mut:tt)?]@var$var:tt@kind[$($kind:tt)*]@rest$tt:tt$($rest:tt)*)=>{$crate::__cargo_equip::crates::proconio::input!(@from[$source]@mut[$($mut)*]@var$var@kind[$($kind)*$tt]@rest$($rest)*);};(from$source:expr,$($rest:tt)*)=>{#[allow(unused_variables,unused_mut)]let mut s=$source;$crate::__cargo_equip::crates::proconio::input!{@from[&mut s]@rest$($rest)*}};($($rest:tt)*)=>{let mut locked_stdin=$crate::__cargo_equip::crates::proconio::STDIN_SOURCE.get_or_init(||{std::sync::Mutex::new($crate::__cargo_equip::crates::proconio::StdinSource::Normal($crate::__cargo_equip::crates::proconio::source::auto::AutoSource::new(std::io::BufReader::new(std::io::stdin())),))}).lock().expect(concat!("failed to lock the stdin; please re-run this program.  ","If this issue repeatedly occur, this is a bug in `proconio`.  ","Please report this issue from ","<https://github.com/statiolake/proconio-rs/issues>."));$crate::__cargo_equip::crates::proconio::input!{@from[&mut*locked_stdin]@rest$($rest)*}drop(locked_stdin);};}macro_rules!input{($($tt:tt)*)=>(crate::__cargo_equip_macro_def_proconio_input!{$($tt)*})}#[macro_export]macro_rules!__cargo_equip_macro_def_proconio_input_interactive{($($rest:tt)*)=>{let mut locked_stdin=$crate::__cargo_equip::crates::proconio::STDIN_SOURCE.get_or_init(||{std::sync::Mutex::new($crate::__cargo_equip::crates::proconio::StdinSource::Interactive($crate::__cargo_equip::crates::proconio::source::line::LineSource::new(std::io::BufReader::new(std::io::stdin())),))}).lock().expect(concat!("failed to lock the stdin; please re-run this program.  ","If this issue repeatedly occur, this is a bug in `proconio`.  ","Please report this issue from ","<https://github.com/statiolake/proconio-rs/issues>."));$crate::__cargo_equip::crates::proconio::input!{from&mut*locked_stdin,$($rest)*}drop(locked_stdin);};}macro_rules!input_interactive{($($tt:tt)*)=>(crate::__cargo_equip_macro_def_proconio_input_interactive!{$($tt)*})}#[macro_export]macro_rules!__cargo_equip_macro_def_proconio_read_value{(@source[$source:expr]@kind[[$($kind:tt)*]])=>{$crate::__cargo_equip::crates::proconio::read_value!(@array@source[$source]@kind[]@rest$($kind)*)};(@array@source[$source:expr]@kind[$($kind:tt)*]@rest)=>{{let len=<usize as$crate::__cargo_equip::crates::proconio::__Readable>::read($source);$crate::__cargo_equip::crates::proconio::read_value!(@source[$source]@kind[[$($kind)*;len]])}};(@array@source[$source:expr]@kind[$($kind:tt)*]@rest;$($rest:tt)*)=>{$crate::__cargo_equip::crates::proconio::read_value!(@array@source[$source]@kind[$($kind)*]@len[$($rest)*])};(@array@source[$source:expr]@kind[$($kind:tt)*]@rest$tt:tt$($rest:tt)*)=>{$crate::__cargo_equip::crates::proconio::read_value!(@array@source[$source]@kind[$($kind)*$tt]@rest$($rest)*)};(@array@source[$source:expr]@kind[$($kind:tt)*]@len[$($len:tt)*])=>{{let len=$($len)*;(0..len).map(|_|$crate::__cargo_equip::crates::proconio::read_value!(@source[$source]@kind[$($kind)*])).collect::<Vec<_>>()}};(@source[$source:expr]@kind[($($kinds:tt)*)])=>{$crate::__cargo_equip::crates::proconio::read_value!(@tuple@source[$source]@kinds[]@current[]@rest$($kinds)*)};(@tuple@source[$source:expr]@kinds[$([$($kind:tt)*])*]@current[]@rest)=>{($($crate::__cargo_equip::crates::proconio::read_value!(@source[$source]@kind[$($kind)*]),)*)};(@tuple@source[$source:expr]@kinds[$($kinds:tt)*]@current[$($curr:tt)*]@rest)=>{$crate::__cargo_equip::crates::proconio::read_value!(@tuple@source[$source]@kinds[$($kinds)*[$($curr)*]]@current[]@rest)};(@tuple@source[$source:expr]@kinds[$($kinds:tt)*]@current[$($curr:tt)*]@rest,$($rest:tt)*)=>{$crate::__cargo_equip::crates::proconio::read_value!(@tuple@source[$source]@kinds[$($kinds)*[$($curr)*]]@current[]@rest$($rest)*)};(@tuple@source[$source:expr]@kinds[$($kinds:tt)*]@current[$($curr:tt)*]@rest$tt:tt$($rest:tt)*)=>{$crate::__cargo_equip::crates::proconio::read_value!(@tuple@source[$source]@kinds[$($kinds)*]@current[$($curr)*$tt]@rest$($rest)*)};(@source[$source:expr]@kind[])=>{compile_error!(concat!("Reached unreachable statement while parsing macro input.  ","This is a bug in `proconio`.  ","Please report this issue from ","<https://github.com/statiolake/proconio-rs/issues>."));};(@source[$source:expr]@kind[$kind:ty])=>{<$kind as$crate::__cargo_equip::crates::proconio::__Readable>::read($source)}}macro_rules!read_value{($($tt:tt)*)=>(crate::__cargo_equip_macro_def_proconio_read_value!{$($tt)*})}pub fn is_stdin_empty()->bool{use source::Source;let mut lock=STDIN_SOURCE.get_or_init(||{Mutex::new(StdinSource::Unknown(LineSource::new(BufReader::new(io::stdin(),))))}).lock().expect(concat!("failed to lock the stdin; please re-run this program.  ","If this issue repeatedly occur, this is a bug in `proconio`.  ","Please report this issue from ","<https://github.com/statiolake/proconio-rs/issues>."));lock.is_empty()}}
    }

    pub(crate) mod macros {
        pub mod proconio {pub use crate::{__cargo_equip_macro_def_proconio_input as input,__cargo_equip_macro_def_proconio_input_interactive as input_interactive,__cargo_equip_macro_def_proconio_read_value as read_value};}
    }

    pub(crate) mod prelude {pub use crate::__cargo_equip::crates::*;}

    mod preludes {
        pub mod proconio {}
    }
}
