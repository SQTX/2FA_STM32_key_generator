/*
 * memoryController.h
 *
 *  Created on: Oct 24, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#ifndef INC_MEMORYCONTROLLER_H_
#define INC_MEMORYCONTROLLER_H_

#include <stdbool.h>
#include "eeprom.h"
#include "dataConverter.h"


#define INIT_BYTE 			0xCA		// CONST first inital byte (0b11001010)
#define FIRST_MEMORY_ADDR 	0x00		// 0 	- First address in memory
#define FIRST_KEY_ADDR		0x06		// 6 	- First address for key in memory
#define LAST_MEMORY_ADDR 	0x7F		// 127 	- Last address in memory

//+----------------------------------------------------------------------------------------------------------+
//|										General Data [6 Bytes]			  									 |
//+------------+------------------+----------+-------------+--------------------+----------------------------+
//| Init Byte  | Last memory ADDR | Max keys | Kesy number | Last used key ADDR | Flags						 |
//+------------+------------------+----------+-------------+--------------------+----------------------------+
//| 0b11001010 | HEX ADDR         | 0-255    | 0-255       | HEX ADDR           | 1-st write by OW flag      |
//| 1Byte      | 1Byte            | 1Byte    | 1Byte       | 1Byte              | 1Byte     				 |
//+------------+------------------+----------+-------------+--------------------+----------------------------+
//|							From FIRST_MEMORY_ADDR to 0x05 memory address			  						 |
//+----------------------------------------------------------------------------------------------------------+
// TODO: Change the address to 32 bit values
#define GENERAL_DATA_SIZE	6			// Max size = 6 Bytes (FROM 0x00 TO 0x05)
//Flag masks:
// NOTE: 1-st flag (0 - save by index and key number, 1 - save by OW flag)
#define OVERWRITE_MODE			0x80	// Overwrite mode flag (0b1000_0000)

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
//Flag masks:
#define OVERWRITE_FLAG			0x80	// Overwrite flag (0b1000_0000)


/**
 * @brief Verifies the EEPROM initialization status by checking specific memory locations.
 *
 * This function reads two specific bytes from EEPROM memory: an initialization byte
 * and the last used memory address. It verifies whether these bytes match the expected
 * values, indicating that the EEPROM has been properly initialized.
 *
 * @return Returns @p true if the EEPROM memory is correctly initialized; otherwise, returns @p false.
 *
 * @note The function loops until successful reads are performed for the required memory locations.
 *       It assumes that constants @p INIT_BYTE and @p LAST_MEMORY_ADDR define the expected values.
 *
 * @warning This function may enter an infinite loop if the EEPROM is unresponsive or if there is
 *          a hardware issue with the I2C connection.
 */
bool checkMemory();

/**
 * @brief Initializes EEPROM memory by calculating the maximum number of keys and storing general data.
 *
 * This function initializes the EEPROM memory by calculating the maximum number of keys that can be stored
 * based on the provided memory size and a predefined key frame size. It then writes general data into
 * EEPROM memory and verifies if initialization was successful by calling @p checkMemory.
 *
 * @param[in] memorySizeBytes The total size of EEPROM memory in bytes.
 *
 * @return Returns @p true if the initialization was successful, or @p false if it failed.
 *
 * @note The function assumes that @p KEY_FRAME_SIZE defines the size of each key frame in bytes, and
 *       that @p writeGeneralDataInMemory handles the actual memory writing operation.
 *
 * @warning Ensure that the @p memorySizeBytes parameter is accurate and that the EEPROM memory is accessible.
 *          The success of initialization depends on the @p checkMemory function returning @p true.
 */
bool initMemory(uint32_t memorySizeBytes);


/**
 * @brief Reads general data from a specified EEPROM memory location.
 *
 * This function reads a block of general data from the EEPROM, starting from a predefined
 * memory address. It loops until a successful read operation is performed.
 *
 * @param[out] generalData Pointer to a buffer where the general data will be stored.
 *
 * @note The function uses a blocking approach, looping until @p eeprom_read returns @p HAL_OK.
 *       The starting address of the data and the expected data size are defined by
 *       @p FIRST_MEMORY_ADDR and @p GENERAL_DATA_SIZE, respectively.
 *
 * @warning Ensure that @p generalData points to a valid buffer large enough to hold
 *          @p GENERAL_DATA_SIZE bytes. The function will enter an infinite loop if the EEPROM
 *          is unresponsive, so hardware readiness should be verified before calling.
 */
void readGeneralDataFromMemory(uint8_t *generalData);


uint8_t readKeyFromMemory(uint8_t *data, uint8_t keysNumber, uint8_t keyAddr, char *searchName, uint8_t* wantedAddr);



void writeGeneralDataInMemory(uint8_t maxKeys, uint8_t keysNumber, uint8_t lastUsedKeyAddr, uint8_t flags);


void writeKeyInMemory(uint32_t keyAddr, uint8_t *keyFrame);


void makeKeyFrame(uint8_t *keyFrame, uint8_t keyIndex, uint8_t keyLength, uint8_t *key, uint8_t nameLength, uint8_t *name, bool overwirteFlag, uint8_t crc);


uint8_t findSpaceForKey(const uint8_t MAX_KEYS, const uint8_t KEYS_NUMBER, const uint8_t GENERAL_FLAGS);

void getAllOWFlagArr(bool* owFlagArray, const uint8_t MAX_KEYS);


void setOWFlag(const uint8_t ADDR);


void resetMemoryTest();
void addKeyTest();

void getEncodedKeyFromUser();
void getKeysNameFromUser();


#endif /* INC_MEMORYCONTROLLER_H_ */
