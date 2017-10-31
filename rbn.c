#include <stdlib.h>
#include "rbn.h"
#include "queue.h"

typedef enum { _BLK, _RED} rbnc_t;

struct rbn_t {
    rbn_t *p;
    rbn_t *l;
    rbn_t *r;
    rbnc_t c;
    void *d;
};

static rbn_t *
_uncle(rbn_t *a)
{
    if (!a || !a->p || !a->p->p) {
        return NULL;
    }
    return a->p->p->l == a->p ? a->p->p->r : a->p->p->l;
}


static void
_left_rotate(rbn_t *a)
{
    rbn_t *b = a->r;
    if (!b) { return; }
    a->r = b->l;
    if (b->l) { b->l->p = a; }
    if (a->p) {
        if (a == a->p->l) { a->p->l = b; }
        else { a->p->r = b; }
    }
    b->p = a->p;
    a->p = b;
    b->l = a;
}

static void
_right_rotate(rbn_t *a)
{
    rbn_t *b = a->l;
    if (!b) { return; }
    a->l = b->r;
    if (b->r) { b->r->p = a; }
    if (a->p) {
        if (a == a->p->l) { a->p->l = b; }
        else { a->p->r = b; }
    }
    b->p = a->p;
    a->p = b;
    b->r = a;
}

static void
_insert_fixup(rbn_t *n)
{
    while (n && n->c != _BLK) {
        if (!n->p) {
            n->c = _BLK;
            return;
        }
        if (n->p->c == _BLK) {
            return;
        }
        /* n->p->c must be _RED */
        if (_uncle(n) && _uncle(n)->c == _RED) {
            n->p->c = _BLK;
            _uncle(n)->c = _BLK;
            /*
               Guaranteed to exist because this red node isn't a child of
               root, otherwise the loop would've exited at case 2
            */
            n->p->p->c = _RED;
            n = n->p->p;
        } else if (!_uncle(n) || _uncle(n)->c == _BLK) {
            if (n->p->p->r == n->p) {
                /* Parent node is right child. */
                if (n->p->l == n) {
                    /* Current node is left child. */
                    n = n->p;
                    _right_rotate(n);
                }
                n->p->c = _BLK;
                n->p->p->c = _RED;
                _left_rotate(n->p->p);
            } else {
                /* Parent node is left child. */
                if (n->p->r == n) {
                    /* Current node is right child. */
                    n = n->p;
                    _left_rotate(n);
                }
                n->p->c = _BLK;
                n->p->p->c = _RED;
                _right_rotate(n->p->p);
            }
        }
    }
}

/* Inserts a node n into a tree t.
   The cmp function should take 2 data each belonging to their respective node's
   d field and return 1 if the right argument is greater than the left argument,
   0 if equal and -1 if the right argument less than the right argument.
   If cmp returns 0, this places the item to the right of the node compared to.

   Returns the new base of the tree.
 */
rbn_t *
rbn_insert(rbn_t *t, rbn_t *n, int (*cmp)(void *,void*))
{
    if (!n) {
        return NULL;
    }
    n->c = _RED;
    while (t) {
        if (cmp(t->d, n->d) >= 0) {
            if (t->l) {
                t = t->l;
            } else {
                t->l = n;
                break;
            }
        } else {
            if (t->r) {
                t = t->r;
            } else {
                t->r = n;
                break;
            }
        }
    }
    n->p = t;
    t = n;
    _insert_fixup(t);
    while (t->p) {
        t = t->p;
    }
    return t;
}

rbn_t *
rbn_new(void *d)
{
    rbn_t *ret = calloc(1,sizeof(rbn_t));
    if (!ret) { return NULL; }
    ret->d = d;
    return ret;
}

void
rbn_free(rbn_t *r)
{
    free(r);
}

/* Estimate the depth, the true depth will be no more than twice this length. */
static size_t
_depth_est(rbn_t *r)
{
    size_t d = 0;
    while (r) {
        d++;
        r = r->l;
    }
/* Add 1 because NULL sentinel nodes are counted. */
    d++;
    return d;
}

/* Iterate over the tree and call a function on each node.
   The function is given an information struct. */
int
_rbn_iter(rbn_t *r,void (*fun)(rbni_t*), void *aux)
{
    if (!r) { return 1; }
    size_t de = 2*_depth_est(r);
/* Queue for breadth first search. Has enough room to store all the nodes if
   need be, could probably be half the size... */
    q_t *q = q_new(de);
    if (!q) { return 1; }
    (void)q_push(q,r);
    rbni_t info = {
        .aux = aux
    };
    int ret = 0;
    size_t d = d;
    while ((r = q_pop(q))) {
        // slow way to get depth
        info.idx = 1;
        rbn_t *tmp = r;
        while (tmp->p) { tmp = tmp->p; info.idx++; }
        info.data = r->d;
        info.branch = !r->p ? rbni_ROOT :
            (r->p->l == r ? rbni_LEFT : rbni_RIGHT);
        fun(&info);
        if ((q_push(q,(void*)r->l) || q_push(q,(void*)r->r))) {
            ret = 1;
            goto end;
        }
    }
end:
    q_free(q);
    return ret;
}

void _rbn_dfs(rbn_t *r,void(*fun)(rbni_t*),size_t d, rbni_branch_t b, void *aux,rbni_t *rbni)
{
    if (r) {
        *rbni = (rbni_t){
            .node = r,
            .idx = d,
            .data = r->d,
            .aux = aux,
            .branch = b
        };
        fun(rbni);
        _rbn_dfs(r->l,fun,2*(1+d)-1,rbni_LEFT,aux,rbni);
        _rbn_dfs(r->r,fun,2*(1+d),rbni_RIGHT,aux,rbni);
    }
}

void *
rbn_get_data(rbn_t *r)
{
    return r->d;
}

char
_rbn_get_color(rbn_t *r)
{
    return r->c == _BLK ? 'b' : 'r';
}

/* d is array of data of length len. */
rbn_t *
rbn_tree_from_array(void **d, size_t len)
{
    void **ptr = d;
    rbn_t *rbns[len];
    size_t i = 0;
    for (i = 0; i < len; i++) {
        if (*ptr) {
            rbns[i] = rbn_new(*ptr);
        } else {
            rbns[i] = NULL;
        }
        ptr++;
    }
    rbns[0]->p = NULL;
    for (i = 1; i < len; i++) {
        if (rbns[i]) { rbns[i]->p = rbns[(i+1)/2-1]; }
    }
    for (i = 0; i < len; i++) {
        if (((i+1)*2-1) < len && rbns[i]) {
            rbns[i]->l = rbns[(i+1)*2-1];
        }
        if (((i+1)*2) < len && rbns[i]) {
            rbns[i]->r = rbns[(i+1)*2];
        }
    }
    return rbns[0];
}

