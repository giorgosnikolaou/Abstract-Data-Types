#pragma once
#include <stdbool.h>
#include "common.h"

typedef struct hash_table* HashTable;
typedef struct hash_node* Node;

typedef size_t (*HashFunc)(void* value);



#define HASH_INT (HashFunc)1
#define HASH_INT2 (HashFunc)2



// Creates a Hashtable with starting capacity 31
// The modules uses double hashing 
// If the second hash function is NULL it will just use linear probing instead
HashTable hash_create(HashFunc hash, HashFunc sec_hash, CompareFunc compare, DestroyFunc destroy);

// Inserts a value to the hashtable
// If the value already exists replaces the old value
// and if the destroy function given when creating 
// the hashtable wasn't NULL, destroys the old value 
void hash_insert(HashTable ht, void* val);

// Removes, destroys a value and returns true, if it exists
// otherwise returns false 
bool hash_remove(HashTable ht, void* val);

// Returns the pointer to the value, if it exists
// Otherwise returns NULL
Node hash_find(HashTable ht, void* val);

void* hash_node_value(Node node);


// Returns the current capacity of the hashtable
size_t hash_capacity(HashTable ht);

// Returns the number of elements currently stored in the hashtable
size_t hash_size(HashTable ht);

// Destroys a hashtable
void hash_destroy(HashTable ht);


Node hash_first(HashTable hash);

Node hash_next(HashTable hash, Node node);

Node hash_get_at(HashTable hash, size_t ind);


void hash_print(HashTable ht, ActionFunc print);


