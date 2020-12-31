#include "neighbour_joining.h"

#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

void nj_find_nearest_clusters(const dist_matrix *dmat, const double u[], uint32_t *c1, uint32_t *c2) {
    assert(dmat->species_count >= 2);
    assert(c1 != NULL);
    assert(c2 != NULL);
   
    /*
     * Find the two clusters that minimize the distance metric:
     *      min { D(c1, c2) - U(c1) - U(c2) } among all c1, c2 in dmat
     */
    double min_distance = INFINITY;

    for (uint32_t i = 0; i < dmat->species_count; i++) {
        for (uint32_t j = 0; j < i; j++) {
            double distance = dist_matrix_distance(dmat, i, j) - u[i] - u[j];

            if (distance < min_distance) {
                min_distance = distance;

                *c1 = i;
                *c2 = j;
            }
        }
    }

    /* A pair of clusters should always be found */
    assert(isfinite(min_distance));
}

dist_matrix *nj_join_clusters(const dist_matrix *dmat, const char *new_name, uint32_t c1, uint32_t c2) {
    assert(dmat->species_count >= 2);

    dist_matrix *out = dist_matrix_init(dmat->species_count - 1);

    if (out == NULL) {
        perror("Unable to create output distance matrix");
        return NULL;
    }
    
    uint32_t k = 0;

    for (uint32_t i = 0; i < dmat->species_count; i++) {
        const char *species_name;
        uint32_t cluster_size;

        if (i == c1) {
            /* Replace c1 with the union of clusters c1 and c2 */
            species_name = new_name;
            cluster_size = dmat->cluster_sizes[c1] + dmat->cluster_sizes[c2];
        } else if (i == c2) {
            /* Remove cluster c2 */
            continue;
        } else {
            /* Leave other clusters unchanged */
            species_name = dmat->species_names[i];
            cluster_size = dmat->cluster_sizes[i];
        }

        dist_matrix_set_species_name(out, k, species_name);
        out->cluster_sizes[k] = cluster_size;

        /* Compute the distances */
        uint32_t l = 0;

        for (uint32_t j = 0; j < i; j++) {
            if (j == c2) {
                /* Remove cluster c2 */
                continue;
            }

            double distance;

            if (i == c1) {
                double d1j = dist_matrix_distance(dmat, c1, j);
                double d2j = dist_matrix_distance(dmat, c2, j);
                double d12 = dist_matrix_distance(dmat, c1, c2);

                distance = (d1j + d2j - d12) / 2;
            } else if (j == c1) {
                double di1 = dist_matrix_distance(dmat, i, c1);
                double di2 = dist_matrix_distance(dmat, i, c2);
                double d12 = dist_matrix_distance(dmat, c1, c2);

                distance = (di1 + di2 - d12) / 2;
            } else {
                distance = dist_matrix_distance(dmat, i, j);
            }

            *(dist_matrix_element(out, k, l)) = distance;

            l++;
        }

        k++;
    }

    return out;
}

btree_storage *nj_tree_init(const dist_matrix *dmat, btree_node **leafs) {
    uint32_t node_count = 2 * dmat->species_count - 1;
    btree_storage *storage = btree_storage_init(node_count);
    
    for (uint32_t i = 0; i < dmat->species_count; i++) {
        leafs[i] = btree_storage_fetch(storage);

        btree_node_set_name(leafs[i], dmat->species_names[i]);
    }
    
    return storage;
}

void nj_tree_add_node(const dist_matrix *dmat, const double u[], btree_storage *storage, btree_node **partial_trees, const char *name, uint32_t c1, uint32_t c2) {
    btree_node *node = btree_storage_fetch(storage);

    btree_node_set_name(node, name);

    node->left = partial_trees[c1];
    node->right = partial_trees[c2];
    
    double distance = dist_matrix_distance(dmat, c1, c2);
    node->distance_left = (distance + u[c1] - u[c2]) / 2;
    node->distance_right = (distance + u[c2] - u[c1]) / 2;

    partial_trees[c1] = node;

    for (uint32_t i = c2 + 1; i < dmat->species_count; i++) {
        partial_trees[i - 1] = partial_trees[i];
    }
}
