cmake_minimum_required(VERSION 3.10)
project(MySTM32Project)

# 使用交叉编译工具链
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-gcc-toolchain.cmake)

# 设置 C 标准
set(CMAKE_C_STANDARD 99)

# 添加头文件搜索路径
include_directories(
        ./Start
        ./Library
        ./User
        ./System
        ./Hardware
)

add_definitions(-DSTM32F10X_MD)
add_definitions(-DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD)

file(GLOB HARDWARE_SRC "Hardware/*.c" "Hardware/*.h")
file(GLOB LIBRARY_SRC "Library/*.c" "Library/*.h")
file(GLOB START_SRC "Start/*.c" "Start/*.h")
file(GLOB SYSTEM_SRC "System/*.c" "System/*.h")
file(GLOB USER_SRC "User/*.c" "User/*.h")

# 将所有源文件组合
set(SOURCE_FILES
        ${HARDWARE_SRC}
        ${LIBRARY_SRC}
        ${START_SRC}
        ${SYSTEM_SRC}
        ${USER_SRC}
)

# 创建可执行文件
add_executable(Test ${SOURCE_FILES})
