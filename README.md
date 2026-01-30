# WinPulse

**Repository:** [https://github.com/2017-z/WinPulse](https://github.com/2017-z/WinPulse)

## Introduction / 简介

WinPulse is a lightweight, non-intrusive system performance monitoring tool designed for the Windows platform. Developed with Modern C++ (C++20), it focuses on background logging with minimal resource overhead. It adheres to strict engineering standards, utilizing RAII for resource management and a modular architecture for high extensibility.

WinPulse 是一款专为 Windows 平台设计的轻量级、低侵入式系统性能监控工具。该项目采用现代 C++ (C++20) 开发，专注于极低资源开销下的后台日志记录。项目遵循严格的工程规范，利用 RAII 进行资源管理，并采用模块化架构以确保高可扩展性。

## Key Features / 核心特性

* **Low Overhead**: Optimized for background execution with minimal CPU and memory footprint.
    * **低开销**：专为后台运行优化，极低的 CPU 和内存占用。
* **Modular Architecture**: Interface-based design (`ICollector`) allows for easy extension (e.g., adding GPU or Network monitoring).
    * **模块化架构**：基于接口的设计 (`ICollector`) 使得扩展功能（如新增 GPU 或网络监控）变得异常简单。
* **Comprehensive Monitoring**:
    * **CPU**: Global usage percentage.
    * **Memory**: Usage percentage, committed/available memory.
    * **Process**: Automatic capture of Top N memory-consuming processes.
    * **Disk**: I/O rate monitoring (Read/Write KB/s) based on PDH, supporting auto-discovery and filtering.
    * **全方位监控**：涵盖 CPU 总使用率、内存状态、Top N 内存占用进程、以及基于 PDH 的磁盘 I/O 速率监控（支持自动发现与过滤）。
* **Configurable**: Fully customizable via command-line arguments (sampling interval, duration, filters).
    * **高度可配置**：支持通过命令行参数完全自定义采样频率、运行时长和过滤规则。
* **Structured Logging**: Generates independent, timestamped log files for each session.
    * **结构化日志**：为每次运行自动生成独立的、带时间戳的日志文件。

## Tech Stack / 技术栈

* **Language**: C++20 (Standard ISO/IEC 14882:2020)
* **Build System**: CMake 3.20+
* **Platform**: Windows 10 / 11
* **APIs**: Windows API, PSAPI, PDH (Performance Data Helper), ToolHelp32

## Build Instructions / 构建指南

Ensure you have a C++ compiler supporting C++20 (MSVC recommended) and CMake installed.

确保您的环境已安装支持 C++20 的编译器（推荐 MSVC）和 CMake。

```bash
# 1. Generate build configuration
# 1. 生成构建配置
cmake -B build

# 2. Build the project (Release mode recommended for performance)
# 2. 编译项目 (推荐使用 Release 模式以获得最佳性能)
cmake --build build --config Release