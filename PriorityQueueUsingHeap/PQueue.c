#include <stdlib.h>
#include <assert.h>
#include "PQueue.h"


// To make things more uniform-easier on removal
struct node 
{
	void* value;				
};

struct priority_queue 
{
	Node array;			
	int size;					
	int capacity;				
	CompareFunc compare;
	DestroyFunc destroy;	
};


static void swap(Node a, Node b)
{
	Node temp = b->value;
	b->value = a->value;
	a->value = temp;
}


static void insert(PQueue pq, size_t node) 
{
	// We got to the root -> we are done
	if (node == 1)
		return;

	size_t parent = node / 2;	// The parent of the node

	// If parent.value < node.value swap and continue up
	if (pq->compare(pq->array[parent].value, pq->array[node].value) < 0) 
	{
		swap(&pq->array[parent], &pq->array[node]);
		insert(pq, parent);
	}
}


static void remove(PQueue pq, size_t node) 
{
	// Find the node's children
	size_t left = 2 * node;
	size_t right = left + 1;

	size_t size = pq->size;

	// If they dont exist we are done
	if (left > size)
		return;

	// Find the max child
	size_t max = left;

	if (right <= size && pq->compare(pq->array[left].value, pq->array[right].value) < 0)
			max = right;

	// If node.value < max.value swap and continue down
	if (pq->compare(pq->array[node].value, pq->array[max].value) < 0) 
	{
		swap(&pq->array[node], &pq->array[max]);
		remove(pq, max);
	}
}


PQueue pq_create(CompareFunc compare, DestroyFunc destroy) 
{	
	PQueue pq = malloc(sizeof(*pq));
	Node array = calloc(HEAP_MIN_CAPACITY, sizeof(*array));		// Initialize values with 0

	
	assert(compare != NULL); 
	assert(pq != NULL && array != NULL); // Check for malloc failure

	// Initialization
	pq->size = 0;
	pq->capacity = HEAP_MIN_CAPACITY;
	pq->array = array;
	pq->compare = compare;
	pq->destroy = destroy;	

	return pq;
}


int pq_empty(PQueue pq)
{
	assert(pq != NULL); 
	return pq->size == 0;
}


int pq_full(PQueue pq)
{
	assert(pq != NULL); 
	return 0;
}


Node pq_top(PQueue pq)
{
	assert(pq != NULL); 
	return &pq->array[1];
}

void* pq_node_value(Node node)
{
	assert(node);
	return node->value;
}


void pq_insert(PQueue pq, void* value)
{
	assert(pq != NULL); 
	
	
	// Increase the size of Heap by 1
	pq->size++;
	
	
	// Grow the array, if we need to, by doubling its size, for better time complexity
	if (pq->capacity == pq->size) 
	{
		Node new_array = realloc(pq->array, 2 * pq->capacity * sizeof(*new_array));

		assert(new_array != NULL); // Check for malloc failure
	

		pq->capacity *= 2;
		pq->array = new_array; // We don't need to free the last value, realloc does it already
	}

	// Insert the element at end of Heap
	pq->array[pq->size].value = value;
	

    // Heapify the new node following a bottom-up approach
	insert(pq, pq->size);
    
}


void pq_remove(PQueue pq)
{
	assert(pq != NULL && pq->size != 0); 

	swap(&pq->array[1], &pq->array[pq->size]);

	// We call the destroy function, if it exists.
	if (pq->destroy != NULL)
		pq->destroy(pq->array[pq->size].value);

	
	pq->size--;

	remove(pq, 1);
}


void pq_reinit(PQueue pq)
{

	if (pq->destroy != NULL)
	{
		for (size_t i = 1; i <= pq->size; i++)
			pq->destroy(pq->array[i].value);
	}

	free(pq->array);
	pq->array = calloc(HEAP_MIN_CAPACITY, sizeof(*pq->array));

	pq->size = 0;

}


void pq_destroy(PQueue pq)
{
	// We destroy every value, if there is a destroy function
	if (pq->destroy != NULL)
	{
		for (size_t i = 1; i <= pq->size; i++)
			pq->destroy(pq->array[i].value);
	}

	// Free array and vector struct
	free(pq->array);
	free(pq);		
}


