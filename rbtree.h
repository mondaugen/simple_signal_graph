#ifndef RBTREE_H
#define RBTREE_H 

typedef struct rbtree_t rbtree_t;

rbtree_t *
rbtree_insert(rbtree_t *root, rbtree_t *n, int (*cmp)(void*,void*));

rbtree_t *
rbtree_new(void *data);

void
rbtree_free(rbtree_t *r);

void
rbtree_eval(rbtree_t *r, void (*fun)(int,void*,void*), void *aux);
#endif /* RBTREE_H */
