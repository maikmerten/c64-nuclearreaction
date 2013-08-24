#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

#include "defs.h"
#include "draw.h"
#include "ai.h"
#include "input.h"
#include "game.h"
#include "petscii.h"


extern void setupInterrupt(); // asm.s
extern void disableInterrupt(); // asm.s
extern char colorwashrow; // asm.s
extern char colorwashcolstart; // asm.s
extern char colorwashcolend; // asm.s

unsigned char help;


void showhelp() {
	char x,y;
	FILE* f;
	int i;

	x = 0;
	y = 0;

	clearScreen(1);
	textcolor(14);

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
	char up,down,quit,selected,wait,update;
	char cnt = 0;
	signed char item = 0;

	update = 1;
	selected = 0;

	clearScreen(1);

	// draw menu border
	fillRect(0,0,40,1,CHAR_FULL,14);
	fillRect(0,1,1,23,CHAR_FULL,14);
	fillRect(39,1,1,23,CHAR_FULL,14);
	fillRect(0,24,40,1,CHAR_FULL,14);
	fillRect(0,0,1,1,CHAR_CHIP_TL,14);
	fillRect(39,0,1,1,CHAR_CHIP_TR,14);
	fillRect(0,24,1,1,CHAR_CHIP_BL,14);
	fillRect(39,24,1,1,CHAR_CHIP_BR,14);

	textcolor(10);
	cputsxy(8, 0, " nuclear reaction 2100 ");

	while(!selected) {
		if (up) {
			--item;
			update = 1;
		}
		if (down) {
			++item;
			update = 1;
		}

		item = (item > 3) ? 3 : item;
		item = (item < 0) ? 0 : item;

		// set up row and column for color wash effect
		colorwashrow = 4 + (3 * item);
		colorwashcolstart = 6;
		colorwashcolend = 31;
		
		if(update) {
			textcolor(15);
			cputsxy(3, 4, "   match against the computer");
			cputsxy(3, 7, "   match between two humans");
			cputsxy(3, 10, "   help and credits");
			cputsxy(3, 13, "   quit");
			textcolor(1);
			cputsxy(4, 3 * item + 4, "Q");
			update = 0;
		}
			

		for(wait = 8; wait > 0; --wait) {
			WAIT_WHILE_RASTERLINE_LOW
			up = isInputUp();
			down = isInputDown();
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
	FILE* f;

	// switch second-highest memory area
	switchBank(2);
	
	// block switching character sets with Shift+C=
	*((char*)0x0291) = 128;

	// upload custom font into bank for text display
	f = fopen("font","r");
	fread((char*)(32768 + 14336), 1, 2048, f);
	fclose(f);
	
	bordercolor(0);
	bgcolor(0);
	clearScreen(1);
	textcolor(1);
	
	// enable custom charset
	setCharsetPosition(7);
	cputsxy(15,12, "loading...");
	showPicture("title");

	setupInterrupt();
	
	while(1) {
		
		if(gamemenu()) break;
		colorwashrow = 25; // disable colorwash
		if(help) {
			showhelp();
		} else {
			gameloop();
			VIC.spr_ena = 0;
		}
	}

	disableInterrupt();

	// restore proper screen setup on exit
	clearScreen(1);
	bordercolor(14);
	bgcolor(6);
	textcolor(14);

	// unblockblock switching character sets with Shift+C=
	*((char*)0x0291) = 0;

	// switch to original VIC bank (lowest memory position)
	switchBank(0);
	
	// enable original upper-case charset
	setCharsetPosition(2);
	
	return EXIT_SUCCESS;
}
