#pragma once
#include <stdbool.h>
#include "common.h"

typedef struct avl_tree* AVLTree;   // Incomplete definition of an AVLTree, the handle of the AVL
typedef struct avl_tree_node* Node;  // Incomplete definition of an Node

// Given a compare and a destroy function, creates a new avl tree
// and returns a pointer to it
AVLTree avl_create(CompareFunc compare, DestroyFunc destroy);


// Returns the size of the avl tree given
size_t avl_size(AVLTree tree);


// Insert, if it doesn't already exist, a new node with key "key",
// otherwise if a destroy function is given, frees any memory allocated for "key"
void avl_insert(AVLTree tree, void* key);


// Removes, if it exists, the node with key "key" and returns 1 
// and if a destroy function is given, frees any memory allocated for "key",
// otherwise returns 0 but doesn't free any memory
bool avl_remove(AVLTree tree, void* key);


// Returns the node with key "key", if it exists
// else returns NULL
Node avl_find_node(AVLTree tree, void* key);


// Returns the key of "node"
void* avl_node_value(Node node);


// Returns the next inorder node of "node"
Node avl_next(AVLTree tree, Node node);


// Returns the previous inorder node of "node"
Node avl_prev(AVLTree tree, Node node);


// Returns the right most node of the tree
Node avl_first(AVLTree tree);

// Returns the left most node of the tree
Node avl_last(AVLTree tree);


void avl_print(AVLTree tree, ActionFunc print);

void avl_inorder(AVLTree tree, ActionFunc visit);


// Destroys a tree
void avl_destroy(AVLTree tree);






