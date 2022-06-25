; This code writes:
;   $20 $20 to $1FFE
;   $1F $FC to $1FFC
; Regardless of pre-decrementation or post-incrementation,
; the value stored is after the pre-dec or post-inc.

				ORG		$2000
				LDX		#$1FFE
				STX		,X++
				LDX		#$1FFE
				STX		,--X
				RTS
