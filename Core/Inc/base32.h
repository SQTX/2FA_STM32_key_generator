#pragma once


//#include <ctype.h>

#define BASE32_ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"
#define UNBASE32_LEN(len)  (((len)/8)*5)

int base32_char_to_value(char c);

int base32_decode(const char *encoded, uint8_t *output, size_t output_size);
