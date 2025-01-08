/*
 * interface.c
 *
 *  Created on: Nov 9, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#include "interface.h"


const uint8_t OPTIONS_NUMBER = 6;
char *OptionsListPtr[] = {"Show keys list", "Change active key", "Add new key", "Delete key", "Settings", "Back"};

const uint8_t SETTINGS_NUMBER = 4;
char *SettingListPtr[] = {"Change module mode", "Set time", "Set time zone", "Back"};

const uint8_t WORKING_MODE_NUMBER = 2;
char *WorkingModeListPtr[] = {"PASSIVE", "ACTIVE"};

enum ListType {
	TYPE_OPTIONS = 0,
	TYPE_SETTINGS,
	TYPE_WORK_MODE,
};


void printControlInfo(int);

//********************************************************************************************
// PUBLIC
//********************************************************************************************
uint8_t printOptions() {
	uint8_t currentOptNum = {0};

	while(1) {
		clearTerminal();
		printControlInfo(TYPE_OPTIONS);

		for(uint8_t i = 0; i < OPTIONS_NUMBER; i++) {
			if( currentOptNum != NONE && currentOptNum == (i+1) ) printf("->");		// Active options
			printf("\t* %s\n", OptionsListPtr[i]);
		}
		printf("\n");

		while(1) {
			HAL_Delay(130);

			if(HAL_GPIO_ReadPin(USER_BTN1_GPIO_Port, USER_BTN1_Pin) == GPIO_PIN_RESET) {
				if(currentOptNum > 0 && currentOptNum <= OPTIONS_NUMBER) {
					printf("Selected option: %u-%s\n", currentOptNum, OptionsListPtr[currentOptNum-1]);
					return currentOptNum;
				} else {
					printf("The selected option [%u] is invalid.\n", currentOptNum);
					return 0;
				}

			} else if(HAL_GPIO_ReadPin(GPIOC, USER_BTN_DOWN_Pin) == GPIO_PIN_RESET) {
//				Set next option:
				if(currentOptNum < OPTIONS_NUMBER) currentOptNum++;
//				printConsolePostfix(PRI_DEBUG);
//				printf("%u\n", currentOptNum);
				break;
			} else if(HAL_GPIO_ReadPin(GPIOC, USER_BTN_UP_Pin) == GPIO_PIN_RESET) {
//				Set previous option:
				if(currentOptNum > 0) currentOptNum--;
//				printConsolePostfix(PRI_DEBUG);
//				printf("%u\n", currentOptNum);
				break;
			}
		}
	}
}


uint8_t printSettings() {
	uint8_t currentSetNum = {0};

	while(1) {
		HAL_Delay(130);

		clearTerminal();
		printControlInfo(TYPE_SETTINGS);

		for(uint8_t i = 0; i < SETTINGS_NUMBER; i++) {
			if( currentSetNum != SET_NONE && currentSetNum == (i+1) ) printf("->");		// Active options
			printf("\t* %s\n", SettingListPtr[i]);
		}
		printf("\n");

		while(1) {
			if(HAL_GPIO_ReadPin(USER_BTN1_GPIO_Port, USER_BTN1_Pin) == GPIO_PIN_RESET) {
				if(currentSetNum > 0 && currentSetNum <= SETTINGS_NUMBER) {
					printf("Selected setting: %u-%s\n", currentSetNum, SettingListPtr[currentSetNum-1]);
					return currentSetNum;
				} else {
					printConsolePostfix(PRI_WARNING);
					printf("The selected setting [%u] is invalid.\n", currentSetNum);
					return 0;
				}

			} else if(HAL_GPIO_ReadPin(GPIOC, USER_BTN_DOWN_Pin) == GPIO_PIN_RESET) {
//				Set next setting:
				if(currentSetNum < SETTINGS_NUMBER) currentSetNum++;
//				printConsolePostfix(PRI_DEBUG);
//				printf("%u\n", currentSetNum);
				break;
			} else if(HAL_GPIO_ReadPin(GPIOC, USER_BTN_UP_Pin) == GPIO_PIN_RESET) {
//				Set previous setting:
				if(currentSetNum > 0) currentSetNum--;
//				printConsolePostfix(PRI_DEBUG);
//				printf("%u\n", currentSetNum);
				break;
			}
		}
	}
}


uint8_t printWrokingMode() {
	uint8_t currentWorkNum = {0};

	while(1) {
		HAL_Delay(130);

		clearTerminal();
		printControlInfo(TYPE_SETTINGS);

		for(uint8_t i = 0; i < WORKING_MODE_NUMBER; i++) {
			if( currentWorkNum != SET_NONE && currentWorkNum == (i+1) ) printf("->");		// Active options
			printf("\t* %s\n", WorkingModeListPtr[i]);
		}
		printf("\n");

		while(1) {
			if(HAL_GPIO_ReadPin(USER_BTN1_GPIO_Port, USER_BTN1_Pin) == GPIO_PIN_RESET) {
				if(currentWorkNum > 0 && currentWorkNum <= WORKING_MODE_NUMBER) {
					printf("Selected working mode: %u-%s\n", currentWorkNum, WorkingModeListPtr[currentWorkNum-1]);
					return currentWorkNum;
				} else {
					printConsolePostfix(PRI_WARNING);
					printf("The selected mode [%u] is invalid.\n", currentWorkNum);
					return 0;
				}

			} else if(HAL_GPIO_ReadPin(GPIOC, USER_BTN_DOWN_Pin) == GPIO_PIN_RESET) {
//				Set next setting:
				if(currentWorkNum < WORKING_MODE_NUMBER) currentWorkNum++;
//				printConsolePostfix(PRI_DEBUG);
//				printf("%u\n", currentSetNum);
				break;
			} else if(HAL_GPIO_ReadPin(GPIOC, USER_BTN_UP_Pin) == GPIO_PIN_RESET) {
//				Set previous setting:
				if(currentWorkNum > 0) currentWorkNum--;
//				printConsolePostfix(PRI_DEBUG);
//				printf("%u\n", currentSetNum);
				break;
			}
		}
	}
}

//********************************************************************************************
// PRIVATE
//********************************************************************************************
void printControlInfo(int infoType) {
	switch(infoType) {
		case TYPE_OPTIONS:
			printf("List of available options:\n");
			break;
		case TYPE_SETTINGS:
			printf("List of available settings:\n");
			break;
		case TYPE_WORK_MODE:
			printf("List of available modes:\n");
			break;
		default:
			printConsolePostfix(PRI_ERROR);
			printf("The argument in the function printing option information does not exist.");
			break;
	}
	printf("* To select a specific option, use the Up and Down buttons or enter the number of a specific option using the keyboard.\n");
	printf("* To confirm the selected option, press the MAIN button or (in the case of a keyboard, confirm the value with the ENTER key)\n");
}


