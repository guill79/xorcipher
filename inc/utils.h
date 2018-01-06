#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "types.h"

#define CHECK_PTR(ptr) \
    do { \
        if (ptr == NULL) { perror("Erreur"); exit(EXIT_FAILURE); } \
    } while (0)

#define CHECK_FILE(file, name) \
    do { \
        if (file == NULL) { \
            fprintf(stderr, "Fichier \"%s\" inaccessible.\n", name); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

byte *init_array(uint32 length);
byte **init_2d_array(uint32 length1, uint32 length2);
void expand_array(byte **array, uint32 new_length);
void expand_2d_array(byte ***array, uint32 new_length);
void free_array(byte **array);
void free_2d_array(byte ***array, uint32 length);
byte *file_to_str(FILE *f, uint32 *f_size);
void str_to_file(byte str[], uint32 str_length, FILE *f);
bool is_zero(byte **array, uint16 nb_elem);
bool is_valid_key(byte key[]);

#endif // UTILS_H
