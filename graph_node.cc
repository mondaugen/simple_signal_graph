#include "graph_node.hh"

static err_t 
graph_node_c::connect(graph_node_c *parent, size_t output_i, graph_node_c *child, size_t input_i)
{
    if ((input_i >= child->ninputs) || (parent->outputs(output_i) == NULL)) {
        return err_EOB;
    }
    child->inputs[i] = parent->outputs(output_i);
    return err_NONE;
}

virtual val_t *
graph_node_c::outputs(size_t i) 
{
    if (i < ninputs) {
        return inputs[i];
    }
    return NULL;
}
