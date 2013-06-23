#ifndef __draw_h
#define __draw_h

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <peekpoke.h>

#include "defs.h"
#include "field.h"
#include "input.h"
#include "sound.h"
#include "petscii.h"

extern unsigned char playercolors[3];
extern unsigned char ki;
extern const char sprHuman[]; // text.s
extern const char sprComputer[]; // text.s


void clearScreen(char textcolor);
void displayPlayerSprite(char player);
void drawAtoms(char x, char y);
void drawCell(char x, char y);
void drawField();
void highlightCell(char x, char y);
void printHUD(char player, char color, char move, char ki);
void setCellColor(char color, char x, char y);
void showPicture(int imgdata);
void switchBank(char base);




#endif
