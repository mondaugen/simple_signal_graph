#ifndef GRALLOC_H
#define GRALLOC_H 

typedef struct gralloc_t gralloc_t;
struct gralloc_t {
    char *cur;
    char *end;
};

void
gralloc_init(gralloc_t *a, char *cur, char *end);


size_t
gralloc_sz(void);

void *
gralloc_alloc(gralloc_t *a, size_t sz);


char *
gralloc_parse_line(gralloc_t *a,
            size_t *sz, 
            char_stream_t *cs);


#endif /* GRALLOC_H */
