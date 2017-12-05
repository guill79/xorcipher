#ifndef C2_H
#define C2_H

#include "types.h"

uint8 frequency_char(byte str[], uint32 str_length, byte c);
uint16 frequency_analysis(byte str[], uint32 str_length);
void c2(byte str[], uint32 str_length, byte **keys, uint32 nb_keys);

#endif // C2_H
