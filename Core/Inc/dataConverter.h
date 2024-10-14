/*
 * dataConverter.h
 *
 *  Created on: Oct 13, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#ifndef INC_DATACONVERTER_H_
#define INC_DATACONVERTER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "base32.h"


/**
 * @brief Converts a Base32-encoded string into its hexadecimal (binary) representation.
 *
 * This function decodes a Base32-encoded string and returns the decoded data as a dynamically
 * allocated array of bytes. The memory for the decoded data is allocated within the function
 * and the pointer to the decoded data is returned via the @p decoded parameter.
 *
 * @param[in] encoded        Pointer to the Base32-encoded input string.
 * @param[in] encodedLength  Length of the Base32-encoded input string.
 * @param[out] decoded       Pointer to a pointer where the decoded data will be stored. The memory
 *                           for the decoded data is dynamically allocated within the function.
 *
 * @return Returns the length of the decoded data if the operation was successful. Returns -1 if an
 *         error occurred (e.g., memory allocation failure or decoding error).
 *
 * @note The caller is responsible for freeing the memory allocated for the @p decoded data
 *       after it is no longer needed.
 *
 * @warning Ensure that the @p encoded string is properly formatted as Base32 and that the
 *          @p encodedLength is correct. Incorrect input may result in decoding errors.
 */
int8_t base32ToHex(const char *encoded, const uint8_t encodedLength, uint8_t **decoded);

#endif /* INC_DATACONVERTER_H_ */
