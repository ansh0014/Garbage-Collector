#include <stdint.h>
#ifdef _WIN32
#include <windows.h>
#include <winnt.h>
#else
#include <stdio.h>
#include <assert.h>
#endif

extern uintptr_t stack_bottom;

void GC_init(void)
{
    static int initted = 0;
    if (initted) return;
    initted = 1;

#ifdef _WIN32
    NT_TIB *tib = (NT_TIB *)NtCurrentTeb();
    stack_bottom = (uintptr_t)tib->StackBase;
#else
    FILE *fp = fopen("/proc/self/stat", "r");
    assert(fp != NULL);

    fscanf(fp,
           "%*d %*s %*c %*d %*d %*d %*d %*d %*u "
           "%*lu %*lu %*lu %*lu %*lu %*lu %*ld %*ld "
           "%*ld %*ld %*ld %*ld %*llu %*lu %*ld "
           "%*lu %*lu %*lu %lu",
           &stack_bottom);
    fclose(fp);
#endif
}