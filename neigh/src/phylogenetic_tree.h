#ifndef NEIGH_PHYLOGENETIC_TREE_H
#define NEIGH_PHYLOGENETIC_TREE_H

#include <stdio.h>
#include <stdint.h>

typedef struct btree_node btree_node;

struct btree_node {
    /* Name of the node */
    char *node_name;

    /* Left and right children */
    btree_node *left, *right;
    
    /* Distances to the left and right children */
    double distance_left, distance_right;
};

typedef struct btree_storage btree_storage;

struct btree_storage {
    /* Number of nodes */
    uint32_t nodes_count;
    
    /* Number of used nodes / Index of first available node */
    uint32_t used_nodes;
    
    /* Array of tree nodes */
    btree_node nodes[];
};

size_t btree_storage_size(uint32_t nodes_count);
btree_storage *btree_storage_init(uint32_t nodes_count);
void btree_storage_free(btree_storage *tree);

/* Fetch the first available node */
btree_node *btree_storage_fetch(btree_storage *storage);

char *btree_node_set_name(btree_node *node, const char *name);

uint32_t btree_get_height(btree_node *root);

void btree_print_tree(btree_node *root);
void btree_print_trees(btree_node **trees, uint32_t tree_count);

#endif /* NEIGH_PHYLOGENETIC_TREE_H */
