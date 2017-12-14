#include <stdio.h>
#include <stdlib.h>
#include "../inc/utils.h"

byte *init_array(uint32 length) {
    byte *array = malloc(length * sizeof(byte));
    ALLOC_CHECK(array);

    return array;
}

byte **init_2d_array(uint32 length1, uint32 length2) {
    byte **array = malloc(length1 * sizeof(byte *));
    ALLOC_CHECK(array);

    for (int i = 0; i < length1; ++i) {
        array[i] = malloc(length2 * sizeof(byte));
        ALLOC_CHECK(array[i]);
    }

    return array;
}

void expand_array(byte **array, uint32 new_length) {
    *array = realloc(*array, new_length * sizeof(byte));
    ALLOC_CHECK(*array);
}

void free_array(byte **array, uint32 length) {
    free(*array);
    *array = NULL;
}

void free_2d_array(byte ***array, uint32 length) {
    for (int i = 0; i < length; ++i) {
        free((*array)[i]);
        (*array)[i] = NULL;
    }
    free(*array);
    *array = NULL;
}

uint32 file_to_str(FILE *f, byte *str[]) {
    uint32 f_size = 0;

    /* Récupération de la taille du fichier */
    fseek(f, 0, SEEK_END);
    f_size = ftell(f);
    rewind(f);

    *str = malloc(f_size * sizeof(byte) + 1);

    /* Lecture du fichier */
    if (fread(*str, sizeof(byte), f_size, f) != f_size) {
        perror("Erreur lecture fichier");
        return -1;
    }

    (*str)[f_size] = '\0';

    return f_size;
}

void str_to_file(byte str[], uint32 str_length, FILE *f) {
    for (int i = 0; i < str_length; ++i) {
        fputc(str[i], f);
    }
}
