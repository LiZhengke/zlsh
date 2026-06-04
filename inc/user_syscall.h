#ifndef USER_SYSCALL_H
#define USER_SYSCALL_H

#include <stdint.h>

int32_t usys_write(int fd, const void *buf, int len);
int32_t usys_read(int fd, void *buf, int len);
int32_t usys_yield(void);
int32_t usys_delay(uint32_t ticks);
int32_t usys_get_tick_count(void);
int32_t usys_task_exec(const char *path, void* arg);
int32_t usys_task_spawn(const char *path, void* arg);
int32_t usys_task_waitpid(int pid, int *status);
int32_t usys_listdir(const char *path);
int32_t usys_ps(void);
#endif /* USER_SYSCALL_H */
