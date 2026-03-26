#ifndef COMMON_H
#define COMMON_H
#include <stddef.h>
#include <stdint.h>
typedef struct header {
    unsigned int size;
    struct header *next;
} header_t;
#define MARK_BIT ((uintptr_t)1)
#define MARK(p) ((header_t *)((uintptr_t)(p) | MARK_BIT))
#define IS_MARKED(p) ((((uintptr_t)(p)) & MARK_BIT) != 0)
#define UNTAG(p) ((header_t *)((uintptr_t)(p) & ~MARK_BIT))
#endif