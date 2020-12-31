#ifndef NEIGH_NEIGHBOUR_JOINING_H
#define NEIGH_NEIGHBOUR_JOINING_H

#include "dist_matrix.h"
#include "phylogenetic_tree.h"

/* Find the nearest clusters in dmat and write their index in c1 and c2. */
void nj_find_nearest_clusters(const dist_matrix *dmat, const double u[], uint32_t *c1, uint32_t *c2);

/* Create a new dist_matrix joining the two specified clusters c1, c2 in a new one called new_name. */
dist_matrix *nj_join_clusters(const dist_matrix *dmat, const char *new_name, uint32_t c1, uint32_t c2);

btree_storage *nj_tree_init(const dist_matrix *dmat, btree_node **leafs);
void nj_tree_add_node(const dist_matrix *dmat, const double u[], btree_storage *storage, btree_node **partial_trees, const char *name, uint32_t c1, uint32_t c2);

#endif //NEIGH_NEIGHBOUR_JOINING_H
