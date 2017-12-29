#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../inc/utils.h"
#include "../inc/types.h"
#include "../inc/xor.h"
#include "../inc/c3.h"
#include "../inc/c2.h"

// Pour chaque indice i, pos[i] correspond à l'indice où la ième lettre commence dans le dictionnaire
static uint16 pos[] = {0, 24378, 39484, 73694, 114188, 149061, 161546, 171736, 177577, 189288, 191686, 192177, 199254, 215546, 219435, 224807, 251882, 253013, 288844, 311208, 327180, 328171, 335388, 335580, 335643, 335735};

byte **load_dict(char *dict_name, uint16 nb_words, uint8 max_word_length) {
    byte **dict = init_2d_array(nb_words, max_word_length);
    FILE *f = fopen(dict_name, "r");
    CHECK_FILE(f, dict_name);

    for (uint16 i = 0; i < nb_words; ++i) {
        if (fscanf(f, "%s", dict[i]) == 0) {
            fprintf(stderr, "Erreur lors du chargement du dictionnaire.\n");
            exit(EXIT_FAILURE);
        }
    }

    fclose(f);

    return dict;
}

bool is_delimiter(byte c) {
    if ((c == 0) || (c == 10) || (c == 32) || (c == 33) || (c == 34)
        || (c == 40) || (c == 41) || (c == 44) || (c == 46) || (c == 58)
        || (c == 59) || (c == 63)) {
        return true;
    }
    return false;
}

byte **extract_words(byte *str, uint32 str_length, uint32 *nb_words) {
    byte **words = init_2d_array(10, 1);
    uint32 cur_array_capacity = 10;
    uint8 start = 0,
          nb_letters = 0;

    *nb_words = 0;
    for (uint32 i = 0; i <= str_length; ++i) {
        if (is_delimiter(str[i])) {
            if (*nb_words >= cur_array_capacity) {
                expand_2d_array(&words, *nb_words + 10);
                cur_array_capacity += 10;
                for (uint16 j = *nb_words; j < cur_array_capacity; ++j) {
                    words[j] = init_array(1);
                }
            }

            expand_array(words + *nb_words, nb_letters + 1);
            memcpy(words[*nb_words], str + start, nb_letters);
            words[*nb_words][nb_letters] = '\0';

            ++(*nb_words);

            // On saute les éventuels autres délimiteurs (par ex. un espace
            // après un point)
            do ++i;
            while (i < str_length && is_delimiter(str[i]));

            start = i;
            nb_letters = 0;
        }
        ++nb_letters;
    }

    return words;
}

bool is_in_dict(byte *word, byte **dict, uint32 nb_words_dict) {
    char c = 0;
    uint32 p = 0;
    c = remove_diacritics(word[0]);
    if (c) {
        p = c - 97;
        word[0] = c;
    }

    uint16 i = pos[p];
    while (i < nb_words_dict && remove_diacritics(dict[i][0]) == c) {
        if (strcmp((char *) word, (char *) dict[i]) == 0) {
            return true;
        }
        ++i;
    }
    return false;
}

void c3(byte str_crypted[], uint32 str_length, byte **keys, uint32 nb_keys, byte **dict, uint32 nb_words_dict) {
    uint32 i_best_key = 0;
    uint32 max_score = 0;
    uint32 cur_score = 0;
    byte *str_decrypted = NULL;
    byte **words = NULL;
    uint32 nb_words_str = 0;
    uint8 word_length = 0;

    str_decrypted = init_array(str_length + 1);
    str_decrypted[str_length] = '\0';

    for (uint32 i_key = 0; i_key < nb_keys; ++i_key) {
        cur_score = 0;
        xor(str_crypted, str_decrypted, str_length, keys[i_key]);
        words = extract_words(str_decrypted, str_length, &nb_words_str);

        for (uint32 i_word = 0; i_word < nb_words_str; ++i_word) {
            word_length = strlen((char *) words[i_word]);
            if (2 <= word_length && word_length <= 4) {
                if (is_in_dict(words[i_word], dict, nb_words_dict)) ++cur_score;
            }
        }

        if (cur_score > max_score) {
            max_score = cur_score;
            i_best_key = i_key;
        }

        // Ici nb_words est le nombre d'éléments de words mais la fonction
        // extract_words a réalloué words par blocs de 10, il faut donc arrondir
        // nb_words à la dizaine supérieure pour libérer toute la mémoire
        free_2d_array(&words, nb_words_str / 10 * 10 + 10);
    }

    printf("%s\n", keys[i_best_key]);

    free_array(&str_decrypted);
}
