# affine

`Affine<P>{a,b}` 表示 x→a*x+b，默认恒等函数、P=998244353。
`ComposeAffine<P>{}(f,g)` 返回先 f 后 g 的复合，系数为普通模余数。
P 的约束同 [mod](mod.md)，系数和输入值均须小于 P。
该操作满足结合律，可作为 [fold](fold.md) 队列/双端队列的幺半群。
