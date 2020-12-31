#include "dist_matrix.h"
#include "utilities.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

size_t dist_matrix_size(uint32_t species_count) {
    size_t matrix_size = species_count * (species_count - 1) / 2;
    size_t struct_size = sizeof(dist_matrix) + matrix_size * member_size(dist_matrix, distances[0]);

    return struct_size;
}

dist_matrix *dist_matrix_init(uint32_t species_count) {
    size_t size = dist_matrix_size(species_count);
    dist_matrix *dmat = malloc(size);

    if (dmat != NULL) {
        dmat->species_count = species_count;

        /* Zero-initialize the array of species names to ensure that dist_matrix_free
         * can correctly free the strings */
        dmat->species_names = calloc(species_count, member_size(dist_matrix, species_names[0]));

        if (dmat->species_names == NULL) {
            dist_matrix_free(dmat);

            return NULL;
        }
        
        dmat->cluster_sizes = malloc(species_count * member_size(dist_matrix, cluster_sizes[0]));
        
        if (dmat->cluster_sizes == NULL) {
            dist_matrix_free(dmat);
            
            return NULL;
        }
    }

    return dmat;
}

void dist_matrix_free(dist_matrix *dmat) {
    if (dmat) {
        if (dmat->species_names) {
            for (uint32_t i = 0; i < dmat->species_count; i++) {
                free(dmat->species_names[i]);
            }

            free(dmat->species_names);
        }
        
        if (dmat->cluster_sizes) {
            free(dmat->cluster_sizes);
        }

        free(dmat);
    }
}

char *dist_matrix_set_species_name(dist_matrix *dmat, uint32_t s, const char *species_name) {
    free(dmat->species_names[s]);

    dmat->species_names[s] = neigh_strdup(species_name);

    return dmat->species_names[s];
}

static uint32_t dist_matrix_get_offset(const dist_matrix *dmat, uint32_t s1, uint32_t s2) {
    assert(s1 != s2);
    assert(s1 < dmat->species_count);
    assert(s2 < dmat->species_count);

    if (s1 < s2) {
        return dist_matrix_get_offset(dmat, s2, s1);
    } else {
        return (s1 * (s1 - 1) / 2) + s2;
    }
}

double *dist_matrix_element(dist_matrix *dmat, uint32_t s1, uint32_t s2) {
    return dmat->distances + dist_matrix_get_offset(dmat, s1, s2);
}

double dist_matrix_distance(const dist_matrix *dmat, uint32_t s1, uint32_t s2) {
    return *(dmat->distances + dist_matrix_get_offset(dmat, s1, s2));
}

double dist_matrix_avg_distance_from_others(const dist_matrix *dmat, uint32_t s) {
    assert(dmat->species_count >= 2);
    
    double distance = 0;
    
    if (dmat->species_count > 2) {
        for (uint32_t i = 0; i < dmat->species_count; i++) {
            if (i == s) {
                continue;
            }
            
            distance += dist_matrix_distance(dmat, s, i);
        }

        distance /= (dmat->species_count - 2);
    }
    
    return distance;
}

void dist_matrix_compute_avg_distances(const dist_matrix *dmat, double distances[]) {
    for (uint32_t i = 0; i < dmat->species_count; i++) {
        distances[i] = dist_matrix_avg_distance_from_others(dmat, i);
    }
}

void dist_matrix_print(const dist_matrix *dmat) {
    
    /* Max value among species_name, the string "c_size" and the distances */
    int max_length = 6;

    for (uint32_t i = 0; i < dmat->species_count; i++) {
        int length = (int)strlen(dmat->species_names[i]);

        if (length > max_length) {
            max_length = length;
        }
    }

    printf("%*s\t", max_length, "");

    for (uint32_t i = 0; i < dmat->species_count; i++) {
        printf("%*s\t", max_length, dmat->species_names[i]);
    }

    printf("\n");

    for (uint32_t i = 0; i < dmat->species_count; i++) {
        printf("%*s\t", max_length, dmat->species_names[i]);

        for (uint32_t j = 0; j < i; j++) {
            printf("%*.4lf\t", max_length, dist_matrix_distance(dmat, i, j));
        }

        printf("%*s\n", max_length, "*");
    }

    printf("\n");
    printf("%*s\t", max_length, "c_size");
    
    
    for (uint32_t i = 0; i < dmat->species_count; i++) {
        printf("%*" PRIu32 "\t", max_length, dmat->cluster_sizes[i]);
    }
    
    printf("\n");

}
