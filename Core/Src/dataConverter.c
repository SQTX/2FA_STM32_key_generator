/*
 * dataConverter.c
 *
 *  Created on: Oct 13, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#include "dataConverter.h"


//********************************************************************************************
// PUBLIC
//********************************************************************************************
int8_t base32ToHex(const char *encoded, const uint8_t encodedLength, uint8_t **decoded) {
	const uint8_t decodedLength = {UNBASE32_LEN(encodedLength)};			// Length of decoded key


	*decoded = (uint8_t*) malloc( (decodedLength+1) * sizeof(uint8_t) );	// Dynamic memory allocation

//	Check if malloc was successful and cleaning
	if (*decoded == NULL) {
		printConsolePostfix(PRI_ERROR);
		printf("Failed to allocate memory.\n");
	    return -1;  // ERROR
	} else {
		for (int i = 0; i < (decodedLength+1); i++) (*decoded)[i] = 0;
	}


	uint8_t decodedSize = base32_decode(encoded, *decoded, (decodedLength+1));	// DECODING


    if (decodedSize < 0) {
    	printConsolePostfix(PRI_ERROR);
        printf("Decoding error\n");
        return -1;  // ERROR
    } else if (decodedSize == decodedLength){
    	return decodedSize;
    }
    return 0;	// Something wrong
}


uint8_t trimZeros(uint8_t* arr, uint8_t length) {
	uint8_t new_length = length;

//	Iterate backwards until you encounter a non-zero element
    while (new_length > 0 && arr[new_length - 1] == 0) {
        new_length--; // Decrease array length
    }

    return new_length; 	// New length without zeros
}

