#include <stdio.h>
#include <stdlib.h>
#include "../inc/utils.h"
#include "../inc/types.h"
#include "../inc/xor.h"

/**
 *  Fonction : xor
 *  ---------------
 *  Applique l'algorithme de chiffrement/déchiffrement XOR sur la chaîne str_in.
 *
 *  Paramètres :
 *    - str_in     : chaîne à chiffrer
 *    - str_out    : chaîne dans laquelle stocker le résultat du chiffrement
 *    - str_length : longueur de str_in
 *    - key        : clé de chiffrement
 */
void xor(byte str_in[], byte str_out[], uint32 str_length, byte key[]) {
    uint8 k = 0;
    for (uint32 i = 0; i < str_length; ++i) {
        if (key[k] == '\0') k = 0;
        str_out[i] = str_in[i] ^ key[k];
        ++k;
    }
    str_out[str_length] = '\0';
}
