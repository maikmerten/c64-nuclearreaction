nuclear: src/nuclear.c src/draw.c src/field.c src/ai.c src/text.s
	cl65 -m memorymap.txt -O -o nuclear src/nuclear.c src/draw.c src/field.c src/ai.c src/text.s
	cp empty.d64 nuclear.d64
	c1541 -attach nuclear.d64 -write nuclear nuclear


clean:
	$(RM) nuclear
	$(RM) nuclear.d64
	$(RM) src/*.o
