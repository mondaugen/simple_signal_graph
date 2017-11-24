#include "rbn.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MEMBLK 4096

typedef struct val_t val_t;
struct val_t {
    char *s;
    val_t *next;
};

void *
val_append(void *_sub, void *_obj)
{
    val_t *sub = _sub, *obj = _obj;
    sub->next = obj;
    return (void*)sub;
}

size_t
val_list_len(val_t *v)
{
    size_t n = 0;
    while (v) {
        n++;
        v = v->next;
    }
    return n;
}

int
val_cmp(void *_a, void *_b)
{
    val_t *a = _a, *b = _b;
    return strcmp(a->s,b->s);
}

typedef struct {
    char *start;
    char *end;
    char *cur;
} alloc_t;

void *
alloc_alloc(size_t sz, void *_a)
{
    alloc_t *a = _a;
    if (a->cur + sz >= a->end) {
        return NULL;
    }
    char *ret = a->cur;
    a->cur += sz;
    return (void*)ret;
}

int
main(void)
{
    char *strs[] = {
        "aaa",
        "aab",
        "bcd",
        "aab",
        "123",
        "bcd",
        0
    };
    alloc_t *alc = malloc(sizeof(alloc_t)+MEMBLK);
    alc->start = (char*)(alc+1);
    alc->cur = alc->start;
    alc->end = alc->start+MEMBLK;
    char **ptr= strs;
    rbn_t *rbn = NULL;
    while (*ptr) {
        val_t *v = alloc_alloc(sizeof(val_t), alc);
        v->s = *ptr;
        v->next = NULL;
        rbn = rbn_insert_append(rbn,
                v,alloc_alloc,alc,val_cmp,val_append);
        ptr++;
    }
    ptr = strs;
    while (*ptr) {
        val_t dum = {*ptr,NULL};
        val_t *v = rbn_search(rbn,&dum,val_cmp);
        if (v) {
            printf("%s, len: %zu\n",v->s,val_list_len(v));
        }
        ptr++;
    }
    free(alc);
    return 0;
}
