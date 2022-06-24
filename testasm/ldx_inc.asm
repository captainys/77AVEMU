				ORG		$2000
				FDB		$FFFE
				LDX		#$2000
				LDX		,X++	; Value loaded is $FFFE, after incrementation it is 0
				BEQ		EQ
				STX		$1FFC
				LDA		#$FF
				STA		$1FFF
				RTS
EQ
				STX		$1FFC
				LDA		#$00
				STA		$1FFF
				RTS
