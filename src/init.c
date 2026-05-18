
#include <io.h>
#include "user_syscall.h"

int main(void)
{
    while (1) {
        int pid = spawn("/bin/sh");

        int status;

        waitpid(pid, &status);

        printf("shell exited: %d\n", status);
    }
}
