#include "graph_node.h"
#include "defs.h"

err_t
graph_node_connect(graph_node_t *parent,
        size_t output_i,
        graph_node_t *child,
        size_t input_i)
{
    if ((input_i >= child->ninputs) 
            || (mf(parent,outputs,output_i) == NULL)) {
        return err_EOB;
    }
    child->inputs[input_i] = mf(parent,outputs,output_i);
    return err_NONE;
}


