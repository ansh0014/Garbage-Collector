#include <stddef.h>
#include <stdint.h>
#include "allocator/arena.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

static unsigned char *g_base = NULL;
static size_t g_cap = 0;
static size_t g_used = 0;

int arena_init(size_t bytes)
{
    if (g_base) return 1;
    if (bytes == 0) bytes = 1024 * 1024;

#ifdef _WIN32
    g_base = (unsigned char *)VirtualAlloc(NULL, bytes, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!g_base) return 0;
#else
    g_base = (unsigned char *)sbrk(bytes);
    if (g_base == (void *)-1) {
        g_base = NULL;
        return 0;
    }
#endif

    g_cap = bytes;
    g_used = 0;
    return 1;
}

void arena_destroy(void)
{
    if (!g_base) return;
#ifdef _WIN32
    VirtualFree(g_base, 0, MEM_RELEASE);
#endif
    g_base = NULL;
    g_cap = 0;
    g_used = 0;
}

header_t *arena_alloc_units(size_t nunits)
{
    size_t bytes;
    uintptr_t p;
    size_t align = sizeof(void *);

    if (!g_base && !arena_init(1024 * 1024)) return NULL;

    bytes = nunits * sizeof(header_t);

    p = (uintptr_t)(g_base + g_used);
    p = (p + (align - 1)) & ~(uintptr_t)(align - 1);

    if ((size_t)(p - (uintptr_t)g_base) + bytes > g_cap) return NULL;

    g_used = (size_t)(p - (uintptr_t)g_base) + bytes;
    return (header_t *)p;
}

size_t arena_bytes_used(void)
{
    return g_used;
}

size_t arena_bytes_capacity(void)
{
    return g_cap;
}