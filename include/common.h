#ifndef COMMON_H
#define COMMON_H
#include <stddef.h>
#include <stdint.h>
typedef struct header {
    unsigned int size;
    struct header *next;
} header_t;
#define UNTAG(p) (((uintptr_t)(p)) & 0xfffffffc)
#endif 