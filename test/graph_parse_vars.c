/* Graph parsing with variables */
#include "defs.h"
#include "file_cs.h"


graph_desc_parser_t *
graph_desc_parser_new(graph_desc_parser_init_t *init)
{
    size_t N_nds = init->max_szs.nd_names_sz,
           N_cxns = init->max_szs.cxns_sz;
    graph_desc_parser_meminit_t gdpmi = {
        .tmp_gd = init->max_szs,
    /* this is additional space to do topological sorting of graphs. In the
       worst case a graph could have N_nds^2 connections, so we allocate enough
       space for this. (This graph cannot be acylical of course, but this is
       determined by the tsortg algorithm). */
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

gd_err_t
graph_desc_parser_parse(graph_desc_parser_t *gdp,
                        char_stream_t *cs,
                        graph_desc_t **gd)
{
    graph_desc_t *ret = NULL;
    int sz = 0;
    int err = gd_err_NONE;
    /* Parse graph's name */
    if ((sz = _parse_line(gdp->tmp_gd->name,
                gdp->max_szs.name_sz,cs)) < 0) {
        err = gd_err_NAME_TOO_LONG;
        goto cleanup;
    }
    gdp->tmp_gd->name_sz = (size_t)sz;
