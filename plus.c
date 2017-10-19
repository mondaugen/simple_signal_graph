#include "graph_node.h"
#include "plus.h"
#include <stdlib.h>

struct plus_t {
    graph_node_t super;
};

static val_t *
_outputs(graph_node_t *gn, size_t i)
{
    if (i != 0) { return NULL; }
    return gn->inputs[0];
}

static void
_eval(graph_node_t *gn)
{
    gn->inputs[0]->f += gn->inputs[1]->f;
}

static void
_free(graph_node_t *gn)
{
    free(gn);
}

graph_node_vt _plus_vt = {_outputs, _eval, _free};

plus_t *
plus_new(void)
{
    plus_t *result = calloc(1,sizeof(plus_t)+sizeof(val_t*)*2);
    graph_node_t *gn = (graph_node_t*)result;
    if (!result) { return NULL; }
    *gn = (graph_node_t) {
            .inputs = (val_t**)(result+1),
            .ninputs = 2,
            .vt = &_plus_vt
    };
    return result;
}
