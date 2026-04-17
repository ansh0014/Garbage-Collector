#include <stdlib.h>
#include "allocator/mark_queue.h"

int mq_init(mark_queue_t *q, size_t cap)
{
    if (!q || cap == 0) return 0;
    q->items = (header_t **)calloc(cap, sizeof(header_t *));
    if (!q->items) return 0;
    q->cap = cap;
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    return 1;
}

void mq_destroy(mark_queue_t *q)
{
    if (!q) return;
    free(q->items);
    q->items = NULL;
    q->cap = 0;
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

int mq_push(mark_queue_t *q, header_t *h)
{
    if (!q || !q->items || q->count == q->cap) return 0;
    q->items[q->tail] = h;
    q->tail = (q->tail + 1) % q->cap;
    q->count++;
    return 1;
}

header_t *mq_pop(mark_queue_t *q)
{
    header_t *h;
    if (!q || !q->items || q->count == 0) return NULL;
    h = q->items[q->head];
    q->head = (q->head + 1) % q->cap;
    q->count--;
    return h;
}

int mq_empty(const mark_queue_t *q)
{
    return (!q || q->count == 0);
}