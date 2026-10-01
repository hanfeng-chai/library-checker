# common

common.h 集中包含本库使用的标准库、Linux 和 x86 头文件，并在 toy 命名空间
提供 i8/i16/i32/i64/i128、u8/u16/u32/u64/u128、usize/isize、f32/f64/f80。

它不导入 std 命名空间。`using namespace toy;` 只引入 toy 的名字，标准库名称
需要写 std::，例如：

```cpp
#include <toy/common.h>
using namespace toy;
std::array<i32, 4> values{};
std::span<const i32> view(values);
```

面向 x86-64、Linux、GCC/Clang、C++23。包含头文件不等于引入相应运行库；
实际可用的功能仍由 cxx_flags.txt 中的编译、链接选项约束。
