#ifndef BITCODE_H
#define BITCODE_H


#include <stdint.h>


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-const-variable"

#define BITCODE(name) static const int8_t name

BITCODE(EXIT) = 0170; // x
BITCODE(NOP) = 0040; // [space]

BITCODE(BYTE) = 0044; // $
BITCODE(REG) = 0100; // @

BITCODE(INC) = 0151; // i
BITCODE(MOV) = 0075; // =
BITCODE(ADD) = 0053; // +
BITCODE(SUB) = 0055; // -
BITCODE(MUL) = 0052; // *
BITCODE(SQ) = 0062; // 2

BITCODE(REGIP) = 0041; // !
BITCODE(REGA) = 0101; // A
BITCODE(REGB) = 0102; // B
BITCODE(REGC) = 0103; // C
BITCODE(REGD) = 0104; // D

BITCODE(DEBUG) = 0140; // `

#pragma GCC diagnostic pop


#endif
