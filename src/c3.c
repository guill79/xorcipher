#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../inc/utils.h"
#include "../inc/types.h"
#include "../inc/chars.h"
#include "../inc/xor.h"
#include "../inc/c2.h"
#include "../inc/c3.h"

/**
 *  Fonction : load_dict
 *  ---------------------
 *  Charge le fichier dictionnaire dans un tableau.
 *  Attention : le fichier dictionnaire doit être encodé en Latin-1, ne doit
 *  contenir que des lettres en minuscule ou des tirets et doit être trié dans
 *  l'ordre alphabétique.
 *
 *  Paramètres :
 *    - dict_name       : nom du fichier contenant le dictionnaire
 *    - nb_words        : nombre de mots du dictionnaire
 *    - max_word_length : longueur maximale d'un mot
 *    - positions       : tableau à 4 dimensions de taille [27][28][28][28],
 *                        initialisé à -1
 *
 *  Renvoie : le tableau contenant les mots du dictionnaire.
 */
byte **load_dict(char *dict_name, uint16 nb_words, uint8 max_word_length,
                 long ****positions) {
    assert(dict_name != NULL && nb_words >= 0);

    byte **dict = NULL;
    FILE *f = NULL;

    // Ouverture du fichier dictionnaire
    f = fopen(dict_name, "r");
    CHECK_FILE(f, dict_name);

    dict = calloc(nb_words, sizeof(byte *));
    CHECK_PTR(dict);

    // On va stocker les lettres actuelles pour le remplissage du tableau
    // positions
    char cur_letters4[4] = {-1};
    char cur_letters3[3] = {-1};
    char cur_letters2[2] = {-1};
    char cur_letters1[1] = {-1};
    uint8 i = 0, j = 0, k = 0, l = 0;

    // Pour chaque mot du dictionnaire
    for (uint16 i_word = 0; i_word < nb_words; ++i_word) {
        dict[i_word] = calloc(max_word_length, sizeof(byte));
        CHECK_PTR(dict[i_word]);

        if (fscanf(f, "%s", dict[i_word]) == 0) {
            fprintf(stderr, "Erreur lors du chargement du dictionnaire.\n");
            exit(EXIT_FAILURE);
        } else {
            /*
             *  Remplissage du tableau 4D positions
             *  Ce tableau contient, à l'indice [i][j][k][l], l'indice du
             *  premier mot dans dict commençant par i+97, j+97, k+97 et l+97.
             *  Si i, j, k ou l vaut 26, c'est un tiret. Si j, k ou l vaut 27,
             *  cela signifie que positions[i][j][k][l] contient l'indice du
             *  premier mot commençant par :
             *      i+97 -------------> si j = k = l = 27
             *      i+97, j+97 -------> si k = l = 27
             *      i+97, j+97, k+97 -> si l = 27
             */

            // Si le caractère est un tiret, on ne peut pas appeler
            // remove_diacritics qui renverrait 0. On met donc i, j, k ou l
            // à 123 pour le placer aux indices 26 de positions
            // (les indices 26 correspondent aux tirets).
            if (dict[i_word][0] == 45) i = 123;
            else i = remove_diacritics(dict[i_word][0]);

            if (dict[i_word][1] == 45) j = 123;
            else j = remove_diacritics(dict[i_word][1]);

            if (dict[i_word][2] == 45) k = 123;
            else k = remove_diacritics(dict[i_word][2]);

            if (dict[i_word][3] == 45) l = 123;
            else l = remove_diacritics(dict[i_word][3]);

            // Indice des mots commençant par i, j, k, l
            if (l && k && j && i && (l != cur_letters4[3]
                                     || k != cur_letters4[2]
                                     || j != cur_letters4[1]
                                     || i != cur_letters4[0])) {
                cur_letters4[0] = i;
                cur_letters4[1] = j;
                cur_letters4[2] = k;
                cur_letters4[3] = l;
                positions[i-97][j-97][k-97][l-97] = i_word;
            }
            // Indice des mots commençant par i, j, k
            if (k && j && i && (k != cur_letters3[2]
                                || j != cur_letters3[1]
                                || i != cur_letters3[0])) {
                cur_letters3[0] = i;
                cur_letters3[1] = j;
                cur_letters3[2] = k;
                positions[i-97][j-97][k-97][27] = i_word;
            }
            // Indice des mots commençant par i, j
            if (j && i && (j != cur_letters2[1] || i != cur_letters2[0])) {
                cur_letters2[0] = i;
                cur_letters2[1] = j;
                positions[i-97][j-97][27][27] = i_word;
            }
            // Indice des mots commençant par i
            if (i && (i != cur_letters1[0])) {
                cur_letters1[0] = i;
                positions[i-97][27][27][27] = i_word;
            }
        }
    }

    fclose(f);

    return dict;
}

/**
 *  Fonction : extract_words
 *  -------------------------
 *  Extrait les mots de la chaîne str.
 *
 *  Paramètres :
 *    - str        : chaîne dont on veut extraire les mots
 *    - str_length : longueur de str
 *    - nb_words   : adresse du uint32 dans lequel on veut stocker le nombre
 *                   de mots de str
 *
 *  Renvoie : le tableau contenant les mots de la chaîne str.
 */
byte **extract_words(byte *str, uint32 str_length, uint32 *nb_words) {
    assert(str != NULL && str_length >= 0);

    uint32 cur_array_capacity = 5;
    byte **words = NULL;
    uint8 start = 0,
          nb_letters = 0;

    words = init_2d_array(cur_array_capacity, 1);

    *nb_words = 0;
    // Pour chaque caractère de str
    for (uint32 i = 0; i <= str_length; ++i) {
        assert(cur_array_capacity == *nb_words / 5 * 5 + 5);

        if (is_delimiter(str[i])) {
            expand_array(words + *nb_words, nb_letters + 1);
            memcpy(words[*nb_words], str + start, nb_letters);
            words[*nb_words][nb_letters] = '\0';

            ++(*nb_words);
            // Si tableau trop petit
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
            while (i < str_length && is_delimiter(str[i]));

            start = i;
            nb_letters = 0;
        }
        ++nb_letters;
    }

    return words;
}

/**
 *  Fonction : is_in_dict
 *  ----------------------
 *  Détermine si le mot word est dans dict.
 *  Attention : ne fonctionne que pour des mots de longueur inférieure ou égale
 *  à 4 (il est facilement possible de gérer des mots plus longs en faisant
 *  quelques modifications mais le sujet demande de ne traiter que les mots de
 *  4 lettres ou moins). Le mot doit aussi ne pas être une constante chaîne de
 *  caractères car on va devoir mettre sa première lettre en minuscule. On
 *  aurait pu le copier et modifier la copie mais le traitement aurait été plus
 *  long.
 *
 *  Paramètres :
 *    - word          : mot à rechercher (de longueur <= 4 et ne doit pas être
 *                      une constante chaîne de caractères)
 *    - word_length   : longueur de word
 *    - dict          : tableau contenant les mots du dictionnaire
 *    - nb_words_dict : nombre de mots dans dict
 *    - positions     : tableau 4D contenant les indices de dict à partir
 *                      desquels se trouvent des mots en fonction des indices
 *                      i, j, k et l (voir load_dict pour plus de détails)
 *
 *  Renvoie :
 *    - true si word est dans dict,
 *    - false sinon.
 */
bool is_in_dict(byte *word, uint8 word_length, byte **dict,
                uint32 nb_words_dict, long ****positions) {
    assert(word != NULL && word_length >= 0 && nb_words_dict >= 0);

    uint8 i = 0, j = 0, k = 0, l = 0;
    long i_start = 0;
    uint8 i_char_max = word_length - 1;
    byte last_char = remove_diacritics(word[i_char_max]);
    char chars[4] = {0};  // Contiendra les lettres non accentuées de word

    chars[0] = remove_diacritics(word[0]);
    // Si chars[0] est une lettre ou word[0] est un tiret
    if (chars[0] || word[0] == 45) {
        // Si chars[0] est une lettre, i représentera
        // la lettre correspondante entre 0 et 25, sinon
        // on met i à 26, qui représente l'indice des tirets.
        i = chars[0] ? chars[0] - 97 : 26;
    } else {
        // Si chars[0] n'est pas une lettre
        // ni un tiret, le mot n'existe pas
        // dans le dictionnaire.
        return false;
    }

    if (word_length >= 2) {
        // Même chose qu'au dessus
        chars[1] = remove_diacritics(word[1]);
        if (chars[1] || word[1] == 45) {
            j = chars[1] ? chars[1] - 97 : 26;
        } else {
            return false;
        }
    } else {
        // Si le mot fait moins de deux lettres, on le signale
        // en mettant i, j et k à 27 : dans le tableau positions,
        // l'indice 27 représente l'absence d'une lettre.
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

    // On met la première lettre en minuscule (c'est pour ça que word ne doit
    // pas être une constante chaîne de caractères)
    word[0] = to_lower(word[0]);

    // i_start représente l'indice dans le tableau dict à partir
    // duquel se trouvent les mots commençant par les lettres du mot
    // en cours de test (word)
    i_start = positions[i][j][k][l];
    if (i_start == -1 || strlen((char *) dict[i_start]) != word_length) {
        // Si i_start vaut -1, cela signifie qu'il n'existe pas de mot
        // commençant par ces lettres. Aussi, si la longueur n'est pas la même,
        // c'est que le mot n'est pas dans le dictionnaire (c'est une des choses
        // à enlever si on veut gérer des mots de plus de 4 lettres).
        return false;
    }

    // On va tester les mots tant qu'il en reste et que la lettre à l'indice
    // word_length - 1 de word (sans accents) est égale à la lettre au même
    // indice de dict[i_start] (sans accents). Cette condition permet de réduire
    // le nombre de mots à comparer.
    while (i_start < nb_words_dict
           && remove_diacritics(dict[i_start][i_char_max]) == last_char) {
        if (strcmp((char *) word, (char *) dict[i_start]) == 0) {
            return true;
        }
        ++i_start;
    }

    return false;
}

/**
 *  Fonction : c3
 *  --------------
 *  Affiche la clé la plus probable en comptant le nombre de mots présents
 *  dans le dictionnaire pour chaque chaîne déchiffrée à partir des clés
 *  possibles.
 *
 *  Paramètres :
 *    - str_crypted   : chaîne chiffrée dont on veut déterminer la clé
 *    - str_length    : longueur de str_crypted
 *    - keys          : tableau contenant les clés possibles
 *    - nb_keys       : nombre de clés dans keys
 *    - dict          : tableau contenant les mots du dictionnaire
 *    - nb_words_dict : nombre de mots dans dict
 *    - positions     : tableau 4D contenant les indices de dict à partir
 *                      desquels se trouvent des mots en fonction des indices
 *                      i, j, k et l (voir load_dict pour plus de détails)
 *
 *  Renvoie : la clé la plus probable après comptage du nombre de mots présents
 *            dans le dictionnaire.
 */
byte *c3(byte str_crypted[], uint32 str_length, byte **keys, uint32 nb_keys,
         byte **dict, uint32 nb_words_dict, long ****positions) {
    assert(str_crypted != NULL && str_length >= 0 && nb_keys >= 0
           && nb_words_dict >= 0);

    uint32 i_best_key = 0;
    uint32 max_score = 0;
    uint32 cur_score = 0;
    byte *str_decrypted = NULL;
    byte **words = NULL;
    uint32 nb_words_str = 0;
    uint8 word_length = 0;

    str_decrypted = init_array(str_length + 1);
    str_decrypted[str_length] = '\0';

    // On parcourt les clés possibles
    for (uint32 i_key = 0; i_key < nb_keys; ++i_key) {
        cur_score = 0;
        // On déchiffre le message
        xor(str_crypted, str_decrypted, str_length, keys[i_key]);
        // On extrait les mots
        words = extract_words(str_decrypted, str_length, &nb_words_str);

        // On parcourt les mots de la chaîne déchiffrée
        for (uint32 i_word = 0; i_word < nb_words_str; ++i_word) {
            word_length = strlen((char *) words[i_word]);
            if (word_length <= 4) {
                if (is_in_dict(words[i_word], word_length, dict, nb_words_dict,
                               positions)) {
                    ++cur_score;
                }
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

    free_array(&str_decrypted);

    return keys[i_best_key];
}
