#include "user_syscall.h"

#define SYS_WRITE_NR 1
#define SYS_DELAY_NR 2
#define SYS_TICK_COUNT_NR 11
#define SYS_TASK_EXEC_NR 13
#define SYSINT 0x30

#define _STR(x) #x
#define STR(x) _STR(x)

int32_t usys_write(int fd, const void *buf, int len)
{
    int32_t ret;

    __asm__ volatile (
        "int $" STR(SYSINT)
        : "=a"(ret)
        : "a"(SYS_WRITE_NR),
          "b"(fd),
          "c"(buf),
          "d"(len)
        : "memory"
    );

    return ret;
}

int32_t usys_delay(uint32_t ticks)
{
    int32_t ret;

    __asm__ volatile (
        "int $" STR(SYSINT)
        : "=a"(ret)
        : "a"(SYS_DELAY_NR),
          "b"(ticks)
        : "memory"
    );

    return ret;
}

int32_t usys_get_tick_count(void)
{
    int32_t ret;

    __asm__ volatile (
        "int $" STR(SYSINT)
        : "=a"(ret)
        : "a"(SYS_TICK_COUNT_NR)
        : "memory"
    );

    return ret;
}

int32_t usys_task_exec(void* arg)
{
    int32_t ret;

    __asm__ volatile (
        "int $" STR(SYSINT)
        : "=a"(ret)
        : "a"(SYS_TASK_EXEC_NR),
          "b"(arg)
        : "memory"
    );

    return ret;
}
