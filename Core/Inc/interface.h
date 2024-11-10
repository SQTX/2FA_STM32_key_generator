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


uint8_t printOptions();


uint8_t printSettings();


#endif /* INC_INTERFACE_H_ */
