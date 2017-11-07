#ifndef PARSE_G_H
#define PARSE_G_H 

/* Describe a connection between 2 nodes. */
typedef struct {
    int p; /* parent */ 
    int o; /* outlet */ 
    int c; /* child */ 
    int i; /* inlet */ 
} nd_cxn_t;

/* Describe a node graph. */
typedef struct {
    size_t n_ins; /* number of inlets */ 
    int *ins; /* inlets */
    size_t n_outs; /* number of outlets */
    int *outs; /* outlets */
    size_t n_nds; /* number of nodes */
    char **nds; /* node names */
    size_t n_cxns; /* number of connections */
    nd_cxn_t *cxns; /* connections */
    int *ord; /* graph evaluation order */
} ndg_desc_t;

typedef struct ndg_file_parser_t ndg_file_parser_t;

#endif /* PARSE_G_H */
