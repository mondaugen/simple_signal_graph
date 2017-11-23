#include "file_cs.h"
#include "graph_desc.h"

void
_graph_desc_printf(graph_desc_t *gd);

int
main(void)
{
    graph_desc_parser_init_t gdpi = {
        .max_szs = (graph_desc_parser_sz_t) {
            .name_sz = 32,
            .nd_names_sz = 16,
            .cxns_sz = 16*16,
            .ord_sz = 16
        }
    };
    int err = 0;
    graph_desc_parser_t *gdp = NULL;
    file_cs_t *fcs = NULL;
    gdp = graph_desc_parser_new(&gdpi);
    if (!gdp) { err = 1; goto cleanup; }
    fcs = file_cs_new_from_path("test/test.sgph");
    if (!fcs) { err = 2; goto cleanup; }
    graph_desc_t *gd = NULL;
    err = graph_desc_parser_parse(gdp,
            (char_stream_t*)fcs,
            &gd);
    if (err) { goto cleanup; }
    _graph_desc_printf(gd);
cleanup:
    if (gdp) { graph_desc_parser_free(gdp); }
    if (fcs) { file_cs_free(fcs); }
    if (gd) { graph_desc_free(gd); }
    return err;
}

