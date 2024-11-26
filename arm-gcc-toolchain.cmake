# 设置工具链路径
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# 设置交叉编译器
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-as)

# 设置编译选项
set(CMAKE_C_FLAGS "-mcpu=cortex-m3 -mthumb -g -O0 -ffunction-sections -fdata-sections")
set(CMAKE_EXE_LINKER_FLAGS "-mcpu=cortex-m3 -mthumb -specs=nosys.specs -Wl,--gc-sections")

# 禁用默认的编译器检查
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)