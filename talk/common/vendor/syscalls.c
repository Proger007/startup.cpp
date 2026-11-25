/*
 * Minimal newlib system call stubs for the vendor-startup stages, the role syscalls.c plays in an
 * STM32CubeIDE project. Without them, newlib's nosys stubs are linked and print
 * "warning: _close is not implemented" for every build, which would spoil the "clean build" screenshot.
 */
#include <errno.h>

int _close(int fd) { (void)fd; errno = EBADF; return -1; }
int _lseek(int fd, int offset, int whence) { (void)fd; (void)offset; (void)whence; errno = ESPIPE; return -1; }
int _read(int fd, char *buf, int len) { (void)fd; (void)buf; (void)len; errno = EBADF; return -1; }
int _write(int fd, const char *buf, int len) { (void)fd; (void)buf; (void)len; errno = EBADF; return -1; }
