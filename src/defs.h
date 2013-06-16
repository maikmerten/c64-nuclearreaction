#ifndef __defs_h
#define __defs_h

// sync to screen helpers
#define WAIT_WHILE_RASTERLINE_LOW	while (!(VIC.ctrl1 & 0x80)) {};
#define WAIT_WHILE_RASTERLINE_HIGH	while (VIC.ctrl1 & 0x80) {};

// joystick defs
#define JOY2  0xDC00
#define JOYUP  0x01
#define JOYDOWN  0x02
#define JOYLEFT  0x04
#define JOYRIGHT  0x08
#define JOYFIRE  0x10

// memory locations
#define TEXTBASE 0x0400
#define COLORRAM 0xD800

// VIC banks
#define BANK0BASE 0
#define BANK1BASE 16384
#define BANK2BASE 32768
#define BANK3BASE 49152

// definitions of field screen position and color
#define FIELDOFF 90
#define FIELDCOLOR 11

// characters 'empty' and 'atom'
#define CHAR_SPACE 32
#define CHAR_ATOM 81

// round corners
#define CHAR_COR_TL 85
#define CHAR_COR_TR 73
#define CHAR_COR_BL 74
#define CHAR_COR_BR 75

// crossings
#define CHAR_CROSS 91
#define CHAR_CROSS_L 107
#define CHAR_CROSS_R 115
#define CHAR_CROSS_T 114
#define CHAR_CROSS_B 113

// bars
#define CHAR_HOR 67
#define CHAR_VER 66


// player number of AI
#define PLAYERAI 2

// sprites
#define SPRITE0_DATA    0x0340
#define SPRITE0_PTR     0x07F8
#define SPRITE_PLAYER_X 45 
#define SPRITE_PLAYER_Y 105

// dimensions of playing field
#define SIZEX 6
#define MAXX 5
#define SIZEY 5
#define MAXY 4


#endif
