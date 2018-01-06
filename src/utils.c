#include <stdio.h>
#include <stdlib.h>
#include "../inc/utils.h"
#include "../inc/chars.h"

/**
 *  Fonction : init_array
 *  ----------------------
 *  Crée un tableau de byte de longueur length.
 *
 *  Paramètre :
 *    - length : taille voulue pour le tableau
 *
 *  Renvoie : un tableau de byte de taille length.
 */
byte *init_array(uint32 length) {
    byte *array = malloc(length * sizeof(byte));
    CHECK_PTR(array);

    return array;
}

/**
 *  Fonction : init_2d_array
 *  -------------------------
 *  Crée un tableau 2D de byte de longueur length1, chacun de longueur length2.
 *
 *  Paramètres :
 *    - length1 : taille voulue pour la première dimension
 *    - length2 : taille voulue pour la deuxième dimension
 *
 *  Renvoie : un tableau à deux dimensions de byte.
 */
byte **init_2d_array(uint32 length1, uint32 length2) {
    byte **array = malloc(length1 * sizeof(byte *));
    CHECK_PTR(array);

    for (uint32 i = 0; i < length1; ++i) {
        array[i] = malloc(length2 * sizeof(byte));
        CHECK_PTR(array[i]);
    }

    return array;
}

/**
 *  Fonction : expand_array
 *  ------------------------
 *  Passe la taille du tableau array à new_length.
 *
 *  Paramètres :
 *    - array      : tableau de byte dont on veut augmenter la taille
 *    - new_length : nouvelle taille désirée pour array
 */
void expand_array(byte **array, uint32 new_length) {
    *array = realloc(*array, new_length * sizeof(byte));
    CHECK_PTR(*array);
}

/**
 *  Fonction : expand_2d_array
 *  ---------------------------
 *  Passe la taille du tableau 2D array à new_length.
 *
 *  Paramètres :
 *    - array      : tableau de byte* dont on veut augmenter la taille
 *    - new_length : nouvelle taille désirée pour array
 */
void expand_2d_array(byte ***array, uint32 new_length) {
    *array = realloc(*array, new_length * sizeof(byte *));
    CHECK_PTR(*array);
}

/**
 *  Fonction : free_array
 *  ----------------------
 *  Libère le tableau array de la mémoire.
 *
 *  Paramètre :
 *    - array : adresse du tableau à libérer
 */
void free_array(byte **array) {
    free(*array);
    *array = NULL;
}

/**
 *  Fonction : free_2d_array
 *  -------------------------
 *  Libère le tableau 2D array de la mémoire.
 *
 *  Paramètres :
 *    - array  : adresse du tableau 2D à libérer
 *    - length : taille du tableau pointé par array
 */
void free_2d_array(byte ***array, uint32 length) {
    for (uint32 i = 0; i < length; ++i) {
        free((*array)[i]);
        (*array)[i] = NULL;
    }
    free(*array);
    *array = NULL;
}

/**
 *  Fonction : file_to_str
 *  -----------------------
 *  Copie le contenu de f dans une chaîne de caractères.
 *
 *  Paramètres :
 *    - f      : pointeur sur le fichier à copier
 *    - f_size : adresse du uint32 dans lequel on veut stocker la taille du
 *               fichier, et donc de la chaîne renvoyée
 *
 *  Renvoie : une chaîne de caractères contenant le contenu du fichier f.
 */
byte *file_to_str(FILE *f, uint32 *f_size) {
    /* Récupération de la taille du fichier */
    fseek(f, 0, SEEK_END);
    *f_size = ftell(f);
    rewind(f);

    byte *str = init_array(*f_size + 1);

    /* Lecture du fichier */
    if (fread(str, sizeof(byte), *f_size, f) != *f_size) {
        perror("Erreur lecture fichier");
        exit(EXIT_FAILURE);
    }

    str[*f_size] = '\0';

    return str;
}

/**
 *  Fonction : str_to_file
 *  -----------------------
 *  Copie la chaîne str dans le fichier f.
 *
 *  Paramètres :
 *    - str        : chaîne de caractères à copier
 *    - str_length : taille de str
 *    - f          : pointeur sur le fichier dans lequel on veut copier str
 */
void str_to_file(byte str[], uint32 str_length, FILE *f) {
    for (uint32 i = 0; i < str_length; ++i) {
        fputc(str[i], f);
    }
}

/**
 *  Fonction : is_zero
 *  -------------------
 *  Détermine s'il existe un i dans array tel que array[i][0] = 0.
 *
 *  Paramètres :
 *    - array   : tableau 2D à tester
 *    - nb_elem : nombre d'éléments de array
 *
 *  Renvoie :
 *    - true si un tel i existe,
 *    - false sinon.
 */
bool is_zero(byte **array, uint16 nb_elem) {
    for (uint16 i = 0; i < nb_elem; ++i) {
        if (array[i][0] == 0) return true;
    }

    return false;
}

/**
 *  Fonction : is_valid_key
 *  ------------------------
 *  Détermine si la clé key est valide.
 *
 *  Paramètre :
 *    - key : clé à tester
 *
 *  Renvoie :
 *    - true si key est valide,
 *    - false sinon.
 */
bool is_valid_key(byte key[]) {
    uint8 i = 0;

    while (key[i] != '\0') {
        if (!char_valid_key(key[i])) return false;
        ++i;
    }

    return true;
}
