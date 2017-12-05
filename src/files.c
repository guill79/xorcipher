#include <stdio.h>
#include <stdlib.h>
#include "../inc/files.h"

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
        return 0;
    }

    (*str)[f_size] = '\0';

    return f_size;
}

void str_to_file(byte str[], uint32 str_length, FILE *f) {
    for (int i = 0; i < str_length; ++i) {
        fputc(str[i], f);
    }
}
