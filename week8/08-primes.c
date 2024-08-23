#include <stdio.h>

#define ELT_T int

#include "08-array.c"
#include "08-clock.c"

#define ARRAYSIZE  50000
#define FILENAME   "08-primes-100000.txt"

int main(int argc, char **argv)
{
    // read ARRAYSIZE primes into an array

    struct Array *array = newArray(ARRAYSIZE);
    FILE *primes = fopen(FILENAME, "r");

    if (0 == primes) {
        fprintf(stderr, "Could not open "FILENAME"\n");
        exit(1);
    }

    for (size_t i = 0;  i < ARRAYSIZE;  ++i) {
        char line[32];
        if (0== fgets(line, sizeof(line), primes)) {
            fprintf(stderr, "EOF while reading "FILENAME"\n");
            fclose(primes);
            exit(1);
        }
        Array_put(array, i, atoi(line));
    }

    fclose(primes);

    unsigned long start = microsecondClock();

    /* int nPrimes = 0; */
    /* for (int i = 1;  i <= ARRAYSIZE;  ++i) { */
    /*     if (i % (ARRAYSIZE/10) == 0) printf("%i %i\n", i, nPrimes); */
    /*     if (Array_linSearch(array, i) >= 0) */
    /*         ++nPrimes; */
    /* } */
    /* printf("there are %i primes between 1 and %i\n", nPrimes, ARRAYSIZE); */

    int nPrimes = 0;
    for (int i = 1;  i <= ARRAYSIZE;  ++i) {
        if (i % (ARRAYSIZE/10) == 0) printf("%i %i\n", i, nPrimes);
        if (Array_binSearch(array, i) >= 0)
            ++nPrimes;
    }
    printf("there are %i primes between 1 and %i\n", nPrimes, ARRAYSIZE);


    unsigned long stop = microsecondClock();

    printf("elapsed time: %f seconds\n", (stop - start) / 1000000.0);

    return 0;
}
