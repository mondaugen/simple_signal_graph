/*
gd_err_t
graph_desc_parser_parse(graph_desc_parser_t *gdp,
                        char_stream_t *cs,
                        graph_desc_t **gd)
{
    graph_desc_t *ret = NULL;
    int sz = 0;
    int err = gd_err_NONE;
    
    if ((sz = _parse_line(gdp->tmp_gd->name,
                gdp->max_szs.name_sz,cs)) < 0) {
        err = gd_err_NAME_TOO_LONG;
        goto cleanup;
    }
    gdp->tmp_gd->name_sz = (size_t)sz;
    size_t idx = 0;
    
    while (1) {
        if (idx == gdp->max_szs.nd_names_sz) {
            err = gd_err_TOO_MANY_NODES;
            goto cleanup;
        }
       if ((sz = _parse_line(gdp->tmp_gd->nd_names[idx],
                   GRAPH_DESC_MAX_NAME_LEN,
                   cs)) < 0) {
           err = gd_err_ND_NAME_TOO_LONG;
           goto cleanup;
       }
       
       if (sz == 0) {
           
           break;
       }
       idx++;
    }
    gdp->tmp_gd->nd_names_sz = idx;
    
    gdp->tmp_gd->ord_sz = idx;
    
    idx = 0;
    char buf[_CHR_BUF_LEN];
    while (1) {
        if (idx == gdp->max_szs.cxns_sz) {
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
            
            break;
        }
        graph_cxn_t *pcxn = &gdp->tmp_gd->cxns[idx];
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
    
    if (_nodes_to_adj_list(
                
                gdp->tmp_gd->cxns,
                
                gdp->tmp_gd->cxns_sz,
                
                gdp->adj_lists,
                
                gdp->adj_list_mem,
                
                gdp->tmp_gd->nd_names_sz)) {
        err = gd_err_BUILDING_ADJ_LIST;
        goto cleanup;
    }
    
    if (tsg_compute(gdp->adj_lists,
                gdp->tmp_gd->nd_names_sz,
                gdp->tmp_gd->ord)) {
        err = gd_err_TSORTING_GRAPH;
        goto cleanup;
    }
    
    
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
*/
