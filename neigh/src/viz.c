//
// Created by Elia Cereda on 20/03/18.
//

#include "viz.h"

#include <graphviz/gvc.h>
#include <stdbool.h>

Agnode_t *viz_process_tree(const btree_node *root, Agraph_t *graph) {
    Agnode_t *node = agnode(graph, root->node_name, true);
    
    /* Define the style attributes for the new generated nodes. */
    agsafeset(node, "fontsize", "10.0", "");
    agsafeset(node, "fontname", "Century Gothic", "");
    agsafeset(node, "fontcolor", "grey22", "");
    agsafeset(node, "shape", "circle", "");

    if (root->left != NULL) {
        Agnode_t *left = viz_process_tree(root->left, graph);
        Agedge_t *edge = agedge(graph, node, left, NULL, true);
        
        /* Convert floating point number (double) to an equivalent string. */
        char edge_label[sizeof(root->distance_left)];
        snprintf(edge_label, sizeof(edge_label), " %.1lf", root->distance_left);
        
        /* Define the style attributes for left edge lables. */
        agsafeset(edge, "taillabel", edge_label, "");
        agsafeset(edge, "labeldistance", "2.7", "");
        agsafeset(edge, "labelangle", "-30", "");
        agsafeset(edge, "fontcolor", "gray45", "");
        agsafeset(edge, "fontsize", "10.0", "");
        agsafeset(edge, "fontname", "Century Gothic", "");
    }

    if (root->right != NULL) {
        Agnode_t *right = viz_process_tree(root->right, graph);
        Agedge_t *edge = agedge(graph, node, right, NULL, true);
        
        /* Convert floating point number (double) to an equivalent string. */
        char edge_label[sizeof(root->distance_right)];
        snprintf(edge_label, sizeof(edge_label), " %.1lf", root->distance_right);
        
        /* Define the style attributes for right edge lables. */
        agsafeset(edge, "taillabel", edge_label, "");
        agsafeset(edge, "labeldistance", "2.7", "");
        agsafeset(edge, "labelangle", "30", "");
        agsafeset(edge, "fontcolor", "gray45", "");
        agsafeset(edge, "fontsize", "10.0", "");
        agsafeset(edge, "fontname", "Century Gothic", "");
    }

    if (root->left == NULL && root->right == NULL) {
        /* Define the style attributes for leaf nodes. */
        agsafeset(node, "fontname", "Century Gothic Bold", "");
        agsafeset(node, "fontcolor", "black", "");
        agsafeset(node, "shape", "rectangle", "");
        agsafeset(node, "fontsize", "15.0", "");
    }
    return node;
}

void viz_visualize_tree(const btree_node *root, const char *output_file, const char *format) {
    /* Set up a graphviz context */
    GVC_t *gvc = gvContext();

    /* Create a simple digraph */
    Agraph_t *graph = agopen((char *)output_file, Agstrictundirected, NULL);

    agattr(graph, AGRAPH, "overlap", "false");
    agattr(graph, AGRAPH, "splines", "true");
    agattr(graph, AGRAPH, "sep", "0.1");
    
    agattr(graph, AGEDGE, "len", "1.3");
    
    viz_process_tree(root, graph);

    /* Compute a layout using layout engine from command line args */
    gvLayout(gvc, graph, "neato");

    /* Write the graph according to -T and -o options */
    gvRenderFilename(gvc, graph, format, output_file);

    /* Free layout data */
    gvFreeLayout(gvc, graph);

    /* Free graph structures */
    agclose(graph);

    gvFreeContext(gvc);
}

Agnode_t *viz_process_trees(btree_node **trees, uint32_t tree_count, Agraph_t *graph) {
    Agnode_t *node = agnode(graph, "root", true);
    
    /* Define the style attributes for internal root node. */
    agsafeset(node, "fontsize", "11.0", "");
    agsafeset(node, "fontname", "Century Gothic", "");
    agsafeset(node, "shape", "diamond", "");
    agsafeset(node, "style", "filled", "");
    agsafeset(node, "fillcolor", "ghostwhite", "");
    
    for (uint32_t i = 0; i < tree_count; i++) {
        Agnode_t *subtree = viz_process_tree(trees[i], graph);
        Agedge_t *edge = agedge(graph, node, subtree, NULL, true);
        
        /* Define the style attributes for edge. */
        agsafeset(edge, "style", "dotted", "");
    }
    
    return node;
}

void viz_visualize_trees(btree_node **trees, uint32_t tree_count, const char *output_file, const char *format) {
    /* Set up a graphviz context */
    GVC_t *gvc = gvContext();
    
    /* Create a simple digraph */
    Agraph_t *graph = agopen((char *)output_file, Agstrictundirected, NULL);
    
    agattr(graph, AGRAPH, "overlap", "false");
    agattr(graph, AGRAPH, "splines", "true");
    agattr(graph, AGRAPH, "sep", "0.1");
    
    agattr(graph, AGEDGE, "len", "1.3");
    
    viz_process_trees(trees, tree_count, graph);
    
    /* Compute a layout using layout engine from command line args */
    gvLayout(gvc, graph, "neato");
    
    /* Write the graph according to -T and -o options */
    gvRenderFilename(gvc, graph, format, output_file);
    
    /* Free layout data */
    gvFreeLayout(gvc, graph);
    
    /* Free graph structures */
    agclose(graph);
    
    gvFreeContext(gvc);
}
