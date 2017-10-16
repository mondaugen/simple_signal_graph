#include "graph_node.hh"
#include "plus.hh"
#include "const.hh"
#include "print.hh"

int main (void)
{
    const_c i1 = const_c(1);
    const_c i2 = const_c(2);
    const_c i3 = const_c(3);
    plus_c p1 = plus_c();
    plus_c p2 = plus_c();
    print_c pr1 = print_c;
    graph_node_c::connect(&i1,0,&p1,0)
    graph_node_c::connect(&i2,0,&p1,1)
    graph_node_c::connect(&p1,0,&p2,0)
    graph_node_c::connect(&i3,0,&p2,1)
    graph_node_c::connect(&p2,0,&pr1,0)
    i1.eval();
    i2.eval();
    i3.eval();
    p1.eval();
    p2.eval();
    pr1.eval();
    return 0;
}

    
