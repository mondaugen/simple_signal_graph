#include "graph_desc.h"
#include "_graph_desc_defs.h"

#define _CHR_BUF_LEN 256

typedef union {
    enum {
        gd_err_NONE = 0,
        gd_err_NAME_TOO_LONG,
        gd_err_NERRS,
    } e;
    gd_err_t i;
} _gd_err_t;

struct graph_desc_parser_t {
    /* Current number of node names, connections parsed, current length of order array. */
    graph_desc_parser_sz_t max_szs;
    graph_desc_t *tmp_gd;
};

graph_desc_parser_t *
graph_desc_parser_new(graph_desc_parser_init_t *init)
{
    size_t sz = sizeof(graph_desc_parser_t) + graph_desc_sz(&init->max_szs);
    graph_desc_parser_t *ret = calloc(1,sz);
    if (!ret) {
        return NULL;
    }
    ret->cur_graph = (graph_desc_t*)(ret+1);
    graph_desc_init(ret->cur_graph,&init->max_szs);
    ret->max_szs = init->max_szs;
    return ret;
}

graph_desc_t *
graph_desc_parser_get_cur_graph(graph_desc_parser_t *gdp)
{
    return gdp->cur_graph;
}

int
_parse_line(char *pch,
            size_t max_len, 
            char_stream_t *cs)
{
    int sz = 0;
    while (1) {
        if (sz == max_len) {
            return -1;
        }
        if (!mf(cs,get_chr,pch)
                || *pch == '\n') {
            *pch = '\0';
            break;
        }
        pch++;
        sz++;
    }
    return sz;
}

gd_err_t
graph_desc_parser_parse(graph_desc_parser_t *gdp,
                        char_stream_t *cs,
                        graph_desc_t **gd)
{
    graph_desc_t *ret = NULL;
    int sz = 0;
    char *pch = ;
    int err = gd_err_NONE;
    /* Parse graph's name */
    if ((sz = _parse_line(gdp->tmp_gd->name,
                gdp->max_szs->name_sz,cs)) < 0) {
        err = gd_err_NAME_TOO_LONG;
        goto cleanup;
    }
    gdp->tmp_gd->name_sz = (size_t)sz;
    size_t idx = 0;
    /* Parse names of nodes in graph */
    while (1) {
        if (idx == gdp->max_szs->nd_names_sz) {
            err = gd_err_TOO_MANY_NODES;
            goto cleanup;
        }
       if ((sz = _parse_line(gdp->tmp_gd->nd_names[idx],
                   GRAPH_DESC_MAX_NAME_LEN,
                   cs)) < 0) {
           err = gd_err_ND_NAME_TOO_LONG;
           goto cleanup;
       }
       /* We don't keep track of each node name's size */
       if (sz == 0) {
           /* Read 2 \n in a row, end of that section */
           break;
       }
       idx++;
    }
    gdp->tmp_gd->nd_names_sz = idx;
    /* Parse connections in graph */
    idx = 0;
    char buf[_CHR_BUF_LEN];
    while (1) {
        if (idx == gdp->max_szs->cxns_sz) {
            err = gd_err_TOO_MANY_CXNS;
            goto cleanup;
        }
        if ((sz = _parse_line(buf,
                        _CHR_BUF_LEN,
                        cs)) < 0) {
            err = gd_err_CXN_DESC_TOO_LONG;
            goto cleanup;
        }
        if (sz == 0) {
            /* Read 2 \n in a row, end of that section */
            break;
        }
        cxn_t *pcxn = &gdp->tmp_gd->cxns[idx];
        sz = sscanf(buf,
                "%d %d %d %d",
                &pcxn->p,
                &pcxn->o,
                &pcxn->c,
                &pcxn->i);
        if (sz != 4) {
            err = gd_err_BAD_CXN_DESC;
            goto cleanup;
        }
        idx++;
    }
    gdp->tmp_gd->cxns_sz = idx;
    /* TODO: build and sort the order graph */
        
    /* success */
    *gd = ret;
    return err;
cleanup:
    if (ret) {
        free(ret);
    }
    return err;
}






