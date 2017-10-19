#ifndef GRAPH_NODE_HH
#define GRAPH_NODE_HH

#include <stddef.h>
#include "err.h"
#include "val.h"

class graph_node_c {
    public:
    static err_t connect(graph_node_c *parent, size_t output_i, graph_node_c *child, size_t input_i);
    val_t **inputs;
    size_t ninputs;
    virtual val_t *outputs(size_t i);
    virtual void eval () = 0;
    virtual void free () = 0;
};
 
#endif /* GRAPH_NODE_HH */
