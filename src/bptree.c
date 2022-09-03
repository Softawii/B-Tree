#include <stdlib.h>
#include <string.h>
#include "../include/bptree.h"
#include "../include/core.h"

int _create_index_node(BPTREE * tree, int parent, int flag);
BPTREE_INDEX_NODE * _get_index_node(BPTREE *tree, int index);
void _set_index_node(BPTREE * tree, BPTREE_INDEX_NODE * node, int index);
int get_index_node_size(BPTREE *tree);

int _create_data_node(BPTREE *tree, int parent, int next);
BPTREE_DATA_NODE * _get_data_node(BPTREE *tree, int index);
void _set_data_node(BPTREE * tree, BPTREE_DATA_NODE * node, int index);
int get_data_node_size(BPTREE *tree);

void _get_meta_data(BPTREE * tree, int * root, int * flag);
void _set_meta_data(BPTREE * tree, int root, int flag);



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

    char * index_file_name = (char *) malloc(sizeof(char) * (strlen(prefix_file_name) + 10));
    strcpy(index_file_name, prefix_file_name);
    tree->index_file = fopen(strcat(index_file_name, "_index.dat"), "w+");

    char * data_file_name = (char *) malloc(sizeof(char) * (strlen(prefix_file_name) + 9));
    strcpy(data_file_name, prefix_file_name);
    tree->data_file = fopen(strcat(data_file_name, "_data.dat"), "w+");

    char * meta_data_file_name = (char *) malloc(sizeof(char) * (strlen(prefix_file_name) + 9));
    strcpy(meta_data_file_name, prefix_file_name);
    tree->meta_data_file = fopen(strcat(meta_data_file_name, "_meta.dat"), "w+");

    tree->element_size = element_size;
    tree->order = order;
    tree->comp = comp;
    tree->from = from;
    tree->to = to;
    tree->show = show;

    int root = _create_index_node(tree, -1, 1);

    _create_data_node(tree, root, LEAF);

    _set_meta_data(tree, root, LEAF);

    #ifdef BPTREE_DEBUG
        printf("bptree_create: end\n\n");
    #endif

    return tree;
}

/**
 * @brief Searches for a key in the B+ tree.
 * 
 * @param tree The b+ tree to be searched.
 * @param key The key to be searched.
 * @param pt_node The node where the key was found.
 * @param pos The position of the key in the node.
 * @return int 
 */
int bp_tree_search(BPTREE *tree, int key, var element, int *pt_node, int *pos) {
    #ifdef BPTREE_DEBUG
        printf("\nbp_tree_search: start\n");
    #endif

    int pt_root, flag;
    _get_meta_data(tree, &pt_root, &flag);

    int status;
    *pos = 0;
    *pt_node = pt_root;

    int temp_pt_node = pt_root;


    while(temp_pt_node != -1) {
        BPTREE_INDEX_NODE *index_node = _get_index_node(tree, temp_pt_node);

        if(temp_pt_node == pt_root && index_node->m == 0) {
            printf("ERROR: Tree is empty.\n");
            return -1;
        }

        int i = 0;
        for(i = 0; i < index_node->m; i++) {
            if(key < index_node->keys[i])
                break;  
        }
        temp_pt_node = index_node->children[i];

        if(index_node->flag == LEAF) {
            BPTREE_DATA_NODE *data_node = _get_data_node(tree, temp_pt_node);
            
            *pt_node = temp_pt_node;

            status = 0;

            for(int j = 0; j < data_node->m; j++) {
                if(tree->comp(data_node->data[j], element) == 0) {
                    *pos = j;
                    status = 1;
                    break;
                }
                else if (tree->comp(data_node->data[j], element) == 1) {
                    *pos = j;
                    break;
                }
            }

            *pos = data_node->m;

            return status;
        }
    }

    #ifdef BPTREE_DEBUG
        printf("bp_tree_search: end\n\n");
    #endif
    return 0;
}



// Utility functions ----------------------------------------------------------


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

    return index;
}

BPTREE_INDEX_NODE * _get_index_node(BPTREE *tree, int index) { 
    BPTREE_INDEX_NODE *node = (BPTREE_INDEX_NODE *)malloc(sizeof(BPTREE_INDEX_NODE));

    fseek(tree->index_file, index * get_index_node_size(tree), SEEK_SET);
    
    node->children = (int *)malloc(sizeof(int) * node->m + 1);
    node->keys = (int *)malloc(sizeof(int) * node->m);

    if(!fread(&node->m, sizeof(node->m), 1, tree->index_file)
    || !fread(&node->flag, sizeof(node->flag), 1, tree->index_file)
    || !fread(&node->parent, sizeof(node->parent), 1, tree->index_file)
    || !fread(node->children, sizeof(int), node->m + 1, tree->index_file)) {
        printf("Error reading index node.\n");
    }

    if(node->m != 0) {
        if(!fread(node->keys, sizeof(int), node->m, tree->index_file)) {
            printf("Error reading index node.\n");
        }
    }

    return node;
}

void _set_index_node(BPTREE * tree, BPTREE_INDEX_NODE * node, int index) {
    fseek(tree->index_file, index * get_index_node_size(tree), SEEK_SET);
    
    fwrite(&node->m, sizeof(node->m), 1, tree->index_file);
    
    fwrite(&node->flag, sizeof(node->flag), 1, tree->index_file);
    
    fwrite(&node->parent, sizeof(node->parent), 1, tree->index_file);
    
    fwrite(node->children, sizeof(int), 2*tree->order + 1, tree->index_file);
    
    fwrite(node->keys, sizeof(int), 2*tree->order, tree->index_file);
    
}

int get_index_node_size(BPTREE *tree) {
    return sizeof(int) + sizeof(int) + sizeof(int) + (sizeof(int) * (tree->order * 2 + 1)) + (sizeof(int) * (tree->order * 2 + 1));
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

BPTREE_DATA_NODE * _get_data_node(BPTREE *tree, int index) {
    BPTREE_DATA_NODE *node = (BPTREE_DATA_NODE *) malloc(sizeof(BPTREE_DATA_NODE));

    fseek(tree->data_file, index * get_data_node_size(tree), SEEK_SET);

    if(!fread(&node->m, sizeof(node->m), 1, tree->data_file)
    || !fread(&node->parent, sizeof(node->parent), 1, tree->data_file)
    || !fread(&node->next, sizeof(node->next), 1, tree->data_file)) {
        printf("Error reading data node\n");
    }

    node->data = (var *) malloc(sizeof(var) * node->m);
    for(int i = 0; i < node->m; i++) {
        node->data[i] = tree->from(tree->data_file);
    }

    return node;
}

void _set_data_node(BPTREE * tree, BPTREE_DATA_NODE * node, int index) {
    fseek(tree->data_file, index * get_data_node_size(tree), SEEK_SET);
    
    fwrite(&node->m, sizeof(node->m), 1, tree->data_file);
    
    fwrite(&node->parent, sizeof(node->parent), 1, tree->data_file);
    
    fwrite(&node->next, sizeof(node->next), 1, tree->data_file);

    for(int i = 0; i < 2*tree->order; i++) {
        tree->to(tree->data_file, node->data[i]);
    }
}

int get_data_node_size(BPTREE *tree) {
    return sizeof(int) + sizeof(int) + sizeof(int) + (tree->element_size * (tree->order * 2));
}

void _get_meta_data(BPTREE * tree, int * root, int * flag) {
    fseek(tree->meta_data_file, 0, SEEK_SET);
    if(!fread(root, sizeof(*root), 1, tree->meta_data_file)
    || !fread(flag, sizeof(*flag), 1, tree->meta_data_file)) {
        printf("Error reading meta data\n");
    }
}

void _set_meta_data(BPTREE * tree, int root, int flag) {
    fseek(tree->meta_data_file, 0, SEEK_SET);
    fwrite(&root, sizeof(root), 1, tree->meta_data_file);
    fwrite(&flag, sizeof(flag), 1, tree->meta_data_file);
}