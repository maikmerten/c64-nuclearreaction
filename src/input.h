#ifndef __input_h
#define __input_h

#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <peekpoke.h>

#include "defs.h"

// joystick defs
#define JOY2  0xDC00
#define JOYUP  0x01
#define JOYDOWN  0x02
#define JOYLEFT  0x04
#define JOYRIGHT  0x08
#define JOYFIRE  0x10

char isInputUp();
char isInputDown();
char isInputLeft();
char isInputRight();
char isInputAction();


#endif