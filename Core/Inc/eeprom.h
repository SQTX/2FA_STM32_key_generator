/*
 * eeprom.h
 *
 *  Created on: Oct 24, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "stm32l4xx.h"
#include "i2c.h"


#define EEPROM_ADDR     0xA0
#define WRITE_TIMEOUT   6


/**
 * @brief Reads data from the EEPROM at the specified address.
 *
 * This function reads a block of data from a specified EEPROM memory address over I2C.
 * It first waits until the EEPROM is ready for a read operation, then uses the HAL I2C
 * library to retrieve the data.
 *
 * @param[in] addr  The EEPROM memory address from which to start reading.
 * @param[out] data Pointer to a buffer where the read data will be stored.
 * @param[in] size  The size of the data block to read in bytes.
 *
 * @return Returns a @p HAL_StatusTypeDef status indicating the result of the operation:
 *         HAL_OK if the read was successful, or an error status if the read failed.
 *
 * @note The function assumes that the EEPROM device address is defined by @p EEPROM_ADDR
 *       and that the I2C handle @p hi2c1 is properly initialized.
 *
 * @warning Ensure that @p data points to a buffer large enough to hold @p size bytes.
 *          The maximum delay, @p HAL_MAX_DELAY, is used in the I2C read operation, so the function
 *          may block if the EEPROM is not responding.
 */
HAL_StatusTypeDef eeprom_read(uint32_t addr, void* data, uint32_t size);


/**
 * @brief Writes data to the EEPROM at the specified address.
 *
 * This function writes a block of data to an EEPROM memory location over I2C.
 * It first waits until the EEPROM is ready for a write operation, then performs
 * the write using the HAL I2C library. The function also records the last write
 * time for potential use in future operations.
 *
 * @param[in] addr  The EEPROM memory address where the data should be written.
 * @param[in] data  Pointer to the data to be written to EEPROM.
 * @param[in] size  The size of the data block in bytes.
 *
 * @return Returns a @p HAL_StatusTypeDef status indicating the result of the operation:
 *         HAL_OK if the write was successful, or an error status if the write failed.
 *
 * @note The function assumes that the EEPROM device address is defined by @p EEPROM_ADDR
 *       and that the I2C handle @p hi2c1 is properly initialized.
 *
 * @warning Ensure that @p data points to a valid memory location with at least @p size bytes.
 *          The maximum delay, @p HAL_MAX_DELAY, is used in the I2C write operation, so the function
 *          may block if the EEPROM is not responding.
 */
HAL_StatusTypeDef eeprom_write(uint32_t addr, const void* data, uint32_t size);


#endif /* INC_EEPROM_H_ */
