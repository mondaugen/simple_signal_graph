HDRSUFFIXES=_decls.h _sdefs.h _defs.h
GHDRS=_graph_desc _graph_desc_parser
GHDRPREFIX=.gen/
GENHDRS=$(addprefix	$(GHDRPREFIX), \
			$(foreach suf, $(HDRSUFFIXES), \
				$(addsuffix $(suf), $(GHDRS))))
SABRVS=$(addprefix structs/, $(addsuffix .sabrv, $(GHDRS)))
GENSCRIPT=gen_struct_alloc.pl

$(GENHDRS) : $(SABRVS) $(GENSCRIPT)
	mkdir -p $(GHDRPREFIX)
	$(foreach file,$(filter %.sabrv,$^),perl \
		gen_struct_alloc.pl $(file) $(GHDRPREFIX);)

graph_desc.o : graph_desc.c $(GENHDRS) $(GENSCRIPT)
	cc -c -I$(GHDRPREFIX) $< -o $@

file_cs.o : file_cs.c
	cc -c -I. $< -o $@

test/bin/graph_desc_parse_test : test/graph_desc_parse_test.c\
   	graph_desc.c file_cs.c tsortg.c $(GENHDRS) $(GENSCRIPT)
	mkdir -p test/bin
	cc -I. -I$(GHDRPREFIX) $(filter %.c,$^) -o $@ -g
	
