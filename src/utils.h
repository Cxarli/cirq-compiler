#ifndef UTILS_H
#define UTILS_H


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


/// Check if a given string is a number
bool isnumber(char *str);

/// Convert given string to number
/// This function checks `isnumber`
int8_t tonumber(char *str);


/// Open the specified `filename`
/// If the filename is `-`, use the specified `std`
/// This function catches IO errors with `exit(2)``
FILE *open_file_or_std(char *filename, char *mode, FILE *std);


#ifdef ENABLE_DEBUG
    static FILE *debug_stream = NULL;

    int debugf(const char *format, ...);
#else
    #define debugf(...) (void) 0
#endif


#endif
