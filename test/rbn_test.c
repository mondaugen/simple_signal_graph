#include <stdio.h>
#include <string.h>
#include "rbn.h"

#define PRINT_ERROR(s) fprintf(stderr,"%s,%d: %s\n",__func__,__LINE__,s);

extern int
_rbn_iter(rbn_t *r,void (*fun)(rbni_t*), void *aux);
extern void _rbn_dfs(rbn_t *r,void(*fun)(rbni_t*),size_t d, rbni_branch_t b, void *aux,rbni_t *rbni);
char
_rbn_get_color(rbn_t *r);

struct _aux_t {
    int *ints;
    char *chars;
};

void
gtree(rbni_t* info)
{
    struct _aux_t *aux = (struct _aux_t*)info->aux;
    int off = info->branch == rbni_LEFT || info->branch == rbni_ROOT ? 0 : 1;
    aux->ints[info->idx] = *(int*)info->data;
    aux->chars[info->idx] = 
        _rbn_get_color(info->node);
}

void
pints(int *ints, size_t size)
{
    while (size--) {
        printf("%d ",*ints++);
    }
    printf("\n");
}

void
pchars(char *chars, size_t size)
{
    while (size--) {
        printf("%c ", *chars++);
    }
    printf("\n");
}

int
intcmp(void *x, void *y)
{
    return (*(int*)x > *(int*)y) - (*(int*)x < *(int*)y);
}

int
main (void)
{
    int ints[] = {1,2,3,4,5,5,6,7,8,9,10,11,12,-1};
    int tary[1 << (sizeof(ints)-1)/sizeof(int)];
    char cary[1 << (sizeof(ints)-1)/sizeof(int)];
    void *pvoids[] = {
        (void*)(ints+3),
        (void*)(ints+1),
        (void*)(ints+5),
        (void*)(ints+0),
        (void*)(ints+2),
        (void*)(ints+4),
        (void*)(ints+6),
        0,0,0,0,0,0,0,
        (void*)(ints+7)
    };
    rbn_t *troot = rbn_tree_from_array(pvoids,sizeof(pvoids)/sizeof(void*));
    memset(tary,0,sizeof(tary));
    struct _aux_t aux = {tary,cary};
    rbni_t rbni;
    _rbn_dfs(troot,gtree,0,rbni_ROOT,(void*)&aux,&rbni);
    pints(tary,32);
    int *pi = ints;
    rbn_t *root = NULL;
    while (*pi != -1) {
        rbn_t *r = rbn_new(pi++);
        if (!r) {
            PRINT_ERROR("Alloc rbn.");
        }
        root = rbn_insert(root,r,intcmp);
        memset(tary,0,sizeof(tary));
        memset(cary,' ',sizeof(cary));
        _rbn_dfs(root,gtree,0,rbni_ROOT,(void*)&aux,&rbni);
        printf("Root: %d\n",*(int*)rbn_get_data(root));
        pints(tary,32);
        pchars(cary,32);
    }
    
    return 0;
}


    
    

