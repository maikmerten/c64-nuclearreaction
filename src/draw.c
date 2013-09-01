#include "draw.h"
#include "field.h"
#include "input.h"
#include "sound.h"
#include "petscii.h"

extern char play_music,draw_field,cursor_x,cursor_y,cursor_color;
extern char* sprite_field_ptrs;
extern char* sprite_field_colors;

void clearScreen(char textcolor) {
	bordercolor(0);
	bgcolor(0);
	memset((char*)TEXTBASE,CHAR_SPACE,1000);
	memset((char*)COLORRAM,textcolor,1000);
}

void drawCell(char x, char y) {
	char tl;
	int pos = TEXTBASE;

	if(x == 0) {
		if(y == 0) {
			tl = CHAR_COR_TL;
		} else {
			tl = CHAR_CROSS_L;
		}
	} else {
		if(y == 0) {
			tl = CHAR_CROSS_T;
		} else {
			tl = CHAR_CROSS;
		}
	}

	pos += FIELDOFF;
	pos += (x * 4);
	pos += (y * 160);

	*((char*)pos++) = tl;
	*((char*)pos++) = CHAR_HOR;
	*((char*)pos++) = CHAR_HOR;
	*((char*)pos++) = CHAR_HOR;
	if(x == MAXX) {
		*((char*)pos) = (y == 0 ? CHAR_COR_TR : CHAR_CROSS_R);
	}
	pos += 36;
	*((char*)pos) = CHAR_VER;
	pos += 4;
	if(x == MAXX) {
		*((char*)pos) = CHAR_VER;
	}
	pos += 36;
	*((char*)pos) = CHAR_VER;
	pos += 4;
	if(x == MAXX) {
		*((char*)pos) = CHAR_VER;
	}
	
	pos += 36;
	*((char*)pos) = CHAR_VER;
	pos += 4;
	if(x == MAXX) {
		*((char*)pos) = CHAR_VER;
	}

	if(y == MAXY) {
		pos += 36;
		if(x == 0) {
			*((char*)pos) = CHAR_COR_BL;
		}
		pos += 1;
		*((char*)pos++) = CHAR_HOR;
		*((char*)pos++) = CHAR_HOR;
		*((char*)pos++) = CHAR_HOR;
		*((char*)pos) = (x == MAXX ? CHAR_COR_BR : CHAR_CROSS_B);
	}
}


void highlightCell(char x, char y) {
	char i,j;	

	for(i = 3; i > 0; --i) {
		setCursor(x,y,1);
		soundBleep();
		for(j = 4; j > 0; --j) {
			WAIT_WHILE_RASTERLINE_LOW
			WAIT_WHILE_RASTERLINE_HIGH
		}
		setCursor(x,y,FIELDCOLOR);
		for(j = 4; j > 0; --j) {
			WAIT_WHILE_RASTERLINE_LOW
			WAIT_WHILE_RASTERLINE_HIGH
		}
	}

}


void drawAtoms(char x, char y) {
	char count = getAtoms(field, x, y);
	char color = playercolors[getOwner(field, x, y)];
	int pos = 0;
	char idx = (y * SIZEX) + x;
	
	*((char*)&sprite_field_ptrs + idx) = 200 + count;
	*((char*)&sprite_field_colors + idx) = color;

}

void drawField() {
	char x, y;
	for(x = 0; x < SIZEX; ++x) {
		for(y = 0; y < SIZEY; ++y) {
			drawCell(x,y);
			drawAtoms(x,y);
		}
	}
	play_music = 0;
	soundMute();
	draw_field = 1;
}

void hideField() {
	draw_field = 0;
	play_music = 1;
	VIC.spr_ena = 0;
}


void setCursor(char x, char y, char color) {
	cursor_x = 88 + (x * 32);
	cursor_y = 75 + (y * 32);
	cursor_color = color;
}


unsigned char getBank() {
	char c = *(char*)0xDD00; // CIA 2
	c &= 0x03;
	return c;
}

unsigned int getBankAddress() {
	char base = 3 - getBank();
	return 0x4000 * base;
}

unsigned int getSpriteAddress(char spriteidx) {
	return getBankAddress() + (spriteidx * 64);
}


void switchBank(char pos) {
	// valid values for pos are 0 to 3
	// 0 is VIC bank in lowest memory position, 3 in highest!
	
	int screenAddr;

	// legal range for pos: 0 - 3
	// notice that 0 selects vic bank 3, 1 selects vic bank 2, etc.
	char c = *(char*)0xDD00; // CIA 2
	c &= 252;
	*(char*)0xDD00 = (c | (3 - pos)); // switch VIC base
	
	if(pos != VICBANKBITMAP) {
		// update kernal pointer to screen RAM
		screenAddr = getBankAddress() + 1024;
		// 0x0288 contains high byte of pointer to screen RAM
		*(char*)0x0288 = screenAddr >> 8;
		// kernal updates screen pointer on clear screen
		printf("\f"); // clear screen

		// update pointer to current line in screen RAM
		*(char*)0xD1 = (char) screenAddr; 	// LSB
		*(char*)0xD2 = (char) screenAddr >> 8;	// MSB
	}

}


void showPicture(char* filename) {
	FILE* f;
	int i = 0;
	int vicconf[3];

	vicconf[0] = VIC.addr;
	vicconf[1] = VIC.ctrl1;
	vicconf[2] = VIC.ctrl2;

	VIC.spr_ena = 0;

	f = fopen(filename, "r");
	
	// consume two bytes of header
	i = fgetc(f);
    i = fgetc(f);
	
	// 8000 bytes bitmap
	fread((char*)(BANK3BASE + 8192), 1, 8000, f);

	// 1000 bytes char mem
	fread((char*)(BANK3BASE + 0x0400), 1, 1000, f);

	// clear screen before loading color data to avoid garbage-colored text
	clearScreen(0);
	
	// 1000 bytes color mem
	fread((char*)(0xD800), 1, 1000, f);

	// 1 byte background color
	bgcolor((char) fgetc(f));
	
	// close input file
	fclose(f);

	// now that we copied the bitmap data, switch bank
	switchBank(VICBANKBITMAP);

	// multicolor on
	// POKE 53265,PEEK(53265) OR 32 : POKE 53270,PEEK(53270) OR 16
	VIC.addr = VIC.addr | 8; // bitmap at 8192
	VIC.ctrl1 = VIC.ctrl1 | 32;
	VIC.ctrl2 = VIC.ctrl2 | 16;

	while(1){
		if(isInputAction()) break;		
	}

	// put VIC back into previous state
	VIC.addr = vicconf[0];
	VIC.ctrl1 = vicconf[1];
	VIC.ctrl2 = vicconf[2];

	// back to bank for text output
	switchBank(VICBANKTEXT);

	clrscr();
	bgcolor(0);
}

void setCharsetPosition(unsigned char pos) {
	// legal range for pos: 0 - 7
	char c = *(char*)0xD018; // VIC mem layout register
	// bits 1 to 3 in 0xD018 determine charset offset in bank, so shift pos
	pos = pos << 1;
	// mask bits 1 to 3 to zero
	c &= 0xf1;
	c |= pos;
	*(char*)0xD018 = c;
}

void displayPlayerSprite(char player) {
	int xpos = SPRITE_PLAYER_X;
	WAIT_WHILE_RASTERLINE_LOW

	if(player == PLAYERAI && ki) {
		*((char*)SPRITE0_PTR) = COMPUTER_PTR;
	} else {
		*((char*)SPRITE0_PTR) = HUMAN_PTR;
	}
	
	if(player > 1) xpos += 255;
	VIC.spr_color[0] = playercolors[player];
	VIC.spr_pos[0].x = (char)xpos & 0xFF;
	VIC.spr_pos[0].y = SPRITE_PLAYER_Y;
	VIC.spr_exp_x = 1;
	VIC.spr_exp_y = 1;
	VIC.spr_hi_x = xpos > 255 ? 1 : 0;
}

void printHUD(char player, char color, char move, char ki) {
	char buf[9];
	displayPlayerSprite(player);
	textcolor(color);
	if(player == 1) cputsxy(8,0, "active player: player 1");
	else {
		if(ki) cputsxy(8,0, "computer is thinking...   ");
		else cputsxy(8,0, "active player: player 2");
	}
	sprintf(buf, "move  %-3i", move);
	cputsxy(16,24, buf);
}

