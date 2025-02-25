; main
MAIN:	add r3, r4

; loop
LOOP:	prn #48
	lea STR, r6
	inc r6

; macro
	macr stop
		add r3, r1
		sub r5, r6
	endmacr

	sub r1, r4
	cmp r3, #-6
	bne END
	add r7, *r6
	sub r4, r1
.entry MAIN
	jmp LOOP

; end
END:	stop

; str
STR:	.string "abcdef"

; list
LIST:	.data 6, -9
	.data -100
	.data 12
	.data 13
	.data 14