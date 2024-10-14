/*
 * base32.c
 *
 *  Created on: Oct 06, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#include "base32.h"


int base32_char_to_value(char);

//********************************************************************************************
// PUBLIC
//********************************************************************************************
int base32_decode(const char *encoded, uint8_t *output, size_t output_size) {
    size_t input_length = strlen(encoded);
    size_t i, bit_index = 0, buffer = 0, output_index = 0;

    for (i = 0; i < input_length; i++) {
//    	Ignore spaces and convert lowercase letters to uppercase.
        char c = toupper(encoded[i]);

        if (c == '=') {
            break; 				// Padding - ignore the rest of the data.
        }

        int value = base32_char_to_value(c);
        if (value == -1) {
            return -1; 			// Return error if an invalid character is encountered.
        }

//      Add value to buffer:
        buffer = (buffer << 5) | value;
        bit_index += 5;

//      If we have at least 8 bits, write a byte to output.
        if (bit_index >= 8) {
            bit_index -= 8;
            if (output_index >= output_size) {
                return -1; 		// No space in the output buffer.
            }
            output[output_index++] = (buffer >> bit_index) & 0xFF;
        }
    }

    return output_index; 		// Returns the number of bytes written.
}


//********************************************************************************************
// PRIVATE
//********************************************************************************************
/**
 * Helper function to convert a character to a value from the BASE32 alphabet
 */
int base32_char_to_value(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    } else if (c >= '2' && c <= '7') {
        return c - '2' + 26;
    } else {
        return -1; // Wrong char
    }
}
