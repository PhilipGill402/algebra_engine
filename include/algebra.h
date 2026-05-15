#ifndef INCLUDE_ALGEBRA_H_
#define INCLUDE_ALGEBRA_H_

#include <math.h>
#include <stdint.h>
#include "tree.h"
#include "string_t.h"

void simplify_tree(node_t* root);
node_t* diff_tree(node_t* root);
uint8_t can_be_simplified(node_t* node);

#endif // !INCLUDE_ALGEBRA_H_
