/*
 * features.c
 *
 *  Created on: Oct 26, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#include "features.h"


void getEncodedKeyFromUser(volatile uint32_t *prevWatchDogReset, char* ,int);
void getKeysNameFromUser(volatile uint32_t *prevWatchDogReset, char*, int);

//********************************************************************************************
// PUBLIC
//********************************************************************************************
int8_t addNewKey(volatile uint32_t *prevWatchDogReset, const uint8_t MAX_KEYS, uint8_t* keysNumber, uint8_t* generalFlags, uint8_t* activeKeyAddr) {
//	--------------------------------------------------
//	Get name and encoded key from user:
//	--------------------------------------------------
	char name[MAX_KEYS_NAME_SIZE+1] = {0};
	char encodedKey[MAX_ENCODED_KEY_SIZE+1] = {0};
	uint8_t key[MAX_KEY_SIZE] = {0};

	getKeysNameFromUser(prevWatchDogReset, name, PRINT_ENTRED_STRINGS);
	getEncodedKeyFromUser(prevWatchDogReset, encodedKey, PRINT_ENTRED_STRINGS);


//	--------------------------------------------------
//	Decoding key from BASE32 to HEX:
//	--------------------------------------------------
	const uint8_t keysNameSize = trimZeros((uint8_t*)name, MAX_KEYS_NAME_SIZE+1);
	printf("Name size: %u\n", keysNameSize);
	const uint8_t encodedSize = trimZeros((uint8_t*)encodedKey, MAX_ENCODED_KEY_SIZE+1);
	const uint8_t decodedSize = UNBASE32_LEN(encodedSize);
//	printf("Size przed: %u\n", encodedSize);
//	printf("Size po: %u\n", decodedSize);

	const int keySize = base32_decode(encodedKey, key, decodedSize);

	if(decodedSize == keySize) {
		printf("Token size: %u\n", keySize);
		printf("Decoded key: ");
		for (int i = 0; i < keySize; i++) {
//			printf("%c", key[i]);
		    printf("0x%x ", key[i]);
		}
		printf("\n");
	}

//	--------------------------------------------------
//	Calc current key index:
//	--------------------------------------------------
	uint8_t keyIndex = 0;

	if( (*generalFlags & 0x80) && (*keysNumber == MAX_KEYS) ) {
		return -1;
	} else if( (*generalFlags & 0x80) && (*keysNumber < MAX_KEYS) ) {
		bool* owFlagArray = (bool*) malloc( MAX_KEYS * sizeof(bool) );

		getAllOWFlagArr(owFlagArray, MAX_KEYS);

		for(uint8_t i = 0; i < MAX_KEYS; i++) {
			if(owFlagArray[i]) {
				keyIndex = (i+1);
				break;
			}
		}
		free(owFlagArray);
	}else {
		keyIndex = (*keysNumber)+1;
	}
	printf("New key index %u\n", keyIndex);

//	--------------------------------------------------
//	Calc CRC check sum:
//	--------------------------------------------------
//	TODO: Add a function that calculates CRC8.
	uint8_t crcValue = 0x00;

//	--------------------------------------------------
//	Set data in key's frame:
//	--------------------------------------------------
	uint8_t newKeyFrame[KEY_FRAME_SIZE] = {0};

	makeKeyFrame(newKeyFrame, keyIndex, decodedSize, key, keysNameSize, (uint8_t*)name, false, crcValue);
	printf("New frame: ");
	for (int i = 0; i < KEY_FRAME_SIZE; i++) {
	    printf("0x%x ", newKeyFrame[i]);
	}
	printf("\n");


//	--------------------------------------------------
//	Find nearest free memory location:
//	--------------------------------------------------
	//FindFreeSpace
	uint8_t freeAddr = {0x00};
	freeAddr = findSpaceForKey(MAX_KEYS, *keysNumber, *generalFlags);

	*activeKeyAddr = freeAddr;		//	Set new key as active:

	printf("Next free ADDR: %x\n", *activeKeyAddr);


//	--------------------------------------------------
//	Write key in memory:
//	--------------------------------------------------
	//AddKeyToMemory
	writeKeyInMemory(freeAddr, newKeyFrame);
	(*keysNumber) += 1;
	printf("Now keys are: %u\n", *keysNumber);


//	--------------------------------------------------
//	Change general data and switch to overwrite mode
//	--------------------------------------------------
	if(*generalFlags & 0x80) {				// OW_mode ON
		writeGeneralDataInMemory(MAX_KEYS, *keysNumber, *activeKeyAddr, *generalFlags);
	}else {									// OW mode OFF
		if(*keysNumber == MAX_KEYS) {		// Turn OW_mode ON
			(*generalFlags) |= 0x80;		// Set OW_flag
			writeGeneralDataInMemory(MAX_KEYS, *keysNumber, *activeKeyAddr, *generalFlags);
		}else {								// OW_mode still OFF
			writeGeneralDataInMemory(MAX_KEYS, *keysNumber, *activeKeyAddr, *generalFlags);
		}
	}

	return 0;
}


int8_t searchKey(volatile uint32_t *prevWatchDogReset, uint8_t keysNumber, uint8_t* keyAddr) {
//	--------------------------------------------------
//	Get key's name from user:
//	--------------------------------------------------
	char name[MAX_KEYS_NAME_SIZE+1] = {0};
	getKeysNameFromUser(prevWatchDogReset, name, PRINT_ENTRED_STRINGS);


//	--------------------------------------------------
//	Searching:
//	--------------------------------------------------
	const uint8_t NOT_USE_ADDR = 0x00;
	uint8_t wantedAddr = 0x00;

	uint8_t currentKey[13] = {0};		// Container for key from memory
	uint8_t currentKeyName[5] = {0};	// Container for name from memory
	uint8_t data[13+5] = {0};			// Container for all data from memory

	int8_t status = readKeyFromMemory(data, keysNumber, NOT_USE_ADDR, name, &wantedAddr);

	if(status == 0) {
		for(int i = 0; i < 13; i++) currentKey[i] = data[i];			//  Get key from data
		for(int i = 0; i < 5; i++) currentKeyName[i] = data[i+13];	//  Get name from data

		printLineSeparator('-');
		printf("THE KEY HAS BEEN FOUND!\n");
		printLineSeparator('-');
		printf("Name:\t\t%s\n", currentKeyName);
		printf("Key:\t\t");
		for (int i = 0; i < 10; i++) {
			printf("0x%x ", currentKey[i]);
		}
		printf("\n");
		printf("Address:\t0x%x\n", wantedAddr);
		printLineSeparator('-');

		*keyAddr = wantedAddr;

		return 0;
	} else if(status == 1) {
		printConsolePostfix(PRI_INFO);
		printf("\tThe entered name does not exist in the memory\n");
		return 1;
	} else {
		printConsolePostfix(PRI_ERROR);
		printf("\tSomething wrong\n");
		return -1;
	}
}


int8_t deleteKey(volatile uint32_t *prevWatchDogReset, const uint8_t MAX_KEYS, uint8_t* keysNumber, uint8_t* generalFlags, uint8_t* activeKeyAddr) {
	//	--------------------------------------------------
	//	Get key's name from user:
	//	--------------------------------------------------
		char name[MAX_KEYS_NAME_SIZE+1] = {0};
		getKeysNameFromUser(prevWatchDogReset, name, PRINT_ENTRED_STRINGS);


	//	--------------------------------------------------
	//	Search key addr:
	//	--------------------------------------------------
		const uint8_t NOT_USE_ADDR = 0x00;
		uint8_t wantedAddr = 0x00;
		uint8_t data[13+5] = {0};			// Container for all data from memory

		int8_t status = readKeyFromMemory(data, *keysNumber, NOT_USE_ADDR, name, &wantedAddr);


	//	--------------------------------------------------
	//	If key is active change it
	//	--------------------------------------------------
		if(*activeKeyAddr == wantedAddr) {
			printConsolePostfix(PRI_INFO);
			printf("The active key has been deleted. The active key has changed\n");

			*activeKeyAddr = FIRST_KEY_ADDR;	// NOTE: Hardcoded address to first key in memory
//			TODO: Here there should be an algorithm that searches the memory for another available key.
//			TODO: If there are no keys in memory, a function should be called to add them
		}

	//	--------------------------------------------------
	//	Set OW flag and change value of keys:
	//	--------------------------------------------------
		if(status == 0) {
			setOWFlag(wantedAddr);
			printConsolePostfix(PRI_INFO);
			printf("The provided key has been deleted\n");

//			Update general data:
			(*keysNumber) -= 1;
			writeGeneralDataInMemory(MAX_KEYS, *keysNumber, *activeKeyAddr, *generalFlags);
			return 0;
		} else if(status == 1) {
			printConsolePostfix(PRI_WARNING);
			printf("The specified name does not exist in the key database\n");
			return 1;
		} else {
			printConsolePostfix(PRI_ERROR);
			printf("Something wrong\n");
			return -1;
		}
}


void showKeysList(uint8_t keysNumber) {
	fflush(stdout);

//	Create a dynamic array of string pointers:
    char** wordsArray = malloc(keysNumber * sizeof(char*));
    if (wordsArray == NULL) {
        fprintf(stderr, "Memory allocation error!\n");
        return;
    }

//  Assign words to the board:
    getAllNames(wordsArray, keysNumber);

//	Display words:
    printf("Keys number: %u\n", keysNumber);
    printf("Keys list:\n");
    for (int i = 0; i < keysNumber; i++) {
        printf("- %s\n", wordsArray[i]);
    }

//  Free memory up:
    for (int i = 0; i < keysNumber; i++) {
        free(wordsArray[i]); 	// Free the memory occupied by strings
    }
    free(wordsArray); 			// Free the memory occupied by the pointer array
    fflush(stdout);
}


//********************************************************************************************
// PRIVATE
//********************************************************************************************
void getEncodedKeyFromUser(volatile uint32_t *prevWatchDogReset, char* encodedKey, int printIt) {
	uint8_t status = {1};

	printf("Enter your key: \t\t(max 21 characters)\n");
	fflush(stdout);
	while(status != 0) {
		resetWatchDog(prevWatchDogReset);
		uint8_t value = {0};

		if (HAL_UART_Receive(&huart2, &value, 1, 0) == HAL_OK) {
			// TODO: Try to print out each char inserted
			status = getStringViaKeyboard(value, encodedKey, MAX_ENCODED_KEY_SIZE+1);

			if (status == 0) {
				printf("Encoded key:\t%s\n", encodedKey);
			} else if(status < 0) {
				printf("[INFO] Something wrong, the data getting process's interrupted\n");
			}
		}
	}

	if(printIt != 0) {
		printf("Entered encoded key: ");
		for(int i = 0; i < MAX_ENCODED_KEY_SIZE; i++) {
			printf("%c ", encodedKey[i]);
		}
		printf("\n");
	}
}


void getKeysNameFromUser(volatile uint32_t *prevWatchDogReset, char* name, int printIt) {
	uint8_t status = {1};

	printf("Enter name for your key: \t(max 5 characters)\n");
	fflush(stdout);
	while(status != 0) {
		resetWatchDog(prevWatchDogReset);
		uint8_t value = {0};

		if (HAL_UART_Receive(&huart2, &value, 1, 0) == HAL_OK) {
			// TODO: Try to print out each char inserted
			status = getStringViaKeyboard(value, name, MAX_KEYS_NAME_SIZE+1);

			if (status == 0) {
				printf("Key's name:\t%s\n", name);
			} else if(status < 0) {
				printf("[INFO] Something wrong, the data getting process's interrupted\n");
			}
		}
	}

	if(printIt != 0) {
		printf("Entered key name: ");
		for(int i = 0; i < MAX_KEYS_NAME_SIZE; i++) {
			printf("%c ", name[i]);
		}
		printf("\n");
	}
}
