#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

extern void __attribute__((cdecl)) sys_write(const char* apStr, int aLen);
extern int __attribute__((cdecl)) sys_read(int aFd, char* apBuffer, int aLen);

#endif // _SYSCALLS_H_
