#include <stddef.h>
#include <io.h>
#include "user_syscall.h"

int main(void)
{
    while (1) {
        int pid = usys_task_spawn("sh", NULL);

        int status;

        if (pid < 0) {
            printf("spawn shell failed: %d\n", pid);
            return pid;
        }

        if (usys_task_waitpid(pid, &status) < 0) {
            printf("waitpid failed for shell: %d\n", pid);
            continue;
        }

        printf("shell exited: %d\n", status);
    }
}
