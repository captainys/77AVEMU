; The following code wrote 100 to $1ffC and $1FFE.
; Even with post-incrementation, the value is what read from the memory.
					ORG		$2000
					FDB		100
					LDU		#$2000
					LDU		,U++
					STU		$1FFC
					LDU		#$2002
					LDU		,--U
					STU		$1FFE
					RTS
