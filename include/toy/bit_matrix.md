# bit_matrix

`BitMatrix(n,m)` 建立全零的 GF(2) 位矩阵，每行按 u64 存储；`get`、`set` 读写
一个位。`pack_bits` 将 01 字符串写入预先清零的行，AVX2 一次压缩 32 个字符；
`unpack_bits` 将任意连续列展开成字符串，BMI2 每次把八个位放入八个字节。

`eliminate(columns,reduced=false)` 只在前 columns 列选主元，其余列跟随变换。
返回秩；`pivot[i]` 是第 i 行的主元列，不保证全局递增。reduced=true 清除所有
其他行的主元位，可用来求逆、解方程及零空间。

每组选至多八个独立行，先将组内主元化为单位块，再预计算所有行的 XOR 组合，
一次查表消去整组主元。跨至多两个机器字的主元集合用 PEXT 提取，其他情形
逐位提取。所有表和行都只有实际列宽，避免窄矩阵的大量填充。

`bit_matrix_product(a,b)` 使用同样的八位查表思想，返回矩阵积。
`binary_intersection(u,v)` 接收 F_2^32 中两组独立基；消元时用高 32 位记录 U
分量，返回交空间的独立基。
