#ifndef NEIGH_DIST_MATRIX_H
#define NEIGH_DIST_MATRIX_H

#include <stdint.h>
#include <stddef.h>

typedef struct dist_matrix dist_matrix;

struct dist_matrix {
    /* Number of species */
    uint32_t species_count;
    
    /* Names of the species */
    char **species_names;
    
    /* Number of species in each cluster */
    uint32_t *cluster_sizes;
    
    /* Distance matrix */
    double distances[];
};

/* Get the size in bytes of a dist_matrix with the given number of species */
size_t dist_matrix_size(uint32_t species_count);

dist_matrix *dist_matrix_init(uint32_t species_count);
void dist_matrix_free(dist_matrix *dmat);

char *dist_matrix_set_species_name(dist_matrix *dmat, uint32_t s, const char *species_name);

/* Extract the distance between species s1 and s2 */
double *dist_matrix_element(dist_matrix *dmat, uint32_t s1, uint32_t s2);
double  dist_matrix_distance(const dist_matrix *dmat, uint32_t s1, uint32_t s2);

/* Calculate the distance between a species s and all the others */
double dist_matrix_avg_distance_from_others(const dist_matrix *dmat, uint32_t s);

/*
 * Populate the given array distances with the distance between each species s
 * and all the others. Distances must be dmat->species_count element long.
 */
void dist_matrix_compute_avg_distances(const dist_matrix *dmat, double distances[]);

/* Print the distance matrix */
void dist_matrix_print(const dist_matrix *dmat);

#endif //NEIGH_DIST_MATRIX_H
