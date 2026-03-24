#include <stdio.h>
#include "allocator/allocator.h"
#include "gc/gc.h"
#include "runtime/init.h"

int main()
{
    GC_init();

    int *a = (int *)GC_malloc(sizeof(int));
    *a = 42;

    printf("Before GC: %d\n", *a);

    GC_collect();  

    a = NULL;

    GC_collect();  

    printf("GC done\n");

    return 0;
}