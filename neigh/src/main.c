#include "config.h"
#include "io.h"
#include "neighbour_joining.h"
#include "viz.h"
#include "utilities.h"

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include<fcntl.h>

void main_prog(int species_num, char* name[], double[],char *result);

int main() {
	char* name[] = { "A","B","C","D" };
	double distances[6] = { 3,4,2,6,4,4 };
	char a[500];
	main_prog(4, name, distances,a);
}


void main_prog(int species_num, char* name[], double distances[],char *result) {
	//const char *input_file = "./hello_world.in";
		//dist_matrix *dmat = load_file(input_file);
	dist_matrix* dmat;
	dmat = load(species_num, name, distances);
	//dmat = load_file(input_file);
	if (!dmat) {
		// Error, stop here
		return EXIT_FAILURE;
	}


	double u[dmat->species_count];

	/* Ensure that cluster_name has enough space for the longest possible name */
	char cluster_name[2 + 3 * sizeof(dmat->species_count) + 1];
	uint32_t cluster_id = 1;
	btree_node* partial_trees[dmat->species_count];
	btree_storage* tree_storage = nj_tree_init(dmat, partial_trees);

	while (dmat->species_count >= 2) {

		// dist_matrix_print(dmat);
		 //btree_print_trees(partial_trees, dmat->species_count);


#if HAS_GRAPHVIZ

#endif

			/* Compute the average distance of each clusters from the others */
		dist_matrix_compute_avg_distances(dmat, u);

		/* Find the pair of nearest clusters */
		uint32_t c1, c2;
		nj_find_nearest_clusters(dmat, u, &c1, &c2);

		/* Generate a name for the new cluster */
		unsigned long result = snprintf(cluster_name, sizeof(cluster_name), "C_%" PRIu32, cluster_id);
		assert(result > 0 && result < sizeof(cluster_name));


		/* Add a node for the new cluster to the array of partial trees */
		nj_tree_add_node(dmat, u, tree_storage, partial_trees, cluster_name, c1, c2);

		/* Create a new dist_matrix joining the specified clusters */
		dist_matrix* joined = nj_join_clusters(dmat, cluster_name, c1, c2);

		if (joined == NULL) {
			/* Error, stop here */

			break;
		}

		/* Release the old distance matrix */
		dist_matrix_free(dmat);
		dmat = joined;

		cluster_id++;
	}
	//btree_print_trees(partial_trees, dmat->species_count);
	btree_node *phyl_tree = partial_trees[0];
	convertTreeAux(phyl_tree,result);
	/*printf(result);
	printf("hello world");*/
	dist_matrix_free(dmat);
	btree_storage_free(tree_storage);
	write_result("result.newick",result);

}


//def convertTreeAux(node, newick) :
//	if node.isLeaf() :
//		newick.append(node.value)
//		if node.leftChild != None :
//			newick.append("(")
//			convertTreeAux(node.leftChild, newick)
//			newick.append(",")
//			if node.rightChild != None :
//				convertTreeAux(node.rightChild, newick)

void convertTreeAux(btree_node* node, char* newick) {
	if(node->left==NULL && node->right==NULL)
		strcat(newick, node->node_name);
	if (node->left != NULL) {
		strcat(newick, "(");
		convertTreeAux(node->left, newick);
		char temp[10];
		sprintf(temp, ":%.2f,", node->distance_left);
		strcat(newick,temp);
				}
	if (node->right != NULL) {
		convertTreeAux(node->right, newick);
		char temp[10];
		sprintf(temp, ":%.2f)", node->distance_right);
		strcat(newick,temp);
	}
	

}
