#include "input.h"


char isInputUp() {
	char joy = PEEK(JOY2) & 0x1f;
	return (!(joy & JOYUP));
}

char isInputDown() {
	char joy = PEEK(JOY2) & 0x1f;
	return (!(joy & JOYDOWN));
}


char isInputLeft() {
	char joy = PEEK(JOY2) & 0x1f;
	return (!(joy & JOYLEFT));
}


char isInputRight() {
	char joy = PEEK(JOY2) & 0x1f;
	return (!(joy & JOYRIGHT));
}

char isInputAction() {
	char joy = PEEK(JOY2) & 0x1f;
	return (!(joy & JOYFIRE));
}
