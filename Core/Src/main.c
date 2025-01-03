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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dataConverter.h"
#include "keyGenerator.h"
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
  /* USER CODE BEGIN 2 */
//  ************************************************************************
//  Stale:
//  const uint32_t TOTP_TIMESTAMP = 30;
//  const uint32_t EPOCH_TIMESTAMP = 1557414000;	// Hard coded Timestamp
  const int8_t TIMEZONE = 2;		// In Poland: UTC+2h (local_time_hour - 2h)

//  ************************************************************************
//  Pobranie czasu:
  struct tm datetime;
//  27.09.2024 - 16:35:00
//  datetime.tm_hour = 16;      	// Moja godzina
//  datetime.tm_min = 35;           // Moja minut
//  datetime.tm_sec = 0;            // Moja sekunda
//  datetime.tm_mday = 27;          // Mój dzień
//  datetime.tm_mon = (9-1);        // Mój miesiąc -1m
//  datetime.tm_year = (2024-1900); // Mój rok -1900y

//  Current hard coded time:
  datetime.tm_hour = (15 - TIMEZONE);  // Moja godzina
  datetime.tm_min = 49;          	 // Moja minut
  datetime.tm_sec = 0;           	 // Moja sekunda
  datetime.tm_mday = 13;         	 // Mój dzień
  datetime.tm_mon = (10-1);       	 // Mój miesiąc -1m
  datetime.tm_year = (2024-1900); 	 // Mój rok -1900y

    time_t t = mktime(&datetime);   					// Convert datatime to timestamp
    const uint32_t EPOCH_TIMESTAMP = (uint32_t) t;		// Casting from time_t to uint32
    printf("Epoch timestamp: %ld\n", EPOCH_TIMESTAMP);

//  ************************************************************************
//  Conversion Encoded key to normal key
//  ************************************************************************
//	const char encoded[] = "JV4UYZLHN5CG633S";		// Encoded key is a word
	const char encoded[] = "JBSWY3DPEHPK3PXP";		// Encoded key isn't a word
	uint8_t encodedLength = {(sizeof encoded)-1};	// Encoded key length (without '\n')
	printf("Code: %s\t| Size: %u\n", encoded, encodedLength);


	uint8_t *key = NULL;
	uint8_t keySize = base32ToHex(encoded, encodedLength, &key);
	if(key == NULL) printf("[ERROR] Failed to allocate memory.\n");


	printf("Token size: %u\n", keySize);
	printf("Dekodowany tekst: ");
	for (int i = 0; i < keySize; i++) {
//		printf("%c", key[i]);
	    printf("0x%x ", key[i]);
	}
	printf("\n");

//  ************************************************************************
//  Generate TOTP TOKEN
//  ************************************************************************
    uint32_t token = generateToken(key, keySize, EPOCH_TIMESTAMP);
    free(key);
    printf("Code:\t\t%lu\n", token);
//  ************************************************************************
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
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
