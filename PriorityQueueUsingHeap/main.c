#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "PQueue.h"

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

int compare_ints_neg(void* a, void* b)
{
    return -compare_ints(a, b);
}

int main(void)
{
    srand(time(NULL));

    size_t size = rand() % 20 + 20;

    PQueue pq_max = pq_create(compare_ints, free);
    PQueue pq_min = pq_create(compare_ints_neg, free);

    for (size_t i = 0; i < size; i++)
    {
        int num = rand() % size;
        pq_insert(pq_max, create_int(num));
        pq_insert(pq_min, create_int(num));
        printf("%d ", num);       
    }
    printf("\n\n");

    while (!pq_empty(pq_max))
    {
        int val = *(int*)pq_node_value(pq_top(pq_max));
        printf("%d ", val);
        pq_remove(pq_max);
    }
    printf("\n");

    while (!pq_empty(pq_min))
    {
        int val = *(int*)pq_node_value(pq_top(pq_min));
        printf("%d ", val);
        pq_remove(pq_min);
    }
    printf("\n");


    pq_destroy(pq_max);
    pq_destroy(pq_min);

    return 0;
}