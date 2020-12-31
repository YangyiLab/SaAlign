#ifndef NEIGH_IO_H
#define NEIGH_IO_H

#include "dist_matrix.h"

dist_matrix *load_file(const char *file_name);
dist_matrix *load(int species_count,char* name[],double distances[]);
void write_result(const char *file_name,char *result);
#endif //NEIGH_IO_H
