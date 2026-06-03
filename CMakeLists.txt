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

set(USER_COMMON_SRC
    src/io_console.c
    src/io_syscall.c
    src/user_syscall.c
    src/start.S
)

function(add_zelix_user_program target main_source)
    add_executable(${target}
        ${main_source}
        ${USER_COMMON_SRC}
    )

    target_include_directories(${target}
        PRIVATE
            ${CMAKE_SOURCE_DIR}/inc
    )

    target_link_options(${target} PRIVATE
        -T${CMAKE_SOURCE_DIR}/link.ld
        -nostdlib
        -Wl,--build-id=none
        -Wl,--gc-sections
        -Wl,-Map=${target}.map
    )

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${target}> $<TARGET_FILE_DIR:${target}>/${target}.bin
        COMMENT "Generating binary image for ${target}"
    )

    if(CMAKE_SIZE)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${target}>
            COMMENT "Print size for ${target}"
        )
    else()
        find_program(SIZE_TOOL NAMES size llvm-size)
        if(SIZE_TOOL)
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${SIZE_TOOL} $<TARGET_FILE:${target}>
                COMMENT "Print size for ${target}"
            )
        endif()
    endif()
endfunction()

add_zelix_user_program(init src/init.c)
add_zelix_user_program(sh src/shell.c)
add_zelix_user_program(test src/test.c)
