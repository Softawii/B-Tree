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

    cliente * client2 = (cliente *) malloc(sizeof(cliente));
    client2->id = 1;
    strcpy(client2->nome, "Sérgio");
    strcpy(client2->nascimento, "05/07/2022");

    cliente * client3 = (cliente *) malloc(sizeof(cliente));
    client3->id = 5;
    strcpy(client3->nome, "Nicolas");
    strcpy(client3->nascimento, "05/07/2022");

    BPTREE *tree = bptree_create("cliente", get_cliente_size(), 2, id_comparator, cliente_from_stream, cliente_to_stream, cliente_show);

    int pt_node, pos;

    bp_tree_insert(tree, client1->id, client1);
    bp_tree_insert(tree, client2->id, client2);
    bp_tree_insert(tree, client3->id, client3);

    bp_tree_search(tree, client1->id, client1, &pt_node, &pos);
    bp_tree_search(tree, client2->id, client2, &pt_node, &pos);
    bp_tree_search(tree, client3->id, client3, &pt_node, &pos);

    return 0;
}