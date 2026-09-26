//https://judge.yosupo.jp/submission/22557
macro_rules! input {
  (source = $s:expr, $($r:tt)*) => {
    let mut iter = $s.split_whitespace();
    input_inner!{iter, $($r)*}
  };
  ($($r:tt)*) => {
    let s = {
      use std::io::Read;
      let mut s = String::new();
      std::io::stdin().read_to_string(&mut s).unwrap();
      s
    };
    let mut iter = s.split_whitespace();
    input_inner!{iter, $($r)*}
  };
}

macro_rules! input_inner {
  ($iter:expr) => {};
  ($iter:expr, ) => {};

  ($iter:expr, $var:ident : $t:tt $($r:tt)*) => {
    let $var = read_value!($iter, $t);
    input_inner!{$iter $($r)*}
  };
}

macro_rules! read_value {
  ($iter:expr, ( $($t:tt),* )) => {
    ( $(read_value!($iter, $t)),* )
  };

  ($iter:expr, [ $t:tt ; $len:expr ]) => {
    (0..$len).map(|_| read_value!($iter, $t)).collect::<Vec<_>>()
  };

  ($iter:expr, chars) => {
    read_value!($iter, String).chars().collect::<Vec<char>>()
  };

  ($iter:expr, usize1) => {
    read_value!($iter, usize) - 1
  };

  ($iter:expr, $t:ty) => {
    $iter.next().unwrap().parse::<$t>().expect("Parse error")
  };
}

fn main() {
  input! {
    n: usize,
    k: usize,
    triplet: [(usize, usize, u32); k],
  }

  let modulus = P998_244_353;
  let matrix = SparseMatrix::new(n, n, triplet, modulus);
  if let Some(lu) = SparseLU::new(&matrix) {
    println!("{}", lu.det());
  } else {
    println!("0");
  }
}

// --- modint ---
//pub use num::{One, Zero};
//pub use num_traits::ops::wrapping::*;
use std::cmp::Ord;
use std::ops::*;

pub trait NumberTrait:
  Copy
  + Ord
  //+ Zero
  //+ One
  + Add<Output = Self>
  + Sub<Output = Self>
  + Mul<Output = Self>
  + Div<Output = Self>
  + Rem<Output = Self>
  + RemAssign
  //+ WrappingSub<Output = Self>
  //+ WrappingAdd<Output = Self>
  //+ WrappingMul<Output = Self>
  + std::fmt::Debug
{
  fn zero() -> Self;
  fn one() -> Self;
  fn wrapping_add(&self, other: &Self) -> Self;
  fn wrapping_sub(&self, other: &Self) -> Self;
  fn wrapping_mul(&self, other: &Self) -> Self;
}
impl NumberTrait for u32 {
  fn zero() -> Self {
    0
  }
  fn one() -> Self {
    1
  }
  fn wrapping_add(&self, other: &Self) -> Self {
    u32::wrapping_add(*self, *other)
  }
  fn wrapping_sub(&self, other: &Self) -> Self {
    u32::wrapping_sub(*self, *other)
  }
  fn wrapping_mul(&self, other: &Self) -> Self {
    u32::wrapping_mul(*self, *other)
  }
}

pub trait Modulus: Copy + std::fmt::Debug {
  type Type: NumberTrait;
  fn modulus(&self) -> Self::Type;

  fn add(&self, a: Self::Type, b: Self::Type) -> Self::Type {
    let c = a + b;
    if c >= self.modulus() {
      c - self.modulus()
    } else {
      c
    }
  }
  fn sub(&self, a: Self::Type, b: Self::Type) -> Self::Type {
    if a >= b {
      a - b
    } else {
      self.modulus() - (b - a)
    }
  }
  fn mul(&self, a: Self::Type, b: Self::Type) -> Self::Type {
    (a * b) % self.modulus()
  }
  fn submul(&self, a: Self::Type, b: Self::Type, c: Self::Type) -> Self::Type {
    self.sub(a, self.mul(b, c))
  }
  fn addmul(&self, a: Self::Type, b: Self::Type, c: Self::Type) -> Self::Type {
    self.add(a, self.mul(b, c))
  }
  fn inv(&self, a: Self::Type) -> Self::Type {
    self.checked_inv(a).unwrap()
  }
  fn checked_inv(&self, a: Self::Type) -> Option<Self::Type> {
    let mut x = (self.modulus(), Self::Type::zero());
    let mut y = (a, Self::Type::one());
    while y.0 != Self::Type::zero() {
      x.1 = x.1.wrapping_sub(&(x.0 / y.0).wrapping_mul(&y.1));
      x.0 %= y.0;
      std::mem::swap(&mut x, &mut y);
    }
    if x.0 != Self::Type::one() {
      None
    } else {
      let x = if x.1 >= self.modulus() {
        x.1.wrapping_add(&self.modulus())
      } else {
        x.1
      };
      Some(x)
    }
  }
  fn checked_div(&self, a: Self::Type, b: Self::Type) -> Option<Self::Type> {
    let mut x = (self.modulus(), Self::Type::zero());
    let mut y = (b, Self::Type::one());
    while y.0 != Self::Type::zero() {
      x.1 = x.1.wrapping_sub(&(x.0 / y.0).wrapping_mul(&y.1));
      x.0 %= y.0;
      std::mem::swap(&mut x, &mut y);
    }
    if a % x.0 != Self::Type::zero() {
      None
    } else {
      let a = a / x.0;
      let x = if x.1 >= self.modulus() {
        x.1.wrapping_add(&self.modulus())
      } else {
        x.1
      };
      Some(self.mul(a, x))
    }
  }

  fn wrap<'a>(&'a self, a: Self::Type) -> ModIntT<'a, Self> {
    ModIntT {
      value: a,
      modulus: self,
    }
  }
}

#[derive(Copy, Clone, Eq, PartialEq, Debug)]
pub struct ModIntT<'a, M: Modulus> {
  value: M::Type,
  modulus: &'a M,
}

impl<'a, M: Modulus> AddAssign for ModIntT<'a, M> {
  fn add_assign(&mut self, other: Self) {
    self.value = self.modulus.add(self.value, other.value);
  }
}
impl<'a, M: Modulus> SubAssign for ModIntT<'a, M> {
  fn sub_assign(&mut self, other: Self) {
    self.value = self.modulus.sub(self.value, other.value);
  }
}
impl<'a, M: Modulus> MulAssign for ModIntT<'a, M> {
  fn mul_assign(&mut self, other: Self) {
    self.value = self.modulus.mul(self.value, other.value);
  }
}
impl<'a, M: Modulus> Add for ModIntT<'a, M> {
  type Output = ModIntT<'a, M>;
  fn add(mut self, other: Self) -> Self::Output {
    self += other;
    self
  }
}
impl<'a, M: Modulus> Sub for ModIntT<'a, M> {
  type Output = ModIntT<'a, M>;
  fn sub(mut self, other: Self) -> Self::Output {
    self -= other;
    self
  }
}
impl<'a, M: Modulus> Mul for ModIntT<'a, M> {
  type Output = ModIntT<'a, M>;
  fn mul(mut self, other: Self) -> Self::Output {
    self *= other;
    self
  }
}

#[derive(Copy, Clone, Eq, PartialEq, Debug)]
pub struct DynamicModulus32 {
  m: u32,
  //  im: u128,
}
impl DynamicModulus32 {
  pub fn new(m: u32) -> Self {
    //    let im = (1_u128 << 64) / m as u128;
    //    Self { m, im }
    Self { m }
  }
}
impl Modulus for DynamicModulus32 {
  type Type = u32;
  fn modulus(&self) -> u32 {
    self.m
  }
  /*
  // Barrett reduction.
  fn mul(&self, a: u32, b: u32) -> u32 {
    let x = a as u64 * b as u64;
    let q = ((x as u128 * self.im) >> 64) as u64;
    let x = (x - q * self.m as u64) as u32;
    if x < self.m {
      x
    } else {
      x - self.m
    }
  }
  */
}

#[derive(Clone, Copy, Eq, PartialEq, Debug)]
pub struct P998_244_353;
impl Modulus for P998_244_353 {
  type Type = u32;
  fn modulus(&self) -> u32 {
    998_244_353
  }
  fn mul(&self, a: u32, b: u32) -> u32 {
    ((a as u64 * b as u64) % self.modulus() as u64) as u32
  }
  fn submul(&self, a: u32, b: u32, c: u32) -> u32 {
    (a as i64 - b as i64 * c as i64).rem_euclid(self.modulus() as i64) as u32
  }
  fn addmul(&self, a: u32, b: u32, c: u32) -> u32 {
    (a as u64 + b as u64 * c as u64).rem_euclid(self.modulus() as u64) as u32
  }
}

// Compressed Sparse Column (CSC) Format
#[derive(Debug)]
pub struct SparseMatrix<M: Modulus> {
  rows: usize,
  cols: usize,
  colptr: Vec<usize>,
  rowval: Vec<(usize, M::Type)>,
  modulus: M,
}
impl<M: Modulus> SparseMatrix<M> {
  pub fn new(
    rows: usize,
    cols: usize,
    mut triplet: Vec<(usize, usize, M::Type)>,
    modulus: M,
  ) -> Self {
    triplet.sort_by_key(|&t| (t.1, t.0));
    let mut colptr = vec![0];
    let mut rowval = vec![];
    let mut k = 0;
    for j in 0..cols {
      while let Some(&(i, j_, x)) = triplet.get(k) {
        if j != j_ {
          break;
        }
        if x != M::Type::zero() {
          rowval.push((i, x));
        }
        k += 1;
      }
      colptr.push(k);
    }
    Self {
      rows,
      cols,
      rowval,
      colptr,
      modulus,
    }
  }
  pub fn rows(&self) -> usize {
    self.rows
  }
  pub fn cols(&self) -> usize {
    self.cols
  }
  pub fn print(&self) {
    let mut a = vec![vec![M::Type::zero(); self.cols()]; self.rows()];
    for col in 0..self.colptr.len() - 1 {
      for &(row, val) in &self.rowval[self.colptr[col]..self.colptr[col + 1]] {
        a[row][col] = val;
      }
    }
    for row in 0..self.rows() {
      eprintln!("{:?}", a[row]);
    }
  }
}
impl<M: Modulus> Mul for &SparseMatrix<M> {
  type Output = SparseMatrix<M>;
  fn mul(self, other: Self) -> Self::Output {
    //let mut result = Matrix::zeros(self.rows(), other.cols(), other.modulus);
    let mut colptr = vec![0];
    let mut rowval = vec![];
    for j in 0..other.cols() {
      let mut cj = vec![];
      for &(k, bkj) in &other.rowval[other.colptr[j]..other.colptr[j + 1]] {
        for &(i, aik) in &self.rowval[self.colptr[k]..self.colptr[k + 1]] {
          cj.push((i, self.modulus.mul(aik, bkj)));
        }
      }
      let mut last = None;
      cj.sort_by_key(|&(i, _)| i);
      for (i, cij) in cj.into_iter() {
        if Some(i) != last {
          rowval.push((i, cij));
        } else {
          let val = &mut rowval.last_mut().unwrap().1;
          *val = self.modulus.add(*val, cij);
        }
        last = Some(i);
      }
      colptr.push(rowval.len());
    }
    SparseMatrix {
      rows: self.rows(),
      cols: other.cols(),
      colptr,
      rowval,
      modulus: self.modulus,
    }
  }
}

#[derive(Debug)]
pub struct SparseLU<M: Modulus> {
  pub lower: SparseMatrix<M>,
  pub upper: SparseMatrix<M>,
  pub row_perm: Vec<usize>,
  pub col_perm: Vec<usize>,
}
impl<M: Modulus> SparseLU<M> {
  fn degree_ordering(matrix: &SparseMatrix<M>) -> Vec<usize> {
    let mut order = (0..matrix.cols()).collect::<Vec<_>>();
    order.sort_by_key(|&i| matrix.colptr[i + 1] - matrix.colptr[i]);
    order
  }
  fn perm_sign(pi: &[usize]) -> bool {
    let mut even = true;
    let mut seen = vec![false; pi.len()];
    for i in 0..pi.len() {
      if seen[i] {
        continue;
      }
      let mut s = i;
      let mut len = 0;
      while !seen[s] {
        seen[s] = true;
        s = pi[s];
        len += 1;
      }
      if len % 2 == 0 {
        even = !even;
      }
    }
    even
  }

  pub fn new(matrix: &SparseMatrix<M>) -> Option<Self> {
    let modulus = matrix.modulus;
    let mut lower = SparseMatrix::<M> {
      rows: matrix.rows(),
      cols: matrix.cols(),
      colptr: vec![0],
      rowval: vec![],
      modulus,
    };
    let mut upper = SparseMatrix::<M> {
      rows: matrix.rows(),
      cols: matrix.cols(),
      colptr: vec![0],
      rowval: vec![],
      modulus,
    };
    let col_perm = Self::degree_ordering(&matrix);
    let mut row_degree = vec![0; matrix.rows()];
    for &(i, _) in &matrix.rowval {
      row_degree[i] += 1;
    }
    let mut row_perm = vec![None; matrix.rows()];
    let mut x = vec![None; matrix.rows()];

    for (k, &col) in col_perm.iter().enumerate() {
      let mut support = vec![];
      let b = &matrix.rowval[matrix.colptr[col]..matrix.colptr[col + 1]];
      Self::cs_spsolve(&lower, b, &mut support, &mut x, &row_perm, true);

      // find pivot
      let mut pivot = None;
      for &i in &support {
        let xi = x[i].unwrap();
        if let Some(k) = row_perm[i] {
          upper.rowval.push((k, xi));
        } else if xi != M::Type::zero() {
          if pivot.map_or(true, |j| row_degree[i] < row_degree[j]) {
            pivot = Some(i);
          }
        }
      }
      if let Some(ipiv) = pivot {
        let xipiv = x[ipiv].unwrap();
        // divide by pivot
        upper.rowval.push((k, xipiv));
        row_perm[ipiv] = Some(k);
        lower.rowval.push((ipiv, M::Type::one()));
        let inv = modulus.inv(xipiv);
        for &i in &support {
          if row_perm[i].is_none() {
            let xi = x[i].unwrap();
            lower.rowval.push((i, modulus.mul(xi, inv)));
          }
          x[i] = None;
        }
        upper.colptr.push(upper.rowval.len());
        lower.colptr.push(lower.rowval.len());
      } else {
        return None;
      }
    }
    // finalize
    let row_perm = row_perm.into_iter().map(|x| x.unwrap()).collect::<Vec<_>>();
    for (i, _) in lower.rowval.iter_mut() {
      *i = row_perm[*i];
    }
    Some(Self {
      lower,
      upper,
      row_perm,
      col_perm,
    })
  }

  // support: k-th column „Å´„Å§„ÅÑ„Å¶Ëß£„ÅÑ„Åü„Å®„Åç„ÅÆ support.
  // ÂÖÉ„ÅÆ index „ÅßÂÖ•„Çã
  //
  // row_perm „ÅØ k „Åß„Éû„ÉÉ„ÉÅ„Åô„Çã
  // row_perm „ÅÆÂÆöÁæ©Ôºé
  // row_perm[row] = Some(k) „Åß row „Åå k Áï™ÁõÆ„ÅÆ pivot „Å´ÈÅ∏„Å∞„Çå„Åü
  // row_perm[row] = None    „Åß row „ÅØ„Åæ„Å† pivot „Å´ÈÅ∏„Å∞„Çå„Å¶„Å™„ÅÑ
  fn dfs(
    i: usize,
    matrix: &SparseMatrix<M>,
    support: &mut Vec<usize>,
    x: &mut [Option<M::Type>],
    row_perm: &[Option<usize>],
  ) {
    if x[i].is_some() {
      return;
    }
    x[i] = Some(M::Type::zero());
    if let Some(k) = row_perm[i] {
      for &(j, _) in &matrix.rowval[matrix.colptr[k]..matrix.colptr[k + 1]] {
        Self::dfs(j, matrix, support, x, row_perm);
      }
    }
    support.push(i);
  }

  pub fn cs_spsolve(
    matrix: &SparseMatrix<M>,
    b: &[(usize, M::Type)],
    support: &mut Vec<usize>,
    x: &mut [Option<M::Type>],
    row_perm: &[Option<usize>],
    low: bool,
  ) {
    for &(i, _) in b.iter() {
      Self::dfs(i, matrix, support, x, row_perm);
    }
    support.reverse();
    for &(i, bi) in b {
      x[i] = Some(bi);
    }
    for &j in support.iter() {
      if let Some(k) = row_perm[j] {
        let (diag, begin, end) = if low {
          (
            matrix.rowval[matrix.colptr[k]].1,
            matrix.colptr[k] + 1,
            matrix.colptr[k + 1],
          )
        } else {
          (
            matrix.rowval[matrix.colptr[k + 1] - 1].1,
            matrix.colptr[k],
            matrix.colptr[k + 1] - 1,
          )
        };
        let diag = matrix.modulus.checked_div(x[j].unwrap(), diag).unwrap();
        x[j] = Some(diag);
        for &(i, aij) in &matrix.rowval[begin..end] {
          assert!(x[i].is_some());
          x[i] = x[i].map(|xi| matrix.modulus.submul(xi, aij, diag));
        }
      }
    }
  }
  pub fn det(&self) -> M::Type {
    let mut det = M::Type::one();
    for i in 0..self.upper.cols() {
      det = self
        .upper
        .modulus
        .mul(det, self.upper.rowval[self.upper.colptr[i + 1] - 1].1);
    }
    if Self::perm_sign(&self.row_perm) ^ Self::perm_sign(&self.col_perm) {
      self.upper.modulus.sub(M::Type::zero(), det)
    } else {
      det
    }
  }
}
