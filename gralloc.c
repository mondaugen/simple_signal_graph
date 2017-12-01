/* Allocation that can only grow. */
#include "gralloc.h"


void
gralloc_init(gralloc_t *a, char *cur, char *end)
{
    a->cur = cur;
    a->end = end;
}

size_t
gralloc_sz(void) { return sizeof(gralloc_t); }

void *
gralloc_alloc(gralloc_t *a, size_t sz)
{
    if (a->cur + sz >= a->end) {
        return NULL;
    }
    void *ret = a->cur;
    a->cur += sz;
    return ret;
}

char *
gralloc_parse_line(gralloc_t *a,
            size_t *sz, 
            char_stream_t *cs)
{
    char *pch = a->cur;
    size_t _sz = 0;
    int flg = 1;
    while (flg) {
        if (pch == a->end) {
            return NULL;
        }
        if (mf(cs,get_chr,pch)
                || *pch == '\n') {
            *pch = '\0';
            flg = 0;
        }
        pch++;
        _sz++;
    }
    char *ret = a->cur;
    a->cur = pch;
    *sz =  _sz;
    return ret;
}

