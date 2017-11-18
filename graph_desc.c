#include "graph_desc.h"

#include "_graph_desc_sdefs.h"
typedef int * _gd_adj_list_t;
#include "_graph_desc_parser_sdefs.h"
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

/* Normally if the graph_desc filled all the memory that was allocated to it, you could just copy it with memcpy, but what we have here is a graph description whose sizes have been temporarily made small. We want to copy it to a graph_desc_t that is "tight", i.e., the graph_desc fills all the memory allocated to it.
If dst is set to NULL, the function just returns the amount of space that needs to be allocated for copying src. */
size_t
graph_desc_copy(graph_desc_t *dst, const graph_desc_t *src)
{
    graph_desc_meminit_t gdmi = {
        .name_sz = src->name_sz,
        .nd_names_sz = src->nd_names_sz,
        .cxns_sz = src->cxns_sz,
        .ord = src->ord_sz
    };
    size_t sz = graph_desc_sz(&gdmi);
    if (dst) {
        graph_desc_meminit(dst,&gdmi);
        memcpy(dst->name,
                src->name,
                sizeof(char)*src->name_sz);
        memcpy(dst->nd_names[0],
                src->nd_names[0],
                GRAPH_DESC_MAX_NAME_LEN*src->nd_names_sz);
        memcpy(dst->cxns,
                src->cxns,
                src->cxns_sz * sizeof(graph_cxn_t));
        memcpy(dst->ord,
                src->ord,
                src->ord_sz * sizeof(int));
    }
    return sz;
}

graph_desc_parser_t *
graph_desc_parser_new(graph_desc_parser_init_t *init)
{
    size_t N_nds = init->max_szs.nd_names_sz,
           N_cxns = init->max_szs.cxns_sz;
    graph_desc_parser_meminit_t gdpmi = {
        .tmp_gd = init->max_szs,
    /* this is additional space to do topological sorting of graphs. In the worst case a graph could have N_nds^2 connections, so we allocate enough space for this. (This graph cannot be acylical of course, but this is determined by the tsortg algorithm). */
        .adj_lists_sz = N_nds,
        .adj_list_mem_sz = (N_cxns*N_cxns) + N_nds
    };
    size_t sz = graph_desc_parser_sz(&gdpmi);
    graph_desc_parser_t *ret = calloc(1,sz);
    if (!ret) {
        return NULL;
    }
    graph_desc_parser_meminit(ret,&gdpmi);
    graph_desc_meminit(ret->tmp_gd,&init->max_szs);
    ret->max_szs = init->max_szs;
    return ret;
}

graph_desc_t *
graph_desc_parser_get_tmp_gd(graph_desc_parser_t *gdp)
{
    return gdp->tmp_gd;
}

static int
_gcxn_compar_p(const void * _a, const void * _b)
{
    const graph_cxn_t *a = _a,
                *b = _b;
    return (a->p > b->p) - (a->p < b->p);
}

/* Checks to see cxns describes the connections of a valid graph and returns an
   adjacency list of length N_nds in adj_lists. See below for the size
   constraints on adj_lists and adj_list_mem.  Returns non-zero on error. */
static int 
_nodes_to_adj_list(graph_cxn_t *cxns,
                   size_t N_cxns,
                   int **adj_lists,
                   int *adj_list_mem,
                   size_t N_nds)
{
    /* N_cxns is length of cxns, the connections in the graph.
       adj_lists has length N_nds and will be filled with the adjacency lists for the graph.
       adj_list_mem is memory for hollding the adjacency lists and has length N_cxns^2 + N_nds.
       */
    if (!N_cxns || !N_nds) {
        return -1;
    }
    size_t n;
    for (n = 0; n < N_cxns; n++) {
        if (cxns[n].c >= N_nds || cxns[n].p >= N_nds) { 
            return -1; 
        }
    }
    qsort(cxns,N_cxns,sizeof(graph_cxn_t),_gcxn_compar_p);
    int **pis = adj_lists;
    int *pi = adj_list_mem;
    *pis++ = pi;
    size_t i = n = 0;
    while (n < N_cxns) {
        while (cxns[n].p > i && i < N_nds) {
            *pi++ = -1;
            *pis++ = pi;
            i++;
        }
        *pi++ = cxns[n++].c;
    }
    *pi++ = -1;
    return 0;
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
    /* This will also be the size of the ordered node array */
    gdp->tmp_gd->ord_sz = idx;
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
    /* Build adjacency lists */
    if (_nodes_to_adj_list(
                /* cxns */
                gdp->tmp_gd->cxns,
                /* N_cxns */
                gdp->tmp_gd->cxns_sz,
                /* adj_lists */
                gdp->adj_lists,
                /* adj_list_mem */
                gdp->adj_lists_mem,
                /* N_nds */
                gdp->tmp_gd->nd_names_sz)) {
        err = gd_err_BUILDING_ADJ_LIST;
        goto cleanup;
    }
    /* Topologically sort, put result into gdp->tmp_gd->ord. */
    if (tsg_compute(gdp->adj_lists,
                gdp->tmp_gd->nd_names_sz,
                gdp->tmp_gd->ord)) {
        err = gd_err_TSORTING_GRAPH;
        goto cleanup;
    }
    /* success */
    /* copy the discovered graph description into one of the correct size (tmp_gd is intentionally oversized so that there is enough room during parsing) */
    size_t ssz = graph_desc_copy(NULL,gdp->tmp_gd);
    ret = calloc(1,ssz);
    if (!ret) {
        err = gd_err_ALLOCATING_GRAPH;
        goto cleanup;
    }
    graph_desc_copy(ret,gdp->tmp_gd);
    *gd = ret;
    return err;
cleanup:
    if (ret) {
        free(ret);
    }
    return err;
}






