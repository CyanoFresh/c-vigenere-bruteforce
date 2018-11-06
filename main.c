#include "stdio.h"
#include "string.h"
#include "time.h"
#include "stdlib.h"

#define SOURCE_LENGTH 13

#define ASCII_LETTERS_START 65
#define ASCII_LETTERS_COUNT 26
#define STR_TO_SUCCESS "HELLO"
#define CHARS_LENGTH 26
const char chars[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
                      'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

int found = 0;

int isRightKey(const char *encoded, const char *key, const char *stringToFind) {
    char decoded[SOURCE_LENGTH + 1] = {'\0'};
    int keyIndex = 0;

    for (int i = 0; i < SOURCE_LENGTH; ++i) {
        if (encoded[i] == ' ') {
            decoded[i] = ' ';
            continue;
        }

        int letterCode =
                ((encoded[i] - ASCII_LETTERS_START) - (key[keyIndex] - ASCII_LETTERS_START)) % ASCII_LETTERS_COUNT;

        if (letterCode < 0) {
            letterCode += ASCII_LETTERS_COUNT;
        }

        decoded[i] = (char) (ASCII_LETTERS_START + letterCode);

//        printf("%c (%d - 65) (%d) - %c (%d - 65) (%d) -> %c (%d - 65) (%d)\n", encoded[i], encoded[i],
//               encoded[i] - ASCII_LETTERS_START, key[keyIndex], key[keyIndex], key[keyIndex] - ASCII_LETTERS_START,
//               ASCII_LETTERS_START + newCharCode,
//               ASCII_LETTERS_START + newCharCode,
//               newCharCode
//        );

        if (keyIndex + 1 >= strlen(key)) {
            keyIndex = 0;
        } else {
            keyIndex++;
        }
    }

//    printf("decoded %s\n", decoded);

    return strstr(decoded, stringToFind) != NULL;
}

void bruteForce(const char *encoded, char *key, const int charsToGo) {
    if (found) {
        return;
    }

    if (charsToGo == 0) {
        if (isRightKey(encoded, key, STR_TO_SUCCESS)) {
//            printf("Key found: '%s'\n", key);
            found = 1;
        }

        return;
    }

    for (int i = 0; i < CHARS_LENGTH; ++i) {
        key[charsToGo - 1] = chars[i];

        bruteForce(encoded, key, charsToGo - 1);
    }
}

void run(const int iterations, const char *source, const int keyLength) {
    clock_t startTime = clock();
    clock_t withoutEncodingSum = 0;

    for (int i = 0; i < iterations; ++i) {
        clock_t startTime1 = clock();

        // Generate key of given length
        char key[keyLength + 1];

        for (int j = 0; j < keyLength; ++j) {
            key[j] = (char) (ASCII_LETTERS_START + rand() % ASCII_LETTERS_COUNT);
        }

        key[keyLength] = '\0';

//        printf("key: %s\n", key);
        found = 0;

        // 1. Encode
        char encoded[SOURCE_LENGTH + 1];
        int keyIndex = 0;

        for (int j = 0; j < SOURCE_LENGTH; ++j) {
            // Skipping symbols
            if (source[j] == ' ') {
                encoded[j] = ' ';
                continue;
            }
            encoded[j] = (char) (ASCII_LETTERS_START +
                                 (((int) source[j] - ASCII_LETTERS_START) +
                                  ((int) key[keyIndex] - ASCII_LETTERS_START)) %
                                 ASCII_LETTERS_COUNT);

            if (keyIndex + 1 >= keyLength) {
                keyIndex = 0;
            } else {
                keyIndex++;
            }
        }

        encoded[SOURCE_LENGTH] = '\0';
//        printf("encoded: %s\n", encoded);

        withoutEncodingSum += clock() - startTime1;

        // 2. Find the key
        char startKey[keyLength + 1];

        for (int k = 0; k < keyLength; ++k) {
            startKey[k] = ' ';
        }

        startKey[keyLength] = '\0';

        bruteForce(encoded, startKey, keyLength);
    }

    clock_t diff = clock() - startTime;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    int msec1 = withoutEncodingSum * 1000 / CLOCKS_PER_SEC;

    printf("iterations: %d; time %d.%.3d s., without encoding - %d.%.3d s.; key length: %d\n", iterations, msec / 1000, msec % 1000, (msec - msec1) / 1000, (msec - msec1) % 1000, keyLength);
}

/**
 * Vigenere cypher brute-forcing task
 */
int main(void) {
    srand(time(NULL));

    const char source[] = "A HELLO WORLD";

    run(1000, source, 1);
    run(1000, source, 2);
    run(1000, source, 3);
    run(1000, source, 4);
    run(1000, source, 5);
}