#pragma once
#include "common.h"


// Handle for the Priority Queue
typedef struct priority_queue* PQueue;
typedef struct node* Node;

// Minimum size of heap
#define HEAP_MIN_CAPACITY 10

// Creates and returns a priority queue 
// where its elements are compared using the function compare
// and destroyed using the function destroy if destroy != NULL
PQueue pq_create(CompareFunc compare, DestroyFunc destroy);


// Returns 1 if priority queue is empty otherwise 0
int pq_empty(PQueue pq);


// Returns 1 if priority queue is full otherwise 0
int pq_full(PQueue pq);


// Returns the node 
Node pq_top(PQueue pq);

void* pq_node_value(Node node);

// Adds value value in priority queue
void pq_insert(PQueue pq, void* value);


// Removes the biggest value of the priority queue
void pq_remove(PQueue pq);


// Reinitializes the priority queue 
void pq_reinit(PQueue pq);

// Frees all the memory allocated for the priority queue 
// and its values (if a destroy function is declared on the creation of it)
void pq_destroy(PQueue pq);
