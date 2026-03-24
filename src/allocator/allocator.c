#include <unistd.h>
#include <stdio.h>
#include "allocator.h"

#define MIN_ALLOC_SIZE 4096

static header_t base;
header_t *freep = &base;
header_t *usedp = NULL;

void add_to_free_list(header_t *bp)
{
    header_t *p;

    for (p = freep; !(bp > p && bp < p->next); p = p->next)
        if (p >= p->next && (bp > p || bp < p->next))
            break;

    if (bp + bp->size == p->next) {
        bp->size += p->next->size;
        bp->next = p->next->next;
    } else {
        bp->next = p->next;
    }

    if (p + p->size == bp) {
        p->size += bp->size;
        p->next = bp->next;
    } else {
        p->next = bp;
    }

    freep = p;
}

static header_t *morecore(size_t num_units)
{
    void *vp;
    header_t *up;

    if (num_units < MIN_ALLOC_SIZE)
        num_units = MIN_ALLOC_SIZE / sizeof(header_t);

    vp = sbrk(num_units * sizeof(header_t));
    if (vp == (void *) -1)
        return NULL;

    up = (header_t *) vp;
    up->size = num_units;

    add_to_free_list(up);
    return freep;
}

void* GC_malloc(size_t alloc_size)
{
    size_t nunits;
    header_t *p, *prevp;

    nunits = (alloc_size + sizeof(header_t) - 1) / sizeof(header_t) + 1;

    if (freep == NULL) {
        base.next = freep = &base;
        base.size = 0;
    }
    prevp = freep;
    for (p = prevp->next;; prevp = p, p = p->next) {
        if (p->size >= nunits) {
            if (p->size == nunits)
                prevp->next = p->next;
            else {
                p->size -= nunits;
                p += p->size;
                p->size = nunits;
            }

            freep = prevp;

            if (usedp == NULL)
                usedp = p->next = p;
            else {
                p->next = usedp->next;
                usedp->next = p;
            }

            return (void *)(p + 1);
        }

        if (p == freep) {
            p = morecore(nunits);
            if (p == NULL)
                return NULL;
        }
    }
}