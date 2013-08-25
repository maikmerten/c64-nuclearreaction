#include "ai.h"
#include "field.h"

unsigned char fieldAI[SIZEX][SIZEY];

char isCritical(char f[SIZEX][SIZEY], char x, char y) {
	if(getAtoms(f, x, y) == getCapacity(x, y)) {
		return 1;
	}
	return 0;
}


char computeDanger(char f[SIZEX][SIZEY], char p, char x, char y) {
	char tmp;
	char danger = 0;

	if(x > 0) {
		tmp = x - 1;
		if(getOwner(f, tmp, y) != p) danger += isCritical(f, tmp, y);
	}
	if(y > 0) {
		tmp = y - 1;
		if(getOwner(f, x, tmp) != p) danger += isCritical(f, x, tmp);
	}
	if(x < MAXX) {
		tmp = x + 1;
		if(getOwner(f, tmp, y) != p) danger += isCritical(f, tmp, y);
	}
	if(y < MAXY) {
		tmp = y + 1;
		if(getOwner(f, x, tmp) != p) danger += isCritical(f, x, tmp);
	}

	return danger;
}

char countEndangered(char f[SIZEX][SIZEY], char p) {
	unsigned char x,y,danger;
	danger = 0;
	for(x = 0; x < SIZEX; ++x) {
		for(y = 0; y < SIZEY; ++y) {
			if(getOwner(f, x, y) == p && computeDanger(f, p, x, y)) ++danger;
		}
	}
	return danger;
}

signed int evaluateField(char f[SIZEX][SIZEY], char p) {
	unsigned char x,y,owner,otherplayer;
	int score = 0;
	otherplayer = p == 1 ? 2 : 1;
	for(x = 0; x < SIZEX; ++x) {
		for(y = 0; y < SIZEY; ++y) {
			owner = getOwner(f, x, y);
			if(owner == p) {
				 // the more cells, the better, but prefer corners and edges
				score += 4 - getCapacity(x, y);
				// small bonus for fields that are ready to explode
				score += isCritical(f, x, y);
				
			}
		}
	}
	 // endangered fields are dangerous...
	score -= (countEndangered(f, p) << 1);
	// ... unless they belong to the other player
	score += countEndangered(f, otherplayer);
	
	return score;
}


int thinkAI() {
	unsigned char x,y,size,owner;
	signed int tmp,score,oppscore,loss;
	unsigned int result;
	size = SIZEX * SIZEY;
	// compute the field score for the opposing player
	oppscore = evaluateField(field, 1);
	score = -32000;
	for(x = 0; x < SIZEX; ++x) {
		for(y = 0; y < SIZEY; ++y) {
			owner = getOwner(field, x, y);
			if(owner == PLAYERAI || owner == 0) {
				// we can use this cell
				memcpy(fieldAI, field, size); // create working copy
				tmp = 0;
				
				// it makes little sense to add atoms to endangered cells
				// unless they can start a chain reaction
				if(computeDanger(fieldAI, PLAYERAI, x, y) > 0 && isCritical(fieldAI, x, y) == 0) {
					tmp -= 10;
				}

				// let the reaction run
				putAtom(fieldAI, PLAYERAI, x, y, 0);
				react(fieldAI, 0);
				
				// evaluate the resulting field constellation
				tmp += evaluateField(fieldAI, PLAYERAI);
				// add loss of the opposing player
				loss = oppscore - evaluateField(fieldAI, 1);
				tmp += loss;

				if(tmp > score || (tmp == score && (rand() & 0x01))) {
					score = tmp;
					result = (x << 8) | y;
				}
			}
		}
	}
	return result;
}

