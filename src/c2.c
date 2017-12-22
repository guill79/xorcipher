#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../inc/utils.h"
#include "../inc/types.h"
#include "../inc/xor.h"
#include "../inc/c2.h"

bool char_matches(byte char_num, byte c) {
    switch (char_num) {
        case 0: // 'a'
            if ((c == 65) || (c == 97) || (c == 192) || (c == 194)
                || (c == 196) || (c == 224) || (c == 226) ||(c == 228)) {
                return true;
            }
            break;
        case 1: // 'b'
            if ((c == 66) || (c == 98)) {
                return true;
            }
            break;
        case 2: // 'c'
            if ((c == 67) || (c == 99) || (c == 131) || (c == 199)) {
                return true;
            }
            break;
        case 3: // 'd'
            if ((c == 68) || (c == 100)) {
                return true;
            }
            break;
        case 4: // 'e'
            if ((c == 69) || (c == 101) || (c >= 200 && c <= 203)
                || (c >= 232 && c <= 235)) {
                return true;
            }
            break;
        case 5: // 'f'
            if ((c == 70) || (c == 102)) {
                return true;
            }
            break;
        case 6: // 'g'
            if ((c == 71) || (c == 103)) {
                return true;
            }
            break;
        case 7: // 'h'
            if ((c == 72) || (c == 104)) {
                return true;
            }
            break;
        case 8: // 'i'
            if ((c == 73) || (c == 105) || (c == 206) || (c == 207)
                || (c == 238) || (c == 239)) {
                return true;
            }
            break;
        case 9: // 'j'
            if ((c == 74) || (c == 106)) {
                return true;
            }
            break;
        case 10: // 'k'
            if ((c == 75) || (c == 107)) {
                return true;
            }
            break;
        case 11: // 'l'
            if ((c == 76) || (c == 108)) {
                return true;
            }
            break;
        case 12: // 'm'
            if ((c == 77) || (c == 109)) {
                return true;
            }
            break;
        case 13: // 'n'
            if ((c == 78) || (c == 110)) {
                return true;
            }
            break;
        case 14: // 'o'
            if ((c == 79) || (c == 111) || (c == 212) || (c == 214)
                || (c == 244) || (c == 246)) {
                return true;
            }
            break;
        case 15: // 'p'
            if ((c == 80) || (c == 112)) {
                return true;
            }
            break;
        case 16: // 'q'
            if ((c == 81) || (c == 113)) {
                return true;
            }
            break;
        case 17: // 'r'
            if ((c == 82) || (c == 114)) {
                return true;
            }
            break;
        case 18: // 's'
            if ((c == 83) || (c == 115)) {
                return true;
            }
            break;
        case 19: // 't'
            if ((c == 84) || (c == 116)) {
                return true;
            }
            break;
        case 20: // 'u'
            if ((c == 85) || (c == 117) || (c == 217) || (c == 219)
                || (c == 249) || (c == 251)) {
                return true;
            }
            break;
        case 21: // 'v'
            if ((c == 86) || (c == 118)) {
                return true;
            }
            break;
        case 22: // 'w'
            if ((c == 87) || (c == 119)) {
                return true;
            }
            break;
        case 23: // 'x'
            if ((c == 88) || (c == 120)) {
                return true;
            }
            break;
        case 24: // 'y'
            if ((c == 89) || (c == 121)) {
                return true;
            }
            break;
        case 25: // 'z'
            if ((c == 90) || (c == 122)) {
                return true;
            }
            break;
    }

    return false;
}

double frequency_char(byte str[], uint32 str_length, byte char_num) {
    double nb_char = 0.0;

    for (uint32 i = 0; i < str_length; ++i) {
        if (char_matches(char_num, str[i])) nb_char += 1;
    }

    return (nb_char / str_length) * 100;
}

double frequency_analysis(byte str[], uint32 str_length) {
    double score = 0.0;
    double th_freq[26] = {9.42, 1.02, 2.64, 3.39, 15.87, 0.95, 1.04, 0.77, \
                          8.41, 0.89, 0.0, 5.34, 3.24, 7.15, 5.14, 2.86, 1.06, \
                          6.46, 7.9, 7.26, 2.15, 0.0, 0.3, 0.24, 0.32};

    for (uint8 char_num = 0; char_num < 26; ++char_num) {
        score += pow(th_freq[char_num] - frequency_char(str, str_length, char_num), 2);
    }

    return score;
}

void c2(byte str_crypted[], uint32 str_length, byte **keys, uint32 nb_keys, uint8 key_length) {
    byte *str_decrypted = init_array(str_length + 1);
    byte *best_key = init_array(key_length + 1);
    double min_distance = 100000;
    double cur_distance = 0;

    for (int i = 0; i < nb_keys; ++i) {
        xor(str_crypted, str_decrypted, str_length, keys[i]);

        cur_distance = frequency_analysis(str_decrypted, str_length);
        if (cur_distance < min_distance) {
            min_distance = cur_distance;
            strcpy((char *) best_key, (char *) keys[i]);
        }
    }

    printf("%s\n", best_key);

    free_array(&best_key);
    free_array(&str_decrypted);
}
