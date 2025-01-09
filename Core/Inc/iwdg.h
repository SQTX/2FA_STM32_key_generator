/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    iwdg.h
  * @brief   This file contains all the function prototypes for
  *          the iwdg.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IWDG_H__
#define __IWDG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern IWDG_HandleTypeDef hiwdg;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_IWDG_Init(void);

/* USER CODE BEGIN Prototypes */

/**
 * @brief Periodically resets the watchdog timer to prevent system reset and toggles an indicator LED.
 *
 * This function checks the elapsed time since the last watchdog reset. If at least 2000 ms have passed, it toggles
 * an LED to provide a visual indicator and refreshes the watchdog timer, preventing the system from resetting.
 * The function then updates the `prevWatchDogReset` timestamp to the current time.
 *
 * @param[in,out] prevWatchDogReset Pointer to a variable storing the last reset timestamp.
 *                                  Updated to the current time after each reset.
 *
 * @note Ensure that `prevWatchDogReset` points to a volatile variable, as it is modified in real-time by this function.
 *
 * @warning This function assumes that the IWDG (Independent Watchdog) is initialized and enabled. Proper setup is necessary for the function to work as expected.
 */
void resetWatchDog(volatile uint32_t *prevWatchDogReset);


/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __IWDG_H__ */

