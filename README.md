# WinPulse

**Repository / 仓库地址**
[https://github.com/2017-z/WinPulse](https://github.com/2017-z/WinPulse)

---

## Introduction / 项目简介

WinPulse is a lightweight, non-intrusive system performance monitoring tool designed for the Windows platform. It is developed using Modern C++ (C++20) and focuses on long-running background logging with minimal resource overhead. The project follows strict engineering practices, leverages RAII for resource management, and adopts a modular architecture to ensure high extensibility.

WinPulse 是一款面向 Windows 平台的轻量级、低侵入式系统性能监控工具，采用现代 C++（C++20）开发，专注于在极低资源占用的前提下进行后台性能日志记录。项目遵循严格的工程规范，使用 RAII 进行资源管理，并通过模块化架构实现良好的可扩展性。

---

## Key Features / 核心特性

### Low Overhead / 低开销

Optimized for background execution with minimal CPU and memory footprint.
专为后台运行优化，CPU 与内存占用极低。

### Modular Architecture / 模块化架构

Interface-based design (`ICollector`) enables easy feature extension, such as adding GPU or network monitoring modules.
基于接口（`ICollector`）的设计，使新增 GPU、网络等监控模块变得非常简单。

### Comprehensive Monitoring / 全方位监控

* **CPU**: Global usage percentage.
  **CPU**：整体使用率。
* **Memory**: Usage percentage, committed memory, and available memory.
  **内存**：使用率、已提交内存与可用内存。
* **Process**: Automatically captures the Top N memory-consuming processes.
  **进程**：自动统计内存占用最高的前 N 个进程。
* **Disk**: Disk I/O rate monitoring (Read/Write KB/s) based on PDH, supporting automatic disk discovery and filtering.
  **磁盘**：基于 PDH 的磁盘读写速率监控（KB/s），支持磁盘自动发现与过滤。

### Configurable / 高度可配置

Fully configurable via command-line arguments, including sampling interval, runtime duration, and filter rules.
支持通过命令行参数灵活配置采样间隔、运行时长及过滤条件。

### Structured Logging / 结构化日志

Automatically generates independent log files with timestamps for each run session.
每次运行都会自动生成独立的、带时间戳的日志文件。

---

## Tech Stack / 技术栈

* **Language / 语言**: C++20 (ISO/IEC 14882:2020)
* **Build System / 构建系统**: CMake 3.20+
* **Platform / 平台**: Windows 10 / Windows 11
* **APIs / 使用接口**: Windows API, PSAPI, PDH (Performance Data Helper), ToolHelp32

---

## Build Instructions / 构建指南

Make sure you have a C++ compiler with C++20 support (MSVC recommended) and CMake installed.

请确保已安装支持 C++20 的编译器（推荐 MSVC）以及 CMake。

```bash
# 1. Generate build configuration / 生成构建配置
cmake -B build

# 2. Build the project (Release recommended) / 编译项目（推荐 Release 模式）
cmake --build build --config Release
```

---

## Installation / 安装与下载

### Option 1: Download Binaries (Recommended)

### 方案一：下载可执行文件（推荐）

If you prefer not to build from source, download the latest pre-compiled binary from the Releases page.

如果不想手动编译源码，可直接前往 Releases 页面下载最新版本的 `WinPulse.exe`。

> **Note**: If you encounter missing DLL errors, please install the Visual C++ Redistributable.
>
> **注意**：如果运行时提示缺少 DLL，请安装微软官方的 Visual C++ 运行库。

### Option 2: Build from Source

### 方案二：源码编译

Refer to the [Build Instructions](#build-instructions--构建指南) section above.

---

## Usage / 使用说明

WinPulse is a command-line tool. Run it from PowerShell or CMD.

WinPulse 为命令行工具，请在 PowerShell 或 CMD 中运行。

### 1. Basic Run (Default) / 基础运行（默认）

Monitors CPU, memory, disk, and the Top 5 memory-consuming processes every second.

默认情况下，每秒监控 CPU、内存、磁盘以及内存占用最高的前 5 个进程。

```bash
WinPulse.exe
```

### 2. Custom Configuration / 自定义配置

Example: Monitor only the `C:` drive, update every 500 ms, and show the Top 3 processes.

示例：仅监控 C 盘，每 500 ms 采样一次，显示内存占用最高的前 3 个进程。

```bash
WinPulse.exe --disk "C:" -i 500 -n 3
```

### 3. Run for Fixed Duration / 指定运行时长

Run for 60 seconds and exit automatically. Useful for scripts and automation.

运行 60 秒后自动退出，适合脚本或自动化场景。

```bash
WinPulse.exe -d 60
```

### 4. View Help / 查看帮助

```bash
WinPulse.exe -h
```
