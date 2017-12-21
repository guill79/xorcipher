#include <stdio.h>
#include <stdlib.h>
#include "../inc/xor.h"
#include "../inc/types.h"
#include "../inc/utils.h"

void xor(byte str_in[], byte str_out[], uint32 str_length, byte key[]) {
    int k = 0;
    for (int i = 0; i < str_length; ++i) {
        if (key[k] == '\0') k = 0;
        str_out[i] = str_in[i] ^ key[k];
        ++k;
    }
    str_out[str_length] = '\0';
}
