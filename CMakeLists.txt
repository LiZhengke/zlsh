cmake_minimum_required(VERSION 3.16)
include(cmake/zelix-target-defaults.cmake)

# ========= 项目定义 =========
project(zlsh LANGUAGES C CXX ASM)

# ========= 架构/工具链 =========
# 如果是交叉编译，建议用 toolchain file（更规范）
# cmake -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake ..

# ========= 编译标准 =========
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# freestanding 必须关闭这些
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

# ========= 输出 =========
set(TARGET zlsh)

# ========= 源文件 =========
file(GLOB_RECURSE SRC_C   src/*.c)
file(GLOB_RECURSE SRC_CPP src/*.cpp)
file(GLOB_RECURSE SRC_ASM src/*.S)

add_executable(${TARGET}
    ${SRC_C}
    ${SRC_CPP}
    ${SRC_ASM}
)

# ========= 头文件 =========
target_include_directories(${TARGET}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/inc
)

# ========= 链接选项 =========
target_link_options(${TARGET} PRIVATE
    -T${CMAKE_SOURCE_DIR}/link.ld
    -nostdlib
    -Wl,--build-id=none
    -Wl,--gc-sections
    -Wl,-Map=${TARGET}.map
)

# ========= 生成二进制 =========
add_custom_command(TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET}> $<TARGET_FILE_DIR:${TARGET}>/${TARGET}.bin
    COMMENT "Generating binary image"
)

# ========= size 信息 =========
if(CMAKE_SIZE)
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${TARGET}>
        COMMENT "Print size:"
    )
else()
    find_program(SIZE_TOOL NAMES size llvm-size)
    if(SIZE_TOOL)
        add_custom_command(TARGET ${TARGET} POST_BUILD
            COMMAND ${SIZE_TOOL} $<TARGET_FILE:${TARGET}>
            COMMENT "Print size"
        )
    endif()
endif()
