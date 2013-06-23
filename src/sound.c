#include "sound.h"


void soundBleep() {
	SID.amp = 15;
	SID.v3.freq = 10000;
	SID.v3.ad = 0x22; // attack, delay
	SID.v3.sr = 0x09; // sustain, release
	SID.v3.ctrl = 0x11; // triangle wave, set attack bit
	SID.v3.ctrl = 0x10; // release attack bit
}

void soundExplode() {
	SID.amp = 15;
	SID.v2.freq = 2000;
	SID.v2.ad = 0x22; // attack, delay
	SID.v2.sr = 0xA8; // sustain, release
	SID.v2.ctrl = 0x21; // square wave, set attack bit
	SID.v2.ctrl = 0x20; // release attack bit
}
