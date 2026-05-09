#ifndef INCLUDE_ENTRY_H_
#define INCLUDE_ENTRY_H_

#include "string_t.h"

typedef struct node_t node_t;

typedef struct {
    string_t name;
    node_t* node;
} entry_t;

#endif // !INCLUDE_ENTRY_H_
