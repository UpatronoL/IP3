#include <stdio.h>
#include <string.h>

#define ELT_T char *

#include "08-array.c"
#include "08-clock.c"

#define ARRAYSIZE  20000
#define FILENAME   "08-strings-100000.txt"

int main(int argc, char **argv)
{
    // read ARRAYSIZE strings into an array

    struct Array *array = newArray(0);
    FILE *strings = fopen(FILENAME, "r");

    if (0 == strings) {
	fprintf(stderr, "Could not open "FILENAME"\n");
	exit(1);
    }

    unsigned long start = microsecondClock();

    for (size_t i = 0;  i < ARRAYSIZE;  ++i) {
        char line[32];
        if (0== fgets(line, sizeof(line), strings)) {
            fprintf(stderr, "EOF while reading "FILENAME"\n");
            fclose(strings);
            exit(1);
        }
        Array_insert(array, strdup(line));
    }

    unsigned long stop = microsecondClock();

    fclose(strings);

    for (int i = 0;  i < 20;  ++i) printf("%s", Array_get(array, i));

    printf("elapsed time: %f seconds\n", (stop - start) / 1000000.0);

    return 0;
}
