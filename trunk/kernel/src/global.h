#ifndef _GLOBAL_H_
#define _GLOBAL_H_


#define KERNEL_CALL //__attribute__((cdecl))

#define NULL        0

#define MEGABYTE    1048576

typedef unsigned char       byte_t;     // 8bit
typedef unsigned short      word_t;     // 16bit
typedef unsigned int        dword_t;    // 32bit
typedef unsigned long long  qword_t;    // 64bit

typedef unsigned int        size_t;     // depends on cpu architecture
typedef void*               pointer_t;  // depends on cpu architecture

typedef enum {
        FALSE,
        TRUE
    }   bool_t;

//


#endif  // _GLOBAL_H_
