#ifndef UTILS_H
#define UTILS_H


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


int min(int a, int b);
int max(int a, int b);


/// Leftpad string with spaces
char *__LEFT_PAD_SPACE(char *dest, double x, unsigned int width);
#define LEFT_PAD_SPACE(x, y) __LEFT_PAD_SPACE((char [32]){""}, (x), (y))


/// Check if a given string is a number
bool isnumber(char *str);

/// Convert given string to number
/// This function checks `isnumber`
int8_t tonumber(char *str);


/// Open the specified `filename`
/// If the filename is `-`, use the specified `std`
/// This function catches IO errors with `exit(2)``
FILE *open_file_or_std(char *filename, char *mode, FILE *std);


/// Easily print binary numbers
#define BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define TO_BINARY(x)  \
  ((x) & 0b10000000 ? '1' : '0'), \
  ((x) & 0b01000000 ? '1' : '0'), \
  ((x) & 0b00100000 ? '1' : '0'), \
  ((x) & 0b00010000 ? '1' : '0'), \
  ((x) & 0b00001000 ? '1' : '0'), \
  ((x) & 0b00000100 ? '1' : '0'), \
  ((x) & 0b00000010 ? '1' : '0'), \
  ((x) & 0b00000001 ? '1' : '0')


#ifdef ENABLE_DEBUG
    static FILE *debug_stream = NULL;

    int debugf(const char *format, ...);
    FILE *get_debug_stream(void);
#else
    #define debugf(...) (void) 0
#endif


#endif
