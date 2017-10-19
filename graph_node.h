#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H 

#include <stddef.h>
#include "defs.h"
#include "val.h"
#include "err.h"

typedef struct graph_node_t graph_node_t;

typedef struct {
    /* Called to get the nth output (n passed to size_t argument) */
    val_t *(*outputs)(graph_node_t *, size_t);
    /* Called to do what the graph node does */
    void (*eval)(graph_node_t *);
    /* Frees the graph node */
    void (*free)(graph_node_t *);
    /* Called after the graph has been constructed. Should return err_NONE
       unless something is wrong with the graph.
       One use for this function is to check the types of the inputs to
       determine which function to call.
     */
    err_t (*pre_render_setup)(graph_node_t *);
} graph_node_vt;

struct graph_node_t {
    val_t **inputs;
    size_t ninputs;
    graph_node_vt *vt;
};

err_t
graph_node_connect(graph_node_t *parent,
        size_t outlet_i,
        graph_node_t *child,
        size_t inlet_t);

#endif /* GRAPH_NODE_H */
