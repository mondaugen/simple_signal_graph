HDRSUFFIXES=_decls.h _sdefs.h _defs.h
GHDRS=_graph_desc _graph_desc_parser
GHDRPREFIX=.gen/
GENHDRS=$(addprefix	$(GHDRPREFIX), \
			$(foreach suf, $(HDRSUFFIXES), \
				$(addsuffix $(suf), $(GHDRS))))
SABRVS=$(addprefix structs/, $(addsuffix .sabrv, $(GHDRS)))

$(GENHDRS) : $(SABRVS)
	$(foreach file,$^,perl gen_struct_alloc.pl $(file) $(GHDRPREFIX);)

graph_desc.o : graph_desc.c $(GENHDRS)
	cc -I$(GHDRPREFIX) $< -o $@
