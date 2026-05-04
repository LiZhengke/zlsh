#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include "io.h"

#define STDOUT_FD 1

static void print_char(char c)
{
    write(STDOUT_FD, &c, 1);
}

static void print_str(const char *s)
{
    if (s == NULL) {
        s = "(null)";
    }

    while (*s != '\0') {
        print_char(*s);
        s++;
    }
}

static void print_signed_long(long x)
{
    char buf[32];
    int i = 0;

    if (x == 0) {
        print_char('0');
        return;
    }

    if (x < 0) {
        print_char('-');
        x = -x;
    }

    while (x > 0) {
        buf[i++] = (char)('0' + (x % 10));
        x /= 10;
    }

    while (i > 0) {
        i--;
        print_char(buf[i]);
    }
}

static void print_unsigned_long(unsigned long x)
{
    char buf[32];
    int i = 0;

    if (x == 0UL) {
        print_char('0');
        return;
    }

    while (x > 0UL) {
        buf[i++] = (char)('0' + (x % 10UL));
        x /= 10UL;
    }

    while (i > 0) {
        i--;
        print_char(buf[i]);
    }
}

static void print_hex(unsigned int x, unsigned int width, int zeroPad, int uppercase, int prefix)
{
    char buf[16];
    int i = 0;
    char padChar = zeroPad ? '0' : ' ';

    if (x == 0) {
        buf[i++] = '0';
    }
    else {
        while (x > 0) {
            int d = x & 0xF;
            buf[i++] = (char)(d < 10 ? '0' + d : (uppercase ? 'A' : 'a') + d - 10);
            x >>= 4;
        }
    }

    while ((unsigned int)i < width) {
        buf[i++] = padChar;
    }

    if (prefix != 0) {
        print_str("0x");
    }

    while (i > 0) {
        i--;
        print_char(buf[i]);
    }
}

void puts(const char *str)
{
    if (str == NULL) {
        str = "(null)";
    }

    while (*str != '\0') {
        write(STDOUT_FD, str, 1);
        str++;
    }
    write(STDOUT_FD, "\n", 1);
}

int printf(const char *restrict format, ...)
{
    va_list ap;
    int written = 0;

    va_start(ap, format);

    while (*format) {
        if (*format == '%') {
            unsigned int width = 0;
            int zeroPad = 0;
            int lengthLong = 0;

            format++;

            if (*format == '0') {
                zeroPad = 1;
                format++;
            }

            while ((*format >= '0') && (*format <= '9')) {
                width = (width * 10U) + (unsigned int)(*format - '0');
                format++;
            }

            if (*format == 'l') {
                lengthLong = 1;
                format++;
            }

            switch (*format) {
                case 's':
                    print_str(va_arg(ap, char *));
                    break;
                case 'd':
                    if (lengthLong != 0) {
                        print_signed_long(va_arg(ap, long));
                    }
                    else {
                        print_signed_long((long)va_arg(ap, int));
                    }
                    break;
                case 'u':
                    if (lengthLong != 0) {
                        print_unsigned_long(va_arg(ap, unsigned long));
                    }
                    else {
                        print_unsigned_long((unsigned long)va_arg(ap, unsigned int));
                    }
                    break;
                case 'x':
                    print_hex(va_arg(ap, unsigned int), width, zeroPad, 0, 1);
                    break;
                case 'X':
                    print_hex(va_arg(ap, unsigned int), width, zeroPad, 1, 0);
                    break;
                case 'p':
                    print_hex((unsigned int)(uintptr_t)va_arg(ap, void *), (unsigned int)(sizeof(uintptr_t) * 2U), 1, 0, 1);
                    break;
                case '%':
                    print_char('%');
                    break;
                default:
                    print_char('%');
                    if (*format != '\0') {
                        print_char(*format);
                    }
            }
        }
        else {
            print_char(*format);
        }

        written++;
        format++;
    }

    va_end(ap);

    return written;
}
