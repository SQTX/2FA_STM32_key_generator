/*
 * clockControl.h
 *
 *  Created on: Oct 16, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#ifndef INC_CLOCKCONTROL_H_
#define INC_CLOCKCONTROL_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "usart.h"
#include "rtc.h"
#include "stm32l4xx.h"
#include "dataFromUser.h"


typedef struct tm DateTime_t;


/**
 * @brief Initializes the RTC clock and checks if it is already configured using pointers to time and date structures.
 *
 * This function retrieves the current time and date from the RTC (Real-Time Clock) using the HAL library.
 * If the RTC is not configured (both the year and hours are zero), it prompts the user to input the correct time.
 * If the RTC is properly configured, it prints the current local time adjusted for the specified time zone.
 *
 * @param[out] rtcTimePtr  Pointer to an @p RTC_TimeTypeDef structure where the retrieved time will be stored.
 * @param[out] rtcDatePtr  Pointer to an @p RTC_DateTypeDef structure where the retrieved date will be stored.
 *
 * @return Returns 1 if the RTC was not configured and the user was prompted to input the time.
 *         Returns 0 if the RTC was already configured and the time was successfully read.
 *
 * @note The RTC must be initialized before calling this function. The local time is adjusted by the @p TIMEZONE constant.
 *
 * @warning The function assumes that the RTC is in binary format (RTC_FORMAT_BIN) when reading the time and date.
 */
uint8_t initClockRTC(RTC_TimeTypeDef *rtcTimePtr, RTC_DateTypeDef *rtcDatePtr);


/**
 * @brief Retrieves the current date and time from the RTC and converts it to a UTC timestamp using the provided time and date structures.
 *
 * This function reads the current date and time from the hardware RTC (Real-Time Clock) using the HAL library.
 * It stores the retrieved time and date in the provided structures, converts the data to a @p DateTime_t structure,
 * and calculates the UTC timestamp, returning it as a 32-bit unsigned integer.
 *
 * @param[out] rtcTimePtr  Pointer to an @p RTC_TimeTypeDef structure that will store the current time retrieved from the RTC.
 * @param[out] rtcDatePtr  Pointer to an @p RTC_DateTypeDef structure that will store the current date retrieved from the RTC.
 *
 * @return Returns the current time as a UTC timestamp (Epoch time) in seconds since 1970.
 *
 * @note The function assumes the RTC is correctly initialized and configured. The time is converted to UTC
 *       based on the current local time stored in the RTC.
 *
 * @warning The function expects the RTC to be in binary format (RTC_FORMAT_BIN) when reading the time and date.
 *          Ensure that both @p rtcTimePtr and @p rtcDatePtr are valid pointers.
 */
uint32_t getTimeStamp(RTC_TimeTypeDef *rtcTimePtr, RTC_DateTypeDef *rtcDatePtr);


/**
 * @brief Prints the local date and time, adjusted for the specified time zone.
 *
 * This function formats and prints the date and time stored in the provided RTC structures.
 * It applies a time zone adjustment to the hours, based on the @p TIMEZONE constant, and
 * displays the result in the format "DD-MM-YYYY, hh:mm:ss".
 *
 * @param[in] rtcTimePtr  Pointer to an @p RTC_TimeTypeDef structure containing the current time.
 * @param[in] rtcDatePtr  Pointer to an @p RTC_DateTypeDef structure containing the current date.
 *
 * @note This function assumes that both @p rtcTimePtr and @p rtcDatePtr are properly initialized and
 *       populated with valid date and time information. The time zone adjustment is applied only to the hours field.
 *
 * @warning Ensure that both pointers, @p rtcTimePtr and @p rtcDatePtr, are valid and point to structures
 *          containing accurate time data before calling this function.
 */
void printLocalTime(RTC_TimeTypeDef *rtcTimePtr, RTC_DateTypeDef *rtcDatePtr);


#endif /* INC_CLOCKCONTROL_H_ */
