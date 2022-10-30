; The purpose of this test program is to see change of STATUS on command $18 (Seek + Head Load)
; The result from actual FM7AV:
;   Status byte changed $C4->$C5->$C1->$C0
; U register was at $30FF when seek command has completed.
; $C4->$C5 when FDC starts working (b0=BUSY flag)
; $C5->$C1 when track left track 0 (b2=TRACK00 flag)
; $C1->$C0 when FDC is done with seek (b0=BUSY flag)

; Conclusion:
;   Even if h-flag (head-load) is specified in the FDC command,
;   HEAD ENGAGED flag (b6) will not be set.

					ORG		$1600

					PSHS	A,B,X,Y,U,CC

					BSR		WAIT_READY
					LDA		#$00	; Restore
					STA		$FD18
					BSR		WAIT_IRQ

					LEAU	$1800,PCR

					BSR		WAIT_READY
					LDA		#10
					STA		$FD1B
					LDA		#$18	; Seek
					STA		$FD18

WAIT_BUSY			LDA		$FD18
					STA		,U+
					LSRA
					BCC		WAIT_BUSY

WAIT_SEEK			LDA		$FD18
					STA		,U+
					LSRA
					BCS		WAIT_SEEK

					STU		$17FE,PCR

FOLLOWUP			LDA		$FD18
					STA		,U+
					CMPU	#$5000
					BCS		FOLLOWUP

					BSR		WAIT_READY
					LDA		#$00	; Restore
					STA		$FD18
					BSR		WAIT_IRQ

					PULS	A,B,X,Y,U,CC,PC

WAIT_READY			LDA		$FD18
					ANDA	#1
					BNE		WAIT_READY
					RTS

WAIT_IRQ			LDA		$FD1F
					LSLA
					BPL		WAIT_IRQ
					RTS
