#include "game.h"


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


void gameloop() {
	char fire,wait,owner,player,updateHUD,aix,aiy;
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

		setCellColor(FIELDCOLOR, posx, posy);

		if (isInputUp()) --posy;
		if (isInputDown()) ++posy;
		if (isInputLeft()) --posx;
		if (isInputRight()) ++posx;

		posx = posx < 0 ? 0 : posx;
		posx = posx >= SIZEX ? SIZEX - 1 : posx;
		posy = posy < 0 ? 0 : posy;
		posy = posy >= SIZEY ? SIZEY - 1 : posy;

		setCellColor(playercolors[player], posx, posy);
		owner = getOwner(field, posx, posy);

		fire = 0;

		if(player == 1 || !ki) {
			// puny human
			if (isInputAction()) fire = 1;
			
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
			printHUD(player, playercolors[player], move, ki);
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

	fire = 0;
	do {
		WAIT_WHILE_RASTERLINE_HIGH
		fire = isInputAction();
		VIC.spr_pos[0].y = (SPRITE_PLAYER_Y + (wait >> 7));
		++wait;
		WAIT_WHILE_RASTERLINE_HIGH
	} while(!fire);

}

