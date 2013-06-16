#include "draw.h"


void clearScreen(char textcolor) {
	bordercolor(0);
	bgcolor(0);
	memset((int*)TEXTBASE,CHAR_SPACE,1000);
	memset((int*)COLORRAM,textcolor,1000);
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
	pos += (x * 3);
	pos += (y * 120);

	*((char*)pos++) = tl;
	*((char*)pos++) = CHAR_HOR;
	*((char*)pos++) = CHAR_HOR;
	if(x == MAXX) {
		*((char*)pos) = (y == 0 ? CHAR_COR_TR : CHAR_CROSS_R);
	}
	pos += 37;
	*((char*)pos) = CHAR_VER;
	pos += 3;
	if(x == MAXX) {
		*((char*)pos) = CHAR_VER;
	}
	pos += 37;
	*((char*)pos) = CHAR_VER;
	pos += 3;
	if(x == MAXX) {
		*((char*)pos) = CHAR_VER;
	}

	if(y == MAXY) {
		pos += 37;
		if(x == 0) {
			*((char*)pos) = CHAR_COR_BL;
		}
		pos += 1;
		*((char*)pos++) = CHAR_HOR;
		*((char*)pos++) = CHAR_HOR;
		*((char*)pos) = (x == MAXX ? CHAR_COR_BR : CHAR_CROSS_B);
	}
}

void setCellColor(char color, char x, char y) {
	int pos = COLORRAM;
	pos += FIELDOFF;
	pos += (x * 3);
	pos += (y * 120);
	++pos;

	*((char*)pos++) = color;
	*((char*)pos++) = color;
	pos += 37;
	*((char*)pos) = color;
	pos += 3;
	*((char*)pos) = color;
	pos += 37;
	*((char*)pos) = color;
	pos += 3;
	*((char*)pos++) = color;
	pos += 37;
	*((char*)pos++) = color;
	*((char*)pos++) = color;
}

void drawAtoms(char x, char y) {
	char count = getAtoms(field, x, y);
	char color = playercolors[getOwner(field, x, y)];
	int pos = 0;

	pos += FIELDOFF;
	pos += (x * 3);
	pos += (y * 120);
	pos += 41;
	*((char*)(COLORRAM + pos)) = color;
	*((char*)(TEXTBASE + pos++)) = count > 0 ? CHAR_ATOM : CHAR_SPACE;

	*((char*)(COLORRAM + pos)) = color;
	*((char*)(TEXTBASE + pos++)) = count > 1 ? CHAR_ATOM : CHAR_SPACE;
	pos += 38;
	*((char*)(COLORRAM + pos)) = color;
	*((char*)(TEXTBASE + pos++)) = count > 2 ? CHAR_ATOM : CHAR_SPACE;

	*((char*)(COLORRAM + pos)) = color;
	*((char*)(TEXTBASE + pos++)) = count > 3 ? CHAR_ATOM : CHAR_SPACE;
}

void drawField() {
	char x, y;
	for(x = 0; x < SIZEX; ++x) {
		for(y = 0; y < SIZEY; ++y) {
			drawCell(x,y);
			drawAtoms(x,y);
		}
	}
}


void switchBank(char base) {
	char c = *(int*)0xDD00; // CIA 2
	c &= 252;
	*(int*)0xDD00 = (c | (3 - base)); // switch VIC base
}


void showPicture(int imgdata) {
	int i = 0;
	int vicconf[3];

	vicconf[0] = VIC.addr;
	vicconf[1] = VIC.ctrl1;
	vicconf[2] = VIC.ctrl2;

	VIC.spr_ena = 0;

	// 8000 bytes bitmap
	memcpy((int*)(BANK3BASE + 8192), (int*)imgdata, 8000);

	// 1000 bytes char mem
	memcpy((int*)(BANK3BASE + 0x0400), (int*)(imgdata + 8000), 1000);

	// 1000 bytes color mem
	memcpy((int*)(0xD800), (int*)(imgdata + 9000), 1000);

	// 1 byte background color
	bgcolor(*((int*)(imgdata + 10000)));

	// now that we copied the bitmap data, switch bank
	switchBank(3);

	// multicolor on
	// POKE 53265,PEEK(53265) OR 32 : POKE 53270,PEEK(53270) OR 16
	VIC.addr = VIC.addr | 8; // bitmap at 8192
	VIC.ctrl1 = VIC.ctrl1 | 32;
	VIC.ctrl2 = VIC.ctrl2 | 16;

	while(1){
		if(!(PEEK(JOY2) & 0x1f & JOYFIRE)) break;		
	}

	// put VIC back into previous state
	VIC.addr = vicconf[0];
	VIC.ctrl1 = vicconf[1];
	VIC.ctrl2 = vicconf[2];

	switchBank(0);

	clrscr();
	bgcolor(0);
}


void displayPlayerSprite(char player) {
	int xpos = SPRITE_PLAYER_X;
	VIC.spr_ena = 0;
	WAIT_WHILE_RASTERLINE_LOW
	if(player == PLAYERAI && ki) memcpy((int*) SPRITE0_DATA, sprComputer, 63);
	else memcpy((int*) SPRITE0_DATA, sprHuman, 63);
	*((char*)SPRITE0_PTR) = SPRITE0_DATA >> 6;
	if(player > 1) xpos += 230;
	VIC.spr_ena = 1;
	VIC.spr_color[0] = playercolors[player];
	VIC.spr_pos[0].x = (char)xpos & 0xFF;
	VIC.spr_pos[0].y = SPRITE_PLAYER_Y;
	VIC.spr_exp_x = 1;
	VIC.spr_exp_y = 1;
	VIC.spr_hi_x = xpos > 255 ? 1 : 0;
}

