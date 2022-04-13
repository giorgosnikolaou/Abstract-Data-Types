#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "AVLTree.h"

int* create_int(int val)
{
    int* p = malloc(sizeof(int));
    *p = val;
    return p;
}


int compare_ints(void* a, void* b)
{
    return *(int*)a - *(int*)b;
}

void print_int(void* a)
{
    printf("%d ", *(int*)a);
}

void test_functionality(void)
{

    AVLTree avl = avl_create(compare_ints, free);

    
    int elements = 1000000;
    int dups = 0;
    int removals = 0;

    for (int i = 0; i < elements; i++)
    {
        int* val = create_int(rand());

        int v = *val;
        size_t size = avl_size(avl);
        avl_insert(avl, val);

        if (size == avl_size(avl))
            dups++;

        assert(avl_find_node(avl, &v));        

        if (rand() % 5 == 0)
        {
            removals++;
            assert(avl_remove(avl, &v));
        }
    }

    assert(removals + dups +  avl_size(avl) == elements);

    // printf("Duplicates inserted: %d\nRemovals: %d\nSize: %d\n\n", dups, removals, avl_size(avl));
    printf("Duplicates inserted: %d\nRemovals: %d\nCorrect sum: %s\nSize: %ld\n\n", dups, removals, (removals + dups +  avl_size(avl) == elements) ? "true" : "false", avl_size(avl));
    avl_destroy(avl);

    avl = avl_create(compare_ints, free);

    elements = 20;

    for (int i = 0; i < elements; )
    {
        int* val = create_int(rand() % (2 * elements) + elements);
        
        size_t size = avl_size(avl);
        avl_insert(avl, val);

        if (size < avl_size(avl))
        {
            i++;
            printf("%d ", *val);
        }
    }
    printf("\n\n");

    for (AVLNode node = avl_first(avl); node != NULL; node = avl_next(avl, node))
    {
        int val = *(int*)avl_node_value(node);
        printf("%d ", val);
    }
    printf("\n");

    avl_inorder(avl, print_int);
    printf("\n");

}


int main(void)
{
    srand(time(NULL));
    test_functionality();

    return 0;


}