#ifndef __draw_h
#define __draw_h

#include "defs.h"

#define VICBANKTEXT 2
#define VICBANKBITMAP 3

// definitions of field screen position and color
#define FIELDOFF 0
#define FIELDCOLOR 11

// definitions for player sprites in HUD
#define SPRITE0_PTR     34808
#define SPRITE_PLAYER_X 30
#define SPRITE_PLAYER_Y 125


extern unsigned char playercolors[3];
extern unsigned char ki;


void clearScreen(char textcolor);
void displayPlayerSprite(char player);
void drawAtoms(char x, char y);
void drawCell(char x, char y);
void drawField();
void hideField();
void setCursor(char x, char y, char color);
void highlightCell(char x, char y);
void highlightCellExplosion(char x, char y);
void printHUD(char player, char color, char move, char ki);
void showPictureFromHandle(FILE* f);
void showPicture(char* filename);
void switchBank(char pos);
void setCharsetPosition(char pos);
unsigned int getSpriteAddress(char spriteidx);




#endif
