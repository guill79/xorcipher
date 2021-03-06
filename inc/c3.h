#ifndef C3_H
#define C3_H

#include "types.h"

#define NB_WORDS_DICT 336531
#define MAX_WORD_LENGTH 26
#define DICT_NAME "fr_dict.txt"

byte **load_dict(char *dict_name, uint16 nb_words, uint8 max_word_length,
                 long ****positions);
byte **extract_words(byte *str, uint32 str_length, uint32 *nb_words);
bool is_in_dict(byte *word, uint8 word_length, byte **dict,
                uint32 nb_words_dict, long ****positions);
byte *c3(byte str_crypted[], uint32 str_length, byte **keys, uint32 nb_keys,
         byte **dict, uint32 nb_words_dict, long ****positions);

#endif // C3_H
