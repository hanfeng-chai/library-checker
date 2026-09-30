# line

`Line{slope,intercept}` 表示 i32 斜率、i64 截距的一次函数。
求值参数为 i32，乘加必须适合 i64。默认 Line{0,3e18} 表示不存在的候选，
用于 li_chao.h 与 line_hull.h；这两个最小值结构要求实际求值严格小于 3e18。
