/*
 * clockControl.c
 *
 *  Created on: Oct 16, 2024
 *      Author: Jakub SQTX Sitarczyk
 */
#include "clockControl.h"


// TODO: Hardcode time zone
//const int8_t TIMEZONE = {2};	// Poland: UTC+2h (local_summer_time_hour - 2h)
const int8_t TIMEZONE = {1};	// Poland: UTC+1h (local_winter_time_hour - 1h)


//********************************************************************************************
// PUBLIC
//********************************************************************************************
uint8_t initClockRTC(RTC_TimeTypeDef *rtcTimePtr, RTC_DateTypeDef *rtcDatePtr) {
	HAL_RTC_GetTime(&hrtc, rtcTimePtr, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, rtcDatePtr, RTC_FORMAT_BIN);

	if(rtcDatePtr->Year <= 0 && rtcTimePtr->Hours <= 0) {
		printf("RTC\t[not configured]\n");
		getTimeFromUser(rtcTimePtr, rtcDatePtr);
		return 1;
	}
	printf("RTC status\t\t\t[OK]\n");
//	printf("Your local time: [%02d-%02d-%04d, %02d:%02d:%02d]\n", rtcDatePtr->Date, rtcDatePtr->Month, rtcDatePtr->Year + 2000,
//			rtcTimePtr->Hours + TIMEZONE, rtcTimePtr->Minutes, rtcTimePtr->Seconds);
	return 0;
}


uint32_t getTimeStamp(RTC_TimeTypeDef *rtcTimePtr, RTC_DateTypeDef *rtcDatePtr) {
	HAL_RTC_GetDate(&hrtc, rtcDatePtr, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, rtcTimePtr, RTC_FORMAT_BIN);


	DateTime_t currentTime;
	currentTime.tm_sec = rtcTimePtr->Seconds;
	currentTime.tm_min = rtcTimePtr->Minutes;
	currentTime.tm_hour = rtcTimePtr->Hours;

	currentTime.tm_mday = rtcDatePtr->Date;
	currentTime.tm_mon = rtcDatePtr->Month - 1;
	currentTime.tm_year = (rtcDatePtr->Year + 2000) - 1900;


	time_t utcTimestamp = mktime(&currentTime);
	return (uint32_t)utcTimestamp;
}


void printLocalTime(RTC_TimeTypeDef *rtcTimePtr, RTC_DateTypeDef *rtcDatePtr) {
	printf("Local time:\t\t\t[%02d-%02d-%04d, %02d:%02d:%02d]\n", rtcDatePtr->Date, rtcDatePtr->Month, rtcDatePtr->Year + 2000,
				rtcTimePtr->Hours + TIMEZONE, rtcTimePtr->Minutes, rtcTimePtr->Seconds);
}


void getTimeFromUser(RTC_TimeTypeDef *rtcTimePtr, RTC_DateTypeDef *rtcDatePtr) {
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
				printf("UTC:\t %d-%d-%d,", datetime.tm_mday, datetime.tm_mon + 1, datetime.tm_year + 1900);
				printf("%d:%d:%d\n", datetime.tm_hour, datetime.tm_min, datetime.tm_sec);

//				Set time in RTC:
				rtcTimePtr->Hours = datetime.tm_hour;
				rtcTimePtr->Minutes = datetime.tm_min;
				rtcTimePtr->Seconds = datetime.tm_sec;
				HAL_RTC_SetTime(&hrtc, rtcTimePtr, RTC_FORMAT_BIN);
				rtcDatePtr->Year = datetime.tm_year + 1900 - 2000;
				rtcDatePtr->Month = datetime.tm_mon + 1;
				rtcDatePtr->Date = datetime.tm_mday;
				HAL_RTC_SetDate(&hrtc, rtcDatePtr, RTC_FORMAT_BIN);
			}
		}
	}
}

//TODO
//void changeTimeZone() {
//
//}


