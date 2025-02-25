/*
 * keyGenerator.c
 *
 *  Created on: Oct 13, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#include "keyGenerator.h"


//********************************************************************************************
// PUBLIC
//********************************************************************************************
uint32_t generateToken(uint8_t* hmacKey, uint8_t keyLength, uint32_t timeStamp) {
	  TOTP(hmacKey, keyLength, TOTP_TIMESTEP);
	  uint32_t totp_token = getCodeFromTimestamp(timeStamp);
	  return totp_token;
}
