#ifndef COMMON_H
#define COMMON_H

#define va_list  __builtin_va_list
#define va_start __builtin_va_start
#define va_end   __builtin_va_end
#define va_arg   __builtin_va_arg

#define NULL ((void*)0)

#define true 1
#define false 0

typedef int bool;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef uint32_t size_t;
typedef uint32_t paddr_t;
typedef uint32_t vaddr_t;

// Get rid of annoying clang warning
#if __SIZEOF_POINTER__ == 4
typedef unsigned int uintptr_t;  // 32-bit pointer
#elif __SIZEOF_POINTER__ == 8
typedef unsigned long uintptr_t; // 64-bit pointer
#else
#error "Unsupported pointer size"
#endif

/*
* stdio.h
*/
void printf(const char *, ...);

/*
* string.h
*/
size_t strlen(const char *str);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *dst, const char *src);


/*
* stdlib.h
*/

void memset(void *buf, char c, size_t n);
void memcpy(void *dst, const void *src, unsigned long n);

#endif
