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

#include "sha1.h"
//#include "base32.h"
#include "TOTP.h"
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
//#include <stdio.h>
#include <stdint.h>
#include <string.h>
//#include <ctype.h>

#define BASE32_ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"

// Funkcja pomocnicza do zamiany znaku na wartość z alfabetu BASE32
int base32_char_to_value(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    } else if (c >= '2' && c <= '7') {
        return c - '2' + 26;
    } else {
        return -1; // Znak nieprawidłowy
    }
}

// Funkcja dekodująca BASE32
int base32_decode(const char *encoded, uint8_t *output, size_t output_size) {
    size_t input_length = strlen(encoded);
    size_t i, bit_index = 0, buffer = 0, output_index = 0;

    for (i = 0; i < input_length; i++) {
        // Ignoruj spacje i małe litery zamień na wielkie
        char c = toupper(encoded[i]);

        if (c == '=') {
            break; // Padding - ignorujemy resztę danych
        }

        int value = base32_char_to_value(c);
        if (value == -1) {
            return -1; // Zwróć błąd, jeśli napotkano nieprawidłowy znak
        }

        // Dodaj wartość do bufora
        buffer = (buffer << 5) | value;
        bit_index += 5;

        // Jeśli mamy co najmniej 8 bitów, zapisz bajt do outputu
        if (bit_index >= 8) {
            bit_index -= 8;
            if (output_index >= output_size) {
                return -1; // Brak miejsca w buforze wyjściowym
            }
            output[output_index++] = (buffer >> bit_index) & 0xFF;
        }
    }

    return output_index; // Zwraca liczbę zapisanych bajtów
}

const char *encoded = "JV4UYZLHN5CG633S";
uint8_t decoded[11];
memset(decoded, 0, sizeof(decoded));

int decoded_size = base32_decode(encoded, decoded, sizeof(decoded));
if (decoded_size < 0) {
    printf("Błąd dekodowania\n");
} else {
    printf("Dekodowany tekst: ");
    for (int i = 0; i < decoded_size; i++) {
        printf("%c", decoded[i]);
    }
    printf("\n");
}

//  unsigned char codeStr[] = "JV4UYZLHN5CG633S";  // 'MyLegoDoor' | 10
////  uint8_t codeStr = {0x4d, 0x79, 0x4c, 0x65, 0x67, 0x6f, 0x44, 0x6f, 0x6f, 0x72};
//  uint8_t plainStr[10] = {};
////  uint8_t plainStr[10] = {};
//
//  base32_decode(codeStr, plainStr, 10);
//
//  printf("Code: %s\n", plainStr);




//  uint8_t hmacKey[] = {0x4d, 0x79, 0x4c, 0x65, 0x67, 0x6f, 0x44, 0x6f, 0x6f, 0x72};               // Secret key
//  TOTP(hmacKey, 10, 30);
  TOTP(decoded, 10, 30);
  uint32_t newCode = getCodeFromTimestamp(1557414000);       // Current timestamp since Unix epoch in seconds// Secret key, Secret key length, Timestep (30s)
  printf("Code:\t\t%u\n", newCode);
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
