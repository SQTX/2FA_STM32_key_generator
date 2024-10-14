/*
 * dataFromUser.c
 *
 *  Created on: Oct 14, 2024
 *      Author: sqtx
 */
#include "dataFromUser.h"


static char line_buffer[DATATIME_MAX_LENGTH + 1];	// +'\0'
static uint32_t line_length;


void extractDateToTm(char *date, DataTime_t *dataStruct) {
    // Użycie sscanf do wyciągnięcia dnia, miesiąca i roku
    int day, month, year;
    sscanf(date, "%d-%d-%d", &day, &month, &year);

    // Przypisanie wartości do odpowiednich pól struktury tm
    dataStruct->tm_mday = day;        // Dzień miesiąca
    dataStruct->tm_mon = month - 1;   // Miesiące w strukturze tm liczone są od 0 (styczeń = 0)
    dataStruct->tm_year = year - 1900; // Rok w strukturze tm liczy się od 1900
}


void extractTimeToTm(char *timeStr, DataTime_t *timeStruct) {
    // Użycie sscanf do wyciągnięcia godzin, minut i sekund
    int hour, minute, second;
    sscanf(timeStr, "%d:%d:%d", &hour, &minute, &second);

    // Przypisanie wartości do odpowiednich pól struktury tm
    timeStruct->tm_hour = hour;   // Godzina
    timeStruct->tm_min = minute;  // Minuty
    timeStruct->tm_sec = second;  // Sekundy
}


void splitDateTime(char *input, char *date, char *time) {
    // Szukamy przecinka oddzielającego datę od godziny
    char *comma = strchr(input, ',');

    if (comma != NULL) {
        // Kopiowanie daty (wszystko przed przecinkiem)
        strncpy(date, input, (comma - input));
        date[comma - input] = '\0';  // Dodanie znaku końca ciągu

        // Kopiowanie czasu (wszystko po przecinku)
        strcpy(time, (comma + 1));
    } else {
        // Jeśli przecinek nie został znaleziony, wypisujemy błąd
        printf("Błędny format danych\n");
    }
}


uint8_t getDataTimeViaKeyboard(uint8_t value, DataTime_t *DataTime) {
	if (value == '\r' || value == '\n') {
//		End of line:
		if (line_length > 0) {
			line_buffer[line_length] = '\0';	// Add null char in the end

//			Get data from String:
//		    char input[] = "12-10-2024,15:45:30";
		    char date[11];  // Miejsce na "DD-MM-YYYY" (10 znaków + \0)
		    char time[9];   // Miejsce na "hh:mm:ss" (8 znaków + \0)

		    splitDateTime(line_buffer, date, time);

//		    printf("Data: %s\n", date);
//		    printf("Godzina: %s\n", time);

		    extractDateToTm(date, DataTime);
		    extractTimeToTm(time, DataTime);

			line_length = 0;	// Buffer RESET
			return 0;
		}
	} else {
//		Get another char:
		if (line_length >= DATATIME_MAX_LENGTH) {
			// za dużo danych, usuwamy wszystko co odebraliśmy dotychczas
			line_length = 0;
		}
		// dopisujemy wartość do bufora
		line_buffer[line_length++] = value;
		return 1;
	}
	return 1;
}
