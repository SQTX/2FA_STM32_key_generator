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
char *SettingListPtr[] = {"Change module mode", "Set time", "Set timezone", "Back"};


//********************************************************************************************
// PUBLIC
//********************************************************************************************
uint8_t printOptions() {
	uint8_t currentOptNum = {0};

	while(1) {
		for(uint8_t i = 0; i < 10; i++) printf('\n');

		printf("Dostepne opcje:\n");
		printf("(Wybor opcji: W celu wyboru konkretnej opcji posluguj sie przyciskami Up i Down lub podaja konkretna opcje na klawiaturze.)\n");
		printf("(Akceptacja: W celu zaakceptowania wybranej opcji nacisnij przyciksk MAIN lub ENTER po wpisaniu wartosci na klawiaturze.)\n");
		for(uint8_t i = 0; i < OPTIONS_NUMBER; i++) {
			if( currentOptNum != NONE && currentOptNum == (i+1) ) printf("->");		// Active options
			printf("\t* %s\n", OptionsListPtr[i]);
		}
		printf("\n");

		while(1) {
			HAL_Delay(130);

			if(HAL_GPIO_ReadPin(USER_BTN1_GPIO_Port, USER_BTN1_Pin) == GPIO_PIN_RESET) {
				if(currentOptNum > 0 && currentOptNum <= OPTIONS_NUMBER) {
					printf("Wybrana opcja: (%u) %s\n", currentOptNum, OptionsListPtr[currentOptNum-1]);
					return currentOptNum;
				} else {
					printf("Wybrana opcja (%u) jest niepoprawna!\n", currentOptNum);
					return 0;
				}

			} else if(HAL_GPIO_ReadPin(GPIOC, USER_BTN_DOWN_Pin) == GPIO_PIN_RESET) {
//				Set next option:
				if(currentOptNum < OPTIONS_NUMBER) currentOptNum++;
				printf("[DEBUG] %u\n", currentOptNum);
				break;
			} else if(HAL_GPIO_ReadPin(GPIOC, USER_BTN_UP_Pin) == GPIO_PIN_RESET) {
//				Set previous option:
				if(currentOptNum > 0) currentOptNum--;
				printf("[DEBUG] %u\n", currentOptNum);
				break;
			}
		}
	}
}


uint8_t printSettings() {
	uint8_t currentSetNum = {0};

	while(1) {
		HAL_Delay(130);

		for(uint8_t i = 0; i < 10; i++) printf("\n");

		printf("Dostepne ustawienia:\n");
		printf("(Wybor opcji: W celu wyboru konkretnej opcji posluguj sie przyciskami Up i Down lub podaja konkretna opcje na klawiaturze.)\n");
		printf("(Akceptacja: W celu zaakceptowania wybranej opcji nacisnij przyciksk MAIN lub ENTER po wpisaniu wartosci na klawiaturze.)\n");
		for(uint8_t i = 0; i < SETTINGS_NUMBER; i++) {
			if( currentSetNum != SET_NONE && currentSetNum == (i+1) ) printf("->");		// Active options
			printf("\t* %s\n", SettingListPtr[i]);
		}
		printf("\n");

		while(1) {
			if(HAL_GPIO_ReadPin(USER_BTN1_GPIO_Port, USER_BTN1_Pin) == GPIO_PIN_RESET) {
				if(currentSetNum > 0 && currentSetNum <= SETTINGS_NUMBER) {
					printf("Wybrane ustawienie: (%u) %s\n", currentSetNum, SettingListPtr[currentSetNum-1]);
					return currentSetNum;
				} else {
					printf("Wybrane ustawienie (%u) jest niepoprawne!\n", currentSetNum);
					return 0;
				}

			} else if(HAL_GPIO_ReadPin(GPIOC, USER_BTN_DOWN_Pin) == GPIO_PIN_RESET) {
//				Set next setting:
				if(currentSetNum < SETTINGS_NUMBER) currentSetNum++;
				printf("[DEBUG] %u\n", currentSetNum);
				break;
			} else if(HAL_GPIO_ReadPin(GPIOC, USER_BTN_UP_Pin) == GPIO_PIN_RESET) {
//				Set previous setting:
				if(currentSetNum > 0) currentSetNum--;
				printf("[DEBUG] %u\n", currentSetNum);
				break;
			}
		}
	}
}

