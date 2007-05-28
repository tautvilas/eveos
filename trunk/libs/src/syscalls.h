#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

extern void __attribute__((cdecl)) sys_write(const char* apStr, int len);

#endif // _SYSCALLS_H_
