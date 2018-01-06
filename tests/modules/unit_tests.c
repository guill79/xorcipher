#include <string.h>
#include "../../inc/utest.h"
#include "../../inc/types.h"
#include "../../inc/utils.h"
#include "../../inc/chars.h"
#include "../../inc/xor.h"
#include "../../inc/c1.h"
#include "../../inc/c2.h"
#include "../../inc/c3.h"

/* Dans ce module de test, on ne teste que les fonctions qui ne sont pas déjà
testées dans test_crack.sh et qui ne sont pas triviales */

/* Test des fonctions de c1.h */
void TEST_test_char_on_str(void) {
    byte str[25] = "Bonjour ceci est un test";
    byte str_crypted[25] = {0};
    uint32 str_length = 24;
    uint8 key_length = 3;

    xor(str, str_crypted, str_length, (byte *) "k:.");

    ASSERT(test_char_on_str('k', 0, key_length, str_crypted, str_length));
    ASSERT(test_char_on_str(':', 1, key_length, str_crypted, str_length));
    ASSERT(test_char_on_str('.', 2, key_length, str_crypted, str_length));
}

void TEST_extract_keys(void) {
    byte **key_chars = NULL;
    uint8 key_chars_length[] = {2, 3};
    uint8 key_length = 2;
    uint32 nb_keys = 0;
    byte **keys = NULL;

    key_chars = init_2d_array(2, 3);
    key_chars[0][0] = 'a';
    key_chars[0][1] = 'b';
    key_chars[1][0] = 'c';
    key_chars[1][1] = 'd';
    key_chars[1][2] = 'e';

    keys = extract_keys(key_chars, key_chars_length, key_length, &nb_keys);
    ASSERT(strcmp((char *) keys[0], "ac") == 0);
    ASSERT(strcmp((char *) keys[1], "ad") == 0);
    ASSERT(strcmp((char *) keys[2], "ae") == 0);
    ASSERT(strcmp((char *) keys[3], "bc") == 0);
    ASSERT(strcmp((char *) keys[4], "bd") == 0);
    ASSERT(strcmp((char *) keys[5], "be") == 0);
    ASSERT(nb_keys == 6);

    free_2d_array(&key_chars, 2);
    free_2d_array(&keys, nb_keys);
}

/* Test des fonctions de c2.h */
void TEST_frequency_analysis(void) {
    byte str[10] = "aaaabbbbc";
    uint32 str_length = 9;

    ASSERT((int) frequency_analysis(str, str_length) == 3848);
}

/* Test des fonctions de c3.h */
void TEST_extract_words(void) {
    byte str[54] = "Test de quoi ? ... de l'extraction,      des mots !!!";
    uint32 str_length = 53;
    uint32 nb_words = 0;
    byte **words = NULL;

    words = extract_words(str, str_length, &nb_words);

    ASSERT(strcmp((char *) words[0], "Test") == 0);
    ASSERT(strcmp((char *) words[1], "de") == 0);
    ASSERT(strcmp((char *) words[2], "quoi") == 0);
    ASSERT(strcmp((char *) words[3], "de") == 0);
    ASSERT(strcmp((char *) words[4], "l'extraction") == 0);
    ASSERT(strcmp((char *) words[5], "des") == 0);
    ASSERT(strcmp((char *) words[6], "mots") == 0);
    ASSERT(nb_words == 7);

    // extract_words réalloue words par blocs de 5, il faut donc arrondir à la
    // demi-dizaine supérieure
    free_2d_array(&words, nb_words / 5 * 5 + 5);
}

void TEST_is_in_dict(void) {
    byte **dict = NULL;
    long ****positions = NULL;

    // Initialisation de positions
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

    dict = load_dict(DICT_NAME, NB_WORDS_DICT, MAX_WORD_LENGTH, positions);

    int nb_words_in_to_test = 1949;
    int nb_words_not_in_to_test = 11;

    // Chargement des mots de test qui sont dans le dico
    FILE *file_words_in = fopen("tests/modules/c3/words_in_dict.txt", "r");
    byte **words_in = init_2d_array(nb_words_in_to_test, 5);
    for (int i = 0; i < nb_words_in_to_test; ++i) {
        if (fscanf(file_words_in, "%s", words_in[i]) == 0) {
            fprintf(stderr, "Erreur chargement fichier mots.\n");
            exit(1);
        }
    }

    // Chargement des mots de test qui ne sont pas dans le dico
    FILE *file_words_out = fopen("tests/modules/c3/words_not_in_dict.txt", "r");
    byte **words_not_in = init_2d_array(nb_words_not_in_to_test, 5);
    for (int i = 0; i < nb_words_not_in_to_test; ++i) {
        if (fscanf(file_words_out, "%s", words_not_in[i]) == 0) {
            fprintf(stderr, "Erreur chargement fichier mots.\n");
            exit(1);
        }
    }

    // Test des mots qui sont dans le dico
    for (int i = 0; i < nb_words_in_to_test; ++i) {
        ASSERT(is_in_dict(words_in[i], strlen((char *) words_in[i]), dict,
                          NB_WORDS_DICT, positions));
    }

    // Test des mots qui ne sont pas dans le dico
    for (int i = 0; i < nb_words_not_in_to_test; ++i) {
        ASSERT(!is_in_dict(words_not_in[i], strlen((char *) words_not_in[i]),
                           dict, NB_WORDS_DICT, positions));
    }

    free_2d_array(&dict, NB_WORDS_DICT);
    free_2d_array(&words_in, nb_words_in_to_test);
    free_2d_array(&words_not_in, nb_words_not_in_to_test);
    // Libération de positions
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

    fclose(file_words_in);
    fclose(file_words_out);
}

int main(void) {
    function functions_list[] = {
        {"C1", 0},
        {"test_char_on_str", TEST_test_char_on_str},
        {"extract_keys", TEST_extract_keys},
        {"C2", 0},
        {"frequency_analysis", TEST_frequency_analysis},
        {"C3", 0},
        {"extract_words", TEST_extract_words},
        {"is_in_dict", TEST_is_in_dict},
        {0}
    };

    run_tests(functions_list);

    return 0;
}
