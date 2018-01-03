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
    switch (c) {
        case 65: case 66: case 67: case 68: case 69: case 70: case 71:
        case 72: case 73: case 74: case 75: case 76: case 77: case 78:
        case 79: case 80: case 81: case 82: case 83: case 84: case 85:
        case 86: case 87: case 88: case 89: case 90:
            return c + 32;
            break;
        case 97: case 98: case 99: case 100: case 101: case 102: case 103:
        case 104: case 105: case 106: case 107: case 108: case 109: case 110:
        case 111: case 112: case 113: case 114: case 115: case 116: case 117:
        case 118: case 119: case 120: case 121: case 122:
            return c;
            break;
        case 192: case 194: case 196: case 224: case 226: case 228:
            return 'a';
            break;
        case 199: case 231:
            return 'c';
            break;
        case 200: case 201: case 202: case 203: case 232: case 233:
        case 234: case 235:
            return 'e';
            break;
        case 206: case 207: case 238: case 239:
            return 'i';
            break;
        case 212: case 214: case 244: case 246:
            return 'o';
            break;
        case 217: case 219: case 220: case 249: case 250: case 251:
        case 252:
            return 'u';
            break;
        default:
            return 0;
            break;
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
