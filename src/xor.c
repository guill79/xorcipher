#include <stdio.h>
#include <stdlib.h>
#include "../inc/xor.h"
#include "../inc/types.h"

byte *xor(byte str[], uint32 str_length, byte key[]) {
    byte *str_out = malloc(str_length * sizeof(byte));

    int k = 0;
    for (int i = 0; i < str_length; ++i) {
        if (key[k] == '\0') k = 0;
        str_out[i] = str[i] ^ key[k];
        ++k;
    }
    return str_out;
}
