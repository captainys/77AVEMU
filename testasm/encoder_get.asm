				ORG		$4000

				BSR		HALT

				LEAX	SUBSYSCMD,PCR
				LDU		#$FC80
SENDLOOP		LDA		,X+
				STA		,U+
				CMPU	#$FCA0
				BNE		SENDLOOP

				CLR		$FD05
				BSR		HALT

				LDX		#$5000
				LDU		#$FC80
RECVLOOP		LDA		,U+
				STA		,X+
				CMPU	#$FCA0
				BNE		RECVLOOP

				LDA		#$80
				STA		$FC80
				CLR		$FD05
				RTS

HALT			LDA		$FD05
				BMI		HALT
				LDA		#$80
				STA		$FD05
WAITHALT		LDA		$FD05
				BPL		WAITHALT
				RTS


SUBSYSCMD		FCB		0,0
				FCB		$45
				FCB		1  ; Get encoding
				FCB		$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
