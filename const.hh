#ifndef CONST_HH
#define CONST_HH 

#include "graph_node.hh"

struct const_c : graph_node_c {
    const_c(val_t c);
    ~const_c();
    val_t c;
};

#endif /* CONST_HH */
