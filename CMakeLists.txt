cmake_minimum_required(VERSION 3.20)
project(WinPulse VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/utf-8)
endif() 
# 强制使用UTF-8编码，否则会中文注释会乱码，改注释太麻烦了

# 自动扫描源文件 (工程化实践)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 包含头文件路径
include_directories(src)

add_executable(WinPulse ${SOURCES})

# 链接 Windows 系统库
target_link_libraries(WinPulse PRIVATE Psapi)

# 新增了 Pdh 库用于磁盘性能计数器
target_link_libraries(WinPulse PRIVATE Psapi Pdh) 