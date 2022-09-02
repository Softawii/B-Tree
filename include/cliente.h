//
// Created by yancf on 26/06/2022.
//

#ifndef HASH_CLIENTE_H
#define HASH_CLIENTE_H

#include "core.h"
#include <stdio.h>

typedef struct{
    int id;
    char nome[50];
    char nascimento[11];
}cliente;


var cliente_from_stream(FILE * file);

void cliente_to_stream(FILE * file, var c);

void cliente_show(var c);

int cliente_hash(var a, int divisor);

int id_comparator (var  a, var  b);

int get_cliente_size ();

#endif //HASH_CLIENTE_H


