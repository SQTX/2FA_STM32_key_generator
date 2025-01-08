/*
 * features.h
 *
 *  Created on: Oct 26, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#ifndef INC_FEATURES_H_
#define INC_FEATURES_H_

#include <stdbool.h>
#include "stm32l4xx.h"
#include "usart.h"
#include "dataFromUser.h"
#include "dataConverter.h"
#include "base32.h"
#include "memoryController.h"


#define MAX_KEYS_NAME_SIZE			5	// Name string
#define MAX_ENCODED_KEY_SIZE		21	// Encoded key string
#define MAX_KEY_SIZE				13	// Key string
#define KEY_FRAME_SIZE				22	// Size of single key's frame = 22 Bytes

#define PRINT_ENTRED_STRINGS		0	// 0 - OFF | 1 - ON


int8_t addNewKey(volatile uint32_t *prevWatchDogReset, const uint8_t MAX_KEYS, uint8_t* keysNumber, uint8_t* generalFlags);

int8_t searchKey(volatile uint32_t *prevWatchDogReset, uint8_t keysNumber, uint8_t* keyAddr);

int8_t deleteKey(volatile uint32_t *prevWatchDogReset, uint8_t* keysNumber);

void showKeysList(uint8_t keysNumber);

#endif /* INC_FEATURES_H_ */
