# Benchmark

使用独立、空闲的评测机；同批保持机器、编译参数、输入和系统配置一致。
不绑核、不预热。编译、正确性检查、数据同步全部在计时前完成。

## Clang 23 对照

从仓库根目录执行以下命令，用 Fedora 45 toolbox 的 Clang 23 编译无手写汇编
版本。使用宿主机 sysroot，保持 C/C++ 头文件、libc/libm 与宿主机 GCC 一致；
否则比较会同时混入容器运行库的变化。

```bash
mkdir -p build/convolution/convolution_mod
toolbox run --container fedora-toolbox-45 clang++ \
  --sysroot=/run/host --gcc-toolchain=/run/host/usr \
  $(cat cxx_flags.txt) src/convolution/convolution_mod/ntt.cxx \
  -o build/convolution/convolution_mod/ntt-clang23
```

随后按下面的流程将产物同步到 Lenovo。Clang 会忽略 GCC 的 `-fwhole-program`；
两者均使用 x86-64-v3。对照产物另命名，避免覆盖默认 GCC binary。报告可在
manifest 的题目条目中用 `compilers`、`labels` 标明同一源码的不同编译器版本。
同题各算法必须使用相同的读写模板；程序自身的工作区分配和转换计入总耗时。

## 全量评测命令

以下命令在本机仓库根目录执行。`master.local` 用 `-j40` 编译和检查，
`lenovo.local` 专门评测；两机均使用账号 chai。已完成题目由
`src/*/*/main.cxx` 确定，包含其全部 `.cxx` 和本地下载提交；自建 checksum
没有 OJ 提交。需要预先生成这些题目的正式数据。

```bash
batch=full-$(date +%Y%m%d-%H%M%S)
stage=bench/$batch/stage
compile_root=/home/chai/.cache/lc-$batch-build
mkdir -p "$stage"

# 当前源码、参数、官方输入/答案和 checker 的传输清单。
python3 tools/bench_suite.py files > "$stage/files.txt"
git rev-parse HEAD > "$stage/source-commit"
ssh -o BatchMode=yes chai@master.local "mkdir -p '$compile_root'"
rsync -a --files-from="$stage/files.txt" ./ "chai@master.local:$compile_root/"
rsync -a "$stage/source-commit" "chai@master.local:$compile_root/.source-commit"

# 强制重编译，清除 AC 标记，重新检查；源码、参数、SHA-256 随批次保存。
ssh -o BatchMode=yes chai@master.local \
  "cd '$compile_root' && python3 tools/bench_suite.py prepare bench/stage -j 40"

# 取回 binary 和清单；输入沿用本机正式数据，核对与 master 检查的输入一致。
rsync -a --exclude='cases' "chai@master.local:$compile_root/bench/stage/" "$stage/"
rsync -a "$stage/bin/" build/
python3 tools/bench_suite.py inputs "$stage"

# 全部准备完成后，将 binary、输入和脚本同步到 Lenovo。
ssh -o BatchMode=yes chai@lenovo.local "mkdir -p '/home/chai/.cache/lc-$batch'"
rsync -a "$stage/" "chai@lenovo.local:/home/chai/.cache/lc-$batch/"

# 单个 SSH 串行执行两轮完整评测；保持此连接，等待命令自行结束。
ssh -o BatchMode=yes -o ServerAliveInterval=0 -o TCPKeepAlive=no chai@lenovo.local \
  "trap '' HUP; exec python3 '/home/chai/.cache/lc-$batch/bench_suite.py' run \
  '/home/chai/.cache/lc-$batch' > '/home/chai/.cache/lc-$batch/environment.log' 2>&1"

# 上一个命令正常结束后，再取回结果、生成报告。
rsync -a --include='result.json' --include='round-*.json' \
  --include='environment.log' --exclude='*' \
  "chai@lenovo.local:/home/chai/.cache/lc-$batch/" "$stage/"
python3 tools/bench_suite.py report "$stage"
```

运行脚本先将栈软限制提高到硬上限，执行 `bench_env.py`，核对所有文件的 SHA-256，
把程序复制到 `/dev/shm`，同步磁盘后空闲 15 秒。输入逐测例暂存到 `/dev/shm`，
复制不计入 perf；同一输入的所有程序串行执行，stdout 丢弃。
测例按 `example*` 优先、其余字母序排列；第一轮完整结束后才开始第二轮。

每个 binary、每个测例选两轮中较小的 task-clock；cycles、instructions 等也取自
这次运行。随后计算 max、sum，包含样例。这是两次观测的较小值，不是平均性能或
统计置信区间；原始两轮保存于 `round-1.json`、`round-2.json`，方便检查波动。

生成的 `src/<分类>/<题目>/bench.txt` 包含所有解答的汇总和逐例计数器；
`tutorial.md` 顶部列自有解答的 max、sum，以及相对全部已测参考最佳值的百分比。
参考 max、sum 分别取最小值，可能来自不同提交；负百分比表示更快。
发布前核对当前源码和 binary 与被测文件一致，缺失任一轮则拒绝生成报告。

## 环境与独占

`bench_env.py` 必须全部为 `OK`、退出码为 0：允许使用的 CPU 均为 performance
governor，Turbo/boost、ASLR、NMI watchdog、SMT 均关闭。缺失接口为 `UNKNOWN`，
不能算通过。评测机需安装 perf 并允许读取硬件计数器。
单独检查环境的完整命令如下；应在准备阶段执行：

```bash
rsync -a tools/bench_env.py chai@lenovo.local:/home/chai/.cache/lc-bench-env.py
ssh -o BatchMode=yes chai@lenovo.local 'python3 /home/chai/.cache/lc-bench-env.py'
ssh -o BatchMode=yes chai@lenovo.local 'cat /proc/sys/kernel/perf_event_paranoid'
```

**评测期间不能在评测机上做其他操作。** 不另起 make/perf，不编译、不测试、
不同步、不编辑，不另开 SSH 查看 ps/top/日志，也不运行监控。
只等待本机已有的 SSH 进程；本机自身可以继续工作。没有进程间锁，由使用者保证全机独占。

命令中的 `BatchMode=yes` 禁止交互；`ServerAliveInterval=0` 和 `TCPKeepAlive=no`
关闭客户端两层保活，减少定时网络活动。这不控制服务端保活，也没有单独证明可观提速。
前者默认就是 0，显式指定可覆盖本地配置。参见 [OpenSSH 文档](https://man.openbsd.org/ssh_config#ServerAliveInterval)。

SSH 连接中断时，不立即另起评测。忽略 HUP 的任务可能仍在运行，应等原批次预计
结束后再登录确认；结果缺失、覆盖不完整或无法确认机器空闲的批次不能发布。

## 单个程序与精度

已有数据下，`make bench-many_aplusb-main` 测一个程序，`make bench-many_aplusb`
测整题；仍是每例一次并缓存到 `bench/<分类>/<题目>/<解答>.txt`。
即使 `make -j`，同一次 make 的 bench 也串行执行。更换环境后须删除旧报告；
环境变化不会自动让缓存失效。全量两轮流程使用上面的独立批次，不复用这些报告。

主要看 **50 ms 以上的大用例**。task-clock 是用户态加内核态 CPU 时间，包含缺页、
内存分配等成本；IPC = instructions/cycles，running 是各计数器覆盖率的最小值。
输出六位小数不代表微秒级精度。既往静默测试的部分大用例波动约为 0.1%–1%，
不是所有题目的误差保证；两个较小值接近时，不能凭小数位断言优化有效。

## 跟进优化时复用结果

完整基准完成后，只计时新程序或 SHA-256 已变化的 binary；已有参考和经典解
沿用原始两轮记录。候选采用后若正式 binary 的 SHA-256 相同，直接复用候选数据。
每个程序仍须有覆盖完整测例的两轮，逐例取其中较小值；保留每个程序的来源批次、
编译参数、binary 和输入哈希，不将不同源码的观察混成一份结果。


新增候选先在构建机完成全部正确性检查，并注意单例是否出现数量级异常。
例如费用流的反 SSP 数据曾让一个正确的选边策略耗时数分钟；应先排除此类候选，
再占用独立评测机。正确性通过不代表候选值得完成两轮计时。

主动中止或执行期间受到干扰的批次不用于最终报告。已完整完成的其他批次仍按
binary、输入 SHA-256 复用；不要为补一个候选重新跑整题的所有参考。
