/* Depth first seaching to do topological sorting and check for graph cycles.
   Based on "Introduction to Algorithms (3rd Ed.)" by Cormen et al. Ch 22. */

#include <stdlib.h>
#include <stdio.h>

typedef struct node_t node_t;
struct node_t {
    int id; /* Unique ID, optional */
    struct node_t **c;
    size_t nc; /* Number of children */
    node_t *next;
    int v; /* Visited? */ 
    int st; /* Time first visited (start time) */
    int ft; /* Time last visited (finish time) */
};

/* Get the size of a node, assuming the space to store pointers to the children
   comes right after the structure proper in memory. */
static size_t
_node_sz(node_t *nd)
{
    return sizeof(node_t)+sizeof(node_t*)*nd->nc;
}

/* If find_be non-zero, returns 1 when grey node is discovered, indicating a
   backedge in the graph, i.e., the graph is cyclic. Otherwise returns 0. */
static int
_dfs_inner(node_t *nd, node_t **dnlist, int *time, int find_be)
{
    int n, ret = 0;
    nd->v = 1;
    nd->st = *time++;
    for (n = 0; n < nd->nc; n++) {
        if (nd->c[n]) {
            if (find_be && nd->c[n]->v == 1) {
                return 1; /* Found a cycle. */ 
            }
            if (nd->c[n]->v == 0) {
                ret = _dfs_inner(nd->c[n],dnlist,time,find_be);
                if (ret) { 
                    return ret;
                }
            }
        }
    }
    nd->ft = *time++;
    nd->next = *dnlist;
    *dnlist = nd;
    nd->v = 2;
    return 0;
}

static void
_reset_graph(node_t *nds, size_t nnds)
{
    while (nnds--) {
        nds->v = 0;
        nds = (node_t*)((char*)nds+_node_sz(nds));
    }
}

/* Returns NULL if cycle found (then no topological sorting is possible). */
static node_t *
_tsort_dfs(node_t *nds, size_t nnds)
{
    int time = 0;
    node_t *dnlist = NULL;
    while (nnds--) {
        if (nds->v == 0) {
            if (_dfs_inner(nds,&dnlist,&time,1)) {
                return NULL;
            }
        }
        nds = (node_t*)((char*)nds+_node_sz(nds));
    }
    return dnlist;
}

typedef struct {
    node_t *nodeary;
    node_t **nodes;
} _nafd_ret_t;

#define _NAFD_DEFAULT (_nafd_ret_t){0,0}

static void
_nafd_ret_destroy(_nafd_ret_t *n)
{
    if (n->nodeary) {free(n->nodeary);}
}

/* Places node array and references to its nodes in nafdr.  When process
   finished with node array, call _nafd_ret_destroy(nafdr).  */
static int
_node_ary_from_desc(const int **cxns, 
                    _nafd_ret_t *nafdr)
{
    const int **cxnsptr = cxns;
/* Number of nodes */
    int nnodes = 0;
/* Total number of connections */
    int nctot = 0;
    const int *cptr;
    while ((cptr = *cxnsptr++)) {
        while (*cptr++ >= 0) { nctot++; }
        nnodes++;
    }
    size_t nodes_sz = sizeof(node_t)*nnodes
            + sizeof(node_t*)*nctot; 
    node_t *ret = calloc(1,nodes_sz
            + sizeof(node_t*)*nnodes);
    if (!ret) { return 1; } /* Error allocating */ 
    node_t **nodes = (node_t**)((char*)ret + nodes_sz);
    node_t *nodetmp = ret;
    cxnsptr = cxns;
    int n = 0;
    while ((cptr = *cxnsptr++)) {
        int nc = 0;
        nodetmp->c = (node_t**)(nodetmp+1);
        nodes[n] = nodetmp;
        while (*cptr++ >= 0) { nc++; }
        /* ID is later used to indicate which index the node is at, so this must
           be == n */
        nodetmp->id = n++;
        nodetmp->nc = nc;
        nodetmp = (node_t*)((char*)nodetmp+_node_sz(nodetmp));
    }
    n = 0;
    cxnsptr = cxns;
    while ((cptr = *cxnsptr++)) {
        int nodeidx, cidx = 0;
        while ((nodeidx = *cptr++) >= 0) { 
            nodes[n]->c[cidx++] = nodes[nodeidx];
        }
        n++;
    }
    *nafdr = (_nafd_ret_t) {
        .nodeary = ret,
        .nodes = nodes
    };
    return 0;
}

/* 
   Returns a topological sorting of graph or indicates that graph contains
   cycles and cannot be sorted.
   Input:

   a graph with n vertices in the following format: 
   {{<integers indicating 0th node's connections>,-1},
    {<integers indicating 1st node's connections>,-1},
    ...,
    {<integers indicating (n-1)th node's connections,-1},
    NULL
   }
   i.e., a NULL terminated array of -1-terminated integer arrays indicating the
   node connections. If the node connections are out of range, non-zero is
   returned and the ordering of the graph is invalid.
   
   an integer array of length n which will contain the ordering upon completion
   if the return value is 0.

   Output:

   Non-zero if the graph description invalid or if graph contains cycles and cannot be sorted.
   If zero returned, "order" contains the topologically sorted graph.
*/
int
tsg_compute(const int **graph, int n_nodes, int *order)
{
    int ret = 0; 
    const int **cxnsptr;
    _nafd_ret_t nodeinfo = _NAFD_DEFAULT;
    node_t *ordered;
    if (_node_ary_from_desc(graph,&nodeinfo)) {
        ret = 1; goto end;
    }
    cxnsptr = graph;
    ordered = _tsort_dfs(nodeinfo.nodeary,n_nodes);
    if (!ordered) {
        ret = 2; goto end;
    }
    while (ordered) {
        *order++ = ordered->id;
        ordered = ordered->next;
    }
end:
    _nafd_ret_destroy(&nodeinfo);
    return ret;
}

#ifdef TEST

/* Does -1 terminated array contain y? */
static int
_contains_int(const int *x, const int y)
{
    while (*x >= 0) { if (*x++ == y) { return 1; } }
    return 0;
}

/* Test it actually does a topological sort. */
static int
_test1(void)
{
    const int *cxns[] = {
        (int[]){7,-1},
        (int[]){4,0,-1},
        (int[]){4,0,7,-1},
        (int[]){8,0,-1},
        (int[]){0,-1},
        (int[]){7,8,3,-1},
        (int[]){1,7,-1},
        (int[]){-1},
        (int[]){4,6,1,-1},
        (int[]){0,3,4,-1},
        0,
    };
    int order[sizeof(cxns)/sizeof(int*)-1],
        tsgcr = 0;
    tsgcr = tsg_compute(cxns,order);
    if(tsgcr == 1) {
        fprintf(stderr,"Error building node array from description.\n");
        return 1;
    } else if (tsgcr == 2) {
        printf("Graph contains cycle.\n");
        return 1;
    }
    printf("order:");
    int correct_order[] = {9, 5, 2, 3, 8, 6, 1, 4, 0};
    int n;
    int ret = 0;
    for (n = 0; n < sizeof(order)/sizeof(int); n++) {
        int m = 0;
        for (m = 0; m <= n; m++) {
            if (_contains_int(cxns[order[n]],order[m])) {
                printf("Graph not topologically sorted!\n");
                ret = 1;
            }
        }
        printf(" %d",order[n]);
    }
    printf("\n");
    return ret;
}

/* Test for cycles. */
static int
_test2(void)
{
    const int *cxns[] = {
        (int[]){-1},
        (int[]){0,2,-1},
        (int[]){0,3,-1},
        (int[]){1,-1},
        0,
    };
    int order[sizeof(cxns)/sizeof(int*)-1],
        tsgcr = 0;
    tsgcr = tsg_compute(cxns,order);
    if(tsgcr == 1) {
        fprintf(stderr,"Error building node array from description.\n");
        return 1;
    } else if (tsgcr == 2) {
        printf("Graph contains cycle.\n");
        return 0;
    }
    return 1;
}

int
main(void)
{
    printf("Test 1, can we topologically sort a directed acyclic graph?\n");
    if (_test1()) {
        printf("failed");
    } else {
        printf("passed");
    }
    printf("\n");
    printf("Test 2, can we determine if a graph has cycles?\n");
    if (_test2()) {
        printf("failed");
    } else {
        printf("passed");
    }
    printf("\n");
    return 0;
}
#endif /* TEST */ 
