#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SYMBOLS 256

typedef struct Node {
    unsigned char symbol;
    int frequency;
    struct Node *left, *right;
} Node;

typedef struct PriorityQueue {
    Node *heap[MAX_SYMBOLS];
    int size;
} PriorityQueue;

void push(PriorityQueue *pq, Node *node) {
    pq->heap[pq->size++] = node;
    int i = pq->size - 1;
    while (i > 0 && pq->heap[i]->frequency < pq->heap[(i - 1) / 2]->frequency) {
        Node *temp = pq->heap[i];
        pq->heap[i] = pq->heap[(i - 1) / 2];
        pq->heap[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

Node *pop(PriorityQueue *pq) {
    Node *top = pq->heap[0];
    pq->heap[0] = pq->heap[--pq->size];
    int i = 0;
    while (2 * i + 1 < pq->size) {
        int smallest = 2 * i + 1;
        if (smallest + 1 < pq->size && pq->heap[smallest + 1]->frequency < pq->heap[smallest]->frequency) {
            smallest++;
        }
        if (pq->heap[i]->frequency <= pq->heap[smallest]->frequency) {
            break;
        }
        Node *temp = pq->heap[i];
        pq->heap[i] = pq->heap[smallest];
        pq->heap[smallest] = temp;
        i = smallest;
    }
    return top;
}

Node *buildHuffmanTree(int *frequencies) {
    PriorityQueue *pq = malloc(sizeof((*pq)));
    pq->size = 0;
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (frequencies[i] > 0) {
            Node *node = malloc(sizeof(*node));
            node->symbol = i;
            node->frequency = frequencies[i];
            node->left = node->right = NULL;
            push(pq, node);
        }
    }
    while (pq->size > 1) {
        Node *left = pop(pq);
        Node *right = pop(pq);
        Node *parent = malloc(sizeof(*parent));
        parent->symbol = 0;
        parent->frequency = left->frequency + right->frequency;
        parent->left = left;
        parent->right = right;
        push(pq, parent);
    }
    return pop(pq);
}

void freeHuffmanTree(Node *root) {
    if (root) {
        freeHuffmanTree(root->left);
        freeHuffmanTree(root->right);
        free(root);
    }
}

void generateCodes(Node *root, char *code, int depth, char **codes) {
    if (!root->left && !root->right) {
        code[depth] = '\0';
        codes[root->symbol] = strdup(code);
        return;
    }
    if (root->left) {
        code[depth] = '0';
        generateCodes(root->left, code, depth + 1, codes);
    }
    if (root->right) {
        code[depth] = '1';
        generateCodes(root->right, code, depth + 1, codes);
    }
}

void compressFile(const char *filename, Node *huffmanTree, char **codes) {
    FILE *file = fopen(filename, "rb");
    FILE *output = fopen("compressed.bin", "wb");

    if (!file || !output) {
        perror("File opening failed");
        return;
    }

    int c;
    while ((c = fgetc(file)) != EOF) {
        fputs(codes[c], output);
    }

    fclose(file);
    fclose(output);
}

void decompressFile(const char *filename, Node *huffmanTree) {
    FILE *file = fopen("compressed.bin", "rb");
    FILE *output = fopen("decompressed.txt", "wb");

    if (!file || !output) {
        perror("File opening failed");
        return;
    }

    Node *current = huffmanTree;
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (c == '0') {
            current = current->left;
        } else {
            current = current->right;
        }
        if (!current->left && !current->right) {
            fputc(current->symbol, output);
            current = huffmanTree;
        }
    }

    fclose(file);
    fclose(output);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    int frequencies[MAX_SYMBOLS] = {0};
    char *codes[MAX_SYMBOLS] = {0};
    FILE *file = fopen(filename, "rb");

    if (!file) {
        perror("File opening failed");
        return 1;
    }
    int c;
    while ((c = fgetc(file)) != EOF) {
        frequencies[c]++;
    }
    fclose(file);
    Node *huffmanTree = buildHuffmanTree(frequencies);
    char code[256];
    generateCodes(huffmanTree, code, 0, codes);
    compressFile(filename, huffmanTree, codes);
    decompressFile(filename, huffmanTree);
    freeHuffmanTree(huffmanTree);
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (codes[i]) {
            free(codes[i]);
        }
    }

    return 0;
}
