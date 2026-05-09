#ifndef INCLUDE_QUEUE_H_
#define INCLUDE_QUEUE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct queue_t{
    int size;
    int capacity;
    int head;
    int rear;
    size_t element_size;
    void* array;
} queue_t;

queue_t create_queue(size_t element_size);
void queue_release(queue_t* queue);
int resize_queue(queue_t* queue, int new_capacity);
int enqueue(queue_t* queue, void* val);
void* dequeue(queue_t* queue);
void* first(const queue_t* queue);
int is_empty(const queue_t* queue);
int queue_size(const queue_t* queue);
void print_queue(const queue_t* queue, void (*print_element)(void*));

#endif // !INCLUDE_QUEUE_H_
