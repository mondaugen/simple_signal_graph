#include "graph_node.h"
#include "const.h"
#include <stdlib.h>

struct const_t {
    graph_node_t super;
    val_t val;
};

static val_t *
_outputs(graph_node_t *gn, size_t i)
{
    if (i != 0) {
        return NULL;
    }
    return &((const_t*)gn)->val;
}

/* No eval function. */

static void
_free(graph_node_t *gn)
{
    free(gn);
}

graph_node_vt _const_vt = {_outputs, NULL, _free};

const_t *
const_new(val_t val)
{
    const_t *ret = calloc(1,sizeof(const_t)
            +sizeof(val_t));
    if (!ret) {return NULL;}
    graph_node_t *gn = (graph_node_t*)ret;
    *gn = (graph_node_t) {
        .inputs = NULL,
        .ninputs = 0,
        .vt = &_const_vt,
    };
    ret->val = val;
    return ret;
}
