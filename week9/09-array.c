#include <stdlib.h>
#include <stdio.h>

#ifdef  TEST_ARRAY
# define ELT_T int
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

int Array_isEmpty(struct Array *array)
{
    return 0 == array->size;
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

/* print a message and exit if array is empty
 */
void Array__emptyCheck(struct Array *array)
{
    if (0 == array->size) {
	fprintf(stderr, "array is empty\n");
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

/* return the last element of the array
 */
ELT_T Array_last(struct Array *array)
{
    Array__emptyCheck(array);
    return array->elements[array->size - 1];
}

/* store the element into the array at the given index
 */
void Array_append(struct Array *array, ELT_T element)
{
    array->size += 1;
    array->elements = realloc(array->elements, sizeof(ELT_T) * array->size);
    array->elements[array->size-1] = element;
}

/* store the element into the array at the given index
 */
ELT_T Array_removeLast(struct Array *array)
{
    Array__boundsCheck(array, 0);
    array->size -= 1;
    return array->elements[array->size];
}

#ifdef TEST_ARRAY

int main(int argc, char **argv)
{
    struct Array *array = newArray(0);

    return 0;
}

#endif
