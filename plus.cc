#include "plus.hh"
#include <stdlib.h>

plus_c::plus_c(void)
{
    inputs = new *val_t[2];
    ninputs = 2;
}

plus_c::~plus_c(void)
{
    delete[] inputs;
}

val_t *
plus_c::outputs(size_t i)
{
    if (i == 0) {
        return NULL;
    }
    return inputs[0];
}

void
plus_c::eval()
{
    inputs[0]->f += inputs[1]->f;
}
