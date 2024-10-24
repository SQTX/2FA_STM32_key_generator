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
//#include <stdint.h>

#define EEPROM_ADDR     0xA0
#define WRITE_TIMEOUT   6


// Opóźnienie niezbędne do zakończenie zapisu
// Funkcja wprowadza automatycznie opóźnienie tylko, gdy jest ono potrzebne
void eeprom_wait(void);

/* Odczyt danych z pamięci EEPROM
 * addr - początkowy adres danych
 * data - bufor na odczytane dane
 * size - liczba bajtów do odczytania
 * return - HAL_OK jeśli odczyt zakończony sukcesem, wpp. błąd
 **/
HAL_StatusTypeDef eeprom_read(uint32_t addr, void* data, uint32_t size);


/* Zapis danych do pamięci EEPROM
 * addr - początkowy adres danych
 * data - bufor na dane zapisu
 * size - liczba bajtów do zapisania
 * return - HAL_OK jeśli zapis zakończony sukcesem, wpp. błąd
 * */
HAL_StatusTypeDef eeprom_write(uint32_t addr, const void* data, uint32_t size);


#endif /* INC_EEPROM_H_ */
