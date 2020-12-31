//
// Created by Elia Cereda on 20/03/18.
//

#ifndef NEIGH_VIZ_H
#define NEIGH_VIZ_H

#include "config.h"

#if HAS_GRAPHVIZ

#include "phylogenetic_tree.h"

void viz_visualize_tree(const btree_node *root, const char *output_file, const char *format);
void viz_visualize_trees(btree_node **trees, uint32_t tree_count, const char *output_file, const char *format);

#endif

#endif //NEIGH_VIZ_H
