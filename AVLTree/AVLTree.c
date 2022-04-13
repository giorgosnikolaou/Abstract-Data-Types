#include <stdlib.h> 
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "AVLTree.h"


struct avl_tree
{
    Node root;
    size_t size;
    CompareFunc compare;
    DestroyFunc destroy;
};


// An AVL tree node 
struct avl_tree_node
{ 
    void* key; 
    Node left; 
    Node right; 
    int height; 
}; 



// Helper functions

static void free_node(Node node, DestroyFunc destroy)
{
    if (destroy)
        destroy(node->key);
    
    free(node);
}


// Returns the height of the subtree rooted with node 
static int height(Node node) 
{ 
    return node ? node->height : 0; 
} 

static int update_height(Node node) 
{ 
    int left = height(node->left);
    int right = height(node->right);

    return ((left > right) ? left : right) + 1; 
} 


// Allocates a new node with the given key and sets its children as NULL. 
static Node new_node(void* key) 
{ 
    Node node = malloc(sizeof(*node)); 

    assert(node);

    node->key   = key; 
    node->left   = NULL; 
    node->right  = NULL; 
    node->height = 1;  // new node is initially added at leaf 
    return node; 
} 


// Standard left, right rotation, updating heights as well
static Node rotate_left(Node node)
{
    Node right = node->right;
    Node right_left = right->left;

    right->left = node;
    node->right = right_left;

    node->height = update_height(node);
    right->height = update_height(right);

    return right;
}

static Node rotate_right(Node node)
{
    Node left = node->left;
    Node left_right = left->right;

    left->right = node;
    node->left = left_right;

    node->height = update_height(node);
    left->height = update_height(left);

    return left;
}


static int balance_factor(Node node) 
{ 
    if (node == NULL) 
        return 0; 

    return height(node->left) - height(node->right); 
} 
  
static Node fix_balance(AVLTree tree, Node node)
{
    node->height = update_height(node);

    int balance = balance_factor(node);

    if (balance > 1)    // Left - Right or Left - Left case
    {
        if (balance_factor(node->left) < 0)
            node->left = rotate_left(node->left);
        
        return rotate_right(node);
    }
    else if (balance < -1)  // Right - Left or Right - Right case
    {
        if (balance_factor(node->right) > 0)
            node->right = rotate_right(node->right);
        
        return rotate_left(node);
    }
    

    // Node isn't unbalanced
    return node;

}


static Node node_find_max(Node node) 
{
    // If there exist a right subtree the max is there, otherwise it's the node itself
	return node && node->right ? node_find_max(node->right)	: node;									
}

static Node node_find_min(Node node) 
{
    // If there exist a left subtree the min is there, otherwise it's the node itself
	return node && node->left ? node_find_min(node->left) : node;
}

// Stores the largest value of a subtree to key
// Removes its node and fixes the heights going up the stack
static Node remove_max(AVLTree tree, Node node, void** key)
{
    if (!node->right) 
    {
        Node left = node->left;
		*key = node->key;       

        free(node);

		return left;		
	} 
    else 
    {
		node->right = remove_max(tree, node->right, key);

		return fix_balance(tree, node);
	}
}


static Node insertR(AVLTree tree, Node root, void* key)
{
    if (!root)
    {
        tree->size++;
        return new_node(key);
    }

    void* root_key = root->key;

    int comp = tree->compare(root_key, key);

    if (comp > 0)
        root->left = insertR(tree, root->left, key);    
    else if (comp < 0)
        root->right = insertR(tree, root->right, key);
    else 
    {
        if (tree->destroy)
            tree->destroy(root_key);
        
        root->key = key;
        return root;
    }
    


    return fix_balance(tree, root);

}

static Node removeR(AVLTree tree, Node root, void* key)
{
    if (!root)
        return NULL;
    
    int comp = tree->compare(root->key, key);

    if (comp > 0)
        root->left = removeR(tree, root->left, key);
    else if (comp < 0)
        root->right = removeR(tree, root->right, key);
    else
    {
        tree->size--;

        if (!root->left)
        {
            Node new_root = root->right;
            free_node(root, tree->destroy);
            return new_root;
        }

        if (!root->right)
        {
            Node new_root = root->left;
            free_node(root, tree->destroy);
            return new_root;
        }

        // Node is not a leaf
        // Find its inorder predecessor, store its value to the root 
        // and free the old value, the one the user wants to remove

        void* new_key = NULL;
        root->left = remove_max(tree, root->left, &new_key);
        
        if (tree->destroy)
            tree->destroy(root->key);

        root->key = new_key;

        return fix_balance(tree, root);
    }

    if (!root)
        return NULL;
    
    return fix_balance(tree, root);

}

static void destroyR(Node root, DestroyFunc destroy)
{
    if (!root)
        return ;
    
    destroyR(root->left, destroy);
    destroyR(root->right, destroy);

    free_node(root, destroy);
}


// Correctly allinging the childer of each node 
static void pt(int tabs)
{
    for (int i = 0; i < tabs; i++)
        printf("\t");
}

static void printR(Node root, ActionFunc print, int tabs)
{
    
    pt(tabs);

    if (!root)
    {
        printf("--\n");
        return ;
    }

    print(root->key);
    printf("\n");
    // printf("height: %d\n", root->height);

    if (!root->left && !root->right)
        return ;
    
    printR(root->left, print, tabs + 1);
    printR(root->right, print, tabs + 1);

}

static void inorderR(Node root, ActionFunc visit)
{
    if (!root)
        return ;
    
    inorderR(root->left, visit);

    visit(root->key);

    inorderR(root->right, visit);
}


static Node find_node(Node node, CompareFunc compare, void* key)
{
    if (!node)
        return NULL;    // Not found

    int comp = compare(key, node->key);

    if (comp < 0)   // key < node->key and therfore key is located to the left subtree of node
        return find_node(node->left, compare, key);
    else if (comp > 0)  // key > node->key and therfore key is located to the right subtree of node
        return find_node(node->right, compare, key);
    
    return node;
}

static Node find_prev(Node node, CompareFunc compare, Node target) 
{
	// target: the node we want to find the previous of
    // node: "parent" of node. Starts with tree->root
    if (node == target) 
    {
		// There are two cases: We find the target node and it has a left subree, therefore 
        // the previous node will be the left most node of the left subtrre
        // or we find the target node and it doesn't have a left subtree, therefore
        // the previous node is the parent node. That is target == prev_node->right
        // In the second case the function will return NULL and this: return res != NULL ? res : node; will return the "parent" node of target
		return node_find_max(node->left);

	} 
    else if (compare(target->key, node->key) < 0) 
    {
		// target->key < node->key and therefore we go to the left subtree
        // In this case the previous node will be either the right most node of the left subtrre or target itself will be the smallest node of the tree
		return find_prev(node->left, compare, target);

	} 


    // target->key > node->key and therefore we go to the right subtree
    Node res = find_prev(node->right, compare, target);
    return res ? res : node;
	
}

static Node find_next(Node node, CompareFunc compare, Node target) 
{
	// target: the node we want to find the next of
    // node: "parent" of node. Starts with tree->root
    if (node == target) 
    {
		// There are two cases: We find the target node and it has a right subree, therefore 
        // the next node will be the left most node of the right subtrre
        // or we find the target node and it doesn't have a right subtree, therefore
        // the next node is the parent node. That is target == prev_node->left
        // In the second case the function will return NULL and this: return res != NULL ? res : node; will return the "parent" node of target
		return node_find_min(node->right);

	} 
    else if (compare(target->key, node->key) > 0) 
    {
		// target->key > node->key and therefore we go to the right subtree
        // In this case the next node will be either the left most node of the right subtrre or target itself will be the biggest node of the tree
		return find_next(node->right, compare, target);

	} 


    // target->key < node->key and therefore we go to the left subtree
    Node res = find_next(node->left, compare, target);
    return res ? res : node;
	
}




// Header functions

AVLTree avl_create(CompareFunc compare, DestroyFunc destroy)
{
    assert(compare != NULL);

    AVLTree tree = malloc(sizeof(*tree));

    assert(tree != NULL);

    tree->root = NULL;
    tree->size = 0;
    tree->compare = compare;
    tree->destroy = destroy;

    return tree;

}


size_t avl_size(AVLTree tree)
{
    assert(tree);
    return tree->size;
}


void avl_insert(AVLTree tree, void* key)
{
    assert(tree);

    tree->root = insertR(tree, tree->root, key);
}

bool avl_remove(AVLTree tree, void* key)
{
    assert(tree && key);

    int old_size = tree->size;

    tree->root = removeR(tree, tree->root, key);

    return old_size > tree->size;


}


Node avl_first(AVLTree tree)
{
    assert(tree);
    return node_find_min(tree->root);
}

Node avl_last(AVLTree tree)
{
    assert(tree);
    return node_find_max(tree->root);
}


void avl_print(AVLTree tree, ActionFunc print)
{
    assert(tree);
    printR(tree->root, print, 0);
}

void avl_inorder(AVLTree tree, ActionFunc visit)
{
    assert(tree);
    inorderR(tree->root, visit);
}


Node avl_find_node(AVLTree tree, void* key)
{
    assert(tree);
    return find_node(tree->root, tree->compare, key);
}

Node avl_next(AVLTree tree, Node node) 
{
    assert(tree);
	return find_next(tree->root, tree->compare, node);
}

Node avl_prev(AVLTree tree, Node node) 
{
    assert(tree);
	return find_prev(tree->root, tree->compare, node);
}


void* avl_node_value(Node node)
{
    assert(node);
    return node->key;
}



void avl_destroy(AVLTree tree)
{
    assert(tree);
    destroyR(tree->root, tree->destroy);
    free(tree);
}

