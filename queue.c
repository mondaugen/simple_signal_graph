#include <stdlib.h>
#include "queue.h"

struct q_t {
    /* sz is a power of 2 - 1, so it also serves as a mask */
    size_t sz;
/* The current size. */
    size_t csz;
/* Where to pop from. */
    size_t h;
/* Where to push to. */
    size_t t;
/* Data in the queue. */
    void **d;
};

/* Create queue with size >= sz. */
q_t *
q_new(size_t sz)
{
    if (sz == 0) { return NULL; }
    size_t _sz = 1;
    while (_sz <= sz) { _sz = _sz << 1; }
    q_t *ret = malloc(sizeof(q_t)+sizeof(void*)*_sz);
    if (!ret) { return NULL; }
    ret->sz = _sz - 1;
    ret->d = (void**)(ret+1);
    ret->csz = 0;
    ret->t = 0;
    ret->h = 0;
    return ret;
}

void
q_free(q_t *q)
{
    free(q);
}

/* Returns non-zero if no room, 0 otherwise. */
int
q_push(q_t *q, void *d)
{
    if (q->csz == q->sz) { return 1; }
    q->d[q->t] = d;
    q->t = (q->t + 1) & q->sz;
    q->csz++;
    return 0;
}

/* Returns NULL if empty or if stored item was NULL, haha. */
void *
q_pop(q_t *q)
{
    if (q->csz == 0) { return NULL; }
    void *ret = q->d[q->h];
    q->h = (q->h + 1) & q->sz;
    q->csz--;
    return ret;
}

