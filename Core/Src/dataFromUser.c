/*
 * dataFromUser.c
 *
 *  Created on: Oct 14, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#include "dataFromUser.h"


static char time_buffer[DATATIME_MAX_LENGTH + 1];	// Character buffer [limit + '\0']
static char string_buffer[MAX_STRING_LENGTH + 1];	// Character buffer [limit + '\0']
static uint32_t line_length;						// Number of characters in buffer


void extractDateToTm(char *, DateTime_t *);
void extractTimeToTm(char *, DateTime_t *);
void splitDateTime(char *, char *, char *);

//********************************************************************************************
// PUBLIC
//********************************************************************************************
uint8_t getDataTimeViaKeyboard(uint8_t valueChar, DateTime_t *DataTime) {
	if (valueChar == '\r' || valueChar == '\n') {
//		End of line:
		if (line_length > 0) {
			time_buffer[line_length] = '\0';	// Add NULL char in the end of String

//			Get data from String:
		    char date[11];  // Space for: "DD-MM-YYYY"	(10 char + \0)
		    char time[9];   // Space for: "hh:mm:ss" 	(8 char + \0)

		    splitDateTime(time_buffer, date, time);

//		    printf("Got date: %s\n", date);
//		    printf("Got time: %s\n", time);

		    extractDateToTm(date, DataTime);
		    extractTimeToTm(time, DataTime);

			line_length = 0;	// Buffer RESET
			return 0;			// Success
		}
	} else {
//		If the data exceeds the specified limit, it is deleted
		if (line_length >= DATATIME_MAX_LENGTH) {
			line_length = 0;
		}
//		Get another char and add it to buffer:
		time_buffer[line_length++] = valueChar;
		return 1;	// Not finished
	}
	return 1;		// Not finished
}


int8_t getStringViaKeyboard(uint8_t valueChar, char* stringContainer, uint8_t stringLenght) {
	if (valueChar == '\r' || valueChar == '\n') {
//		End of line:
		if (line_length > 0) {
			string_buffer[line_length] = '\0';	// Add NULL char in the end of String

			for(int i = 0; i < stringLenght+1; i++) {
				stringContainer[i] = string_buffer[i];
			}

			line_length = 0;	// Buffer RESET
			return 0;			// Success
		}
	} else {
//		If the data exceeds the specified limit, it is deleted
		if (line_length >= MAX_STRING_LENGTH || line_length >= stringLenght) {
			printConsolePostfix(PRI_WARNING);
			printf("The received string of characters exceeds the allowed limit\n");
			line_length = 0;
			return -1;			// ERROR
		}
//		Get another char and add it to buffer:
		string_buffer[line_length++] = valueChar;
		return 1;	// Not finished
	}
	return 1;		// Not finished
}

//********************************************************************************************
// PRIVATE
//********************************************************************************************
/**
 * Function that breaks the [String] date variable into individual data: day, month and year.
 * Values ​​are assigned to a pointer to a DateTime_t object.
 */
void extractDateToTm(char *date, DateTime_t *dataStruct) {
//	Extracting the day, month and year using sscanf:
    int day, month, year;
    sscanf(date, "%d-%d-%d", &day, &month, &year);

//    Assigning extracted values ​​to object fields:
    dataStruct->tm_mday = day;
    dataStruct->tm_mon = month - 1;   	// January = 0
    dataStruct->tm_year = year - 1900; 	// Years are counted from 1900
}


/**
 * Function that breaks the [String] time variable into individual data: hour, minute and second.
 * Values ​​are assigned to a pointer to a DateTime_t object.
 */
void extractTimeToTm(char *timeStr, DateTime_t *timeStruct) {
//	Extracting the hour, minute and second using sscanf:
    int hour, minute, second;
    sscanf(timeStr, "%d:%d:%d", &hour, &minute, &second);

//    Assigning extracted values ​​to object fields:
    timeStruct->tm_hour = hour;
    timeStruct->tm_min = minute;
    timeStruct->tm_sec = second;
}


/**
 * A function that splits a string of characters received from the user.
 * If the text matches the pattern, the data is split and assigned to [String] date and time variables.
 */
void splitDateTime(char *input, char *date, char *time) {
//	Finding the comma separating the date from the time:
    char *comma = strchr(input, ',');

    if (comma != NULL) {
//    	Copying date (everything before comma)
        strncpy(date, input, (comma - input));
        date[comma - input] = '\0';  			// Add NULL char in the end of String

//      Copying time (everything after comma).
        strcpy(time, (comma + 1));
    } else {
    	printConsolePostfix(PRI_ERROR);
        printf("Date and time assigned inconsistently with pattern\n");	// If comma is not found, we print error.
    }
}
