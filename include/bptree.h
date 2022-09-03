#include <stdlib.h>
#include "core.h"

#ifndef BPTREE_H
#define BPTREE_H

#define LEAF 1
#define NON_LEAF 0

typedef struct bptree {
    FILE *index_file;
    FILE *data_file;
    FILE *meta_data_file;
    int element_size;
    int order;
    comparator *comp;
    from_stream *from;
    to_stream *to;
    show *show;
} BPTREE;

typedef struct bptree_data_node {
    int m; // Number of elements in the node
    int parent;
    int next;
    var *data;
} BPTREE_DATA_NODE;

typedef struct bptree_index_node
{
    int m; // Number of elements in the node
    int flag;
    int parent;
    int *children;
    int *keys;
} BPTREE_INDEX_NODE;

BPTREE * bptree_create(char *prefix_file_name, int element_size, int order, comparator * comp, from_stream * from, to_stream * to, show * show);
int bp_tree_search(BPTREE *tree, int key, var element, int *pt_node, int *pos);
void bp_tree_insert(BPTREE *tree, int key, var element);
void bp_tree_delete(BPTREE *tree, int key);

#endif