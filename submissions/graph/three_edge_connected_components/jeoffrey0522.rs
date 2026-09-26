use std::{collections::HashMap, io::Write};

use jagged::CSR;

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

mod rand {
    // Written in 2015 by Sebastiano Vigna (vigna@acm.org)
    // https://xoshiro.di.unimi.it/splitmix64.c
    use std::ops::Range;

    pub struct SplitMix64(u64);

    impl SplitMix64 {
        pub fn new(seed: u64) -> Self {
            assert_ne!(seed, 0);
            Self(seed)
        }

        // Available on x86-64 and target feature rdrand only.
        #[cfg(target_arch = "x86_64")]
        pub fn from_entropy() -> Option<Self> {
            let mut seed = 0;
            unsafe { (std::arch::x86_64::_rdrand64_step(&mut seed) == 1).then(|| Self(seed)) }
        }
        #[cfg(not(target_arch = "x86_64"))]
        pub fn from_entropy() -> Self {
            use std::time::{SystemTime, UNIX_EPOCH};
            let seed = SystemTime::now()
                .duration_since(UNIX_EPOCH)
                .unwrap()
                .as_nanos();
            Self(seed as u64)
        }

        pub fn next_u64(&mut self) -> u64 {
            self.0 = self.0.wrapping_add(0x9e3779b97f4a7c15);
            let mut x = self.0;
            x = (x ^ (x >> 30)).wrapping_mul(0xbf58476d1ce4e5b9);
            x = (x ^ (x >> 27)).wrapping_mul(0x94d049bb133111eb);
            x ^ (x >> 31)
        }

        pub fn range_u64(&mut self, range: Range<u64>) -> u64 {
            let Range { start, end } = range;
            debug_assert!(start < end);

            let width = end - start;
            let test = (u64::MAX - width) % width;
            loop {
                let value = self.next_u64();
                if value >= test {
                    return start + value % width;
                }
            }
        }

        pub fn shuffle<T>(&mut self, xs: &mut [T]) {
            let n = xs.len();
            if n == 0 {
                return;
            }

            for i in 0..n - 1 {
                let j = self.range_u64(i as u64..n as u64) as usize;
                xs.swap(i, j);
            }
        }
    }
}

pub mod jagged {
    use std::fmt::Debug;
    use std::mem::MaybeUninit;
    use std::ops::{Index, IndexMut};

    // Compressed sparse row format, for static jagged array
    // Provides good locality for graph traversal
    #[derive(Clone, PartialEq, Eq, PartialOrd, Ord)]
    pub struct CSR<T> {
        pub links: Vec<T>,
        head: Vec<u32>,
    }

    impl<T> Default for CSR<T> {
        fn default() -> Self {
            Self {
                links: vec![],
                head: vec![0],
            }
        }
    }

    impl<T: Clone> CSR<T> {
        pub fn from_pairs(n: usize, pairs: impl Iterator<Item = (u32, T)> + Clone) -> Self {
            let mut head = vec![0u32; n + 1];

            for (u, _) in pairs.clone() {
                debug_assert!(u < n as u32);
                head[u as usize] += 1;
            }
            for i in 0..n {
                head[i + 1] += head[i];
            }
            let mut data: Vec<_> = (0..head[n]).map(|_| MaybeUninit::uninit()).collect();

            for (u, v) in pairs {
                head[u as usize] -= 1;
                data[head[u as usize] as usize] = MaybeUninit::new(v.clone());
            }

            // Rustc is likely to perform in‑place iteration without new allocation.
            // [https://doc.rust-lang.org/stable/std/iter/trait.FromIterator.html#impl-FromIterator%3CT%3E-for-Vec%3CT%3E]
            let data = data
                .into_iter()
                .map(|x| unsafe { x.assume_init() })
                .collect();

            CSR { links: data, head }
        }
    }

    impl<T, I> FromIterator<I> for CSR<T>
    where
        I: IntoIterator<Item = T>,
    {
        fn from_iter<J>(iter: J) -> Self
        where
            J: IntoIterator<Item = I>,
        {
            let mut data = vec![];
            let mut head = vec![];
            head.push(0);

            let mut cnt = 0;
            for row in iter {
                data.extend(row.into_iter().inspect(|_| cnt += 1));
                head.push(cnt);
            }
            CSR { links: data, head }
        }
    }

    impl<T> CSR<T> {
        pub fn len(&self) -> usize {
            self.head.len() - 1
        }

        pub fn edge_range(&self, index: usize) -> std::ops::Range<usize> {
            self.head[index] as usize..self.head[index as usize + 1] as usize
        }
    }

    impl<T> Index<usize> for CSR<T> {
        type Output = [T];

        fn index(&self, index: usize) -> &Self::Output {
            &self.links[self.edge_range(index)]
        }
    }

    impl<T> IndexMut<usize> for CSR<T> {
        fn index_mut(&mut self, index: usize) -> &mut Self::Output {
            let es = self.edge_range(index);
            &mut self.links[es]
        }
    }

    impl<T> Debug for CSR<T>
    where
        T: Debug,
    {
        fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            let v: Vec<Vec<&T>> = (0..self.len()).map(|i| self[i].iter().collect()).collect();
            v.fmt(f)
        }
    }
}

const UNSET: u32 = !0;

mod dset {
    use std::{cell::Cell, mem};

    #[derive(Clone)]
    pub struct DisjointSet {
        // Represents parent if >= 0, size if < 0
        parent_or_size: Vec<Cell<i32>>,
    }

    impl DisjointSet {
        pub fn new(n: usize) -> Self {
            Self {
                parent_or_size: vec![Cell::new(-1); n],
            }
        }

        fn get_parent_or_size(&self, u: usize) -> Result<usize, u32> {
            let x = self.parent_or_size[u].get();
            if x >= 0 {
                Ok(x as usize)
            } else {
                Err((-x) as u32)
            }
        }

        fn set_parent(&self, u: usize, p: usize) {
            self.parent_or_size[u].set(p as i32);
        }

        fn set_size(&self, u: usize, s: u32) {
            self.parent_or_size[u].set(-(s as i32));
        }

        pub fn find_root_with_size(&self, u: usize) -> (usize, u32) {
            match self.get_parent_or_size(u) {
                Ok(p) => {
                    let (root, size) = self.find_root_with_size(p);
                    self.set_parent(u, root);
                    (root, size)
                }
                Err(size) => (u, size),
            }
        }

        pub fn find_root(&self, u: usize) -> usize {
            self.find_root_with_size(u).0
        }

        // Returns true if two sets were previously disjoint
        pub fn merge(&mut self, u: usize, v: usize) -> bool {
            let (mut u, size_u) = self.find_root_with_size(u);
            let (mut v, size_v) = self.find_root_with_size(v);
            if u == v {
                return false;
            }

            if size_u < size_v {
                mem::swap(&mut u, &mut v);
            }
            self.set_parent(v, u);
            self.set_size(u, size_u + size_v);
            true
        }
    }
}

fn main() {
    let mut input = fast_io::stdin();
    let mut output = fast_io::stdout();

    let n: usize = input.value();
    let m: usize = input.value();

    let edges: Vec<_> = (0..m).map(|_| [input.u32(), input.u32()]).collect();
    let neighbors = CSR::from_pairs(
        n,
        edges
            .iter()
            .enumerate()
            .flat_map(|(e, &[u, v])| [(u, (v, e as u32)), (v, (u, e as u32))]),
    );

    let mut rng = rand::SplitMix64::from_entropy().unwrap();
    let zobrist: Vec<_> = (0..m).map(|_| rng.next_u64()).collect();

    let mut lowpt = vec![0u32; n];
    let mut lowe = vec![UNSET; n];
    let mut n_cover = vec![0i32; n];
    let mut xor_cover = vec![0; n];

    let mut t_in = vec![UNSET; n];
    let mut parent: Vec<_> = (0..n as u32).collect();
    let mut parent_edge = vec![UNSET; n];
    let mut timer = 0;

    let mut associated_cut = vec![0u8; m]; // Alternative name: `is_bridge`
    let mut groups = HashMap::<_, Vec<u32>>::new();

    let mut current_edge: Vec<_> = (0..n)
        .map(|u| neighbors.edge_range(u).start as u32)
        .collect();
    for root in 0..n {
        if t_in[root] != UNSET {
            continue;
        }
        t_in[root] = timer;
        timer += 1;

        let mut u = root as u32;
        loop {
            let p = parent[u as usize];
            let ie = current_edge[u as usize];
            current_edge[u as usize] += 1;
            if ie == neighbors.edge_range(u as usize).start as u32 {
                // On enter
                t_in[u as usize] = timer;
                lowpt[u as usize] = timer;
                timer += 1;
            }
            if ie == neighbors.edge_range(u as usize).end as u32 {
                // On exit
                if p == u {
                    break;
                }

                match n_cover[u as usize] {
                    0 => {
                        associated_cut[parent_edge[u as usize] as usize] = 1;
                    }
                    1 => {
                        associated_cut[parent_edge[u as usize] as usize] = 2;
                        associated_cut[lowe[u as usize] as usize] = 2;
                    }
                    _ => {
                        groups.entry(xor_cover[u as usize]).or_default().push(u);
                    }
                }

                if lowpt[u as usize] < lowpt[p as usize] {
                    lowpt[p as usize] = lowpt[u as usize];
                    lowe[p as usize] = lowe[u as usize];
                }

                n_cover[p as usize] += n_cover[u as usize];
                xor_cover[p as usize] ^= xor_cover[u as usize];

                u = p;
                continue;
            }

            let (v, e) = neighbors.links[ie as usize];
            if e == parent_edge[u as usize] {
                continue;
            }

            if t_in[v as usize] == UNSET {
                // Front edge
                parent[v as usize] = u;
                parent_edge[v as usize] = e;

                u = v;
            } else if t_in[v as usize] < t_in[u as usize] {
                // Back edge
                if t_in[v as usize] < lowpt[u as usize] {
                    lowpt[u as usize] = t_in[v as usize];
                    lowe[u as usize] = e;
                }

                n_cover[u as usize] += 1;
                n_cover[v as usize] -= 1;
                xor_cover[u as usize] ^= zobrist[e as usize];
                xor_cover[v as usize] ^= zobrist[e as usize];
            }
        }
    }

    let mut additional_edges = vec![];
    for path in groups.into_values() {
        if path.len() <= 1 {
            continue;
        }

        for &u in &path {
            associated_cut[parent_edge[u as usize] as usize] = 2;
        }

        let tail = path[0];
        let head = parent[*path.last().unwrap() as usize];
        additional_edges.push([tail, head]);
    }

    let mut conn_3ec = dset::DisjointSet::new(n);
    for e in 0..m {
        if associated_cut[e] != 0 {
            continue;
        }
        let [u, v] = edges[e];
        conn_3ec.merge(u as usize, v as usize);
    }
    for [u, v] in additional_edges {
        conn_3ec.merge(u as usize, v as usize);
    }

    let comps_3ec =
        jagged::CSR::from_pairs(n, (0..n).map(|u| (conn_3ec.find_root(u) as u32, u as u32)));

    writeln!(
        output,
        "{}",
        (0..n).filter(|&u| !comps_3ec[u].is_empty()).count()
    )
    .unwrap();
    for r in 0..n {
        if comps_3ec[r].is_empty() {
            continue;
        }

        write!(output, "{} ", comps_3ec[r].len()).unwrap();
        for &u in &comps_3ec[r] {
            write!(output, "{} ", u).unwrap();
        }
        writeln!(output).unwrap();
    }
}
