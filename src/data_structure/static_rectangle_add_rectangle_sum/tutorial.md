# Static Rectangle Add Rectangle Sum / 静态矩形加、矩形求和

## 中文

每个更新给半开矩形 `[l,r) x [d,u)` 的每个单位格加 `w`，查询另一个半开
矩形中的总和。逐对求交会是 `O(NQ)`；本解把每个矩形变成双线性函数的事件。

### 1. 二维前缀的形状

令 `F(X,Y)` 表示 `[0,X) x [0,Y)` 中的总和。一个更新矩形对它的贡献是

```text
w * clamp(X-l, 0, r-l) * clamp(Y-d, 0, u-d)。
```

每个 clamp 都是“先为零、再线性增长、最后变常数”的折线。扫描 X 时，在
`x=l` 加入斜率 `+w`，在 `x=r` 加入斜率 `-w`。对 y 同样在 `d,u` 做差分。

### 2. 四个系数从哪里来

设某个 x 事件位于 `x0`，某个 y 差分端点位于 `y0`，两者合成的带符号系数
为 `c`。当 `X>x0,Y>y0` 时，它贡献

```text
c * (X-x0) * (Y-y0)
= c*X*Y - c*y0*X - c*x0*Y + c*x0*y0。
```

因此只需在压缩 y 上维护四个系数：

```text
XY: c
X : -c*y0
Y : -c*x0
1 : c*x0*y0
```

一个矩形产生两个 x 事件；每个 x 事件又在 `d` 加一组系数、在 `u` 加相反
系数。公共实现把四个数放在同一个 `Coefficient` 中，让一次 Fenwick 路径
同时更新四项，而不是走四棵独立树。

### 3. 查询也只需要两个 x 事件

对固定 X，先计算

```text
F(X,u) - F(X,d)
```

就得到纵向 `[d,u)` 在横向前缀 `[0,X)` 中的和。最终矩形答案是这项在
`X=r` 的值减去 `X=l` 的值。因此每个查询只生成两个 x 事件，而非四个独立
二维前缀事件。

扫描查询事件 `X` 前，只加入 `x0<X` 的更新事件。等号处
`(X-x0)=0`，加入与否理论上都为零；严格小于使半开语义和实现不变量更直接。

### 4. 正确性

四系数展开与 `c(X-x0)(Y-y0)` 恒等。x 的 `+w/-w` 事件把无限线性增长截断
为 `clamp(X-l,0,r-l)`，y 的两个端点同理形成另一个 clamp，所以扫描结构在
任意 `(X,Y)` 求得的值恰好是所有更新对 `F(X,Y)` 的贡献之和。纵轴前缀差和
横轴两个事件再分别做一次集合容斥，故最终值正是查询矩形中的总和。

### 5. 模运算、复杂度与性能

所有系数以模 `998244353` 表示。Fenwick 结点用 64 位无符号整数暂存若干模
值之和，在本题规模下不会溢出；求值时再归约，避免每次树更新做四次取模。

- 时间：`O((N+Q)logN)`；
- 空间：`O(N+Q)`。

只保留 `coefficient_fenwick_sweep.cpp` 这一份核心清楚且已达到头部性能的
算法。代表性最大随机用例约
`569 ms`；公开第一 `#210893` 的稳定最好结果约 `618 ms`（个别墙钟样本受
系统量化影响更大）。

## English

Each update adds `w` to every unit cell of `[l,r) x [d,u)`, and each query asks
for the sum in another half-open rectangle. Pairwise intersections would cost
`O(NQ)`; the sweep represents every update as bilinear-function events.

Let `F(X,Y)` be the sum in `[0,X) x [0,Y)`. One update contributes

```text
w * clamp(X-l, 0, r-l) * clamp(Y-d, 0, u-d).
```

During an x sweep, events at `l` and `r` start and stop the x slope. A signed x
event at `x0` combined with a y endpoint `y0` contributes

```text
c(X-x0)(Y-y0)
= cXY - c*y0*X - c*x0*Y + c*x0*y0.
```

Thus a y-Fenwick tree stores four coefficients `(XY, X, Y, 1)`. Each rectangle
has two x events, and each event applies opposite coefficient records at `d`
and `u`. The implementation stores all four fields in one Fenwick element, so
one traversal updates them together.

For a fixed X, `F(X,u)-F(X,d)` gives the requested y strip in the x prefix.
Evaluating this expression at `r` and subtracting its value at `l` means each
query also needs only two x events.

The coefficient identity is exact. Opposite events at `l/r` and `d/u` form the
two clamped factors, so evaluation yields the correct 2D prefix. The final x
and y differences are standard inclusion-exclusion, proving the rectangle
answer.

`coefficient_fenwick_sweep.cpp` represents coefficients modulo `998244353`;
64-bit Fenwick accumulators
delay reductions safely under the problem bounds. Time is
`O((N+Q)logN)` and memory is `O(N+Q)`. The representative maximum random case
takes about `569 ms`, versus a stable best near `618 ms` for public leader
`#210893`.
