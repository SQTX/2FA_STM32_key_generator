/*
 * dataFromUser.h
 *
 *  Created on: Oct 14, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#ifndef INC_DATAFROMUSER_H_
#define INC_DATAFROMUSER_H_

//Std:
#include <stdio.h>
#include <string.h>
#include <time.h>
//My:
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


/**
 * @brief Receives characters from the keyboard and constructs a string in the specified buffer until the end of the line is detected.
 *
 * This function accumulates characters one by one in an internal buffer until an end-of-line character ('\r' or '\n') is received,
 * indicating the completion of the input. Once the line is complete, the accumulated string is copied into the provided
 * `stringContainer`, limited to the specified `stringLength`. If the input exceeds the allowed length, the buffer is cleared,
 * and an error is returned.
 *
 * @param[in] valueChar       Character received from the keyboard input.
 * @param[out] stringContainer Pointer to the buffer where the final string will be stored upon completion.
 * @param[in] stringLength    Maximum allowable length of the string to be stored in `stringContainer`.
 *
 * @return int8_t             Status of the function execution:
 *                             - 0 on success (string fully received and stored in `stringContainer`),
 *                             - 1 if the line input is still in progress,
 *                             - -1 if an error occurs due to exceeding the allowed length.
 *
 * @note The function resets the buffer if the input length exceeds `MAX_STRING_LENGTH` or `stringLength`.
 *
 * @warning Ensure `stringContainer` is large enough to hold `stringLength + 1` bytes to accommodate the NULL terminator.
 */
int8_t getStringViaKeyboard(uint8_t value, char* stringContainer, uint8_t stringLenght);

#endif /* INC_DATAFROMUSER_H_ */
