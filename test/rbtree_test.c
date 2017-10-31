#include "rbtree.h"
#include <stdio.h>
#include <string.h>

int intcmp(void *_x, void *_y)
{
    int x = *(int*)_x, y = *(int*)_y;
    return x < y ? -1 : 1;
}

typedef struct int_tree {
    int *x;
    int size;
    int tog;
} int_tree;

void intprint(int depth, void *_dat, void *_aux)
{
    int dat = *(int*)_dat;
    int_tree *aux = (int_tree*)_aux;
    aux->x[depth*2 - 1 + aux->tog] = dat;
    aux->tog = 1 - aux->tog;
}

void print_int_tree(int_tree *it)
{
    int i;
    for (i = 0; i < it->size; i++) {
        printf("%d ", it->x[i]);
    }
    printf("\n");
}

int main (void)
{
    int ints[] = {1,2,3,4,5,6,7,8,8,9,10};
    int i;
    int_tree it;
    int _x[20];
    it.x = _x;
    it.size = sizeof(_x)/sizeof(int);
    rbtree_t *rbt = rbtree_new((void*)ints);
    for (i = 1; i <= sizeof(ints)/sizeof(int); i++) {
        memset(&_x,0,sizeof(_x));
        it.tog = 1;
        rbtree_eval(rbt, intprint, &it);
        print_int_tree(&it);
        rbt = rbtree_insert(rbt,rbtree_new((void*)(ints+i)),intcmp);
    }
    return 0;
}
