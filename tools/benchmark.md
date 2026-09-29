# Benchmark 要点

使用独立、空闲的评测机。同批比较保持机器、编译参数、输入和系统配置一致。
默认不绑核、不预热，每个测例只运行一次；沿用外部设置的 CPU 亲和性。

评测前完成编译、生成数据、正确性检查和文件同步，再运行环境检查：

```sh
python3 tools/bench_env.py
```

必须全部为 `OK`、退出码为 0：允许使用的 CPU 均为 performance governor，
Turbo（支持 Intel/AMD 接口）、ASLR、NMI watchdog、SMT 均关闭。
检查脚本只读配置；缺失接口显示 `UNKNOWN`，不能算通过。评测机还需安装 perf 并有计数权限。

确认准备完成、机器空闲后，在仓库根目录执行：

```sh
make bench-many_aplusb-chaihf  # 单个解答
make bench-many_aplusb        # 整题，二选一执行
# make bench                 # 全部题目
```

**评测期间不能在评测机上做其他操作。** 不编译、不测试、不同步或编辑文件，
不另开 SSH 查询状态或运行监控，也不能另起 make/perf。全部结束后再查看、取回结果。
单次 make 的 bench 会串行执行，但没有进程间锁，全机独占由使用者保证。

binary 和输入暂存到 `/dev/shm`，复制不计入 perf，程序输出丢弃。
测例按 `example*` 优先、组内字母序执行。结果保存在 `bench/<分类>/<题目>/<解答>.txt`。
`task-clock` 是 CPU 时间；`max`、`sum` 仅汇总该指标，包含 example。
IPC 为 instructions/cycles，running 为各事件计数覆盖率的最小值，不是稳定性评分。

重点比较 **50 ms 以上的大用例**。之前静默实测的大用例波动约为 0.1%–1%，
这只是部分用例的相对标准差，不是所有题目的误差保证。输出六位小数不代表相应的真实精度；
小用例易受启动开销影响，单次结果不足以证明约 1% 或更小的差距是有效优化。

结果会被缓存。更换机器或改变亲和性、内核参数等环境后，先删除对应报告再评测；
Makefile 不会自动识别这些环境变化。
