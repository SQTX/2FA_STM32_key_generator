/*
 * memoryController.c
 *
 *  Created on: Oct 24, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#include "memoryController.h"


bool compareArrays(const char*, const uint8_t*, size_t);

//********************************************************************************************
// PUBLIC
//********************************************************************************************
//------ Checking and initializing memory ----------------------------------------------------
bool checkMemory() {
	uint8_t initByte = 0;
	uint8_t lastMemoryAddr = 0;

	while (eeprom_read(0x00, &initByte, 1) != HAL_OK) {}
	while (eeprom_read(0x01, &lastMemoryAddr, 1) != HAL_OK) {}

//	printf("Init byte: %x\n", initByte);
//	printf("Last addr: %x\n", lastMemoryAddr);

	if(initByte == INIT_BYTE && lastMemoryAddr == LAST_MEMORY_ADDR) return true;
	else return false;
}


bool initMemory(uint32_t memorySizeBytes) {
	uint8_t maxKeys = (uint8_t)(memorySizeBytes/KEY_FRAME_SIZE);
	writeGeneralDataInMemory(maxKeys, 0, 0x06, 0x00);

	uint8_t index = 0x06;
	uint8_t zeroValue = 0x00;
	while(index < LAST_MEMORY_ADDR) {
		if(eeprom_write(index, &zeroValue, 1) != HAL_OK) Error_Handler();
		index++;
	}

	if(checkMemory() == true) return true;		// Check whether initialization is successful
	else return false;
}


//------ Reading data from memory ------------------------------------------------------------
void readGeneralDataFromMemory(uint8_t *generalData) {
	while (eeprom_read(FIRST_MEMORY_ADDR, generalData, GENERAL_DATA_SIZE) != HAL_OK) {}
}


uint8_t readKeyFromMemory(uint8_t *data, uint8_t keysNumber, uint8_t keyAddr, char *searchName, uint8_t* wantedAddr) {
	const uint8_t JUMP_TO_KEY_VALUE = 2;
	const uint8_t JUMP_TO_NAME_VALUE = 15;
	const uint8_t JUMP_TO_FLAG_VALUE = 20;
	const uint8_t JUMP_TO_NEXT_KEY = KEY_FRAME_SIZE;


	uint8_t key[KEY_FRAME_KEY_SIZE] = {0};
	uint8_t keyName[KEY_FRAME_NAME_SIZE] = {0};


	if(keyAddr != 0x00) {
		while (eeprom_read(keyAddr+JUMP_TO_KEY_VALUE, key, KEY_FRAME_KEY_SIZE) != HAL_OK) {}
		while (eeprom_read(keyAddr+JUMP_TO_NAME_VALUE, keyName, KEY_FRAME_NAME_SIZE) != HAL_OK) {}

		for(int i = 0; i < KEY_FRAME_KEY_SIZE; i++){
			data[i] = key[i];
		}
		for(int i = 0; i < KEY_FRAME_NAME_SIZE; i++){
			data[i+13] = keyName[i];
		}

		return 0;
	} else if(searchName != NULL) {
		*wantedAddr = 0x00;
		uint8_t currentKeyAddr = FIRST_KEY_ADDR;

		for(uint8_t keyIndex = 0; keyIndex < keysNumber; keyIndex++) {
//			Check key's overwrite flag:
			uint8_t keyOWFlag = {0};
			while (eeprom_read(currentKeyAddr+JUMP_TO_FLAG_VALUE, &keyOWFlag, 1) != HAL_OK) {}

			if(keyOWFlag == 0x80) {
//				Skip key if has overwrite flag:
				currentKeyAddr += JUMP_TO_NEXT_KEY;
				keyIndex--;
				continue;
			}
//			-----------------------------------
//			Check key's name:
			while (eeprom_read(currentKeyAddr+JUMP_TO_NAME_VALUE, keyName, KEY_FRAME_NAME_SIZE) != HAL_OK) {}

//
			uint8_t wordSize = trimZeros(keyName, KEY_FRAME_NAME_SIZE);
//			printf("Nazwa klucza do sprawdzenia: %s \t %u\n", keyName, wordSize);
			if(compareArrays(searchName, keyName, wordSize)) {
//				FOUND KEY!
				while (eeprom_read(currentKeyAddr+JUMP_TO_KEY_VALUE, key, KEY_FRAME_KEY_SIZE) != HAL_OK) {}

				for(int i = 0; i < KEY_FRAME_KEY_SIZE; i++){
					data[i] = key[i];
				}
				for(int i = 0; i < KEY_FRAME_NAME_SIZE; i++){
					data[i+13] = keyName[i];
				}

				*wantedAddr = currentKeyAddr;	// RETURN ADDR
				return 0;
			} else {
				currentKeyAddr += JUMP_TO_NEXT_KEY;
				continue;
			}

		}

		return 1;
	} else {
		return 1;
	}
}


//------ Writing data in memory --------------------------------------------------------------
void writeGeneralDataInMemory(uint8_t maxKeys, uint8_t keysNumber, uint8_t lastUsedKeyAddr, uint8_t flags) {
	uint8_t generalData[GENERAL_DATA_SIZE] = {0};

	generalData[0] = INIT_BYTE;				// Inital byte
	generalData[1] = LAST_MEMORY_ADDR;		// Last address in memory

	generalData[2] = maxKeys;				// Number of max kesy in memory
	generalData[3] = keysNumber;			// Number keys write in memory
	generalData[4] = lastUsedKeyAddr;		// Addres of last used key

	generalData[5] = flags;		// General data flags


	for(int i = 0; i < GENERAL_DATA_SIZE; i++) {
		if(eeprom_write(FIRST_MEMORY_ADDR+i, &generalData[i], sizeof(generalData[i])) != HAL_OK) Error_Handler();
	}
}


void writeKeyInMemory(uint32_t keyAddr, uint8_t *keyFrame) {
	for(int i = 0; i < KEY_FRAME_SIZE; i++) {
		if(eeprom_write(keyAddr+i, &keyFrame[i], sizeof(keyFrame[i])) != HAL_OK) Error_Handler();
	}
}


//------ OTHER -------------------------------------------------------------------------------
void makeKeyFrame(uint8_t *keyFrame, uint8_t keyIndex, uint8_t keyLength, uint8_t *key, uint8_t nameLength, uint8_t *name, bool overwirteFlag, uint8_t crc) {
	for(int i = 0; i < KEY_FRAME_SIZE; i++) keyFrame[i] = 0x00;		// Set 0 in all bytes
//	-----------------------------------
//	1-st [0] byte
	keyFrame[0] = keyIndex;
//	-----------------------------------
//	2-nd [1] byte
//	Save 2 4-bits sizes on 1 Byte variable:
	uint8_t keySize = keyLength;
	uint8_t nameSize = nameLength;
	keySize &= 0xF;
	nameSize &= 0xF;
	uint8_t fullSizes = (keySize << 4) | nameSize;
	keyFrame[1] = fullSizes;
//	-----------------------------------
//	3-15-th [2-14] bytes
	for(int i = 2; i < (keyLength+2); i++) {
		keyFrame[i] = key[i-2];
	}
//	-----------------------------------
//	16-20-th [15-19] bytes
	for(int i = 15; i < (nameLength+15); i++) {
		keyFrame[i] = name[i-15];
	}
//	-----------------------------------
//	21-th [20] byte
	uint8_t flags = 0x00;
	if(overwirteFlag) flags = 0x80;		// 0b1000_0000
	keyFrame[20] = flags;
//	-----------------------------------
//	22-th [21] byte
	uint8_t crcValue = 0x00;
	keyFrame[21] = crcValue;
//	-----------------------------------
}


uint8_t findSpaceForKey(const uint8_t MAX_KEYS, const uint8_t KEYS_NUMBER, const uint8_t GENERAL_FLAGS) {
	const uint8_t JUMP_TO_NEXT_KEY = {KEY_FRAME_SIZE};
	const uint8_t JUMP_FROM_BEGIN_TO_FLAGS = {(KEY_FRAME_INDEX_SIZE+KEY_FRAME_2SIZE_SIZE+KEY_FRAME_KEY_SIZE+KEY_FRAME_NAME_SIZE)};

	uint8_t freeSpaceAddr = {0x00};


	if(GENERAL_FLAGS & OVERWRITE_MODE) {	// OW_mode ON
		uint8_t currentAddr = {FIRST_KEY_ADDR+JUMP_FROM_BEGIN_TO_FLAGS};

		for(uint8_t i = 0; i < MAX_KEYS; i++) {
			uint8_t keyFlag = {0x00};
			while (eeprom_read(currentAddr, &keyFlag, 1) != HAL_OK) {}

			if(keyFlag & OVERWRITE_FLAG) {
				currentAddr -= JUMP_FROM_BEGIN_TO_FLAGS;
				    return currentAddr;
			}else {
				currentAddr += JUMP_TO_NEXT_KEY;
				   continue;
			}
		}
	} else {	// OW_mode OFF
		uint8_t currentAddr = {FIRST_KEY_ADDR + (KEYS_NUMBER*JUMP_TO_NEXT_KEY)};
		return currentAddr;
	}


	return freeSpaceAddr;
}


void getAllNames(char** namesArray, uint8_t keysNumber) {
	const uint8_t JUMP_TO_NAME_VALUE = 15;
	const uint8_t JUMP_TO_FLAG_VALUE = 20;
	const uint8_t JUMP_TO_NEXT_KEY = KEY_FRAME_SIZE;

	uint8_t currentKeyAddr = {FIRST_KEY_ADDR};


	for(uint8_t keyIndex = 0; keyIndex < keysNumber; keyIndex++) {
//		Check key's overwrite flag:
		uint8_t keyOWFlag = {0};
		while (eeprom_read(currentKeyAddr+JUMP_TO_FLAG_VALUE, &keyOWFlag, 1) != HAL_OK) {}

		if(keyOWFlag == 0x80) {
//				Skip key if has overwrite flag:
			currentKeyAddr += JUMP_TO_NEXT_KEY;
			keyIndex--;
			continue;
		}else {
			uint8_t keyName[KEY_FRAME_NAME_SIZE] = {0x00};
			while (eeprom_read(currentKeyAddr+JUMP_TO_NAME_VALUE, keyName, KEY_FRAME_NAME_SIZE) != HAL_OK) {}

			uint8_t wordSize = trimZeros(keyName, KEY_FRAME_NAME_SIZE);
			char* newName = (char*) malloc ( (wordSize+1)*sizeof(char) );

			for(int i = 0; i < wordSize; i++) {
				newName[i] = keyName[i];
			}
			newName[wordSize] = '\0';

			namesArray[keyIndex] = newName;

			currentKeyAddr += JUMP_TO_NEXT_KEY;
		}

	}
}


void getAllOWFlagArr(bool* owFlagArray, const uint8_t MAX_KEYS) {
	const uint8_t JUMP_FROM_BEGIN_TO_FLAGS = {(KEY_FRAME_INDEX_SIZE+KEY_FRAME_2SIZE_SIZE+KEY_FRAME_KEY_SIZE+KEY_FRAME_NAME_SIZE)};

	uint8_t currentAddr = {FIRST_KEY_ADDR+JUMP_FROM_BEGIN_TO_FLAGS};

	for(uint8_t i = 0; i < MAX_KEYS; i++) {
		uint8_t flags = {0x00};
		while (eeprom_read(currentAddr, &flags, 1) != HAL_OK) {}

		if(flags & OVERWRITE_FLAG) owFlagArray[i] = true;
		else owFlagArray[i] = false;

		currentAddr += KEY_FRAME_SIZE;
	}
}


void setOWFlag(const uint8_t ADDR) {
	const uint8_t JUMP_TO_FLAG_VALUE = 20;
	uint8_t flagsByte = {0x00};

//	Get flag byte:
	while (eeprom_read(ADDR+JUMP_TO_FLAG_VALUE, &flagsByte, 1) != HAL_OK) {}

//	Set bit in byte:
	flagsByte |= OVERWRITE_FLAG;
	if(eeprom_write(ADDR+JUMP_TO_FLAG_VALUE, &flagsByte, 1) != HAL_OK) Error_Handler();
}


//********************************************************************************************
// TEST
//********************************************************************************************
void resetMemoryTest() {
	uint8_t foo = 0xA1;
	if(eeprom_write(FIRST_MEMORY_ADDR, &foo, sizeof(foo)) != HAL_OK) Error_Handler();
}


void addKeyTest() {
	writeGeneralDataInMemory(0x05, 0x01, 0x06, 0x00);
	uint8_t foo[22] = {0x01, 0xA3, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x21, 0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x00, 0x00, 0x66, 0x6F, 0x6F, 0x00, 0x00, 0x00, 0x00};
	for(int i = 0; i < 22; i++) {
		if(eeprom_write(FIRST_KEY_ADDR+i, &foo[i], sizeof(foo[i])) != HAL_OK) Error_Handler();
	}
}

//********************************************************************************************
// PRIVATE
//********************************************************************************************
bool compareArrays(const char* array1, const uint8_t* array2, size_t length) {
    for (size_t i = 0; i < length; i++) {
//    	printf("%c (%x) == %c (%x)\n", array1[i], array1[i], array2[i], array2[i]);
        if ((uint8_t)array1[i] != array2[i]) {
            return false;  // Zwróć false, jeśli elementy są różne
        }
    }
    return true;  // Zwróć true, jeśli wszystkie elementy są równe
}
