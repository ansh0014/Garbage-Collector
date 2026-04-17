#ifndef BARRIER_H
#define BARRIER_H

#include <stddef.h>
#include "common.h"

int barrier_init(size_t cap);
void barrier_shutdown(void);
void barrier_record(header_t *owner, void *value);
size_t barrier_count(void);

#endif