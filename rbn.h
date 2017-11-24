#ifndef RBN_H
#define RBN_H 

#include <stddef.h>

typedef struct rbn_t rbn_t;

typedef enum { rbni_LEFT, rbni_RIGHT, rbni_ROOT } rbni_branch_t;

typedef struct {
    rbn_t *node;
    size_t idx; /* Depth of node */ 
    void *data; /* Data with node */
    void *aux; /* Auxiliary data passed to function */
    /* Which branch of the parent it is on */
    rbni_branch_t branch;
} rbni_t;

void rbn_free(rbn_t *r);
rbn_t * rbn_new(void *d);
rbn_t * rbn_insert(rbn_t *t, rbn_t *n, int (*cmp)(void *,void*));
void * rbn_get_data(rbn_t *r);
rbn_t * rbn_tree_from_array(void **d, size_t len);
size_t rbn_sz(void);
void rbn_init(rbn_t *r, void *d);
rbn_t *
rbn_insert_append(rbn_t *t, 
        void *d, 
        void *(*alloc)(size_t,void*),
        void *alloc_aux,
        int (*cmp)(void *,void*),
        void *(*append)(void *sub, void *obj));
void *
rbn_search(rbn_t *t, void *d,int (*cmp)(void*,void*));

#endif /* RBN_H */
