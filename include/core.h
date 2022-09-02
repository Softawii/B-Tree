//
// Created by yancf on 02/07/2022.
//

#ifndef HASH_CORE_H
#define HASH_CORE_H

#include <stdio.h>

typedef void * var;
typedef var  (from_stream)(FILE *);
typedef void (to_stream)  (FILE *, var);
typedef int  (comparator) (var, var);
typedef void (show)       (var);



#endif //HASH_CORE_H
