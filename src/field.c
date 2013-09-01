#include "field.h"
#include "draw.h"
#include "sound.h"


void setAtoms(char f[SIZEX][SIZEY], char atoms, char x, char y) {
	atoms = atoms > 4 ? 4 : atoms;
	f[x][y] = (f[x][y] & 0xF0) | atoms;
}

char getAtoms(char f[SIZEX][SIZEY], char x, char y) {
	return f[x][y] & 0x0F;
}

void setOwner(char f[SIZEX][SIZEY], char owner, char x, char y) {
	owner &= 0x0F;
	owner = owner << 4;
	f[x][y] = (f[x][y] & 0x0F) | owner;
}

char getOwner(char f[SIZEX][SIZEY], char x, char y) {
	return (f[x][y] & 0xF0) >> 4;
}

char getOwnerCount(char f[SIZEX][SIZEY], char owner) {
	char x,y,count;
	count = 0;
	for(x = 0; x < SIZEX; ++x) {
		for(y = 0; y < SIZEY; ++y) {
			if(getOwner(f,x,y) == owner) {
				count += getAtoms(f, x, y);
			}
		}
	}
	return count;
}



void clearField() {
	char x,y;
	for(x = 0; x < SIZEX; ++x) {
		for(y = 0; y < SIZEY; ++y) {
			setAtoms(field, 0, x, y);
			setOwner(field, 0, x, y);
		}
	}
}


char getCapacity(char x, char y) {
	char capacity = 3;
	if(x == 0 || x == MAXX) --capacity;
	if(y == 0 || y == MAXY) --capacity;
	return capacity;
}


void spreadAtoms(char f[SIZEX][SIZEY], char x, char y, char p, char draw) {
	char i;
	if(x > 0) {
		i = x - 1;
		setAtoms(f, getAtoms(f, i, y) + 1, i, y);
		setOwner(f, p, i, y);
		if(draw) drawAtoms(i,y);
	}
	if(y > 0) {
		i = y - 1;
		setAtoms(f, getAtoms(f, x, i) + 1, x, i);
		setOwner(f, p, x, i);
		if(draw) drawAtoms(x,i);
	}
	if(x < MAXX) {
		i = x + 1;
		setAtoms(f, getAtoms(f, i, y) + 1, i, y);
		setOwner(f, p, i, y);
		if(draw) drawAtoms(i,y);
	}
	if(y < MAXY) {
		i = y + 1;
		setAtoms(f, getAtoms(f, x, i) + 1, x, i);
		setOwner(f, p, x, i);
		if(draw) drawAtoms(x,i);
	}
}


void react(char f[SIZEX][SIZEY], char draw) {
	char stable,count,x,y,iter,player;
	iter = 16;
	stable = 0;
	while(!stable && iter > 0) {
		stable = 1;
		for(x = 0; x < SIZEX; ++x) {
			for(y = 0; y < SIZEY; ++y) {
				count = getAtoms(f, x, y);
				if(count > getCapacity(x, y)) {
					if(draw) {
						soundExplode();
						highlightCellExplosion(x, y);
					}
					player = getOwner(f, x, y);
					stable = 0;
					spreadAtoms(f, x, y, player, draw);
					setAtoms(f, 0, x, y);
					setOwner(f, 0, x, y);
					drawAtoms(x, y);
				}
			}
		}
		--iter;
	}
}

void putAtom(char f[SIZEX][SIZEY], char p, char x, char y, char draw) {
	char atoms;
	char owner = getOwner(f, x, y);
	if(owner != 0 && owner != p) { return; }
	atoms = getAtoms(f, x, y) + 1;
	setAtoms(f, atoms, x, y);
	setOwner(f, p, x, y);
	if(draw) drawAtoms(x, y);
}

