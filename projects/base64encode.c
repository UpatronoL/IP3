#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define ENCODE_ARRAY "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

char *encodeB64(char *string, size_t string_len) {
    size_t output_len = 4 * ((string_len + 2) / 3);
    char *output_string = calloc(output_len, sizeof(*output_string));
    for(size_t i = 0, j = 0; i < string_len;) {
        size_t a = i < string_len ? string[i++] : 0;
        size_t b = i < string_len ? string[i++] : 0;
        size_t c = i < string_len ? string[i++] : 0;

        size_t total_val = (a << 16) | (b << 8) | c;

        output_string[j++] = ENCODE_ARRAY[(total_val >> 18) & 0x3F];
        output_string[j++] = ENCODE_ARRAY[(total_val >> 12) & 0x3F];
        output_string[j++] = ENCODE_ARRAY[(total_val >> 6) & 0x3F];
        output_string[j++] = ENCODE_ARRAY[total_val & 0x3F];
    }

    for (size_t i = 0; i < (3 - string_len % 3) % 3; i++)
        output_string[output_len - 1 - i] = '=';

    output_string[output_len] = '\0';
    return output_string;
}

char *decodeB64(char *string, size_t string_len) {
    if (string_len % 4 != 0) return 0;

    int *decoding_table = calloc(256, sizeof(int));

    for (int i = 0; i < 64; i++) {
        decoding_table[ENCODE_ARRAY[i]] = i;
    }

    size_t output_len = string_len / 4 * 3;
    if (string[string_len - 1] == '=') (output_len)--;
    if (string[string_len - 2] == '=') (output_len)--;

    char *output_string = malloc(output_len);
    if (output_string == NULL) return 0;

    for (size_t i = 0, j = 0; i < string_len;) {
        uint32_t a = string[i] == '=' ? 0 & i++ : decoding_table[(unsigned char)string[i++]];
        uint32_t b = string[i] == '=' ? 0 & i++ : decoding_table[(unsigned char)string[i++]];
        uint32_t c = string[i] == '=' ? 0 & i++ : decoding_table[(unsigned char)string[i++]];
        uint32_t d = string[i] == '=' ? 0 & i++ : decoding_table[(unsigned char)string[i++]];

        uint32_t triple = (a << 18) | (b << 12) | (c << 6) | d;

        if (j < output_len) output_string[j++] = (triple >> 16) & 0xFF;
        if (j < output_len) output_string[j++] = (triple >> 8) & 0xFF;
        if (j < output_len) output_string[j++] = triple & 0xFF;
    }

    output_string[output_len] = '\0';
    return output_string;
}

int isBase64Encoded(char *string, size_t string_len) {
    if (string_len % 4 != 0) return 0;

    for (int i = 0; i < string_len; i++) {
    if (!isalnum(string[i]) && string[i] != '+' && string[i] != '/' && string[i] != '=') {
            return 0;
        }
    }

    if (string[string_len - 1] == '=') {
        if (string[string_len - 2] == '=') {
            if (string_len >= 3 && string[string_len - 3] == '=') {
                return 0;
            }
        }
        else if (string[string_len - 2] != '=') {
        } else {
            return 0;
        }
    }

    return 1;
}

int main(int argc, char **argv) {
    char message[1024]; 

    if (fgets(message, sizeof(message), stdin) == NULL) return -1;
    if (isBase64Encoded(message, strlen(message) - 1)) { 
        printf("%s\n", decodeB64(message, strlen(message) - 1));
    } else {
        printf("%s\n", encodeB64(message, strlen(message)));
    }
    return 0;
}
