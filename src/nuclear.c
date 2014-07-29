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
#include "sound.h"


extern void setupInterrupt(); // asm.s
extern void disableInterrupt(); // asm.s
extern void enableTurbo(); // asm.s
extern void disableTurbo(); // asm.s
extern char colorwashrow; // asm.s
extern char colorwashcolstart; // asm.s
extern char colorwashcolend; // asm.s

// binary includes from asm.s
extern char* customfont, music;
extern char* spr_cell0,spr_cell1,spr_cell2,spr_cell3,spr_cell4,spr_human,spr_computer,spr_explosion,spr_cursor;

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

void showhelp2() {
	clearScreen(1);

	textcolor(1);
	cputsxy(0,0,  "about this game:");

	textcolor(14);
	cputsxy(0,2,  "original amiga game developed by robert");
	cputsxy(0,3,  "akerberg. music by richard bayliss.");
	cputsxy(0,4,  "developed for the c64 by maik merten.");

	textcolor(1);
	cputsxy(0,6,  "how to play:");

	textcolor(14);
	cputsxy(0,8,  "players place atoms in turns. each");
	cputsxy(0,9,  "field explodes when reaching a certain");
	cputsxy(0,10, "number of atoms.");

	cputsxy(0,12, "corners: 2 atoms");
	cputsxy(0,13, "  edges: 3 atoms");
	cputsxy(0,14, "  other: 4 atoms");

	cputsxy(0,16, "upon explosion fields distribute atoms");
	cputsxy(0,17, "into neighbouring fields, which are now");
	cputsxy(0,18, "owned by the owner of the exploding");
	cputsxy(0,19, "field. if newly aquired fields reach");
	cputsxy(0,20, "their threshold, they in turn explode,");
	cputsxy(0,21, "possibly causing a chain reaction.");

	cputsxy(0,23, "a player wins if no fields are left to");
	cputsxy(0,24, "the other player.");

	while(1) {
		char wait;
		for(wait = 8; wait > 0; --wait) {
			WAIT_WHILE_RASTERLINE_LOW
			WAIT_WHILE_RASTERLINE_HIGH
		}
		if(isInputAction()) break;
	}
}

char isChameleon() {
	char result = 0;
	char* ptr = (char*)0xD0FE;
	char val;
	
	*ptr = 0x2A;
	val = *ptr;
	if(val != 0xFF) {
		result = 1;
	}
	*ptr = 0xFF;
	
	return result;
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
			if(isChameleon()) {
				cputsxy(32, 22, "turbo!");
			}
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

void loadAssets() {
	
	bordercolor(0);
	bgcolor(0);
	clearScreen(1);

	// upload custom font into bank for text display
	memcpy((char*)(32768 + 14336),(char*)&customfont, 2048);

	// enable custom charset
	setCharsetPosition(7);
	
	// loading sprite for empty game field cell
	memcpy((char*)getSpriteAddress(CELL0_PTR),(char*)&spr_cell0, 63);
	// loading sprite for game field cell with 1 atom
	memcpy((char*)getSpriteAddress(CELL1_PTR),(char*)&spr_cell1, 63);
	// loading sprite for game field cell with 2 atoms
	memcpy((char*)getSpriteAddress(CELL2_PTR),(char*)&spr_cell2, 63);
	// loading sprite for game field cell with 3 atoms
	memcpy((char*)getSpriteAddress(CELL3_PTR),(char*)&spr_cell3, 63);
	// loading sprite for game field cell with 4 atoms
	memcpy((char*)getSpriteAddress(CELL4_PTR),(char*)&spr_cell4, 63);
	// loading sprite for game field cursor
	memcpy((char*)getSpriteAddress(CURSOR_PTR),(char*)&spr_cursor, 63);
	// loading sprite for human player
	memcpy((char*)getSpriteAddress(HUMAN_PTR),(char*)&spr_human, 63);
	// loading sprite for computer player
	memcpy((char*)getSpriteAddress(COMPUTER_PTR),(char*)&spr_computer, 63);
	// loading sprite for explosion
	memcpy((char*)getSpriteAddress(EXPLOSION_PTR),(char*)&spr_explosion, 63);

	// copy up to 4096 of SID tune
	memcpy((char*)0x7000, (char*)&music, 4096);
}


int main(void) {
	// switch second-highest memory area
	switchBank(2);
	
	// block switching character sets with Shift+C=
	*((char*)0x0291) = 128;

	loadAssets();

	setupInterrupt();
	enableTurbo();
	
	while(1) {
		if(gamemenu()) break;
		colorwashrow = 25; // disable colorwash
		if(help) {
			showhelp2();
		} else {
			// consume all key presses
			while(kbhit()) cgetc();
			gameloop();
			hideField();
		}
	}

	disableTurbo();
	disableInterrupt();
	
	soundMute();

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
