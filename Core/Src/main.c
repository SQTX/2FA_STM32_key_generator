/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "i2c.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//Std:
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
//My:
#include "clockControl.h"
#include "dataConverter.h"
#include "keyGenerator.h"
#include "dataFromUser.h"
#include "eeprom.h"
#include "memoryController.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef struct tm DateTime_t;

//********************************************************************************************
//CONSTS
//********************************************************************************************
//const char encoded[] = {"JV4UYZLHN5CG633S"};	// Encoded key is a word
//const char encodedKey[] = {"JBSWY3DPEHPK3PXP"};	// Encoded key isn't a word


//********************************************************************************************
//GLOBAL VARIABLES (RAM)
//********************************************************************************************
RTC_TimeTypeDef rtcTime = {0};
RTC_DateTypeDef rtcDate = {0};

uint8_t* key = {NULL};
char* name = {NULL};
uint8_t keySize = {0};
uint8_t nameSize = {0};


//********************************************************************************************
//Token generator functions:
//********************************************************************************************
//void addKey() {
//	printf("Wpisz nowy klucz: \n");
////	keyBase32 = typeText();
//	char keyBase32[] = {"JBSWY3DPEHPK3PXP"};
//
//	uint8_t keyBase32Length = {(sizeof keyBase32)-1};					// Encoded key length (without '\n')
//	uint8_t *key = {NULL};												// Empty pointer for key array
//	uint8_t keySize = {base32ToHex(keyBase32, keyBase32Length, &key)};	// Convert key from BASE32 to Hex
//	if(key == NULL) printf("[ERROR] Failed to allocate memory.\n");		// ERROR alert
////	Print key in HEX:
//	printf("Dekodowany tekst: ");
//	for (int i = 0; i < keySize; i++) {
//	    printf("0x%x ", key[i]);
//	}
//	printf("\n");
//
//
//	printf("Nadaj kluczowi nazwe: \n");
////	keyName = typeText()
////	uint8_t keyNameLength = {(sizeof keyName)-1};
//
////	TODO: CRC
//
////	saveKeyInMemory(key, keySize, keyName, keyNameLength)
//}
//
//
//void getKey(keyName) {
////	keySize = getKeyFromMemory(keyName, key);
////	if (keySize = (sizeof keyBase32)-1)
////		retun key
//}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_CRC_Init();
  MX_RTC_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
//  resetMemoryTest();		// TEST FUNCTION
//  addKeyTest();			// TEST FUNCTION

  printf("===========================================================\n");
  printf("Device initialization process\n");
  printf("-----------------------------------------------------------\n");
//  ********************************************************************************************
//  Initializing the Memory
//  ********************************************************************************************
  if(checkMemory()){
	  printf("Memory status\t\t\t[OK]\n");
  } else {
	  printf("Memory status\t\t\t[WRONG]\n");
	  printf("Memory initializing...\n");
	  if(initMemory(128)){
		  printf("Memory initialize\t\t[OK]\n");
//		  TODO: ADD NEW, first KEY
	  } else {
		  printf("Memory initialize\t\t[ERROR]\n");
	  }
  }

//  ********************************************************************************************
//  Initializing the RTC clock
//  ********************************************************************************************
  initClockRTC(&rtcTime, &rtcDate);											// Initializing the RTC clock


//  ********************************************************************************************
//  Get general data from memory
//  ********************************************************************************************
  uint8_t generalData[6] = {0};
  readGeneralDataFromMemory(generalData);

  const uint8_t MAX_KEYS = generalData[2];
  uint8_t keysNumber = generalData[3];
  uint8_t currentKeyAddr = generalData[4];

  printf("-----------------------------------------------------------\n");
  printf("General information: \n");
  printf("-----------------------------------------------------------\n");
  printLocalTime(&rtcTime, &rtcDate);
  printf("Epoch TimeStamp:\t\t[%ld]\n", getTimeStamp(&rtcTime, &rtcDate));	// Show current epoch TimeStamp
  printf("Maximum keys in memory:\t\t[%u]\n", MAX_KEYS);
  printf("Current number of keys:\t\t[%u]\n", keysNumber);


//  ********************************************************************************************
//  Get default/last used key from memory
//  ********************************************************************************************
  uint8_t currentKey[13] = {0};		// Container for key from memory
  uint8_t currentKeyName[5] = {0};	// Container for name from memory
  uint8_t data[13+5] = {0};			// Container for all data from memory

  readKeyFromMemory(data, keysNumber, currentKeyAddr, NULL);


  for(int i = 0; i < 13; i++) currentKey[i] = data[i];			//  Get key from data
  for(int i = 0; i < 5; i++) currentKeyName[i] = data[i+13];	//  Get name from data


//  Trim zeros and optimization arrays:
  keySize = trimZeros(currentKey, 13);
  nameSize = trimZeros(currentKeyName, 5);

//  Dynamic allocated key array:
  key = (uint8_t*) malloc( keySize * sizeof(uint8_t) );
  for(int i = 0; i < keySize; i++) {
	  key[i] = currentKey[i];
  }
//  Dynamic allocated name array:
  name = (char*) malloc( (nameSize+1) * sizeof(char) );
  for(int i = 0; i < nameSize; i++) {
	  name[i] = (char)currentKeyName[i];
  }
  name[nameSize] = '\0';


  printf("Current used key:\t\t['%s']\n", name);				// Print current used key name
//  printf("Key: ");
//  for(int i = 0; i < 13; i++) printf("%x ", currentKey[i]);
//  printf("\n");

  printf("===========================================================\n");

  /*
//  ********************************************************************************************
//  Conversion Encoded key to normal key
//  ********************************************************************************************
	uint8_t encodedLength = {(sizeof encodedKey)-1};					// Encoded key length (without '\n')
	printf("Code: %s\t| Size: %u\n", encodedKey, encodedLength);


	uint8_t *key = {NULL};												// Empty pointer for key array
	uint8_t keySize = {base32ToHex(encodedKey, encodedLength, &key)};	// Convert key from BASE32 to Hex
	if(key == NULL) printf("[ERROR] Failed to allocate memory.\n");		// ERROR alert


	printf("Token size: %u\n", keySize);
	printf("Dekodowany tekst: ");
	for (int i = 0; i < keySize; i++) {
//		printf("%c", key[i]);
	    printf("0x%x ", key[i]);
	}
	printf("\n");
*/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//  printf("-----------------------------------------------------------\n");

  while (1)
  {
//	  ********************************************************************************************
//	  Generate TOTP TOKEN
//	  ********************************************************************************************
//	  TODO: Use an interrupt
	  uint32_t token = {generateToken(key, keySize, getTimeStamp(&rtcTime, &rtcDate))};	// Generate TOTP token
	  printf("Token:\t\t[%06lu]\n", token);


	  HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);
	  HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
	  uint8_t nowSeconds = rtcTime.Seconds;
	  uint8_t secToRegenerate = {0};

	  if (nowSeconds < 30) {
		  secToRegenerate = 30 - nowSeconds;
	  } else if (nowSeconds > 30 && nowSeconds < 60) {
		  secToRegenerate = 60 - nowSeconds;
	  } else {
		  secToRegenerate = 30;
	  }

	  HAL_Delay(secToRegenerate*1000);	// New token in 0' and 30' second
//	  HAL_Delay(30000);					// New token every 30sec
//	  ********************************************************************************************
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  free(key);
  free(name);
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
