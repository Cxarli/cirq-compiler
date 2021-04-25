#ifndef DEFINES_H
#define DEFINES_H


#include <stdio.h>
#include <string.h>
#include <stdbool.h>


// The default size for buffers, vectors, etc
#define BUF_SIZE 256


#define CONCAT_(x, y) x ## y
#define CONCAT(x, y) CONCAT_(x, y)


// Scope a variable for the following block
#define with(statement) \
	for (char CONCAT(__wx, __LINE__) = 1; CONCAT(__wx, __LINE__);) \
		for (statement; CONCAT(__wx, __LINE__); CONCAT(__wx, __LINE__) = 0) \

// Scope another variable for the following block
#define and_with(statement) \
	for (char CONCAT(__wy, __LINE__) = 1; CONCAT(__wy, __LINE__);) \
		for (statement; CONCAT(__wy, __LINE__); CONCAT(__wy, __LINE__) = 0) \


// Easier string switch'ing
#define switch_str(x) \
	with(char *SWITCH_STR = (x))


#define case_str(y) \
	if (strcmp(SWITCH_STR, (y)) == 0)


/*
// use it like:

switch_str(name) {
	case_str("foo") {

	}

	[else ]case_str("bar") {

	}
}
*/


#endif
