#ifndef QUEUE_H
#define QUEUE_H 

#include <stddef.h>

typedef struct q_t q_t;

q_t * q_new(size_t sz);
void q_free(q_t *q);
int q_push(q_t *q, void *d);
void * q_pop(q_t *q);

#endif /* QUEUE_H */
