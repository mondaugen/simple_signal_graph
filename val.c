#include "val.h"

/* TODO: Add accelerated routines.
   Note: these routines do no do type checking. They should be used only after
   determining the correct types. */

#define VAL_VECT_ARITH(a,b,len,opt)\
    size_t __n;\
    for (__n = 0; __n < len; __n++) {\
        a[__n] opt b[__n];\
    }

#define VAL_VECT_DYAD(T,O,N)\
    void\
    val_ ## N ## _v ## T(val_t *x, val_t *y)\
    {\
        VAL_VECT_ARITH(x->v.T,y->v.T,x->v.len,O);\
    }

//#define VAL_SEARCH_F(T,N)\
//    val_dyad_f\
//    val_get_ ## N (a,b)\
//    {\
//        if (a.type +=


#define X(a,b,c) VAL_VECT_DYAD(a,b,c)
VAL_VECT_OPTS(f32);
VAL_VECT_OPTS(i32);
#undef X

#define VAL_GET_F(N)\
    val_dyad_f\
    val_get_ ## N (val_t *x, val_t *y)\
    {\
        if (x->type != y->type) {\
            return NULL;\
        }\
        switch (x->type) {\
            case val_t_VINT32:\
                return val_ ## N ## _vi32;\
            case val_t_VFLOAT32:\
                return val_ ## N ## _vf32;\
            case val_t_INT32:\
                return NULL;\
            case val_t_FLOAT32:\
                return NULL;\
            case val_t_STR:\
                return NULL;\
        }\
        return NULL;\
    }

#define X(a,b,c) VAL_GET_F(c)
VAL_VECT_OPTS(all);
#undef X
