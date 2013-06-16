#include <stdio.h>
#include <conio.h>
#include <string.h>

#include "defs.h"
#include "draw.h"
#include "ai.h"


extern const char titlecard[]; // text.s

unsigned char help;

unsigned char field[SIZEX][SIZEY];
unsigned char ki;
unsigned char playercolors[3] = {0, 10, 14 };
unsigned char playerplayed[3];
unsigned char winner;
unsigned char move;
signed char posx,posy; // signed for simple detection of underflow



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




void printHUD(char player) {
	char buf[9];
	char color = playercolors[player];
	displayPlayerSprite(player);
	textcolor(color);
	if(player == 1) cputsxy(8,0, "active player: player 1");
	else {
		if(ki) cputsxy(8,0, "computer is thinking...   ");
		else cputsxy(8,0, "active player: player 2");
	}
	sprintf(buf, "move  %-3i", move);
	cputsxy(16,20, buf);
}


void gameloop() {
	char joy,fire,wait,owner,player,updateHUD,aix,aiy;
	int airesult;

	// enable charset 1
	*((char*)53272) = 21;

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

	while(!winner) {
		WAIT_WHILE_RASTERLINE_LOW

		joy=PEEK(JOY2) & 0x1f;
		setCellColor(FIELDCOLOR, posx, posy);

		if (!(joy & JOYUP)) --posy;
		if (!(joy & JOYDOWN)) ++posy;
		if (!(joy & JOYLEFT)) --posx;
		if (!(joy & JOYRIGHT)) ++posx;

		posx = posx < 0 ? 0 : posx;
		posx = posx >= SIZEX ? SIZEX - 1 : posx;
		posy = posy < 0 ? 0 : posy;
		posy = posy >= SIZEY ? SIZEY - 1 : posy;

		setCellColor(playercolors[player], posx, posy);
		owner = getOwner(field, posx, posy);

		fire = 0;

		if(player == 1 || !ki) {
			// puny human
			if (!(joy & JOYFIRE)) fire = 1;
			
			if(fire && (owner == 0 || owner == player)) {
				putAtom(field, player, posx, posy, 1);
				react(field, 1);
				winner = checkWinner(field);
				player = (player == 1) ? 2 : 1;
				++move;
				updateHUD = 1;
			}
		} else {
			// mighty computer
			airesult = thinkAI();
			aix = airesult >> 8;
			aiy = airesult & 0xFF;
			putAtom(field, player, aix, aiy, 1);
			react(field, 1);
			winner = checkWinner(field);
			player = (player == 1) ? 2 : 1;
			updateHUD = 1;
			++move;
		}

		if(kbhit()) {
			if(cgetc() == 57) break;
		}

		if(!winner && updateHUD) {
			printHUD(player);
			updateHUD = 0;
		}

		WAIT_WHILE_RASTERLINE_HIGH
		for(wait = 5; wait > 0; --wait) {
			WAIT_WHILE_RASTERLINE_LOW
			srand(rand());
			WAIT_WHILE_RASTERLINE_HIGH
		}
	}

	textcolor(1);
	if(winner == 1) cputsxy(8, 22, "player 1 won");
	else if(winner == 2) {
		if(ki) cputsxy(8, 22, "the computer won");
		else cputsxy(8, 22, "player 2 won");
	}

	wait = 0;
	cputsxy(8, 24, "press fire to continue");
	do {
		WAIT_WHILE_RASTERLINE_HIGH
		joy=PEEK(JOY2) & 0x1f;
		fire = 0;
		if (!(joy & JOYFIRE)) fire = 1;
		VIC.spr_pos[0].y = (SPRITE_PLAYER_Y + (wait >> 7));
		++wait;
		WAIT_WHILE_RASTERLINE_HIGH
	} while(!fire);

}


void showhelp() {
	char joy,x,y;
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
		joy=PEEK(JOY2) & 0x1f;
		if(!(joy & JOYFIRE)) break;
	}

}


char gamemenu() {
	char quit,joy,selected,wait,update;
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
		joy=PEEK(JOY2) & 0x1f;
		if (!(joy & JOYUP)) {
			--item;
			update = 1;
		}
		if (!(joy & JOYDOWN)) {
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
			joy=PEEK(JOY2) & 0x1f;
			selected = (!(joy & JOYFIRE)) ? 1 : 0;
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
