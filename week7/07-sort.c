#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

void swap(int array[], int a, int b) {
    int j = array[a];
    array[a] = array[b];
    array[b] = j;
}

void bubbleSort(int array[], int left, int right)
{
    while(right > left) {
        for(int i = 0; i < right; i++) {
            if(array[i] > array[i+1]) swap(array, i+1, i);
        }
        right--;
    }
}

void selectionSort(int array[], int left, int right) {
    while(right > left) {
        int mindex = left;
        for(int i = left+1; i <= right; i++) {
            if(array[i] < array[mindex]) mindex = i;
        }
        swap(array, left, mindex);
        left++;
    }
}

void insertionSort(int array[], int left, int right) {
    for(int i = left + 1; i <= right; i++) { 
        for(int j = i; j > left; j--) { 
            if(array[j-1] <= array[j]) break;
            swap(array, j-1, j);
        }
    }
}

void quickSort(int array[], int left, int right) {
    if(right <= left) return;
    int pivot = array[(left + right) / 2];
    int l = left - 1;
    int r = right + 1;
    for (;;) {
        while (array[++l] < pivot);
        while (array[--r] > pivot);
        if(l >= r) break;
        swap(array, l,r);
    }
    quickSort(array, left, r);
    quickSort(array, r+1, right);
}

void swaps(char* array[], int i, int j) {
    char* temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

void quickSortString(char* array[], int left, int right) {
    if (right <= left) return;
    char* pivot = array[(left + right) / 2];
    int l = left - 1;
    int r = right + 1;
    for (;;) {
        while (strcmp(array[++l], pivot) < 0);
        while (strcmp(array[--r], pivot) > 0);
        if (l >= r) break;
        swaps(array, l, r);
    }
    quickSortString(array, left, r);
    quickSortString(array, r + 1, right);
}

void printArray(int array[], int max) {
    for(int i = 0; i < max; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

#include <sys/time.h>

unsigned long millis(void)
{
    struct timeval tv;
    static unsigned long epoch = 0;
    gettimeofday(&tv, 0);
    if (!epoch) epoch = tv.tv_sec * 1000L + tv.tv_usec / 1000;
    return tv.tv_sec * 1000L + tv.tv_usec / 1000 - epoch;
}

void fill(int array[], int left, int right)
{
    for (int i = left;  i <= right;  ++i)
	array[i] = rand() & 4095; // random number in the range [0..4095]
}

#define NMAX 40000 // increase to maximum required number of elements in the array


int main(int argc, char **argv)
{
    // fill array with NMAX random numbers
    char *strings[] = { "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog",};

    int num_strings = sizeof(strings) / sizeof(*strings);
    quickSortString(strings, 0, num_strings - 1);
    for (int i = 0; i < num_strings; ++i) {
        printf("%s\n", strings[i]);
    }
    /* for(int max = 2000; max <= NMAX; max +=2000) { */
    /* int array[max]; */
        /* int array1[max]; */
        /* fill(array, 0, max - 1); */

        /* memcpy(array1, array, max); */
        /* unsigned long t1 = millis(); */
        /* bubbleSort(array, 0, max - 1); */
        /* unsigned long t2 = millis(); */
        /* for (int i = 1;  i < max;  ++i) assert(array[i-1] <= array[i]); */
        /* unsigned long bubletime = t2-t1; */

        /* memcpy(array1, array, max); */
        /* t1 = millis(); */
        /* selectionSort(array1, 0, max - 1); */
        /* t2 = millis(); */
        /* for (int i = 1;  i < max;  ++i) assert(array1[i-1] <= array1[i]); */
        /* unsigned long selectiontime = t2-t1; */

        /* memcpy(array1, array, max); */
        /* t1 = millis(); */
        /* insertionSort(array1, 0, max - 1); */
        /* t2 = millis(); */
        /* for (int i = 1;  i < max;  ++i) assert(array1[i-1] <= array1[i]); */
        /* unsigned long insertiontime = t2-t1; */

        /* memcpy(array1, array, max); */
        /* t1 = millis(); */
        /* quickSort(array1, 0, max - 1); */
        /* t2 = millis(); */
        /* for (int i = 1;  i < max;  ++i) assert(array1[i-1] <= array1[i]); */
        /* unsigned long quicktime = t2-t1; */

        /* printf("%d, %lu, %lu, %lu\n", max, bubletime, selectiontime, insertiontime, quicktime); */
    /* } */

    return 0;
}
