; OK.  So, I thought that FM77AV FDD is running at 600rpm.  Is it real?  Or is it 300rpm?
; Just measure it to confirm.

; 60.95 seconds for 300 times index hole detection on actual FM77AV.

; So, 300 it is.

				LDX		#300
LOOP			BSR		WAITINDEX
				LEAX	-1,X
				BNE		LOOP
				RTS

WAITINDEX
				LDA		#2
WAITINDEXIN		BITA	$FD18
				BEQ		WAITINDEXIN

WAITINDEXOUT	BITA	$FD18
				BNE		WAITINDEXOUT

				RTS
