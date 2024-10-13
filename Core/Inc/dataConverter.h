/*
 * dataConverter.h
 *
 *  Created on: Oct 13, 2024
 *      Author: sqtx
 */

#ifndef INC_DATACONVERTER_H_
#define INC_DATACONVERTER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "base32.h"


int8_t base32ToHex(const char *encoded, const uint8_t encodedLength, uint8_t **decoded);

#endif /* INC_DATACONVERTER_H_ */
