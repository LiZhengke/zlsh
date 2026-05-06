#ifndef USER_SYSCALL_H
#define USER_SYSCALL_H

#include <stdint.h>

int32_t usys_write(int fd, const void *buf, int len);
int32_t usys_read(int fd, void *buf, int len);
int32_t usys_yield(void);
int32_t usys_delay(uint32_t ticks);
int32_t usys_get_tick_count(void);
int32_t usys_task_exec(void* arg);
#endif /* USER_SYSCALL_H */
