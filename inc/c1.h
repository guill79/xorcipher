#ifndef C1_H
#define C1_H

#include "types.h"

// Nombre max de caractères possibles pour un caractère d'une clé
#define MAX_NB_VALID_KEY_CHARS 70

bool test_char_on_str(byte c, uint8 pos, uint8 key_length, byte str[],
                      uint32 str_length);
void print_chars(byte chars[], uint8 nb_chars);
void extract_keys_loop(uint8 i, uint16 *i_key, byte **keys, byte **key_chars,
                       byte key_chars_length[], byte temp_key[],
                       uint8 key_length);
byte **extract_keys(byte **key_chars, uint8 key_chars_length[],
                    uint8 key_length, uint32 *nb_keys);
byte **c1(byte str_crypted[], uint32 str_length, uint8 key_length,
          uint8 key_chars_length[]);

#endif // C1_H
