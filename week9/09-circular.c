#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define ELT_T int

struct Queue
{
    ELT_T  *elements;
    size_t  capacity;
    size_t  index;
    size_t  outdex;
};

struct Queue *newQueue(size_t size)
{
    struct Queue *q = calloc(1, sizeof(struct Queue));
    q->elements = malloc(sizeof(ELT_T) * size);
    q->capacity = size;
    q->index    = 0;
    q->outdex   = 0;
    return q;
}

// implement the other required functions here
//
// [1, , , 3, 4]
//    i    o

size_t Queue_size(struct Queue *q) {
    if(q->index < q->outdex) {
        return q->outdex - q->index;
    }
    return q->index - q->outdex;
}

int Queue_isEmpty(struct Queue *q) {
    return q->index == q->outdex;
}

int Queue_isFull(struct Queue *q) {
    return (q->index + 1) % q->capacity == q->outdex;
}

void Queue_resize(struct Queue *q) {
    size_t new_capacity = q->capacity * 2;
    q->elements = realloc(q->elements, new_capacity * sizeof(ELT_T));
    if(q->index < q->outdex) {
        memmove(q->elements + q->capacity + 1, q->elements, sizeof(ELT_T)*(q->index-1));
        q->index = q->capacity + q->index;
    }
    q->capacity = new_capacity;
}

void Queue_add(struct Queue *q, ELT_T elemtnt) {
    if(Queue_isFull(q)) {
        Queue_resize(q);
    }
    q->elements[q->index] = elemtnt;
    q->index = (q->index+1) % q->capacity;
}

ELT_T Queue_remove (struct Queue *q) {
    if(Queue_isEmpty(q)) exit(1);
    ELT_T temp = q->elements[q->outdex];
    q->outdex = (q->outdex+1) % q->capacity;
    return temp;
}

ELT_T Queue_first(struct Queue *q) {
    return q->elements[q->outdex];
}

int main(int argc, char **argv)
{
    struct Queue *queue = newQueue(32);

    assert(Queue_isEmpty(queue));

    for (int i = 1;  i < 10;  ++i) {
        Queue_add(queue, i*i);
        assert(!Queue_isEmpty(queue));
    }

    for (int i = 1;  i < 10;  ++i) {
        assert(!Queue_isEmpty(queue));
        assert(Queue_first(queue) == i*i);
        int j = Queue_remove(queue);
        assert(j == i*i);
    }

    assert(Queue_isEmpty(queue));

    for (int i = 1;  i < 64;  ++i) {
        Queue_add(queue, i*i);
        /*assert(!Queue_isFull(queue));*/
    }
    /*assert(Queue_isFull(queue));*/

    for (int i = 1;  i < 64;  ++i) {
        Queue_remove(queue);
    }
    assert(Queue_isEmpty(queue));

    printf("all tests passed\n");

    return 0;
}
