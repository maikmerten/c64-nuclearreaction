nuclear: src/nuclear.c src/draw.h src/draw.c src/field.h src/field.c src/ai.h src/ai.c src/input.h src/input.c src/game.h src/game.c src/sound.h src/sound.c src/petscii.h src/petscii.c src/asm.s
	cl65 -m memorymap.txt -O -o nuclear.prg src/nuclear.c src/draw.c src/field.c src/ai.c src/input.c src/game.c src/sound.c src/petscii.c src/asm.s
	cp empty.d64 nuclear.d64
	c1541 -attach nuclear.d64 -write nuclear.prg nuclear.prg
	c1541 -attach nuclear.d64 -write src/resources/readme.txt readme
	c1541 -attach nuclear.d64 -write src/resources/title.koa title
	c1541 -attach nuclear.d64 -write src/resources/nuclearfont font
	c1541 -attach nuclear.d64 -write src/resources/time_portal_7000.sid music
	c1541 -attach nuclear.d64 -write src/resources/cell0.raw cell0
	c1541 -attach nuclear.d64 -write src/resources/cell1.raw cell1
	c1541 -attach nuclear.d64 -write src/resources/cell2.raw cell2
	c1541 -attach nuclear.d64 -write src/resources/cell3.raw cell3
	c1541 -attach nuclear.d64 -write src/resources/cell4.raw cell4
	c1541 -attach nuclear.d64 -write src/resources/cursor.raw cursor
	c1541 -attach nuclear.d64 -write src/resources/human.raw human
	c1541 -attach nuclear.d64 -write src/resources/computer.raw computer
	c1541 -attach nuclear.d64 -write src/resources/explosion.raw explosion

clean:
	$(RM) nuclear
	$(RM) nuclear.d64
	$(RM) src/*.o
