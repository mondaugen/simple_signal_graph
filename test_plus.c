#include "graph_node.h"
#include "const.h"
#include "plus.h"
#include "print.h"

int main (void)
{
    graph_node_t *graph[] = {
        (graph_node_t*)const_new(
                (val_t){.type=val_t_FLOAT,.f=1.}),
        (graph_node_t*)const_new(
                (val_t){.type=val_t_FLOAT,.f=2.5}),
        (graph_node_t*)plus_new(),
        (graph_node_t*)print_new("Result"),
        NULL
    };
    graph_node_connect(graph[0],0,graph[2],0);
    graph_node_connect(graph[1],0,graph[2],1);
    graph_node_connect(graph[2],0,graph[3],0);
    graph_node_t **gptr = graph;
    while (*gptr) {
        if (pmf(*gptr,eval)) {
            mf(*gptr,eval);
        }
        gptr++;
    }
    return 0;
}

