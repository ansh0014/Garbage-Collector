#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>
#include "common.h"

void* GC_malloc(size_t size);
void add_to_free_list(header_t *bp);

extern header_t *usedp;
extern header_t *freep;

#endif