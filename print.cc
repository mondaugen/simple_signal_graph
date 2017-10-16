#include "print.hh"
#include <stdio.h>

print_c::print_c(void)
{
    inputs = new *val_t[1];
    ninputs = 1;
}

print_c::~print_c(void)
{
    delete[1] inputs;
}

val_t *
print_c::outputs(size_t i)
{
    return NULL;
}

void
print_c::eval(void)
{
    printf("%f\n",inputs[0]->f);
}
