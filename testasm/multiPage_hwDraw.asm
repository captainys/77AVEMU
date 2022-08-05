; F-BASIC V3.4 L20 draws a filled-box using sub-system command $15, which breaks the menu frame
; if drawn as is.
; Only explanation why it doesn't break is hardware-drawing and hardware line-drawing is affected
; by multi-page ($FD37).
; The explanation of the I/O tells low 3 bits controls CPU to VRAM access.  But, does it also
; control hardware drawing?
; This program is to find answer to this question.

; This program writes #6 to $FD37 to mask CPU-to-VRAM access of Red and Green planes, and then
; fill the screen with hardware line drawing.
; If hardware line-drawing repects $FD37, the output should be blue.  If it doesn't, the output
; should be white.

; The verdict from the real FM77AV was BLUE.  In conclusion, FM77AV's hardware line drawing respects $FD37.

							ORG		$2000

							PSHS	A,B,X,Y,U

SUB_HALT					LDA		$FD05
							BMI		SUB_HALT
							LDA		#$80
							STA		$FD05
							STA		$FC80
							CLR		$FC82


							LDA		#$30
							LDX		#$FD80
MMR_INIT_LOOP				STA		,X+
							INCA
							CMPA	#$40
							BNE		MMR_INIT_LOOP

							LDA		#$1D
							STA		$FD8D

							LDA		#$80
							STA		$FD93

							LDA		#6		; Mask Red and Green planes.  If Hardware Drawing respects this register, I should see blue.
							STA		$FD37

							LDX		#$FFFF
							STX		$D422

							LDY		#0
							STY		$D420
LINE_LOOP
							LDB		$D430
							ANDB	#$10
							BEQ		LINE_LOOP		; Wait ready

							LDX		#$8007
							STX		$D410

							LDB		#8
							STB		$D41B

							LDX		#0
							STX		$D424
							STY		$D426
							LDX		#639
							STX		$D428
							STY		$D42A

							LEAY	1,Y
							CMPY	#200
							BNE		LINE_LOOP


WAIT_LINE_END				LDB		$D430
							ANDB	#$10
							BEQ		WAIT_LINE_END

							CLR		$D410

							CLR		$FD05
							CLR		$FD37
							CLR		$FD93
							PULS	A,B,X,Y,U,PC

