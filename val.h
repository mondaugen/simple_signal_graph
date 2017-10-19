#ifndef VAL_H
#define VAL_H 

#include <stddef.h>
#include <stdint.h>

typedef struct {
    enum {
        val_t_INT32,
        val_t_FLOAT32,
        val_t_VINT32,
        val_t_VFLOAT32,
        val_t_STR
    } type;
    union {
        int32_t i32;
        float f32;
        struct {
            union {
                float *f32;
                int *i32;
            };
            size_t len;
        } v;
        char *c;
    };
} val_t;

typedef void (*val_dyad_f)(val_t*,val_t*);

#define VAL_VECT_DYAD_DECL(T,O,N)\
    void\
    val_ ## N ## _v ## T(val_t *x, val_t *y)

#define VAL_GET_F_DECL(N)\
    val_dyad_f\
    val_get_ ## N (val_t *x, val_t *y)

#define VAL_VECT_OPTS(T)\
    X(T,+=,acc);\
    X(T,*=,mul);\
    X(T,/=,div);\
    X(T,-=,sub)

#define X(a,b,c) VAL_VECT_DYAD_DECL(a,b,c)
VAL_VECT_OPTS(f32);
VAL_VECT_OPTS(i32);
#undef X

#define X(a,b,c) VAL_GET_F_DECL(c)
VAL_VECT_OPTS(all);
#undef X

#endif /* VAL_H */
