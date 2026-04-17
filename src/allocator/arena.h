#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include "common.h"

int arena_init(size_t bytes);
void arena_destroy(void);
header_t *arena_alloc_units(size_t nunits);
size_t arena_bytes_used(void);
size_t arena_bytes_capacity(void);

#endif