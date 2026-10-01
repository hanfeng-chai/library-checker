# 连续存储的顶点分组

`VertexGroups(vertices, groups)` 预留顶点及组数上限；`add(v)` 追加顶点，`finish()` 结束一组，`operator[]` 返回该组的 span。允许不同组重复出现同一顶点，适合点双连通分量。调用方保证容量足够。
