#include <stdio.h>
#include <stdlib.h>
#include "allocator/allocator.h"
#include "gc/gc.h"
#include "runtime/init.h"

#define N 1000

int main(void)
{
    GC_init();

    int *ptrs[N];
    for (int i = 0; i < N; i++) {
        ptrs[i] = (int *)GC_malloc(sizeof(int));
        if (!ptrs[i]) {
            printf("FAIL: allocation failed at i=%d\n", i);
            return 1;
        }
        *ptrs[i] = i * 10;
    }

    for (int i = 1; i < N; i += 2) {
        ptrs[i] = NULL;
    }

    GC_collect();

    for (int i = 0; i < N; i += 2) {
        if (!ptrs[i]) {
            printf("FAIL: live pointer became NULL at i=%d\n", i);
            return 1;
        }
        if (*ptrs[i] != i * 10) {
            printf("FAIL: live data corrupted at i=%d (got %d expected %d)\n",
                   i, *ptrs[i], i * 10);
            return 1;
        }
    }

    int reuse_ok = 0;
    for (int i = 0; i < N / 2; i++) {
        int *p = (int *)GC_malloc(sizeof(int));
        if (!p) {
            printf("FAIL: post-GC allocation failed at i=%d\n", i);
            return 1;
        }
        *p = 1234 + i;
        reuse_ok++;
    }

    GC_collect();

    for (int i = 0; i < N; i += 2) {
        if (*ptrs[i] != i * 10) {
            printf("FAIL: second-GC data corrupted at i=%d (got %d expected %d)\n",
                   i, *ptrs[i], i * 10);
            return 1;
        }
    }

    printf("PASS: GC stress test completed\n");
    printf("PASS: validated live objects and post-GC allocations (%d)\n", reuse_ok);
    return 0;
}