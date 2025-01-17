#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct NODE {
    int freq;
    int c;
    struct NODE *left;
    struct NODE *right;
} Node;

typedef struct priotiryQueue {
    int size, capacity;
    Node **queue;
} priorityQueue;

Node *newNode(int c, int freq) {
    Node *temp = malloc(sizeof(*temp));
    temp->c = c;
    temp->freq = freq;
    temp->left = temp->right = NULL;
    return temp;
}

priorityQueue *newPriorityQueue(void) {
    priorityQueue *pq = malloc(sizeof(*pq));
    pq->size = 0;
    pq->capacity = 0;
    pq->queue = 0;
    return pq;
}

void push(priorityQueue *pq, Node *node) {
    assert(pq);
    assert(node);
    if (pq->size >= pq->capacity) {
        pq->queue = realloc(pq->queue, sizeof(*pq->queue) * (pq->capacity += 32));
    }
    pq->queue[pq->size++] = node;
    for (int i = pq->size - 1;  i;  --i) {
        if (pq->queue[i]->freq > pq->queue[i-1]->freq) {
            Node *p = pq->queue[i];
            pq->queue[i] = pq->queue[i-1];
            pq->queue[i-1] = p;
        }
    }
}

Node *removeNode(priorityQueue *pq, int c) {
    int i = 0;
    while (i < pq->size) {
        if (pq->queue[i]->c == c) break;
        ++i;
    }
    if (i == pq->size) return 0;
    Node *n = pq->queue[i];
    while (i < pq->size - 1) {
        pq->queue[i] = pq->queue[i+1];
        ++i;
    }
    return n;
}

int main() {
    priorityQueue *pq = newPriorityQueue();
    int c;
    while (EOF != (c = getchar())) {
        Node *n = removeNode(pq, c);
        if (!n) n = newNode(c, 0);
        n->freq++;
        push(pq, n);
    }
    for (int i = 0;  i < pq->size;  ++i) {
        Node *n = pq->queue[i];
        printf("%c -> %d\n", n->c, n->freq);
    }
    return 0;
}
