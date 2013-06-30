#include "input.h"

char readJoysticks(char mask) {
	char joy = PEEK(JOY2) & 0x1f;
	char result = (!(joy & mask));
	joy = PEEK(JOY1) & 0x1f;
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
