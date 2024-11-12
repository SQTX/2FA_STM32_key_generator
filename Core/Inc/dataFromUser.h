/*
 * dataFromUser.h
 *
 *  Created on: Oct 14, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#ifndef INC_DATAFROMUSER_H_
#define INC_DATAFROMUSER_H_

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "printer.h"

#define DATATIME_MAX_LENGTH		21		// DD-MM-YYYY,hh:mm:ss`\n``\r`
#define MAX_STRING_LENGTH		22

typedef struct tm DateTime_t;


/**
 * @brief Reads the date and time entered via keyboard in the format "DD-MM-YYYY,hh:mm:ss".
 *
 * The function takes consecutive characters entered from the keyboard, stores them in a buffer,
 * and after detecting the end of the line (characters '\r' or '\n'), processes the input string,
 * splitting it into the date and time. The extracted data is then converted and stored in the
 * @p DataTime structure.
 *
 * @param[in] value      A single character entered from the keyboard.
 * @param[out] DataTime  Pointer to a @p DateTime_t structure where the processed date and time
 *                       will be stored.
 *
 * @return Returns 0 when the entire string has been successfully processed, and the data has
 *         been saved to the @p DataTime structure. Returns 1 if buffering is still in progress
 *         or if an invalid data length was encountered (buffer overflow).
 *
 * @note The function expects the data to be entered in the format "DD-MM-YYYY,hh:mm:ss".
 *       The maximum length of the input string is defined by the @p DATATIME_MAX_LENGTH constant.
 *
 * @warning If the buffer reaches the maximum length, all previously collected data will be discarded.
 */
uint8_t getDataTimeViaKeyboard(uint8_t value, DateTime_t *DataTime);

int8_t getStringViaKeyboard(uint8_t value, char* stringContainer, uint8_t stringLenght);

#endif /* INC_DATAFROMUSER_H_ */
