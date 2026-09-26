use __bundled::anmitsu::modint::fps998244353::FPS;
use __bundled::fastio::Fastio;
fn main() {
    let timer = std::time::Instant::now();
    let mut sc = Fastio::new();
    let n = sc.u64() as usize;
    let k = sc.u64() as usize;
    let m = sc.u64() as usize;
    let mut ia = vec![];
    for _ in 0..k {
        let (i, a) = (sc.u64() as usize, sc.u32());
        ia.push((i, a));
    }
    dbg!(timer.elapsed().as_micros());
    let timer = std::time::Instant::now();
    let mut f = FPS::new(vec![]);
    for (i, a) in ia {
        f.set(i, a);
    }
    let ans = f.pow_sparse(m, n - 1);
    dbg!(timer.elapsed().as_micros());
    let timer = std::time::Instant::now();
    for i in 0..n {
        sc.write(ans.get(i));
        sc.write('\n');
    }
    sc.flush();
    dbg!(timer.elapsed().as_micros());
}
pub mod __bundled {
    pub mod anmitsu {
        pub mod modint {
            pub mod convolution998244353 {
                pub const MOD: u32 = 998244353;
                pub const MAX_NTT_LEN: usize = 1 << 22;
                const NTT_RATE: [u32; 22] = [
                    0x3656d65b, 0x1e5ea9e6, 0x16038782, 0x13caac90, 0x3a9a4cfa, 0x761af21,
                    0xe372007, 0x3a2be7d4, 0x23fe18b2, 0x330f5b68, 0x7d37cf9, 0x3239edef,
                    0x2b8ea5c3, 0x382d2452, 0x300e9be2, 0x908b3f5, 0x1e726cd9, 0x1e02c2f0,
                    0x2c49629c, 0x2c2b7c93, 0x35a5081, 0x33b69d8b,
                ];
                const INTT_RATE: [u32; 22] = [
                    0x52929a6, 0x163456b8, 0x16400573, 0x267c5b5f, 0x6b059a5, 0x294c15f1,
                    0x94415d9, 0x2f83389c, 0x569c0ec, 0x3346ebba, 0x37473ab0, 0x1524e16f,
                    0x68442e3, 0x117ab9d0, 0x1fe52df0, 0x1263f553, 0x7392943, 0x24433aa8,
                    0x1a2993eb, 0x156d2fbf, 0x311e570f, 0x6294a13,
                ];
                const INVS: [u32; 23] = [
                    1, 499122177, 748683265, 873463809, 935854081, 967049217, 982646785, 990445569,
                    994344961, 996294657, 997269505, 997756929, 998000641, 998122497, 998183425,
                    998213889, 998229121, 998236737, 998240545, 998242449, 998243401, 998243877,
                    998244115,
                ];
                #[inline]
                fn add_mod(lhs: u32, rhs: u32) -> u32 {
                    let sum = lhs + rhs;
                    if sum >= MOD {
                        sum - MOD
                    } else {
                        sum
                    }
                }
                #[inline]
                fn sub_mod(lhs: u32, rhs: u32) -> u32 {
                    if lhs >= rhs {
                        lhs - rhs
                    } else {
                        lhs + MOD - rhs
                    }
                }
                #[inline]
                fn mul_mod(lhs: u32, rhs: u32) -> u32 {
                    ((lhs as u64 * rhs as u64) % MOD as u64) as u32
                }
                pub fn ntt(a: &mut [u32]) {
                    if a.is_empty() {
                        return;
                    }
                    let n = a.len();
                    assert!(
                        n.is_power_of_two(),
                        "NTT length {} is not a power of two",
                        n
                    );
                    assert!(
                        n <= MAX_NTT_LEN,
                        "NTT length {} exceeds supported maximum {}",
                        n,
                        MAX_NTT_LEN
                    );
                    let h = n.trailing_zeros();
                    for len in 0..h {
                        let p = 1 << (h - len - 1);
                        let mut rot = 1;
                        let step = 1 << (h - len);
                        for (s, chunk) in a.chunks_mut(step).enumerate() {
                            let ptr = chunk.as_mut_ptr();
                            for i in 0..p {
                                unsafe {
                                    let l = *ptr.add(i);
                                    let r = mul_mod(*ptr.add(i + p), rot);
                                    *ptr.add(i) = add_mod(l, r);
                                    *ptr.add(i + p) = sub_mod(l, r);
                                }
                            }
                            rot = mul_mod(rot, NTT_RATE[s.trailing_ones() as usize]);
                        }
                    }
                }
                pub fn intt(a: &mut [u32]) {
                    if a.is_empty() {
                        return;
                    }
                    let n = a.len();
                    assert!(
                        n.is_power_of_two(),
                        "NTT length {} is not a power of two",
                        n
                    );
                    assert!(
                        n <= MAX_NTT_LEN,
                        "NTT length {} exceeds supported maximum {}",
                        n,
                        MAX_NTT_LEN
                    );
                    let h = n.trailing_zeros();
                    for len in (1..=h).rev() {
                        let mut irot = 1;
                        let p = 1 << (h - len);
                        let step = 1 << (h - len + 1);
                        for (s, chunk) in a.chunks_mut(step).enumerate() {
                            let ptr = chunk.as_mut_ptr();
                            for i in 0..p {
                                unsafe {
                                    let l = *ptr.add(i);
                                    let r = *ptr.add(i + p);
                                    *ptr.add(i) = add_mod(l, r);
                                    *ptr.add(i + p) = mul_mod(sub_mod(l, r), irot);
                                }
                            }
                            irot = mul_mod(irot, INTT_RATE[s.trailing_ones() as usize]);
                        }
                    }
                }
                pub fn convolution(a: &[u32], b: &[u32]) -> Vec<u32> {
                    if a.is_empty() || b.is_empty() {
                        return Vec::new();
                    }
                    debug_assert!(a.iter().all(|&x| x < MOD));
                    debug_assert!(b.iter().all(|&x| x < MOD));
                    let s = a.len() + b.len() - 1;
                    if a.len().min(b.len()) <= 32 {
                        let mut res = vec![0; s];
                        for i in 0..a.len() {
                            let ai = a[i];
                            for j in 0..b.len() {
                                res[i + j] = add_mod(res[i + j], mul_mod(ai, b[j]));
                            }
                        }
                        return res;
                    }
                    let t = s.next_power_of_two();
                    assert!(
                        t <= MAX_NTT_LEN,
                        "Convolution length {} exceeds supported maximum {}",
                        t,
                        MAX_NTT_LEN
                    );
                    let mut fa = Vec::with_capacity(t);
                    fa.extend_from_slice(a);
                    fa.resize(t, 0);
                    let mut fb = Vec::with_capacity(t);
                    fb.extend_from_slice(b);
                    fb.resize(t, 0);
                    ntt(&mut fa);
                    ntt(&mut fb);
                    fa.iter_mut()
                        .zip(fb.iter())
                        .for_each(|(x, y)| *x = mul_mod(*x, *y));
                    intt(&mut fa);
                    let t_inv = INVS[t.trailing_zeros() as usize];
                    fa.iter_mut().take(s).for_each(|x| *x = mul_mod(*x, t_inv));
                    fa.truncate(s);
                    fa
                }
            }
            pub mod fps998244353 {
                use super::{convolution998244353, modulo998244353};
                use std::{
                    fmt,
                    ops::{Add, AddAssign, Mul, MulAssign, Neg, Sub, SubAssign},
                };
                const fn mul_mod(a: u32, b: u32) -> u32 {
                    ((a as u64 * b as u64) % modulo998244353::M as u64) as u32
                }
                pub const INV_INDICES_LEN: usize = 1_000_000;
                const fn build_inv_indices() -> [u32; INV_INDICES_LEN] {
                    let mut inv = [0u32; INV_INDICES_LEN];
                    if INV_INDICES_LEN > 1 {
                        inv[1] = 1;
                        let mut i = 2usize;
                        while i < INV_INDICES_LEN {
                            let iu = i as u32;
                            let q = modulo998244353::M / iu;
                            let r = (modulo998244353::M % iu) as usize;
                            let t = mul_mod(q, inv[r]);
                            let val = modulo998244353::M - t;
                            inv[i] = if val == modulo998244353::M { 0 } else { val };
                            i += 1;
                        }
                    }
                    inv
                }
                pub const INV_INDICES_TABLE: [u32; INV_INDICES_LEN] = build_inv_indices();
                #[derive(Clone, Debug, PartialEq, Eq)]
                pub struct FPS {
                    coeffs: Vec<u32>,
                }
                impl FPS {
                    pub fn new(mut coefficients: Vec<u32>) -> Self {
                        coefficients
                            .iter_mut()
                            .for_each(|c| assert!(*c < modulo998244353::M));
                        trim(&mut coefficients);
                        FPS {
                            coeffs: coefficients,
                        }
                    }
                    pub fn len(&self) -> usize {
                        self.coeffs.len()
                    }
                    pub fn degree(&self) -> Option<usize> {
                        if self.is_zero() {
                            None
                        } else {
                            Some(self.len() - 1)
                        }
                    }
                    pub fn is_zero(&self) -> bool {
                        self.coeffs.is_empty()
                    }
                    pub fn get(&self, index: usize) -> u32 {
                        *self.coeffs.get(index).unwrap_or(&0)
                    }
                    pub fn set(&mut self, index: usize, value: u32) {
                        assert!(value < modulo998244353::M);
                        if value == 0 && index >= self.len() {
                            return;
                        }
                        if self.len() <= index {
                            self.coeffs.resize(index + 1, 0);
                        }
                        self.coeffs[index] = value;
                        if value == 0 {
                            trim(&mut self.coeffs);
                        }
                    }
                    pub fn coefficients(&self) -> &[u32] {
                        &self.coeffs
                    }
                    pub fn truncate(&mut self, len: usize) {
                        self.coeffs.truncate(len);
                        trim(&mut self.coeffs);
                    }
                    pub fn mul_xk(&self, k: usize) -> Self {
                        if self.is_zero() {
                            return FPS { coeffs: Vec::new() };
                        }
                        let mut coeffs = vec![0; k + self.len()];
                        coeffs[k..].copy_from_slice(&self.coeffs);
                        FPS { coeffs }
                    }
                    pub fn div_xk(&self, k: usize) -> Self {
                        if k >= self.len() {
                            return FPS { coeffs: Vec::new() };
                        }
                        FPS {
                            coeffs: self.coeffs[k..].to_vec(),
                        }
                    }
                    pub fn inverse(&self, degree: usize) -> Option<Self> {
                        let len = degree + 1;
                        let constant = self.get(0);
                        if constant == 0 {
                            return None;
                        }
                        if len == 1 {
                            return Some(FPS {
                                coeffs: vec![modulo998244353::inv(constant)],
                            });
                        }
                        let mut inverse_coeffs = vec![modulo998244353::inv(constant)];
                        let mut current_len = 1;
                        let mut f_vals = Vec::with_capacity(2 * len);
                        let mut g_vals = Vec::with_capacity(2 * len);
                        let mut h_vals = Vec::with_capacity(2 * len);
                        while current_len < len {
                            let next_len = (current_len << 1).min(len);
                            let ntt_len = current_len << 1;
                            f_vals.clear();
                            f_vals.extend(self.coeffs.iter().cloned().take(ntt_len));
                            f_vals.resize(ntt_len, 0);
                            g_vals.clear();
                            g_vals.extend(inverse_coeffs.iter().cloned());
                            g_vals.resize(ntt_len, 0);
                            convolution998244353::ntt(&mut f_vals);
                            convolution998244353::ntt(&mut g_vals);
                            let inv_ntt_len = modulo998244353::inv(ntt_len as u32);
                            for (value, g_value) in f_vals.iter_mut().zip(g_vals.iter()) {
                                *value = modulo998244353::mul(*value, *g_value);
                            }
                            convolution998244353::intt(&mut f_vals);
                            f_vals.iter_mut().for_each(|value| {
                                *value = modulo998244353::mul(*value, inv_ntt_len);
                            });
                            h_vals.clear();
                            h_vals.resize(ntt_len, 0);
                            for i in 0..current_len {
                                if current_len + i < ntt_len {
                                    h_vals[i] = f_vals[current_len + i];
                                }
                            }
                            convolution998244353::ntt(&mut h_vals);
                            for (value, g_value) in h_vals.iter_mut().zip(g_vals.iter()) {
                                *value = modulo998244353::mul(*value, *g_value);
                            }
                            convolution998244353::intt(&mut h_vals);
                            h_vals.iter_mut().for_each(|value| {
                                *value = modulo998244353::mul(*value, inv_ntt_len);
                            });
                            let mut updated = Vec::with_capacity(next_len);
                            updated.extend(inverse_coeffs.iter().cloned().take(current_len));
                            updated.resize(next_len, 0);
                            for i in 0..(next_len - current_len) {
                                updated[current_len + i] =
                                    modulo998244353::neg(h_vals.get(i).copied().unwrap_or(0));
                            }
                            inverse_coeffs = updated;
                            current_len = next_len;
                        }
                        inverse_coeffs.truncate(len);
                        trim(&mut inverse_coeffs);
                        Some(FPS {
                            coeffs: inverse_coeffs,
                        })
                    }
                    pub fn derivative(&mut self) -> &mut Self {
                        let len = self.len();
                        if len <= 1 {
                            self.coeffs.clear();
                            return self;
                        }
                        for i in 0..(len - 1) {
                            self.coeffs[i] =
                                modulo998244353::mul((i + 1) as u32, self.coeffs[i + 1]);
                        }
                        self.coeffs.truncate(len - 1);
                        trim(&mut self.coeffs);
                        self
                    }
                    pub fn integral(&mut self) -> &mut Self {
                        assert!(
                            self.len() + 1 < modulo998244353::M as usize,
                            "Integral requires degree + 1 < modulus"
                        );
                        if self.is_zero() {
                            return self;
                        }
                        let len = self.len();
                        let mut fact = Vec::with_capacity(len + 1);
                        fact.push(1);
                        for i in 1..=len {
                            fact.push(modulo998244353::mul(fact[i - 1], i as u32));
                        }
                        let mut ifact = Vec::with_capacity(len + 1);
                        ifact.push(modulo998244353::inv(fact[len]));
                        for i in 0..len {
                            ifact.push(modulo998244353::mul(ifact[i], (len - i) as u32));
                        }
                        ifact.reverse();
                        self.coeffs.push(0);
                        for i in (0..len).rev() {
                            let scaled = modulo998244353::mul(self.coeffs[i], ifact[i + 1]);
                            let integrated = modulo998244353::mul(scaled, fact[i]);
                            self.coeffs[i + 1] = integrated;
                        }
                        self.coeffs[0] = 0;
                        trim(&mut self.coeffs);
                        self
                    }
                    pub fn log(&self, degree: usize) -> Option<Self> {
                        if self.get(0) != 1 {
                            return None;
                        }
                        let target_len = degree + 1;
                        if target_len == 1 {
                            return Some(FPS { coeffs: vec![0] });
                        }
                        let mut f = FPS {
                            coeffs: self.coeffs.iter().cloned().take(target_len).collect(),
                        };
                        trim(&mut f.coeffs);
                        let inv = f.inverse(degree.saturating_sub(1))?;
                        f.derivative();
                        let mut res = f * inv;
                        res.truncate(degree);
                        res.integral();
                        res.truncate(target_len);
                        Some(res)
                    }
                    pub fn exp(&self, degree: usize) -> Option<Self> {
                        if self.get(0) != 0 {
                            return None;
                        }
                        let target_len_full = degree + 1;
                        if target_len_full == 1 {
                            return Some(FPS { coeffs: vec![1] });
                        }
                        let mut res = FPS { coeffs: vec![1] };
                        let mut current_len = 1;
                        while current_len < target_len_full {
                            let target_len = (current_len << 1).min(target_len_full);
                            let mut truncated = FPS {
                                coeffs: self.coeffs.iter().cloned().take(target_len).collect(),
                            };
                            trim(&mut truncated.coeffs);
                            let mut delta = truncated - res.log(target_len - 1)?;
                            if delta.is_zero() {
                                delta = FPS { coeffs: vec![1] };
                            } else {
                                let first = modulo998244353::add(delta.coeffs[0], 1);
                                delta.coeffs[0] = first;
                            }
                            res *= delta;
                            res.truncate(target_len);
                            current_len = target_len;
                        }
                        res.truncate(target_len_full);
                        Some(res)
                    }
                    pub fn inverse_sparse(&self, degree: usize) -> Option<Self> {
                        let target_len = degree + 1;
                        let constant = self.get(0);
                        if constant == 0 {
                            return None;
                        }
                        let inv_const = modulo998244353::inv(constant);
                        let mut sparse_terms = Vec::new();
                        for (i, &c) in self.coeffs.iter().enumerate().skip(1) {
                            if i > degree {
                                break;
                            }
                            if c != 0 {
                                sparse_terms.push((i, c));
                            }
                        }
                        let mut res = vec![0; target_len];
                        res[0] = inv_const;
                        for n in 1..target_len {
                            let mut acc = 0_u32;
                            for &(i, c) in sparse_terms.iter().take_while(|&&(i, _)| i <= n) {
                                acc =
                                    modulo998244353::sub(acc, modulo998244353::mul(c, res[n - i]));
                            }
                            res[n] = modulo998244353::mul(acc, inv_const);
                        }
                        trim(&mut res);
                        Some(FPS { coeffs: res })
                    }
                    pub fn log_sparse(&self, degree: usize) -> Option<Self> {
                        if self.get(0) != 1 {
                            return None;
                        }
                        let target_len = degree + 1;
                        if target_len == 1 {
                            return Some(FPS { coeffs: vec![0] });
                        }
                        let mut sparse_terms = Vec::new();
                        for (i, &c) in self.coeffs.iter().enumerate().skip(1) {
                            if i > degree {
                                break;
                            }
                            if c != 0 {
                                sparse_terms.push((i, c));
                            }
                        }
                        let deriv_len = target_len - 1;
                        let mut quotient = vec![0_u32; deriv_len];
                        for n in 0..deriv_len {
                            let mut acc = 0_u32;
                            for &(i, c) in &sparse_terms {
                                if i > n {
                                    break;
                                }
                                acc = modulo998244353::add(
                                    acc,
                                    modulo998244353::mul(c, quotient[n - i]),
                                );
                            }
                            let deriv_n = modulo998244353::mul(n as u32 + 1, self.get(n + 1));
                            quotient[n] = modulo998244353::sub(deriv_n, acc);
                        }
                        let mut res = FPS { coeffs: quotient };
                        res.integral();
                        res.truncate(target_len);
                        Some(res)
                    }
                    pub fn exp_sparse(&self, degree: usize) -> Option<Self> {
                        if self.get(0) != 0 {
                            return None;
                        }
                        let target_len = degree + 1;
                        if target_len == 1 {
                            return Some(FPS { coeffs: vec![1] });
                        }
                        let max_deriv_degree = degree - 1;
                        let mut sparse_deriv_terms = Vec::new();
                        for (i, &c) in self.coeffs.iter().enumerate().skip(1) {
                            if i > degree {
                                break;
                            }
                            if c != 0 {
                                let deg = i - 1;
                                if deg > max_deriv_degree {
                                    break;
                                }
                                let coeff = modulo998244353::mul(i as u32, c);
                                sparse_deriv_terms.push((deg, coeff));
                            }
                        }
                        let mut inv_indices = vec![0_u32; target_len];
                        {
                            let mut fact = Vec::with_capacity(target_len);
                            let mut ifact = Vec::with_capacity(target_len);
                            fact.push(1);
                            for i in 1..target_len {
                                let next =
                                    modulo998244353::mul(*fact.last().unwrap_or(&1), i as u32);
                                fact.push(next);
                            }
                            ifact.push(modulo998244353::inv(*fact.last().unwrap_or(&1)));
                            for i in 0..degree {
                                let next =
                                    modulo998244353::mul(ifact[i], (target_len - 1 - i) as u32);
                                ifact.push(next);
                            }
                            ifact.reverse();
                            for i in 1..target_len {
                                inv_indices[i] = modulo998244353::mul(ifact[i], fact[i - 1]);
                            }
                        }
                        let mut coeffs = vec![0_u32; target_len];
                        coeffs[0] = 1;
                        for n in 0..degree {
                            let mut acc = 0_u32;
                            for &(deg, c) in &sparse_deriv_terms {
                                if deg > n {
                                    break;
                                }
                                acc = modulo998244353::add(
                                    acc,
                                    modulo998244353::mul(coeffs[n - deg], c),
                                );
                            }
                            coeffs[n + 1] = modulo998244353::mul(acc, inv_indices[n + 1]);
                        }
                        trim(&mut coeffs);
                        Some(FPS { coeffs })
                    }
                    pub fn pow_sparse(&self, exponent: usize, degree: usize) -> Self {
                        let target_len = degree + 1;
                        if exponent == 0 {
                            return FPS { coeffs: vec![1] };
                        }
                        if self.len() == 0 {
                            return FPS { coeffs: Vec::new() };
                        }
                        let mut sparse_terms = Vec::new();
                        for (i, &c) in self.coeffs.iter().enumerate() {
                            if c != 0 {
                                assert!(c < modulo998244353::M);
                                sparse_terms.push((i, c));
                            }
                        }
                        let min_non_zero_degree = sparse_terms[0].0;
                        let first_non_zero_coeff = sparse_terms[0].1;
                        let total_degree_shift = exponent.saturating_mul(min_non_zero_degree);
                        if total_degree_shift >= target_len {
                            return FPS::new(vec![]);
                        }
                        let normalized_len = target_len - total_degree_shift;
                        let scaled_leading_power =
                            modulo998244353::pow(first_non_zero_coeff, exponent);
                        let inv_first_coeff = modulo998244353::inv(first_non_zero_coeff);
                        let mut normalized_sparse_terms = Vec::new();
                        for (idx, coef) in sparse_terms.into_iter() {
                            if idx < min_non_zero_degree {
                                continue;
                            }
                            let shifted_degree = idx - min_non_zero_degree;
                            if shifted_degree >= normalized_len {
                                break;
                            }
                            let scaled = modulo998244353::mul(coef, inv_first_coeff);
                            normalized_sparse_terms.push((shifted_degree, scaled));
                        }
                        let mut base_series_terms = Vec::new();
                        for (degree, coeff) in normalized_sparse_terms.into_iter() {
                            if degree == 0 {
                                continue;
                            }
                            base_series_terms.push((degree, coeff));
                        }
                        let exponent_mod = modulo998244353::modulo(exponent as u64);
                        let offset = target_len - normalized_len;
                        let mut ans = vec![0_u32; target_len];
                        ans[offset] = 1;
                        for k in 0..(normalized_len - 1) {
                            let mut next = 0_u32;
                            for &(i, f_i) in &base_series_terms {
                                if i <= offset + k + 1 && offset + k + 1 <= target_len + i {
                                    let ci = modulo998244353::sub(
                                        modulo998244353::mul(exponent_mod, i as u32),
                                        k as u32 - i as u32 + 1,
                                    );
                                    let cont = modulo998244353::mul(f_i, ans[offset + k + 1 - i]);
                                    next =
                                        modulo998244353::add(next, modulo998244353::mul(ci, cont));
                                } else {
                                    break;
                                }
                            }
                            next = modulo998244353::mul(next, INV_INDICES_TABLE[k + 1]);
                            ans[offset + k + 1] = next;
                        }
                        for idx in offset..target_len {
                            ans[idx] = modulo998244353::mul(scaled_leading_power, ans[idx]);
                        }
                        trim(&mut ans);
                        FPS { coeffs: ans }
                    }
                }
                impl Add for FPS {
                    type Output = FPS;
                    fn add(mut self, rhs: Self) -> Self::Output {
                        if self.coeffs.len() < rhs.coeffs.len() {
                            self.coeffs.resize(rhs.coeffs.len(), 0);
                        }
                        for (i, coeff) in rhs.coeffs.into_iter().enumerate() {
                            self.coeffs[i] = modulo998244353::add(self.coeffs[i], coeff);
                        }
                        trim(&mut self.coeffs);
                        self
                    }
                }
                impl Sub for FPS {
                    type Output = FPS;
                    fn sub(mut self, rhs: Self) -> Self::Output {
                        if self.coeffs.len() < rhs.coeffs.len() {
                            self.coeffs.resize(rhs.coeffs.len(), 0);
                        }
                        for (i, coeff) in rhs.coeffs.into_iter().enumerate() {
                            self.coeffs[i] = modulo998244353::sub(self.coeffs[i], coeff);
                        }
                        trim(&mut self.coeffs);
                        self
                    }
                }
                impl Mul for FPS {
                    type Output = FPS;
                    fn mul(self, rhs: Self) -> Self::Output {
                        let mut coeffs = multiply(&self.coeffs, &rhs.coeffs);
                        trim(&mut coeffs);
                        FPS { coeffs }
                    }
                }
                impl Neg for FPS {
                    type Output = FPS;
                    fn neg(mut self) -> Self::Output {
                        self.coeffs
                            .iter_mut()
                            .for_each(|c| *c = modulo998244353::neg(*c));
                        self
                    }
                }
                impl AddAssign for FPS {
                    fn add_assign(&mut self, rhs: Self) {
                        if self.coeffs.len() < rhs.coeffs.len() {
                            self.coeffs.resize(rhs.coeffs.len(), 0);
                        }
                        for (i, coeff) in rhs.coeffs.into_iter().enumerate() {
                            self.coeffs[i] = modulo998244353::add(self.coeffs[i], coeff);
                        }
                        trim(&mut self.coeffs);
                    }
                }
                impl SubAssign for FPS {
                    fn sub_assign(&mut self, rhs: Self) {
                        if self.coeffs.len() < rhs.coeffs.len() {
                            self.coeffs.resize(rhs.coeffs.len(), 0);
                        }
                        for (i, coeff) in rhs.coeffs.into_iter().enumerate() {
                            self.coeffs[i] = modulo998244353::sub(self.coeffs[i], coeff);
                        }
                        trim(&mut self.coeffs);
                    }
                }
                impl MulAssign for FPS {
                    fn mul_assign(&mut self, rhs: Self) {
                        let mut coeffs = multiply(&self.coeffs, &rhs.coeffs);
                        trim(&mut coeffs);
                        self.coeffs = coeffs;
                    }
                }
                impl fmt::Display for FPS {
                    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
                        if self.is_zero() {
                            return write!(f, "0");
                        }
                        let terms = self
                            .coeffs
                            .iter()
                            .enumerate()
                            .map(|(i, c)| format!("{}x^{}", c, i))
                            .collect::<Vec<String>>();
                        write!(f, "{}", terms.join(" + "))
                    }
                }
                fn trim(coeffs: &mut Vec<u32>) {
                    while coeffs.last().map_or(false, |c| *c == 0) {
                        coeffs.pop();
                    }
                }
                fn multiply(lhs: &[u32], rhs: &[u32]) -> Vec<u32> {
                    if lhs.is_empty() || rhs.is_empty() {
                        return Vec::new();
                    }
                    convolution998244353::convolution(lhs, rhs)
                }
            }
            pub mod modulo998244353 {
                pub const M: u32 = 998244353;
                pub const fn modulo(a: u64) -> u32 {
                    (a % M as u64) as u32
                }
                pub const fn add(a: u32, b: u32) -> u32 {
                    debug_assert!(a < M);
                    debug_assert!(b < M);
                    let t = a + b;
                    if t < M {
                        t
                    } else {
                        t.wrapping_sub(M)
                    }
                }
                pub const fn sub(a: u32, b: u32) -> u32 {
                    debug_assert!(a < M);
                    debug_assert!(b < M);
                    let (t, f) = a.overflowing_sub(b);
                    if !f {
                        t
                    } else {
                        t.wrapping_add(M)
                    }
                }
                pub const fn mul(a: u32, b: u32) -> u32 {
                    debug_assert!(a < M);
                    debug_assert!(b < M);
                    modulo(a as u64 * b as u64)
                }
                pub const fn neg(a: u32) -> u32 {
                    debug_assert!(a < M);
                    if a == 0 {
                        0
                    } else {
                        M - a
                    }
                }
                pub const fn pow(a: u32, mut n: usize) -> u32 {
                    debug_assert!(a < M);
                    let mut res = 1;
                    let mut x = a;
                    while n > 0 {
                        if n % 2 == 1 {
                            res = mul(res, x);
                        }
                        x = mul(x, x);
                        n /= 2;
                    }
                    res
                }
                pub const fn inv(a: u32) -> u32 {
                    debug_assert!(a < M);
                    debug_assert!(a != 0);
                    pow(a, M as usize - 2)
                }
            }
        }
        pub fn add(left: u64, right: u64) -> u64 {
            left + right
        }
    }
    pub mod fastio {
        use std::{fmt, io, str};
        pub trait FastWrite {
            fn write_to(self, output: &mut Vec<u8>);
        }
        impl FastWrite for char {
            fn write_to(self, output: &mut Vec<u8>) {
                output.push(self as u8);
            }
        }
        impl FastWrite for &str {
            fn write_to(self, output: &mut Vec<u8>) {
                output.extend_from_slice(self.as_bytes());
            }
        }
        impl FastWrite for String {
            fn write_to(self, output: &mut Vec<u8>) {
                output.extend_from_slice(self.as_bytes());
            }
        }
        impl FastWrite for &[u8] {
            fn write_to(self, output: &mut Vec<u8>) {
                output.extend_from_slice(self);
            }
        }
        impl FastWrite for Vec<u8> {
            fn write_to(self, output: &mut Vec<u8>) {
                output.extend_from_slice(&self);
            }
        }
        static DIGITS_LUT: [u8; 40000] = {
            let mut lut = [0u8; 40000];
            let mut i = 0;
            while i < 10000 {
                let p = i * 4;
                lut[p] = b'0' + (i / 1000) as u8;
                lut[p + 1] = b'0' + (i / 100 % 10) as u8;
                lut[p + 2] = b'0' + (i / 10 % 10) as u8;
                lut[p + 3] = b'0' + (i % 10) as u8;
                i += 1;
            }
            lut
        };
        static REM_START_LUT: [u8; 10000] = {
            let mut lut = [0u8; 10000];
            let mut i = 0;
            while i < 10000 {
                lut[i] = if i >= 1000 {
                    0
                } else if i >= 100 {
                    1
                } else if i >= 10 {
                    2
                } else {
                    3
                };
                i += 1;
            }
            lut
        };
        impl FastWrite for u32 {
            fn write_to(mut self, output: &mut Vec<u8>) {
                let mut buf = [0u8; 10];
                let mut pos = 10;
                while self >= 10_000 {
                    let q = self / 10_000;
                    let r = (self - q * 10_000) as usize;
                    pos -= 4;
                    let idx = r * 4;
                    buf[pos..pos + 4].copy_from_slice(&DIGITS_LUT[idx..idx + 4]);
                    self = q;
                }
                let rem = self as usize;
                let start = REM_START_LUT[rem] as usize;
                let len = 4 - start;
                let idx = rem * 4 + start;
                pos -= len;
                buf[pos..pos + len].copy_from_slice(&DIGITS_LUT[idx..idx + len]);
                output.extend_from_slice(&buf[pos..]);
            }
        }
        impl FastWrite for u64 {
            fn write_to(mut self, output: &mut Vec<u8>) {
                if self == 0 {
                    output.push(b'0');
                    return;
                }
                let mut buf = [0_u8; 20];
                let mut index = buf.len();
                while self > 0 {
                    index -= 1;
                    buf[index] = (self % 10) as u8 + b'0';
                    self /= 10;
                }
                output.extend_from_slice(&buf[index..]);
            }
        }
        impl FastWrite for usize {
            fn write_to(self, output: &mut Vec<u8>) {
                Fastio::write_unsigned_naive(output, self as u64);
            }
        }
        impl FastWrite for i64 {
            fn write_to(self, output: &mut Vec<u8>) {
                Fastio::write_signed_naive(output, self);
            }
        }
        impl FastWrite for i32 {
            fn write_to(self, output: &mut Vec<u8>) {
                Fastio::write_signed_naive(output, self as i64);
            }
        }
        impl FastWrite for isize {
            fn write_to(self, output: &mut Vec<u8>) {
                Fastio::write_signed_naive(output, self as i64);
            }
        }
        pub struct Fastio {
            buffer: Vec<u8>,
            cursor: usize,
            output: Vec<u8>,
        }
        impl Fastio {
            pub fn new() -> Self {
                let mut buffer = Vec::with_capacity(100000);
                let output = Vec::with_capacity(100000);
                io::Read::read_to_end(&mut io::stdin().lock(), &mut buffer)
                    .expect("failed to read from source");
                Self {
                    buffer,
                    cursor: 0,
                    output,
                }
            }
            pub fn from_bytes(data: impl AsRef<[u8]>) -> Self {
                Self {
                    buffer: data.as_ref().to_vec(),
                    cursor: 0,
                    output: Vec::new(),
                }
            }
            pub fn char(&mut self) -> char {
                self.parse_token()
            }
            pub fn i32(&mut self) -> i32 {
                let token = self.read_token();
                let value = Self::parse_i64_bytes(token);
                value as i32
            }
            pub fn u32(&mut self) -> u32 {
                let token = self.read_token();
                let value = Self::parse_u64_bytes(token);
                value as u32
            }
            pub fn i64(&mut self) -> i64 {
                let token = self.read_token();
                Self::parse_i64_bytes(token)
            }
            pub fn u64(&mut self) -> u64 {
                let token = self.read_token();
                Self::parse_u64_bytes(token)
            }
            pub fn isize1(&mut self) -> isize {
                let token = self.read_token();
                let value64 = Self::parse_i64_bytes(token);
                let value = value64 as isize;
                value - 1
            }
            pub fn usize1(&mut self) -> usize {
                let token = self.read_token();
                let value64 = Self::parse_u64_bytes(token);
                value64 as usize - 1
            }
            pub fn chars(&mut self) -> Vec<char> {
                let token = self.read_token();
                let text = str::from_utf8(token).expect("input must be UTF-8");
                text.chars().collect()
            }
            pub fn write<T>(&mut self, value: T)
            where
                T: FastWrite,
            {
                value.write_to(&mut self.output);
            }
            pub fn flush(&mut self) {
                let mut stdout = io::stdout().lock();
                self.flush_to(&mut stdout);
            }
            pub fn flush_to(&mut self, mut writer: impl io::Write) {
                if self.output.is_empty() {
                    return;
                }
                io::Write::write_all(&mut writer, &self.output)
                    .and_then(|_| io::Write::flush(&mut writer))
                    .expect("failed to write buffered output");
                self.output.clear();
            }
            fn parse_token<T>(&mut self) -> T
            where
                T: str::FromStr,
                T::Err: fmt::Display,
            {
                let token = self.read_token();
                let text = str::from_utf8(token).expect("input must be UTF-8");
                match T::from_str(text) {
                    Ok(value) => value,
                    Err(error) => panic!("failed to parse token \"{}\": {}", text, error),
                }
            }
            fn read_token(&mut self) -> &[u8] {
                while self.cursor < self.buffer.len()
                    && self.buffer[self.cursor].is_ascii_whitespace()
                {
                    self.cursor += 1;
                }
                let start = self.cursor;
                while self.cursor < self.buffer.len()
                    && !self.buffer[self.cursor].is_ascii_whitespace()
                {
                    self.cursor += 1;
                }
                &self.buffer[start..self.cursor]
            }
            #[inline(always)]
            fn parse_u64_bytes(bytes: &[u8]) -> u64 {
                let mut index = 0;
                let digit_count = bytes.len();
                assert!(digit_count <= 20, "{}", digit_count);
                let mut value = 0_u64;
                let leading = digit_count % 8;
                let end_leading = index + leading;
                while index < end_leading {
                    value = value * 10 + (bytes[index] - 0x30) as u64;
                    index += 1;
                }
                while index + 8 <= bytes.len() {
                    let chunk_value = Self::parse_8digits_le(&bytes[index..index + 8]) as u64;
                    value = value * 100_000_000 + chunk_value;
                    index += 8;
                }
                value
            }
            fn parse_i64_bytes(bytes: &[u8]) -> i64 {
                let mut index = 0;
                let mut negative = false;
                if bytes[0] == b'-' {
                    index = 1;
                    negative = true;
                }
                let magnitude = Self::parse_u64_bytes(&bytes[index..]);
                if negative {
                    if magnitude == 0 {
                        return 0;
                    }
                    let limit = (i64::MAX as u64) + 1;
                    if magnitude == limit {
                        i64::MIN
                    } else {
                        -(magnitude as i64)
                    }
                } else {
                    magnitude as i64
                }
            }
            fn parse_8digits_le(bytes: &[u8]) -> u32 {
                let mut buf = [0_u8; 8];
                buf.copy_from_slice(bytes);
                let x = u64::from_le_bytes(buf);
                let v1 = (x & 0x0f0f0f0f0f0f0f0f).wrapping_mul(0xA01) >> 8;
                let v2 = (v1 & 0x00ff00ff00ff00ff).wrapping_mul(0x640001) >> 16;
                let v3 = (v2 & 0x0000ffff0000ffff).wrapping_mul(0x271000000001) >> 32;
                v3 as u32
            }
            fn write_unsigned_naive(output: &mut Vec<u8>, mut value: u64) {
                if value == 0 {
                    output.push(b'0');
                    return;
                }
                let mut buf = [0_u8; 20];
                let mut index = buf.len();
                while value > 0 {
                    index -= 1;
                    buf[index] = (value % 10) as u8 + b'0';
                    value /= 10;
                }
                output.extend_from_slice(&buf[index..]);
            }
            fn write_signed_naive(output: &mut Vec<u8>, value: i64) {
                if value < 0 {
                    output.push(b'-');
                    if value == i64::MIN {
                        let magnitude = (i64::MAX as u64) + 1;
                        Self::write_unsigned_naive(output, magnitude);
                        return;
                    }
                    let magnitude = (-value) as u64;
                    Self::write_unsigned_naive(output, magnitude);
                } else {
                    Self::write_unsigned_naive(output, value as u64);
                }
            }
        }
    }
}
