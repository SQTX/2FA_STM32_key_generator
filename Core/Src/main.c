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
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//Std:
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//My:
#include "dataConverter.h"
#include "keyGenerator.h"
#include "dataFromUser.h"
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
const int8_t TIMEZONE = {2};					// Poland: UTC+2h (local_time_hour - 2h)

//const char encoded[] = {"JV4UYZLHN5CG633S"};	// Encoded key is a word
const char encoded[] = {"JBSWY3DPEHPK3PXP"};	// Encoded key isn't a word


RTC_TimeTypeDef rtcTime = {0};
RTC_DateTypeDef rtcDate = {0};
//********************************************************************************************
//Functions:
//********************************************************************************************
/**
 * @brief Prompts the user to enter a date and time, then converts it to a UTC timestamp.
 *
 * This function prompts the user to input a date and time in the format "DD-MM-YYYY,hh:mm:ss" via UART.
 * It collects the input character by character and parses it using the @p getDataTimeViaKeyboard function.
 * After receiving valid input, the function prints both the local time and the UTC time (adjusted for timezone),
 * and returns the UTC timestamp.
 *
 * @param[out] dataTimePtr  Pointer to a @p DateTime_t structure that will store the parsed date and time.
 *
 * @return Returns the UTC timestamp as a 32-bit unsigned integer upon successful input.
 *         Returns 0 if no valid timestamp could be retrieved.
 *
 * @note The function expects the UART interface (e.g., @p huart2) to be properly configured beforehand.
 *
 * @warning The input must be in the correct format for the function to work as expected. The time zone adjustment
 *          is applied based on the @p TIMEZONE constant.
 */
uint32_t getTimeFromUser() {
	DateTime_t datetime;

	uint8_t flag = {1};

	printf("Enter your current time [DD-MM-YYYY,hh:mm:ss]:\n");
	fflush(stdout);
	while(flag != 0) {
		uint8_t value = {0};

		if (HAL_UART_Receive(&huart2, &value, 1, 0) == HAL_OK) {
			// TODO: Try to print out each char inserted
			flag = getDataTimeViaKeyboard(value, &datetime);

			if (flag == 0) {
//				time_t localTimestamp = mktime(dataTimePtr);
				printf("Local:\t %d-%d-%d,", datetime.tm_mday, datetime.tm_mon + 1, datetime.tm_year + 1900);
				printf("%d:%d:%d\n", datetime.tm_hour, datetime.tm_min, datetime.tm_sec);

				datetime.tm_hour -= TIMEZONE;
				time_t utcTimestamp = mktime(&datetime);
				printf("UTC:\t %d-%d-%d,", datetime.tm_mday, datetime.tm_mon + 1, datetime.tm_year + 1900);
				printf("%d:%d:%d\n", datetime.tm_hour, datetime.tm_min, datetime.tm_sec);

//				Set time in RTC:
				rtcTime.Hours = datetime.tm_hour;
				rtcTime.Minutes = datetime.tm_min;
				rtcTime.Seconds = datetime.tm_sec;
				HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
				rtcDate.Year = datetime.tm_year + 1900 - 2000;
				rtcDate.Month = datetime.tm_mon + 1;
				rtcDate.Date = datetime.tm_mday;
				HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);

//				printf("EPOCH TimeStamp: %ld\n", (uint32_t)utcTimestamp);
				return (uint32_t)utcTimestamp;
			}
		}
	}
	return 0;
}


uint8_t initClockRTC() {
	HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);

	if(rtcDate.Year == 0 && rtcTime.Hours == 0) {
		printf("RTC\t[not configured]\n");
		getTimeFromUser();
		return 1;
	}
	printf("RTC\t[OK]\n");
	printf("Your local time: [%02d-%02d-%04d, %02d:%02d:%02d]\n", rtcDate.Date, rtcDate.Month, rtcDate.Year + 2000, rtcTime.Hours + TIMEZONE, rtcTime.Minutes, rtcTime.Seconds);
	return 0;
}


uint32_t getTimeStamp() {
	HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);


	DateTime_t currentTime;
	currentTime.tm_sec = rtcTime.Seconds;
	currentTime.tm_min = rtcTime.Minutes;
	currentTime.tm_hour = rtcTime.Hours;

	currentTime.tm_mday = rtcDate.Date;
	currentTime.tm_mon = rtcDate.Month - 1;
	currentTime.tm_year = (rtcDate.Year + 2000) - 1900;


	time_t utcTimestamp = mktime(&currentTime);
	return (uint32_t)utcTimestamp;
}
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
  /* USER CODE BEGIN 2 */
//  ********************************************************************************************
//  Initializing the RTC clock
//  ********************************************************************************************
  printf("-----------------------------------------------------------\n");
  initClockRTC();
  printf("Epoch TimeStamp: %ld\n", getTimeStamp());					// Show current epoch timestamp

//  ********************************************************************************************
//  Conversion Encoded key to normal key
//  ********************************************************************************************
	uint8_t encodedLength = {(sizeof encoded)-1};					// Encoded key length (without '\n')
	printf("Code: %s\t| Size: %u\n", encoded, encodedLength);


	uint8_t *key = {NULL};											// Empty pointer for key array
	uint8_t keySize = {base32ToHex(encoded, encodedLength, &key)};	// Convert key from BASE32 to Hex
	if(key == NULL) printf("[ERROR] Failed to allocate memory.\n");	// ERROR alert


	printf("Token size: %u\n", keySize);
	printf("Dekodowany tekst: ");
	for (int i = 0; i < keySize; i++) {
//		printf("%c", key[i]);
	    printf("0x%x ", key[i]);
	}
	printf("\n");
//  ********************************************************************************************
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  printf("-----------------------------------------------------------\n");

  while (1)
  {
//	  ********************************************************************************************
//	  Generate TOTP TOKEN
//	  ********************************************************************************************
//	  TODO: Use an interrupt
	  uint32_t token = {generateToken(key, keySize, getTimeStamp())};	// Generate TOTP token
	  printf("Code:\t\t%06lu\n", token);


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
