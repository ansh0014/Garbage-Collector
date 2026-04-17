#ifndef COMPACT_H
#define COMPACT_H

#include <stddef.h>

int compact_init(size_t semispace_bytes);
void compact_shutdown(void);
size_t compact_young_once(void);

#endif