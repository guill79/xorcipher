#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../inc/utils.h"
#include "../inc/types.h"
#include "../inc/xor.h"
#include "../inc/c2.h"
#include "../inc/c3.h"

byte **load_dict(char *dict_name, uint16 nb_words, uint8 max_word_length, long ****positions) {
    byte **dict = NULL;
    FILE *f = NULL;

    f = fopen(dict_name, "r");
    CHECK_FILE(f, dict_name);

    dict = calloc(nb_words, sizeof(byte *));
    CHECK_PTR(dict);

    int cur_letters4[4] = {-1};
    int cur_letters3[3] = {-1};
    int cur_letters2[2] = {-1};
    int cur_letters1[1] = {-1};

    uint8 i = 0, j = 0, k = 0, l = 0;
    for (uint16 i_word = 0; i_word < nb_words; ++i_word) {
        dict[i_word] = calloc(max_word_length, sizeof(byte));
        CHECK_PTR(dict[i_word]);

        if (fscanf(f, "%s", dict[i_word]) == 0) {
            fprintf(stderr, "Erreur lors du chargement du dictionnaire.\n");
            exit(EXIT_FAILURE);
        } else {
            // Si le caractère est un tiret, on ne peut pas appeler
            // remove_diacritics qui renverrait 0. On met donc le caractère
            // à 123 pour le placer aux indices 26 de positions
            // (les indices 26 sont réservés aux tirets).
            if (dict[i_word][0] == 45) i = 123;
            else i = remove_diacritics(dict[i_word][0]);

            if (dict[i_word][1] == 45) j = 123;
            else j = remove_diacritics(dict[i_word][1]);

            if (dict[i_word][2] == 45) k = 123;
            else k = remove_diacritics(dict[i_word][2]);

            if (dict[i_word][3] == 45) l = 123;
            else l = remove_diacritics(dict[i_word][3]);

            if (i && j && k && l && (i != cur_letters4[0] || j != cur_letters4[1]
                                     || k != cur_letters4[2] || l != cur_letters4[3])) {
                cur_letters4[0] = i;
                cur_letters4[1] = j;
                cur_letters4[2] = k;
                cur_letters4[3] = l;
                positions[i-97][j-97][k-97][l-97] = i_word;
            }
            if (i && j && k && (i != cur_letters3[0] || j != cur_letters3[1]
                                     || k != cur_letters3[2])) {
                cur_letters3[0] = i;
                cur_letters3[1] = j;
                cur_letters3[2] = k;
                positions[i-97][j-97][k-97][27] = i_word;
            }
            if (i && j && (i != cur_letters2[0] || j != cur_letters2[1])) {
                cur_letters2[0] = i;
                cur_letters2[1] = j;
                positions[i-97][j-97][27][27] = i_word;
            }
            if (i && (i != cur_letters1[0])) {
                cur_letters1[0] = i;
                positions[i-97][27][27][27] = i_word;
            }
        }
    }

    fclose(f);

    return dict;
}

bool is_delimiter(byte c) {
    switch (c) {
        case 13: case 32: case 33: case 34: case 44: case 46: case 58:
        case 59: case 63:
            return true;
        default:
            return false;
    }
}

byte **extract_words(byte *str, uint32 str_length, uint32 *nb_words) {
    byte **words = init_2d_array(5, 1);
    uint32 cur_array_capacity = 5;
    uint8 start = 0,
          nb_letters = 0;

    *nb_words = 0;
    for (uint32 i = 0; i <= str_length; ++i) {
        if (is_delimiter(str[i])) {
            expand_array(words + *nb_words, nb_letters + 1);
            memcpy(words[*nb_words], str + start, nb_letters);
            words[*nb_words][nb_letters] = '\0';

            ++(*nb_words);
            if (*nb_words == cur_array_capacity) {
                expand_2d_array(&words, *nb_words + 5);
                cur_array_capacity += 5;
                for (uint16 j = *nb_words; j < cur_array_capacity; ++j) {
                    words[j] = init_array(1);
                }
            }

            // On saute les éventuels autres délimiteurs (par ex. un espace
            // après un point)
            do ++i;
            while (is_delimiter(str[i]) && i < str_length);

            start = i;
            nb_letters = 0;
        }
        ++nb_letters;
    }

    return words;
}

byte to_lower(byte c) {
    if (c >= 65 && c <= 90) {
        return c + 32;
    }
    return c;
}

bool is_in_dict(byte *word, uint8 word_length, byte **dict, uint32 nb_words_dict, long ****positions) {
    uint8 i = 0, j = 0, k = 0, l = 0;
    long i_start = 0;
    uint8 i_char_max = word_length - 1;
    byte last_char = remove_diacritics(word[i_char_max]);
    // chars contiendra les lettres non accentuées de word
    char chars[4] = {0};

    chars[0] = remove_diacritics(word[0]);
    // Si chars[0] est une lettre ou word[0] est un tiret
    if (chars[0] || word[0] == 45) {
        // Si chars[0] est une lettre, i représentera
        // la lettre correspondante entre 0 et 25, sinon
        // on met i à 26, qui représente l'indice des tirets
        i = chars[0] ? chars[0] - 97 : 26;
    } else {
        // Si chars[0] n'est pas une lettre
        // ni un tiret, le mot n'existe pas
        // dans le dictionnaire
        return false;
    }

    if (word_length >= 2) {
        chars[1] = remove_diacritics(word[1]);
        if (chars[1] || word[1] == 45) {
            j = chars[1] ? chars[1] - 97 : 26;
        } else {
            return false;
        }
    } else {
        // Si le mot fait moins de deux lettres, on le signale
        // en mettant i, j et k à 27 : dans le tableau positions,
        // l'indice 27 représente l'absence d'une lettre
        j = 27;
        k = 27;
        l = 27;
    }

    if (word_length >= 3) {
        chars[2] = remove_diacritics(word[2]);
        if (chars[2] || word[2] == 45) {
            k = chars[2] ? chars[2] - 97 : 26;
        } else {
            return false;
        }
    } else {
        k = 27;
        l = 27;
    }

    if (word_length == 4) {
        chars[3] = remove_diacritics(word[3]);
        if (chars[3] || word[3] == 45) {
            l = chars[3] ? chars[3] - 97 : 26;
        } else {
            return false;
        }
    } else {
        l = 27;
    }

    word[0] = to_lower(word[0]);

    // i_start représente l'indice dans le tableau dictionnaire à partir
    // duquel se trouvent les mots commençant par les lettres du mot
    // en cours de test (word)
    i_start = positions[i][j][k][l];
    if (i_start == -1 || strlen((char *) dict[i_start]) != word_length) return false;

    while (i_start < nb_words_dict && remove_diacritics(dict[i_start][i_char_max]) == last_char) {
        if (strcmp((char *) word, (char *) dict[i_start]) == 0) {
            return true;
        }
        ++i_start;
    }

    return false;
}

void c3(byte str_crypted[], uint32 str_length, byte **keys, uint32 nb_keys, byte **dict, uint32 nb_words_dict, long ****positions) {
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
            if (word_length <= 4) {
                if (is_in_dict(words[i_word], word_length, dict, nb_words_dict, positions)) ++cur_score;
            }
        }

        if (cur_score > max_score) {
            max_score = cur_score;
            i_best_key = i_key;
        }

        // Ici nb_words est le nombre d'éléments de words mais la fonction
        // extract_words a réalloué words par blocs de 5, il faut donc arrondir
        // nb_words à la demi-dizaine supérieure pour libérer toute la mémoire
        free_2d_array(&words, nb_words_str / 5 * 5 + 5);
    }

    printf("%s\n", keys[i_best_key]);

    free_array(&str_decrypted);
}
