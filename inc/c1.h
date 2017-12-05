#ifndef C1_H
#define C1_H

#include <stdbool.h>
#include "types.h"

bool char_valid_message(byte c);
bool char_valid_key(byte c);
bool test_char_on_str(byte c, uint8 pos, uint8 key_length, byte str[],
                      uint32 str_length);
void print_chars(byte chars[], uint8 nb_chars);
void extract_keys_loop(uint8 i, byte **key_chars, byte key_chars_length[],
                       byte *temp_key, uint8 key_length);
byte **extract_keys(byte **key_chars, uint8 key_chars_length[],
                    uint8 key_length);
byte **c1(byte str[], uint32 str_length, uint8 key_length,
          uint8 key_chars_length[], uint32 *nb_keys);

#endif // C1_H
