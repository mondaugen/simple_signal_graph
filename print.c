#include "graph_node.h"
#include "print.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct print_t {
    graph_node_t super;
    char *prefix;
};

static val_t *
_outputs(graph_node_t *gn, size_t i)
{
    /* No output */
    return NULL;
}

static void
_eval(graph_node_t *gn)
{
    printf("%s: %f\n",((print_t*)gn)->prefix,gn->inputs[0]->f);
}

static void
_free(graph_node_t *gn)
{
    free(gn);
}

graph_node_vt _print_vt = {_outputs, _eval, _free};

print_t *
print_new(const char *prefix)
{
    print_t *ret = calloc(1,sizeof(print_t)
            +sizeof(val_t*)+strlen(prefix)+1);
    if (!ret) {return NULL;}
    graph_node_t *gn = (graph_node_t*)ret;
    *gn = (graph_node_t) {
        .inputs = (val_t**)(ret+1),
        .ninputs = 1,
        .vt = &_print_vt,
    };
    ret->prefix = (char*)(gn->inputs+1);
    strcpy(ret->prefix,prefix);
    return ret;
}
