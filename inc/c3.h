#ifndef C3_H
#define C3_H

#include "types.h"

#define NB_WORDS_DICT 336531
#define MAX_WORD_LENGTH 30
#define DICT_NAME "fr_dict.txt"

byte **load_dict(char *dict_name, uint16 nb_words, uint8 max_word_length);
bool is_delimiter(byte c);
byte **extract_words(byte *str, uint32 str_length, uint32 *nb_words);
bool is_in_dict(byte *word, byte **dict, uint32 nb_words_dict);
void c3(byte str_crypted[], uint32 str_length, byte **keys, uint32 nb_keys, byte **dict, uint32 nb_words_dict);

#endif // C3_H
