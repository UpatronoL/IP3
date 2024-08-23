#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 4
int main() {
    int *array = malloc(SIZE*sizeof(int));
    *array = 1;
    *(array+1) = 2;
    *(array+3) = 3;
    int new_size = SIZE * 2;
    array = realloc(array, new_size * sizeof(int));
    memmove(&array[4], &array[0], 2*sizeof(int));
    memset(array, NULL, sizeof(int)*2);
    printf("New array: ");
    for (int i = 0; i < new_size; i++) {
        if (array[i] == 0) {
            printf(" , ");
        } else {
            printf("%d, ", array[i]);
        }
    }
    free(array);
    return 0;
}
