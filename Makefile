nuclear: src/nuclear.c src/draw.h src/draw.c src/field.h src/field.c src/ai.h src/ai.c src/input.h src/input.c src/game.h src/game.c src/sound.h src/sound.c src/petscii.h src/petscii.c src/asm.s
	cl65 -u __EXEHDR__ -m memorymap.txt -O -o nuclear.prg src/nuclear.c src/draw.c src/field.c src/ai.c src/input.c src/game.c src/sound.c src/petscii.c src/asm.s
	cp empty.d64 nuclear.d64
	c1541 -attach nuclear.d64 -write nuclear.prg nuclear.prg

clean:
	$(RM) nuclear
	$(RM) nuclear.d64
	$(RM) src/*.o
