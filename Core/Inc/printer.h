/*
 * printer.h
 *
 *  Created on: Nov 10, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#ifndef INC_PRINTER_H_
#define INC_PRINTER_H_

#include <stdio.h>
#include <ctype.h>

#define SEPARATOR_LENGTH	80
#define NEW_LINE_NUMBERS	15

typedef enum {
	PRI_ERROR = 1,
	PRI_WARNING,
	PRI_INFO,
	PRI_DEBUG,
	PRI_LOG
} messageType;


void printConsolePostfix(int messType);

void printLineSeparator(char separator);

void clearTerminal();

void printTitle(const char *str);

void printSubTitle(const char *str);

void printStartInfo();





#endif /* INC_PRINTER_H_ */
