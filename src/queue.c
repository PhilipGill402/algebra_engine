#include "queue.h"

queue_t create_queue(size_t element_size){
    queue_t queue;
    
    queue.size = 0;
    queue.capacity = 10;
    queue.head = 0;
    queue.rear = 0;
    queue.element_size = element_size;
    queue.array = malloc(element_size * 10);

    return queue;
}

void queue_release(queue_t* queue){
    free(queue->array);
    queue->array = NULL;
}

int resize_queue(queue_t* queue, int new_capacity){
    void* new_array;
    new_array = malloc(queue->element_size * new_capacity);

    if (new_array == NULL){
        fprintf(stderr, "failed to allocate memory");
        return -1;
    }
    
    for (int i = 0; i < queue->size; i++){
        int index = (queue->head + i) % queue->capacity;
        memcpy((char*)new_array + i * queue->element_size, (char*)queue->array + index * queue->element_size, queue->element_size);
    }
    
    free(queue->array);

    queue->array = new_array;
    queue->head = 0;
    queue->rear = queue->size;
    queue->capacity = new_capacity;

    return 0;
}

int enqueue(queue_t* queue, void* val){
    if (queue->size == queue->capacity){
        if (resize_queue(queue, queue->capacity * 2) == -1){
            fprintf(stderr, "failed to resize queue"); 
            return -1;
        }
    } 
    
    if (is_empty(queue)){
        queue->head = queue->rear;
    }
    
    memcpy((char*)queue->array + queue->rear * queue->element_size, val, queue->element_size);
    queue->size++;
    queue->rear = (queue->rear + 1) % queue->capacity;
    return 0;
}

void* dequeue(queue_t* queue){
    if (is_empty(queue)){
        fprintf(stderr, "can't dequeue from empty queue");
        return NULL;
    } 
    
    void* val = (char*)queue->array + queue->head * queue->element_size;
    queue->size--;
    queue->head = (queue-> head + 1) % queue->capacity;
    
    return val;
}

void* first(const queue_t* queue){
    void* val = (char*)queue->array + queue->head * queue->element_size;
    return val;
}

int is_empty(const queue_t* queue){
    return queue->size == 0;
}

int queue_size(const queue_t* queue){
    return queue->size;
}

void print_queue(const queue_t* queue, void (*print_element)(void*)) {
    printf("[");

    for (int count = 0; count < queue->size; count++) {
        int index = (queue->head + count) % queue->capacity;
        void* element = (char*)queue->array + index * queue->element_size;

        print_element(element);

        if (count != queue->size - 1) {
            printf(", ");
        }
    }

    printf("]\n");
}
