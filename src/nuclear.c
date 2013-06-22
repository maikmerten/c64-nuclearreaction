#include <stdio.h>
#include <conio.h>
#include <string.h>

#include "defs.h"
#include "draw.h"
#include "ai.h"
#include "input.h"
#include "game.h"


extern const char titlecard[]; // text.s

unsigned char help;


void showhelp() {
	char x,y;
	FILE* f;
	int i;

	x = 0;
	y = 0;

	clearScreen(1);

	f = fopen("readme", "r");

	i = fgetc(f);
	while(i != EOF) {
		if(i != 0x0A) { // newline is unprintable
			cputsxy(x++, y, (char*)&i);
		} else {
			x = 0;
			++y;
		}
		i = fgetc(f);
	}


	fclose(f);

	while(1) {
		if(isInputAction()) break;
	}

}


char gamemenu() {
	char quit,selected,wait,update;
	char cnt = 0;

	signed char item = 0;
	update = 1;
	selected = 0;

	// enable charset 2
	*((char*)53272) = 23;

	clearScreen(1);
	textcolor(1);
	cputsxy(4, 0, "--- Nuclear Reaction 2100 ---");
	
	while(!selected) {
		if (isInputUp()) {
			--item;
			update = 1;
		}
		if (isInputDown()) {
			++item;
			update = 1;
		}

		item = (item > 3) ? 3 : item;
		item = (item < 0) ? 0 : item;

		if(update) {
			textcolor(15);
			cputsxy(3, 4, "   Match against the computer");
			cputsxy(3, 7, "   Match between two humans");
			cputsxy(3, 10, "   Help");
			cputsxy(3, 13, "   Quit");
			cputsxy(4, 3 * item + 4, "*");
			update = 0;
		}
			

		for(wait = 8; wait > 0; --wait) {
			WAIT_WHILE_RASTERLINE_LOW
			selected = (isInputAction()) ? 1 : 0;
			WAIT_WHILE_RASTERLINE_HIGH
		}
	};

	ki = (item == 0) ? 1 : 0;
	help = (item == 2) ? 1 : 0;
	quit = (item == 3) ? 1 : 0;

	return quit;
}



int main(void) {

	bordercolor(0);
	showPicture((int)&titlecard);

	while(1) {
		if(gamemenu()) break;
		if(help) {
			showhelp();
		} else {
			gameloop();
			VIC.spr_ena = 0;
		}
	}

	return EXIT_SUCCESS;
}
