#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/utils.h"
#include "../inc/types.h"
#include "../inc/chars.h"
#include "../inc/xor.h"
#include "../inc/c2.h"

static double fr_th_freq[ALPHABET_LENGTH] = {9.42, 1.02, 2.64, 3.39, 15.87,
                                             0.95, 1.04, 0.77, 8.41, 0.89, 0.0,
                                             5.34, 3.24, 7.15, 5.14, 2.86, 1.06,
                                             6.46, 7.9, 7.26, 6.24, 2.15, 0.0,
                                             0.3, 0.24, 0.32};

/**
 *  Fonction : frequency_analysis
 *  ------------------------------
 *  Effectue l'analyse fréquentielle des lettres de str et calcule la distance
 *  entre ces fréquences et les fréquences théoriques de la langue française.
 *
 *  Paramètres :
 *    - str        : chaîne à analyser
 *    - str_length : longueur de str
 *
 *  Renvoie : la distance entre les fréquences de str et celles de la langue
 *            française.
 */
double frequency_analysis(byte str[], uint32 str_length) {
    double distance = 0.0;
    double temp = 0.0;
    uint32 nb_occurs[ALPHABET_LENGTH] = {0};
    uint32 nb_chars = 0;
    byte non_diacritic_char = 0;

    // On compte le nombre d'occurences de chaque lettre de str
    for (uint32 i = 0; i < str_length; ++i) {
        non_diacritic_char = remove_diacritics(str[i]);

        if (non_diacritic_char) {
            ++nb_occurs[non_diacritic_char - 97];
            ++nb_chars;
        }
    }

    // On calcule la distance avec les fréquences théoriques
    for (byte i = 0; i < ALPHABET_LENGTH; ++i) {
        temp = fr_th_freq[i] - ((double) nb_occurs[i] / nb_chars) * 100;
        distance += temp * temp;
    }

    return distance;
}

/**
 *  Fonction : c2
 *  --------------
 *  Affiche la clé qui, appliquée sur str_crypted, permet d'obtenir une chaîne
 *  déchiffrée dont les fréquences d'apparition des lettres sont celles les plus
 *  proches de la langue française.
 *
 *  Paramètres :
 *    - str_crypted : chaîne chiffrée dont on veut déterminer la clé
 *    - str_length  : longueur de str_crypted
 *    - keys        : tableau contenant les clés possibles
 *    - nb_keys     : nombre de clés dans keys
 *    - key_length  : longueur des clés dans keys
 *
 *  Renvoie : la clé la plus probable en fonction de l'analyse fréquentielle.
 */
byte *c2(byte str_crypted[], uint32 str_length, byte **keys, uint32 nb_keys,
        uint8 key_length) {
    uint32 i_best_key = 0;
    double min_distance = 100000.0;
    double cur_distance = 0.0;
    byte *str_decrypted = init_array(str_length + 1);
    str_decrypted[str_length] = '\0';

    // Pour chaque clé
    for (uint32 i = 0; i < nb_keys; ++i) {
        // On déchiffre le message
        xor(str_crypted, str_decrypted, str_length, keys[i]);

        // On fait l'analyse fréquentielle de ce message déchiffré
        cur_distance = frequency_analysis(str_decrypted, str_length);

        if (cur_distance < min_distance) {
            min_distance = cur_distance;
            i_best_key = i;
        }
    }

    free_array(&str_decrypted);

    return keys[i_best_key];
}
