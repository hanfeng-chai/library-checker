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

pub mod jagged {
    use std::fmt::Debug;
    use std::iter;
    use std::mem::MaybeUninit;
    use std::ops::{Index, IndexMut};

    // Trait for painless switch between different representations of a jagged array
    pub trait Jagged<T>: IndexMut<usize, Output = [T]> {
        fn len(&self) -> usize;
    }

    impl<T, C> Jagged<T> for C
    where
        C: AsRef<[Vec<T>]> + IndexMut<usize, Output = [T]>,
    {
        fn len(&self) -> usize {
            <Self as AsRef<[Vec<T>]>>::as_ref(self).len()
        }
    }

    // Compressed sparse row format for jagged array
    // Provides good locality for graph traversal, but works only for static ones.
    #[derive(Clone, PartialEq, Eq, PartialOrd, Ord)]
    pub struct CSR<T> {
        data: Vec<T>,
        head: Vec<u32>,
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
            CSR { data, head }
        }
    }

    impl<T: Clone> CSR<T> {
        pub fn from_pairs(n: usize, pairs: &[(u32, T)]) -> Self {
            let mut head = vec![0u32; n + 1];

            for &(u, _) in pairs {
                debug_assert!(u < n as u32);
                head[u as usize + 1] += 1;
            }
            for i in 2..n + 1 {
                head[i] += head[i - 1];
            }
            let mut data: Vec<_> = iter::repeat_with(|| MaybeUninit::uninit())
                .take(head[n] as usize)
                .collect();
            let mut pos = head.clone();

            for (u, v) in pairs {
                data[pos[*u as usize] as usize] = MaybeUninit::new(v.clone());
                pos[*u as usize] += 1;
            }

            let data = std::mem::ManuallyDrop::new(data);
            let data = unsafe {
                Vec::from_raw_parts(data.as_ptr() as *mut T, data.len(), data.capacity())
            };

            CSR { data, head }
        }
    }

    impl<T> Index<usize> for CSR<T> {
        type Output = [T];

        fn index(&self, index: usize) -> &Self::Output {
            &self.data[self.head[index] as usize..self.head[index + 1] as usize]
        }
    }

    impl<T> IndexMut<usize> for CSR<T> {
        fn index_mut(&mut self, index: usize) -> &mut Self::Output {
            &mut self.data[self.head[index] as usize..self.head[index + 1] as usize]
        }
    }

    impl<T> Jagged<T> for CSR<T> {
        fn len(&self) -> usize {
            self.head.len() - 1
        }
    }
}

pub mod chordal_graph {
    use crate::jagged::{self, Jagged};

    mod linked_list {
        use std::{
            marker::PhantomData,
            num::NonZeroU32,
            ops::{Index, IndexMut},
        };

        #[derive(Debug)]
        pub struct Cursor<T> {
            idx: NonZeroU32,
            _marker: PhantomData<*const T>,
        }

        // Arena-allocated pool of doubly linked lists.
        // Semantically unsafe, as cursors can outlive and access removed elements.
        #[derive(Clone, Debug)]
        pub struct MultiList<T> {
            links: Vec<[Option<Cursor<T>>; 2]>,
            values: Vec<T>,
            freed: Vec<Cursor<T>>,
        }

        impl<T> Clone for Cursor<T> {
            fn clone(&self) -> Self {
                Self::new(self.idx.get() as usize)
            }
        }

        impl<T> Copy for Cursor<T> {}

        impl<T> Cursor<T> {
            fn new(idx: usize) -> Self {
                Self {
                    idx: NonZeroU32::new(idx as u32).unwrap(),
                    _marker: PhantomData,
                }
            }

            pub fn usize(&self) -> usize {
                self.idx.get() as usize
            }
        }

        impl<T> Index<Cursor<T>> for MultiList<T> {
            type Output = T;
            fn index(&self, index: Cursor<T>) -> &Self::Output {
                &self.values[index.usize()]
            }
        }

        impl<T> IndexMut<Cursor<T>> for MultiList<T> {
            fn index_mut(&mut self, index: Cursor<T>) -> &mut Self::Output {
                &mut self.values[index.usize()]
            }
        }

        impl<T: Default> MultiList<T> {
            pub fn new() -> Self {
                Self {
                    links: vec![[None; 2]],
                    values: vec![Default::default()],
                    freed: vec![],
                }
            }

            pub fn next(&self, i: Cursor<T>) -> Option<Cursor<T>> {
                self.links[i.usize()][1]
            }

            pub fn prev(&self, i: Cursor<T>) -> Option<Cursor<T>> {
                self.links[i.usize()][0]
            }

            pub fn singleton(&mut self, value: T) -> Cursor<T> {
                if let Some(idx) = self.freed.pop() {
                    self.links[idx.usize()] = [None; 2];
                    self.values[idx.usize()] = value;
                    idx
                } else {
                    let idx = self.links.len();
                    self.links.push([None; 2]);
                    self.values.push(value);
                    Cursor::new(idx)
                }
            }

            fn link(&mut self, u: Cursor<T>, v: Cursor<T>) {
                self.links[u.usize()][1] = Some(v);
                self.links[v.usize()][0] = Some(u);
            }

            pub fn insert_left(&mut self, i: Cursor<T>, value: T) -> Cursor<T> {
                let v = self.singleton(value);
                if let Some(j) = self.prev(i) {
                    self.link(j, v);
                }
                self.link(v, i);
                v
            }

            pub fn insert_right(&mut self, i: Cursor<T>, value: T) -> Cursor<T> {
                let v = self.singleton(value);
                if let Some(j) = self.next(i) {
                    self.link(v, j);
                }
                self.link(i, v);
                v
            }

            pub fn erase(&mut self, i: Cursor<T>) {
                let l = self.prev(i);
                let r = self.next(i);
                if let Some(l_inner) = l {
                    self.links[l_inner.usize()][1] = r;
                }
                if let Some(r_inner) = r {
                    self.links[r_inner.usize()][0] = l;
                }
                self.links[i.usize()] = [None; 2];

                self.freed.push(i);
            }
        }
    }

    // Lexicographic bfs, O(N)
    pub fn lex_bfs(neighbors: &impl Jagged<u32>) -> (Vec<u32>, Vec<u32>) {
        let n = neighbors.len();

        let mut heads = linked_list::MultiList::new();
        let h_begin = heads.singleton(0u32);
        let h_end = heads.insert_right(h_begin, n as u32);

        let mut bfs: Vec<_> = (0..n as u32).collect();
        let mut t_in: Vec<_> = (0..n as u32).collect();
        let mut visited = vec![false; n];
        let mut owner = vec![h_begin; n];

        let erase_head = |heads: &mut linked_list::MultiList<u32>, h: &mut _| {
            if heads[*h] + 1 != heads[heads.next(*h).unwrap()] {
                heads[*h] += 1;
            } else {
                heads.erase(*h);
                *h = h_end;
            }
        };

        let mut t_split = vec![!0; n];
        for i in 0..n {
            let u = bfs[i] as usize;
            visited[u] = true;
            erase_head(&mut heads, &mut owner[u]);

            for &v in &neighbors[u] {
                let v = v as usize;
                if visited[v] {
                    continue;
                }

                let h = bfs[heads[owner[v]] as usize] as usize;
                t_in.swap(h, v);
                bfs.swap(t_in[h] as usize, t_in[v] as usize);

                if t_split.len() <= owner[v].usize() {
                    t_split.resize(owner[v].usize() + 1, !0);
                }
                let p = if t_split[owner[v].usize()] == i as u32 {
                    heads.prev(owner[v]).unwrap()
                } else {
                    t_split[owner[v].usize()] = i as u32;
                    heads.insert_left(owner[v], t_in[v])
                };
                erase_head(&mut heads, &mut owner[v]);
                owner[v] = p;
            }
        }

        (bfs, t_in)
    }

    // Perfect elimination ordering, reversed.
    pub fn rev_peo(neighbors: &impl Jagged<u32>) -> Result<(Vec<u32>, Vec<u32>), Vec<u32>> {
        let n = neighbors.len();
        let (bfs, t_in) = lex_bfs(neighbors);

        let mut successors = vec![];
        for u in 0..n {
            let mut t_max = None;
            for &v in &neighbors[u] {
                if t_in[v as usize] < t_in[u] {
                    t_max = t_max.max(Some(t_in[v as usize]));
                }
            }

            if let Some(t_max) = t_max {
                successors.push((bfs[t_max as usize], u as u32));
            }
        }

        let successors = jagged::CSR::from_pairs(n, &successors);

        let mut marker = vec![!0; n];
        for p in 0..n {
            if successors[p].is_empty() {
                continue;
            }

            for &v in &neighbors[p] {
                marker[v as usize] = p as u32;
            }

            for &u in &successors[p] {
                for &v in &neighbors[u as usize] {
                    if t_in[v as usize] < t_in[p] && marker[v as usize] != p as u32 {
                        const UNSET: u32 = !0;
                        const UNVISITED: u32 = !0 - 1;
                        const BANNED: u32 = !0 - 2;
                        let mut timer = 0;
                        let mut bfs = vec![v];
                        let mut parent = vec![UNVISITED; n];

                        parent[u as usize] = BANNED;
                        for &y in &neighbors[u as usize] {
                            parent[y as usize] = BANNED;
                        }
                        parent[v as usize] = UNSET;
                        parent[p] = UNVISITED;

                        while let Some(&x) = bfs.get(timer) {
                            timer += 1;
                            if x == p as u32 {
                                break;
                            }
                            for &y in &neighbors[x as usize] {
                                if parent[y as usize] != UNVISITED {
                                    continue;
                                }
                                parent[y as usize] = x;
                                bfs.push(y);
                            }
                        }

                        let mut cycle = vec![];
                        let mut c = p;
                        while c != !0u32 as usize {
                            cycle.push(c as u32);
                            c = parent[c] as usize;
                        }

                        cycle.push(u);
                        return Err(cycle);
                    }
                }
            }
        }

        Ok((bfs, t_in))
    }
}

fn main() {
    let mut input = fast_io::stdin();
    let mut output = fast_io::stdout();

    let n: usize = input.value();
    let m: usize = input.value();
    let mut edges = vec![];
    for _ in 0..m {
        let u = input.u32();
        let v = input.u32();
        edges.push((u, v));
        edges.push((v, u));
    }
    let neighbors = jagged::CSR::from_pairs(n, &edges);

    match chordal_graph::rev_peo(&neighbors) {
        Ok((bfs, _t_in)) => {
            writeln!(output, "YES").ok();
            for u in bfs.into_iter().rev() {
                write!(output, "{} ", u).ok();
            }
        }
        Err(cycle) => {
            writeln!(output, "NO").ok();
            writeln!(output, "{}", cycle.len()).ok();
            for u in cycle {
                write!(output, "{} ", u).ok();
            }
        }
    }
}
