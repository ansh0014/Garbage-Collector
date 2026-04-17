#ifndef MARK_QUEUE_H
#define MARK_QUEUE_H

#include <stddef.h>
#include "common.h"

typedef struct {
    header_t **items;
    size_t cap;
    size_t head;
    size_t tail;
    size_t count;
} mark_queue_t;

int mq_init(mark_queue_t *q, size_t cap);
void mq_destroy(mark_queue_t *q);
int mq_push(mark_queue_t *q, header_t *h);
header_t *mq_pop(mark_queue_t *q);
int mq_empty(const mark_queue_t *q);

#endif