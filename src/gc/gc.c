#include <stdint.h>
#include "common.h"
#include "allocator/allocator.h"

uintptr_t stack_bottom;

static void scan_region(uintptr_t *sp, uintptr_t *end)
{
    header_t *bp;

    for (; sp < end; sp++) {
        uintptr_t v = *sp;

        if (!usedp) continue;

        bp = usedp;
        do {
            header_t *real = UNTAG(bp);

            if ((uintptr_t)(real + 1) <= v &&
                (uintptr_t)(real + 1 + real->size) > v) {
                bp->next = MARK(bp->next);
                break;
            }

            bp = UNTAG(bp->next);
        } while (bp != usedp);
    }
}

static void scan_heap(void)
{
    header_t *bp, *up;
    uintptr_t *vp;

    if (!usedp) return;

    for (bp = UNTAG(usedp->next); bp != usedp; bp = UNTAG(bp->next)) {
        if (!IS_MARKED(bp->next))
            continue;

        for (vp = (uintptr_t *)(bp + 1);
             vp < (uintptr_t *)(bp + bp->size + 1);
             vp++) {

            uintptr_t v = *vp;
            up = usedp;

            do {
                header_t *real = UNTAG(up);

                if (real != bp &&
                    (uintptr_t)(real + 1) <= v &&
                    (uintptr_t)(real + 1 + real->size) > v) {
                    up->next = MARK(up->next);
                    break;
                }

                up = UNTAG(up->next);
            } while (up != usedp);
        }
    }
}

void GC_collect(void)
{
    header_t *p, *prevp, *tp;
    uintptr_t stack_top;

    if (!usedp)
        return;


    volatile uintptr_t stack_anchor = 0;
    stack_top = (uintptr_t)&stack_anchor;

    if (stack_top < stack_bottom)
        scan_region((uintptr_t *)stack_top, (uintptr_t *)stack_bottom);
    else
        scan_region((uintptr_t *)stack_bottom, (uintptr_t *)stack_top);

    scan_heap();

    for (prevp = usedp, p = UNTAG(usedp->next); ; prevp = p, p = UNTAG(p->next)) {

        if (!IS_MARKED(p->next)) {
            tp = p;
            p = UNTAG(p->next);

            add_to_free_list(tp);

            if (usedp == tp) {
                usedp = NULL;
                break;
            }

            prevp->next = p;
            continue;
        }

        p->next = UNTAG(p->next);

        if (p == usedp)
            break;
    }
}