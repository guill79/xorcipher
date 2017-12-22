#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/utils.h"
#include "../inc/types.h"
#include "../inc/c1.h"

bool char_valid_message(byte c) {
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)
        || (c == 44) || (c == 45) || (c == 46) || (c == 58)
        || (c == 63) || (c == 95) || (c == 123) || (c == 125)
        || (c == 32) || (c == 192) || (c == 194) || (c == 196)
        || (c == 224) || (c == 226) || (c == 228) || (c == 199)
        || (c == 131) || (c == 200) || (c == 201) || (c == 202)
        || (c == 203) || (c >= 232 && c <= 235) || (c == 206)
        || (c == 207) || (c == 238) || (c == 239) || (c == 217)
        || (c == 219) || (c == 249) || (c == 251) || (c == 212)
        || (c == 214) || (c == 244) || (c == 246) || (c == 255)
        || (c == 10) || (c == 33) || (c == 34) || (c == 39)
        || (c == 40) || (c == 41) || (c == 59)) {
        return true;
    }
    return false;
}

bool char_valid_key(byte c) {
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)
        || (c == 44) || (c == 45) || (c == 46) || (c == 58)
        || (c == 63) || (c == 95) || (c == 123) || (c == 125)
        || (c >= 48 && c <= 57)) {
        return true;
    }
    return false;
}

bool test_char_on_str(byte c, uint8 pos, uint8 key_length, byte str[],
                      uint32 str_length) {
    // On parcourt le message
    for (uint32 i = 0; i < str_length; ++i) {
        // Si le caractère i est codé par le carac c en position pos
        if (i % key_length == pos) {
            // S'il est invalide, on sort
            if (!char_valid_message(c ^ str[i])) return false;
        }
    }
    return true;
}

void print_chars(byte chars[], uint8 nb_chars) {
    printf("[");
    for (uint8 i = 0; i < nb_chars; ++i) {
        printf("%c", chars[i]);
    }
    printf("] ");
}

void extract_keys_loop(uint8 i, uint16 *i_key, byte **keys, byte **key_chars,
                       byte key_chars_length[], byte temp_key[], uint8 key_length) {
    if (i == key_length) {
        temp_key[key_length] = '\0';
        strcpy((char *) keys[*i_key], (char *) temp_key);
        ++(*i_key);
        return;
    }
    for (uint8 j = 0; j < key_chars_length[i]; ++j) {
        temp_key[i] = key_chars[i][j];
        extract_keys_loop(i + 1, i_key, keys, key_chars, key_chars_length, temp_key, key_length);
    }
}

byte **extract_keys(byte **key_chars, uint32 *nb_keys, uint8 key_chars_length[],
                    uint8 key_length) {
    *nb_keys = 1;
    for (uint8 i = 0; i < key_length; ++i) *nb_keys *= key_chars_length[i];

    byte **keys = init_2d_array(*nb_keys, key_length + 1);
    byte *temp_key = init_array(key_length + 1);

    uint16 i_key = 0;
    extract_keys_loop(0, &i_key, keys, key_chars, key_chars_length, temp_key, key_length);

    free_array(&temp_key);

    return keys;
}

byte **c1(byte str[], uint32 str_length, uint8 key_length,
          uint8 key_chars_length[]) {
    byte **key_chars = init_2d_array(key_length, 1);

    uint8 j = 0;
    // Pour chaque caractère potentiel de la clé
    for (uint8 i = 0; i < key_length; ++i) {
        j = 0;
        // On parcourt tous les caractères possibles d'une clé
        for (byte c = 44; c < 126; ++c) {
            if (char_valid_key(c)) {
                if (test_char_on_str(c, i, key_length, str, str_length)) {
                    // Le caractère c a passé tous les tests : on l'ajoute
                    // aux caractères possibles de la clé
                    key_chars[i][j] = c;
                    ++j;
                    expand_array(key_chars + i, j + 1);
                }
            }
        }
        key_chars_length[i] = j;
    }

    return key_chars;
}
