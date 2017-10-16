#include "const.hh"

const_c::const_c(val_t c)
{
    inputs = NULL;
    ninputs = 0;
    this->c = c;
}

val_t *
const_c::outputs(size_t i)
{
    if (i != 0) {
        return NULL;
    }
    return &c;
}

void
const_c::eval(void)
{
    // Value is already there
}
