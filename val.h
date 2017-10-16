#ifndef VAL_H
#define VAL_H 

typedef struct {
    enum {
        val_t_INT,
        val_t_FLOAT,
        var_t_STR
    } type;
    union {
        int i;
        float f;
        char *c;
    };
} val_t;

#endif /* VAL_H */
