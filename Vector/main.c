#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "Vector.h"

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


int main(void)
{
    srand(time(NULL));

    Vector vector = vector_create(10, free);

    int elements = 1000000;
    int dups = 0;
    // int removals = 0;

    for (int i = 0; i < elements; i++)
    {
        int* val = create_int(rand());
        size_t size = vector_size(vector);
        vector_insert(vector, val);

        if (size == vector_size(vector))
            dups++;

        // assert(vector_find(vector, val, compare_ints));
        

        // if (rand() % 5 == 0)
        // {
        //     removals++;
        //     vector_remove(vector);
        // }
    }

    vector_qsort(vector, compare_ints);



    
    vector_destroy(vector);

    return 0;
}