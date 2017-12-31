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
    for (uint32 i = 0; i < nb_words; ++i) {
        dict[i] = calloc(max_word_length, sizeof(byte));
        CHECK_PTR(dict[i]);
    }

    int cur_letters4[4] = {-1};
    int cur_letters3[3] = {-1};
    int cur_letters2[2] = {-1};
    int cur_letters1[1] = {-1};

    int word_length = 0;

    uint8 i = 0, j = 0, k = 0, l = 0;
    for (uint16 i_word = 0; i_word < nb_words; ++i_word) {
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
    if ((c == 13) || (c == 32) || (c == 33) || (c == 34) || (c == 39)
        || (c == 44) || (c == 46) || (c == 58)
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

byte to_lower(byte c) {
    if (c >= 65 && c <= 90) {
        return c + 32;
    }
    return c;
}

bool is_in_dict(byte *word, uint8 word_length, byte **dict, uint32 nb_words_dict, long ****positions) {
    char chars[4] = {0};
    uint8 i = 0, j = 0, k = 0, l = 0;

    // int word_length = strlen((char *) word);
    // printf("%d\n", str_length);

    if (word_length >= 4) {
        chars[0] = remove_diacritics(word[0]);
        chars[1] = remove_diacritics(word[1]);
        chars[2] = remove_diacritics(word[2]);
        chars[3] = remove_diacritics(word[3]);

        // On vérifie que ce sont bien des lettres ou des tirets
        // (sinon le mot n'est pas dans le dictionnaire)
        if (chars[0] && chars[1] && chars[2] && chars[3]) {
            i = chars[0] - 97;
            j = chars[1] - 97;
            k = chars[2] - 97;
            l = chars[3] - 97;
            word[0] = to_lower(word[0]);
        } else if ((word[0] == 45 || word[1] == 45
                           || word[2] == 45 || word[3] == 45) && (!chars[0] && !chars[1] && !chars[2] && !chars[3])) {
            if (word[0] == 45) i = 26;
            else i = chars[0] - 97;
            if (word[1] == 45) j = 26;
            else j = chars[1] - 97;
            if (word[2] == 45) k = 26;
            else k = chars[2] - 97;
            if (word[3] == 45) l = 26;
            else l = chars[3] - 97;        
        } else {
            return false;
        }

        // if ((word[1] >= 65 && word[1] <= 90)
        //     || (word[2] >= 65 && word[2] <= 90)
        //     || (word[3] >= 65 && word[3] <= 90)) 
        //     return false;

        // Si un mot commençant par ces quatres lettres n'existe pas
        // printf("i : %d, j : %d, k : %d, l : %d\n", i,j,k,l);
        // printf("%s\n", word);
        long i_start = positions[i][j][k][l];
        // printf("yo\n");
        if (positions[i][j][k][l] == -1) return false;

        // printf("Mot : %s     position : %ld\n", word, positions[i][j][k][l]);
        if (strlen((char *) dict[i_start]) != word_length) return false;

        while (i_start < nb_words_dict && dict[i_start][3] == word[3]) {
            if (strcmp((char *) word, (char *) dict[i_start]) == 0) {
                // printf("A comparer : %s et %s : %d\n", word, dict[i_start], strcmp((char *) word, (char *) dict[i_start]) == 0);
                return true;
            }
            ++i_start;
        }
    } else if (word_length == 3) {
        chars[0] = remove_diacritics(word[0]);
        chars[1] = remove_diacritics(word[1]);
        chars[2] = remove_diacritics(word[2]);

        // On vérifie que ce sont bien des lettres ou des tirets
        // (sinon le mot n'est pas dans le dictionnaire)
        if (chars[0] && chars[1] && chars[2]) {
            i = chars[0] - 97;
            j = chars[1] - 97;
            k = chars[2] - 97;
            l = 27;
            // On retire au passage la majuscule de la première lettre
            word[0] = to_lower(word[0]);
            // word[3] = chars[3];
        } else if ((word[0] == 45 || word[1] == 45
                           || word[2] == 45) && (!chars[0] && !chars[1] && !chars[2])) {
            if (word[0] == 45) i = 26;
            else i = chars[0] - 97;
            if (word[1] == 45) j = 26;
            else j = chars[1] - 97;
            if (word[2] == 45) k = 26;
            else k = chars[2] - 97;
            l = 27;
        } else {
            return false;
        }

        // if ((word[1] >= 65 && word[1] <= 90)
        //     || (word[2] >= 65 && word[2] <= 90)
        //     || (word[3] >= 65 && word[3] <= 90)) 
        //     return false;

        long i_start = positions[i][j][k][l];
        // Si un mot commençant par ces quatres lettres n'existe pas
        if (positions[i][j][k][l] == -1) return false;

        // printf("Mot : %s     position : %ld\n", word, positions[i][j][k][l]);

        // printf("Start : %ld\n", i_start);
        if (strlen((char *)dict[i_start]) != word_length) return false;
        while (i_start < nb_words_dict && dict[i_start][2] == word[2]) {
            if (strcmp((char *) word, (char *) dict[i_start]) == 0) {
                // printf("A comparer : %s et %s : %d\n", word, dict[i_start], strcmp((char *) word, (char *) dict[i_start]) == 0);
                return true;
            }
            ++i_start;
        }
    } else if (word_length == 2) {
        chars[0] = remove_diacritics(word[0]);
        chars[1] = remove_diacritics(word[1]);

        // On vérifie que ce sont bien des lettres ou des tirets
        // (sinon le mot n'est pas dans le dictionnaire)
        if (chars[0] && chars[1]) {
            i = chars[0] - 97;
            j = chars[1] - 97;
            k = 27;
            l = 27;
            // On retire au passage la majuscule de la première lettre
            word[0] = to_lower(word[0]);
            // word[3] = chars[3];
        } else if ((word[0] == 45 || word[1] == 45)  && (!chars[0] && !chars[1])) {
            if (word[0] == 45) i = 26;
            else i = chars[0] - 97;
            if (word[1] == 45) j = 26;
            else j = chars[1] - 97;
            k = 27;
            l = 27;
        } else {
            return false;
        }

        // if ((word[1] >= 65 && word[1] <= 90)
        //     || (word[2] >= 65 && word[2] <= 90)
        //     || (word[3] >= 65 && word[3] <= 90)) 
        //     return false;

        long i_start = positions[i][j][k][l];
        // Si un mot commençant par ces quatres lettres n'existe pas
        if (positions[i][j][k][l] == -1) return false;

        // printf("Mot : %s     position : %ld\n", word, positions[i][j][k][l]);

        // printf("Start : %ld\n", i_start);
        // printf("%ld\n", i_start);
        if (strlen((char *)dict[i_start]) != word_length) return false;
        while (i_start < nb_words_dict && dict[i_start][1] == word[1]) {
            // printf("yo\n");
            if (strcmp((char *) word, (char *) dict[i_start]) == 0) {
                // printf("A comparer : %s et %s : %d\n", word, dict[i_start], strcmp((char *) word, (char *) dict[i_start]) == 0);
                return true;
            }
            ++i_start;
        }    
    } else if (word_length == 1) {
        chars[0] = remove_diacritics(word[0]);

        // On vérifie que ce sont bien des lettres ou des tirets
        // (sinon le mot n'est pas dans le dictionnaire)
        if (chars[0]) {
            i = chars[0] - 97;
            j = 27;
            k = 27;
            l = 27;
            // On retire au passage la majuscule de la première lettre
            word[0] = to_lower(word[0]);
            // word[3] = chars[3];
        } else if (word[0] == 45 && !chars[0]) {
            if (word[0] == 45) i = 26;
            else i = chars[0] - 97;
            j = 27;
            k = 27;
            l = 27;
        } else {
            return false;
        }

        // if ((word[1] >= 65 && word[1] <= 90)
        //     || (word[2] >= 65 && word[2] <= 90)
        //     || (word[3] >= 65 && word[3] <= 90)) 
        //     return false;

        long i_start = positions[i][j][k][l];
        // Si un mot commençant par ces quatres lettres n'existe pas
        if (positions[i][j][k][l] == -1) return false;

        // printf("Mot : %s     position : %ld\n", word, positions[i][j][k][l]);

        // printf("Start : %ld\n", i_start);
        if (strlen((char *) dict[i_start]) != word_length) return false;
        while (i_start < nb_words_dict && dict[i_start][0] == word[0]) {
            if (strcmp((char *) word, (char *) dict[i_start]) == 0) {
                // printf("A comparer : %s et %s : %d\n", word, dict[i_start], strcmp((char *) word, (char *) dict[i_start]) == 0);
                return true;
            }
            ++i_start;
        }    
    }
    
    // word[0] = to_lower(word[0]);

    // for (uint32 m = 0; m < nb_words_dict; ++m) {
    //     if (strcmp((char *) word, (char *) dict[m]) == 0) {
    //         return true;
    //     }
    // }

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


        // printf(">>> Chaine à tester : %s\n", str_decrypted);
        for (uint32 i_word = 0; i_word < nb_words_str; ++i_word) {
            word_length = strlen((char *) words[i_word]);
            if (word_length <= 4) {
                if (is_in_dict(words[i_word], word_length, dict, nb_words_dict, positions)) ++cur_score;
                // if (strcmp((char*) keys[i_key], "Gff.Lf")==0) {
                //     printf("%s : %d\n", words[i_word], is_in_dict(words[i_word], word_length, dict, nb_words_dict, positions));
                // }
            }
        }

        if (cur_score > max_score) {
            max_score = cur_score;
            i_best_key = i_key;
        }

        // if (strcmp((char*) keys[i_key], "adijd")==0)
            // printf("--Mots--\n");
            // for (int i = 0; i < nb_words_str; ++i) {
            //     printf("%s\n", words[i]);
            // }            
        // printf("score : %lu\n", cur_score);
        // }

        // Ici nb_words est le nombre d'éléments de words mais la fonction
        // extract_words a réalloué words par blocs de 10, il faut donc arrondir
        // nb_words à la dizaine supérieure pour libérer toute la mémoire
        // free_2d_array(&words, nb_words_str / 10 * 10 + 10);
        // free_2d_array(&words, nb_words_str);
        // printf("reussi\n");
    }

    printf("%s\n", keys[i_best_key]);

    free_array(&str_decrypted);
}
