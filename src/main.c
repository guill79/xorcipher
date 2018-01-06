#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "../inc/utils.h"
#include "../inc/chars.h"
#include "../inc/xor.h"
#include "../inc/c1.h"
#include "../inc/c2.h"
#include "../inc/c3.h"

/**
 *  Fonction : print_usage
 *  -----------------------
 *  Affiche l'usage de la commande et termine l'exécution du programme.
 *
 *  Paramètre :
 *    - prog_name : nom du programme
 */
void print_usage(char *prog_name) {
    fprintf(stderr,
            "Erreur commande.\n"
            "Usage :\n"
            "  Chiffrement/déchiffrement : %s -i input_file -o output_file "
                                             "-k key\n"
            "  Décryptage                : %s -i input_file -m mode "
                                             "[-l key_length]\n",
            prog_name, prog_name);
    exit(EXIT_FAILURE);
}

/**
 *  Fonction : launch_xor_module
 *  -----------------------------
 *  Prépare les données nécessaires et lance le module XOR.
 *
 *  Paramètres :
 *    - file_out   : nom du fichier de sortie
 *    - str_in     : chaîne à chiffrer/déchiffrer
 *    - str_length : longueur str_in
 *    - key        : clé de chiffrement
 */
void launch_xor_module(char *file_out, byte *str_in, uint32 str_length,
                       byte *key) {
    FILE *f_out = NULL;
    byte *str_out = NULL;

    f_out = fopen(file_out, "w");
    CHECK_FILE(f_out, file_out);

    str_out = init_array(str_length + 1);
    str_out[str_length] = '\0';

    xor(str_in, str_out, str_length, key);
    str_to_file(str_out, str_length, f_out);

    free_array(&str_out);
    fclose(f_out);
}

/**
 *  Fonction : launch_c1_module
 *  ----------------------------
 *  Prépare les données nécessaires et lance le module C1.
 *
 *  Paramètres :
 *    - str_in     : chaîne à tester
 *    - str_length : longueur de str_in
 *    - key_length : longueur de la clé
 */
void launch_c1_module(byte *str_in, uint32 str_length, uint8 key_length) {
    byte *key_chars_length = NULL;
    byte **key_chars = NULL;

    key_chars_length = init_array(key_length);

    key_chars = c1(str_in, str_length, key_length, key_chars_length);

    // Affichage des caractères
    if (!is_zero(key_chars, key_length)) {
        for (uint8 i = 0; i < key_length; ++i) {
            print_chars(key_chars[i], key_chars_length[i]);
        }
        printf("\n");
    }

    free_2d_array(&key_chars, key_length);
    free_array(&key_chars_length);
}

/**
 *  Fonction : launch_c2_module
 *  ----------------------------
 *  Prépare les données nécessaires et lance le module C2.
 *
 *  Paramètres :
 *    - str_in     : chaîne à tester
 *    - str_length : longueur de str_in
 *    - key_length : longueur de la clé
 */
void launch_c2_module(byte *str_in, uint32 str_length, uint8 key_length) {
    uint8 *key_chars_length = NULL;
    byte **key_chars = NULL,
         **keys = NULL;
    uint32 nb_keys = 0;

    key_chars_length = init_array(str_length);

    // Récupération des caractères valides depuis C1
    key_chars = c1(str_in, str_length, key_length, key_chars_length);

    if (!is_zero(key_chars, key_length)) {
        keys = extract_keys(key_chars, key_chars_length, key_length, &nb_keys);
        printf("%s\n", c2(str_in, str_length, keys, nb_keys, key_length));

        free_2d_array(&keys, nb_keys);
    }

    free_2d_array(&key_chars, key_length);
    free_array(&key_chars_length);
}

/**
 *  Fonction : launch_c3_module
 *  ----------------------------
 *  Prépare les données nécessaires et lance le module C3.
 *
 *  Paramètres :
 *    - str_in     : chaîne à tester
 *    - str_length : longueur de str_in
 *    - key_length : longueur de la clé
 *    - dict       : tableau contenant les mots du dictionnaire
 *    - positions  : tableau 4D contenant la position des mots (voir c3.c pour
 *                   plus de détails)
 */
void launch_c3_module(byte *str_in, uint32 str_length, uint8 key_length,
                      byte **dict, long ****positions) {
    uint8 *key_chars_length = NULL;
    byte **key_chars = NULL,
         **keys = NULL;
    uint32 nb_keys = 0;

    key_chars_length = init_array(str_length);

    key_chars = c1(str_in, str_length, key_length, key_chars_length);

    if (!is_zero(key_chars, key_length)) {

        keys = extract_keys(key_chars, key_chars_length, key_length, &nb_keys);

        printf("%s\n", c3(str_in, str_length, keys, nb_keys, dict,
                          NB_WORDS_DICT, positions));

        free_2d_array(&keys, nb_keys);
    }

    free_2d_array(&key_chars, key_length);
    free_array(&key_chars_length);
}

/**
 *  Fonction : check_args
 *  ----------------------
 *  Vérifie la validité des arguments de la ligne de commande.
 *
 *  Paramètres :
 *    - file_out       : nom du fichier de sortie
 *    - key            : clé de chiffrement
 *    - key_length_str : longueur de la clé depuis la ligne de commande
 *    - mode_str       : mode choisi depuis la ligne de commande
 *    - mode           : adresse du uint8 dans lequel on veut stocker le mode
 *                       choisi sous la forme d'un entier
 *    - key_length     : adresse du uint16 dans lequel on veut stocker la
 *                       longueur de la clé sous la forme d'un entier
 *    - prog_name      : nom du programme
 */
void check_args(char *file_out, char *key, char *key_length_str, char *mode_str,
                uint8 *mode, uint16 *key_length, char *prog_name) {
    // Mode chiffrement/déchiffrement
    if (file_out != NULL && key != NULL && mode_str == NULL
        && key_length_str == NULL) {

        if (!is_valid_key((byte *) key)) {
            fprintf(stderr, "Clé invalide.\n");
            exit(EXIT_FAILURE);
        }
        *mode = 0;
    }
    // Mode décryptage
    else if (mode_str != NULL && file_out == NULL && key == NULL) {
        // Si longueur fournie
        if (key_length_str != NULL) {
            *key_length = atoi(key_length_str);
            // Si longueur invalide (doit tenir sur un uint16)
            if (*key_length <= 0 || *key_length > 65535) {
                fprintf(stderr, "Erreur longueur clé.\n");
                exit(EXIT_FAILURE);
            }
        }

        // Vérification du mode
        *mode = atoi(mode_str);
        if (*mode != 1 && *mode != 2 && *mode != 3) {
            fprintf(stderr, "Erreur mode : doit être compris entre 1 et 3.\n");
            exit(EXIT_FAILURE);
        }
    } else {
        print_usage(prog_name);
    }
}

int main(int argc, char *argv[]) {
    // clock_t start, end = 0;
    char *file_in = NULL,
         *file_out = NULL,
         *key = NULL,
         *mode_str = NULL,
         *key_length_str = NULL;
    uint8 mode = 0;
    uint16 key_length = 0;
    int opt = 0;
    opterr = 0;
    uint32 str_length = 0;
    byte **dict = NULL;
    long ****positions = NULL;

    while ((opt = getopt(argc, argv, "i:o:k:m:l:")) != -1) {
        switch (opt) {
            case 'i':
                file_in = optarg;
                break;
            case 'o':
                file_out = optarg;
                break;
            case 'k':
                key = optarg;
                break;
            case 'm':
                mode_str = optarg;
                break;
            case 'l':
                key_length_str = optarg;
                break;
            case '?':
            default:
                print_usage(argv[0]);
                break;       
        }
    }

    if (optind != argc) print_usage(argv[0]);

    check_args(file_out, key, key_length_str, mode_str, &mode, &key_length,
               argv[0]);

    // Ouverture du fichier d'entrée et copie dans str_in
    FILE *f_in = fopen(file_in, "r");
    CHECK_FILE(f_in, file_in);
    byte *str_in = file_to_str(f_in, &str_length);

    // start = clock();
    switch (mode) {
        case 0:
            launch_xor_module(file_out, str_in, str_length, (byte *) key);
            break;
        case 1:
            if (key_length != 0) {
                launch_c1_module(str_in, str_length, key_length);
            } else {
                for (uint8 length = 3; length < 8; ++length) {
                    launch_c1_module(str_in, str_length, length);
                }
            }
            break;
        case 2:
            if (key_length != 0) {
                launch_c2_module(str_in, str_length, key_length);
            } else {
                for (uint8 length = 3; length < 8; ++length) {
                    launch_c2_module(str_in, str_length, length);
                }
            }
            break;
        case 3:
            // Initialisation du tableau 4D positions à -1
            positions = malloc(27 * sizeof(long ***));
            CHECK_PTR(positions);
            for (uint8 i = 0; i < 27; ++i) {
                positions[i] = malloc(28 * sizeof(long **));
                CHECK_PTR(positions[i]);
                for (uint8 j = 0; j < 28; ++j) {
                    positions[i][j] = malloc(28 * sizeof(long *));
                    CHECK_PTR(positions[i][j]);
                    for (uint8 k = 0; k < 28; ++k) {
                        positions[i][j][k] = malloc(28 * sizeof(long));
                        CHECK_PTR(positions);
                        for (uint8 l = 0; l < 28; ++l) {
                            positions[i][j][k][l] = -1;
                        }
                    }
                }
            }

            dict = load_dict(DICT_NAME, NB_WORDS_DICT, MAX_WORD_LENGTH,
                             positions);

            if (key_length != 0) {
                    launch_c3_module(str_in, str_length, key_length, dict,
                                     positions);
            } else {
                for (uint8 length = 3; length < 8; ++length) {
                    launch_c3_module(str_in, str_length, length, dict,
                                     positions);
                }
            }

            free_2d_array(&dict, NB_WORDS_DICT);
            // Libération du tableau positions
            for (int i = 0; i < 27; ++i) {
                for (int j = 0; j < 28; ++j) {
                    for (int k = 0; k < 28; ++k) {
                        free(positions[i][j][k]);
                        positions[i][j][k] = NULL;
                    }
                    free(positions[i][j]);
                    positions[i][j] = NULL;
                }
                free(positions[i]);
                positions[i] = NULL;
            }
            free(positions);
            positions = NULL;
            break;

        // Pas besoin de default : le mode a déjà été vérifié
    }
    // end = clock();
    // printf("Temps CPU : %1.2lf s\n", ((double) end - start) / CLOCKS_PER_SEC);

    free_array(&str_in);
    fclose(f_in);

    return 0;
}
