/*
 * memoryController.h
 *
 *  Created on: Oct 24, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#ifndef INC_MEMORYCONTROLLER_H_
#define INC_MEMORYCONTROLLER_H_

#include "eeprom.h"
#include <stdbool.h>


#define INIT_BYTE 			0xCA		// CONST first inital byte (0b11001010)
#define FIRST_MEMORY_ADDR 	0x00		// 0 	- First address in memory
#define FIRST_KEY_ADDR		0x06		// 6 	- First address for key in memory
#define LAST_MEMORY_ADDR 	0x7F		// 127 	- Last address in memory

//+------------------------------------------------------------------------------------------+
//|										General Data [6 Bytes]			  					 |
//+------------+------------------+----------+-------------+--------------------+------------+
//| Init Byte  | Last memory ADDR | Max keys | Kesy number | Last used key ADDR | Free Space |
//+------------+------------------+----------+-------------+--------------------+------------+
//| 0b11001010 | HEX ADDR         | 0-255    | 0-255       | HEX ADDR           | ...        |
//| 1Byte      | 1Byte            | 1Byte    | 1Byte       | 1Byte              | 1Byte      |
//+------------+------------------+----------+-------------+--------------------+------------+
//|							From FIRST_MEMORY_ADDR to 0x05 memory address			  		 |
//+------------------------------------------------------------------------------------------+
// TODO: Change the address to 32 bit values
#define GENERAL_DATA_SIZE	6			// Max size = 6 Bytes (FROM 0x00 TO 0x05)

//+---------------------------------------------------------------------------------------------+
//|										Key Frame [22 Bytes] 			  						|
//+-----------+-------------------+------------+-------------+-------------------------+--------+
//| Key index |       Sizes       |    Key     |    Name     |          Flags          |  CRC   |
//+-----------+-------------------+------------+-------------+-------------------------+--------+
//| 0-255     | 2x0-15            | max 13 hex | max 5 chars | 1-st bit overwrite flag | ...    |
//| 1 Byte    | 2x4 bits          | 13 Bytes   | 5 Bytes     | 1 Byte                  | 1 Byte |
//| ...       | 1-st: key length  |            |             |                         |        |
//| ...       | 2-nd: name length |            |             |                         |        |
//+-----------+-------------------+------------+-------------+-------------------------+--------+
//|						From 0x06 to LAST_MEMORY_ADDR memory address			  				|
//+---------------------------------------------------------------------------------------------+
#define KEY_FRAME_SIZE				22	// Size of single key's frame = 22 Bytes
//#define START_BITS				0xF	// WARNING: NOT USED
//#define END_BITS					0x0	// WARNING: NOT USED
#define KEY_FRAME_INDEX_SIZE		1	// Key's index in memory
#define KEY_FRAME_2SIZE_SIZE		1	// 2 sizes of strings (key and name)
#define KEY_FRAME_KEY_SIZE			13	// Key string
#define KEY_FRAME_NAME_SIZE			5	// Name string
#define KEY_FRAME_OVERWRITE_SIZE	1	// Overwrite flag
#define KEY_FRAME_CRC_SIZE			1	// Check sum


bool checkMemory();
bool initMemory(uint32_t memorySizeBytes);

void readGeneralDataFromMemory(uint8_t *generalData);
uint8_t readKeyFromMemory(uint8_t *data, uint8_t keysNumber, uint8_t keyAddr, uint8_t *searchName);

void writeGeneralDataInMemory(uint8_t maxKeys, uint8_t keysNumber, uint8_t lastUsedKeyAddr);	// Default values for this args are: NONE, 0, FIRST_KEY_ADDR
void writeKeyInMemory(uint32_t keyAddr, uint8_t *keyFrame);
void makeKeyFrame(uint8_t *keyFrame, uint8_t keyIndex, uint8_t keyLength, uint8_t *key, uint8_t nameLength, uint8_t *name, bool overwirteFlag, uint8_t crc);

void resetMemoryTest();
void addKeyTest();

#endif /* INC_MEMORYCONTROLLER_H_ */
