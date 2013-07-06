#ifndef __defs_h
#define __defs_h

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <peekpoke.h>
#include <c64.h>

// sync to screen helpers
#define WAIT_WHILE_RASTERLINE_LOW	while (!(VIC.ctrl1 & 0x80)) {};
#define WAIT_WHILE_RASTERLINE_HIGH	while (VIC.ctrl1 & 0x80) {};

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
