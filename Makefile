nuclear: src/nuclear.c src/draw.h src/draw.c src/field.h src/field.c src/ai.h src/ai.c src/input.h src/input.c src/game.h src/game.c src/sound.h src/sound.c src/petscii.h src/petscii.c src/asm.s
	cl65 -m memorymap.txt -O -o nuclear.prg src/nuclear.c src/draw.c src/field.c src/ai.c src/input.c src/game.c src/sound.c src/petscii.c src/asm.s
	cp empty.d64 nuclear.d64
	cat src/resources/nuclearfont \
		src/resources/cell0.raw \
		src/resources/cell1.raw \
		src/resources/cell2.raw \
		src/resources/cell3.raw \
		src/resources/cell4.raw \
		src/resources/cursor.raw \
		src/resources/human.raw \
		src/resources/computer.raw \
		src/resources/explosion.raw \
		src/resources/time_portal_7000.sid > assets.bin
	c1541 -attach nuclear.d64 -write nuclear.prg nuclear.prg
	c1541 -attach nuclear.d64 -write src/resources/readme.txt readme
	c1541 -attach nuclear.d64 -write src/resources/title.koa title
	c1541 -attach nuclear.d64 -write assets.bin nuclearass

clean:
	$(RM) nuclear
	$(RM) nuclear.d64
	$(RM) src/*.o
