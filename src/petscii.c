#include "petscii.h"

void fillRect(char x, char y, char width, char height, char character, char color) {
	int offset;
	char i,j;

	for(i = 0; i < width; ++i) {
		for(j = 0; j < height; ++j) {
			offset = ((j+y) * 40) + (i+x);
			*((char*)(COLORRAM + offset)) = color;
			*((char*)(TEXTBASE + offset)) = character;
		}
	}


}
