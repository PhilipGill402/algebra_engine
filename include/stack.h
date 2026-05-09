#ifndef INCLUDE_STACK_H_
#define INCLUDE_STACK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stack_s {
    int size;
    int capacity;
    int top;
    size_t element_size;
    void* stack; 
} stack_s;

stack_s create_stack(size_t element_size);
int stack_push(stack_s* stack, void* val);
void* stack_pop(stack_s* stack);
void* stack_top(const stack_s* stack);
int stack_size(const stack_s* stack);
int stack_is_empty(const stack_s* stack);
void stack_release(stack_s* stack);

#endif // !INCLUDE_STACK_H_
