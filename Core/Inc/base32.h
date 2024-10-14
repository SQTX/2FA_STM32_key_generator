/*
 * base32.h
 *
 *  Created on: Oct 06, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#ifndef INC_BASE32_H_
#define INC_BASE32_H_

#include <stdint.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>


#define BASE32_ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"	// Base32 character database
#define UNBASE32_LEN(len)  (((len)/8)*5)					// Key length after decoding


/**
 * @brief Decodes a Base32-encoded string into its raw byte representation.
 *
 * This function decodes a given Base32-encoded string and stores the result in the provided output buffer.
 * It handles padding characters ('=') and ignores spaces. Additionally, it converts lowercase letters
 * to uppercase during decoding.
 *
 * @param[in] encoded      Pointer to the Base32-encoded input string.
 * @param[out] output      Pointer to the buffer where the decoded bytes will be stored.
 * @param[in] output_size  Size of the output buffer in bytes.
 *
 * @return Returns the number of bytes written to the output buffer upon successful decoding.
 *         Returns -1 if an invalid character is encountered in the input or if the output buffer
 *         is too small to hold the decoded data.
 *
 * @note The caller is responsible for ensuring that the output buffer is large enough to hold
 *       the decoded data.
 *
 * @warning The function does not check for null pointers. Ensure that both @p encoded and
 *          @p output are valid pointers.
 */
int base32_decode(const char *encoded, uint8_t *output, size_t output_size);

#endif /* INC_BASE32_H_ */
