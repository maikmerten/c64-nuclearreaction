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
#define TEXTBASE 0x8400 // this is 32768 + 1024
#define COLORRAM 0xD800

// VIC banks
// notice that on the real hardware, bank 0 is bank 3 etc.
#define BANK0BASE 0
#define BANK1BASE 16384
#define BANK2BASE 32768
#define BANK3BASE 49152

// player number of AI
#define PLAYERAI 2


// position of sprites in VIC bank
#define HUMAN_PTR 198
#define COMPUTER_PTR 199
#define CELL0_PTR 200
#define CELL1_PTR 201
#define CELL2_PTR 202
#define CELL3_PTR 203
#define CELL4_PTR 204
#define CURSOR_PTR 205

// dimensions of playing field
#define SIZEX 6
#define MAXX 5
#define SIZEY 5
#define MAXY 4


#endif
