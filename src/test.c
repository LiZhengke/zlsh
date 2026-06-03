#include <stdint.h>
#include "io.h"
#include "user_syscall.h"

int main(void)
{
    int i;

    printf("[test] start tick=%lu\n",
           (unsigned long)usys_get_tick_count());

    for (i = 1; i <= 3; i++) {
        printf("[test] step %d/3\n", i);
        usys_delay(20);
    }

    printf("[test] done\n");
    return 42;
}
