/*
 * interface.h
 *
 *  Created on: Nov 9, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#ifndef INC_INTERFACE_H_
#define INC_INTERFACE_H_

//Std:
#include <stdio.h>
#include <stdbool.h>
//Stm:
#include "stm32l4xx.h"
#include "iwdg.h"
#include "gpio.h"
//My:
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


/**
 * @brief Displays a list of selectable options in the terminal and captures user input to navigate and select an option.
 *
 * This function continuously displays a list of options in the terminal and updates the display to show the currently
 * highlighted option. It refreshes the watchdog timer periodically to prevent system reset during the selection process.
 * The user can navigate through options using up and down buttons and confirm their selection with the select button.
 *
 * @param[in,out] prevWatchDogReset Pointer to a variable storing the last reset timestamp for the watchdog. Updated
 *                                  periodically within the function to maintain the watchdog's activity.
 *
 * @return uint8_t The number of the selected option, or 0 if an invalid option is selected.
 *
 * @note The function will continue running until a valid option is selected or the user exits by pressing the select button.
 *
 * @warning Ensure that `prevWatchDogReset` is properly initialized and `resetWatchDog` is configured to prevent
 *          system reset during user interaction.
 *
 * @dependencies
 * - `resetWatchDog`: Called periodically to refresh the watchdog timer.
 * - `clearTerminal`: Clears the terminal screen for re-rendering options.
 * - `printControlInfo`: Displays instructional information at the top of the terminal.
 * - HAL GPIO functions: Reads GPIO states to capture button presses for navigation.
 */
uint8_t printOptions(volatile uint32_t *prevWatchDogReset);


/**
 * @brief Displays a list of settings in the terminal and captures user input to navigate and select a setting.
 *
 * This function renders a menu of settings in the terminal, allowing the user to navigate through available settings
 * and make a selection. It refreshes the watchdog timer periodically to prevent a system reset during user interaction.
 * Navigation through settings is achieved using up and down buttons, and the selection is confirmed with the select button.
 *
 * @param[in,out] prevWatchDogReset Pointer to a variable that stores the last reset timestamp for the watchdog timer.
 *                                  This is updated within the function to ensure the watchdog remains active.
 *
 * @return uint8_t The number of the selected setting, or 0 if an invalid setting is chosen.
 *
 * @note The function operates in a continuous loop, re-rendering the settings until a valid selection is confirmed.
 *
 * @warning Ensure that `prevWatchDogReset` is properly initialized before calling this function.
 *          An improperly reset watchdog could result in an unintentional system reset during execution.
 *
 * @dependencies
 * - `resetWatchDog`: Periodically called within the function to reset the watchdog timer.
 * - `clearTerminal`: Clears the terminal screen for updating the settings display.
 * - `printControlInfo`: Displays instructional information regarding available settings.
 * - HAL GPIO functions: Used to capture button presses for navigating through settings.
 */
uint8_t printSettings(volatile uint32_t *prevWatchDogReset);


/**
 * @brief Displays a list of working modes in the terminal and captures user input to navigate and select a mode.
 *
 * This function displays a menu of available working modes in the terminal, allowing the user to navigate using
 * buttons and make a selection. It periodically resets the watchdog timer to prevent unintended system resets
 * while the user is making a choice.
 *
 * @param[in,out] prevWatchDogReset Pointer to a variable that stores the last reset timestamp for the watchdog timer.
 *                                  This is updated within the function to ensure the watchdog remains active.
 *
 * @return uint8_t The number of the selected working mode, or 0 if an invalid mode is chosen.
 *
 * @note The function will continue displaying the list of modes until a valid selection is confirmed by the user.
 *
 * @warning Ensure that `prevWatchDogReset` is correctly initialized before calling this function,
 *          as an unhandled watchdog reset could cause an unintended system reset.
 *
 * @dependencies
 * - `resetWatchDog`: Called periodically to reset the watchdog timer.
 * - `clearTerminal`: Clears the terminal screen for re-rendering the working mode list.
 * - `printControlInfo`: Displays instructional information on how to use the working mode selection.
 * - HAL GPIO functions: Used to capture button presses for navigation.
 */
uint8_t printWrokingMode(volatile uint32_t *prevWatchDogReset);


#endif /* INC_INTERFACE_H_ */
