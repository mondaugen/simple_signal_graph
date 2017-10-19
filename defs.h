#ifndef DEFS_H
#define DEFS_H 

#define pmf(c,f) (c)->vt->f
//#define mf(c,f,__VA_ARGS__) (c)->vt->f(c,__VA_ARGS__)
#define mf(c,f,...) (c)->vt->f(c,##__VA_ARGS__)

#endif /* DEFS_H */
