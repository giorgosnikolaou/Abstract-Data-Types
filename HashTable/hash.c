#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "hash.h"


#define MIN_HASH_SIZE 31
#define MAX_LOAD_FACTOR 0.5

// To make things more uniform-easier on removal
struct hash_node 
{
	void* value;	
    int is_used;	
    size_t index;		
};


struct hash_table
{
	Node array;			
	size_t size;					
	size_t capacity;		
    HashFunc hash;	
    HashFunc sec_hash;	
    CompareFunc compare;
	DestroyFunc destroy;	
};


static size_t prime_sizes[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
	786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741, 3221225479, 6442450967};


// Premade hash functions

static size_t hash_int(void* val)
{
    return *(int*)val;
}


static size_t hash_int2(void* val)
{
    int prime = 13;
    return (prime - (*(int*)val % prime));

}


static size_t hash_linear_probing(void* val)
{
    return 1;
}


static HashFunc set_hash_func(HashFunc hash)
{
    if (hash == NULL)
        return hash_linear_probing;
    else if (hash == HASH_INT)
        return hash_int;
    else if (hash == HASH_INT2)
        return hash_int2;
    
    

    return hash;
}


static Node create_array(HashTable ht)
{
	Node new_array = calloc(ht->capacity, sizeof(*new_array));
    assert(new_array);

    for (size_t i = 0; i < ht->capacity; i++)
        new_array[i].index = i;
    
    return new_array;
}

static void rehash(HashTable ht) 
{
	size_t old_capacity = ht->capacity;
    Node old_array = ht->array;

    size_t prime_no = sizeof(prime_sizes) / sizeof(size_t);    

    for (size_t i = 0; i < prime_no; i++) 
    {                    
        if (prime_sizes[i] > old_capacity) 
        {
            ht->capacity = prime_sizes[i]; 
            break;
        }
    }

    if (ht->capacity == old_capacity)                    
        ht->capacity *= 2;
    
    // her just to avoid annoying gcc -Werror error 

	Node new_array = create_array(ht);

    ht->array = new_array; 

	ht->size = 0;
    
    
	for (int i = 0; i < old_capacity; i++)
	{
        if (old_array[i].is_used == 1)
			hash_insert(ht, old_array[i].value);
        
    }

	free(old_array);
}


static size_t find_node(HashTable ht, void* val)
{
    for (size_t index = ht->hash(val) % ht->capacity, step = ht->sec_hash(val), adj = 0, count = 0, ind = index;
         count != ht->capacity && ht->array[ind].is_used != 0; 
         count++, adj += step, ind = (index + adj) % ht->capacity)
    {
        if (ht->array[ind].is_used == 1 && ht->compare(ht->array[ind].value, val) == 0)
            return ind;
        
    }   

    return ht->capacity;
}



HashTable hash_create(HashFunc hash, HashFunc sec_hash, CompareFunc compare, DestroyFunc destroy)
{
    assert(hash && compare);
    
    HashTable ht = malloc(sizeof(*ht));
    assert(ht);
    

    ht->capacity = MIN_HASH_SIZE;
    ht->size = 0;
    ht->hash = set_hash_func(hash);
    ht->sec_hash = set_hash_func(sec_hash);
    ht->compare = compare;
    ht->destroy = destroy;
    ht->array = create_array(ht);

    return ht;
}


void hash_insert(HashTable ht, void* val)
{
    assert(ht && val); 
	
    
    size_t index = ht->hash(val) % ht->capacity;
  
    size_t step = ht->sec_hash(val);

    size_t del_ind = ht->capacity + 1;
    

    for (size_t j = 0, adj = 0; j < ht->capacity; j++, adj += step)
    {

        size_t index2 = (index + adj) % ht->capacity;

        if (ht->array[index2].is_used == 0) 
        {
            index = (del_ind != ht->capacity + 1) ? del_ind : index2;
            break;
        }
        else if (ht->array[index2].is_used == -1) 
        {
            if (del_ind == ht->capacity + 1)
                del_ind = index2;

        }
        else if(ht->compare(ht->array[index2].value, val) == 0) // If the key is already in the hastable replace it with the new and destroy the old 
        {

            if (ht->destroy)
                ht->destroy(ht->array[index2].value);
            
            ht->array[index2].value = val;

            return ;
        }
    }


    ht->array[index].value = val;
    ht->array[index].is_used = 1;
    ht->size++;

    float load_factor = (float)ht->size / ht->capacity;
	if (load_factor > MAX_LOAD_FACTOR)
        rehash(ht);

}


Node hash_find(HashTable ht, void* val)
{
    assert(ht && val);

    size_t ind = find_node(ht, val);
    
    return (ind != ht->capacity) ? &ht->array[ind] : NULL;
}

void* hash_node_value(Node node)
{
    assert(node);
    return node->value;
}


bool hash_remove(HashTable ht, void* val)
{
    assert(ht && val);
 

    size_t ind = find_node(ht, val);

    if (ind != ht->capacity)
    {
        if (ht->destroy)
            ht->destroy(ht->array[ind].value);
        
        
        ht->array[ind].is_used = -1;
        ht->array[ind].value = NULL;
        ht->size--;

        return true;
    }

    return false; 
}


size_t hash_capacity(HashTable ht)
{
    return ht->capacity;
}


size_t hash_size(HashTable ht)
{
    return ht->size;
}


void hash_destroy(HashTable ht)
{
    for (size_t i = 0; i < ht->capacity; i++)
    {
        if (ht->destroy && ht->array[i].value)
            ht->destroy(ht->array[i].value);
        
    }

    free(ht->array);
    free(ht);
}


void hash_print(HashTable ht, ActionFunc print)
{
    for (size_t i = 0; i < ht->capacity; i++)
    {
        if (ht->array[i].is_used == 1 )
            print(ht->array[i].value);
    }
}


Node hash_next(HashTable hash, Node node)
{
    assert(hash && node);

    for (size_t i = node->index + 1; i < hash->capacity; i++)
    {
        if (hash->array[i].is_used == 1)
            return &hash->array[i];
    }

    return NULL;
}

Node hash_get_at(HashTable hash, size_t ind)
{
    assert(hash && ind >= 0 && ind < hash->capacity);

    return hash->array[ind].is_used == 1 ? hash->array[ind].value : NULL;

}

Node hash_first(HashTable hash)
{
    for (size_t i = 0; i < hash->capacity; i++)
    {
        if (hash->array[i].is_used == 1)
            return &hash->array[i];
    }
    return NULL;
}
