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

#endif /* RBN_H */
