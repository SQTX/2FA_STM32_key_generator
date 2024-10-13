/*
 * keyGenerator.h
 *
 *  Created on: Oct 13, 2024
 *      Author: sqtx
 */

#ifndef INC_KEYGENERATOR_H_
#define INC_KEYGENERATOR_H_

#include <stdio.h>
#include <stdlib.h>
#include "TOTP.h"

#define TOTP_TIMESTEP 30


uint32_t generateToken(uint8_t* hmacKey, uint8_t keyLength, uint32_t timeStamp);


#endif /* INC_KEYGENERATOR_H_ */
