#ifndef IO_H
#define IO_H

/* Minimal freestanding console I/O API. */
int write(int fd, const void *str, int len);
void puts(const char *str);
int printf(const char *restrict format, ...);

#endif /* IO_H */
