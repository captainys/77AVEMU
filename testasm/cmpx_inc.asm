; 0 is written to $1FFF
				ORG		$2000
				FDB		$2000
				CLRA
				LDX		#$2000
				CMPX	,X++
				BEQ		EQ
				STA		$1FFF	; A=0
				RTS
EQ				COMA
				STA		$1FFF	; A=FF
				RTS

