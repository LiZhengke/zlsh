#include "io.h"
#include "user_syscall.h"

int write(int fd, const void *str, int len)
{
    return (int)usys_write(fd, str, len);
}
