#ifndef __ai_h
#define __ai_h

#include "defs.h"

char isCritical(char f[SIZEX][SIZEY], char x, char y);
char computeDanger(char f[SIZEX][SIZEY], char p, char x, char y);
char countEndangered(char f[SIZEX][SIZEY], char p);
int thinkAI();


#endif
