use __bundled::anmitsu::modulo998244353::fps::bostan_mori::linear_recurrence_kth_term;
use __bundled::fastio::Fastio;
fn main() {
    let mut io = Fastio::new();
    let d = io.u64() as usize;
    let k = io.u64() as usize;
    let mut a = Vec::with_capacity(d);
    for _ in 0..d {
        a.push(io.u32());
    }
    let mut c = Vec::with_capacity(d);
    for _ in 0..d {
        c.push(io.u32());
    }
    let ans = linear_recurrence_kth_term(&a, &c, k);
    io.writeln(ans);
    io.flush();
}
pub mod __bundled {
    pub mod anmitsu {
        pub mod modulo998244353 {
            pub mod convolution {
                use super::modulo;
                pub const MOD: u32 = 998244353;
                pub const MAX_NTT_LEN: usize = 1 << 18;
                const NTT_NAIVE_THRESHOLD: usize = 32;
                const NTT_NAIVE_LG_MAX: usize = (usize::BITS as usize - 1)
                    - (NTT_NAIVE_THRESHOLD.leading_zeros() as usize);
                const NAIVE_NTT_OMEGA_POWS: [[u32; NTT_NAIVE_THRESHOLD]; NTT_NAIVE_LG_MAX
                    + 1] = build_naive_ntt_omega_pows();
                const NAIVE_INTT_OMEGA_INV: [u32; NTT_NAIVE_LG_MAX + 1] = build_naive_intt_omega_inv();
                const NAIVE_NTT_BASE_POWS: [[[u32; NTT_NAIVE_THRESHOLD]; NTT_NAIVE_THRESHOLD]; NTT_NAIVE_LG_MAX
                    + 1] = build_naive_ntt_base_pows();
                const NAIVE_INTT_BASE_POWS: [[[u32; NTT_NAIVE_THRESHOLD]; NTT_NAIVE_THRESHOLD]; NTT_NAIVE_LG_MAX
                    + 1] = build_naive_intt_base_pows();
                const NAIVE_BIT_REVERSE: [[usize; NTT_NAIVE_THRESHOLD]; NTT_NAIVE_LG_MAX
                    + 1] = build_naive_bit_reverse();
                pub const PRIMITIVE_ROOT: u32 = 3;
                pub const NTT_RATE: [u32; 22] = [
                    0x3656d65b, 0x1e5ea9e6, 0x16038782, 0x13caac90, 0x3a9a4cfa,
                    0x761af21, 0xe372007, 0x3a2be7d4, 0x23fe18b2, 0x330f5b68, 0x7d37cf9,
                    0x3239edef, 0x2b8ea5c3, 0x382d2452, 0x300e9be2, 0x908b3f5,
                    0x1e726cd9, 0x1e02c2f0, 0x2c49629c, 0x2c2b7c93, 0x35a5081, 0x33b69d8b,
                ];
                pub const INTT_RATE: [u32; 22] = [
                    0x52929a6, 0x163456b8, 0x16400573, 0x267c5b5f, 0x6b059a5, 0x294c15f1,
                    0x94415d9, 0x2f83389c, 0x569c0ec, 0x3346ebba, 0x37473ab0, 0x1524e16f,
                    0x68442e3, 0x117ab9d0, 0x1fe52df0, 0x1263f553, 0x7392943, 0x24433aa8,
                    0x1a2993eb, 0x156d2fbf, 0x311e570f, 0x6294a13,
                ];
                pub const INVS: [u32; 23] = [
                    1, 499122177, 748683265, 873463809, 935854081, 967049217, 982646785,
                    990445569, 994344961, 996294657, 997269505, 997756929, 998000641,
                    998122497, 998183425, 998213889, 998229121, 998236737, 998240545,
                    998242449, 998243401, 998243877, 998244115,
                ];
                const fn build_naive_ntt_omega_pows() -> [[u32; NTT_NAIVE_THRESHOLD]; NTT_NAIVE_LG_MAX
                    + 1] {
                    let mut table = [[0_u32; NTT_NAIVE_THRESHOLD]; NTT_NAIVE_LG_MAX + 1];
                    let mut lg = 0usize;
                    while lg <= NTT_NAIVE_LG_MAX {
                        let n = 1usize << lg;
                        let exp = (MOD as usize - 1) / n;
                        let omega = modulo::pow(PRIMITIVE_ROOT, exp);
                        let mut p = 1_u32;
                        let mut i = 0usize;
                        while i < n {
                            table[lg][i] = p;
                            p = modulo::mul(p, omega);
                            i += 1;
                        }
                        lg += 1;
                    }
                    table
                }
                const fn build_naive_intt_omega_inv() -> [u32; NTT_NAIVE_LG_MAX + 1] {
                    let mut table = [0_u32; NTT_NAIVE_LG_MAX + 1];
                    let mut lg = 0usize;
                    while lg <= NTT_NAIVE_LG_MAX {
                        let n = 1usize << lg;
                        let exp = (MOD as usize - 1) / n;
                        let omega = modulo::pow(PRIMITIVE_ROOT, exp);
                        table[lg] = modulo::inv(omega);
                        lg += 1;
                    }
                    table
                }
                const fn build_naive_ntt_base_pows() -> [[[u32; NTT_NAIVE_THRESHOLD]; NTT_NAIVE_THRESHOLD]; NTT_NAIVE_LG_MAX
                    + 1] {
                    let mut table = [[[0_u32; NTT_NAIVE_THRESHOLD]; NTT_NAIVE_THRESHOLD]; NTT_NAIVE_LG_MAX
                        + 1];
                    let mut lg = 0usize;
                    while lg <= NTT_NAIVE_LG_MAX {
                        let n = 1usize << lg;
                        let mut k = 0usize;
                        while k < n {
                            let base = NAIVE_NTT_OMEGA_POWS[lg][k];
                            let mut p = 1_u32;
                            let mut j = 0usize;
                            while j < n {
                                table[lg][k][j] = p;
                                p = modulo::mul(p, base);
                                j += 1;
                            }
                            k += 1;
                        }
                        lg += 1;
                    }
                    table
                }
                const fn build_naive_intt_base_pows() -> [[[u32; NTT_NAIVE_THRESHOLD]; NTT_NAIVE_THRESHOLD]; NTT_NAIVE_LG_MAX
                    + 1] {
                    let mut table = [[[0_u32; NTT_NAIVE_THRESHOLD]; NTT_NAIVE_THRESHOLD]; NTT_NAIVE_LG_MAX
                        + 1];
                    let mut lg = 0usize;
                    while lg <= NTT_NAIVE_LG_MAX {
                        let n = 1usize << lg;
                        let omega_inv = NAIVE_INTT_OMEGA_INV[lg];
                        let mut omega_inv_pows = [0_u32; NTT_NAIVE_THRESHOLD];
                        let mut p = 1_u32;
                        let mut j = 0usize;
                        while j < n {
                            omega_inv_pows[j] = p;
                            p = modulo::mul(p, omega_inv);
                            j += 1;
                        }
                        j = 0usize;
                        while j < n {
                            let base = omega_inv_pows[j];
                            let mut p = 1_u32;
                            let mut k = 0usize;
                            while k < n {
                                table[lg][j][k] = p;
                                p = modulo::mul(p, base);
                                k += 1;
                            }
                            j += 1;
                        }
                        lg += 1;
                    }
                    table
                }
                const fn build_naive_bit_reverse() -> [[usize; NTT_NAIVE_THRESHOLD]; NTT_NAIVE_LG_MAX
                    + 1] {
                    let mut table = [[0usize; NTT_NAIVE_THRESHOLD]; NTT_NAIVE_LG_MAX
                        + 1];
                    let mut lg = 0usize;
                    while lg <= NTT_NAIVE_LG_MAX {
                        let n = 1usize << lg;
                        let mut i = 0usize;
                        while i < n {
                            let mut x = i;
                            let mut res = 0usize;
                            let mut j = 0usize;
                            while j < lg {
                                res = (res << 1) | (x & 1);
                                x >>= 1;
                                j += 1;
                            }
                            table[lg][i] = res;
                            i += 1;
                        }
                        lg += 1;
                    }
                    table
                }
                pub fn bit_reverse(mut x: usize, lg: usize) -> usize {
                    let mut res = 0usize;
                    for _ in 0..lg {
                        res = (res << 1) | (x & 1);
                        x >>= 1;
                    }
                    res
                }
                fn ntt_naive(a: &mut [u32]) {
                    let n = a.len();
                    debug_assert!(n.is_power_of_two());
                    debug_assert!(n > 0);
                    debug_assert!(n <= NTT_NAIVE_THRESHOLD);
                    if n == 1 {
                        return;
                    }
                    let lg = n.trailing_zeros() as usize;
                    let bit_reverse = &NAIVE_BIT_REVERSE[lg];
                    let mut buf = [0_u32; NTT_NAIVE_THRESHOLD];
                    for i in 0..n {
                        let k = bit_reverse[i];
                        let pow_table = &NAIVE_NTT_BASE_POWS[lg][k];
                        let mut sum = 0_u32;
                        for j in 0..n {
                            sum = modulo::add(sum, modulo::mul(a[j], pow_table[j]));
                        }
                        buf[i] = sum;
                    }
                    a.copy_from_slice(&buf[..n]);
                }
                fn intt_naive(a: &mut [u32]) {
                    let n = a.len();
                    debug_assert!(n.is_power_of_two());
                    debug_assert!(n > 0);
                    debug_assert!(n <= NTT_NAIVE_THRESHOLD);
                    if n == 1 {
                        return;
                    }
                    let lg = n.trailing_zeros() as usize;
                    let bit_reverse = &NAIVE_BIT_REVERSE[lg];
                    let mut buf = [0_u32; NTT_NAIVE_THRESHOLD];
                    for j in 0..n {
                        let pow_table = &NAIVE_INTT_BASE_POWS[lg][j];
                        let mut sum = 0_u32;
                        for k in 0..n {
                            sum = modulo::add(
                                sum,
                                modulo::mul(a[bit_reverse[k]], pow_table[k]),
                            );
                        }
                        buf[j] = sum;
                    }
                    a.copy_from_slice(&buf[..n]);
                }
                fn ntt_butterfly(a: &mut [u32]) {
                    let n = a.len();
                    debug_assert!(n.is_power_of_two());
                    debug_assert!(n > 0);
                    debug_assert!(a.iter().all(|& x | x < MOD));
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
                                    let r = modulo::mul(*ptr.add(i + p), rot);
                                    *ptr.add(i) = modulo::add(l, r);
                                    *ptr.add(i + p) = modulo::sub(l, r);
                                }
                            }
                            rot = modulo::mul(rot, NTT_RATE[s.trailing_ones() as usize]);
                        }
                    }
                }
                fn intt_butterfly(a: &mut [u32]) {
                    let n = a.len();
                    debug_assert!(n.is_power_of_two());
                    debug_assert!(n > 0);
                    debug_assert!(a.iter().all(|& x | x < MOD));
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
                                    *ptr.add(i) = modulo::add(l, r);
                                    *ptr.add(i + p) = modulo::mul(modulo::sub(l, r), irot);
                                }
                            }
                            irot = modulo::mul(
                                irot,
                                INTT_RATE[s.trailing_ones() as usize],
                            );
                        }
                    }
                }
                pub fn ntt(a: &mut [u32]) {
                    if a.is_empty() {
                        return;
                    }
                    let n = a.len();
                    assert!(
                        n.is_power_of_two(), "NTT length {} is not a power of two", n
                    );
                    assert!(
                        n <= MAX_NTT_LEN, "NTT length {} exceeds supported maximum {}",
                        n, MAX_NTT_LEN
                    );
                    debug_assert!(a.iter().all(|& x | x < MOD));
                    #[cfg(target_arch = "x86_64")]
                    {
                        if std::is_x86_feature_detected!("avx2") {
                            unsafe {
                                super::convolution_avx2::ntt_avx2(a);
                            }
                            return;
                        }
                    }
                    if n <= NTT_NAIVE_THRESHOLD {
                        ntt_naive(a);
                        return;
                    }
                    ntt_butterfly(a);
                }
                pub fn intt(a: &mut [u32]) {
                    if a.is_empty() {
                        return;
                    }
                    let n = a.len();
                    assert!(
                        n.is_power_of_two(), "NTT length {} is not a power of two", n
                    );
                    assert!(
                        n <= MAX_NTT_LEN, "NTT length {} exceeds supported maximum {}",
                        n, MAX_NTT_LEN
                    );
                    debug_assert!(a.iter().all(|& x | x < MOD));
                    #[cfg(target_arch = "x86_64")]
                    {
                        if std::is_x86_feature_detected!("avx2") {
                            unsafe {
                                super::convolution_avx2::intt_avx2(a);
                            }
                            return;
                        }
                    }
                    if n <= NTT_NAIVE_THRESHOLD {
                        intt_naive(a);
                        return;
                    }
                    intt_butterfly(a);
                }
                pub fn convolution(mut a: Vec<u32>, mut b: Vec<u32>) -> Vec<u32> {
                    if a.is_empty() || b.is_empty() {
                        return Vec::new();
                    }
                    debug_assert!(a.iter().all(|& x | x < MOD));
                    debug_assert!(b.iter().all(|& x | x < MOD));
                    #[cfg(target_arch = "x86_64")]
                    {
                        if std::is_x86_feature_detected!("avx2") {
                            unsafe {
                                return super::convolution_avx2::convolution_avx2(a, b);
                            }
                        }
                    }
                    let s = a.len() + b.len() - 1;
                    if a.len().min(b.len()) <= 32 {
                        let mut res = vec![0; s];
                        for i in 0..a.len() {
                            let ai = a[i];
                            for j in 0..b.len() {
                                res[i + j] = modulo::add(res[i + j], modulo::mul(ai, b[j]));
                            }
                        }
                        return res;
                    }
                    let t = s.next_power_of_two();
                    assert!(
                        t <= MAX_NTT_LEN,
                        "Convolution length {} exceeds supported maximum {}", t,
                        MAX_NTT_LEN
                    );
                    a.resize(t, 0);
                    b.resize(t, 0);
                    ntt(&mut a);
                    ntt(&mut b);
                    a.iter_mut()
                        .zip(b.iter())
                        .for_each(|(x, y)| *x = modulo::mul(*x, *y));
                    intt(&mut a);
                    let t_inv = INVS[t.trailing_zeros() as usize];
                    a.iter_mut().take(s).for_each(|x| *x = modulo::mul(*x, t_inv));
                    a.truncate(s);
                    a
                }
            }
            mod convolution_avx2 {
                use super::convolution;
                #[cfg(target_arch = "x86_64")]
                use super::convolution_mont;
                #[cfg(target_arch = "x86_64")]
                pub(super) unsafe fn ntt_avx2(a: &mut [u32]) {
                    unsafe {
                        debug_assert!(std::is_x86_feature_detected!("avx2"));
                        debug_assert!(! a.is_empty());
                        debug_assert!(a.len().is_power_of_two());
                        convolution_mont::standard_to_mont(a);
                        convolution_mont::ntt_mont(a);
                        convolution_mont::mont_to_standard(a);
                    }
                }
                #[cfg(target_arch = "x86_64")]
                pub(super) unsafe fn intt_avx2(a: &mut [u32]) {
                    unsafe {
                        debug_assert!(std::is_x86_feature_detected!("avx2"));
                        debug_assert!(! a.is_empty());
                        debug_assert!(a.len().is_power_of_two());
                        convolution_mont::standard_to_mont(a);
                        convolution_mont::intt_mont(a);
                        convolution_mont::mont_to_standard(a);
                    }
                }
                #[cfg(target_arch = "x86_64")]
                pub unsafe fn convolution_avx2(
                    mut a: Vec<u32>,
                    mut b: Vec<u32>,
                ) -> Vec<u32> {
                    if a.is_empty() || b.is_empty() {
                        return Vec::new();
                    }
                    debug_assert!(a.iter().all(|& x | x < convolution::MOD));
                    debug_assert!(b.iter().all(|& x | x < convolution::MOD));
                    let s = a.len() + b.len() - 1;
                    let t = s.next_power_of_two();
                    assert!(
                        t <= convolution::MAX_NTT_LEN,
                        "Convolution length {} exceeds supported maximum {}", t,
                        convolution::MAX_NTT_LEN
                    );
                    unsafe {
                        if a.len().min(b.len()) <= 32 {
                            let mut res = vec![0_u32; s];
                            for (i, &ai) in a.iter().enumerate() {
                                for (j, &bj) in b.iter().enumerate() {
                                    res[i + j] = super::modulo::add(
                                        res[i + j],
                                        super::modulo::mul(ai, bj),
                                    );
                                }
                            }
                            return res;
                        }
                        a.resize(t, 0);
                        b.resize(t, 0);
                        convolution_mont::standard_to_mont(&mut a);
                        convolution_mont::standard_to_mont(&mut b);
                        convolution_mont::ntt_mont(&mut a);
                        convolution_mont::ntt_mont(&mut b);
                        convolution_mont::mul_pointwise_mont(&mut a, &mut b);
                        convolution_mont::intt_mont(&mut a);
                        let inv_len_mont = convolution_mont::inv_len_mont(
                            t.trailing_zeros() as usize,
                        );
                        convolution_mont::mul_scalar_mont(&mut a, inv_len_mont);
                        convolution_mont::mont_to_standard(&mut a);
                        let mut res = Vec::with_capacity(s);
                        res.extend_from_slice(&a.as_mut_slice()[..s]);
                        res
                    }
                }
            }
            pub mod convolution_mont {
                use super::super::modulo998244353::modulo;
                use super::convolution;
                use std::arch::x86_64;
                use std::sync;
                const N_INV: u32 = 998244351;
                pub const R: u32 = 301989884;
                const R2: u32 = 932051910;
                pub const NTT_RATE_MONT: [u32; 22] = [
                    0x2934548a, 0x125558a6, 0x21c90447, 0x34588745, 0x165c4943,
                    0x83d9830, 0x1dd6967a, 0x4b74372, 0x2f24280c, 0x3a503634, 0x26d3f337,
                    0x12667d13, 0x2b181adb, 0x1c4cd5c4, 0x28bbc449, 0x2a18c05,
                    0x2000526a, 0x3860c1e5, 0xa74a97e, 0x1ff54d24, 0x31931580, 0x2b009445,
                ];
                pub const INTT_RATE_MONT: [u32; 22] = [
                    0x124bab77, 0x34f7035f, 0x844bfb0, 0x3ea0705, 0x323893d2, 0x38d16113,
                    0xba20d91, 0x7137c51, 0x2f35c41b, 0x316125c4, 0x362a09f8, 0xd06f7b0,
                    0x25764555, 0xecb65ec, 0x21c524da, 0x5fe919, 0x4ebf1f8, 0x2fab632,
                    0xd6f87e4, 0x14cfdeae, 0x3aaa342, 0x2d7dadf0,
                ];
                const INVS_MONT: [u32; 23] = [
                    0x11fffffc, 0x08fffffe, 0x047fffff, 0x20000000, 0x10000000,
                    0x08000000, 0x04000000, 0x02000000, 0x01000000, 0x00800000,
                    0x00400000, 0x00200000, 0x00100000, 0x00080000, 0x00040000,
                    0x00020000, 0x00010000, 0x00008000, 0x00004000, 0x00002000,
                    0x00001000, 0x00000800, 0x00000400,
                ];
                const AVX2_U32_LANES: usize = 8;
                #[inline(always)]
                pub fn reduce_mont(val: u64) -> u32 {
                    let t = (val as u32).wrapping_mul(N_INV);
                    let res = ((val + t as u64 * convolution::MOD as u64) >> 32) as u32;
                    if res >= convolution::MOD { res - convolution::MOD } else { res }
                }
                #[inline(always)]
                pub fn mul_mont(a: u32, b: u32) -> u32 {
                    reduce_mont(a as u64 * b as u64)
                }
                #[inline(always)]
                pub fn standard_to_mont_scalar(x: u32) -> u32 {
                    mul_mont(x, R2)
                }
                #[inline(always)]
                pub fn mont_to_standard_scalar(x_mont: u32) -> u32 {
                    dbg!("mont_to_standard_scalar");
                    reduce_mont(x_mont as u64)
                }
                #[inline(always)]
                pub fn inv_len_mont(lg: usize) -> u32 {
                    INVS_MONT[lg]
                }
                const NTT_DOUBLING_LG_MAX: usize = convolution::MAX_NTT_LEN
                    .trailing_zeros() as usize - 1;
                const NTT_DOUBLING_ZETA: [u32; NTT_DOUBLING_LG_MAX + 1] = build_ntt_doubling_zeta();
                const fn build_ntt_doubling_zeta() -> [u32; NTT_DOUBLING_LG_MAX + 1] {
                    let mut table = [0_u32; NTT_DOUBLING_LG_MAX + 1];
                    let mut lg = 0usize;
                    while lg <= NTT_DOUBLING_LG_MAX {
                        let n = 1usize << lg;
                        let exp = (modulo::M as usize - 1) / (2 * n);
                        table[lg] = modulo::pow(convolution::PRIMITIVE_ROOT, exp);
                        lg += 1;
                    }
                    table
                }
                struct NttDoublingPowersMont {
                    zeta_mont_by_lg: [u32; NTT_DOUBLING_LG_MAX + 1],
                    pows_mont_by_lg: [sync::OnceLock<
                        Box<[u32]>,
                    >; NTT_DOUBLING_LG_MAX + 1],
                }
                impl NttDoublingPowersMont {
                    fn new() -> Self {
                        let zeta_mont_by_lg = std::array::from_fn(|lg| standard_to_mont_scalar(
                            NTT_DOUBLING_ZETA[lg],
                        ));
                        let pows_mont_by_lg = std::array::from_fn(|_| sync::OnceLock::new());
                        Self {
                            zeta_mont_by_lg,
                            pows_mont_by_lg,
                        }
                    }
                    fn powers(&self, n: usize) -> &[u32] {
                        debug_assert!(n.is_power_of_two());
                        debug_assert!(n > 0);
                        debug_assert!(2 * n <= convolution::MAX_NTT_LEN);
                        let lg = n.trailing_zeros() as usize;
                        debug_assert!(lg <= NTT_DOUBLING_LG_MAX);
                        self.pows_mont_by_lg[lg]
                            .get_or_init(|| {
                                let zeta_mont = self.zeta_mont_by_lg[lg];
                                let mut res = vec![0_u32; n];
                                let mut p = standard_to_mont_scalar(1);
                                for v in res.iter_mut() {
                                    *v = p;
                                    p = mul_mont(p, zeta_mont);
                                }
                                res.into_boxed_slice()
                            })
                            .as_ref()
                    }
                }
                static NTT_DOUBLING_POWERS_MONT: sync::OnceLock<NttDoublingPowersMont> = sync::OnceLock::new();
                static NTT: sync::OnceLock<Ntt> = sync::OnceLock::new();
                #[derive(Clone, Copy)]
                struct MontgomerySimd {
                    mod_v: x86_64::__m256i,
                    n_inv_v: x86_64::__m256i,
                    r2_v: x86_64::__m256i,
                }
                impl MontgomerySimd {
                    #[target_feature(enable = "avx2")]
                    unsafe fn new() -> Self {
                        Self {
                            mod_v: x86_64::_mm256_set1_epi32(convolution::MOD as i32),
                            n_inv_v: x86_64::_mm256_set1_epi32(N_INV as i32),
                            r2_v: x86_64::_mm256_set1_epi32(R2 as i32),
                        }
                    }
                    #[inline(always)]
                    fn shrink(self, vec: x86_64::__m256i) -> x86_64::__m256i {
                        unsafe {
                            x86_64::_mm256_min_epu32(
                                vec,
                                x86_64::_mm256_sub_epi32(vec, self.mod_v),
                            )
                        }
                    }
                    #[inline(always)]
                    unsafe fn reduce(
                        self,
                        x0246: x86_64::__m256i,
                        x1357: x86_64::__m256i,
                    ) -> x86_64::__m256i {
                        unsafe {
                            let x0246_ninv = x86_64::_mm256_mul_epu32(
                                x0246,
                                self.n_inv_v,
                            );
                            let x1357_ninv = x86_64::_mm256_mul_epu32(
                                x1357,
                                self.n_inv_v,
                            );
                            let x0246_res = x86_64::_mm256_add_epi64(
                                x0246,
                                x86_64::_mm256_mul_epu32(x0246_ninv, self.mod_v),
                            );
                            let x1357_res = x86_64::_mm256_add_epi64(
                                x1357,
                                x86_64::_mm256_mul_epu32(x1357_ninv, self.mod_v),
                            );
                            let mut res = x86_64::_mm256_or_si256(
                                x86_64::_mm256_bsrli_epi128(x0246_res, 4),
                                x1357_res,
                            );
                            res = self.shrink(res);
                            res
                        }
                    }
                    #[inline(always)]
                    unsafe fn mul_u32x8<const B_USE_ONLY_EVEN: bool>(
                        self,
                        a: x86_64::__m256i,
                        b: x86_64::__m256i,
                    ) -> x86_64::__m256i {
                        unsafe {
                            let a_sh = x86_64::_mm256_bsrli_epi128(a, 4);
                            let b_sh = if B_USE_ONLY_EVEN {
                                b
                            } else {
                                x86_64::_mm256_bsrli_epi128(b, 4)
                            };
                            let x0246 = x86_64::_mm256_mul_epu32(a, b);
                            let x1357 = x86_64::_mm256_mul_epu32(a_sh, b_sh);
                            self.reduce(x0246, x1357)
                        }
                    }
                }
                struct Ntt {
                    mts: MontgomerySimd,
                    ntt_rots_by_chunks_lg_max: Box<[u32]>,
                    intt_rots_by_chunks_lg_max: Box<[u32]>,
                }
                impl Ntt {
                    fn build_rot_pows(rate: &[u32; 22], chunks_lg: usize) -> Box<[u32]> {
                        let chunks = 1usize << chunks_lg;
                        let mut res = vec![0_u32; chunks];
                        let mut rot = standard_to_mont_scalar(1);
                        for (s, v) in res.iter_mut().enumerate() {
                            *v = rot;
                            let idx = (s as u32).trailing_ones() as usize;
                            rot = mul_mont(rot, rate[idx]);
                        }
                        res.into_boxed_slice()
                    }
                    #[target_feature(enable = "avx2")]
                    unsafe fn new() -> Self {
                        unsafe {
                            let mts = MontgomerySimd::new();
                            let max_chunks_lg = convolution::MAX_NTT_LEN.trailing_zeros()
                                as usize - 1;
                            let ntt_rots_by_chunks_lg_max = Self::build_rot_pows(
                                &NTT_RATE_MONT,
                                max_chunks_lg,
                            );
                            let intt_rots_by_chunks_lg_max = Self::build_rot_pows(
                                &INTT_RATE_MONT,
                                max_chunks_lg,
                            );
                            Self {
                                mts,
                                ntt_rots_by_chunks_lg_max,
                                intt_rots_by_chunks_lg_max,
                            }
                        }
                    }
                    #[target_feature(enable = "avx2")]
                    unsafe fn to_mont(&self, data: *mut u32, len: usize) {
                        unsafe {
                            debug_assert!(len.is_power_of_two());
                            debug_assert!(len >= AVX2_U32_LANES);
                            debug_assert_eq!(0, len % AVX2_U32_LANES);
                            let mts = self.mts;
                            for i in (0..len).step_by(AVX2_U32_LANES) {
                                let v = x86_64::_mm256_loadu_si256(data.add(i).cast());
                                let v = mts.mul_u32x8::<true>(v, mts.r2_v);
                                x86_64::_mm256_storeu_si256(data.add(i).cast(), v);
                            }
                        }
                    }
                    #[target_feature(enable = "avx2")]
                    unsafe fn to_standard(&self, data: *mut u32, len: usize) {
                        unsafe {
                            debug_assert!(len.is_power_of_two());
                            debug_assert!(len >= AVX2_U32_LANES);
                            debug_assert_eq!(0, len % AVX2_U32_LANES);
                            let mts = self.mts;
                            let one = x86_64::_mm256_set1_epi32(1);
                            for i in (0..len).step_by(AVX2_U32_LANES) {
                                let v = x86_64::_mm256_loadu_si256(data.add(i).cast());
                                let v = mts.mul_u32x8::<true>(v, one);
                                x86_64::_mm256_storeu_si256(data.add(i).cast(), v);
                            }
                        }
                    }
                    #[target_feature(enable = "avx2")]
                    unsafe fn mul_pointwise_mont(
                        &self,
                        a: *mut u32,
                        b: *const u32,
                        len: usize,
                    ) {
                        unsafe {
                            debug_assert!(len.is_power_of_two());
                            debug_assert!(len >= AVX2_U32_LANES);
                            debug_assert_eq!(0, len % AVX2_U32_LANES);
                            let mts = self.mts;
                            for i in (0..len).step_by(AVX2_U32_LANES) {
                                let va = x86_64::_mm256_loadu_si256(a.add(i).cast());
                                let vb = x86_64::_mm256_loadu_si256(b.add(i).cast());
                                let prod = mts.mul_u32x8::<false>(va, vb);
                                x86_64::_mm256_storeu_si256(a.add(i).cast(), prod);
                            }
                        }
                    }
                    #[target_feature(enable = "avx2")]
                    unsafe fn mul_scalar_mont(
                        &self,
                        data: *mut u32,
                        len: usize,
                        sc_mont: u32,
                    ) {
                        unsafe {
                            debug_assert!(len.is_power_of_two());
                            debug_assert!(len >= AVX2_U32_LANES);
                            debug_assert_eq!(0, len % AVX2_U32_LANES);
                            let mts = self.mts;
                            let sc_v = x86_64::_mm256_set1_epi32(sc_mont as i32);
                            for i in (0..len).step_by(AVX2_U32_LANES) {
                                let v = x86_64::_mm256_loadu_si256(data.add(i).cast());
                                let v = mts.mul_u32x8::<true>(v, sc_v);
                                x86_64::_mm256_storeu_si256(data.add(i).cast(), v);
                            }
                        }
                    }
                    #[target_feature(enable = "avx2")]
                    unsafe fn ntt_mont(&self, data: *mut u32, len: usize) {
                        unsafe {
                            debug_assert!(len.is_power_of_two());
                            debug_assert!(len >= AVX2_U32_LANES);
                            let lg = len.trailing_zeros() as usize;
                            let n = len;
                            let mts = self.mts;
                            for stage in 0..lg {
                                let p = 1usize << (lg - stage - 1);
                                let step = 1usize << (lg - stage);
                                let chunks = n / step;
                                let rots = self.ntt_rots_by_chunks_lg_max.as_ref();
                                debug_assert!(rots.len() >= chunks);
                                if p >= 8 {
                                    for s in 0..chunks {
                                        let ptr = data.add(s * step);
                                        let rot = rots[s];
                                        let rot_v = x86_64::_mm256_set1_epi32(rot as i32);
                                        for i in (0..p).step_by(8) {
                                            let l = x86_64::_mm256_loadu_si256(ptr.add(i).cast());
                                            let r = x86_64::_mm256_loadu_si256(ptr.add(i + p).cast());
                                            let r = mts.mul_u32x8::<true>(r, rot_v);
                                            let sum = mts.shrink(x86_64::_mm256_add_epi32(l, r));
                                            let diff = mts
                                                .shrink(
                                                    x86_64::_mm256_sub_epi32(
                                                        x86_64::_mm256_add_epi32(l, mts.mod_v),
                                                        r,
                                                    ),
                                                );
                                            x86_64::_mm256_storeu_si256(ptr.add(i).cast(), sum);
                                            x86_64::_mm256_storeu_si256(ptr.add(i + p).cast(), diff);
                                        }
                                    }
                                } else {
                                    if p == 4 {
                                        for s in 0..chunks {
                                            let ptr = data.add(8 * s);
                                            let rot = rots[s];
                                            let l0 = *ptr.add(0);
                                            let l1 = *ptr.add(1);
                                            let l2 = *ptr.add(2);
                                            let l3 = *ptr.add(3);
                                            let r0 = mul_mont(*ptr.add(4), rot);
                                            let r1 = mul_mont(*ptr.add(5), rot);
                                            let r2 = mul_mont(*ptr.add(6), rot);
                                            let r3 = mul_mont(*ptr.add(7), rot);
                                            *ptr.add(0) = super::modulo::add(l0, r0);
                                            *ptr.add(1) = super::modulo::add(l1, r1);
                                            *ptr.add(2) = super::modulo::add(l2, r2);
                                            *ptr.add(3) = super::modulo::add(l3, r3);
                                            *ptr.add(5) = super::modulo::sub(l1, r1);
                                            *ptr.add(4) = super::modulo::sub(l0, r0);
                                            *ptr.add(6) = super::modulo::sub(l2, r2);
                                            *ptr.add(7) = super::modulo::sub(l3, r3);
                                        }
                                    } else if p == 2 {
                                        debug_assert_eq!(0, chunks % 2);
                                        let idx_l = x86_64::_mm256_setr_epi32(
                                            0,
                                            1,
                                            0,
                                            1,
                                            4,
                                            5,
                                            4,
                                            5,
                                        );
                                        let idx_r = x86_64::_mm256_setr_epi32(
                                            2,
                                            3,
                                            2,
                                            3,
                                            6,
                                            7,
                                            6,
                                            7,
                                        );
                                        for s in (0..chunks).step_by(2) {
                                            let s1 = s + 1;
                                            let rot0 = rots[s];
                                            let rot1 = rots[s1];
                                            let mul_v = x86_64::_mm256_setr_epi32(
                                                R as i32,
                                                R as i32,
                                                rot0 as i32,
                                                rot0 as i32,
                                                R as i32,
                                                R as i32,
                                                rot1 as i32,
                                                rot1 as i32,
                                            );
                                            let ptr = data.add(4 * s);
                                            let v = x86_64::_mm256_loadu_si256(ptr.cast());
                                            let v = mts.mul_u32x8::<true>(v, mul_v);
                                            let l = x86_64::_mm256_permutevar8x32_epi32(v, idx_l);
                                            let r = x86_64::_mm256_permutevar8x32_epi32(v, idx_r);
                                            let sum = mts.shrink(x86_64::_mm256_add_epi32(l, r));
                                            let diff = mts
                                                .shrink(
                                                    x86_64::_mm256_sub_epi32(
                                                        x86_64::_mm256_add_epi32(l, mts.mod_v),
                                                        r,
                                                    ),
                                                );
                                            let out = x86_64::_mm256_blend_epi32(sum, diff, 0xCC);
                                            x86_64::_mm256_storeu_si256(ptr.cast(), out);
                                        }
                                    } else if p == 1 {
                                        debug_assert_eq!(0, chunks % 4);
                                        let idx_l = x86_64::_mm256_setr_epi32(
                                            0,
                                            0,
                                            2,
                                            2,
                                            4,
                                            4,
                                            6,
                                            6,
                                        );
                                        let idx_r = x86_64::_mm256_setr_epi32(
                                            1,
                                            1,
                                            3,
                                            3,
                                            5,
                                            5,
                                            7,
                                            7,
                                        );
                                        for s in (0..chunks).step_by(4) {
                                            let s1 = s + 1;
                                            let s2 = s + 2;
                                            let s3 = s + 3;
                                            let rot0 = rots[s];
                                            let rot1 = rots[s1];
                                            let rot2 = rots[s2];
                                            let rot3 = rots[s3];
                                            let mul_v = x86_64::_mm256_setr_epi32(
                                                R as i32,
                                                rot0 as i32,
                                                R as i32,
                                                rot1 as i32,
                                                R as i32,
                                                rot2 as i32,
                                                R as i32,
                                                rot3 as i32,
                                            );
                                            let ptr = data.add(2 * s);
                                            let v = x86_64::_mm256_loadu_si256(ptr.cast());
                                            let v = mts.mul_u32x8::<false>(v, mul_v);
                                            let l = x86_64::_mm256_permutevar8x32_epi32(v, idx_l);
                                            let r = x86_64::_mm256_permutevar8x32_epi32(v, idx_r);
                                            let sum = mts.shrink(x86_64::_mm256_add_epi32(l, r));
                                            let diff = mts
                                                .shrink(
                                                    x86_64::_mm256_sub_epi32(
                                                        x86_64::_mm256_add_epi32(l, mts.mod_v),
                                                        r,
                                                    ),
                                                );
                                            let out = x86_64::_mm256_blend_epi32(sum, diff, 0xAA);
                                            x86_64::_mm256_storeu_si256(ptr.cast(), out);
                                        }
                                    } else {
                                        unreachable!();
                                    }
                                }
                            }
                        }
                    }
                    #[target_feature(enable = "avx2")]
                    unsafe fn intt_mont(&self, data: *mut u32, len: usize) {
                        unsafe {
                            debug_assert!(len.is_power_of_two());
                            debug_assert!(len >= AVX2_U32_LANES);
                            let lg = len.trailing_zeros() as usize;
                            let n = len;
                            let mts = self.mts;
                            for stage in (1..=lg).rev() {
                                let p = 1usize << (lg - stage);
                                let step = 1usize << (lg - stage + 1);
                                let chunks = n / step;
                                let irots = self.intt_rots_by_chunks_lg_max.as_ref();
                                debug_assert!(irots.len() >= chunks);
                                if p >= 8 {
                                    for s in 0..chunks {
                                        let ptr = data.add(s * step);
                                        let irot = irots[s];
                                        let irot_v = x86_64::_mm256_set1_epi32(irot as i32);
                                        for i in (0..p).step_by(8) {
                                            let l = x86_64::_mm256_loadu_si256(ptr.add(i).cast());
                                            let r = x86_64::_mm256_loadu_si256(ptr.add(i + p).cast());
                                            let sum = mts.shrink(x86_64::_mm256_add_epi32(l, r));
                                            let diff = mts
                                                .shrink(
                                                    x86_64::_mm256_sub_epi32(
                                                        x86_64::_mm256_add_epi32(l, mts.mod_v),
                                                        r,
                                                    ),
                                                );
                                            let diff = mts.mul_u32x8::<true>(diff, irot_v);
                                            x86_64::_mm256_storeu_si256(ptr.add(i).cast(), sum);
                                            x86_64::_mm256_storeu_si256(ptr.add(i + p).cast(), diff);
                                        }
                                    }
                                } else {
                                    if p == 4 {
                                        for s in 0..chunks {
                                            let ptr = data.add(8 * s);
                                            let irot = irots[s];
                                            let l0 = *ptr.add(0);
                                            let l1 = *ptr.add(1);
                                            let l2 = *ptr.add(2);
                                            let l3 = *ptr.add(3);
                                            let r0 = *ptr.add(4);
                                            let r1 = *ptr.add(5);
                                            let r2 = *ptr.add(6);
                                            let r3 = *ptr.add(7);
                                            let sum0 = super::modulo::add(l0, r0);
                                            let sum1 = super::modulo::add(l1, r1);
                                            let sum2 = super::modulo::add(l2, r2);
                                            let sum3 = super::modulo::add(l3, r3);
                                            let diff0 = super::modulo::sub(l0, r0);
                                            let diff1 = super::modulo::sub(l1, r1);
                                            let diff2 = super::modulo::sub(l2, r2);
                                            let diff3 = super::modulo::sub(l3, r3);
                                            *ptr.add(0) = sum0;
                                            *ptr.add(1) = sum1;
                                            *ptr.add(2) = sum2;
                                            *ptr.add(3) = sum3;
                                            *ptr.add(4) = mul_mont(diff0, irot);
                                            *ptr.add(5) = mul_mont(diff1, irot);
                                            *ptr.add(6) = mul_mont(diff2, irot);
                                            *ptr.add(7) = mul_mont(diff3, irot);
                                        }
                                    } else if p == 2 {
                                        debug_assert_eq!(0, chunks % 2);
                                        let idx_l = x86_64::_mm256_setr_epi32(
                                            0,
                                            1,
                                            0,
                                            1,
                                            4,
                                            5,
                                            4,
                                            5,
                                        );
                                        let idx_r = x86_64::_mm256_setr_epi32(
                                            2,
                                            3,
                                            2,
                                            3,
                                            6,
                                            7,
                                            6,
                                            7,
                                        );
                                        for s in (0..chunks).step_by(2) {
                                            let s1 = s + 1;
                                            let irot0 = irots[s];
                                            let irot1 = irots[s1];
                                            let mul_v = x86_64::_mm256_setr_epi32(
                                                R as i32,
                                                R as i32,
                                                irot0 as i32,
                                                irot0 as i32,
                                                R as i32,
                                                R as i32,
                                                irot1 as i32,
                                                irot1 as i32,
                                            );
                                            let ptr = data.add(4 * s);
                                            let v = x86_64::_mm256_loadu_si256(ptr.cast());
                                            let l = x86_64::_mm256_permutevar8x32_epi32(v, idx_l);
                                            let r = x86_64::_mm256_permutevar8x32_epi32(v, idx_r);
                                            let sum = mts.shrink(x86_64::_mm256_add_epi32(l, r));
                                            let diff = mts
                                                .shrink(
                                                    x86_64::_mm256_sub_epi32(
                                                        x86_64::_mm256_add_epi32(l, mts.mod_v),
                                                        r,
                                                    ),
                                                );
                                            let out = x86_64::_mm256_blend_epi32(sum, diff, 0xCC);
                                            let out = mts.mul_u32x8::<true>(out, mul_v);
                                            x86_64::_mm256_storeu_si256(ptr.cast(), out);
                                        }
                                    } else if p == 1 {
                                        debug_assert_eq!(0, chunks % 4);
                                        let idx_l = x86_64::_mm256_setr_epi32(
                                            0,
                                            0,
                                            2,
                                            2,
                                            4,
                                            4,
                                            6,
                                            6,
                                        );
                                        let idx_r = x86_64::_mm256_setr_epi32(
                                            1,
                                            1,
                                            3,
                                            3,
                                            5,
                                            5,
                                            7,
                                            7,
                                        );
                                        for s in (0..chunks).step_by(4) {
                                            let s1 = s + 1;
                                            let s2 = s + 2;
                                            let s3 = s + 3;
                                            let irot0 = irots[s];
                                            let irot1 = irots[s1];
                                            let irot2 = irots[s2];
                                            let irot3 = irots[s3];
                                            let mul_v = x86_64::_mm256_setr_epi32(
                                                R as i32,
                                                irot0 as i32,
                                                R as i32,
                                                irot1 as i32,
                                                R as i32,
                                                irot2 as i32,
                                                R as i32,
                                                irot3 as i32,
                                            );
                                            let ptr = data.add(2 * s);
                                            let v = x86_64::_mm256_loadu_si256(ptr.cast());
                                            let l = x86_64::_mm256_permutevar8x32_epi32(v, idx_l);
                                            let r = x86_64::_mm256_permutevar8x32_epi32(v, idx_r);
                                            let sum = mts.shrink(x86_64::_mm256_add_epi32(l, r));
                                            let diff = mts
                                                .shrink(
                                                    x86_64::_mm256_sub_epi32(
                                                        x86_64::_mm256_add_epi32(l, mts.mod_v),
                                                        r,
                                                    ),
                                                );
                                            let out = x86_64::_mm256_blend_epi32(sum, diff, 0xAA);
                                            let out = mts.mul_u32x8::<false>(out, mul_v);
                                            x86_64::_mm256_storeu_si256(ptr.cast(), out);
                                        }
                                    } else {
                                        unreachable!();
                                    }
                                }
                            }
                        }
                    }
                }
                #[target_feature(enable = "avx2")]
                pub unsafe fn standard_to_mont(a: &mut [u32]) {
                    unsafe {
                        debug_assert!(! a.is_empty());
                        debug_assert!(a.len().is_power_of_two());
                        if a.len() == 1 {
                            a[0] = standard_to_mont_scalar(a[0]);
                            return;
                        } else if a.len() == 2 {
                            a[0] = standard_to_mont_scalar(a[0]);
                            a[1] = standard_to_mont_scalar(a[1]);
                            return;
                        } else if a.len() == 4 {
                            a[0] = standard_to_mont_scalar(a[0]);
                            a[1] = standard_to_mont_scalar(a[1]);
                            a[2] = standard_to_mont_scalar(a[2]);
                            a[3] = standard_to_mont_scalar(a[3]);
                            return;
                        }
                        debug_assert!(a.len() >= AVX2_U32_LANES);
                        debug_assert_eq!(0, a.len() % AVX2_U32_LANES);
                        let ntt = NTT.get_or_init(|| Ntt::new());
                        ntt.to_mont(a.as_mut_ptr(), a.len());
                    }
                }
                #[target_feature(enable = "avx2")]
                pub unsafe fn mont_to_standard(a: &mut [u32]) {
                    unsafe {
                        debug_assert!(! a.is_empty());
                        debug_assert!(a.len().is_power_of_two());
                        if a.len() == 1 {
                            a[0] = mont_to_standard_scalar(a[0]);
                            return;
                        } else if a.len() == 2 {
                            a[0] = mont_to_standard_scalar(a[0]);
                            a[1] = mont_to_standard_scalar(a[1]);
                            return;
                        } else if a.len() == 4 {
                            a[0] = mont_to_standard_scalar(a[0]);
                            a[1] = mont_to_standard_scalar(a[1]);
                            a[2] = mont_to_standard_scalar(a[2]);
                            a[3] = mont_to_standard_scalar(a[3]);
                            return;
                        }
                        debug_assert!(a.len() >= AVX2_U32_LANES);
                        debug_assert_eq!(0, a.len() % AVX2_U32_LANES);
                        let ntt = NTT.get_or_init(|| Ntt::new());
                        ntt.to_standard(a.as_mut_ptr(), a.len());
                    }
                }
                #[target_feature(enable = "avx2")]
                pub unsafe fn mul_pointwise_mont(a: &mut [u32], b: &[u32]) {
                    unsafe {
                        debug_assert_eq!(a.len(), b.len());
                        debug_assert!(! a.is_empty());
                        debug_assert!(a.len().is_power_of_two());
                        if a.len() == 1 {
                            a[0] = mul_mont(a[0], b[0]);
                            return;
                        } else if a.len() == 2 {
                            a[0] = mul_mont(a[0], b[0]);
                            a[1] = mul_mont(a[1], b[1]);
                            return;
                        } else if a.len() == 4 {
                            a[0] = mul_mont(a[0], b[0]);
                            a[1] = mul_mont(a[1], b[1]);
                            a[2] = mul_mont(a[2], b[2]);
                            a[3] = mul_mont(a[3], b[3]);
                            return;
                        }
                        let ntt = NTT.get_or_init(|| Ntt::new());
                        ntt.mul_pointwise_mont(a.as_mut_ptr(), b.as_ptr(), a.len());
                    }
                }
                #[target_feature(enable = "avx2")]
                pub unsafe fn mul_scalar_mont(a: &mut [u32], sc_mont: u32) {
                    unsafe {
                        let n = a.len();
                        debug_assert!(n.is_power_of_two());
                        debug_assert!(n > 0);
                        if n < AVX2_U32_LANES {
                            if n == 1 {
                                a[0] = mul_mont(a[0], sc_mont);
                                return;
                            } else if n == 2 {
                                a[0] = mul_mont(a[0], sc_mont);
                                a[1] = mul_mont(a[1], sc_mont);
                                return;
                            } else if n == 4 {
                                a[0] = mul_mont(a[0], sc_mont);
                                a[1] = mul_mont(a[1], sc_mont);
                                a[2] = mul_mont(a[2], sc_mont);
                                a[3] = mul_mont(a[3], sc_mont);
                                return;
                            }
                        } else {
                            let ntt = NTT.get_or_init(|| Ntt::new());
                            ntt.mul_scalar_mont(a.as_mut_ptr(), a.len(), sc_mont);
                        }
                    }
                }
                #[target_feature(enable = "avx2")]
                pub unsafe fn ntt_mont(a: &mut [u32]) {
                    unsafe {
                        let n = a.len();
                        debug_assert!(n.is_power_of_two());
                        debug_assert!(n > 0);
                        if n < AVX2_U32_LANES {
                            if n == 1 {
                                return;
                            } else if n == 2 {
                                let t0 = modulo::add(a[0], a[1]);
                                let t1 = modulo::sub(a[0], a[1]);
                                a[0] = t0;
                                a[1] = t1;
                            } else if n == 4 {
                                const OMEGA_1_4_MONT: u32 = 691295370;
                                let e0 = modulo::add(a[0], a[2]);
                                let e1 = modulo::sub(a[0], a[2]);
                                let o0 = modulo::add(a[1], a[3]);
                                let o1 = modulo::sub(a[1], a[3]);
                                let t1 = mul_mont(OMEGA_1_4_MONT, o1);
                                a[0] = modulo::add(e0, o0);
                                a[1] = modulo::sub(e0, o0);
                                a[2] = modulo::add(e1, t1);
                                a[3] = modulo::sub(e1, t1);
                            } else {
                                unreachable!()
                            }
                        } else {
                            let ntt = NTT.get_or_init(|| Ntt::new());
                            ntt.ntt_mont(a.as_mut_ptr(), n);
                        }
                    }
                }
                #[target_feature(enable = "avx2")]
                pub unsafe fn intt_mont(a: &mut [u32]) {
                    unsafe {
                        let n = a.len();
                        debug_assert!(n.is_power_of_two());
                        debug_assert!(n > 0);
                        if n < AVX2_U32_LANES {
                            if n == 1 {
                                return;
                            } else if n == 2 {
                                let t0 = modulo::add(a[0], a[1]);
                                let t1 = modulo::sub(a[0], a[1]);
                                a[0] = t0;
                                a[1] = t1;
                                return;
                            } else if n == 4 {
                                const OMEGA_1_4_INV_MONT: u32 = 998244353 - 691295370;
                                let e0 = modulo::add(a[0], a[1]);
                                let e1 = modulo::sub(a[0], a[1]);
                                let o0 = modulo::add(a[2], a[3]);
                                let o1 = modulo::sub(a[2], a[3]);
                                let t1 = mul_mont(OMEGA_1_4_INV_MONT, o1);
                                a[0] = modulo::add(e0, o0);
                                a[2] = modulo::sub(e0, o0);
                                a[3] = modulo::sub(e1, t1);
                                a[1] = modulo::add(e1, t1);
                                return;
                            } else {
                                unreachable!()
                            }
                        } else {
                            let ntt = NTT.get_or_init(|| Ntt::new());
                            ntt.intt_mont(a.as_mut_ptr(), n);
                        }
                    }
                }
                #[target_feature(enable = "avx2")]
                pub unsafe fn ntt_doubling(a_ntt: &mut Vec<u32>) {
                    unsafe {
                        let n = a_ntt.len();
                        debug_assert!(n.is_power_of_two());
                        debug_assert!(2 * n <= convolution::MAX_NTT_LEN);
                        standard_to_mont(a_ntt);
                        ntt_doubling_mont(a_ntt);
                        mont_to_standard(a_ntt);
                    }
                }
                #[target_feature(enable = "avx2")]
                pub unsafe fn ntt_doubling_mont(a_ntt_mont: &mut Vec<u32>) {
                    unsafe {
                        let n = a_ntt_mont.len();
                        debug_assert!(n.is_power_of_two());
                        debug_assert!(2 * n <= convolution::MAX_NTT_LEN);
                        let mut coeffs = a_ntt_mont.clone();
                        intt_mont(&mut coeffs);
                        let lg = n.trailing_zeros() as usize;
                        let inv_n_mont = inv_len_mont(lg);
                        mul_scalar_mont(&mut coeffs, inv_n_mont);
                        let powers = NTT_DOUBLING_POWERS_MONT
                            .get_or_init(|| NttDoublingPowersMont::new())
                            .powers(n);
                        let mut twisted = coeffs;
                        mul_pointwise_mont(&mut twisted, powers);
                        ntt_mont(&mut twisted);
                        a_ntt_mont.resize(2 * n, 0);
                        a_ntt_mont[n..].copy_from_slice(&twisted);
                    }
                }
            }
            pub mod fps {
                mod add {
                    use super::super::modulo;
                    use std::ops::{Add, AddAssign};
                    impl Add for super::FPS {
                        type Output = super::FPS;
                        fn add(mut self, rhs: Self) -> Self::Output {
                            if self.coeffs.len() < rhs.coeffs.len() {
                                self.coeffs.resize(rhs.coeffs.len(), 0);
                            }
                            for (i, coeff) in rhs.coeffs.into_iter().enumerate() {
                                self.coeffs[i] = modulo::add(self.coeffs[i], coeff);
                            }
                            super::FPS::trim(&mut self.coeffs);
                            self
                        }
                    }
                    impl AddAssign for super::FPS {
                        fn add_assign(&mut self, rhs: Self) {
                            if self.coeffs.len() < rhs.coeffs.len() {
                                self.coeffs.resize(rhs.coeffs.len(), 0);
                            }
                            for (i, coeff) in rhs.coeffs.into_iter().enumerate() {
                                self.coeffs[i] = modulo::add(self.coeffs[i], coeff);
                            }
                            super::FPS::trim(&mut self.coeffs);
                        }
                    }
                }
                pub mod bostan_mori {
                    use super::super::modulo;
                    pub fn bostan_mori(p: &super::FPS, q: &super::FPS, k: usize) -> u32 {
                        #[cfg(target_arch = "x86_64")]
                        {
                            if std::is_x86_feature_detected!("avx2") {
                                return bostan_mori_avx2(p, q, k);
                            }
                        }
                        bostan_mori_scalar(p, q, k)
                    }
                    fn bostan_mori_scalar(
                        p: &super::FPS,
                        q: &super::FPS,
                        mut k: usize,
                    ) -> u32 {
                        let mut p = p.clone();
                        let mut q = q.clone();
                        while k > 0 {
                            let q_neg_x = {
                                let mut res = q.clone();
                                res.coeffs
                                    .iter_mut()
                                    .skip(1)
                                    .step_by(2)
                                    .for_each(|x| *x = modulo::neg(*x));
                                res
                            };
                            p *= q_neg_x.clone();
                            q *= q_neg_x;
                            p.coeffs = p
                                .coeffs
                                .into_iter()
                                .skip((k % 2) as usize)
                                .step_by(2)
                                .collect();
                            q.coeffs = q.coeffs.into_iter().step_by(2).collect();
                            super::FPS::trim(&mut p.coeffs);
                            super::FPS::trim(&mut q.coeffs);
                            k /= 2;
                            if p.len() > k as usize && q.len() > k as usize {
                                return (p * q.inverse(k).unwrap()).get(k);
                            }
                        }
                        modulo::mul(p.get(0), modulo::inv(q.get(0)))
                    }
                    #[cfg(target_arch = "x86_64")]
                    fn bostan_mori_avx2(
                        p: &super::FPS,
                        q: &super::FPS,
                        k: usize,
                    ) -> u32 {
                        use super::super::convolution;
                        use super::super::convolution_mont;
                        debug_assert!(std::is_x86_feature_detected!("avx2"));
                        if q.get(0) == 0 {
                            return 0;
                        }
                        let n = q.len().next_power_of_two().max(1);
                        if 2 * n > convolution::MAX_NTT_LEN {
                            return bostan_mori_scalar(p, q, k);
                        }
                        let mut k = k as usize;
                        if k < n {
                            let mut p = p.coeffs.clone();
                            let mut q = q.coeffs.clone();
                            p.resize(k + 1, 0);
                            q.resize(k + 1, 0);
                            return (super::FPS::new(p)
                                * super::FPS::new(q).inverse(k).unwrap())
                                .get(k);
                        }
                        let lg = n.trailing_zeros() as usize;
                        let inv2 = convolution_mont::standard_to_mont_scalar(
                            modulo::inv(2),
                        );
                        let inv_n = convolution_mont::inv_len_mont(lg);
                        let w = build_w_for_pairing(n);
                        let mut p = p.coeffs.clone();
                        let mut q = q.coeffs.clone();
                        p.resize(2 * n, 0);
                        q.resize(2 * n, 0);
                        unsafe {
                            convolution_mont::standard_to_mont(&mut p);
                            convolution_mont::standard_to_mont(&mut q);
                            convolution_mont::ntt_mont(&mut p);
                            convolution_mont::ntt_mont(&mut q);
                        }
                        while k >= n {
                            debug_assert_eq!(2 * n, p.len());
                            debug_assert_eq!(2 * n, q.len());
                            if k % 2 == 0 {
                                for i in 0..n {
                                    let p0 = p[2 * i];
                                    let p1 = p[2 * i + 1];
                                    let q0 = q[2 * i];
                                    let q1 = q[2 * i + 1];
                                    let t = modulo::add(
                                        convolution_mont::mul_mont(p0, q1),
                                        convolution_mont::mul_mont(p1, q0),
                                    );
                                    p[i] = convolution_mont::mul_mont(t, inv2);
                                    q[i] = convolution_mont::mul_mont(q0, q1);
                                }
                            } else {
                                for i in 0..n {
                                    let p0 = p[2 * i];
                                    let p1 = p[2 * i + 1];
                                    let q0 = q[2 * i];
                                    let q1 = q[2 * i + 1];
                                    let t = modulo::sub(
                                        convolution_mont::mul_mont(p0, q1),
                                        convolution_mont::mul_mont(p1, q0),
                                    );
                                    p[i] = convolution_mont::mul_mont(t, w[i]);
                                    q[i] = convolution_mont::mul_mont(q0, q1);
                                }
                            }
                            p.truncate(n);
                            q.truncate(n);
                            k /= 2;
                            if k < n {
                                break;
                            }
                            unsafe {
                                convolution_mont::ntt_doubling_mont(&mut p);
                                convolution_mont::ntt_doubling_mont(&mut q);
                            }
                        }
                        unsafe {
                            convolution_mont::intt_mont(&mut p);
                            convolution_mont::intt_mont(&mut q);
                            convolution_mont::mul_scalar_mont(&mut p, inv_n);
                            convolution_mont::mul_scalar_mont(&mut q, inv_n);
                            convolution_mont::mont_to_standard(&mut p);
                            convolution_mont::mont_to_standard(&mut q);
                        }
                        p.truncate(k + 1);
                        q.truncate(k + 1);
                        (super::FPS::new(p) * super::FPS::new(q).inverse(k).unwrap())
                            .get(k)
                    }
                    #[cfg(target_arch = "x86_64")]
                    fn build_w_for_pairing(n: usize) -> Vec<u32> {
                        use super::super::convolution;
                        use super::super::convolution_mont;
                        debug_assert!(std::is_x86_feature_detected!("avx2"));
                        debug_assert!(n.is_power_of_two());
                        debug_assert!(n > 0);
                        debug_assert!(2 * n <= convolution::MAX_NTT_LEN);
                        let lg = n.trailing_zeros() as usize;
                        let omega = modulo::pow(
                            convolution::PRIMITIVE_ROOT,
                            (modulo::M as usize - 1) / (2 * n),
                        );
                        let omega_inv = modulo::inv(omega);
                        let inv2 = modulo::inv(2);
                        let mut res = vec![0_u32; n];
                        let mut x_inv = 1_u32;
                        for exp in 0..n {
                            let i = convolution::bit_reverse(exp, lg);
                            res[i] = modulo::mul(inv2, x_inv);
                            x_inv = modulo::mul(x_inv, omega_inv);
                        }
                        unsafe {
                            convolution_mont::standard_to_mont(&mut res);
                        }
                        res
                    }
                    pub fn linear_recurrence_kth_term(
                        initial_terms: &[u32],
                        coefficients: &[u32],
                        k: usize,
                    ) -> u32 {
                        assert!(! initial_terms.is_empty());
                        assert_eq!(initial_terms.len(), coefficients.len());
                        let degree = initial_terms.len();
                        if k < degree {
                            return initial_terms[k];
                        }
                        let mut q_coeffs = Vec::with_capacity(degree + 1);
                        q_coeffs.push(1);
                        for &c in coefficients {
                            q_coeffs.push(modulo::neg(c));
                        }
                        let a = super::FPS::new(initial_terms.to_vec());
                        let q = super::FPS::new(q_coeffs);
                        let mut p = a * q.clone();
                        p.truncate(degree);
                        bostan_mori(&p, &q, k)
                    }
                }
                pub mod inv {
                    use super::super::convolution;
                    use super::super::modulo;
                    impl super::FPS {
                        pub fn inverse(&self, degree: usize) -> Option<Self> {
                            if self.should_use_sparse_inverse(degree) {
                                self.inverse_sparse(degree)
                            } else {
                                self.inverse_dense(degree)
                            }
                        }
                        fn should_use_sparse_inverse(&self, degree: usize) -> bool {
                            let len = degree + 1;
                            let non_zero_count = self
                                .non_zero_terms_iter()
                                .take_while(|(i, _)| *i <= degree)
                                .filter(|(i, _)| *i != 0)
                                .count();
                            let t = len.next_power_of_two();
                            let sparse_cost = (len as f32) * (non_zero_count as f32);
                            let dense_cost = (t as f32) * (t as f32).log2();
                            sparse_cost < dense_cost
                        }
                        pub fn inverse_dense(&self, degree: usize) -> Option<Self> {
                            #[cfg(target_arch = "x86_64")]
                            {
                                if std::is_x86_feature_detected!("avx2") {
                                    return self.inverse_dense_avx2(degree);
                                }
                            }
                            self.inverse_dense_scalar(degree)
                        }
                        #[cfg(target_arch = "x86_64")]
                        fn inverse_dense_avx2(&self, degree: usize) -> Option<Self> {
                            use super::super::convolution_mont;
                            debug_assert!(std::is_x86_feature_detected!("avx2"));
                            let mut poly = self.coeffs.clone();
                            let len = degree + 1;
                            let constant = *poly.get(0).unwrap_or(&0);
                            if constant == 0 {
                                return None;
                            }
                            if len == 1 {
                                return Some(Self {
                                    coeffs: vec![modulo::inv(constant)],
                                });
                            }
                            let mut inverse_coeffs = vec![
                                convolution_mont::standard_to_mont_scalar(modulo::inv(constant,))
                            ];
                            poly.resize(poly.len().next_power_of_two(), 0);
                            unsafe {
                                convolution_mont::standard_to_mont(&mut poly);
                            }
                            let mut current_len = 1;
                            let mut f_vals = Vec::with_capacity(2 * len);
                            let mut g_vals = Vec::with_capacity(2 * len);
                            let mut h_vals = Vec::with_capacity(2 * len);
                            while current_len < len {
                                let next_len = 2 * current_len;
                                f_vals.clear();
                                f_vals.extend(poly.iter().cloned().take(next_len));
                                f_vals.resize(next_len, 0);
                                g_vals.clear();
                                g_vals.extend(inverse_coeffs.iter().cloned());
                                g_vals.resize(next_len, 0);
                                unsafe {
                                    convolution_mont::ntt_mont(&mut f_vals);
                                    convolution_mont::ntt_mont(&mut g_vals);
                                }
                                let inv_ntt_len = convolution_mont::standard_to_mont_scalar(
                                    modulo::inv(next_len as u32),
                                );
                                unsafe {
                                    convolution_mont::mul_pointwise_mont(
                                        &mut f_vals,
                                        &mut g_vals,
                                    );
                                    convolution_mont::intt_mont(&mut f_vals);
                                    convolution_mont::mul_scalar_mont(&mut f_vals, inv_ntt_len);
                                }
                                h_vals.clear();
                                h_vals.resize(next_len, 0);
                                for i in 0..current_len {
                                    h_vals[i] = f_vals[current_len + i];
                                }
                                unsafe {
                                    convolution_mont::ntt_mont(&mut h_vals);
                                    convolution_mont::mul_pointwise_mont(
                                        &mut h_vals,
                                        &mut g_vals,
                                    );
                                    convolution_mont::intt_mont(&mut h_vals);
                                    convolution_mont::mul_scalar_mont(&mut h_vals, inv_ntt_len);
                                }
                                let mut updated = Vec::with_capacity(next_len);
                                updated.extend(inverse_coeffs.iter().cloned());
                                updated.resize(next_len, 0);
                                for i in 0..current_len {
                                    updated[current_len + i] = modulo::neg(h_vals[i]);
                                }
                                inverse_coeffs = updated;
                                current_len = next_len;
                            }
                            unsafe {
                                convolution_mont::mont_to_standard(&mut inverse_coeffs);
                            }
                            inverse_coeffs.truncate(len);
                            while inverse_coeffs.last().map_or(false, |c| *c == 0) {
                                inverse_coeffs.pop();
                            }
                            Some(Self { coeffs: inverse_coeffs })
                        }
                        fn inverse_dense_scalar(&self, degree: usize) -> Option<Self> {
                            let len = degree + 1;
                            let constant = self.get(0);
                            if constant == 0 {
                                return None;
                            }
                            if len == 1 {
                                return Some(Self {
                                    coeffs: vec![modulo::inv(constant)],
                                });
                            }
                            let mut inverse_coeffs = vec![modulo::inv(constant)];
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
                                convolution::ntt(&mut f_vals);
                                convolution::ntt(&mut g_vals);
                                let inv_ntt_len = modulo::inv(ntt_len as u32);
                                for (value, g_value) in f_vals.iter_mut().zip(g_vals.iter())
                                {
                                    *value = modulo::mul(*value, *g_value);
                                }
                                convolution::intt(&mut f_vals);
                                f_vals
                                    .iter_mut()
                                    .for_each(|value| {
                                        *value = modulo::mul(*value, inv_ntt_len);
                                    });
                                h_vals.clear();
                                h_vals.resize(ntt_len, 0);
                                for i in 0..current_len {
                                    if current_len + i < ntt_len {
                                        h_vals[i] = f_vals[current_len + i];
                                    }
                                }
                                convolution::ntt(&mut h_vals);
                                for (value, g_value) in h_vals.iter_mut().zip(g_vals.iter())
                                {
                                    *value = modulo::mul(*value, *g_value);
                                }
                                convolution::intt(&mut h_vals);
                                h_vals
                                    .iter_mut()
                                    .for_each(|value| {
                                        *value = modulo::mul(*value, inv_ntt_len);
                                    });
                                let mut updated = Vec::with_capacity(next_len);
                                updated
                                    .extend(inverse_coeffs.iter().cloned().take(current_len));
                                updated.resize(next_len, 0);
                                for i in 0..(next_len - current_len) {
                                    updated[current_len + i] = modulo::neg(
                                        h_vals.get(i).copied().unwrap_or(0),
                                    );
                                }
                                inverse_coeffs = updated;
                                current_len = next_len;
                            }
                            inverse_coeffs.truncate(len);
                            Self::trim(&mut inverse_coeffs);
                            Some(Self { coeffs: inverse_coeffs })
                        }
                        pub fn inverse_sparse(&self, degree: usize) -> Option<Self> {
                            let target_len = degree + 1;
                            let constant = self.get(0);
                            if constant == 0 {
                                return None;
                            }
                            let inv_const = modulo::inv(constant);
                            let sparse_terms = self
                                .non_zero_terms_iter()
                                .skip(1)
                                .take_while(|(i, _)| *i <= degree)
                                .collect::<Vec<(usize, u32)>>();
                            let mut res = vec![0; target_len];
                            res[0] = inv_const;
                            for n in 1..target_len {
                                let mut acc = 0_u32;
                                for &(i, c) in sparse_terms
                                    .iter()
                                    .take_while(|&&(i, _)| i <= n)
                                {
                                    acc = modulo::sub(acc, modulo::mul(c, res[n - i]));
                                }
                                res[n] = modulo::mul(acc, inv_const);
                            }
                            Self::trim(&mut res);
                            Some(Self { coeffs: res })
                        }
                    }
                }
                mod mul {
                    use super::super::convolution;
                    use std::ops::{Mul, MulAssign};
                    impl super::FPS {
                        pub fn mul_xk(&self, k: usize) -> Self {
                            if self.is_zero() {
                                return Self { coeffs: Vec::new() };
                            }
                            let mut coeffs = vec![0; k + self.len()];
                            coeffs[k..].copy_from_slice(&self.coeffs);
                            Self { coeffs }
                        }
                    }
                    impl Mul for super::FPS {
                        type Output = super::FPS;
                        fn mul(self, rhs: Self) -> Self::Output {
                            let mut coeffs = convolution::convolution(
                                self.coeffs,
                                rhs.coeffs,
                            );
                            super::FPS::trim(&mut coeffs);
                            Self { coeffs }
                        }
                    }
                    impl MulAssign for super::FPS {
                        fn mul_assign(&mut self, rhs: Self) {
                            let a = std::mem::take(&mut self.coeffs);
                            let mut coeffs = convolution::convolution(a, rhs.coeffs);
                            super::FPS::trim(&mut coeffs);
                            self.coeffs = coeffs;
                        }
                    }
                }
                mod sub {
                    use super::super::modulo;
                    use std::ops::{Neg, Sub, SubAssign};
                    impl Sub for super::FPS {
                        type Output = super::FPS;
                        fn sub(mut self, rhs: Self) -> Self::Output {
                            if self.coeffs.len() < rhs.coeffs.len() {
                                self.coeffs.resize(rhs.coeffs.len(), 0);
                            }
                            for (i, coeff) in rhs.coeffs.into_iter().enumerate() {
                                self.coeffs[i] = modulo::sub(self.coeffs[i], coeff);
                            }
                            super::FPS::trim(&mut self.coeffs);
                            self
                        }
                    }
                    impl Neg for super::FPS {
                        type Output = super::FPS;
                        fn neg(mut self) -> Self::Output {
                            self.coeffs.iter_mut().for_each(|c| *c = modulo::neg(*c));
                            self
                        }
                    }
                    impl SubAssign for super::FPS {
                        fn sub_assign(&mut self, rhs: Self) {
                            if self.coeffs.len() < rhs.coeffs.len() {
                                self.coeffs.resize(rhs.coeffs.len(), 0);
                            }
                            for (i, coeff) in rhs.coeffs.into_iter().enumerate() {
                                self.coeffs[i] = modulo::sub(self.coeffs[i], coeff);
                            }
                            super::FPS::trim(&mut self.coeffs);
                        }
                    }
                }
                use super::modulo;
                use std::fmt;
                #[derive(Clone, Debug, PartialEq, Eq)]
                pub struct FPS {
                    coeffs: Vec<u32>,
                }
                impl FPS {
                    pub fn new(mut coefficients: Vec<u32>) -> Self {
                        coefficients.iter_mut().for_each(|c| assert!(* c < modulo::M));
                        Self::trim(&mut coefficients);
                        FPS { coeffs: coefficients }
                    }
                    pub fn len(&self) -> usize {
                        self.coeffs.len()
                    }
                    pub fn degree(&self) -> Option<usize> {
                        if self.is_zero() { None } else { Some(self.len() - 1) }
                    }
                    pub fn is_zero(&self) -> bool {
                        self.coeffs.is_empty()
                    }
                    pub fn get(&self, index: usize) -> u32 {
                        *self.coeffs.get(index).unwrap_or(&0)
                    }
                    pub fn set(&mut self, index: usize, value: u32) {
                        assert!(value < modulo::M);
                        if value == 0 && index >= self.len() {
                            return;
                        }
                        if self.len() <= index {
                            self.coeffs.resize(index + 1, 0);
                        }
                        self.coeffs[index] = value;
                        if value == 0 {
                            Self::trim(&mut self.coeffs);
                        }
                    }
                    pub fn coefficients(&self) -> &[u32] {
                        &self.coeffs
                    }
                    pub fn non_zero_terms_iter(
                        &self,
                    ) -> impl Iterator<Item = (usize, u32)> + '_ {
                        self.coeffs
                            .iter()
                            .enumerate()
                            .filter_map(|(i, &c)| {
                                if c == 0 { None } else { Some((i, c)) }
                            })
                    }
                    pub fn non_zero_terms(&self) -> Vec<(usize, u32)> {
                        self.non_zero_terms_iter().collect()
                    }
                    pub fn truncate(&mut self, len: usize) {
                        self.coeffs.truncate(len);
                        Self::trim(&mut self.coeffs);
                    }
                    pub fn derivative(&mut self) -> &mut Self {
                        let len = self.len();
                        if len <= 1 {
                            self.coeffs.clear();
                            return self;
                        }
                        for i in 0..(len - 1) {
                            self.coeffs[i] = modulo::mul(
                                (i + 1) as u32,
                                self.coeffs[i + 1],
                            );
                        }
                        self.coeffs.truncate(len - 1);
                        Self::trim(&mut self.coeffs);
                        self
                    }
                    pub fn integral(&mut self) -> &mut Self {
                        assert!(
                            self.len() + 1 < modulo::M as usize,
                            "Integral requires degree + 1 < modulus"
                        );
                        if self.is_zero() {
                            return self;
                        }
                        let len = self.len();
                        let mut fact = Vec::with_capacity(len + 1);
                        fact.push(1);
                        for i in 1..=len {
                            fact.push(modulo::mul(fact[i - 1], i as u32));
                        }
                        let mut ifact = Vec::with_capacity(len + 1);
                        ifact.push(modulo::inv(fact[len]));
                        for i in 0..len {
                            ifact.push(modulo::mul(ifact[i], (len - i) as u32));
                        }
                        ifact.reverse();
                        self.coeffs.push(0);
                        for i in (0..len).rev() {
                            let scaled = modulo::mul(self.coeffs[i], ifact[i + 1]);
                            let integrated = modulo::mul(scaled, fact[i]);
                            self.coeffs[i + 1] = integrated;
                        }
                        self.coeffs[0] = 0;
                        Self::trim(&mut self.coeffs);
                        self
                    }
                    fn trim(coeffs: &mut Vec<u32>) {
                        while coeffs.last().map_or(false, |c| *c == 0) {
                            coeffs.pop();
                        }
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
                            .filter(|(_, c)| **c != 0)
                            .map(|(i, c)| format!("{}x^{}", c, i))
                            .collect::<Vec<String>>();
                        write!(f, "{}", terms.join(" + "))
                    }
                }
            }
            pub mod modulo {
                pub const M: u32 = 998244353;
                pub const fn modulo(a: u64) -> u32 {
                    (a % M as u64) as u32
                }
                pub const fn add(a: u32, b: u32) -> u32 {
                    debug_assert!(a < M);
                    debug_assert!(b < M);
                    let t = a + b;
                    if t < M { t } else { t.wrapping_sub(M) }
                }
                pub const fn sub(a: u32, b: u32) -> u32 {
                    debug_assert!(a < M);
                    debug_assert!(b < M);
                    let (t, f) = a.overflowing_sub(b);
                    if !f { t } else { t.wrapping_add(M) }
                }
                pub const fn mul(a: u32, b: u32) -> u32 {
                    debug_assert!(a < M);
                    debug_assert!(b < M);
                    modulo(a as u64 * b as u64)
                }
                pub const fn neg(a: u32) -> u32 {
                    debug_assert!(a < M);
                    if a == 0 { 0 } else { M - a }
                }
                pub fn build_inv_indices(len: usize) -> Vec<u32> {
                    assert!(
                        len < M as usize, "build_inv_indices requires len < modulus"
                    );
                    if len == 0 {
                        return Vec::new();
                    }
                    let mut inv = vec![0_u32; len];
                    if len > 1 {
                        inv[1] = 1;
                        for i in 2..len {
                            let iu = i as u32;
                            let q = M / iu;
                            let r = (M % iu) as usize;
                            let t = mul(q, inv[r]);
                            let val = M - t;
                            inv[i] = if val == M { 0 } else { val };
                        }
                    }
                    inv
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
        use std::{ffi, io};
        #[cfg(target_os = "linux")]
        use std::os::unix;
        const OUT_BUF_SIZE: usize = 1 << 18;
        const OUT_BUF_FLUSH_THRESHOLD: usize = 32;
        const OUT_FLUSH_LIMIT: usize = OUT_BUF_SIZE - OUT_BUF_FLUSH_THRESHOLD;
        static mut OUT_BUF: [u8; OUT_BUF_SIZE] = [0_u8; OUT_BUF_SIZE];
        const DIGIT4_TABLE: [u32; 10_000] = {
            let mut table = [b'0' as u32; 10_000];
            let mut i = 0;
            while i < 10_000 {
                table[i] |= (((i / 1000) % 10) as u32 + b'0' as u32) << 0;
                table[i] |= (((i / 100) % 10) as u32 + b'0' as u32) << 8;
                table[i] |= (((i / 10) % 10) as u32 + b'0' as u32) << 16;
                table[i] |= (((i / 1) % 10) as u32 + b'0' as u32) << 24;
                i += 1;
            }
            table
        };
        #[cfg(target_os = "linux")]
        #[link(name = "c")]
        extern "C" {
            fn mmap(
                addr: *mut ffi::c_void,
                length: usize,
                prot: i32,
                flags: i32,
                fd: i32,
                offset: isize,
            ) -> *mut ffi::c_void;
            fn write(fd: i32, buf: *const ffi::c_void, count: usize) -> isize;
        }
        fn is_8digits(mut bytes: u64) -> bool {
            bytes ^= 0x3030303030303030;
            bytes &= 0xf0f0f0f0f0f0f0f0;
            return bytes == 0;
        }
        fn parse_8digits(bytes: u64) -> u32 {
            debug_assert!(is_8digits(bytes));
            let v1 = (bytes & 0x0f0f0f0f0f0f0f0f).wrapping_mul(0xA01) >> 8;
            let v2 = (v1 & 0x00ff00ff00ff00ff).wrapping_mul(0x640001) >> 16;
            let v3 = (v2 & 0x0000ffff0000ffff).wrapping_mul(0x271000000001) >> 32;
            v3 as u32
        }
        pub trait FastWrite {
            fn write_to(self, io: &mut Fastio);
            fn writeln_to(self, io: &mut Fastio);
        }
        pub struct Fastio {
            in_cursor: *const u8,
            out_buf: &'static mut [u8],
            out_pos: usize,
            _input_storage: Vec<u8>,
        }
        impl Fastio {
            pub fn new() -> Self {
                #[cfg(target_os = "linux")]
                unsafe {
                    const PROT_READ: i32 = 0x1;
                    const MAP_PRIVATE: i32 = 0x02;
                    const MAP_FAILED: *mut ffi::c_void = (-1isize) as *mut ffi::c_void;
                    'mmap_try: {
                        let Ok(file) = std::fs::File::open("/dev/stdin") else {
                            break 'mmap_try;
                        };
                        let Ok(metadata) = file.metadata() else {
                            break 'mmap_try;
                        };
                        if !metadata.is_file() {
                            break 'mmap_try;
                        }
                        let len = metadata.len() as usize;
                        let fd = unix::io::AsRawFd::as_raw_fd(&file);
                        let addr = std::ptr::null_mut();
                        let ptr = mmap(addr, len, PROT_READ, MAP_PRIVATE, fd, 0);
                        if ptr == MAP_FAILED {
                            break 'mmap_try;
                        }
                        let in_cursor = ptr as *const u8;
                        assert!(
                            OUT_BUF_SIZE > OUT_BUF_FLUSH_THRESHOLD,
                            "buffer too small for flush threshold"
                        );
                        let out_buf = &mut OUT_BUF[..];
                        return Self {
                            in_cursor,
                            out_buf,
                            out_pos: 0,
                            _input_storage: vec![],
                        };
                    }
                }
                let mut input_storage = vec![];
                io::Read::read_to_end(&mut io::stdin().lock(), &mut input_storage)
                    .expect("failed to read from source");
                input_storage.extend_from_slice(&[0xff; 8]);
                let in_cursor = input_storage.as_ptr();
                let out_buf = unsafe { &mut OUT_BUF[..] };
                Self {
                    in_cursor,
                    out_buf,
                    out_pos: 0,
                    _input_storage: input_storage,
                }
            }
            pub fn char(&mut self) -> char {
                unsafe {
                    self.skip_whitespace();
                    let ch = *self.in_cursor as char;
                    self.in_cursor = self.in_cursor.add(1);
                    ch
                }
            }
            pub fn i32(&mut self) -> i32 {
                unsafe {
                    self.skip_whitespace();
                    let negative = *self.in_cursor == b'-';
                    if negative {
                        self.in_cursor = self.in_cursor.add(1);
                    }
                    let mut value = 0_i32;
                    loop {
                        let bytes = std::ptr::read_unaligned(
                            self.in_cursor as *const u64,
                        );
                        if !is_8digits(bytes) {
                            break;
                        }
                        let parsed = parse_8digits(bytes) as i32;
                        if negative {
                            value = value * 100_000_000 - parsed;
                        } else {
                            value = value * 100_000_000 + parsed;
                        }
                        self.in_cursor = self.in_cursor.add(8);
                    }
                    while *self.in_cursor >= b'0' {
                        let digit = (*self.in_cursor - b'0') as i32;
                        if negative {
                            value = 10 * value - digit;
                        } else {
                            value = 10 * value + digit;
                        }
                        self.in_cursor = self.in_cursor.add(1);
                    }
                    value
                }
            }
            pub fn u64(&mut self) -> u64 {
                unsafe {
                    self.skip_whitespace();
                    let mut value = 0_u64;
                    loop {
                        let bytes = std::ptr::read_unaligned(
                            self.in_cursor as *const u64,
                        );
                        if !is_8digits(bytes) {
                            break;
                        }
                        value = value * (100_000_000 as u64)
                            + parse_8digits(bytes) as u64;
                        self.in_cursor = self.in_cursor.add(8);
                    }
                    while *self.in_cursor >= b'0' {
                        value = 10 * value + (*self.in_cursor - b'0') as u64;
                        self.in_cursor = self.in_cursor.add(1);
                    }
                    value
                }
            }
            pub fn u32(&mut self) -> u32 {
                unsafe {
                    self.skip_whitespace();
                    let mut value = 0_u32;
                    loop {
                        let bytes = std::ptr::read_unaligned(
                            self.in_cursor as *const u64,
                        );
                        if !is_8digits(bytes) {
                            break;
                        }
                        value = value * 100_000_000 + parse_8digits(bytes);
                        self.in_cursor = self.in_cursor.add(8);
                    }
                    while *self.in_cursor >= b'0' {
                        value = 10 * value + (*self.in_cursor - b'0') as u32;
                        self.in_cursor = self.in_cursor.add(1);
                    }
                    value
                }
            }
            pub fn i64(&mut self) -> i64 {
                unsafe {
                    self.skip_whitespace();
                    let negative = *self.in_cursor == b'-';
                    if negative {
                        self.in_cursor = self.in_cursor.add(1);
                    }
                    let mut value = 0_i64;
                    loop {
                        let bytes = std::ptr::read_unaligned(
                            self.in_cursor as *const u64,
                        );
                        if !is_8digits(bytes) {
                            break;
                        }
                        let parsed = parse_8digits(bytes) as i64;
                        if negative {
                            value = value * 100_000_000 - parsed;
                        } else {
                            value = value * 100_000_000 + parsed;
                        }
                        self.in_cursor = self.in_cursor.add(8);
                    }
                    while *self.in_cursor >= b'0' {
                        let digit = (*self.in_cursor - b'0') as i64;
                        if negative {
                            value = 10 * value - digit;
                        } else {
                            value = 10 * value + digit;
                        }
                        self.in_cursor = self.in_cursor.add(1);
                    }
                    value
                }
            }
            pub fn isize1(&mut self) -> isize {
                unsafe {
                    self.skip_whitespace();
                    let mut value = 0_isize;
                    loop {
                        let bytes = std::ptr::read_unaligned(
                            self.in_cursor as *const u64,
                        );
                        if !is_8digits(bytes) {
                            break;
                        }
                        value = value * 100_000_000 + parse_8digits(bytes) as isize;
                        self.in_cursor = self.in_cursor.add(8);
                    }
                    while *self.in_cursor >= b'0' {
                        value = 10 * value + (*self.in_cursor - b'0') as isize;
                        self.in_cursor = self.in_cursor.add(1);
                    }
                    value - 1
                }
            }
            pub fn usize1(&mut self) -> usize {
                unsafe {
                    self.skip_whitespace();
                    let mut value = 0_usize;
                    loop {
                        let bytes = std::ptr::read_unaligned(
                            self.in_cursor as *const u64,
                        );
                        if !is_8digits(bytes) {
                            break;
                        }
                        value = value * 100_000_000 + parse_8digits(bytes) as usize;
                        self.in_cursor = self.in_cursor.add(8);
                    }
                    while *self.in_cursor >= b'0' {
                        value = 10 * value + (*self.in_cursor - b'0') as usize;
                        self.in_cursor = self.in_cursor.add(1);
                    }
                    value - 1
                }
            }
            pub fn chars(&mut self) -> Vec<char> {
                unsafe {
                    self.skip_whitespace();
                    let mut res = vec![];
                    while *self.in_cursor > b' ' {
                        res.push(*self.in_cursor as char);
                        self.in_cursor = self.in_cursor.add(1);
                    }
                    res
                }
            }
            #[inline(always)]
            fn out_flush(&mut self) {
                if self.out_pos == 0 {
                    return;
                }
                #[cfg(target_os = "linux")]
                unsafe {
                    let mut written = 0_usize;
                    let base = self.out_buf.as_ptr();
                    while written < self.out_pos {
                        let n = write(
                            1,
                            base.add(written) as *const ffi::c_void,
                            self.out_pos - written,
                        );
                        if n < 0 {
                            panic!("failed to write buffered output");
                        }
                        written += n as usize;
                    }
                }
                #[cfg(not(target_os = "linux"))]
                {
                    let mut stdout = io::stdout().lock();
                    io::Write::write_all(&mut stdout, &self.out_buf[..self.out_pos])
                        .and_then(|_| io::Write::flush(&mut stdout))
                        .expect("failed to write buffered output");
                }
                self.out_pos = 0;
            }
            #[inline(always)]
            fn out_maybe_flush(&mut self) {
                if self.out_pos > OUT_FLUSH_LIMIT {
                    self.out_flush();
                }
            }
            #[inline(always)]
            fn out_put_4digits(&mut self, x: u16) {
                unsafe {
                    let dst = self.out_buf.as_mut_ptr().add(self.out_pos);
                    std::ptr::write_unaligned(dst as *mut u32, DIGIT4_TABLE[x as usize]);
                }
                self.out_pos += 4;
            }
            #[inline(always)]
            fn out_put_upto_4digits(&mut self, x: u16) {
                unsafe {
                    let dst = self.out_buf.as_mut_ptr().add(self.out_pos);
                    if x <= 9 {
                        std::ptr::write_unaligned(
                            dst as *mut u32,
                            DIGIT4_TABLE[(x * 1000) as usize],
                        );
                        self.out_pos += 1;
                    } else if x <= 99 {
                        std::ptr::write_unaligned(
                            dst as *mut u32,
                            DIGIT4_TABLE[(x * 100) as usize],
                        );
                        self.out_pos += 2;
                    } else if x <= 999 {
                        std::ptr::write_unaligned(
                            dst as *mut u32,
                            DIGIT4_TABLE[(x * 10) as usize],
                        );
                        self.out_pos += 3;
                    } else {
                        std::ptr::write_unaligned(
                            dst as *mut u32,
                            DIGIT4_TABLE[x as usize],
                        );
                        self.out_pos += 4;
                    }
                }
            }
            pub fn write<T>(&mut self, value: T)
            where
                T: FastWrite,
            {
                value.write_to(self);
            }
            pub fn writeln<T>(&mut self, value: T)
            where
                T: FastWrite,
            {
                value.writeln_to(self);
            }
            pub fn flush(&mut self) {
                self.out_flush();
            }
            #[inline(always)]
            unsafe fn skip_whitespace(&mut self) {
                unsafe {
                    while *self.in_cursor <= b' ' {
                        self.in_cursor = self.in_cursor.add(1);
                    }
                }
            }
        }
        impl FastWrite for char {
            fn write_to(self, io: &mut Fastio) {
                io.out_buf[io.out_pos] = self as u8;
                io.out_pos += 1;
                io.out_maybe_flush();
            }
            fn writeln_to(self, io: &mut Fastio) {
                io.out_buf[io.out_pos] = self as u8;
                io.out_pos += 1;
                io.out_buf[io.out_pos] = b'\n';
                io.out_pos += 1;
                io.out_maybe_flush();
            }
        }
        impl FastWrite for u64 {
            #[inline(always)]
            fn write_to(self, io: &mut Fastio) {
                if self >= 10_000_000_000_000_000 {
                    io.out_put_upto_4digits((self / 10_000_000_000_000_000) as u16);
                    io.out_put_4digits(
                        ((self / 1_000_000_000_000) as u32 % 10_000) as u16,
                    );
                    io.out_put_4digits(((self / 100_000_000) % 10_000) as u16);
                    io.out_put_4digits(((self / 10_000) % 10_000) as u16);
                    io.out_put_4digits((self % 10_000) as u16);
                } else if self >= 1_000_000_000_000 {
                    io.out_put_upto_4digits(
                        ((self / 1_000_000_000_000) as u32 % 10_000) as u16,
                    );
                    io.out_put_4digits(((self / 100_000_000) % 10_000) as u16);
                    io.out_put_4digits(((self / 10_000) % 10_000) as u16);
                    io.out_put_4digits((self % 10_000) as u16);
                } else if self >= 100_000_000 {
                    io.out_put_upto_4digits(((self / 100_000_000) % 10_000) as u16);
                    io.out_put_4digits(((self / 10_000) % 10_000) as u16);
                    io.out_put_4digits((self % 10_000) as u16);
                } else if self >= 10_000 {
                    io.out_put_upto_4digits(((self / 10_000) % 10_000) as u16);
                    io.out_put_4digits((self % 10_000) as u16);
                } else {
                    io.out_put_upto_4digits((self % 10_000) as u16);
                }
                io.out_buf[io.out_pos] = b'\n';
                io.out_pos += 1;
                io.out_maybe_flush();
            }
            #[inline(always)]
            fn writeln_to(self, io: &mut Fastio) {
                self.write_to(io);
            }
        }
        impl FastWrite for u32 {
            #[inline(always)]
            fn write_to(self, io: &mut Fastio) {
                if self >= 100_000_000 {
                    io.out_put_upto_4digits((self / 100_000_000) as u16);
                    io.out_put_4digits(((self / 10_000) % 10_000) as u16);
                    io.out_put_4digits((self % 10_000) as u16);
                } else if self >= 10_000 {
                    io.out_put_upto_4digits((self / 10_000) as u16);
                    io.out_put_4digits((self % 10_000) as u16);
                } else {
                    io.out_put_upto_4digits(self as u16);
                }
                io.out_buf[io.out_pos] = b'\n';
                io.out_pos += 1;
                io.out_maybe_flush();
            }
            #[inline(always)]
            fn writeln_to(self, io: &mut Fastio) {
                self.write_to(io);
            }
        }
        impl FastWrite for i64 {
            #[inline(always)]
            fn write_to(self, io: &mut Fastio) {
                if self < 0 {
                    if self == i64::MIN {
                        io.out_buf[io.out_pos..io.out_pos + 20]
                            .copy_from_slice(b"-9223372036854775808");
                        io.out_pos += 20;
                        io.out_buf[io.out_pos] = b'\n';
                        io.out_pos += 1;
                        io.out_maybe_flush();
                        return;
                    }
                    io.out_buf[io.out_pos] = b'-';
                    io.out_pos += 1;
                }
                let x = if self < 0 { -self } else { self };
                if x >= 10_000_000_000_000_000 {
                    io.out_put_upto_4digits((x / 10_000_000_000_000_000) as u16);
                    io.out_put_4digits(((x / 1_000_000_000_000) % 10_000) as u16);
                    io.out_put_4digits(((x / 100_000_000) % 10_000) as u16);
                    io.out_put_4digits(((x / 10_000) % 10_000) as u16);
                    io.out_put_4digits((x % 10_000) as u16);
                } else if x >= 1_000_000_000_000 {
                    io.out_put_upto_4digits(((x / 1_000_000_000_000) % 10_000) as u16);
                    io.out_put_4digits(((x / 100_000_000) % 10_000) as u16);
                    io.out_put_4digits(((x / 10_000) % 10_000) as u16);
                    io.out_put_4digits((x % 10_000) as u16);
                } else if x >= 100_000_000 {
                    io.out_put_upto_4digits(((x / 100_000_000) % 10_000) as u16);
                    io.out_put_4digits(((x / 10_000) % 10_000) as u16);
                    io.out_put_4digits((x % 10_000) as u16);
                } else if x >= 10_000 {
                    io.out_put_upto_4digits(((x / 10_000) % 10_000) as u16);
                    io.out_put_4digits((x % 10_000) as u16);
                } else {
                    io.out_put_upto_4digits((x % 10_000) as u16);
                }
                io.out_buf[io.out_pos] = b'\n';
                io.out_pos += 1;
                io.out_maybe_flush();
            }
            #[inline(always)]
            fn writeln_to(self, io: &mut Fastio) {
                self.write_to(io);
            }
        }
        impl FastWrite for i32 {
            #[inline(always)]
            fn write_to(self, io: &mut Fastio) {
                if self < 0 {
                    if self == i32::MIN {
                        io.out_buf[io.out_pos..io.out_pos + 11]
                            .copy_from_slice(b"-2147483648");
                        io.out_pos += 11;
                        io.out_buf[io.out_pos] = b'\n';
                        io.out_pos += 1;
                        io.out_maybe_flush();
                        return;
                    }
                    io.out_buf[io.out_pos] = b'-';
                    io.out_pos += 1;
                }
                let x = if self < 0 { -self } else { self } as u32;
                if x >= 100_000_000 {
                    io.out_put_upto_4digits((x / 100_000_000) as u16);
                    io.out_put_4digits(((x / 10_000) % 10_000) as u16);
                    io.out_put_4digits((x % 10_000) as u16);
                } else if x >= 10_000 {
                    io.out_put_upto_4digits((x / 10_000) as u16);
                    io.out_put_4digits((x % 10_000) as u16);
                } else {
                    io.out_put_upto_4digits(x as u16);
                }
                io.out_buf[io.out_pos] = b'\n';
                io.out_pos += 1;
                io.out_maybe_flush();
            }
            #[inline(always)]
            fn writeln_to(self, io: &mut Fastio) {
                self.write_to(io);
            }
        }
    }
}
