#include <iso646.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef  ELT_T
# define ELT_T char*
#endif

struct Array
{
    ELT_T *elements;	// memory containing
    size_t size;	// this many elements
};

/* make a new Array and pre-allocate enough space for size elements
 */
struct Array *newArray(size_t size)
{
    struct Array *array = malloc(sizeof(struct Array));
    array->elements = calloc(size, sizeof(ELT_T));
    array->size     = size;
    return array;
}

size_t Array_size(struct Array *array)
{
    return array->size;
}

/* print a message and exit if index is not within the bounds of the array
 */
void Array__boundsCheck(struct Array *array, size_t index)
{
    if (index >= array->size) {
	fprintf(stderr, "index %zi is out of bounds for array of size %zi\n", index, array->size);
	exit(1);
    }
}

/* return the element of the array at the given index
 */
ELT_T Array_get(struct Array *array, size_t index)
{
    Array__boundsCheck(array, index);
    return array->elements[index];
}

/* store the element into the array at the given index
 */
void Array_put(struct Array *array, size_t index, ELT_T element)
{
    Array__boundsCheck(array, index);
    array->elements[index] = element;
}

/* store the element into the array at the given index
 */
void Array_append(struct Array *array, ELT_T element)
{
    array->size += 1;
    array->elements = realloc(array->elements, sizeof(ELT_T) * array->size);
    array->elements[array->size-1] = element;
}

ssize_t Array_linSearch(struct Array *array, ELT_T element) {
    for (ssize_t i = 0; i < Array_size(array); i++) {
        if (strcmp(Array_get(array, i), element) == 0) {
            return i;
        }
    }
    return -1;
}

ssize_t Array_binSearch(struct Array *array, ELT_T element) {
    ssize_t left = 0;
    ssize_t right = Array_size(array) - 1;
    while (left <= right) {
        ssize_t mid = (left + right) / 2;
        int cmp = strcmp(element, Array_get(array, mid));
        if (cmp > 0) right = mid - 1;
        else if (cmp < 0) left = mid + 1;
        else return mid;
    }
    return -1 - left;
}

void swap(struct Array *array, size_t left, size_t right) {
    ELT_T temp = Array_get(array, left);
    array->elements[left] = Array_get(array, right);
    array->elements[right] = temp;
}

void Array_insert(struct Array *array, ELT_T element) {
    ssize_t index = Array_binSearch(array, element);
    if (index >= 0) return;
    
    index = -1 - index; 
    
    array->size++;
    array->elements = realloc(array->elements, array->size * sizeof(ELT_T));
    if (!array->elements) {
        return;
    }
    memmove(array->elements + index + 1, array->elements + index, (array->size - index - 1) * sizeof(ELT_T));
    array->elements[index] = element;
}
