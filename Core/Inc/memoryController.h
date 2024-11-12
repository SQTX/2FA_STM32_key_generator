/*
 * memoryController.h
 *
 *  Created on: Oct 24, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#ifndef INC_MEMORYCONTROLLER_H_
#define INC_MEMORYCONTROLLER_H_

//Std:
#include <stdbool.h>
//My:
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


/**
 * @brief Reads a key from memory either by address or by name.
 *
 * This function attempts to read a key from the memory either using the provided key address (`keyAddr`)
 * or by searching for a key name (`searchName`). The data read from the memory is returned in the `data` array.
 * If a matching key is found by name, the address of the key is returned in `wantedAddr`.
 *
 * @param data Pointer to a buffer where the key data and name will be stored.
 * @param keysNumber The total number of keys in memory.
 * @param keyAddr The address of the key to be read, or `0x00` to search by name.
 * @param searchName The name of the key to search for, or `NULL` if reading by address.
 * @param wantedAddr Pointer to store the address of the found key.
 *
 * @return
 * - `0` if the key is successfully read by address or found by name.
 * - `1` if no matching key was found by name or address.
 *
 * @note
 * - If `keyAddr` is non-zero, the function directly reads the key from the provided address.
 * - If `keyAddr` is `0x00`, the function searches the memory for a key with a name matching `searchName`.
 *
 * @example
 * // Example usage:
 * uint8_t data[KEY_FRAME_KEY_SIZE + KEY_FRAME_NAME_SIZE];
 * uint8_t wantedAddr = 0x00;
 * int result = readKeyFromMemory(data, totalKeys, 0x00, "MyKey", &wantedAddr);
 * if (result == 0) {
 *     // Key found and data is in `data` buffer, address is in `wantedAddr`
 * } else {
 *     // No matching key found
 * }
 */
uint8_t readKeyFromMemory(uint8_t *data, uint8_t keysNumber, uint8_t keyAddr, char *searchName, uint8_t* wantedAddr);


/**
 * @brief Writes general data to memory, such as the number of keys, last used key address, and flags.
 *
 * This function writes the general information about the keys stored in memory to the EEPROM.
 * The general data includes the maximum number of keys allowed, the number of keys currently in memory,
 * the address of the last used key, and other flags.
 *
 * @param maxKeys The maximum number of keys that can be stored in memory.
 * @param keysNumber The current number of keys stored in memory.
 * @param lastUsedKeyAddr The address of the last used key in memory.
 * @param flags General flags to be stored in the data.
 *
 * @return None
 *
 * @note
 * - The general data is stored in a predefined format starting from `FIRST_MEMORY_ADDR`.
 * - The data is written sequentially to memory, one byte at a time.
 *
 * @example
 * // Example usage:
 * uint8_t maxKeys = 10;
 * uint8_t keysNumber = 5;
 * uint8_t lastUsedKeyAddr = 0x20;
 * uint8_t flags = 0x01;
 * writeGeneralDataInMemory(maxKeys, keysNumber, lastUsedKeyAddr, flags);
 */
void writeGeneralDataInMemory(uint8_t maxKeys, uint8_t keysNumber, uint8_t lastUsedKeyAddr, uint8_t flags);


/**
 * @brief Writes general data to memory, such as the maximum number of keys, number of keys in memory,
 *        the address of the last used key, and other flags.
 *
 * This function writes general metadata about the keys stored in memory to the EEPROM. The general data includes:
 * - The maximum number of keys allowed in memory (`maxKeys`),
 * - The current number of keys stored in memory (`keysNumber`),
 * - The address of the last used key (`lastUsedKeyAddr`),
 * - General flags (`flags`).
 *
 * The data is stored in a predefined format in the EEPROM, starting from the address `FIRST_MEMORY_ADDR`.
 * If any error occurs during the writing process, the `Error_Handler` function will be called.
 *
 * @param maxKeys The maximum number of keys allowed to be stored in memory.
 * @param keysNumber The current number of keys stored in memory.
 * @param lastUsedKeyAddr The address of the last used key.
 * @param flags General flags related to the state of the key storage system.
 *
 * @return None
 *
 * @note
 * - The function writes the general data to memory starting from the address `FIRST_MEMORY_ADDR`.
 * - The data is written byte by byte. If any byte fails to be written, the `Error_Handler` function will be invoked.
 * - The general data size is defined by `GENERAL_DATA_SIZE`.
 *
 * @example
 * // Example usage:
 * uint8_t maxKeys = 10;
 * uint8_t keysNumber = 5;
 * uint8_t lastUsedKeyAddr = 0x20;
 * uint8_t flags = 0x01;
 * writeGeneralDataInMemory(maxKeys, keysNumber, lastUsedKeyAddr, flags);
 */
void writeKeyInMemory(uint32_t keyAddr, uint8_t *keyFrame);


/**
 * @brief Creates a key frame with the provided key data, and stores it in the specified `keyFrame` array.
 *
 * This function assembles a key frame structure in the `keyFrame` array. The key frame includes the following:
 * - Key index (byte 0),
 * - Key size and name size (byte 1),
 * - Key data (bytes 2 to `keyLength + 1`),
 * - Key name (bytes 15 to `nameLength + 14`),
 * - Overwrite flag (byte 20),
 * - CRC value (byte 21).
 *
 * The key frame is used to store key information in EEPROM or other memory.
 * The size of the key and name is encoded in 4-bit segments in byte 1, where the upper nibble stores the key size and the lower nibble stores the name size.
 *
 * @param keyFrame The buffer where the key frame will be stored. It must have space for `KEY_FRAME_SIZE` bytes.
 * @param keyIndex The index of the key in the key storage (byte 0 of the frame).
 * @param keyLength The length of the key data (byte 1 upper nibble).
 * @param key The key data that will be included in the frame (bytes 2 to `keyLength + 1`).
 * @param nameLength The length of the key name (byte 1 lower nibble).
 * @param name The key name that will be included in the frame (bytes 15 to `nameLength + 14`).
 * @param overwirteFlag A boolean flag indicating if the key is marked for overwriting (byte 20).
 * @param crc The CRC value (byte 21), typically used for error checking.
 *
 * @return None
 *
 * @note
 * - The function assumes that the `keyFrame` buffer has been pre-allocated with sufficient space (`KEY_FRAME_SIZE`).
 * - If the `overwirteFlag` is set to `true`, the corresponding flag byte (byte 20) will be set to `0x80`.
 * - The function does not calculate the CRC value but places a placeholder (`0x00`) in the frame.
 *
 * @example
 * // Example usage:
 * uint8_t keyFrame[KEY_FRAME_SIZE];
 * uint8_t key[KEY_FRAME_KEY_SIZE] = {0x01, 0x02, 0x03}; // Sample key
 * uint8_t name[KEY_FRAME_NAME_SIZE] = {'K', 'e', 'y', '1'}; // Sample key name
 * makeKeyFrame(keyFrame, 1, 3, key, 4, name, true, 0x00);
 */
void makeKeyFrame(uint8_t *keyFrame, uint8_t keyIndex, uint8_t keyLength, uint8_t *key, uint8_t nameLength, uint8_t *name, bool overwirteFlag, uint8_t crc);


/**
 * @brief Finds an available memory address to store a new key.
 *
 * This function searches for an available address in memory to store a new key. It checks whether the system is in overwrite mode
 * or normal mode, and based on that, it either looks for a space where the overwrite flag is set or looks for the next available
 * space in the memory.
 *
 * @param MAX_KEYS The maximum number of keys that can be stored in memory.
 * @param KEYS_NUMBER The current number of keys stored in memory.
 * @param GENERAL_FLAGS The general flags that define the operation mode (overwrite mode or normal mode).
 *
 * @return The address of the available space where a new key can be stored. If no space is found, returns `0x00`.
 *
 * @note
 * - The function assumes the memory has space for up to `MAX_KEYS` keys.
 * - If overwrite mode is enabled (checked via `GENERAL_FLAGS`), the function will return the address of the first available key
 *   that has the overwrite flag set.
 * - If overwrite mode is disabled, the function will return the next available address based on the number of keys currently stored.
 * - The function interacts with EEPROM via `eeprom_read` and `eeprom_write` functions to check for available addresses.
 *
 * @example
 * // Example usage:
 * uint8_t spaceAddr = findSpaceForKey(10, 5, OVERWRITE_MODE);
 * if (spaceAddr != 0x00) {
 *     // Use the space for the new key.
 * }
 */
uint8_t findSpaceForKey(const uint8_t MAX_KEYS, const uint8_t KEYS_NUMBER, const uint8_t GENERAL_FLAGS);


/**
 * @brief Reads the names of all keys stored in memory and stores them in an array.
 *
 * This function retrieves the names of all keys from memory and stores them in the provided array of strings. It checks each key for
 * its overwrite flag and skips keys with the overwrite flag set. It also trims the trailing zeros from the name data and allocates
 * memory dynamically to store the names of keys.
 *
 * @param namesArray Pointer to an array of strings (array of `char*`) where the names of keys will be stored.
 * @param keysNumber The number of keys whose names should be retrieved.
 *
 * @note
 * - This function assumes that `keysNumber` specifies the correct number of keys stored in memory.
 * - The memory for each name is dynamically allocated, and it is the responsibility of the caller to free this memory once it is no
 *   longer needed.
 * - The function interacts with EEPROM via `eeprom_read` and assumes that the memory layout is consistent with the `JUMP_TO_*`
 *   offsets defined in the code.
 *
 * @example
 * // Example usage:
 * char* names[10];
 * getAllNames(names, 5);
 * for (int i = 0; i < 5; i++) {
 *     printf("Key name %d: %s\n", i, names[i]);
 *     free(names[i]);  // Don't forget to free the memory!
 * }
 */
void getAllNames(char** namesArray, uint8_t keysNumber);


/**
 * @brief Reads the overwrite flags of all keys stored in memory and stores them in an array.
 *
 * This function retrieves the overwrite flag (`0x80` bit) for each key stored in memory and stores the flags
 * in the provided boolean array (`owFlagArray`). It checks each key's flags and assigns `true` if the overwrite
 * flag is set, or `false` otherwise.
 *
 * @param owFlagArray Pointer to a boolean array where the overwrite flags of keys will be stored.
 * @param MAX_KEYS The maximum number of keys stored in memory to check.
 *
 * @note
 * - This function assumes that `MAX_KEYS` specifies the correct number of keys stored in memory.
 * - The function interacts with EEPROM via `eeprom_read` and assumes that the memory layout is consistent with
 *   the defined `JUMP_FROM_BEGIN_TO_FLAGS` and `KEY_FRAME_SIZE`.
 * - It checks for the overwrite flag at a specific position in each key's memory frame.
 *
 * @example
 * // Example usage:
 * bool owFlags[10];
 * getAllOWFlagArr(owFlags, 5);
 * for (int i = 0; i < 5; i++) {
 *     printf("Key %d overwrite flag: %s\n", i, owFlags[i] ? "True" : "False");
 * }
 */
void getAllOWFlagArr(bool* owFlagArray, const uint8_t MAX_KEYS);


/**
 * @brief Sets the overwrite flag for a key in memory.
 *
 * This function sets the overwrite flag (bit `0x80`) for a key stored in memory at the specified address.
 * The function first reads the current flag byte, then sets the overwrite flag, and writes the modified
 * byte back to the EEPROM memory.
 *
 * @param ADDR The memory address where the key's data is stored. The overwrite flag is located at a fixed
 *             offset within the key's memory frame.
 *
 * @note
 * - The overwrite flag is set by modifying the byte at a specific location (`JUMP_TO_FLAG_VALUE`) in the
 *   key's memory frame.
 * - If the EEPROM read or write operation fails, the `Error_Handler()` function is called to handle the error.
 *
 * @example
 * // Example usage:
 * uint8_t keyAddr = 0x10; // Example key address
 * setOWFlag(keyAddr);
 *
 * // This would set the overwrite flag for the key at address `keyAddr`.
 */
void setOWFlag(const uint8_t ADDR);


//==============================
//TEST Function - DON'T USE!
//==============================
void resetMemoryTest();
void addKeyTest();
//void getEncodedKeyFromUser();
//void getKeysNameFromUser();


#endif /* INC_MEMORYCONTROLLER_H_ */
