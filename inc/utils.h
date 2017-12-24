#ifndef UTILS_H
#define UTILS_H

#include "types.h"

#define CHECK_PTR(ptr) \
    do { \
        if (ptr == NULL) { perror("Erreur"); exit(1); } \
    } while(0)

byte *init_array(uint32 length);
byte **init_2d_array(uint32 length1, uint32 length2);
void expand_array(byte **array, uint32 new_length);
void free_array(byte **array);
void free_2d_array(byte ***array, uint32 length);
bool is_zero(byte **array, uint16 nb_elem);
byte *file_to_str(FILE *f, uint32 *f_size);
void str_to_file(byte str[], uint32 str_length, FILE *f);

#endif // UTILS_H
