// The following program wrote FF to $1FFF on actual FM77AV.
// It means BEQ EQ branched.
// Therefore, CMPX compared the value of X after the pre-incrementation.

				ORG		$2000
				FDB		#$2000
				LDX		#$2002
				CMPX	,--X
				BEQ		EQ
				LDA		#0
				STA		$1FFF
				STX		$1FFC
				RTS

EQ				LDA		#$FF
				STA		$1FFF
				STX		$1FFC
				RTS
