#include <stdint.h>
#include <string.h>
#include <stdint.h>

// Funkcja pomocnicza do zamiany znaku na wartość z alfabetu BASE32
int base32_char_to_value(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    } else if (c >= '2' && c <= '7') {
        return c - '2' + 26;
    } else {
        return -1; // Znak nieprawidłowy
    }
}

// Funkcja dekodująca BASE32
int base32_decode(const char *encoded, uint8_t *output, size_t output_size) {
    size_t input_length = strlen(encoded);
    size_t i, bit_index = 0, buffer = 0, output_index = 0;

    for (i = 0; i < input_length; i++) {
        // Ignoruj spacje i małe litery zamień na wielkie
        char c = toupper(encoded[i]);

        if (c == '=') {
            break; // Padding - ignorujemy resztę danych
        }

        int value = base32_char_to_value(c);
        if (value == -1) {
            return -1; // Zwróć błąd, jeśli napotkano nieprawidłowy znak
        }

        // Dodaj wartość do bufora
        buffer = (buffer << 5) | value;
        bit_index += 5;

        // Jeśli mamy co najmniej 8 bitów, zapisz bajt do outputu
        if (bit_index >= 8) {
            bit_index -= 8;
            if (output_index >= output_size) {
                return -1; // Brak miejsca w buforze wyjściowym
            }
            output[output_index++] = (buffer >> bit_index) & 0xFF;
        }
    }

    return output_index; // Zwraca liczbę zapisanych bajtów
}
