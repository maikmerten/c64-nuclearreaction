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


int thinkAI() {
	unsigned char x,y,oppscore,size;
	signed char tmp,score;
	unsigned int result;
	size = SIZEX * SIZEY;
	oppscore = getOwnerCount(field, 1);
	score = -127;
	for(x = 0; x < SIZEX; ++x) {
		for(y = 0; y < SIZEY; ++y) {
			memcpy(fieldAI, field, size);
			tmp = getOwner(fieldAI, x, y);
			if(tmp == PLAYERAI || tmp == 0) {
				tmp = 0;
				// we can use this cell
				putAtom(fieldAI, PLAYERAI, x, y, 0);
				react(fieldAI, 0);
				tmp += getOwnerCount(fieldAI, PLAYERAI);
				// compute lost enemy fields
				tmp += oppscore - getOwnerCount(fieldAI, 1);
				tmp += getAtoms(fieldAI, x, y);
				tmp += ((x == 0 || x == MAXX) && (y == 0 || y == MAXY)) ? 1 : 0;
				tmp -= (computeDanger(fieldAI, PLAYERAI, x, y)) << 2;
				tmp -= countEndangered(fieldAI, PLAYERAI);
				if(tmp > score || (tmp == score && (rand() & 0x01))) {
					score = tmp;
					result = (x << 8) | y;

				}

			}
		}
	}
	return result;
}

