#ifndef GRAPH_DESC_H
#define GRAPH_DESC_H 

#include <stdlib.h>

#define GRAPH_DESC_MAX_NAME_LEN 32

typedef int gd_err_t;

typedef struct {
    int p; /* parent */
    int o; /* output */
    int c; /* child */
    int i; /* inlet */
} graph_cxn_t;

#include "_graph_desc_decls.h"

typedef graph_desc_meminit_t graph_desc_parser_sz_t;
typedef struct graph_desc_parser_t graph_desc_parser_t;

typedef struct {
    graph_desc_parser_sz_t max_szs;
} graph_desc_parser_init_t;

graph_desc_parser_t *
graph_desc_parser_new(graph_desc_parser_init_t *init);

#endif /* GRAPH_DESC_H */
