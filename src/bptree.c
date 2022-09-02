#include <stdlib.h>
#include <string.h>
#include "../include/bptree.h"
#include "../include/core.h"

void _set_data_node(BPTREE * tree, BPTREE_DATA_NODE * node, int index) {
    fseek(tree->data_file, index * sizeof(BPTREE_DATA_NODE), SEEK_SET);
    
    fwrite(&node->m, sizeof(node->m), 1, tree->data_file);
    
    fwrite(&node->parent, sizeof(node->parent), 1, tree->data_file);
    
    fwrite(&node->next, sizeof(node->next), 1, tree->data_file);

    for(int i = 0; i < 2*tree->order; i++) {
        tree->to(tree->data_file, node->data[i]);
    }
}


int _create_data_node(BPTREE *tree, int parent, int next) {
    BPTREE_DATA_NODE *node = (BPTREE_DATA_NODE *) malloc(sizeof(BPTREE_DATA_NODE));
    node->m = 0;
    node->parent = parent;
    node->next = next;
    node->data = (var *) malloc(sizeof(var) * 2 * tree->order);
    
    for(int i = 0; i < 2 * tree->order; i++) {
        node->data[i] = (char *) malloc(tree->element_size);
        node->data[i] = "";
    }

    fseek(tree->data_file, 0, SEEK_END);
    int index = ftell(tree->data_file) / sizeof(BPTREE_DATA_NODE);
    _set_data_node(tree, node, index);

    free(node);

    return index;
}

void _set_index_node(BPTREE * tree, BPTREE_INDEX_NODE * node, int index) {
    fseek(tree->index_file, index * sizeof(BPTREE_INDEX_NODE), SEEK_SET);
    
    fwrite(&node->m, sizeof(node->m), 1, tree->index_file);
    
    fwrite(&node->flag, sizeof(node->flag), 1, tree->index_file);
    
    fwrite(&node->parent, sizeof(node->parent), 1, tree->index_file);
    
    for(int i = 0; i < 2*tree->order + 1; i++) {
        fwrite(&node->children[i], sizeof(node->children[i]), 1, tree->index_file);
    }

    for(int i = 0; i < 2*tree->order; i++) {
        fwrite(&node->keys[i], sizeof(node->keys[i]), 1, tree->index_file);
    }
}


int _create_index_node(BPTREE * tree, int parent, int flag) {
    BPTREE_INDEX_NODE *node = (BPTREE_INDEX_NODE *) malloc(sizeof(BPTREE_INDEX_NODE));
    node->m = 0;
    node->flag = flag;
    node->parent = parent;
    node->children = (int *) malloc(sizeof(int) * (2 * tree->order + 1));
    node->keys = (int *) malloc(sizeof(int) * 2 * tree->order);

    for(int i = 0; i < 2 * tree->order; i++) {
        node->keys[i] = -1;
    }
    for(int i = 0; i < 2 * tree->order + 1; i++) {
        node->children[i] = -1;
    }

    fseek(tree->index_file, 0, SEEK_END);
    int index = ftell(tree->index_file) / sizeof(BPTREE_INDEX_NODE);
    _set_index_node(tree, node, index);

    free(node);
}


/**
 * @brief Creates a new B+ tree
 * 
 * @param prefix_file_name File prefix name to be used.
 * @param element_size Size (in bytes) of the element to be stored.
 * @param order Tree order to use. Must be greater than 1.
 * @param comp Function to comparate your data strutcture.
 * @param from Function to get your data strtucture from a stream.
 * @param to Function to get your data strtucture to a stream.
 * @param show Function to print your data strtucture.
 * @return BPTREE* The B+ tree created.
 */
BPTREE * bptree_create(char *prefix_file_name, int element_size, int order, comparator * comp, from_stream * from, to_stream * to, show * show){
    #ifdef BPTREE_DEBUG
        printf("\nbptree_create: start\n");
    #endif

    BPTREE *tree = (BPTREE *) malloc(sizeof(BPTREE));

    char * data_file_name = (char *) malloc(sizeof(char) * (strlen(prefix_file_name) + 9));
    strcpy(data_file_name, prefix_file_name);
    tree->data_file = fopen(strcat(data_file_name, "_data.dat"), "w+b");

    char * index_file_name = (char *) malloc(sizeof(char) * (strlen(prefix_file_name) + 10));
    strcpy(index_file_name, prefix_file_name);
    tree->index_file = fopen(strcat(index_file_name, "_index.dat"), "wb+");

    char * meta_data_file_name = (char *) malloc(sizeof(char) * (strlen(prefix_file_name) + 9));
    strcpy(meta_data_file_name, prefix_file_name);
    tree->meta_data_file = fopen(strcat(meta_data_file_name, "_meta.dat"), "wb+");

    tree->element_size = element_size;
    tree->order = order;
    tree->comp = comp;
    tree->from = from;
    tree->to = to;
    tree->show = show;

    _create_data_node(tree, -1, -1);

    _create_index_node(tree, -1, 1);

    #ifdef BPTREE_DEBUG
        printf("\nbptree_create: end\n");
    #endif

    return tree;
}