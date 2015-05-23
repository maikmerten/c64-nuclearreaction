#ifndef __petscii_h
#define __petscii_h


#include "defs.h"


// characters 'empty' and 'atom'
#define CHAR_SPACE 32
#define CHAR_ATOM 81

// round corners
#define CHAR_COR_TL 85
#define CHAR_COR_TR 73
#define CHAR_COR_BL 74
#define CHAR_COR_BR 75

// chipped corners
#define CHAR_CHIP_TL 233
#define CHAR_CHIP_TR 223
#define CHAR_CHIP_BL 95
#define CHAR_CHIP_BR 105


// crossings
#define CHAR_CROSS 91
#define CHAR_CROSS_L 107
#define CHAR_CROSS_R 115
#define CHAR_CROSS_T 114
#define CHAR_CROSS_B 113

// bars
#define CHAR_HOR 67
#define CHAR_VER 66

// patterns
#define CHAR_EMPTY 96
#define CHAR_FULL 160
#define CHAR_HALF 102


void fillRect(char x1, char y1, char x2, char y2, char character, char color);


#endif
