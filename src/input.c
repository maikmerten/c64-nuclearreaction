#include "input.h"

char readJoysticks(char mask) {
	char joy, result;
	POKE(56322,224); // disable keyboard
	joy = PEEK(JOY2) & 0x1f;
	result = (!(joy & mask));
	joy = PEEK(JOY1) & 0x1f;
	POKE(56322,255); // enable keyboard
	result |= (!(joy & mask));
	return result;
}


char isInputUp() {
	return readJoysticks(JOYUP);
}

char isInputDown() {
	return readJoysticks(JOYDOWN);
}


char isInputLeft() {
	return readJoysticks(JOYLEFT);
}


char isInputRight() {
	return readJoysticks(JOYRIGHT);
}

char isInputAction() {
	return readJoysticks(JOYFIRE);
}

char isInputExit() {
	if(kbhit()) {
		// check for escape key
		if(cgetc() == 3) {
			return 1;
		}
	}
	return 0;
}
