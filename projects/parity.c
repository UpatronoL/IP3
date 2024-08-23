#include <stdio.h>

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

int main(int argc, char **argv) {
    char text [48] = "Peter piper picked a peck of pickled peppers. \n";
    for(int i = 0; i < sizeof(text); i++) {
        text[i] = addParity(text[i]);
    }
    for(int i = 0; i < sizeof(text); i++) {
        putchar (getParity(text [i]) ? '?' : text[i] & 0x7F);
    }
    text[ 5] ^= (1 << 2);
    text[ 7] ^= (1 << 3);
    text[11] ^= (1 << 5);
    text[13] ^= (1 << 7);
    text[17] ^= (1 << 4);

    for(int i = 0; i < sizeof(text); i++) {
        putchar (getParity(text [i]) ? '?' : text[i] & 0x7F);
    }
    return 0;
}
