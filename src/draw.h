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

extern unsigned char field[SIZEX][SIZEY];
extern unsigned char playercolors[3];
extern unsigned char ki;
extern const char sprHuman[]; // text.s
extern const char sprComputer[]; // text.s


void clearScreen(char textcolor);
void drawCell(char x, char y);
void setCellColor(char color, char x, char y);
void drawAtoms(char x, char y);
void drawField();
void switchBank(char base);
void showPicture(int imgdata);
void displayPlayerSprite(char player);
void printHUD(char player, char color, char move, char ki);

#endif
