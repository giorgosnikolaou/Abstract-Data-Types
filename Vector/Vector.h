#pragma once
#include <stdbool.h>
#include "common.h"

typedef struct vector* Vector;


// Pointer to a function that prints the value of a and a single whitespace after that
// ex 1.  printf("%5d ", *(int*)a);
//    2.  printf("%2.4f ", *(float)a);
// Logic applies on structs as well
typedef void (*PrintFunc)(void* a);


// Creates and retruns a vector
// Size is the minimum starting size for the vector 
Vector vector_create(size_t size, DestroyFunc destroy);

// Returns vector's size
size_t vector_size(Vector vec);


// Duplicates are allowed 

// Inserts value to the end of the vector
void vector_insert(Vector vec, void* value);

// Inserts value in the vector keeping an in increasing order
// "Increasing" depends on the compare function
void vector_insert_sorted(Vector vec, void* value, CompareFunc compare);

// Inserts value at an index on the vector  
void vector_insert_at(Vector vec, size_t index, void* value);

// Removes the last element of the vector
void vector_remove(Vector vec);

// Removes the elements of index ind from the vector
void vector_remove_at(Vector vec, size_t ind);

// Returns the value at index ind
void* vector_get_at(Vector vec, size_t ind);

// Sets the value at index ind
void* vector_set_at(Vector vec, size_t ind, void* value);

// Finds and returns value, if it exists, otherwise returns NULL
// Linear search
void* vector_find(Vector vec, void* value, CompareFunc compare);

// Same as above but uses binary search
// Vector should be in inccreasing order defined by compare
void* vector_bsearch(Vector vec, void* value, CompareFunc compare);

// Uses quicksort to sort the array
void vector_qsort(Vector vec, CompareFunc compare);

// Prints all the elements of the vector
void vector_print(Vector vec, PrintFunc print);

// Destroys the vector 
void vector_destroy(Vector vec);




