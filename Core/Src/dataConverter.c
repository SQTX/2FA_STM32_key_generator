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
		printf("[ERROR] Failed to allocate memory.\n");
	    return -1;  // ERROR
	} else {
		for (int i = 0; i < (decodedLength+1); i++) (*decoded)[i] = 0;
	}


	uint8_t decodedSize = base32_decode(encoded, *decoded, (decodedLength+1));	// DECODING


    if (decodedSize < 0) {
        printf("[ERROR] Decoding error\n");
        return -1;  // ERROR
    } else if (decodedSize == decodedLength){
    	return decodedSize;
    }
    return 0;	// Something wrong
}


uint8_t trimZeros(uint8_t* arr, uint8_t length) {
	uint8_t new_length = length;

    // Iteruj od końca, aż napotkasz niezerowy element
    while (new_length > 0 && arr[new_length - 1] == 0) {
        new_length--; // Zmniejsz nową długość tablicy
    }

    return new_length; // Zwróć nową długość bez końcowych zer
}

