.export _setupInterrupt
.export _disableInterrupt
.export _colorwashrow
.export _colorwashcolstart
.export _colorwashcolend
.export _play_music
.export _init_music
.export _draw_field
.export _cursor_x
.export _cursor_y
.export _cursor_ptr
.export _cursor_color
.export _sprite_field_ptrs
.export _sprite_field_colors
.export _enableTurbo
.export _disableTurbo

.zeropage

_tmp16: .res 2

.segment "CODE"

; ################ binary includes ###################

.export _customfont
_customfont: .incbin "resources/nuclearfont"

.export _music
; include SID file, skip 126 bytes (header)
_music: .incbin "resources/time_portal_7000.sid",126

.export _spr_cell0
_spr_cell0: .incbin "resources/cell0.raw"
.export _spr_cell1
_spr_cell1: .incbin "resources/cell1.raw"
.export _spr_cell2
_spr_cell2: .incbin "resources/cell2.raw"
.export _spr_cell3
_spr_cell3: .incbin "resources/cell3.raw"
.export _spr_cell4
_spr_cell4: .incbin "resources/cell4.raw"

.export _spr_human
_spr_human: .incbin "resources/human.raw"

.export _spr_computer
_spr_computer: .incbin "resources/computer.raw"

.export _spr_explosion
_spr_explosion: .incbin "resources/explosion.raw"

.export _spr_cursor
_spr_cursor: .incbin "resources/cursor.raw"


; ############ end of binary includes ##############


sid_init = $7000
sid_play = $7003


clock_init = 2

_irq_req: .res 1

_clock: .res 1


_color: .byte $f,$e,$e,$b,$6,$6,$b,$e,$e,$f,$1,$1,$1,$1,$1,$1,$1,$1,$1,$1,$f,$a,$a,$b,$2,$2,$b,$a,$a,$f,$1,$1,$1,$1,$1,$1,$1,$1,$1,$1


; A table with the starting address for every row of the color RAM
_colramrows_MSB: .byte $D8,$D8,$D8,$D8,$D8,$D8,$D8,$D9,$D9,$D9,$D9,$D9,$D9,$DA,$DA,$DA,$DA,$DA,$DA,$DA,$DB,$DB,$DB,$DB,$DB
_colramrows_LSB: .byte $00,$28,$50,$78,$A0,$C8,$F0,$18,$40,$68,$90,$B8,$E0,$08,$30,$58,$80,$A8,$D0,$F8,$20,$48,$70,$98,$C0

_colorwashrow: .byte 25 ; initialize so colorwash will be skipped
_colorwashcolstart: .byte 0 ; column start for colorwash
_colorwashcolend: .byte 39 ; column end for colorwash

; sprite data pointers for each cell of the sprite-based game field
_sprite_field_ptrs:
	.byte 201,201,201,201,201,201
	.byte 201,201,201,201,201,201
	.byte 201,201,201,201,201,201
	.byte 201,201,201,201,201,201
	.byte 201,201,201,201,201,201

; sprite colors for each cell of the sprite-based game field
_sprite_field_colors:
	.byte 10,10,14,10,10,14
	.byte 10,14,10,10,10,10
	.byte 10,10,10,14,10,10
	.byte 14,10,10,10,10,10
	.byte 10,10,10,10,14,10

; sprite cursor for the sprite-based game field
_cursor_x: .byte 127
_cursor_y: .byte 127
_cursor_ptr: .byte 205
_cursor_color: .byte 1

; switch to enable/disable sprite-based game field
_draw_field: .byte 0
; switch to enable/disable music
_play_music: .byte 1
; does music need to be initialized?
_init_music: .byte 1

; some assembler variables for setting up the sprite-based game field
row0 = 75
row_inc = 32
row1 = row0 + 1 * row_inc
row2 = row0 + 2 * row_inc
row3 = row0 + 3 * row_inc
row4 = row0 + 4 * row_inc

col0 = 88
col_inc = 32


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

; ########## special case of colorwash for the menu item selection
.proc _colorwash_menu: near

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

	ldx _colorwashrow ; row to color wash

	lda _colramrows_LSB,x;
	sta _tmp16;
	lda _colramrows_MSB,x;
	sta _tmp16+1;

	ldy _colorwashcolstart ; loop counter for screen cols
	loop_copy:

		lda _color,y
		sta (_tmp16),y
		iny
		cpy _colorwashcolend
		bne loop_copy
	loop_copy_end:
	lda _color,y
	sta (_tmp16),y ; last copy for the case _colorwashend == y

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
 
	lda #<_interrupt_music ; point IRQ Vector to our custom irq routine
	ldx #>_interrupt_music
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


; ########## helper-routines for sprite-based game field

.proc _setup_next_interrupt
	sta $314 ; store interrupt vector in $314/$315
	stx $315
	
	sty $d012 ; store raster line for next interrupt
	rts
.endproc

.proc _setup_sprite_cells
	lda #$FF
	sta $D015 ; enable all sprites

	; enable multicolor mode for sprites 2 to 7
	lda #$FC
	sta $D01C

	; set multicolor colors for sprites
	lda #11
	sta $D025
	lda #1
	sta $D026

	lda #col0
	sta $D004 ; x position sprite 2
	lda #col0 + col_inc
	sta $D006 ; x position sprite 3
	lda #col0 + 2 * col_inc
	sta $D008 ; x position sprite 4
	lda #col0 + 3 * col_inc
	sta $D00A ; x position sprite 5
	lda #col0 + 4 * col_inc
	sta $D00C ; x position sprite 6
	lda #col0 + 5 * col_inc
	sta $D00E ; x position sprite 7

	rts
.endproc

.proc _update_sprite_cells
	sta $D005 ; y position sprite 2
	sta $D007 ; y position sprite 3
	sta $D009 ; y position sprite 4
	sta $D00B ; y position sprite 5
	sta $D00D ; y position sprite 6
	sta $D00F ; y position sprite 7

	lda _sprite_field_ptrs,x
	sta $87FA ; data pointer for sprite 2 in VIC bank 1
	lda _sprite_field_colors,x
	sta $D029 ; sprite color for sprite 2

	inx
	lda _sprite_field_ptrs,x
	sta $87FB ; data pointer for sprite 3
	lda _sprite_field_colors,x
	sta $D02A ; sprite color for sprite 3


	inx
	lda _sprite_field_ptrs,x
	sta $87FC ; data pointer for sprite 4
	lda _sprite_field_colors,x
	sta $D02B ; sprite color for sprite 4

	inx
	lda _sprite_field_ptrs,x
	sta $87FD ; data pointer for sprite 5
	lda _sprite_field_colors,x
	sta $D02C ; sprite color for sprite 5

	inx
	lda _sprite_field_ptrs,x
	sta $87FE ; data pointer for sprite 6
	lda _sprite_field_colors,x
	sta $D02D ; sprite color for sprite 6

	inx
	lda _sprite_field_ptrs,x
	sta $87FF ; data pointer for sprite 7	
	lda _sprite_field_colors,x
	sta $D02E ; sprite color for sprite 7

	rts
.endproc

; ############# interrupts for playing music and color wash effect

.proc _interrupt_music
	dec $d019 ; acknowledge IRQ


	lda _init_music
	beq skip_music_init
	jsr sid_init ; #### init SID
	lda #0
	sta _init_music
	skip_music_init:

	lda _play_music
	beq skip_music
	jsr sid_play ; #### play SID
	skip_music:

	dec _clock;
	bne timer_not_expired ; only do custom interrupt if clock is zero
	timer_expired:
		ldy #clock_init
		sty _clock;

		lda _colorwashrow
		cmp #25
		bpl colorwash_skip ; apply colorwash only to rows < 25

		colorwash:
			jsr _colorwash_menu
		colorwash_skip:

	timer_not_expired:

		lda _draw_field
		beq skip_draw_field

		lda #<_interrupt_row0 ; next interrupt vector
		ldx #>_interrupt_row0
		ldy #row0 - 4 ; raster line for next interrupt
		jsr _setup_next_interrupt 

		; set up configuration for sprite based game field
		lda $D015 ; if this is zero we need to initialize sprites again
		bne skip_draw_field
		jsr _setup_sprite_cells

		skip_draw_field:
			lda _draw_field;
			beq handle_keyboard
			jmp $ea81 ; return to kernel interrupt routine (without keyboard)
			handle_keyboard:
				jmp $ea31; ; return to kernel interrupt routine (w. keyboard)


.endproc

; ########## interrupts for drawing a sprite-based game field

.proc _interrupt_row0
	dec $d019 ; acknowledge IRQ

	lda #row0
	ldx #0 ; index into array with sprite data pointers
	jsr _update_sprite_cells

	lda #<_interrupt_row1 ; next interrupt vector
	ldx #>_interrupt_row1
	ldy #row1 - 4 ; raster line for next interrupt
	jsr _setup_next_interrupt 

	jmp $ea81 ; return to kernel interrupt routine	
.endproc


.proc _interrupt_row1
	dec $d019 ; acknowledge IRQ

	lda #row1
	ldx #6 ; index into array with sprite data pointers
	jsr _update_sprite_cells

	lda #<_interrupt_row2 ; next interrupt vector
	ldx #>_interrupt_row2
	ldy #row2 - 4 ; raster line for next interrupt
	jsr _setup_next_interrupt 

	jmp $ea81 ; return to kernel interrupt routine	
.endproc


.proc _interrupt_row2
	dec $d019 ; acknowledge IRQ

	lda #row2
	ldx #12 ; index into array with sprite data pointers
	jsr _update_sprite_cells

	lda #<_interrupt_row3 ; next interrupt vector
	ldx #>_interrupt_row3
	ldy #row3 - 4 ; raster line for next interrupt
	jsr _setup_next_interrupt 

	jmp $ea81 ; return to kernel interrupt routine	
.endproc


.proc _interrupt_row3
	dec $d019 ; acknowledge IRQ

	lda #row3
	ldx #18 ; index into array with sprite data pointers
	jsr _update_sprite_cells

	lda #<_interrupt_row4 ; next interrupt vector
	ldx #>_interrupt_row4
	ldy #row4 - 4 ; raster line for next interrupt
	jsr _setup_next_interrupt 

	jmp $ea81 ; return to kernel interrupt routine	
.endproc


.proc _interrupt_row4
	dec $d019 ; acknowledge IRQ

	lda #row4
	ldx #24 ; index into array with sprite data pointers
	jsr _update_sprite_cells

	lda #<_interrupt_music ; next interrupt vector
	ldx #>_interrupt_music
	ldy #0 ; raster line for next interrupt
	jsr _setup_next_interrupt 


	; set cursor position and color
	lda _cursor_x
	sta $D002 ; x position sprite 1
	lda _cursor_y
	sta $D003 ; y position sprite 1
	lda _cursor_ptr
	sta $87F9 ; data pointer for sprite 1
	lda _cursor_color
	sta $D028 ; sprite color for sprite 1

	jmp $ea31 ; return to kernel interrupt routine (with keyboard)

.endproc


.proc _enableTurbo : near
	pha

	lda $D0FE		; load value of $D0FE
	pha				; ... and save
	lda #$2A
	sta $D0FE
	lda $D0FE
	cmp #$FF		; Chameleon will NOT be $FF
	beq end

	lda $D0F3		; load turbo configuration register
	ora #$80		; set turbo enabled bit
	sta $D0F3

end:
	pla				; retrieve backup of $D0FE
	sta $D0FE		; restore

	pla
	rts
.endproc


.proc _disableTurbo : near

	pha

	lda $D0FE		; load value of $D0FE
	pha				; ... and save
	lda #$2A
	sta $D0FE
	lda $D0FE
	cmp #$FF		; Chameleon will NOT be $FF
	beq end

	lda $D0F3		; load turbo configuration register
	and #$7F		; clear turbo enabled bit
	sta $D0F3

end:
	pla				; retrieve backup of $D0FE
	sta $D0FE		; restore

	pla
	rts
.endproc