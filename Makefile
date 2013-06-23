nuclear: src/nuclear.c src/draw.h src/draw.c src/field.h src/field.c src/ai.h src/ai.c src/input.h src/input.c src/game.h src/game.c src/sound.h src/sound.c src/text.s
	cl65 -m memorymap.txt -O -o nuclear src/nuclear.c src/draw.c src/field.c src/ai.c src/input.c src/game.c src/sound.c src/text.s
	cp empty.d64 nuclear.d64
	c1541 -attach nuclear.d64 -write nuclear nuclear
	c1541 -attach nuclear.d64 -write src/resources/readme.txt readme


clean:
	$(RM) nuclear
	$(RM) nuclear.d64
	$(RM) src/*.o
