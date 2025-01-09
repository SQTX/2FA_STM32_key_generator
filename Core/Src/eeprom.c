/*
 * eeprom.c
 *
 *  Created on: Oct 24, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#include "eeprom.h"


static uint32_t last_write;


void eeprom_wait(void);

//********************************************************************************************
// PUBLIC
//********************************************************************************************
HAL_StatusTypeDef eeprom_read(uint32_t addr, void* data, uint32_t size) {
    eeprom_wait();
    return HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR, addr, 1, data, size, HAL_MAX_DELAY);
}


HAL_StatusTypeDef eeprom_write(uint32_t addr, const void* data, uint32_t size) {
    HAL_StatusTypeDef rc;

    eeprom_wait();
    rc = HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDR, addr, 1, (void*)data, size, HAL_MAX_DELAY);
    last_write = HAL_GetTick();

    return rc;
}


//********************************************************************************************
// PRIVATE
//********************************************************************************************
/**
 * Delay necessary to complete the write.
 * The function automatically introduces a delay only when it is needed
 */
void eeprom_wait(void) {
    while (HAL_GetTick() - last_write <= WRITE_TIMEOUT) {}
}
