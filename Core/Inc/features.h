/*
 * features.h
 *
 *  Created on: Oct 26, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#ifndef INC_FEATURES_H_
#define INC_FEATURES_H_

//Std:
#include <stdbool.h>
//Stm:
#include "stm32l4xx.h"
#include "iwdg.h"
#include "usart.h"
//My:
#include "dataFromUser.h"
#include "dataConverter.h"
#include "base32.h"
#include "memoryController.h"
#include "printer.h"

#define MAX_KEYS_NAME_SIZE			5	// Name string
#define MAX_ENCODED_KEY_SIZE		21	// Encoded key string
#define MAX_KEY_SIZE				13	// Key string
#define KEY_FRAME_SIZE				22	// Size of single key's frame = 22 Bytes

#define PRINT_ENTRED_STRINGS		0	// 0 - OFF | 1 - ON


/**
 * @brief Adds a new key to the system's memory, encoding and saving its data along with metadata.
 *
 * This function retrieves a new key's name and encoded key data from the user, decodes it, calculates an index
 * for storage, and saves it in memory. It also manages the overwrite (OW) mode if the maximum number of keys is reached.
 *
 * @param[in,out] prevWatchDogReset Pointer to the last reset timestamp for the watchdog timer, updated periodically
 *                                  to prevent system resets during function execution.
 * @param[in] MAX_KEYS Maximum allowable number of keys in memory.
 * @param[in,out] keysNumber Pointer to the current count of keys stored in memory; incremented if a new key is added.
 * @param[in,out] generalFlags Pointer to the general flags byte, where the 0x80 bit indicates if overwrite (OW) mode is active.
 * @param[out] activeKeyAddr Pointer to the address where the new key is stored; updated to point to the latest key.
 *
 * @return int8_t Returns 0 on successful addition of a key, or -1 if adding a new key is not allowed due to memory constraints.
 *
 * @note The function enters overwrite (OW) mode when `keysNumber` equals `MAX_KEYS`, allowing the oldest keys to be overwritten.
 *
 * @dependencies
 * - `getKeysNameFromUser`, `getEncodedKeyFromUser`: Functions to get user input for key name and encoded key.
 * - `trimZeros`: Removes trailing zero bytes from arrays.
 * - `base32_decode`: Decodes the BASE32-encoded key into hexadecimal format.
 * - `makeKeyFrame`: Constructs a data frame for the key with metadata.
 * - `writeKeyInMemory`: Writes the key's data frame to memory.
 * - `writeGeneralDataInMemory`: Updates general data, including the overwrite mode status.
 *
 * @warning Ensure `MAX_KEYS` does not exceed memory limitations to avoid overwriting critical data.
 *          `prevWatchDogReset` must be initialized to maintain system stability.
 */
int8_t addNewKey(volatile uint32_t *prevWatchDogReset, const uint8_t MAX_KEYS, uint8_t* keysNumber, uint8_t* generalFlags, uint8_t* activeKeyAddr);


/**
 * @brief Searches for a key by name in memory and returns its address if found.
 *
 * This function prompts the user to enter the name of the key they are searching for, retrieves this key's data from memory
 * if it exists, and displays relevant information. If the key is not found, an informational message is displayed.
 *
 * @param[in,out] prevWatchDogReset Pointer to the last reset timestamp for the watchdog timer, updated periodically
 *                                  to prevent system resets during function execution.
 * @param[in] keysNumber The total number of keys stored in memory.
 * @param[out] keyAddr Pointer to store the memory address of the found key, if it exists.
 *
 * @return int8_t Returns 0 if the key is found, 1 if the key is not found, and -1 if an error occurs during the search process.
 *
 * @note This function relies on `readKeyFromMemory` to locate the key based on the provided name.
 *
 * @dependencies
 * - `getKeysNameFromUser`: Retrieves the key's name from the user.
 * - `readKeyFromMemory`: Searches memory for a key with the provided name and sets `wantedAddr` if found.
 * - `printLineSeparator`: Prints a separator line for console readability.
 * - `printConsolePostfix`: Adds prefix for logging levels (e.g., info, error).
 *
 * @warning The `prevWatchDogReset` pointer must be initialized properly for the watchdog reset mechanism.
 *          Ensure `keyAddr` is valid to store the address of the found key.
 */
int8_t searchKey(volatile uint32_t *prevWatchDogReset, uint8_t keysNumber, uint8_t* keyAddr);


/**
 * @brief Deletes a key from memory by name and updates active and general key information.
 *
 * This function prompts the user to enter the name of the key they wish to delete. If the key is found in memory,
 * it marks the key's memory location as overwritten (using an OverWrite flag) and updates the total number of keys.
 * If the deleted key was the active key, the active key pointer is reset to the first key in memory.
 *
 * @param[in,out] prevWatchDogReset Pointer to the last reset timestamp for the watchdog timer, updated during the function's execution.
 * @param[in] MAX_KEYS The maximum number of keys that can be stored in memory.
 * @param[in,out] keysNumber Pointer to the current count of keys, which is decremented if a key is deleted.
 * @param[in,out] generalFlags Pointer to the general flags register that tracks key settings and states, including the overwrite mode flag.
 * @param[out] activeKeyAddr Pointer to the active key's memory address, updated if the deleted key was active.
 *
 * @return int8_t Returns 0 if the key is successfully deleted, 1 if the key is not found, and -1 if an error occurs.
 *
 * @note This function relies on `readKeyFromMemory` to search for the key by name and uses `setOWFlag` to mark the key as deleted.
 *
 * @dependencies
 * - `getKeysNameFromUser`: Retrieves the name of the key to be deleted from the user.
 * - `readKeyFromMemory`: Searches memory for the key and provides its address if found.
 * - `setOWFlag`: Marks the key's memory location as available for overwrite.
 * - `writeGeneralDataInMemory`: Updates general data, including key count and active key information, in memory.
 *
 * @warning Ensure `prevWatchDogReset` is properly initialized to avoid unnecessary system resets.
 *          Active key pointer (`activeKeyAddr`) should be monitored, as deleting the active key can affect system functionality.
 */
int8_t deleteKey(volatile uint32_t *prevWatchDogReset, const uint8_t MAX_KEYS, uint8_t* keysNumber, uint8_t* generalFlags, uint8_t* activeKeyAddr);


/**
 * @brief Displays a list of stored key names in memory.
 *
 * This function retrieves and displays the names of all stored keys by dynamically allocating an array of string pointers.
 * The names are loaded from memory using `getAllNames`, then printed to the console. Memory allocated for each key name and
 * for the array of pointers is freed after use.
 *
 * @param[in] keysNumber The total number of keys currently stored in memory.
 *
 * @note This function relies on `getAllNames` to populate `wordsArray` with key names.
 *
 * @dependencies
 * - `getAllNames`: Retrieves all key names from memory and assigns them to `wordsArray`.
 *
 * @warning Ensure that `keysNumber` accurately reflects the number of keys in memory, as incorrect values may cause
 *          segmentation faults or undefined behavior.
 *
 * @remark If memory allocation fails, an error message is displayed, and the function exits without attempting to display keys.
 */
void showKeysList(uint8_t keysNumber);


#endif /* INC_FEATURES_H_ */
