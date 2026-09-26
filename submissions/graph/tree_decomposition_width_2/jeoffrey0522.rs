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

pub mod map {
    use std::{hash::Hash, mem::MaybeUninit};

    pub enum AdaptiveHashSet<K, const STACK_CAP: usize> {
        Small([MaybeUninit<K>; STACK_CAP], usize),
        Large(std::collections::HashSet<K>),
    }

    impl<K, const STACK_CAP: usize> Drop for AdaptiveHashSet<K, STACK_CAP> {
        fn drop(&mut self) {
            match self {
                Self::Small(arr, size) => {
                    for i in 0..*size {
                        unsafe { arr[i].assume_init_drop() }
                    }
                }
                _ => {}
            }
        }
    }

    impl<K: Clone, const STACK_CAP: usize> Clone for AdaptiveHashSet<K, STACK_CAP> {
        fn clone(&self) -> Self {
            match self {
                Self::Small(arr, size) => {
                    let mut cloned = std::array::from_fn(|_| MaybeUninit::uninit());
                    for i in 0..*size {
                        cloned[i] = MaybeUninit::new(unsafe { arr[i].assume_init_ref().clone() });
                    }
                    Self::Small(cloned, *size)
                }
                Self::Large(set) => Self::Large(set.clone()),
            }
        }
    }

    impl<K, const STACK_CAP: usize> Default for AdaptiveHashSet<K, STACK_CAP> {
        fn default() -> Self {
            Self::Small(std::array::from_fn(|_| MaybeUninit::uninit()), 0)
        }
    }

    impl<K: Eq + Hash, const STACK_CAP: usize> AdaptiveHashSet<K, STACK_CAP> {
        pub fn len(&self) -> usize {
            match self {
                Self::Small(_, size) => *size,
                Self::Large(set) => set.len(),
            }
        }

        pub fn contains(&self, key: &K) -> bool {
            match self {
                Self::Small(arr, size) => arr[..*size]
                    .iter()
                    .find(|&x| unsafe { x.assume_init_ref() } == key)
                    .is_some(),
                Self::Large(set) => set.contains(key),
            }
        }

        pub fn insert(&mut self, key: K) -> bool {
            if self.contains(&key) {
                return false;
            }
            match self {
                Self::Small(arr, size) => {
                    if arr[..*size]
                        .iter()
                        .find(|&x| unsafe { x.assume_init_ref() } == &key)
                        .is_some()
                    {
                        return false;
                    }

                    if *size < STACK_CAP {
                        arr[*size] = MaybeUninit::new(key);
                        *size += 1;
                    } else {
                        let arr =
                            std::mem::replace(arr, std::array::from_fn(|_| MaybeUninit::uninit()));
                        *size = 0; // Prevent `drop` call on arr elements
                        *self = Self::Large(
                            arr.into_iter()
                                .map(|x| unsafe { x.assume_init() })
                                .chain(Some(key))
                                .collect(),
                        );
                    }
                    true
                }
                Self::Large(set) => set.insert(key),
            }
        }

        pub fn remove(&mut self, key: &K) -> bool {
            match self {
                Self::Small(_, 0) => false,
                Self::Small(arr, size) => {
                    for i in 0..*size {
                        unsafe {
                            if arr[i].assume_init_ref() == key {
                                *size -= 1;
                                arr[i].assume_init_drop();
                                arr[i] = std::mem::replace(&mut arr[*size], MaybeUninit::uninit());
                                return true;
                            }
                        }
                    }

                    false
                }
                Self::Large(set) => set.remove(key),
            }
        }

        pub fn for_each(&mut self, mut visitor: impl FnMut(&K)) {
            match self {
                Self::Small(arr, size) => {
                    arr[..*size]
                        .iter()
                        .for_each(|x| visitor(unsafe { x.assume_init_ref() }));
                }
                Self::Large(set) => set.iter().for_each(visitor),
            }
        }
    }
}

pub mod tree_decomp {
    use std::collections::VecDeque;

    // pub type HashSet<T> = std::collections::HashSet<T>;
    pub type HashSet<T> = crate::map::AdaptiveHashSet<T, 5>;

    pub const UNSET: u32 = u32::MAX;

    // Tree decomposition of treewidth 2.
    #[derive(Clone)]
    pub struct TW2 {
        // Perfect elimination ordering in the chordal completion
        pub topological_order: Vec<u32>,
        pub t_in: Vec<u32>,
        pub parents: Vec<[u32; 2]>,
    }

    impl TW2 {
        pub fn from_edges(
            n_verts: usize,
            edges: impl IntoIterator<Item = [u32; 2]>,
        ) -> Option<Self> {
            let mut neighbors = vec![HashSet::default(); n_verts];
            for [u, v] in edges {
                neighbors[u as usize].insert(v);
                neighbors[v as usize].insert(u);
            }

            let mut visited = vec![false; n_verts];
            let mut parents = vec![[UNSET; 2]; n_verts];

            let mut topological_order = vec![];
            let mut t_in = vec![UNSET; n_verts];
            let mut root = None;

            let mut queue: [_; 3] = std::array::from_fn(|_| VecDeque::new());
            for u in 0..n_verts {
                let d = neighbors[u].len();
                if d <= 2 {
                    visited[u] = true;
                    queue[d].push_back(u as u32);
                }
            }

            while let Some(u) = (0..=2).flat_map(|i| queue[i].pop_front()).next() {
                t_in[u as usize] = topological_order.len() as u32;
                topological_order.push(u);

                match neighbors[u as usize].len() {
                    0 => {
                        if let Some(old_root) = root {
                            parents[old_root as usize][0] = u;
                        }
                        root = Some(u);
                    }
                    1 => {
                        let mut p = UNSET;
                        std::mem::take(&mut neighbors[u as usize]).for_each(|&v| p = v);
                        neighbors[p as usize].remove(&u);

                        parents[u as usize][0] = p;

                        if !visited[p as usize] && neighbors[p as usize].len() <= 2 {
                            visited[p as usize] = true;
                            queue[neighbors[p as usize].len()].push_back(p);
                        }
                    }
                    2 => {
                        let mut ps = [UNSET; 2];
                        let mut i = 0;
                        std::mem::take(&mut neighbors[u as usize]).for_each(|&v| {
                            ps[i] = v;
                            i += 1;
                        });
                        let [p, q] = ps;

                        neighbors[p as usize].remove(&u);
                        neighbors[q as usize].remove(&u);

                        neighbors[p as usize].insert(q);
                        neighbors[q as usize].insert(p);

                        parents[u as usize] = [p, q];

                        for w in [p, q] {
                            if !visited[w as usize] && neighbors[w as usize].len() <= 2 {
                                visited[w as usize] = true;
                                queue[neighbors[w as usize].len()].push_back(w);
                            }
                        }
                    }
                    _ => unreachable!(),
                }
            }

            if topological_order.len() != n_verts {
                return None;
            }
            assert_eq!(root, topological_order.iter().last().copied());

            for u in 0..n_verts {
                let ps = &mut parents[u];
                if ps[1] != UNSET && t_in[ps[0] as usize] > t_in[ps[1] as usize] {
                    ps.swap(0, 1);
                }
            }

            Some(Self {
                parents,
                topological_order,
                t_in,
            })
        }
    }
}

fn main() {
    let mut input = fast_io::stdin();
    let mut output = fast_io::stdout();

    assert!(input.token() == "p");
    assert!(input.token() == "tw");
    let n: usize = input.value();
    let m: usize = input.value();
    let edges = (0..m).map(|_| [input.u32() - 1, input.u32() - 1]);
    let Some(td) = tree_decomp::TW2::from_edges(n, edges) else {
        writeln!(output, "-1").ok();
        return;
    };

    writeln!(output, "s td {} {} {}", n, 2, n).ok();
    for b in 0..n {
        let mut line = vec![];
        write!(line, "b {} {} ", b + 1, b + 1).ok();
        for &p in &td.parents[b] {
            if p != tree_decomp::UNSET {
                write!(line, "{} ", p + 1).ok();
            }
        }
        line.pop();
        let line = unsafe { std::str::from_utf8_unchecked(&line) };
        writeln!(output, "{}", line).ok();
    }

    for b in 0..n {
        let a = td.parents[b][0];
        if a != tree_decomp::UNSET {
            writeln!(output, "{} {}", a + 1, b + 1).ok();
        }
    }
}
