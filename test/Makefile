BINDIR=test/bin

rbn_test: queue.c rbn.c test/rbn_test.c
	cc -o $(BINDIR)/$@ $^ -I. -g

.gen/_graph_desc_decls.h .gen/_graph_desc_defs.h: structs/_graph_desc.sabrv
	mkdir -p .gen;
	perl gen_struct_alloc.pl $< .gen;

graph_desc_init_test: graph_desc.c test/graph_desc_init_test.c .gen/_graph_desc_decls.h .gen/_graph_desc_defs.h
	cc -I . -I .gen -o $(BINDIR)/$@ $(filter %.c,$^) -g
