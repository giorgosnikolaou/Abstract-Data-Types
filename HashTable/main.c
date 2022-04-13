#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "hash.h"

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


size_t hash1(void* val)
{
    return *(int*)val;
}


size_t hash2(void* val)
{
    int prime = 13;
    return (prime - (*(int*)val % prime));

}



int main(void)
{
    srand(time(NULL));

    HashTable hash = hash_create(hash1, hash2, compare_ints, free);

    
    int elements = 10000000;
    int dups = 0;
    int removals = 0;

    for (int i = 0; i < elements; i++)
    {
        int* val = create_int(rand());
        size_t size = hash_size(hash);
        hash_insert(hash, val);

        if (size == hash_size(hash))
            dups++;

        assert(hash_find(hash, val));
        

        if (rand() % 5 == 0)
        {
            removals++;
            assert(hash_remove(hash, val));
        }
    }


    printf("Duplicates inserted: %d\nRemovals: %d\nCorrect sum: %s\nSize: %ld\nCapacity: %ld\n\n", dups, removals, (removals + dups +  hash_size(hash) == elements) ? "true" : "false", hash_size(hash), hash_capacity(hash));

    hash_destroy(hash);

    hash = hash_create(hash1, hash2, compare_ints, free);

    elements = 20;

    for (int i = 0; i < elements; )
    {
        int* val = create_int(rand() % (2 * elements) + elements);
        
        size_t size = hash_size(hash);
        hash_insert(hash, val);

        if (size < hash_size(hash))
        {
            i++;
            printf("%d ", *val);
        }
    }
    printf("\n\n");

    for (Node node = hash_first(hash); node != NULL; node = hash_next(hash, node))
    {
        int val = *(int*)hash_node_value(node);
        printf("%d ", val);
    }
    printf("\n");

    hash_destroy(hash);

    return 0;
}