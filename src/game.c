#include "game.h"
#include "ai.h"
#include "draw.h"
#include "field.h"
#include "input.h"

extern char colorwashrow; // asm.s
extern char colorwashcolstart; // asm.s
extern char colorwashcolend; // asm.s

unsigned char field[SIZEX][SIZEY];
unsigned char ki;
unsigned char playercolors[3] = {0, 10, 14 };
unsigned char player;
unsigned char playerplayed[3];
unsigned char winner;
unsigned char move;
unsigned char updateHUD;

char checkWinner(char f[SIZEX][SIZEY]) {
	char p1,p2,winner;
	winner = 0;
	p1 = getOwnerCount(f, 1);
	p2 = getOwnerCount(f, 2);

	if(playerplayed[1] && !p1) {
		winner = 2;
	} else if(playerplayed[2] && !p2) {
		winner = 1;
	}
	return winner;
}

void doMove(signed char x, signed char y,) {
	highlightCell(x,y);
	putAtom(field, player, x, y, 1);
	playerplayed[player] = 1;
	react(field, 1);
	winner = checkWinner(field);
	player = (player == 1) ? 2 : 1;
	++move;
	updateHUD = 1;
}


void gameloop() {
	signed char posx,posy; // signed for simple detection of underflow
	char up,down,left,right,fire,wait,owner;
	int airesult;
	char textbuf[28];

	clearScreen(FIELDCOLOR);
	clearField();
	drawField();

	player = 1;
	playerplayed[1] = 0;
	playerplayed[2] = 0;
	winner = 0;
	posx = 0;
	posy = 0;
	move = 1;
	updateHUD = 1;
	up = down = left = right = 0;

	while(!winner) {
		WAIT_WHILE_RASTERLINE_LOW

		if (up) --posy;
		if (down) ++posy;
		if (left) --posx;
		if (right) ++posx;

		posx = posx < 0 ? 0 : posx;
		posx = posx >= SIZEX ? SIZEX - 1 : posx;
		posy = posy < 0 ? 0 : posy;
		posy = posy >= SIZEY ? SIZEY - 1 : posy;

		setCursor(posx, posy, playercolors[player]);
		owner = getOwner(field, posx, posy);

		fire = 0;

		if(player == 1 || !ki) {
			// puny human
			if (isInputAction()) fire = 1;
			
			if(fire && (owner == 0 || owner == player)) {
				doMove(posx, posy);
			}
		} else {
			// mighty computer
			airesult = thinkAI();
			doMove((char)(airesult >> 8),(char)(airesult & 0xFF));
		}

		if(isInputExit()) break;

		if(!winner && updateHUD) {
			printHUD(player, playercolors[player], move, ki);
			updateHUD = 0;
		}

		WAIT_WHILE_RASTERLINE_HIGH
		for(wait = 5; wait > 0; --wait) {
			WAIT_WHILE_RASTERLINE_LOW
			up = isInputUp();
			down = isInputDown();
			left = isInputLeft();
			right = isInputRight();
			srand(rand());
			WAIT_WHILE_RASTERLINE_HIGH
		}
	}

	posx = 8;
	posy = 24;
	if(winner == 1) {
		sprintf(textbuf, "player 1 won in %i moves", move);
	} else if(winner == 2) {
		if(ki) {
			sprintf(textbuf, "the computer won in %i moves", move);
			posx -= 2;
		} else {
			sprintf(textbuf, "player 2 won in %i moves", move);
		}
	}

	if(winner != 0) {
		cputsxy(posx, posy, textbuf);
		// enable triumphant color wash
		colorwashrow = 24;
		colorwashcolstart = 0;
		colorwashcolend = 39;
		
		wait = 0;
		fire = 0;
		do {
			WAIT_WHILE_RASTERLINE_HIGH
			fire = isInputAction();
			VIC.spr_pos[0].y = (SPRITE_PLAYER_Y + (wait >> 7));
			++wait;
			WAIT_WHILE_RASTERLINE_HIGH
		} while(!fire);
	}
	colorwashrow = 25;
}

