#include <stdio.h>
#include <assert.h>
#include <stdint.h>

extern uintptr_t stack_bottom;

void GC_init(void)
{
    FILE *fp;
    static int initted = 0;

    if (initted)
        return;

    initted = 1;

    fp = fopen("/proc/self/stat", "r");
    assert(fp != NULL);

    fscanf(fp,
           "%*d %*s %*c %*d %*d %*d %*d %*d %*u "
           "%*lu %*lu %*lu %*lu %*lu %*lu %*ld %*ld "
           "%*ld %*ld %*ld %*ld %*llu %*lu %*ld "
           "%*lu %*lu %*lu %lu",
           &stack_bottom);

    fclose(fp);
}