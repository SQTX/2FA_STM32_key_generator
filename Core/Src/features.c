/*
 * features.c
 *
 *  Created on: Oct 26, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#include "features.h"


void getEncodedKeyFromUser(char* ,int);
void getKeysNameFromUser(char*, int);

//********************************************************************************************
// PUBLIC
//********************************************************************************************
int8_t addNewKey(const uint8_t MAX_KEYS, uint8_t* keysNumber, uint8_t* generalFlags) {
//	--------------------------------------------------
//	Get name and encoded key from user:
//	--------------------------------------------------
	char name[MAX_KEYS_NAME_SIZE+1] = {0};
	char encodedKey[MAX_ENCODED_KEY_SIZE+1] = {0};
	uint8_t key[MAX_KEY_SIZE] = {0};

	getKeysNameFromUser(name, PRINT_ENTRED_STRINGS);
	getEncodedKeyFromUser(encodedKey, PRINT_ENTRED_STRINGS);


//	--------------------------------------------------
//	Decoding key from BASE32 to HEX:
//	--------------------------------------------------
	const uint8_t keysNameSize = trimZeros((uint8_t*)name, MAX_KEYS_NAME_SIZE+1);
	printf("Name size: %u\n", keysNameSize);
	const uint8_t encodedSize = trimZeros((uint8_t*)encodedKey, MAX_ENCODED_KEY_SIZE+1);
	const uint8_t decodedSize = UNBASE32_LEN(encodedSize);
	printf("Size przed: %u\n", encodedSize);
	printf("Size po: %u\n", decodedSize);

	const int keySize = base32_decode(encodedKey, key, decodedSize);

	if(decodedSize == keySize) {
		printf("Token size: %u\n", keySize);
		printf("Dekodowany tekst: ");
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

	printf("Next free ADDR: %x\n", freeAddr);


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
	if(*generalFlags & 0x80) {		// OW_mode ON
		writeGeneralDataInMemory(MAX_KEYS, *keysNumber, freeAddr, 0x80);
	}else {							// OW mode OFF
		if(*keysNumber == MAX_KEYS) {	// Turn OW_mode ON
			writeGeneralDataInMemory(MAX_KEYS, *keysNumber, freeAddr, 0x80);
		}else {							// OW_mode still OFF
			(*generalFlags) |= 0x80;		// Set OW_flag
			writeGeneralDataInMemory(MAX_KEYS, *keysNumber, freeAddr, *generalFlags);
		}
	}

	return 0;
}


int8_t searchKey(uint8_t keysNumber) {
//	--------------------------------------------------
//	Get key's name from user:
//	--------------------------------------------------
	char name[MAX_KEYS_NAME_SIZE+1] = {0};
	getKeysNameFromUser(name, PRINT_ENTRED_STRINGS);


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

		printf("--------------------------\n");
		printf("THE KEY HAS BEEN FOUND!\n");
		printf("--------------------------\n");
		printf("Name:\t\t%s\n", currentKeyName);
		printf("Key:\t\t");
		for (int i = 0; i < 10; i++) {
			printf("0x%x ", currentKey[i]);
		}
		printf("\n");
		printf("Address:\t0x%x\n", wantedAddr);
		printf("--------------------------\n");

		return 0;
	} else if(status == 1) {
		printf("[INFO]\tThe entered name does not exist in the key database\n");
		return 1;
	} else {
		printf("[ERROR]\tSomething wrong\n");
		return -1;
	}
}


int8_t deleteKey(uint8_t* keysNumber) {
	//	--------------------------------------------------
	//	Get key's name from user:
	//	--------------------------------------------------
		char name[MAX_KEYS_NAME_SIZE+1] = {0};
		getKeysNameFromUser(name, PRINT_ENTRED_STRINGS);


	//	--------------------------------------------------
	//	Search key addr:
	//	--------------------------------------------------
		const uint8_t NOT_USE_ADDR = 0x00;
		uint8_t wantedAddr = 0x00;

		uint8_t data[13+5] = {0};			// Container for all data from memory

		int8_t status = readKeyFromMemory(data, *keysNumber, NOT_USE_ADDR, name, &wantedAddr);

	//	--------------------------------------------------
	//	Set OW flag and change value of keys:
	//	--------------------------------------------------
		if(status == 0) {
			setOWFlag(wantedAddr);
			printf("[INFO] Podany klucz zostal usuniety\n");
			(*keysNumber) -= 1;
			return 0;
		} else if(status == 1) {
			printf("Podana nazwa nie istnieje w bazie kluczy\n");
			return 1;
		} else {
			printf("Something wrong\n");
			return -1;
		}
}


void showKeysList(uint8_t keysNumber) {
	fflush(stdout);

    // Tworzymy dynamiczną tablicę wskaźników na stringi
    char** wordsArray = malloc(keysNumber * sizeof(char*));
    if (wordsArray == NULL) {
        fprintf(stderr, "Błąd alokacji pamięci!\n");
        return;
    }

    // Przypisujemy słowa do tablicy
    getAllNames(wordsArray, keysNumber);

    // Wyświetlamy słowa
    printf("Keys number: %u\n", keysNumber);
    printf("Keys list:\n");
    for (int i = 0; i < keysNumber; i++) {
        printf("- %s\n", wordsArray[i]);
    }

    // Zwalniamy pamięć
    for (int i = 0; i < keysNumber; i++) {
        free(wordsArray[i]); // Zwalniamy pamięć zajmowaną przez stringi
    }
    free(wordsArray); // Zwalniamy pamięć zajmowaną przez tablicę wskaźników
    fflush(stdout);
}


//********************************************************************************************
// PRIVATE
//********************************************************************************************
void getEncodedKeyFromUser(char* encodedKey, int printIt) {
	uint8_t status = {1};

	printf("Enter your key: \t\t(max 21 characters)\n");
	fflush(stdout);
	while(status != 0) {
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


void getKeysNameFromUser(char* name, int printIt) {
	uint8_t status = {1};

	printf("Enter name for your key: \t(max 5 characters)\n");
	fflush(stdout);
	while(status != 0) {
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
