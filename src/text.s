.export _rolling1
_rolling1: .asciiz "      programmed by maik merten        "

.export _rolling2
_rolling2: .asciiz "      maikmerten@googlemail.com        "

.export _rolling3
_rolling3: .asciiz " original amiga game by robert akerberg"

.export _rolling4
_rolling4: .asciiz "  control menu and game with joystick 2"

.export _titlecard
_titlecard: .incbin "resources/title.koa",2,10001

.export _sprHuman
_sprHuman: .incbin "resources/human.raw",0,63

.export _sprComputer
_sprComputer: .incbin "resources/computer.raw",0,63
