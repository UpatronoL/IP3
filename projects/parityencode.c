#include <stdio.h>

typedef unsigned char byte;

int getParity(int data) {
    data ^= data >> 4;
    data ^= data >> 2;
    data ^= data >> 1;
    return data & 1;
}

int addParity(int data) {
    int parity = getParity(data);
    return (data & 0x7F) | (parity << 7);
}

void print2(unsigned long b) {
    if (b > 1) print2(b / 2);
    putchar('0' + b % 2);
}

size_t encodeBlocks(byte *input, size_t length, byte *output) {
    for (int i = 0; i < length; i++) {
        if (i % 7 == 0) {
            // make a parity word
            i++;
            length++;
        }
        output[i] = addParity(input[i]);
    }
    output[i] = '\0';
    return output;
}

int main(int argc, char **argv) {
    return 0;
}
