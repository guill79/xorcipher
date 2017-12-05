#ifndef UTILS_H
#define UTILS_H

#include "types.h"

uint32 file_to_str(FILE *f, byte *str[]);
void str_to_file(byte str[], uint32 str_length, FILE *f);

#endif // UTILS_H
