#include "rbtree.h"
#include <stddef.h>
#include <stdlib.h>
#if defined(DEBUG)
#include <assert.h>
#endif 

typedef enum {
    __BLACK,
    __RED,
} _color_t;

struct rbtree_t {
    rbtree_t *parent;
    rbtree_t *left;
    rbtree_t *right;
    _color_t color;
    void *data;
};

static rbtree_t *
_grandparent(rbtree_t *r)
{
#if defined(DEBUG)
    assert(r);
#endif
    rbtree_t *p;
    if ((p = r->parent)) {
        return p->parent;
    }
    return NULL;
}

static rbtree_t *
_sibling(rbtree_t *r)
{
#if defined(DEBUG)
    assert(r);
#endif
    if (!r->parent) {
        return NULL;
    }
    if (r->parent->left == r) {
        return r->parent->right;
    }
    return r->parent->left;
}

static rbtree_t *
_uncle(rbtree_t *r)
{
#if defined(DEBUG)
    assert(r);
#endif
    rbtree_t *p = r->parent;
    if (!_grandparent(r)) {
        return NULL;
    }
    return _sibling(p);
}

static void
_rotate_right(rbtree_t *n)
{
#if defined(DEBUG)
    assert(n);
    assert(n->left);
#endif
    n->parent->right = n->left;
    n->left->parent = n->parent;
    rbtree_t *oldr = n->left->right;
    n->left->right = n;
    n->parent = n->left;
    n->left = oldr;
    oldr->parent = n;
}

static void
_rotate_left(rbtree_t *n)
{
#if defined(DEBUG)
    assert(n);
    assert(n->right);
#endif
    n->parent->left = n->right;
    n->right->parent = n->parent;
    rbtree_t *oldl = n->right->left;
    n->right->left = n;
    n->parent = n->right;
    n->right = oldl;
    oldl->parent = n;
}

static void
_insert_repair_tree(rbtree_t *n)
{
#if defined(DEBUG)
    assert(n);
#endif
    if (!n->parent) {
        n->color = __BLACK;
        return;
    }
    if (n->parent->color == __BLACK) {
        return;
    }
    // There should always be an uncle as the tree is balanced just before this
    // insertion.
    if (_uncle(n) && _uncle(n)->color == __RED) {
        n->parent->color = __BLACK;
        _uncle(n)->color = __BLACK;
        rbtree_t *g = _grandparent(n);
        g->color = __RED;
        _insert_repair_tree(g);
        return;
    }
    rbtree_t *p = n->parent;
    rbtree_t *g = _grandparent(n);
    if (g->left->right == n) {
        _rotate_left(p);
        n = n->left;
    } else if (g->right->left == n) {
        _rotate_right(p);
        n = n->right;
    }
    p = n->parent;
    g = _grandparent(n);
    if (n == p->left) {
        _rotate_right(g);
    } else {
        _rotate_left(g);
    }
    p->color = __BLACK;
    g->color = __RED;
}

void
_insert_recurse(rbtree_t *root, rbtree_t *n, int (*cmp)(void*,void*))
{
    if (root && cmp(n->data,root->data) <= 0) {
        if (root->left) {
            _insert_recurse(root->left,n,cmp);
        } else {
            root->left = n;
        }
    } else if (root && cmp(n->data,root->data) > 0) {
        if (root->right) {
            _insert_recurse(root->right,n,cmp);
        } else {
            root->right = n;
        }
    }
    n->parent = root;
    n->left = n->right = NULL;
    n->color = __RED;
}

rbtree_t *
rbtree_insert(rbtree_t *root, rbtree_t *n, int (*cmp)(void*,void*))
{
    _insert_recurse(root,n,cmp);
    _insert_repair_tree(n);
    // Return new root
    while (n->parent) {
        n = n->parent;
    }
    return n;
}

rbtree_t *
rbtree_new(void *data)
{
    rbtree_t *x = calloc(1,sizeof(rbtree_t));
    if (!x) { return NULL; }
    x->data = data;
    return x;
}

void
rbtree_free(rbtree_t *r)
{
    free(r);
}

static void
_eval_inner(rbtree_t *r, void (*fun)(int,void*,void*), void *aux, int depth)
{
    if (r) {
        fun(depth,r->data,aux);
        _eval_inner(r->left,fun,aux,depth+1);
        _eval_inner(r->right,fun,aux,depth+1);
    }
}

void
rbtree_eval(rbtree_t *r, void (*fun)(int,void*,void*), void *aux)
{
    _eval_inner(r,fun,aux,0);
}
