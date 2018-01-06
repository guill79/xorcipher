#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/utils.h"
#include "../inc/types.h"
#include "../inc/chars.h"
#include "../inc/c1.h"

/**
 *  Fonction : test_char_on_str
 *  ----------------------------
 *  Détermine si le caractère c, à l'indice pos de la clé key, est correct.
 *  Autrement dit s'il est possible que ce caractère ait été utilisé, à cet
 *  indice de la clé, pour chiffrer la chaîne str.
 *
 *  Paramètres :
 *    - c          : caractère à tester
 *    - pos        : indice de c dans la clé
 *    - key_length : longueur de la clé
 *    - str        : chaîne sur laquelle tester c
 *    - str_length : longueur de str
 *
 *  Renvoie :
 *    - true s'il est possible que c ait été utilisé pour chiffrer la chaîne,
 *    - false sinon.
 */
bool test_char_on_str(byte c, uint8 pos, uint8 key_length, byte str[],
                      uint32 str_length) {
    // Pour chaque caractère de la chaîne
    for (uint32 i = 0; i < str_length; ++i) {
        // Si le caractère i est codé par le carac c en position pos
        if (i % key_length == pos) {
            if (!char_valid_message(c ^ str[i])) return false;
        }
    }
    return true;
}

/**
 *  Fonction : print_chars
 *  -----------------------
 *  Affiche les caractères de char sous la forme : [abc].
 *
 *  Paramètres :
 *    - chars    : tableau contenant les caractères à afficher
 *    - nb_chars : nombre d'éléments de chars
 */
void print_chars(byte chars[], uint8 nb_chars) {
    printf("[");
    for (uint8 i = 0; i < nb_chars; ++i) {
        printf("%c", chars[i]);
    }
    printf("]");
}

/**
 *  Fonction : extract_keys_loop
 *  -----------------------------
 *  Permet l'extraction des clés. Utilisée dans extract_keys.
 *
 *  Paramètres :
 *    - i                : indice courant du caractère extrait dans temp_key
 *    - i_key            : indice courant de la clé dans keys
 *    - keys             : tableau dans lequel les clés sont extraites
 *    - key_chars        : tableau contenant les caractères possibles pour
 *                         chaque caractère de la clé
 *    - key_chars_length : tableau contenant à l'indice i, le nombre d'éléments
 *                         de keys_chars[i]
 *    - temp_key         : tableau contenant les caractères de la clé en cours
 *                         d'extraction
 *    - key_length       : longueur de la clé
 */
void extract_keys_loop(uint8 i, uint16 *i_key, byte **keys, byte **key_chars,
                       byte key_chars_length[], byte temp_key[],
                       uint8 key_length) {
    // Si on atteint la fin du mot
    if (i == key_length) {
        temp_key[key_length] = '\0';
        strcpy((char *) keys[*i_key], (char *) temp_key);
        ++(*i_key);
        return;
    }

    for (uint8 j = 0; j < key_chars_length[i]; ++j) {
        temp_key[i] = key_chars[i][j];
        extract_keys_loop(i + 1, i_key, keys, key_chars, key_chars_length,
                          temp_key, key_length);
    }
}

/**
 *  Fonction : extract_keys
 *  ------------------------
 *  Extrait les clés à partir du tableau des caractères possibles.
 *  Par exemple si les caractères sont [ab][cde], les clés sont "ac", "ad",
 *  "ae", "bc", "bd" et "be".
 *
 *  Paramètres :
 *    - key_chars        : tableau contenant les caractères possibles pour
 *                         chaque caractère de la clé
 *    - key_chars_length : tableau contenant à l'indice i, le nombre d'éléments
 *                         de keys_chars[i]
 *    - key_length       : longueur de la clé
 *    - nb_keys          : adresse du uint32 dans lequel on veut stocker le
 *                         nombre de clés extraites
 *
 *  Renvoie : le tableau contenant les clés extraites.
 */
byte **extract_keys(byte **key_chars, uint8 key_chars_length[],
                    uint8 key_length, uint32 *nb_keys) {
    // On compte le nombre de clés
    *nb_keys = 1;
    for (uint8 i = 0; i < key_length; ++i) *nb_keys *= key_chars_length[i];

    byte **keys = init_2d_array(*nb_keys, key_length + 1);
    byte *temp_key = init_array(key_length + 1);

    uint16 i_key = 0;
    extract_keys_loop(0, &i_key, keys, key_chars, key_chars_length,
                      temp_key, key_length);

    free_array(&temp_key);

    return keys;
}

/**
 *  Fonction : c1
 *  --------------
 *  Détermine les caractères possibles de la clé permettant d'obtenir un
 *  message déchiffré valide.
 *
 *  Paramètres :
 *    - str_crypted      : chaîne chiffrée à analyser
 *    - str_length       : longueur de str_crypted
 *    - key_length       : longueur de la clé
 *    - key_chars_length : tableau qui contiendra le nombre de caractères
 *                         possibles pour chaque caractère de la clé
 *
 *  Renvoie : le tableau contenant les caractères possibles de la clé.
 */
byte **c1(byte str_crypted[], uint32 str_length, uint8 key_length,
          uint8 key_chars_length[]) {
    uint8 array_capacity = 10;
    byte **key_chars = init_2d_array(key_length, array_capacity);

    uint8 j = 0;
    // Pour chaque caractère potentiel de la clé
    for (uint8 i = 0; i < key_length; ++i) {
        j = 0;
        // On parcourt tous les caractères possibles d'une clé
        for (byte c = 44; c < 126; ++c) {
            if (char_valid_key(c)) {
                if (test_char_on_str(c, i, key_length, str_crypted, str_length)) {
                    // Le caractère c a passé tous les tests : on l'ajoute
                    // aux caractères possibles de la clé
                    key_chars[i][j] = c;
                    ++j;

                    // Si le tableau est trop petit, on augmente sa taille
                    if (j == array_capacity) {
                        expand_array(key_chars + i, MAX_NB_VALID_KEY_CHARS);
                    }
                }
            }
        }
        // si j == 0 ça veut dire que pour le ième caractère de la clé, aucun
        // caractère n'est possible : inutile de continuer
        if (j == 0) {
            // On met le premier élément à 0 pour pouvoir savoir plus tard que
            // ce tableau n'est pas complet et qu'il ne faut pas l'afficher
            key_chars[i][0] = 0;
            return key_chars;
        }
        key_chars_length[i] = j;
    }

    return key_chars;
}
