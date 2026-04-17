#include <stdlib.h>
#include "allocator/barrier.h"

typedef struct {
    header_t *owner;
    void *value;
} rem_entry_t;

static rem_entry_t *g_rs = NULL;
static size_t g_cap = 0;
static size_t g_cnt = 0;

int barrier_init(size_t cap)
{
    if (g_rs) return 1;
    if (cap == 0) cap = 4096;

    g_rs = (rem_entry_t *)calloc(cap, sizeof(rem_entry_t));
    if (!g_rs) return 0;

    g_cap = cap;
    g_cnt = 0;
    return 1;
}

void barrier_shutdown(void)
{
    free(g_rs);
    g_rs = NULL;
    g_cap = 0;
    g_cnt = 0;
}

void barrier_record(header_t *owner, void *value)
{
    if (!g_rs || g_cnt == g_cap) return;
    g_rs[g_cnt].owner = owner;
    g_rs[g_cnt].value = value;
    g_cnt++;
}

size_t barrier_count(void)
{
    return g_cnt;
}