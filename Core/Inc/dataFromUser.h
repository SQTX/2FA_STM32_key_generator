/*
 * dataFromUser.h
 *
 *  Created on: Oct 14, 2024
 *      Author: sqtx
 */

#ifndef INC_DATAFROMUSER_H_
#define INC_DATAFROMUSER_H_

#include <stdio.h>
#include <string.h>
#include <time.h>

#define DATATIME_MAX_LENGTH	21		// DD-MM-YYYY,hh:mm:ss`\n``\r`

typedef struct tm DataTime_t;

uint8_t getDataTimeViaKeyboard(uint8_t value, DataTime_t *DataTime);

#endif /* INC_DATAFROMUSER_H_ */
