#ifndef CHARS_H
#define CHARS_H

#include "types.h"

bool char_valid_message(byte c);
bool char_valid_key(byte c);
byte remove_diacritics(byte c);
bool is_delimiter(byte c);
byte to_lower(byte c);

#endif // CHARS_H
