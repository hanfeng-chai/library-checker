pub use npk_cp_lib::prelude::*;

use std::io::{stdin, stdout, BufWriter, Write};

use output::IntBuffer;
use reader::FastBufReader;

fn main() {
    let mut input = FastBufReader::<{ 1 << 18 }, _>::new(stdin().lock());
    let mut output = BufWriter::with_capacity(1 << 18, stdout().lock());
    let mut buf = IntBuffer::new();

    let n: usize = input.parse_next_token().unwrap();

    let points = {
        let mut points = Vec::with_capacity(n);
        points.extend((0..n).map(|_| {
            Point2D::new(
                input.parse_next_token().unwrap(),
                input.parse_next_token().unwrap(),
            )
        }));
        points.sort_unstable_by(|a, b| a.cmp_by_atan2(b));

        points
    };

    for Point2D { x, y } in points {
        output.write(buf.format(x).as_bytes()).unwrap();
        output.write(b" ").unwrap();
        output.write(buf.format(y).as_bytes()).unwrap();
        output.write(b"\n").unwrap();
    }
}

struct Point2D {
    x: i32,
    y: i32,
}

impl Point2D {
    fn new(x: i32, y: i32) -> Self {
        Self { x, y }
    }

    fn cmp_by_atan2(&self, other: &Self) -> std::cmp::Ordering {
        (self.y.cmp(&0).then(0.cmp(&self.x)))
            .cmp(&other.y.cmp(&0).then(0.cmp(&other.x)))
            .then_with(|| (other.x as i64 * self.y as i64).cmp(&(other.y as i64 * self.x as i64)))
    }
}

// The following code was expanded by `cargo-equip`.

///  # Bundled libraries
/// 
#[cfg_attr(any(), rustfmt::skip)]
#[allow(unused)]
mod npk_cp_lib {
    pub(crate) mod crates {
        pub mod from_bytes {#[cold]const fn cold_path(){}#[inline(always)]const fn parse_16_digits(bytes:[u8;16])->Option<u64>{let(mut hi,mut lo)={let bytes=bytes.as_chunks::<8>().0;(u64::from_le_bytes(bytes[0])^0x3030_3030_3030_3030,u64::from_le_bytes(bytes[1])^0x3030_3030_3030_3030,)};if(hi|lo|(hi+0x0606_0606_0606_0606)|(lo+0x0606_0606_0606_0606))&0xf0f0_f0f0_f0f0_f0f0==0{hi=(hi.wrapping_mul((10<<8)+1)>>8)&0x00ff_00ff_00ff_00ff;lo=(lo.wrapping_mul((10<<8)+1)>>8)&0x00ff_00ff_00ff_00ff;hi=(hi.wrapping_mul((100<<16)+1)>>16)&0x0000_ffff_0000_ffff;lo=(lo.wrapping_mul((100<<16)+1)>>16)&0x0000_ffff_0000_ffff;hi=hi.wrapping_mul((10000<<32)+1)>>32;lo=lo.wrapping_mul((10000<<32)+1)>>32;Some(hi*1_0000_0000+lo)}else{cold_path();None}}#[inline(always)]const fn parse_8_digits(bytes:[u8;8])->Option<u64>{let mut n=u64::from_le_bytes(bytes)^0x3030_3030_3030_3030;if(n|(n+0x0606_0606_0606_0606))&0xf0f0_f0f0_f0f0_f0f0==0{n=(n.wrapping_mul((10<<8)+1)>>8)&0x00ff_00ff_00ff_00ff;n=(n.wrapping_mul((100<<16)+1)>>16)&0x0000_ffff_0000_ffff;n=n.wrapping_mul((10000<<32)+1)>>32;Some(n)}else{cold_path();None}}#[inline(always)]const fn parse_4_digits(bytes:[u8;4])->Option<u32>{let mut n=u32::from_le_bytes(bytes)^0x3030_3030;if(n|n+0x0606_0606)&0xf0f0_f0f0==0{n=(n.wrapping_mul((10<<8)+1)>>8)&0x00ff_00ff;n=n.wrapping_mul((100<<16)+1)>>16;Some(n)}else{cold_path();None}}pub trait FromBytes:Sized{type Err;fn from_bytes(bytes:&[u8])->Result<Self,Self::Err>;}impl FromBytes for u64{type Err=();fn from_bytes(bytes:&[u8])->Result<Self,Self::Err>{let digits=match bytes{[b'+',rest@..]=>rest,_=>bytes,};if digits.is_empty(){cold_path();return Err(());}let(pre,suf)=digits.as_rchunks::<8>();let mut n={let mut bytes=[b'0';8];match pre.len(){1=>bytes[7..].copy_from_slice(&pre),2=>bytes[6..].copy_from_slice(&pre),3=>bytes[5..].copy_from_slice(&pre),4=>bytes[4..].copy_from_slice(&pre),5=>bytes[3..].copy_from_slice(&pre),6=>bytes[2..].copy_from_slice(&pre),7=>bytes[1..].copy_from_slice(&pre),_=>{}};parse_8_digits(bytes).ok_or(())?};let mut of=false;for chunk in suf{let x=n.overflowing_mul(1_0000_0000);n=x.0;of|=x.1;let x=n.overflowing_add(parse_8_digits(*chunk).ok_or(())?);n=x.0;of|=x.1;}if of{cold_path();Err(())}else{Ok(n)}}}impl FromBytes for u128{type Err=();fn from_bytes(bytes:&[u8])->Result<Self,Self::Err>{let digits=match bytes{[b'+',rest@..]=>rest,_=>bytes,};if digits.is_empty(){cold_path();return Err(());}let(pre,suf)=digits.as_rchunks::<16>();let mut n={let mut bytes=[b'0';16];match pre.len(){1=>bytes[15..].copy_from_slice(&pre),2=>bytes[14..].copy_from_slice(&pre),3=>bytes[13..].copy_from_slice(&pre),4=>bytes[12..].copy_from_slice(&pre),5=>bytes[11..].copy_from_slice(&pre),6=>bytes[10..].copy_from_slice(&pre),7=>bytes[9..].copy_from_slice(&pre),8=>bytes[8..].copy_from_slice(&pre),9=>bytes[7..].copy_from_slice(&pre),10=>bytes[6..].copy_from_slice(&pre),11=>bytes[5..].copy_from_slice(&pre),12=>bytes[4..].copy_from_slice(&pre),13=>bytes[3..].copy_from_slice(&pre),14=>bytes[2..].copy_from_slice(&pre),15=>bytes[1..].copy_from_slice(&pre),_=>{}};match pre.len(){1|2|3|4=>parse_4_digits(bytes.as_chunks::<4>().0[3]).ok_or(())?as u128,5|6|7|8=>parse_8_digits(bytes.as_chunks::<8>().0[1]).ok_or(())?as u128,9|10|11|12|13|14|15=>parse_16_digits(bytes).ok_or(())?as u128,_=>0,}};let mut of=false;for chunk in suf{let x=n.overflowing_mul(1_0000_0000_0000_0000);n=x.0;of|=x.1;let x=n.overflowing_add(parse_16_digits(*chunk).ok_or(())?as u128);n=x.0;of|=x.1;}if of{cold_path();Err(())}else{Ok(n)}}}impl FromBytes for i64{type Err=();fn from_bytes(bytes:&[u8])->Result<Self,Self::Err>{let(is_positive,digits)=match bytes{[b'-',rest@..]=>(false,rest),[b'+',rest@..]|rest=>(true,rest),};if digits.is_empty(){cold_path();return Err(());}let(pre,suf)=digits.as_rchunks::<8>();let mut n={let mut bytes=[b'0';8];match pre.len(){1=>bytes[7..].copy_from_slice(&pre),2=>bytes[6..].copy_from_slice(&pre),3=>bytes[5..].copy_from_slice(&pre),4=>bytes[4..].copy_from_slice(&pre),5=>bytes[3..].copy_from_slice(&pre),6=>bytes[2..].copy_from_slice(&pre),7=>bytes[1..].copy_from_slice(&pre),_=>{}};parse_8_digits(bytes).ok_or(())?as i64};let mut of=false;if is_positive{for chunk in suf{let x=n.overflowing_mul(1_0000_0000);n=x.0;of|=x.1;let x=n.overflowing_add(parse_8_digits(*chunk).ok_or(())?as i64);n=x.0;of|=x.1;}}else{n=-n;for chunk in suf{let x=n.overflowing_mul(1_0000_0000);n=x.0;of|=x.1;let x=n.overflowing_sub(parse_8_digits(*chunk).ok_or(())?as i64);n=x.0;of|=x.1;}}if of{cold_path();Err(())}else{Ok(n)}}}impl FromBytes for i128{type Err=();fn from_bytes(bytes:&[u8])->Result<Self,Self::Err>{let(is_positive,digits)=match bytes{[b'-',rest@..]=>(false,rest),[b'+',rest@..]|rest=>(true,rest),};if digits.is_empty(){cold_path();return Err(());}let(pre,suf)=digits.as_rchunks::<16>();let mut n={let mut bytes=[b'0';16];match pre.len(){1=>bytes[15..].copy_from_slice(&pre),2=>bytes[14..].copy_from_slice(&pre),3=>bytes[13..].copy_from_slice(&pre),4=>bytes[12..].copy_from_slice(&pre),5=>bytes[11..].copy_from_slice(&pre),6=>bytes[10..].copy_from_slice(&pre),7=>bytes[9..].copy_from_slice(&pre),8=>bytes[8..].copy_from_slice(&pre),9=>bytes[7..].copy_from_slice(&pre),10=>bytes[6..].copy_from_slice(&pre),11=>bytes[5..].copy_from_slice(&pre),12=>bytes[4..].copy_from_slice(&pre),13=>bytes[3..].copy_from_slice(&pre),14=>bytes[2..].copy_from_slice(&pre),15=>bytes[1..].copy_from_slice(&pre),_=>{}};match pre.len(){1|2|3|4=>parse_4_digits(bytes.as_chunks::<4>().0[3]).ok_or(())?as i128,5|6|7|8=>parse_8_digits(bytes.as_chunks::<8>().0[1]).ok_or(())?as i128,9|10|11|12|13|14|15=>parse_16_digits(bytes).ok_or(())?as i128,_=>0,}};let mut of=false;if is_positive{for chunk in suf{let x=n.overflowing_mul(1_0000_0000_0000_0000);n=x.0;of|=x.1;let x=n.overflowing_add(parse_16_digits(*chunk).ok_or(())?as i128);n=x.0;of|=x.1;}}else{n=-n;for chunk in suf{let x=n.overflowing_mul(1_0000_0000_0000_0000);n=x.0;of|=x.1;let x=n.overflowing_sub(parse_16_digits(*chunk).ok_or(())?as i128);n=x.0;of|=x.1;}}if of{cold_path();Err(())}else{Ok(n)}}}macro_rules!from_bytes_derive{($tar:ty as$src:ty)=>{impl FromBytes for$tar{type Err=();fn from_bytes(bytes:&[u8])->Result<Self,Self::Err>{let wide=<$src>::from_bytes(bytes)?;<$tar>::try_from(wide).map_err(|_|())}}};}from_bytes_derive!(u8 as u64);from_bytes_derive!(u16 as u64);from_bytes_derive!(u32 as u64);from_bytes_derive!(usize as u64);from_bytes_derive!(i8 as i64);from_bytes_derive!(i16 as i64);from_bytes_derive!(i32 as i64);from_bytes_derive!(isize as i64);}
        pub mod output {use std::io::Write;pub struct IntBuffer{buf:[u8;40],len:usize,}impl IntBuffer{pub const fn new()->Self{let buf=[0;_];let len=buf.len();Self{buf,len}}pub fn format<T>(&mut self,n:T)->&str where T:BufFormat<Buffer=Self>,{T::format(n,self)}pub fn format_iter<T>(&mut self,buf:&mut impl Write,mut iter:impl Iterator<Item=T>,sep:impl AsRef<[u8]>,)->std::io::Result<()>where T:BufFormat<Buffer=Self>,{if let Some(v)=iter.next(){buf.write(T::format(v,self).as_bytes())?;}while let Some(v)=iter.next(){buf.write(sep.as_ref())?;buf.write(T::format(v,self).as_bytes())?;}Ok(())}}static LUT4:[[u8;4];10000]=const{let mut lut=[[0;4];10000];let mut i=0;while i<10000{lut[i][3]=(i/0001%10)as u8+b'0';lut[i][2]=(i/0010%10)as u8+b'0';lut[i][1]=(i/0100%10)as u8+b'0';lut[i][0]=(i/1000%10)as u8+b'0';i+=1;}lut};mod sealed{pub trait Sealed{}macro_rules!seal{($($t:ty)*)=>{$(impl Sealed for$t{})*};}seal!(i8 u8 i16 u16 i32 u32 i64 u64 i128 u128 isize usize);impl<T>Sealed for&T where T:Sealed{}impl<T>Sealed for&mut T where T:Sealed{}}pub trait BufFormat:sealed::Sealed{type Buffer;fn format(self,buf:&mut Self::Buffer)->&str;}impl<T>BufFormat for&T where T:Copy+BufFormat,{type Buffer=T::Buffer;#[inline(always)]fn format(self,buf:&mut Self::Buffer)->&str{(*self).format(buf)}}impl<T>BufFormat for&mut T where T:Copy+BufFormat,{type Buffer=T::Buffer;#[inline(always)]fn format(self,buf:&mut Self::Buffer)->&str{(*self).format(buf)}}macro_rules!impl_format_uint{($($t:ty)*)=>{$(impl BufFormat for$t{type Buffer=IntBuffer;fn format(mut self,buf:&mut Self::Buffer)->&str{buf.len=buf.buf.len();while{let rem=self%10000;self/=10000;buf.len-=4;buf.buf[buf.len..buf.len+4].copy_from_slice(&LUT4[rem as usize]);self>0}{}let n=u32::from_le_bytes(buf.buf[buf.len..].as_chunks::<4>().0[0])^0x0030_3030;let offset=n.trailing_zeros()as usize/8;buf.len+=offset;unsafe{str::from_utf8_unchecked(&buf.buf[buf.len..])}}})*};}impl_format_uint!(u16 u32 u64 usize);impl BufFormat for u8{type Buffer=IntBuffer;fn format(self,buf:&mut Self::Buffer)->&str{buf.format(self as u16)}}impl BufFormat for u128{type Buffer=IntBuffer;fn format(mut self,buf:&mut Self::Buffer)->&str{buf.len=buf.buf.len();let mut x;while{x=(self%1_0000_0000_0000_0000)as u64;self/=1_0000_0000_0000_0000;self>0}{for _ in 0..4{let rem=x%10000;x/=10000;buf.len-=4;buf.buf[buf.len..buf.len+4].copy_from_slice(&LUT4[rem as usize]);}}while{let rem=x%10000;x/=10000;buf.len-=4;buf.buf[buf.len..buf.len+4].copy_from_slice(&LUT4[rem as usize]);x>0}{}let n=u32::from_le_bytes(buf.buf[buf.len..].as_chunks::<4>().0[0])^0x0030_3030;let offset=n.trailing_zeros()as usize/8;buf.len+=offset;unsafe{str::from_utf8_unchecked(&buf.buf[buf.len..])}}}macro_rules!impl_format_int{($($t:ty)*)=>{$(impl BufFormat for$t{type Buffer=IntBuffer;fn format(self,buf:&mut Self::Buffer)->&str{buf.format(self.unsigned_abs());if self.is_negative(){buf.len-=1;buf.buf[buf.len]=b'-';}unsafe{str::from_utf8_unchecked(&buf.buf[buf.len..])}}})*};}impl_format_int!(i8 i16 i32 i64 i128 isize);}
        pub mod reader {use crate::npk_cp_lib::preludes::reader::*;use core::slice;use std::{borrow::Cow,io::Read,mem::MaybeUninit};use from_bytes::FromBytes;pub struct FastBufReader<const N:usize,R>where R:Read,{reader:R,buf:Box<[MaybeUninit<u8>;N]>,filled:usize,cursor:usize,}impl<const N:usize,R:Read>FastBufReader<N,R>{pub fn new(reader:R)->Self{Self{reader,buf:Box::new([const{MaybeUninit::uninit()};_]),filled:0,cursor:0,}}fn fill_buf(&mut self)->std::io::Result<usize>{if self.cursor<self.filled{self.buf.copy_within(self.cursor..self.filled,0);}self.filled-=self.cursor;self.cursor=0;let uninit=&mut self.buf[self.filled..];let buf=unsafe{slice::from_raw_parts_mut(uninit.as_mut_ptr().cast::<u8>(),uninit.len())};let n=self.reader.read(buf)?;self.filled+=n;Ok(n)}fn skip_until_ascii_whitespace(&mut self){self.cursor+=self.buf[self.cursor..self.filled].iter().take_while(|b|unsafe{b.assume_init()}.is_ascii_whitespace()).count();}fn position_ascii_whitespace(&self)->usize{const D:usize=std::mem::size_of::<usize>();const ASCII_WHITESPACE:[usize;3]=[usize::from_ne_bytes([b'\n';D]),usize::from_ne_bytes([b'\r';D]),usize::from_ne_bytes([b' ';D]),];let init=&self.buf[self.cursor..self.filled];let buf=unsafe{slice::from_raw_parts(init.as_ptr().cast::<u8>(),init.len())};let(chunks,remainder)=buf.as_chunks::<D>();let mut n=0;for chunk in chunks{let packed=usize::from_le_bytes(*chunk);let one=const{usize::from_ne_bytes([0x01;D])};let acc=ASCII_WHITESPACE.iter().fold(0,|acc,tar|{let v=(packed^tar).wrapping_sub(one);acc|v})&!packed&const{usize::from_ne_bytes([0x80;D])};let pos=acc.trailing_zeros()as usize/8;n+=pos;if pos<8{return n;}}n+=remainder.iter().position(|b|b.is_ascii_whitespace()).unwrap_or(remainder.len());n}pub fn next_token(&mut self)->std::io::Result<Cow<'_,[u8]>>{self.skip_until_ascii_whitespace();if const{N>40}{if self.filled-self.cursor<40{self.fill_buf()?;self.skip_until_ascii_whitespace();}}let n=self.position_ascii_whitespace();self.cursor+=n;if self.cursor<self.filled{let token=&self.buf[self.cursor-n..self.cursor];let ret=unsafe{slice::from_raw_parts(token.as_ptr().cast::<u8>(),token.len())};return Ok(Cow::from(ret));}self.cursor-=n;let mut ret=Vec::with_capacity(1<<19);loop{{let token=&self.buf[self.cursor..self.filled];ret.extend_from_slice(unsafe{slice::from_raw_parts(token.as_ptr().cast::<u8>(),token.len())});}self.cursor=self.filled;self.fill_buf()?;debug_assert_eq!(self.cursor,0,"bug: `self.cursor` should be initialized to `0`");let n=self.position_ascii_whitespace();if n<self.filled{{let token=&self.buf[self.cursor..self.cursor+n];ret.extend_from_slice(unsafe{slice::from_raw_parts(token.as_ptr().cast::<u8>(),token.len())});}self.cursor=n;break;}}Ok(Cow::from(ret))}#[inline(always)]pub fn parse_next_token<T>(&mut self)->Option<T>where T:FromBytes,{let token=self.next_token().ok()?;T::from_bytes(&token).ok()}#[inline(always)]pub fn parse_next_token_vec<T>(&mut self,n:usize)->Option<Vec<T>>where T:FromBytes,{let mut ret=Vec::with_capacity(n);for _ in 0..n{let token=self.next_token().ok()?;ret.push(T::from_bytes(&token).ok()?)}Some(ret)}}}
    }

    pub(crate) mod macros {
        pub mod from_bytes {}
        pub mod output {}
        pub mod reader {}
    }

    pub(crate) mod prelude {pub use crate::npk_cp_lib::crates::*;}

    mod preludes {
        pub mod from_bytes {}
        pub mod output {}
        pub mod reader {pub(in crate::npk_cp_lib)use crate::npk_cp_lib::crates::from_bytes;}
    }
}