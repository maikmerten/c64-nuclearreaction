.export _setupInterrupt
.export _disableInterrupt

.zeropage

_tmp16: .res 2

.segment "CODE"


clock_init = 3

_irq_req: .res 1

_clock: .res 1

_color: .byte $09,$09,$02,$02,$08,$08,$0a,$0a,$0f,$0f,$07,$07,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$07,$07,$0f,$0f,$0a,$0a,$08,$08,$02,$02,$09,$09 

; A table with the starting address for every row of the color RAM
_colramrows_MSB: .byte $D8,$D8,$D8,$D8,$D8,$D8,$D8,$D9,$D9,$D9,$D9,$D9,$D9,$DA,$DA,$DA,$DA,$DA,$DA,$DA,$DB,$DB,$DB,$DB,$DB
_colramrows_LSB: .byte $00,$28,$50,$78,$A0,$C8,$F0,$18,$40,$68,$90,$B8,$E0,$08,$30,$58,$80,$A8,$D0,$F8,$20,$48,$70,$98,$C0


.proc _colorwash: near

	; ###### shift colors

	ldy _color+0	; save first color before overwriting

	ldx #0		; loop counter
	loop_colorshift:
		lda _color+1,x	; load color next cell
		sta _color,x	; copy to current position
		inx		; increase loop counter
		cpx #40
		bne loop_colorshift
	loop_colorshift_end:

	sty _color+39	; copy first color to last position

	; ###### copy to color RAM

	ldx #0 ; loop counter for screen rows
	loop_outer:

		lda _colramrows_LSB,x;
		sta _tmp16;
		lda _colramrows_MSB,x;
		sta _tmp16+1;

		ldy #0 ; loop counter for screen cols
		loop_inner:

			lda _color,y
			sta (_tmp16),y
			iny
			cpy #40
			bne loop_inner
		loop_inner_end:

		inx
		cpx #25
		bne loop_outer

	loop_outer_end:

	rts
.endproc




.proc _setupInterrupt: near
	; this is based on http://dustlayer.com/c64-coding-tutorials/2013/4/8/episode-2-3-did-i-interrupt-you
	sei ; set interrupt disable flag

	; reset clock
	ldy #clock_init;
	sty _clock;


	; Save interrupt request mask
	ldy $d01a
	sty _irq_req

	; Turn off CIA Timer interrupts
	ldy #$7f ; $7f = %01111111
	sty $dc0d ; Turn off CIAs Timer interrupts
	sty $dd0d ; Turn off CIAs Timer interrupts
	lda $dc0d ; cancel all CIA-IRQs in queue/unprocessed
	lda $dd0d ; cancel all CIA-IRQs in queue/unprocessed
	lda #$01 ; Set Interrupt Request Mask...
	sta $d01a ; ...we want IRQ by Rasterbeam
 
	lda #<_interrupt ; point IRQ Vector to our custom irq routine
	ldx #>_interrupt
	sta $314 ; store in $314/$315
	stx $315
 
	lda #$0 ; trigger first interrupt at row zero
	sta $d012
 
	lda $d011 ; Bit#0 of $d011 is basically...
	and #$7f ; ...the 9th Bit for $d012
	sta $d011 ; we need to make sure it is set to zero

 
	cli ; clear interrupt disable flag

	rts	
.endproc	


.proc _interrupt
	dec $d019 ; acknowledge IRQ

	dec _clock;
	bne skip
	ldy #clock_init
	sty _clock;

	jsr _colorwash

skip:
	jmp $ea31 ; return to kernel interrupt routine
.endproc


.proc _disableInterrupt: near
	sei ; set interrupt disable flag

	; restore IRQ request mask
	ldy _irq_req
	sty $d01a


	; restore IRQ vector to kernel interrupt routine
	ldx #$31
	ldy #$ea
	stx $314
	sty $315

	; restore CIA state	
	lda #$ff
	sta $dc0d
	lda #$ff
	sta $dd0d

	lda $dc0d ; cancel all CIA-IRQs in queue/unprocessed
	lda $dd0d ; cancel all CIA-IRQs in queue/unprocessed


	cli ; clear interrupt disable flag

	rts	
.endproc



