#ifndef UTILS_H
#define UTILS_H

#include "types.h"

/* Fonctions pour tableaux */
void init_array(byte ***array, uint8 length);
void expand_array(byte **array, uint32 new_length);
void free_array(byte **array, uint32 length);
void free_2d_array(byte ***array, uint32 length);

/* Fonctions pour I/O fichiers */
uint32 file_to_str(FILE *f, byte *str[]);
void str_to_file(byte str[], uint32 str_length, FILE *f);

#endif // UTILS_H
