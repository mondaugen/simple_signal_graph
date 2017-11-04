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
size_t
node_sz(node_t *nd)
{
    return sizeof(node_t)+sizeof(node_t*)*nd->nc;
}

void
_dfs_inner(node_t *nd, node_t **dnlist, int *time)
{
    int n;
    nd->v = 1;
    nd->st = *time++;
    for (n = 0; n < nd->nc; n++) {
        if (nd->c[n] && nd->c[n]->v == 0) {
            _dfs_inner(nd->c[n],dnlist,time);
        }
    }
    nd->ft = *time++;
    nd->next = *dnlist;
    *dnlist = nd;
    nd->v = 2;
}

node_t *
node_dfs(node_t *nds, size_t nnds)
{
    int time = 0;
    node_t *dnlist = NULL;
    while (nnds--) {
        if (nds->v == 0) {
            _dfs_inner(nds,&dnlist,&time);
        }
        nds = (node_t*)((char*)nds+node_sz(nds));
    }
    return dnlist;
}

struct nafd_ret_t {
    node_t *nodeary;
    node_t **nodes;
}
node_ary_from_desc(int **cxns)
{
    int **cxnsptr = cxns;
/* Number of nodes */
    int nnodes = 0;
/* Total number of connections */
    int nctot = 0;
    int *cptr;
    while ((cptr = *cxnsptr++)) {
        while (*cptr++ >= 0) { nctot++; }
        nnodes++;
    }
    size_t nodes_sz = sizeof(node_t)*nnodes
            + sizeof(node_t*)*nctot; 
    node_t *ret = calloc(1,nodes_sz
            +sizeof(node_t*)*nnodes);
    if (!ret) { return (struct nafd_ret_t) {NULL,NULL}; }
    node_t **nodes = (node_t**)((char*)ret + nodes_sz);
    node_t *nodetmp = ret;
    cxnsptr = cxns;
    int n = 0;
    while ((cptr = *cxnsptr++)) {
        int nc = 0;
        nodetmp->c = (node_t**)(nodetmp+1);
        nodes[n] = nodetmp;
        while (*cptr++ >= 0) { nc++; }
        nodetmp->id = n++;
        nodetmp->nc = nc;
        nodetmp = (node_t*)((char*)nodetmp+node_sz(nodetmp));
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
    return (struct nafd_ret_t) {ret, nodes};
}

int
main (void)
{
    int *cxns[] = {
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
    struct nafd_ret_t nodeinfo = node_ary_from_desc(cxns);
    if (!nodeinfo.nodeary) {
        return 1;
    }
    int n = 0, **cxnsptr = cxns;
    while (*cxnsptr++) { n++; }
    node_t *ordered = node_dfs(nodeinfo.nodeary,n);
    printf("order:");
    while (ordered) {
        printf(" %d",ordered->id);
        ordered = ordered->next;
    }
    printf("\n");
    free(nodeinfo.nodeary);
    return 0;
}
