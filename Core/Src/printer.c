/*
 * printer.c
 *
 *  Created on: Nov 10, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#include "printer.h"


void printAsciiArt();
void printLicense();

//********************************************************************************************
// PUBLIC
//********************************************************************************************
void printConsolePostfix(int messType) {
	switch(messType) {
	case PRI_ERROR:
		printf("[ERROR] ");
		break;
	case PRI_WARNING:
		printf("[WARNING] ");
		break;
	case PRI_INFO:
		printf("[INFO] ");
		break;
	case PRI_DEBUG:
		printf("[DEBUG] ");
		break;
	case PRI_LOG:
		printf("[LOG] ");
		break;
	default:
		break;
	}
}


void printLineSeparator(char separator) {
	for(int i = 0; i < SEPARATOR_LENGTH; i++) printf("%c", separator);
	printf("\n");
}


void clearTerminal() {
	for(int i = 0; i < NEW_LINE_NUMBERS; i++) printf("\n");
}

void printTitle(const char *str) {
	printLineSeparator('=');
    while (*str) {
        putchar(toupper(*str));  // Konwertuje i drukuje każdy znak jako wielką literę
        str++;
    }
    putchar('\n');  // Przejście do nowej linii po wydrukowaniu całego ciągu
    printLineSeparator('=');
}

void printSubTitle(const char *str) {
	printLineSeparator('-');
    while (*str) {
        putchar(*str);  // Konwertuje i drukuje każdy znak jako wielką literę
        str++;
    }
    putchar('\n');  // Przejście do nowej linii po wydrukowaniu całego ciągu
    printLineSeparator('-');
}

void printStartInfo() {
	clearTerminal();
	printAsciiArt();
	printf("\n");
	printLineSeparator('=');
	printLicense();
	printLineSeparator('=');
	printf("\n");
}

//********************************************************************************************
// PRIVATE
//********************************************************************************************
void printAsciiArt() {
//	  ____  _____ _      ____ _____ __  __   _  _________   __   ____ _____ _   _
//	 |___ \|  ___/ \    / ___|_   _|  \/  | | |/ | ____\ \ / /  / ___| ____| \ | |
//	   __) | |_ / _ \   \___ \ | | | |\/| | | ' /|  _|  \ V /  | |  _|  _| |  \| |
//	  / __/|  _/ ___ \   ___) || | | |  | | | . \| |___  | |   | |_| | |___| |\  |_
//	 |_____|_|/_/   \_\ |____/ |_| |_|  |_| |_|\_|_____| |_|    \____|_____|_| \_(_)
//
	printf(" ____  _____ _      ____ _____ __  __   _  _________   __   ____ _____ _   _\n");
	printf("|___ \\|  ___/ \\    / ___|_   _|  \\/  | | |/ | ____\\ \\ / /  / ___| ____| \\ | |\n");
	printf("  __) | |_ / _ \\   \\___ \\ | | | |\\/| | | ' /|  _|  \\ V /  | |  _|  _| |  \\| |\n");
	printf(" / __/|  _/ ___ \\   ___) || | | |  | | | . \\| |___  | |   | |_| | |___| |\\  |_ \n");
	printf("|_____|_|/_/   \\_\\ |____/ |_| |_|  |_| |_|\\_|_____| |_|    \\____|_____|_| \\_(_)\n");
	printf("\n");
}


void printLicense() {
    printf("MIT License\n\n");
    printf("Copyright (c) [year] [fullname]\n\n");
    printf("Permission is hereby granted, free of charge, to any person obtaining a copy\n");
    printf("of this software and associated documentation files (the \"Software\"), to deal\n");
    printf("in the Software without restriction, including without limitation the rights\n");
    printf("to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n");
    printf("copies of the Software, and to permit persons to whom the Software is\n");
    printf("furnished to do so, subject to the following conditions:\n\n");
    printf("The above copyright notice and this permission notice shall be included in all\n");
    printf("copies or substantial portions of the Software.\n\n");
    printf("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n");
    printf("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n");
    printf("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n");
    printf("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n");
    printf("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n");
    printf("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n");
    printf("SOFTWARE.\n");
}
