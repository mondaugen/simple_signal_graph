/* Parse a graph description and return a graph_desc_t. */

#include "parse_g.h"

typedef struct {
	/* max number of nodes, inlets, outlets */
	size_t		max_n_nds;
	/* max number of connections (cannot be more than n*(n-1)/2 where n
	 * is max number of nodes).  */
	size_t		max_n_cxns;
	/* maximum length of a node name */
	size_t		max_name_len;
} ndg_file_parser_lims_t;

struct {
    ndg_file_parser_lims_t;
    ndg_desc_t cur_desc;
} ndg_file_parser_t;

ndg_file_parser_t *
ndg_file_parser_new(ndg_file_parser_lims_t lims)
{
    ndg_file_parser_t *ret = calloc(1,sizeof(ndg_file_parser_t)
            + sizeof(int)(2*lims.max_n_cxns)
            + sizeof(char*)*lims.max_n_nds
            + sizeof(nd_cxt_t)*lims.max_n_cxns
            + sizeof(int)*lims.max_n_nds
            + sizeof(char)*lims.max_name_len*lims.max_n_nds);
    if (!ret) {
        return NULL;
    }
    char *ptr = (char*)(ret+1);
    ret->cur_desc.ins = (int*)ptr;
    ptr = (char*)((int*)ptr + lims.max_n_nds);
    ret->cur_desc.outs = (int*)ptr;
    ptr = (char*)((int*)ptr + lims.max_n_nds);
    ret->cur_desc.nds = (char**)ptr;
    ptr = (char*)((char**)ptr + lims.max_n_nds


