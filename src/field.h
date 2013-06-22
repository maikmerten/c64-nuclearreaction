#ifndef __field_h
#define __field_h

#include "defs.h"
#include "draw.h"


extern unsigned char field[SIZEX][SIZEY];

void setAtoms(char f[SIZEX][SIZEY], char atoms, char x, char y);
char getAtoms(char f[SIZEX][SIZEY], char x, char y);
void setOwner(char f[SIZEX][SIZEY], char owner, char x, char y);
char getOwner(char f[SIZEX][SIZEY], char x, char y);
char getOwnerCount(char f[SIZEX][SIZEY], char owner);
void clearField();
char getCapacity(char x, char y);
void spreadAtoms(char f[SIZEX][SIZEY], char x, char y, char p, char draw);
void react(char f[SIZEX][SIZEY], char draw);
void putAtom(char f[SIZEX][SIZEY], char p, char x, char y, char draw);


#endif
