//
// Created by yancf on 05/07/2022.
//
#include "../include/cliente.h"
#include "../include/core.h"
#include <stdlib.h>

var cliente_from_stream(FILE * file) {

    cliente *temp = (cliente *) malloc(sizeof(cliente));

    if(!fread(&temp->id, sizeof(temp->id), 1, file)) {
        printf("ERRO: Erro ao ler o id do cliente\n");
        return NULL;
    }

    if(!fread(temp->nome, sizeof(temp->nome), 1, file)) {
        printf("ERRO: Erro ao ler o nome do cliente\n");
        return NULL;
    }

    if(!fread(temp->nascimento, sizeof(temp->nascimento), 1, file)) {
        printf("ERRO: Erro ao ler o nascimento do cliente\n");
        return NULL;
    }


    #ifdef DEBUG
        printf("ID: %d | Nome: %s | Nascimento: %s\n", temp->id, temp->nome, temp->nascimento);
    #endif

    // Return
    return temp;
}

void cliente_to_stream(FILE * file, var c) {
    cliente *tempCliente = (cliente*) c;
    fwrite(&tempCliente->id, sizeof(tempCliente->id), 1, file);
    fwrite(tempCliente->nome, sizeof(tempCliente->nome), 1, file);
    fwrite(tempCliente->nascimento, sizeof(tempCliente->nascimento), 1, file);
}

void cliente_show(var c) {
    cliente *tempCliente = (cliente*) c;
    printf("ID: %d, Name: %s, Birth: %s", tempCliente->id, tempCliente->nome, tempCliente->nascimento);
}

int cliente_hash(var a, int divisor) {
    int p1 = ((cliente*) a)->id;
    return p1 % divisor;
}

int id_comparator (var  a, var  b) {
    int p1 = ((cliente*) a)->id;
    int p2 = ((cliente*) b)->id;

    if(p1 > p2)
        return 1;
    else if(p1 == p2)
        return 0;
    else
        return -1;
}

int get_cliente_size () {
    cliente cliente;
    return sizeof(cliente.nome) + sizeof(cliente.nascimento) + sizeof(cliente.id);
}