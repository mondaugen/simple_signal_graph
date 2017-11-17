/* Does the graph_desc initialize ? */
#include <stdio.h>
#include <assert.h>
#include "graph_desc.h"

#define ND_NAMES_SZ 16
#define CXNS_SZ     8
#define ORD_SZ      16

extern graph_desc_t * graph_desc_parser_get_cur_graph(graph_desc_parser_t *gdp);

int 
main(void)
{
    graph_desc_parser_init_t gdpi = {
        .max_szs = {
            .nd_names_sz = ND_NAMES_SZ,
            .cxns_sz = CXNS_SZ,
            .ord_sz = ORD_SZ,
        }
    };
    /* These sizes should be equal */
    size_t correct_sz = sizeof(graph_desc_t) 
        + ND_NAMES_SZ*GRAPH_DESC_MAX_NAME_LEN 
        + ND_NAMES_SZ*sizeof(char*) 
        + CXNS_SZ * sizeof(graph_cxn_t)
        + ORD_SZ * sizeof(int);
    assert(correct_sz == graph_desc_sz(&gdpi.max_szs));
    printf("graph_desc_sz gives correct size %zu\n",correct_sz);
    graph_desc_parser_t *gdp = graph_desc_parser_new(&gdpi);
    if (!gdp) {
        return 1;
    }
    int i;
    graph_desc_t *cg = graph_desc_parser_get_cur_graph(gdp);
    char chr = 'A';
    for (i = 0; i < ND_NAMES_SZ; i++) {
        int j;
        for (j = 0; j < 31; j++) {
            cg->nd_names[i][j] = chr;
            chr = (chr + 1) > 'z' ? 'A' : chr + 1;
        }
    }
    for (i = 0; i < ND_NAMES_SZ; i++) {
        printf("%s\n",cg->nd_names[i]);
    }
    free(gdp);
    return 0;
}
