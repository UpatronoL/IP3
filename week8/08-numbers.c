#include <stdio.h>

#define ELT_T int

#include "08-array.c"
#include "08-clock.c"

#define ARRAYSIZE  50000
#define FILENAME   "08-numbers-100000.txt"

int main(int argc, char **argv)
{
    // read ARRAYSIZE numbers into an array

    struct Array *array = newArray(0);
    FILE *numbers = fopen(FILENAME, "r");

    if (0 == numbers) {
	fprintf(stderr, "Could not open "FILENAME"\n");
	exit(1);
    }

    unsigned long start = microsecondClock();

    for (size_t i = 0;  i < ARRAYSIZE;  ++i) {
	char line[32];
	if (0 == fgets(line, sizeof(line), numbers)) {
	    fprintf(stderr, "EOF while reading "FILENAME"\n");
	    fclose(numbers);
	    exit(1);
	}
	int number = atoi(line);
	if (Array_binSearch(array, number) < 0)
	    Array_insert(array, number);
    }

    unsigned long stop = microsecondClock();

    fclose(numbers);

    for (int i = 1;  i <= 20;  ++i) printf("%i\n", Array_get(array, i));

    printf("elapsed time: %f seconds\n", (stop - start) / 1000000.0);

    return 0;
}
