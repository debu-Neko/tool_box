# tool_box

A small Windows toolbox written in C++20. CLI first, no GUI.

一个用 C++20 写的 Windows 小工具箱，命令行优先，没有 GUI。

目标：做一个日常能用的文件、系统、网络工具集，同时练习现代 C++ 和工程化流程。

## Features

- Single static `tool_box.exe`, no runtime DLLs required
- Subcommand based CLI, JSON output friendly
- Built with CMake + vcpkg + MinGW + Ninja

- 单个静态 `tool_box.exe`，运行时不需要额外 DLL
- 子命令式 CLI，方便脚本化，支持 JSON 输出
- 使用 CMake + vcpkg + MinGW + Ninja 构建

完整计划见 [Roadmap](#roadmap)。

## Build

Prerequisites:

- MinGW-w64 with GCC 13+ (tested on GCC 15.2.0)
- CMake 3.21+
- Ninja
- vcpkg

构建前置条件：

- MinGW-w64，GCC 13+（在 GCC 15.2.0 上测试）
- CMake 3.21+
- Ninja
- vcpkg

Set up vcpkg once:

首次配置 vcpkg：

```powershell
git clone https://github.com/microsoft/vcpkg.git D:\dev\vcpkg
cd D:\dev\vcpkg
.\bootstrap-vcpkg.bat

setx VCPKG_ROOT "D:\dev\vcpkg"
setx VCPKG_DEFAULT_TRIPLET "x64-mingw-static"
```

Reopen the terminal, then build:

重开终端后构建：

```powershell
cmake --preset mingw-debug
cmake --build --preset debug
```

Release build:

Release 构建：

```powershell
cmake --preset mingw-release
cmake --build --preset release
```

## Usage

```powershell
# Print help / 打印帮助
.\build\debug\tool_box.exe --help

# Print version / 打印版本
.\build\debug\tool_box.exe --version

# Greet / 打个招呼
.\build\debug\tool_box.exe -n Alice
```

## Roadmap

### Done

- [x] **Project skeleton** — CMake、vcpkg、MinGW、Ninja
- [x] **CLI framework** — CLI11 + fmt
- [x] **`--help` / `--version`** — 基础应用信息

### In progress

- [x] **`hash <file> --algo sha256 --json`** — 计算文件校验和

### Planned

File tools / 文件工具：

- [ ] **`hash <file>`** — 支持 MD5 / SHA256，流式读取，JSON 输出
- [ ] **`dup <dir>`** — 按大小 + 哈希查找重复文件
- [ ] **`size <dir> --depth N`** — 目录磁盘占用统计
- [ ] **`find <dir> --ext .log --older-than 7d`** — 按条件筛选文件
- [ ] **`tree <dir> --depth N`** — 目录树

System info / 系统信息：

- [ ] **`sysinfo --json`** — 输出 CPU、内存、磁盘信息
- [ ] **`proc list --sort cpu --top N`** — 进程列表
- [ ] **`proc kill <pid>`** — 结束进程
- [ ] **`port list --pid <pid>`** — 查看进程占用的端口
- [ ] **`env list --filter PATH`** — 查看环境变量

Network tools / 网络工具：

- [ ] **`http get <url> -v`** — HTTP 客户端，支持详细输出
- [ ] **`http post <url> -d <body>`** — 发送 JSON 请求
- [ ] **`port-scan <ip> --range 1-1024`** — TCP 端口扫描
- [ ] **`dns resolve <name>`** — DNS 查询
- [ ] **`ping <host> --count N`** — ICMP ping

Serial / device / 串口与设备：

- [ ] **`serial list`** — 枚举 COM 口
- [ ] **`serial monitor -p COM3 -b 115200`** — 实时串口日志
- [ ] **`serial send -p COM3 <data>`** — 向串口发送数据

Engineering / 工程化：

- [ ] **Unit tests** — Catch2，用 `ctest` 运行
- [ ] **Integration tests** — Python + pytest 调用 exe
- [ ] **CI** — GitHub Actions，MinGW 构建 + 测试
- [ ] **Formatting** — 配置 clang-format
- [ ] **Static analysis** — 配置 clang-tidy
- [ ] **Common flags** — `--verbose`、`--quiet`、`--json`、`--no-color`
- [ ] **Logging** — spdlog，支持 `--log-level`
- [ ] **Config file** — `~/.tool_box/config.toml`

Architecture / 架构：

- [ ] **Thread pool** — 并行哈希与扫描，支持 `--threads N`
- [ ] **Platform abstraction** — 例如 `ISystemInfoProvider`
- [ ] **Subcommand registry** — 新命令自动注册
- [ ] **Plugin / script extension** — Lua 或 Python 扩展

## Project layout

```text
tool_box/
  CMakeLists.txt
  CMakePresets.json
  vcpkg.json
  src/
    main.cpp
  tests/
  scripts/
  .gitignore
  README.md
```

## Design notes

- C++20，避免在热路径上使用异常
- 优先使用 `std::filesystem`、`std::span`、`std::string_view`
- 每条命令都支持 `--json`，方便脚本调用
- 静态链接，运行时不依赖外部 DLL
- 每个分支只做一件小事，能构建能运行就合并到 `main`

设计原则：

- 使用 C++20，热路径尽量不用异常
- 优先使用 `std::filesystem`、`std::span`、`std::string_view`
- 每条命令都支持 `--json`，方便脚本调用
- 静态链接，运行时不依赖外部 DLL
- 每个分支只做一件小事，能构建能运行就合并到 `main`

## License

MIT