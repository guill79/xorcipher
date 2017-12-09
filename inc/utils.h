#ifndef UTILS_H
#define UTILS_H

#include "types.h"

#define ALLOC_CHECK(ptr) \
    do { \
        if (ptr == NULL) { perror("Probème d'allocation"); exit(1); } \
    } while(0)

/* Fonctions pour tableaux */
byte *init_array(uint32 length);
byte **init_2d_array(uint32 length1, uint32 length2);
void expand_array(byte **array, uint32 new_length);
void free_array(byte **array, uint32 length);
void free_2d_array(byte ***array, uint32 length);

/* Fonctions pour I/O fichiers */
uint32 file_to_str(FILE *f, byte *str[]);
void str_to_file(byte str[], uint32 str_length, FILE *f);

#endif // UTILS_H
