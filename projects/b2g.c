#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

unsigned long bin2gray(unsigned long val) {
    return (val ^ (val >> 1));
}

unsigned long gray2bin(unsigned long val) {
    unsigned long temp = 0b0;
    while(val > 0b0) {
        temp = (temp ^ val);
        val = (val >> 1);
    }
    return temp; 
}

void print2(unsigned long b) {
    if (b > 1) print2(b / 2);
    putchar('0' + b % 2);
}

int main(int argc, char **argv) {
    assert(bin2gray(0b10001000) == 0b11001100);
    assert(gray2bin(0b11001100) == 0b10001000);
    for (unsigned long bin1 = 0;  bin1 < 256;  ++bin1) {
        unsigned long gray = bin2gray(bin1);
        unsigned long bin2 = gray2bin(gray);
        print2(bin1);   printf("->");
        print2(gray);   printf("->");
        print2(bin2);   printf("\n");
        assert(bin1 == bin2);
    }
    return 0;
}
