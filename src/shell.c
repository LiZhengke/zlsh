#include <stdint.h>
#include <stddef.h>
#include "io.h"
#include "user_syscall.h"

#define MAX_CMD_LEN 128
#define MAX_ARGC 8

static inline uint16_t get_cpl(void)
{
    uint16_t cs;
    __asm__ volatile ("mov %%cs, %0" : "=r"(cs));
    return cs & 0x3;
}

static char cmdline[MAX_CMD_LEN];
static char *argv[MAX_ARGC];

static inline void cpu_relax(void)
{
    usys_yield();
}

static int str_eq(const char *lhs, const char *rhs)
{
    if ((lhs == NULL) || (rhs == NULL)) {
        return 0;
    }

    while ((*lhs != '\0') && (*rhs != '\0')) {
        if (*lhs != *rhs) {
            return 0;
        }
        lhs++;
        rhs++;
    }

    return (*lhs == '\0') && (*rhs == '\0');
}

static void read_line(char *buf, uint32_t buf_len)
{
    uint32_t i = 0;

    if ((buf == NULL) || (buf_len == 0U)) {
        return;
    }

    for (;;) {
        char ch = '\0';
        int32_t n = usys_read(0, &ch, 1);

        if (n <= 0) {
            cpu_relax();
            continue;
        }

        if (ch == '\r' || ch == '\n') {
            printf("\n");
            break;
        }

        if ((ch == '\b' || ch == 0x7f) && i > 0U) {
            i--;
            printf("\b \b");
            continue;
        }

        if (ch >= ' ' && ch <= '~' && i + 1U < buf_len) {
            buf[i++] = ch;
            printf("%c", ch);
        }
    }

    buf[i] = '\0';
}

static int parse_cmdline(char *buf, char **out_argv)
{
    int argc = 0;

    while ((*buf != '\0') && (argc < MAX_ARGC)) {
        while (*buf == ' ') {
            buf++;
        }
        if (*buf == '\0') {
            break;
        }

        out_argv[argc++] = buf;

        while ((*buf != '\0') && (*buf != ' ')) {
            buf++;
        }
        if (*buf != '\0') {
            *buf = '\0';
            buf++;
        }
    }

    out_argv[argc] = NULL;
    return argc;
}

static void handle_command(int argc, char **args)
{
    if (str_eq(args[0], "exit")) {
        return;
    }

    if (str_eq(args[0], "ls")) {
        /* TODO: add ls syscall integration. */
        return;
    }

    if (str_eq(args[0], "run") && (argc > 1)) {
        /* TODO: add exec syscall integration. */
        usys_task_exec(args[1]);
        return;
    }

    printf("unknown command\n");
}

int zlsh_main(void)
{
    printf("[shell] tick=%lu cpl=%d\n", (unsigned long)usys_get_tick_count(), get_cpl());

    for (;;) {
        printf("sh> ");
        read_line(cmdline, (uint32_t)sizeof(cmdline));

        int argc = parse_cmdline(cmdline, argv);
        if (argc == 0) {
            continue;
        }

        if (str_eq(argv[0], "exit")) {
            break;
        }

        handle_command(argc, argv);
    }

    return 0;
}
