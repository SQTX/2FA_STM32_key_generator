/*
 * interface.h
 *
 *  Created on: Nov 9, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#ifndef INC_INTERFACE_H_
#define INC_INTERFACE_H_

#include <stdio.h>
#include <stdbool.h>
#include "stm32l4xx.h"
//#include "iwdg.h"
#include "gpio.h"
#include "printer.h"


typedef enum {
	NONE = 0,
	KEYS_LIST,
	CHANGE_KEY,
	ADD_KEY,
	DELETE_KEY,
	SETTINGS,
	BACK
} MainOptions;


typedef enum {
	SET_NONE = 0,
	SET_CHANGE_MODE,
	SET_CHANGE_TIME,
	SET_CHANGE_TIMEZONE,
	SET_BACK
} SettingOptions;


typedef enum {
	WORK_NONE = 0,
	WORK_PASSIVE,		// PASSIVE_MODE - the device itself generates a token every 0 and 30 seconds
	WORK_ACTIVE			// ACTIVE_MODE  - the device generates a token only when the button is pressed
} WorkingMode;


uint8_t printOptions(volatile uint32_t *prevWatchDogReset);

uint8_t printSettings(volatile uint32_t *prevWatchDogReset);

uint8_t printWrokingMode(volatile uint32_t *prevWatchDogReset);


#endif /* INC_INTERFACE_H_ */
