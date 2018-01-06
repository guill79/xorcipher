#ifndef C2_H
#define C2_H

#include "types.h"

#define ALPHABET_LENGTH 26

double frequency_analysis(byte str[], uint32 str_length);
byte *c2(byte str_crypted[], uint32 str_length, byte **keys, uint32 nb_keys,
        uint8 key_length);

#endif // C2_H
