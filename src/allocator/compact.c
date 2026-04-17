#include "allocator/compact.h"

static size_t g_semispace = 0;
static int g_ready = 0;

int compact_init(size_t semispace_bytes)
{
    g_semispace = semispace_bytes;
    g_ready = 1;
    return 1;
}

void compact_shutdown(void)
{
    g_semispace = 0;
    g_ready = 0;
}

size_t compact_young_once(void)
{
    if (!g_ready) return 0;
    return 0;
}