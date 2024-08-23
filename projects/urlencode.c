#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define FORBIDDEN " !#$%&'()*+,/:;=?@[]"

void changeForbidden(char c) {
    printf("%%%02x", c);
}

void changeToSafe(char first, char second) {
    char hex[3] = {first, second, '\0'};
    printf("%c", (char)strtol(hex, NULL, 16));
}

void encode(char *url) {
    while(*url) {
        if (strrchr(FORBIDDEN, *url) != NULL) changeForbidden(*url);
        else putchar(*url);
        url++;
    }
    putchar('\n');
}

void decode(char *url) {
    while (*url) {
        if (*url == '%') {
            changeToSafe(*(url+1), *(url+2));
            url += 2;
        }
        else putchar(*url);
        url++;
    }
    putchar('\n');
}

int main(int argc, char **argv) {
    int encode_flag = 0;
    int decode_flag = 0;
    char url[1024]; 

    if (fgets(url, sizeof(url), stdin) == NULL) {
        printf("ERROR: Failed to read input\n");
        return -1;
    }

    char *ptr = url;
    while (*ptr) {
        if (strchr(FORBIDDEN, *ptr) != NULL) {
            encode_flag = 1;
        }  
        if (*ptr == '%' && isxdigit(*(ptr+1))) {
            decode_flag = 1;
        }
        ptr++;
    }

    if (decode_flag) {
        decode(url);
        return 0;
    }
    encode(url);
    return 0;
}
