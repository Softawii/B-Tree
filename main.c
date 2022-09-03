#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/bptree.h"
#include "include/cliente.h"

int main() {

    cliente * client1 = (cliente *) malloc(sizeof(cliente));
    client1->id = 3;
    strcpy(client1->nome, "Yancf");
    strcpy(client1->nascimento, "05/07/2022");

    BPTREE *tree = bptree_create("cliente", get_cliente_size(), 2, id_comparator, cliente_from_stream, cliente_to_stream, cliente_show);

    int pt_node, pos;

    bp_tree_search(tree, 3, client1, &pt_node, &pos);


    return 0;
}