/*
 * keyGenerator.h
 *
 *  Created on: Oct 13, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#ifndef INC_KEYGENERATOR_H_
#define INC_KEYGENERATOR_H_

#include <stdio.h>
#include <stdlib.h>
#include "TOTP.h"


#define TOTP_TIMESTEP 30


/**
 * @brief Generates a TOTP (Time-based One-Time Password) token using the provided HMAC key and timestamp.
 *
 * This function generates a TOTP token based on the provided HMAC key, key length, and a given timestamp.
 * It uses the TOTP algorithm with a fixed time step to compute the token.
 *
 * @param[in] hmacKey    Pointer to the HMAC key used for token generation.
 * @param[in] keyLength  Length of the HMAC key in bytes.
 * @param[in] timeStamp  The timestamp used for generating the TOTP token (typically the current time or a specific moment).
 *
 * @return Returns a 32-bit unsigned integer representing the generated TOTP token.
 *
 * @note The TOTP algorithm follows a time step defined by the constant @p TOTP_TIMESTEP.
 *       Ensure that the HMAC key and timestamp are correctly provided.
 */
uint32_t generateToken(uint8_t* hmacKey, uint8_t keyLength, uint32_t timeStamp);


#endif /* INC_KEYGENERATOR_H_ */
