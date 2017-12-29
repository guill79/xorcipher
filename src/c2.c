#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/utils.h"
#include "../inc/types.h"
#include "../inc/xor.h"
#include "../inc/c2.h"

static double th_freq[ALPHABET_LENGTH] = {9.42, 1.02, 2.64, 3.39, 15.87, 0.95,
                                          1.04, 0.77, 8.41, 0.89, 0.0, 5.34, 3.24,
                                          7.15, 5.14, 2.86, 1.06, 6.46, 7.9, 7.26,
                                          6.24, 2.15, 0.0, 0.3, 0.24, 0.32};

byte remove_diacritics(byte c) {
    if (c >= 65 && c <= 90) {
        return c + 32;
    } else if (c >= 97 && c <= 122) {
        return c;
    } else if ((c == 192) || (c == 194) || (c == 196) || (c == 224)
                || (c == 226) || (c == 228)) {
        return 'a';
    } else if ((c == 199) || (c == 231)) {
        return 'c';
    } else if ((c >= 200 && c <= 203) || (c >= 232 && c <= 235)) {
        return 'e';
    } else if ((c == 206) || (c == 207) || (c == 238) || (c == 239)) {
        return 'i';
    } else if ((c == 212) || (c == 214) || (c == 244) || (c == 246)) {
        return 'o';
    } else if ((c == 217) || (c == 219) || (c == 249) || (c == 251)) {
        return 'u';
    } else {
        return 0;
    }
}

double frequency_analysis(byte str[], uint32 str_length) {
    double distance = 0.0;
    double temp = 0.0;
    uint32 nb_occurs[ALPHABET_LENGTH] = {0};
    uint32 nb_chars = 0;
    byte non_diacritic_char = 0;

    for (uint32 i = 0; i < str_length; ++i) {
        non_diacritic_char = remove_diacritics(str[i]);

        if (non_diacritic_char) {
            ++nb_occurs[non_diacritic_char - 97];
            ++nb_chars;
        }
    }

    for (byte i = 0; i < ALPHABET_LENGTH; ++i) {
        temp = th_freq[i] - ((double) nb_occurs[i] / nb_chars) * 100;
        distance += temp * temp;
    }

    return distance;
}

void c2(byte str_crypted[], uint32 str_length, byte **keys, uint32 nb_keys, uint8 key_length) {
    uint32 i_best_key = 0;
    double min_distance = 100000.0;
    double cur_distance = 0.0;
    byte *str_decrypted = init_array(str_length + 1);
    str_decrypted[str_length] = '\0';

    for (uint32 i = 0; i < nb_keys; ++i) {
        xor(str_crypted, str_decrypted, str_length, keys[i]);

        cur_distance = frequency_analysis(str_decrypted, str_length);
        if (cur_distance < min_distance) {
            min_distance = cur_distance;
            i_best_key = i;
        }
    }

    printf("%s\n", keys[i_best_key]);

    free_array(&str_decrypted);
}
