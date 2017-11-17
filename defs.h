#ifndef DEFS_H
#define DEFS_H 

/* Get a pointer to a member function */
#define pmf(c,f) (c)->_vt->f
/* Call a class c's member function f with any non-negative number of arguments. */
#define mf(c,f,...) (c)->_vt->f(c,##__VA_ARGS__)

#endif /* DEFS_H */
