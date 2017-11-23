#ifndef CHAR_STREAM_H
#define CHAR_STREAM_H 

typedef int cs_int_t;

typedef struct char_stream_t char_stream_t;

typedef struct {
    cs_int_t (*get_chr)(char_stream_t *, char *);
} char_stream_vt_t;

struct char_stream_t {
    char_stream_vt_t *_vt;
};

#endif /* CHAR_STREAM_H */
