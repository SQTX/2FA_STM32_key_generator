/*
 * printer.h
 *
 *  Created on: Nov 10, 2024
 *      Author: Jakub SQTX Sitarczyk
 */

#ifndef INC_PRINTER_H_
#define INC_PRINTER_H_

//Std:
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


/**
 * @brief Prints a console message prefix based on the message type.
 *
 * This function prints a message prefix based on the severity level specified by `messType`.
 * The prefix is one of the predefined types, such as `[ERROR]`, `[WARNING]`, `[INFO]`, `[DEBUG]`, or `[LOG]`.
 * The purpose of this function is to provide context for log messages to help the user or developer identify the
 * severity of the message.
 *
 * @param[in] messType An integer representing the type of message:
 * - `PRI_ERROR`: Indicates an error message.
 * - `PRI_WARNING`: Indicates a warning message.
 * - `PRI_INFO`: Indicates an informational message.
 * - `PRI_DEBUG`: Indicates a debug message.
 * - `PRI_LOG`: Indicates a log message.
 *
 * @note The function prints the prefix to the console without a newline. The caller is responsible for printing
 *       the actual message content after the prefix.
 *
 * @dependencies None.
 *
 * @example
 * // Example usage:
 * printConsolePostfix(PRI_INFO);  // Prints: [INFO]
 * printf("This is an informational message.\n");
 *
 * @remark This function is used primarily for categorizing and labeling log messages based on their importance.
 */
void printConsolePostfix(int messType);


/**
 * @brief Prints a line of specified characters to the console.
 *
 * This function prints a horizontal line made of a single character (`separator`) to the console.
 * The line's length is determined by the constant `SEPARATOR_LENGTH`, which is the number of times
 * the character is printed. After the line, a newline character (`\n`) is added to move to the next line.
 * This function is useful for separating sections in the console output, making it easier to read.
 *
 * @param[in] separator The character used to print the separator line.
 *
 * @note The length of the separator line is fixed and defined by the `SEPARATOR_LENGTH` constant.
 *
 * @example
 * // Example usage:
 * printLineSeparator('-');  // Prints a line of dashes, e.g. "----------"
 *
 * @remark This function is typically used for visual formatting in console-based applications.
 */
void printLineSeparator(char separator);


/**
 * @brief Clears the terminal screen by printing a series of newlines.
 *
 * This function simulates clearing the terminal screen by printing a specified number of newline
 * characters. This approach is used to move the previous content off the screen, creating the
 * appearance of a cleared terminal. The number of newlines printed is defined by the constant
 * `NEW_LINE_NUMBERS`, which controls how much content is removed from view.
 *
 * @note This method does not actually clear the terminal buffer; it only simulates the effect
 *       by printing multiple newline characters. For actual terminal clearing, platform-specific
 *       commands like `system("clear")` on Unix-based systems or `system("cls")` on Windows could be used.
 *
 * @example
 * // Example usage:
 * clearTerminal();  // Prints multiple newlines to simulate clearing the screen.
 *
 * @remark This function is typically used for creating a "clean" look in console-based applications,
 *         especially in menu-driven interfaces or while updating the display.
 */
void clearTerminal();


/**
 * @brief Prints a title with uppercase characters surrounded by line separators.
 *
 * This function prints a title string, converting all characters to uppercase before printing,
 * and surrounds the title with separator lines made of the '=' character. The separator lines
 * are printed before and after the title to give it emphasis in the output.
 *
 * @param str A pointer to the string to be printed as the title. The string is processed
 *            character by character, and each character is converted to uppercase before
 *            printing.
 *
 * @note The function uses `toupper()` to convert each character to uppercase, so it only affects
 *       alphabetic characters. Non-alphabetic characters are printed as they are.
 *
 * @example
 * // Example usage:
 * printTitle("Welcome to the System");
 * // Output:
 * // ============================
 * // WELCOME TO THE SYSTEM
 * // ============================
 *
 * @see printLineSeparator() for the separator line printing functionality.
 */
void printTitle(const char *str);


/**
 * @brief Prints a subtitle surrounded by line separators.
 *
 * This function prints a subtitle string surrounded by separator lines made of the '-' character.
 * The subtitle is printed as-is, without any transformations, and the separator lines are printed
 * before and after the subtitle to visually highlight it in the output.
 *
 * @param str A pointer to the string to be printed as the subtitle. The string is printed character
 *            by character without any modifications.
 *
 * @note The subtitle is printed exactly as given, without altering the case or characters.
 *       The function simply prints the subtitle with surrounding line separators.
 *
 * @example
 * // Example usage:
 * printSubTitle("Important Information");
 * // Output:
 * // --------------------------
 * // Important Information
 * // --------------------------
 *
 * @see printLineSeparator() for the separator line printing functionality.
 */
void printSubTitle(const char *str);


/**
 * @brief Displays the initial startup information, including ASCII art and license details.
 *
 * This function clears the terminal, prints the project logo in ASCII art form, and then displays
 * the project license. It uses the `printAsciiArt` and `printLicense` functions to handle specific
 * sections of the startup information.
 *
 * @example
 * // Example usage:
 * printStartInfo();
 * // Output:
 * // (Terminal is cleared)
 * // (ASCII Art logo is printed)
 * // (License information is printed)
 */
void printStartInfo();


#endif /* INC_PRINTER_H_ */
