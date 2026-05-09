#include "stack.h"

stack_s create_stack(size_t element_size){
    stack_s stack;
    
    stack.stack = malloc(element_size * 10); // starts with a capacity of 10 values 
    stack.size = 0;
    stack.capacity = 10;
    stack.top = -1; // must start it at -1 to make it a 0 based index
    stack.element_size = element_size;
    
    return stack;
}

int stack_is_empty(const stack_s* stack){
    return stack->size == 0;
}

int stack_push(stack_s* stack, void* val){
    //checks if stack is full and if so, it doubles its capacity
    if (stack->size == stack->capacity){
        stack->stack = realloc(stack->stack, sizeof(void*) * (stack->size * 2));
        
        if (!stack->stack){
            perror("realloc"); 
            return -1;
        }
        
        stack->capacity *= 2;
    }
    
    memcpy((char*)stack->stack + stack->size * stack->element_size, val, stack->element_size);
    stack->top++;
    stack->size++;

    return 0;
}

void* stack_pop(stack_s* stack){
    if (stack_is_empty(stack)){
        return NULL;
    }    

    void* val = (char*)stack->stack + stack->top * stack->element_size;
    stack->top--;
    stack->size--;
    
    return val;
}

void* stack_top(const stack_s* stack){
    if (stack_is_empty(stack)){
        return NULL;
    }

    void* val = (char*)stack->stack + stack->top * stack->element_size;
    return val;
}

int stack_size(const stack_s* stack){
    return stack->size;
}

void stack_release(stack_s* stack){
    free(stack->stack);
    stack->stack = NULL;
}
