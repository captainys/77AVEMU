; This program hsa been tested on real FM77AV
; This program is not supposed to return.
; Due to the undocumented behavior of Sub-CPU BUSY flag, EXEC &H2000 will return.
; See the bottom comment.

					ORG		$2000
					PSHS	A,B,X,Y

WAIT_READY			LDA		$FD05
					BMI		WAIT_READY
					LDA		#$80
					STA		$FD05
WAIT_HALT			LDA		$FD05
					BPL		WAIT_HALT

					LEAX	SUBSYS_CMD,PCR
					LDY		#$FC80
					LDB		#$7F
TRANSFER_LOOP		LDA		,X+
					STA		,Y+
					DECB
					BNE		TRANSFER_LOOP

					CLR		$FD05 ; This is supposed to automatically raise BUSY flag.
WAIT_UNHALT			LDA		$FD05
					BPL		WAIT_UNHALT


MAGUS_BUG_MAIN_WAIT
					CLR		$FD05
					CLR		$FD05
					LDA		$FD05
					BMI		MAGUS_BUG_MAIN_WAIT


					LDA		#$80
					STA		$FD05
WAIT_HALT2			LDA		$FD05
					BPL		WAIT_HALT2

					STA		$FC80
					CLR		$FD05

					PULS	A,B,X,Y,PC



SUBSYS_CMD
					FCB		0,0
					FCB		$3F
					FCB		"YAMAUCHI"
					FCB		$93
					FDB		$D380+(TEST_ROUTINE-SUBSYS_CMD)
					FCB		$90
TEST_ROUTINE
					CLR		$D380
MAGUS_BUG_WAIT_SUB
					CLR		$D40A
					CLR		$D40A
					LDA		$D380 ; Wait for main CPU write something to the first byte of the shared RAM.
					BEQ		MAGUS_BUG_WAIT_SUB
					RTS


; The hypothesis:
; MAGUS was a masterpiece pinball game for FM-7 series.  However, it had a bug in the program.
;
; In MAGUS, while the main CPU is waiting for the sub-CPU to become ready (clear of BUSY flag) in the following loop:
; 3082 7FFD05     CLR     $FD05 (7 clocks)
; 3085 7FFD05     CLR     $FD05 (7 clocks)
; 3088 F6FD05     LDB     $FD05 (5 clocks)
; 308B 2BF5       BMI     $3082 (3 clocks)
;
; the sub-CPU is waiting for the command from the main CPU in the following loop:
; C027 7FD40A     CLR     $D40A (7 clocks)
; C02A 7FD40A     CLR     $D40A (7 clocks)
; C02D B6D380     LDA     $D380 (5 clocks)
; C030 27F5       BEQ     $C027 (3 clocks)
;
; The problem is, CLR $D40A raises BUSY flag instead of clearing it.  Therefore if the emulator emulates as is,
; the main CPU will never gets out of the wait loop.
;
; This code, however, works on actual FM-7.  What's happening probably is CLR $FD05 from the main CPU continuously
; bombards CLR pin of a 74LS74, and CLR $D40A from the sub-CPU CLK pin.  So, 74LS74 was receiving pulse of CLK and
; CLR simultaneously.  It may put the state of 74LS74 unstable.  BUSY flag is output ~Q pin from the particular
; 74LS74.  The main CPU may be picking up when ~Q goes unstable, and took it as LOW.
;
; However, this chip is replaced with a custom chip MB113T614 in FM77AV.  Nonetheless, the above bugged code works
; on FM77AV.  How MB113T614 reacts to the bombardment to the $FD05 and $D40A is unknown.  But, MAGUS was such a
; famous masterpiece for FM-7.  Whoever was designing FM77AV might have tested it and recognized its bug, and
; maybe designed this custom chip so that MAGUS can run on FM77AV.
;
; FM-7 series emulator XM7 had a special option for running MAGUS.
; I haven't decided how to support MAGUS in Mutsu yet (when I am writing this line).

