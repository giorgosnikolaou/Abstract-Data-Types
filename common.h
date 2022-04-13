#pragma once
#include "common.h"

// Pointer to a fuction that compares 2 pointers a and b and returns:
// Defines the relationship between values
typedef int (*CompareFunc)(void* a, void* b);

// Pointer to a function that frees the memory allocated for the value
typedef void (*DestroyFunc)(void* value);

typedef void (*ActionFunc)(void* value);