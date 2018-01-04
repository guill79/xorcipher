#include "../inc/types.h"
#include "../inc/chars.h"

bool char_valid_message(byte c) {
    bool is_valid = 0;

    switch (c) {
        case 10: case 32: case 33: case 34: case 39: case 40: case 41: case 44:
        case 45: case 46: case 58: case 59: case 63: case 65: case 66: case 67:
        case 68: case 69: case 70: case 71: case 72: case 73: case 74: case 75:
        case 76: case 77: case 78: case 79: case 80: case 81: case 82: case 83:
        case 84: case 85: case 86: case 87: case 88: case 89: case 90: case 95:
        case 97: case 98: case 99: case 100: case 101: case 102: case 103:
        case 104: case 105: case 106: case 107: case 108: case 109: case 110:
        case 111: case 112: case 113: case 114: case 115: case 116: case 117:
        case 118: case 119: case 120: case 121: case 122: case 123: case 125:
        case 192: case 194: case 196: case 199: case 200: case 201: case 202:
        case 203: case 206: case 207: case 212: case 214: case 217: case 219:
        case 224: case 226: case 228: case 231: case 232: case 233: case 234:
        case 235: case 238: case 239: case 244: case 246: case 249: case 251:
            is_valid = true;
            break;
        default:
            is_valid = false;
            break;
    }

    return is_valid;
}

bool char_valid_key(byte c) {
    bool is_valid = 0;

    switch (c) {
        case 44: case 45: case 46: case 48: case 49: case 50: case 51: case 52:
        case 53: case 54: case 55: case 56: case 57: case 58: case 63: case 65:
        case 66: case 67: case 68: case 69: case 70: case 71: case 72: case 73:
        case 74: case 75: case 76: case 77: case 78: case 79: case 80: case 81:
        case 82: case 83: case 84: case 85: case 86: case 87: case 88: case 89:
        case 90: case 95: case 97: case 98: case 99: case 100: case 101:
        case 102: case 103: case 104: case 105: case 106: case 107: case 108:
        case 109: case 110: case 111: case 112: case 113: case 114: case 115:
        case 116: case 117: case 118: case 119: case 120: case 121: case 122:
        case 123: case 125:
            is_valid = true;
            break;
        default:
            is_valid = false;
            break;
    }

    return is_valid;
}

byte remove_diacritics(byte c) {
    byte non_diacritic_c = 0;

    switch (c) {
        case 65: case 66: case 67: case 68: case 69: case 70: case 71: case 72:
        case 73: case 74: case 75: case 76: case 77: case 78: case 79: case 80:
        case 81: case 82: case 83: case 84: case 85: case 86: case 87: case 88:
        case 89: case 90:
            non_diacritic_c = c + 32;
            break;
        case 97: case 98: case 99: case 100: case 101: case 102: case 103:
        case 104: case 105: case 106: case 107: case 108: case 109: case 110:
        case 111: case 112: case 113: case 114: case 115: case 116: case 117:
        case 118: case 119: case 120: case 121: case 122:
            non_diacritic_c = c;
            break;
        case 192: case 194: case 196: case 224: case 226: case 228:
            non_diacritic_c = 'a';
            break;
        case 199: case 231:
            non_diacritic_c = 'c';
            break;
        case 200: case 201: case 202: case 203: case 232: case 233:
        case 234: case 235:
            non_diacritic_c = 'e';
            break;
        case 206: case 207: case 238: case 239:
            non_diacritic_c = 'i';
            break;
        case 212: case 214: case 244: case 246:
            non_diacritic_c = 'o';
            break;
        case 217: case 218: case 219: case 220: case 249: case 250:
        case 251: case 252:
            non_diacritic_c = 'u';
            break;
        default:
            non_diacritic_c = 0;
            break;
    }

    return non_diacritic_c;
}

bool is_delimiter(byte c) {
    bool is_delim = 0;

    switch (c) {
        // On considère que 0 est un séparateur, qui indique la fin d'une
        // chaîne de caractères
        case 0: case 10: case 13: case 32: case 33: case 34: case 44: case 46:
        case 58: case 59: case 63:
            is_delim = true;
            break;
        default:
            is_delim = false;
            break;
    }

    return is_delim;
}

byte to_lower(byte c) {
    byte lowered_c = 0;

    switch (c) {
        case 65: case 66: case 67: case 68: case 69: case 70: case 71: case 72:
        case 73: case 74: case 75: case 76: case 77: case 78: case 79: case 80:
        case 81: case 82: case 83: case 84: case 85: case 86: case 87: case 88:
        case 89: case 90: case 192: case 194: case 196: case 199: case 200:
        case 201: case 202: case 203: case 206: case 207: case 212: case 214:
        case 217: case 218: case 219: case 220:
            lowered_c = c + 32;
            break;
        default:
            lowered_c = c;
            break;
    }

    return lowered_c;
}
