#include <iostream>
#include "cpputil.h"
#include "miscutil.h"
#include "fm77avcommand.h"
#include "fm77avlineparser.h"
#include "outside_world.h"
#include "yspngenc.h"


FM77AVCommandInterpreter::FM77AVCommandInterpreter()
{
	primaryCmdMap["HELP"]=CMD_HELP;
	primaryCmdMap["HLP"]=CMD_HELP;
	primaryCmdMap["H"]=CMD_HELP;
	primaryCmdMap["QUIT"]=CMD_QUIT;
	primaryCmdMap["Q"]=CMD_QUIT;
	primaryCmdMap["FORCEQUIT"]=CMD_FORCE_QUIT;
	primaryCmdMap["RESET"]=CMD_RESET;
	primaryCmdMap["NOWAIT"]=CMD_NOWAIT;
	primaryCmdMap["YESWAIT"]=CMD_YESWAIT;
	primaryCmdMap["RUN"]=CMD_RUN;
	primaryCmdMap["PAUSE"]=CMD_PAUSE;
	primaryCmdMap["PAU"]=CMD_PAUSE;
	primaryCmdMap["T"]=CMD_RUN_ONE_INSTRUCTION;
	primaryCmdMap["MUTE"]=CMD_MUTE;
	primaryCmdMap["UNMUTE"]=CMD_UNMUTE;
	primaryCmdMap["ENABLE"]=CMD_ENABLE;
	primaryCmdMap["ENA"]=CMD_ENABLE;
	primaryCmdMap["DISABLE"]=CMD_DISABLE;
	primaryCmdMap["DIS"]=CMD_DISABLE;
	primaryCmdMap["STARTAUDIOREC"]=CMD_START_AUDIO_RECORDING;
	primaryCmdMap["ENDAUDIOREC"]=CMD_END_AUDIO_RECORDING;
	primaryCmdMap["STOPAUDIOREC"]=CMD_END_AUDIO_RECORDING;
	primaryCmdMap["SAVEAUDIOREC"]=CMD_SAVE_AUDIO_RECORDING;
	primaryCmdMap["FMCH"]=CMD_FMCH;
	primaryCmdMap["PSGCH"]=CMD_PSGCH;
	primaryCmdMap["STA"]=CMD_PRINT_STATUS;
	primaryCmdMap["HIST"]=CMD_PRINT_HISTORY;
	primaryCmdMap["U"]=CMD_DISASM;
	primaryCmdMap["UM"]=CMD_DISASM_MAIN;
	primaryCmdMap["US"]=CMD_DISASM_SUB;
	primaryCmdMap["PRINT"]=CMD_DUMP;
	primaryCmdMap["PRI"]=CMD_DUMP;
	primaryCmdMap["P"]=CMD_DUMP;
	primaryCmdMap["DUMP"]=CMD_DUMP;
	primaryCmdMap["DM"]=CMD_DUMP;
	primaryCmdMap["MEMDUMP"]=CMD_MEMDUMP;
	primaryCmdMap["MD"]=CMD_MEMDUMP;
	primaryCmdMap["BP"]=CMD_ADD_BREAKPOINT;
	primaryCmdMap["BPPC"]=CMD_ADD_BREAKPOINT_WITH_PASSCOUNT;
	primaryCmdMap["MP"]=CMD_ADD_MONITORPOINT;
	primaryCmdMap["BC"]=CMD_DELETE_BREAKPOINT;
	primaryCmdMap["BL"]=CMD_LIST_BREAKPOINTS;
	primaryCmdMap["BRKON"]=CMD_BREAK_ON;
	primaryCmdMap["CBRKON"]=CMD_DONT_BREAK_ON;
	primaryCmdMap["MON"]=CMD_MONITOR;
	primaryCmdMap["CMON"]=CMD_DONT_MONITOR;
	primaryCmdMap["SAVEHIST"]=CMD_SAVE_HISTORY;
	primaryCmdMap["KEYBOARD"]=CMD_KEYBOARD;
	primaryCmdMap["TYPE"]=CMD_TYPE_KEYBOARD;
	primaryCmdMap["TAPELOAD"]=CMD_TAPE_LOAD;
	primaryCmdMap["TAPEFORSAVE"]=CMD_TAPE_SAVETAPE,
	primaryCmdMap["TAPEEJECT"]=CMD_TAPE_EJECT;
	primaryCmdMap["TAPEWP"]=CMD_TAPE_WRITE_PROTECT;
	primaryCmdMap["TAPEUP"]=CMD_TAPE_WRITE_UNPROTECT;
	primaryCmdMap["TAPEREWIND"]=CMD_TAPE_REWIND;
	primaryCmdMap["TAPEREC"]=CMD_TAPE_REC_BUTTON_ON;
	primaryCmdMap["TAPEPLAY"]=CMD_TAPE_REC_BUTTON_RELEASE;
	primaryCmdMap["TAPESAVEAS"]=CMD_TAPE_SAVE_AS;
	primaryCmdMap["SAVETAPEAS"]=CMD_TAPE_SAVE_AS;
	primaryCmdMap["FD0LOAD"]=CMD_FD0LOAD;
	primaryCmdMap["FD0EJECT"]=CMD_FD0EJECT;
	primaryCmdMap["FD0WP"]=CMD_FD0WRITEPROTECT;
	primaryCmdMap["FD0UP"]=CMD_FD0WRITEUNPROTECT;
	primaryCmdMap["FD1LOAD"]=CMD_FD1LOAD;
	primaryCmdMap["FD1EJECT"]=CMD_FD1EJECT;
	primaryCmdMap["FD1WP"]=CMD_FD1WRITEPROTECT;
	primaryCmdMap["FD1UP"]=CMD_FD1WRITEUNPROTECT;
	primaryCmdMap["LOADEVT"]=CMD_LOAD_EVENTLOG;
	primaryCmdMap["PLAYEVT"]=CMD_PLAY_EVENTLOG;
	primaryCmdMap["STOPEVT"]=CMD_STOP_EVENTLOG;
	primaryCmdMap["SS"]=CMD_SAVE_SCREENSHOT;
	primaryCmdMap["QSS"]=CMD_QUICK_SCREENSHOT;
	primaryCmdMap["QSSDIR"]=CMD_QUICK_SCREENSHOT_DIR;
	primaryCmdMap["SAVECOM0"]=CMD_SAVE_COM0OUT;
	primaryCmdMap["CLEARCOM0"]=CMD_CLEAR_COM0OUT;
	primaryCmdMap["LET"]=CMD_LET;
	primaryCmdMap["SPEED"]=CMD_SETSPEED;
	primaryCmdMap["GAMEPORT"]=CMD_GAMEPORT;
	primaryCmdMap["SAVESTATE"]=CMD_SAVE_STATE;
	primaryCmdMap["LOADSTATE"]=CMD_LOAD_STATE;
	primaryCmdMap["AUTOSTOPKEY"]=CMD_AUTOSTOPKEY;
	primaryCmdMap["ADDLAB"]=CMD_ADD_LABEL;
	primaryCmdMap["ADDLABEL"]=CMD_ADD_LABEL;
	primaryCmdMap["ADDDLB"]=CMD_ADD_DATALABEL;
	primaryCmdMap["ADDDATALABEL"]=CMD_ADD_DATALABEL;
	primaryCmdMap["ADDREM"]=CMD_ADD_COMMENT;
	primaryCmdMap["ADDCMT"]=CMD_ADD_COMMENT;
	primaryCmdMap["DEFRAW"]=CMD_DEF_RAW_BYTES;
	primaryCmdMap["DELSYM"]=CMD_DEL_SYMBOL;
	primaryCmdMap["IMPORTLSTSYM"]=CMD_IMPORT_LST_SYMTABLE;
	primaryCmdMap["IMMISIO"]=CMD_IMM_IS_IOPORT;
	primaryCmdMap["IMMISSYM"]=CMD_IMM_IS_LABEL;
	primaryCmdMap["IMMISLAB"]=CMD_IMM_IS_LABEL;
	primaryCmdMap["OFFSETISLAB"]=CMD_OFFSET_IS_LABEL;
	primaryCmdMap["OFSISLAB"]=CMD_OFFSET_IS_LABEL;
	primaryCmdMap["OFFISLAB"]=CMD_OFFSET_IS_LABEL;
	primaryCmdMap["SYM"]=CMD_PRINT_SYMBOL;
	primaryCmdMap["SYMLAB"]=CMD_PRINT_SYMBOL_LABEL_PROC;
	primaryCmdMap["SYMPROC"]=CMD_PRINT_SYMBOL_PROC;
	primaryCmdMap["SYMFIND"]=CMD_PRINT_SYMBOL_FIND;


	featureMap["IOMON"]=ENABLE_IOMONITOR;
	featureMap["FDCMON"]=ENABLE_FDCMONITOR;
	featureMap["SUBCMDMON"]=ENABLE_SUBSYSCMD_MONITOR;
	featureMap["BIOSMON"]=ENABLE_BIOSCMD_MONITOR;
	featureMap["DEBUG"]=ENABLE_DEBUGGER;
	featureMap["DEBUGGER"]=ENABLE_DEBUGGER;
	featureMap["CALLSTACK"]=ENABLE_CALLSTACK;
	featureMap["CST"]=ENABLE_CALLSTACK;
	featureMap["COM0TX"]=ENABLE_PRINT_COM0;
	featureMap["PSGLOG"]=ENABLE_PSG_LOG;
	featureMap["AUTOSTOP"]=ENABLE_AUTOSTOP;
	featureMap["RKANA"]=ENABLE_RKANA;
	featureMap["AUDIOLEVEL"]=ENABLE_AUDIOLEVELMETER;
	featureMap["LOGDISASM"]=ENABLE_LOG_DISASM;
	featureMap["LOGALLREGS"]=ENABLE_LOG_ALLREGISTERS;


	breakEventMap["SUBUNHALT"]=BREAK_ON_SUBCPU_UNHALT;
	breakEventMap["UNHALTSUB"]=BREAK_ON_SUBCPU_UNHALT;
	breakEventMap["SUBCMD"]=BREAK_ON_SUBCMD;
	breakEventMap["MEMREAD"]=BREAK_ON_MEM_READ;
	breakEventMap["MEMR"]=BREAK_ON_MEM_READ;
	breakEventMap["MEMWRITE"]=BREAK_ON_MEM_WRITE;
	breakEventMap["MEMW"]=BREAK_ON_MEM_WRITE;
	breakEventMap["HWDRAW"]=BREAK_ON_HARDWARE_VRAM_WRITE;
	breakEventMap["HWLINE"]=BREAK_ON_HARDWARE_LINE_DRAWING;

	dumpableMap["FDC"]=DUMP_FDC;
	dumpableMap["TAPE"]=DUMP_TAPE;
	dumpableMap["HIST"]=DUMP_PC_LOG;
	dumpableMap["SOUND"]=DUMP_SOUND;
	dumpableMap["MEM"]=DUMP_MEMORY;
	dumpableMap["CRTC"]=DUMP_CRTC;
	dumpableMap["CALLSTACK"]=DUMP_CALLSTACK;
	dumpableMap["CST"]=DUMP_CALLSTACK;
	dumpableMap["IRQ"]=DUMP_IRQ;
	dumpableMap["PSGLOG"]=DUMP_PSG_LOG;
	dumpableMap["CAS0"]=DUMP_CAS0;
	dumpableMap["CAS1"]=DUMP_CAS1;
}

void FM77AVCommandInterpreter::PrintHelp(void) const
{
	std::cout << "<< Primary Command >>" << std::endl;
	std::cout << "HELP|HLP|H|?" << std::endl;
	std::cout << "  Print help." << std::endl;
	std::cout << "QUIT|Q" << std::endl;
	std::cout << "  Quit." << std::endl;
	std::cout << "NOWAIT" << std::endl;
	std::cout << "  Run at full-speed without adjusting for real-time." << std::endl;
	std::cout << "SPEED FM8/FM7/FM77" << std::endl;
	std::cout << "  Select speed." << std::endl;
	std::cout << "  FM8 speed is for FM-7 in slow mode (Dip Switch 4)" << std::endl;
	std::cout << "  FM7 speed is native FM-7 speed or slow mode of FM-77." << std::endl;
	std::cout << "  FM77 speed is native FM-77 speed." << std::endl;
	std::cout << "YESWAIT" << std::endl;
	std::cout << "  Match speed to the wall-clock time." << std::endl;
	std::cout << "  Albert Einstein said your wall-clock and my wall-clock runs differently," << std::endl;
	std::cout << "  But, hopefully the error is small enough." << std::endl;
	std::cout << "KEYBOARD keyboardMode" <<std::endl;
	std::cout << "  Select TRANSLATE or DIRECT mode." << std::endl;
	std::cout << "  TRANSLATE or TRANS mode (TRANSLATE1 or TRANS1) will be good for typing commands, but" << std::endl;
	std::cout << "  cannot sense key release correctly." << std::endl;
	std::cout << "  TRANS1 will make virtual BREAK+ESC from physical ESC." << std::endl;
	std::cout << "  TRANS2 will make virtual ESC from physical ESC." << std::endl;
	std::cout << "  TRANS3 will make virtual BREAK from physical ESC." << std::endl;
	std::cout << "  DIRECT mode is good for games, but affected by the keyboard layout." << std::endl;
	std::cout << "  US keyboard cannot type some of the characters." << std::endl;
	std::cout << "TYPE string" << std::endl;
	std::cout << "  Virtually type string." << std::endl;

	std::cout << "FD0LOAD filename" << std::endl;
	std::cout << "FD1LOAD filename" << std::endl;
	std::cout << "  Load FD image.  The number 0 or 1 is the drive number." << std::endl;
	std::cout << "FD0EJECT" << std::endl;
	std::cout << "FD1EJECT" << std::endl;
	std::cout << "  Eject FD." << std::endl;
	std::cout << "FD0WP" << std::endl;
	std::cout << "FD1WP" << std::endl;
	std::cout << "  Write protect floppy disk." << std::endl;
	std::cout << "FD0UP" << std::endl;
	std::cout << "FD1UP" << std::endl;
	std::cout << "  Write un-protect floppy disk." << std::endl;

	std::cout << "TAPEWP" << std::endl;
	std::cout << "  Write protect tape." << std::endl;
	std::cout << "TAPEUP" << std::endl;
	std::cout << "  Write unprotect tape." << std::endl;
	std::cout << "TAPEREWIND" << std::endl;
	std::cout << "  Rewind tape." << std::endl;
	std::cout << "TAPEREC" << std::endl;
	std::cout << "  Press REC button." << std::endl;
	std::cout << "TAPEPLAY" << std::endl;
	std::cout << "  Release REC button." << std::endl;
	std::cout << "SAVETAPEAS filename.t77" << std::endl;
	std::cout << "TAPESAVEAS filename.t77" << std::endl;
	std::cout << "  Save tape image as filename." << std::endl;

	std::cout << "SAVESTATE fileName" << std::endl;
	std::cout << "  Save machine state (experimental)" << std::endl;
	std::cout << "LOADSTATE fileName" << std::endl;
	std::cout << "  Load machine state (experimental)" << std::endl;

	std::cout << "GAMEPORT 0/1 device" << std::endl;
	std::cout << "  Connect device to game port." << std::endl;

	std::cout << "RUN" << std::endl;
	std::cout << "  Run." << std::endl;
	std::cout << "RUN M:addr/S:addr" << std::endl;
	std::cout << "  Run to specified address." << std::endl;
	std::cout << "RUN M:addr/S:addr addr" << std::endl;
	std::cout << "  Run to specified address range (RUN S:0000 DFFF)." << std::endl;
	std::cout << "RUN addr" << std::endl;
	std::cout << "  If one of the CPUs is muted (by MUTE command), run to the address" <<std::endl;
	std::cout << "  in the unmuted CPU." << std::endl;
	std::cout << "T" << std::endl;
	std::cout << "  Trace.  Run one instruction." << std::endl;
	std::cout << "PAUSE|PAU" << std::endl;
	std::cout << "  Pause VM." << std::endl;
	std::cout << "MUTE M|S" << std::endl;
	std::cout << "UNMUTE M|S" << std::endl;
	std::cout << "  Mute/Unmute state output of main/sub CPU." << std::endl;
	std::cout << "  If no parameter is given, mute/unmutes both CPUs." << std::endl;
	std::cout << "ENA feature|ENABLE feature" << std::endl;
	std::cout << "  Enable a feature." << std::endl;
	std::cout << "DIS feature|DISABLE feature" << std::endl;
	std::cout << "  Disable a feature." << std::endl;
	std::cout << "STA / STA m(ain) / STA s(ub)" << std::endl;
	std::cout << "  Print CPU status." << std::endl;
	std::cout << "HIST / HIST m(ain) / HIST s(ub)" << std::endl;
	std::cout << "  Same as PRI HIST command." << std::endl;
	std::cout << "U cpu:addr" << std::endl;
	std::cout << "UM addr" << std::endl;
	std::cout << "US addr" << std::endl;
	std::cout << "  Disassemble.  If no address is given, it continues to disassemble" << std::endl;
	std::cout << "  from the last disassembly address." << std::endl;
	std::cout << "  U command takes MAIN or SUB as cpu." << std::endl;
	std::cout << "  UM disassembles main CPU memory, US sub-CPU memory." << std::endl;
	std::cout << "PRINT info|PRI info|P info" << std::endl;
	std::cout << "DUMP info|DM info" << std::endl;
	std::cout << "  Print/Dump information." << std::endl;
	std::cout << "MEMDUMP or MD seg:address" << std::endl;
	std::cout << "MEMDUMP or MD seg:address wid hei step" << std::endl;
	std::cout << "MEMDUMP or MD seg:address wid hei step 1/0" << std::endl;
	std::cout << "  Memory Dump.  If you enter wid,hei,step it will dump non-16x16 columns." << std::endl;
	std::cout << "  If you enter 1/0, you can control to show or hide ASCII dump." << std::endl;
	std::cout << "BP EIP|BRK EIP" << std::endl;
	std::cout << "BP CS:EIP|BRK CS:EIP" << std::endl;
	std::cout << "  Add a break point." << std::endl;
	std::cout << "BPPC EIP|BRK EIP passCount" << std::endl;
	std::cout << "BPPC CS:EIP passCount" << std::endl;
	std::cout << "  Add a break point with pass count" << std::endl;
	std::cout << "  Majority of the numbers specified in the parameters are hexadecimal, but" << std::endl;
	std::cout << "  The number here (passCount) is a decimal number." << std::endl;
	std::cout << "MP EIP|BRK EIP" << std::endl;
	std::cout << "MP CS:EIP|BRK CS:EIP" << std::endl;
	std::cout << "  Add a monitor-only break point.  At a monitor-only break point, the VM will print" << std::endl;
	std::cout << "  the status, but will not stop the execution." << std::endl;
	std::cout << "BC Num" << std::endl;
	std::cout << "  Delete a break point." << std::endl;
	std::cout << "  Num is the number printed by PRINT BRK." << std::endl;
	std::cout << "  BC * to erase all break points." << std::endl;
	std::cout << "BL main|sub" << std::endl;
	std::cout << "  List break points." << std::endl;
	std::cout << "  Break on event." << std::endl;
	std::cout << "BRKON event" << std::endl;
	std::cout << "  Break on event." << std::endl;
	std::cout << "CBRKON event" << std::endl;
	std::cout << "  Don't break on event." << std::endl;
	std::cout << "MON event" << std::endl;
	std::cout << "  Monitor event." << std::endl;
	std::cout << "CMON event" << std::endl;
	std::cout << "  Don't monitor event. (It works the same way as CBRKON)" << std::endl;
	std::cout << "SAVEHIST filename.txt" << std::endl;
	std::cout << "  Save CS:EIP Log to file." << std::endl;

	std::cout << "STARTAUDIOREC" << std::endl;
	std::cout << "STOPAUDIOREC / ENDAUDIOREC" << std::endl;
	std::cout << "SAVEAUDIOREC filename.wav" << std::endl;
	std::cout << "  Start/stop/save audio recording." << std::endl;
	std::cout << "FMCH 0/1 0/1 0/1" << std::endl;
	std::cout << "PSGCH 0/1 0/1 0/1" << std::endl;
	std::cout << "  Turn on or off FM/PSG channels." << std::endl;

	std::cout << "SAVECOM0 filename" << std::endl;
	std::cout << "  Save COM0 log to file." << std::endl;
	std::cout << "CLEARCOM0" << std::endl;
	std::cout << "  Clear COM0 log." << std::endl;

	std::cout << "LET main/sub:register value" << std::endl;
	std::cout << "  Load a register value." << std::endl;

	std::cout << "LOADEVT filename.txt" << std::endl;
	std::cout << "  Load Event Log." << std::endl;
	std::cout << "PLAYEVT" << std::endl;
	std::cout << "  Playback Event Log." << std::endl;
	std::cout << "STOPEVT" << std::endl;
	std::cout << "  Stop Playback Event Log." << std::endl;

	std::cout << "SS filename.png" << std::endl;
	std::cout << "  Save screenshot." << std::endl;
	std::cout << "QSS" << std::endl;
	std::cout << "  Quick Screen Shot.  File name is automatically given." << std::endl;
	std::cout << "  File is saved in the quick-screenshot directory." << std::endl;
	std::cout << "  Optionally, can specify page 0 or 1 as a parameter." << std::endl;
	std::cout << "QSSDIR dir" << std::endl;
	std::cout << "  Specify quick-screenshot directory." << std::endl;

	std::cout << "AUTOSTOPKEY keyCode" << std::endl;
	std::cout << "  Specify which key is pressed for stopping in a FM-8/FM-7 games." << std::endl;

	std::cout << "ADDSYM addr label" << std::endl;
	std::cout << "  Add a symbol.  An address can have one symbol,label,data label, or data, and one comment." << std::endl;
	std::cout << "  If a symbol is added to an address that already has a symbol, label, or data label," << std::endl;
	std::cout << "  the address's label, or data label will be overwritten as a symbol." << std::endl;
	std::cout << "ADDLAB|ADDLABEL addr label" << std::endl;
	std::cout << "  Add a label.  An address can have one symbol,label, or data label, or data and one comment." << std::endl;
	std::cout << "  If a symbol is added to an address that already has a symbol, label, or data label," << std::endl;
	std::cout << "  the address's label, or data label will be overwritten as a label." << std::endl;
	std::cout << "ADDDLB|ADDDATALABEL addr label" << std::endl;
	std::cout << "  Add a data label.  An address can have one symbol,label, or data label, or data and one comment." << std::endl;
	std::cout << "  If a symbol is added to an address that already has a symbol, label, or data label," << std::endl;
	std::cout << "  the address's label, or data label will be overwritten as a data label." << std::endl;
	std::cout << "ADDREM|ADDCMT addr label" << std::endl;
	std::cout << "  Add a comment.  An address can have one symbol,label, or data label, and one comment." << std::endl;
	std::cout << "DEFRAW addr label numBytes" << std::endl;
	std::cout << "  Define raw data bytes.  Disassembler will take this address as raw data." << std::endl;
	std::cout << "IMMISIO addr" << std::endl;
	std::cout << "  Take Imm operand of the address as IO-port address in disassembly." << std::endl;
	std::cout << "IMMISSYM addr" << std::endl;
	std::cout << "IMMISLAB addr" << std::endl;
	std::cout << "  Take Imm operand as label in disassembly." << std::endl;
	std::cout << "OFFSETISLAB addr" << std::endl;
	std::cout << "OFFISLAB addr" << std::endl;
	std::cout << "OFSISLAB addr" << std::endl;
	std::cout << "  Take address offset as label in disassembly." << std::endl;

	std::cout << "DELSYM addr label" << std::endl;
	std::cout << "  Delete a symbol.  A symbol and comment associated with the address will be deleted." << std::endl;

	std::cout << "SYM" << std::endl;
	std::cout << "  Print symbol table." << std::endl;
	std::cout << "SYMLAB" << std::endl;
	std::cout << "  Print jump label and procedures in the symbol table." << std::endl;
	std::cout << "SYMPROC" << std::endl;
	std::cout << "  Print procedures in the symbol table." << std::endl;
	std::cout << "SYMFIND wildcard" << std::endl;
	std::cout << "  Keyword search in the symbol table." << std::endl;
	std::cout << "  Keyword can use wildcard." << std::endl;
	std::cout << "IMPORTLSTSYM main/sub filename.lst" << std::endl;
	std::cout << "  Import .LST symbol table." << std::endl;



	std::cout << "<< Features that can be enabled|disabled >>" << std::endl;
	std::cout << "DEBUG/DEBUGGER" << std::endl;
	std::cout << "  Debugger." << std::endl;
	std::cout << "IOMON iopotMin ioportMax" << std::endl;
	std::cout << "  IO Monitor." << std::endl;
	std::cout << "  ioportMin and ioportMax are optional." << std::endl;
	std::cout << "  Can specify multiple range by enabling IOMON multiple times." << std::endl;
	std::cout << "FDCMON" << std::endl;
	std::cout << "  FDC monitor." << std::endl;
	std::cout << "SUBCMDMON" << std::endl;
	std::cout << "  Monitor sub-system command when sub-CPU is unhalted." << std::endl;
	std::cout << "BIOSMON" << std::endl;
	std::cout << "  Monitor BIOS Call." << std::endl;
	std::cout << "PSGLOG" << std::endl;
	std::cout << "  PSG register-write log." << std::endl;
	std::cout << "CALLSTACK main/sub" << std::endl;
	std::cout << "CST main/sub" << std::endl;
	std::cout << "  Call stack.  In Mutsu, call stack is not automatically enabled" << std::endl;
	std::cout << "  when you enable debugger.  You can specify main or sub." << std::endl;
	std::cout << "COM0TX" << std::endl;
	std::cout << "  Print tx from COM0" << std::endl;
	std::cout << "RKANA" << std::endl;
	std::cout << "  Romaji type mode." << std::endl;
	std::cout << "AUDIOLEVEL" << std::endl;
	std::cout << "  Audio Level Meter." << std::endl;
	std::cout << "LOGDISASM" << std::endl;
	std::cout << "  Log disassembly in history." << std::endl;
	std::cout << "LOGALLREGS" << std::endl;
	std::cout << "  Log all registers in history." << std::endl;


	std::cout << "<< Event that can break or monitor>>" << std::endl;
	std::cout << "SUBUNHALT | UNHALTSUB" << std::endl;
	std::cout << "  Sub-CPU Unhalted." << std::endl;
	std::cout << "SUBCMD ##" << std::endl;
	std::cout << "  Break on Sub-CPU command.  ## is a number in hexadecimal." << std::endl;
	std::cout << "  Break timing is same as SUBUNHALT/UNHALTSUB" << std::endl;
	std::cout << "MEMREAD addr" << std::endl;
	std::cout << "MEMREAD addr DATA=byteData" << std::endl;
	std::cout << "MEMREAD addr D=byteData" << std::endl;
	std::cout << "MEMWRITE addr" << std::endl;
	std::cout << "MEMWRITE addr DATA=byteData" << std::endl;
	std::cout << "MEMWRITE addr D=byteData" << std::endl;
	std::cout << "  Memory Read/Write." << std::endl;
	std::cout << "  For memory range, do something like main:0000 00FF." << std::endl;
	std::cout << "  For clearing range, do main:0000 00FF or sub:D380 D38F." << std::endl;
	std::cout << "HWDRAW type" << std::endl;
	std::cout << "  Break on hardware VRAM write.  Type can be" << std::endl;
	std::cout << "  0:PSET 1:N/A 2:OR 3:AND 4:XOR 5:NOT 6:TILE 7:COMPARE" << std::endl;

	std::cout << "<< Printable >>" << std::endl;
	std::cout << "FDC" << std::endl;
	std::cout << "  Floppy Disk Controller Status." << std::endl;
	std::cout << "CRTC" << std::endl;
	std::cout << "  CRTC Status." << std::endl;
	std::cout << "TAPE" << std::endl;
	std::cout << "  Cassette Tape Status." << std::endl;
	std::cout << "HIST main/sub" << std::endl;
	std::cout << "  Program-Counter history (PC log)." << std::endl;
	std::cout << "SOUND" << std::endl;
	std::cout << "  AY-3-8910 and YM2203C Status." << std::endl;
	std::cout << "MEM" << std::endl;
	std::cout << "  Memory-Management Status." << std::endl;
	std::cout << "PSGLOG" << std::endl;
	std::cout << "  PSG register-write log." << std::endl;
	std::cout << "CAS0" << std::endl;
	std::cout << "  Cassette files." << std::endl;
	std::cout << "CAS1" << std::endl;
	std::cout << "  Cassette (Save) files." << std::endl;
}

FM77AVCommandInterpreter::Command FM77AVCommandInterpreter::Interpret(const std::string &cmdline) const
{
	Command cmd;

	cmd.cmdline=cmdline;
	cmd.argv=cpputil::Parser(cmdline.c_str());
	cmd.primaryCmd=CMD_NONE;

	if(0<cmd.argv.size())
	{
		auto primaryCmd=cmd.argv[0];
		cpputil::Capitalize(primaryCmd);
		auto iter=primaryCmdMap.find(primaryCmd);
		if(primaryCmdMap.end()!=iter)
		{
			cmd.primaryCmd=iter->second;
		}
	}

	return cmd;
}

void FM77AVCommandInterpreter::Error_Common(const Command &cmd)
{
	std::cout << "% " << cmd.cmdline << std::endl;
}
void FM77AVCommandInterpreter::Error_TooFewArgs(const Command &cmd)
{
	std::cout << "Too few arguments." << std::endl;
	Error_Common(cmd);
}
void FM77AVCommandInterpreter::Error_UnknownCPU(const Command &cmd)
{
	std::cout << "Unknown CPU." << std::endl;
	Error_Common(cmd);
}
void FM77AVCommandInterpreter::Error_UnknownRegister(const Command &cmd)
{
	std::cout << "Unknown Register." << std::endl;
	Error_Common(cmd);
}
void FM77AVCommandInterpreter::Error_CPUOrAddress(const Command &cmd)
{
	std::cout << "Error in CPU or Address." << std::endl;
	Error_Common(cmd);
}
void FM77AVCommandInterpreter::Error_UnknownFeature(const Command &cmd)
{
	std::cout << "Unknown Feature." << std::endl;
	Error_Common(cmd);
}
void FM77AVCommandInterpreter::Error_UnknownEvent(const Command &cmd)
{
	std::cout << "Unknown Event." << std::endl;
	Error_Common(cmd);
}
void FM77AVCommandInterpreter::Error_IllegalSubCommand(const Command &cmd)
{
	std::cout << "Illegal Sub-CPU Command." << std::endl;
	Error_Common(cmd);
}
void FM77AVCommandInterpreter::Error_WrongParameter(const Command &cmd)
{
	std::cout << "Wrong Parameter." << std::endl;
	Error_Common(cmd);
}
void FM77AVCommandInterpreter::Error_CannotSaveFile(const Command &cmd)
{
	std::cout << "Cannot Save File." << std::endl;
	Error_Common(cmd);
}
void FM77AVCommandInterpreter::Error_CannotOpenFile(const Command &cmd)
{
	std::cout << "Cannot Open File." << std::endl;
	Error_Common(cmd);
}
void FM77AVCommandInterpreter::Error_Addressing(const Command &cmd)
{
	std::cout << "Error in addressing." << std::endl;
	Error_Common(cmd);
}
void FM77AVCommandInterpreter::Error_SymbolNotFound(const Command &cmd)
{
	std::cout << "Symbol Not Found." << std::endl;
	Error_Common(cmd);
}
void FM77AVCommandInterpreter::Error_CouldNotDeleteSymbol(const Command &cmd)
{
	std::cout << "Could Not Delete Symbol." << std::endl;
	Error_Common(cmd);
}

/* static */ std::string FM77AVCommandInterpreter::JustMonitorOrBreakString(bool justMonitor)
{
	return (justMonitor ? " Monitor" : "Break");
}

FM77AV::Address FM77AVCommandInterpreter::DecodeAddress(const FM77AV &fm77av,std::string arg,uint8_t defaultCPU,uint8_t defaultAddrType)
{
	FM77AV::Address addr;
	addr.type=defaultAddrType;

	const MC6809 *defCPU=nullptr;
	if(CPU_MAIN==defaultCPU || CPU_SUB==defaultCPU)
	{
		defCPU=&fm77av.CPU(defaultCPU);
	}

	for(int i=0; i<arg.size(); ++i)
	{
		if(':'==arg[i])
		{
			auto addrStr=arg.substr(0,i);
			addr.type=FM77AV::StrToAddressType(addrStr);
			if(FM77AV::ADDR_MAIN==addr.type)
			{
				defCPU=&fm77av.mainCPU;
			}
			if(FM77AV::ADDR_SUB==addr.type)
			{
				defCPU=&fm77av.subCPU;
			}
			arg=arg.substr(i+1);
			break;
		}
	}

	FM77AVLineParserHexadecimal parser(&fm77av.mainCPU,&fm77av.subCPU,defCPU);
	if(true==parser.Analyze(arg))
	{
		addr.addr=parser.Evaluate();
		if(true==parser.evaluationError)
		{
			std::cout << "Error in offset description." << std::endl;
			addr.type=FM77AV::ADDR_NONE;
			addr.addr=0;
		}
	}
	else
	{
		std::cout << "Error in offset description." << std::endl;
		addr.type=FM77AV::ADDR_NONE;
		addr.addr=0;
	}
	return addr;
}
/* static */ FM77AV::CPUAddr FM77AVCommandInterpreter::MakeCPUandAddress(const FM77AV &fm77av,std::string arg)
{
	FM77AV::CPUAddr ptr;
	ptr.cpu=CPU_UNKNOWN;

	std::string cpuStr;
	for(int i=0; i<arg.size(); ++i)
	{
		if(':'==arg[i])
		{
			std::string addrStr=arg.substr(i+1);
			ptr.cpu=StrToCPU(cpuStr);
			if(CPU_UNKNOWN!=ptr.cpu)
			{
				FM77AVLineParserHexadecimal parser(&fm77av.mainCPU,&fm77av.subCPU,&fm77av.CPU(ptr.cpu));
				if(true==parser.Analyze(addrStr))
				{
					ptr.addr=parser.Evaluate();
				}
				else
				{
					std::cout << "Error in offset description." << std::endl;
					ptr.cpu=CPU_UNKNOWN;
					ptr.addr=0;
				}
			}
		}
		else
		{
			cpuStr.push_back(arg[i]);
		}
	}
	return ptr;
}
/* static */ FM77AV::CPUAddr FM77AVCommandInterpreter::MakeCPUandAddress(const FM77AVThread &thr,const FM77AV &fm77av,std::string arg)
{
	auto ptr=MakeCPUandAddress(fm77av,arg);
	if(CPU_UNKNOWN==ptr.cpu)
	{
		ptr.cpu=thr.OnlyOneCPUIsUnmuted();
		if(CPU_UNKNOWN!=ptr.cpu)
		{
			FM77AVLineParserHexadecimal parser(&fm77av.mainCPU,&fm77av.subCPU,&fm77av.CPU(ptr.cpu));
			if(true==parser.Analyze(arg))
			{
				ptr.addr=parser.Evaluate();
			}
			else
			{
				std::cout << "Error in offset description." << std::endl;
				ptr.cpu=CPU_UNKNOWN;
				ptr.addr=0;
			}
		}
	}
	return ptr;
}
/* static */ uint32_t FM77AVCommandInterpreter::DecodePhysicalAddress(const FM77AV &fm77av,std::string arg)
{
	std::string cpuStr;
	for(int i=0; i<arg.size(); ++i)
	{
		if(':'==arg[i])
		{
			std::string addrStr=arg.substr(i+1);
			cpputil::Capitalize(cpuStr);
			if("P"==cpuStr || "PHYS"==cpuStr)
			{
				FM77AVLineParserHexadecimal parser(&fm77av.mainCPU,&fm77av.subCPU,&fm77av.mainCPU);
				if(true==parser.Analyze(addrStr))
				{
					return parser.Evaluate();
				}
				else
				{
					std::cout << "Error in offset description." << std::endl;
					break;
				}
			}
		}
		else
		{
			cpuStr.push_back(arg[i]);
		}
	}
	return 0xFFFFFFFF;
}
/* static */ uint16_t FM77AVCommandInterpreter::MakeAddressForCPU(const FM77AV &fm77av,const MC6809 &cpu,std::string arg)
{
	FM77AVLineParserHexadecimal parser(&fm77av.mainCPU,&fm77av.subCPU,&cpu);
	if(true==parser.Analyze(arg))
	{
		return parser.Evaluate();
	}
	else
	{
		std::cout << "Error in offset description." << std::endl;
		return 0;
	}
}
void FM77AVCommandInterpreter::Execute(FM77AVThread &thr,FM77AV &fm77av,class Outside_World *outside_world,Command &cmd)
{
	switch(cmd.primaryCmd)
	{
	case CMD_NONE:
		std::cout << "Do what?" << std::endl;
		break;
	case CMD_HELP:
		PrintHelp();
		break;
	case CMD_QUIT:
		thr.SetRunMode(FM77AVThread::RUNMODE_EXIT);
		cuiQuit=true;
		break;
	case CMD_NOWAIT:
		fm77av.var.noWait=true;
		break;
	case CMD_YESWAIT:
		fm77av.var.noWait=false;
		break;

	case CMD_FD0LOAD:
		Execute_FDLoad(0,fm77av,cmd);
		break;
	case CMD_FD1LOAD:
		Execute_FDLoad(1,fm77av,cmd);
		break;
	case CMD_FD0EJECT:
		Execute_FDEject(0,fm77av,cmd);
		break;
	case CMD_FD1EJECT:
		Execute_FDEject(1,fm77av,cmd);
		break;
	case CMD_FD0WRITEPROTECT:
		fm77av.fdc.SetWriteProtect(0,true);
		break;
	case CMD_FD0WRITEUNPROTECT:
		fm77av.fdc.SetWriteProtect(0,false);
		break;
	case CMD_FD1WRITEPROTECT:
		fm77av.fdc.SetWriteProtect(1,true);
		break;
	case CMD_FD1WRITEUNPROTECT:
		fm77av.fdc.SetWriteProtect(1,false);
		break;

	case CMD_TAPE_LOAD:
		Execute_LoadTape(thr,fm77av,cmd);
		break;
	case CMD_TAPE_SAVETAPE:
		Execute_TapeForSave(thr,fm77av,cmd);
		break;
	case CMD_TAPE_EJECT:
		Execute_TapeEject(thr,fm77av,cmd);
		break;
	case CMD_TAPE_WRITE_PROTECT:
		fm77av.dataRecorder.state.primary.t77.writeProtect=true;
		std::cout << "Write Protect TAPE." << std::endl;
		break;
	case CMD_TAPE_WRITE_UNPROTECT:
		fm77av.dataRecorder.state.primary.t77.writeProtect=false;
		std::cout << "Write Unprotect TAPE." << std::endl;
		break;
	case CMD_TAPE_REWIND:
		fm77av.dataRecorder.Rewind();
		std::cout << "Rewind TAPE." << std::endl;
		break;
	case CMD_TAPE_REC_BUTTON_ON:
		fm77av.dataRecorder.state.recButton=true;
		std::cout << "REC Button ON." << std::endl;
		break;
	case CMD_TAPE_REC_BUTTON_RELEASE:
		fm77av.dataRecorder.state.recButton=false;
		std::cout << "REC Button OFF." << std::endl;
		break;
	case CMD_TAPE_SAVE_AS:
		if(2<=cmd.argv.size())
		{
			if(true==fm77av.dataRecorder.state.primary.t77.SaveAs(cmd.argv[1]))
			{
				std::cout << "Saved tape image." << std::endl;
			}
			else
			{
				Error_CannotSaveFile(cmd);
			}
		}
		else
		{
			Error_TooFewArgs(cmd);
		}
		break;
	case CMD_FORCE_QUIT:
		exit(0);
		break;
	case CMD_RESET:
		fm77av.Reset();
		break;
	case CMD_RUN:
		Execute_Run(thr,fm77av,outside_world,cmd);
		break;
	case CMD_PAUSE:
		thr.SetRunMode(FM77AVThread::RUNMODE_PAUSE);
		thr.PrintStatus(fm77av,false,false);
		break;
	case CMD_RUN_ONE_INSTRUCTION:
		thr.SetRunMode(FM77AVThread::RUNMODE_ONE_INSTRUCTION);
		break;
	case CMD_MUTE:
		if(cmd.argv.size()<2)
		{
			thr.output.main.mute=true;
			thr.output.sub.mute=true;
			std::cout << "Muted Main and Sub CPU" << std::endl;
		}
		else
		{
			switch(StrToCPU(cmd.argv[1]))
			{
			case CPU_MAIN:
				thr.output.main.mute=true;
				std::cout << "Muted Main CPU" << std::endl;
				break;
			case CPU_SUB:
				thr.output.sub.mute=true;
				std::cout << "Muted Sub CPU" << std::endl;
				break;
			default:
				Error_UnknownCPU(cmd);
				break;
			}
		}
		break;
	case CMD_UNMUTE:
		if(cmd.argv.size()<2)
		{
			thr.output.main.mute=false;
			thr.output.sub.mute=false;
			std::cout << "Unmuted Main and Sub CPU" << std::endl;
		}
		else
		{
			switch(StrToCPU(cmd.argv[1]))
			{
			case CPU_MAIN:
				thr.output.main.mute=false;
				std::cout << "Unmuted Main CPU" << std::endl;
				break;
			case CPU_SUB:
				thr.output.sub.mute=false;
				std::cout << "Unmuted Sub CPU" << std::endl;
				break;
			default:
				Error_UnknownCPU(cmd);
				break;
			}
		}
		break;
	case CMD_ENABLE:
		Execute_Enable(thr,fm77av,outside_world,cmd);
		break;
	case CMD_DISABLE:
		Execute_Disable(thr,fm77av,outside_world,cmd);
		break;

	case CMD_START_AUDIO_RECORDING:
		fm77av.sound.StartRecording();
		break;
	case CMD_END_AUDIO_RECORDING:
		fm77av.sound.EndRecording();
		break;
	case CMD_SAVE_AUDIO_RECORDING:
		if(2<=cmd.argv.size())
		{
			fm77av.sound.SaveRecording(cmd.argv[1]);
		}
		break;
	case CMD_FMCH:
		if(4<=cmd.argv.size())
		{
			fm77av.sound.state.ym2203c.channelMute[0]=(0==cpputil::Atoi(cmd.argv[1].c_str()));
			fm77av.sound.state.ym2203c.channelMute[1]=(0==cpputil::Atoi(cmd.argv[2].c_str()));
			fm77av.sound.state.ym2203c.channelMute[2]=(0==cpputil::Atoi(cmd.argv[3].c_str()));
		}
		else
		{
			Error_TooFewArgs(cmd);
		}
		break;
	case CMD_PSGCH:
		if(4<=cmd.argv.size())
		{
			fm77av.sound.state.ay38910.channelMute[0]=(0==cpputil::Atoi(cmd.argv[1].c_str()));
			fm77av.sound.state.ay38910.channelMute[1]=(0==cpputil::Atoi(cmd.argv[2].c_str()));
			fm77av.sound.state.ay38910.channelMute[2]=(0==cpputil::Atoi(cmd.argv[3].c_str()));
		}
		else
		{
			Error_TooFewArgs(cmd);
		}
		break;

	case CMD_PRINT_STATUS:
		thr.PrintStatus(fm77av,false,false);
		break;
	case CMD_PRINT_HISTORY:
		{
			decltype(cmd.argv) argvCopy;
			std::swap(argvCopy,cmd.argv);
			cmd.argv.push_back("PRI");
			cmd.argv.insert(cmd.argv.end(),argvCopy.begin(),argvCopy.end());
			Execute_PrintHistory(thr,fm77av,cmd);
		}
		break;

	case CMD_DISASM:
		Execute_Disassemble(thr,fm77av,outside_world,cmd);
		break;
	case CMD_DISASM_MAIN:
		Execute_Disassemble_Main(thr,fm77av,outside_world,cmd);
		break;
	case CMD_DISASM_SUB:
		Execute_Disassemble_Sub(thr,fm77av,outside_world,cmd);
		break;
	case CMD_MEMDUMP:
		Execute_MemoryDump(thr,fm77av,cmd);
		break;
	case CMD_DUMP:
		Execute_Dump(thr,fm77av,cmd);
		break;
	case CMD_SAVE_HISTORY:
		Execute_SaveHistory(thr,fm77av,cmd);
		break;
	case CMD_ADD_BREAKPOINT:
		Execute_AddBreakPoint(thr,fm77av,cmd);
		break;
	case CMD_ADD_BREAKPOINT_WITH_PASSCOUNT:
		Execute_AddBreakPointWithPassCount(thr,fm77av,cmd);
		break;
	case CMD_ADD_MONITORPOINT:
		Execute_AddMonitorPoint(thr,fm77av,cmd);
		break;
	case CMD_DELETE_BREAKPOINT:
		Execute_DeleteBreakPoint(thr,fm77av,cmd);
		break;
	case CMD_LIST_BREAKPOINTS:
		Execute_ListBreakPoints(thr,fm77av,cmd);
		break;
	case CMD_BREAK_ON:
		Execute_BreakOnOrMonitor(thr,fm77av,cmd,false);
		break;
	case CMD_DONT_BREAK_ON:
		Execute_DontBreakOn(thr,fm77av,cmd);
		break;
	case CMD_MONITOR:
		Execute_BreakOnOrMonitor(thr,fm77av,cmd,true);
		break;
	case CMD_DONT_MONITOR:
		Execute_DontBreakOn(thr,fm77av,cmd);
		break;
	case CMD_TYPE_KEYBOARD:
		Execute_TypeKeyboard(fm77av,cmd);
		break;
	case CMD_KEYBOARD:
		if (cmd.argv.size() < 2) {
			Error_TooFewArgs(cmd);
		}
		else if(nullptr!=outside_world)
		{
			std::string MODE=cmd.argv[1];
			cpputil::Capitalize(MODE);
			if("TRANS"==MODE || "TRANSLATE"==MODE || "TRANS1"==MODE || "TRANSLATE1"==MODE)
			{
				outside_world->keyboardMode=FM77AV_KEYBOARD_MODE_TRANSLATION1;
				std::cout << "Keyboard TRANSLATION Mode 1 (ESC->ESC+BREAK)" << std::endl;
			}
			else if("TRANS2"==MODE || "TRANSLATE2"==MODE)
			{
				outside_world->keyboardMode=FM77AV_KEYBOARD_MODE_TRANSLATION2;
				std::cout << "Keyboard TRANSLATION Mode 2 (ESC->ESC)" << std::endl;
			}
			else if("TRANS3"==MODE || "TRANSLATE3"==MODE)
			{
				outside_world->keyboardMode=FM77AV_KEYBOARD_MODE_TRANSLATION3;
				std::cout << "Keyboard TRANSLATION Mode 3 (ESC->BREAK)" << std::endl;
			}
			else if("DIRECT"==MODE)
			{
				outside_world->keyboardMode=FM77AV_KEYBOARD_MODE_DIRECT;
				std::cout << "Keyboard DIRECT Mode" << std::endl;
			}
			else
			{
				Error_WrongParameter(cmd);
			}
		}
		break;
	case CMD_LOAD_EVENTLOG:
		if(true==fm77av.eventLog.LoadEventLog(cmd.argv[1]))
		{
			std::cout << "Loaded event log." << std::endl;
			std::cout << "PLAYEVT command for play back.\n" << std::endl;
		}
		else
		{
			Error_CannotOpenFile(cmd);
		}
		break;
	case CMD_PLAY_EVENTLOG:
		std::cout << "Start Event-Log Playback." << std::endl;
		fm77av.eventLog.BeginPlayback(fm77av.state.fm77avTime);
		break;
	case CMD_STOP_EVENTLOG:
		std::cout << "Stop Event-Log Playback." << std::endl;
		fm77av.eventLog.StopPlayback();
		break;
	case CMD_SAVE_SCREENSHOT:
		Execute_SaveScreenShot(fm77av,cmd);
		break;
	case CMD_QUICK_SCREENSHOT:
		Execute_QuickScreenShot(fm77av,cmd);
		break;
	case CMD_QUICK_SCREENSHOT_DIR:
		Execute_QuickScreenShotDirectory(fm77av,cmd);
		break;
	case CMD_SAVE_COM0OUT:
		Execute_SaveCOM0Out(fm77av,cmd);
		break;
	case CMD_CLEAR_COM0OUT:
		Execute_ClearCom0Out(fm77av,cmd);
		break;
	case CMD_LET:
		Execute_Let(thr,fm77av,cmd);
		break;
	case CMD_SETSPEED:
		Execute_SetSpeed(thr,fm77av,cmd);
		break;
	case CMD_GAMEPORT:
		Execute_Gameport(fm77av,outside_world,cmd);
		break;
	case CMD_SAVE_STATE:
		if(2<=cmd.argv.size())
		{
			if(true!=fm77av.SaveState(cmd.argv[1]))
			{
				Error_CannotSaveFile(cmd);
			}
			else
			{
				std::cout << "Saved " << cmd.argv[1] << std::endl;
			}
		}
		else
		{
			Error_TooFewArgs(cmd);
		}
		break;
	case CMD_LOAD_STATE:
		if(2<=cmd.argv.size())
		{
			if(true!=fm77av.LoadState(cmd.argv[1],*outside_world))
			{
				Error_CannotOpenFile(cmd);
			}
			else
			{
				std::cout << "Loaded " << cmd.argv[1] << std::endl;
			}
		}
		else
		{
			Error_TooFewArgs(cmd);
		}
		break;
	case CMD_AUTOSTOPKEY:
		if(2<=cmd.argv.size())
		{
			auto keyCode=FM77AVKeyLabelToKeyCode(cmd.argv[1]);
			if(AVKEY_NULL!=keyCode)
			{
				std::cout << "Set auto-stop key to " << FM77AVKeyCodeToKeyLabel(keyCode) << std::endl;
				fm77av.keyboard.var.autoStopKey=keyCode;
			}
			else
			{
				std::cout << "Key code can be one of:" << std::endl;
				for(int i=0; i<AVKEY_NUM_KEYCODE; ++i)
				{
					if(AVKEY_NULL!=i)
					{
						std::cout << FM77AVKeyCodeToKeyLabel(i) << std::endl;
					}
				}
			}
		}
		else
		{
			Error_TooFewArgs(cmd);
		}
		break;
	case CMD_ADD_SYMBOL:
	case CMD_ADD_LABEL:
	case CMD_ADD_DATALABEL:
	case CMD_ADD_COMMENT:
	case CMD_DEF_RAW_BYTES:
	case CMD_IMM_IS_IOPORT:
	case CMD_IMM_IS_LABEL:
	case CMD_OFFSET_IS_LABEL:
		Execute_AddSymbol(thr,fm77av,cmd);
		break;
	case CMD_DEL_SYMBOL:
		Execute_DelSymbol(thr,fm77av,cmd);
		break;
	case CMD_IMPORT_LST_SYMTABLE:
		Execute_ImportLSTSymbolTable(thr,fm77av,cmd);
		break;

	case CMD_PRINT_SYMBOL:
	case CMD_PRINT_SYMBOL_LABEL_PROC:
	case CMD_PRINT_SYMBOL_PROC:
	case CMD_PRINT_SYMBOL_FIND:
		Execute_SymbolInquiry(thr,fm77av,cmd);
		break;
	}
}

void FM77AVCommandInterpreter::Execute_Run(FM77AVThread &thr,FM77AV &fm77av,class Outside_World *outside_world,Command &cmd)
{
	if(3<=cmd.argv.size())
	{
		auto ptr=MakeCPUandAddress(thr,fm77av,cmd.argv[1]);
		if(CPU_UNKNOWN!=ptr.cpu)
		{
			FM77AVLineParserHexadecimal parser(&fm77av.mainCPU,&fm77av.subCPU,&fm77av.CPU(ptr.cpu));
			parser.Analyze(cmd.argv[2].data());
			auto endAddr=parser.Evaluate();
			fm77av.CPU(ptr.cpu).debugger.SetOneTimeBreakPoint(ptr.addr,endAddr);
			fm77av.mainCPU.debugger.ClearStopFlag();
			fm77av.subCPU.debugger.ClearStopFlag();
			thr.SetRunMode(FM77AVThread::RUNMODE_RUN);
		}
		else
		{
			Error_CPUOrAddress(cmd);
		}
	}
	else if(2<=cmd.argv.size())
	{
		auto ptr=MakeCPUandAddress(thr,fm77av,cmd.argv[1]);
		if(CPU_UNKNOWN!=ptr.cpu)
		{
			fm77av.CPU(ptr.cpu).debugger.SetOneTimeBreakPoint(ptr.addr,ptr.addr);
			fm77av.mainCPU.debugger.ClearStopFlag();
			fm77av.subCPU.debugger.ClearStopFlag();
			thr.SetRunMode(FM77AVThread::RUNMODE_RUN);
		}
		else
		{
			Error_CPUOrAddress(cmd);
		}
	}
	else
	{
		fm77av.mainCPU.debugger.ClearStopFlag();
		fm77av.subCPU.debugger.ClearStopFlag();
		thr.SetRunMode(FM77AVThread::RUNMODE_RUN);
	}
}
void FM77AVCommandInterpreter::Execute_Enable(FM77AVThread &thr,FM77AV &fm77av,class Outside_World *outside_world,Command &cmd)
{
	if(2<=cmd.argv.size())
	{
		cpputil::Capitalize(cmd.argv[1]);
		auto foundFeature=featureMap.find(cmd.argv[1]);
		if(featureMap.end()==foundFeature)
		{
			Error_UnknownFeature(cmd);
			return;
		}
		switch(foundFeature->second)
		{
		case ENABLE_DEBUGGER:
			fm77av.mainCPU.debugger.enabled=true;
			fm77av.subCPU.debugger.enabled=true;
			std::cout << "Enabled debugger." << std::endl;
			break;
		case ENABLE_IOMONITOR:
			if(2==cmd.argv.size())
			{
				// All IO Port Read/Write
				for(auto &b : fm77av.var.monitorIOReadMain)
				{
					b=true;
				}
				for(auto &b : fm77av.var.monitorIOReadSub)
				{
					b=true;
				}
				for(auto &b : fm77av.var.monitorIOWriteMain)
				{
					b=true;
				}
				for(auto &b : fm77av.var.monitorIOWriteSub)
				{
					b=true;
				}
				std::cout << "Enabled IO Monitor All IO Addresses Read and Write" << std::endl;
			}
			else if(3==cmd.argv.size())
			{
				auto addr=cpputil::Xtoi(cmd.argv[2].c_str());
				if(0xFD00==(addr&0xFF00))
				{
					fm77av.var.monitorIOReadMain[addr&0xFF]=true;
					fm77av.var.monitorIOWriteMain[addr&0xFF]=true;
					std::cout << "Enabled IO Monitor for main-CPU " << cpputil::Ustox(addr) << std::endl;
				}
				if(0xD400==(addr&0xFF00))
				{
					fm77av.var.monitorIOReadSub[addr&0xFF]=true;
					fm77av.var.monitorIOWriteSub[addr&0xFF]=true;
					std::cout << "Enabled IO Monitor for sub-CPU " << cpputil::Ustox(addr) << std::endl;
				}
			}
			else if(4==cmd.argv.size())
			{
				auto addrFrom=cpputil::Xtoi(cmd.argv[2].c_str());
				auto addrTo=cpputil::Xtoi(cmd.argv[3].c_str());
				for(auto addr=addrFrom; addr<addrTo; ++addr)
				{
					if(0xFD00==(addr&0xFF00))
					{
						fm77av.var.monitorIOReadMain[addr&0xFF]=true;
						fm77av.var.monitorIOWriteMain[addr&0xFF]=true;
						std::cout << "Enabled IO Monitor for main-CPU " << cpputil::Ustox(addr) << std::endl;
					}
					if(0xD400==(addr&0xFF00))
					{
						fm77av.var.monitorIOReadSub[addr&0xFF]=true;
						fm77av.var.monitorIOWriteSub[addr&0xFF]=true;
						std::cout << "Enabled IO Monitor for sub-CPU " << cpputil::Ustox(addr) << std::endl;
					}
				}
			}
			break;
		case ENABLE_FDCMONITOR:
			fm77av.fdc.monitorFDC=true;
			std::cout << "Enabled FDC Monitor." << std::endl;
			break;
		case ENABLE_SUBSYSCMD_MONITOR:
			fm77av.var.monitorSubSysCmd=true;
			std::cout << "Enabled Sub-System Command Monitor." << std::endl;
			break;
		case ENABLE_BIOSCMD_MONITOR:
			fm77av.var.monitorBIOSCall=true;
			std::cout << "Enabled BIOS Monitor." << std::endl;
			break;
		case ENABLE_CALLSTACK:
			Execute_EnableCallStack(thr,fm77av,cmd);
			break;
		case ENABLE_PRINT_COM0:
			fm77av.serialport.cli0.printRecvText=true;
			std::cout << "Enabled COM0 Print." << std::endl;
			break;
		case ENABLE_PSG_LOG:
			fm77av.sound.state.ay38910.registerLog.clear();
			fm77av.sound.state.ay38910.takeRegisterLog=true;
			std::cout << "Enabled PSG AY3-8910 register log." << std::endl;
			break;
		case ENABLE_AUTOSTOP:
			if(3<=cmd.argv.size())
			{
				auto autoStopType=FM77AVKeyboard::StrToAutoStop(cmd.argv[2]);
				if(FM77AVKeyboard::AUTOSTOP_NONE!=autoStopType)
				{
					fm77av.keyboard.var.autoStopAfterThis=autoStopType;
					std::cout << "Enabled Auto Stop (" << FM77AVKeyboard::AutoStopToStr(autoStopType) << ")" << std::endl;
				}
				else
				{
					std::cout << "AutoStopType can be one of:" << std::endl;
					std::cout << "  AFTER_NUM_RELEASE" << std::endl;
					std::cout << "  AFTER_NUM_RELEASE_AND_RETYPE" << std::endl;
					std::cout << "  AFTER_ARROW_RELEASE_AND_RETYPE" << std::endl;
					std::cout << "  AFTER_ARROW_RELEASE_AND_RETYPE" << std::endl;
					std::cout << "  AFTER_ANY_KEY_RELEASE" << std::endl;
				}
			}
			else
			{
				fm77av.keyboard.var.autoStopAfterThis=FM77AVKeyboard::AUTOSTOP_AFTER_NUM_RELEASE_AND_RETYPE;;
				std::cout << "Enabled Auto Stop (" << FM77AVKeyboard::AutoStopToStr(fm77av.keyboard.var.autoStopAfterThis) << ")" << std::endl;
			}
			break;
		case ENABLE_RKANA:
			fm77av.keyboard.var.rKanaMode=true;
			fm77av.keyboard.var.romaji.clear();
			std::cout << "Enabled Romaji Type Mode" << std::endl;
			break;
		case ENABLE_AUDIOLEVELMETER	:
			outside_world->visualizeAudioOut=true;
			std::cout << "Enabled Audio Level Meter" << std::endl;
			break;
		case ENABLE_LOG_DISASM:
			fm77av.mainCPU.debugger.logDisassembly=true;
			fm77av.subCPU.debugger.logDisassembly=true;
			std::cout << "Enabled logging disassembly in history." << std::endl;
			break;
		case ENABLE_LOG_ALLREGISTERS:
			fm77av.mainCPU.debugger.logAllRegisters=true;
			fm77av.subCPU.debugger.logAllRegisters=true;
			std::cout << "Enabled logging all registers in history." << std::endl;
			break;
		default:
			std::cout << "Enable What?" << std::endl;
			Error_WrongParameter(cmd);
			break;
		}
	}
	else
	{
		Error_TooFewArgs(cmd);
	}
}
void FM77AVCommandInterpreter::Execute_Disable(FM77AVThread &thr,FM77AV &fm77av,class Outside_World *outside_world,Command &cmd)
{
	if(2<=cmd.argv.size())
	{
		cpputil::Capitalize(cmd.argv[1]);
		auto foundFeature=featureMap.find(cmd.argv[1]);
		if(featureMap.end()==foundFeature)
		{
			Error_UnknownFeature(cmd);
			return;
		}
		switch(foundFeature->second)
		{
		case ENABLE_DEBUGGER:
			fm77av.mainCPU.debugger.enabled=false;
			fm77av.subCPU.debugger.enabled=false;
			std::cout << "Disabled debugger." << std::endl;
			break;
		case ENABLE_IOMONITOR:
			if(2==cmd.argv.size())
			{
				// All IO Port Read/Write
				for(auto &b : fm77av.var.monitorIOReadMain)
				{
					b=false;
				}
				for(auto &b : fm77av.var.monitorIOReadSub)
				{
					b=false;
				}
				for(auto &b : fm77av.var.monitorIOWriteMain)
				{
					b=false;
				}
				for(auto &b : fm77av.var.monitorIOWriteSub)
				{
					b=false;
				}
				std::cout << "Disabled IO Monitor All IO Addresses Read and Write" << std::endl;
			}
			break;
		case ENABLE_FDCMONITOR:
			fm77av.fdc.monitorFDC=false;
			std::cout << "Disabled FDC Monitor." << std::endl;
			break;
		case ENABLE_SUBSYSCMD_MONITOR:
			fm77av.var.monitorSubSysCmd=false;
			std::cout << "Disabled Sub-System Command Monitor." << std::endl;
			break;
		case ENABLE_BIOSCMD_MONITOR:
			fm77av.var.monitorBIOSCall=false;
			std::cout << "Disabled BIOS Monitor." << std::endl;
			break;
		case ENABLE_CALLSTACK:
			Execute_DisableCallStack(thr,fm77av,cmd);
			break;
		case ENABLE_PRINT_COM0:
			fm77av.serialport.cli0.printRecvText=false;
			std::cout << "Disabled COM0 Print." << std::endl;
			break;
		case ENABLE_PSG_LOG:
			fm77av.sound.state.ay38910.takeRegisterLog=false;
			std::cout << "Disabled PSG AY3-8910 register log." << std::endl;
			break;
		case ENABLE_AUTOSTOP:
			fm77av.keyboard.var.autoStopAfterThis=FM77AVKeyboard::AUTOSTOP_NONE;
			std::cout << "Disabled Auto Stop" << std::endl;
			break;
		case ENABLE_RKANA:
			fm77av.keyboard.var.rKanaMode=false;
			fm77av.keyboard.var.romaji.clear();
			std::cout << "Disabled Romaji Type Mode" << std::endl;
			break;
		case ENABLE_AUDIOLEVELMETER	:
			outside_world->visualizeAudioOut=false;
			std::cout << "Disabled Audio Level Meter" << std::endl;
			break;
		case ENABLE_LOG_DISASM:
			fm77av.mainCPU.debugger.logDisassembly=false;
			fm77av.subCPU.debugger.logDisassembly=false;
			std::cout << "Disabled logging disassembly in history." << std::endl;
			break;
		case ENABLE_LOG_ALLREGISTERS:
			fm77av.mainCPU.debugger.logAllRegisters=false;
			fm77av.subCPU.debugger.logAllRegisters=false;
			std::cout << "Disabled logging all registers in history." << std::endl;
			break;
		default:
			std::cout << "Disable What?" << std::endl;
			Error_WrongParameter(cmd);
			break;
		}
	}
	else
	{
		Error_TooFewArgs(cmd);
	}
}
void FM77AVCommandInterpreter::Execute_Disassemble(FM77AVThread &thr,FM77AV &fm77av,class Outside_World *outside_world,Command &cmd)
{
	if(2<=cmd.argv.size())
	{
		auto ptr=MakeCPUandAddress(thr,fm77av,cmd.argv[1]);
		if(CPU_UNKNOWN!=ptr.cpu)
		{
			fm77av.var.lastDisassemblyCPU=ptr.cpu;
			auto &cpu=fm77av.CPU(ptr.cpu);
			auto &mem=fm77av.MemAccess(ptr.cpu);
			auto PC=ptr.addr;
			for(int i=0; i<DISASM_NUM_LINES; ++i)
			{
				auto inst=cpu.NonDestructiveFetchInstruction(mem,PC);
				for(auto str : cpu.WholeDisassembly(mem,PC))
				{
					std::cout << str << std::endl;
				}
				PC+=inst.length;
			}
			cpu.debugger.nextDisassemblyAddr=PC;
		}
		else
		{
			Error_UnknownCPU(cmd);
		}
	}
	else if(CPU_UNKNOWN!=fm77av.var.lastDisassemblyCPU)
	{
		auto &cpu=fm77av.CPU(fm77av.var.lastDisassemblyCPU);
		auto &mem=fm77av.MemAccess(fm77av.var.lastDisassemblyCPU);
		auto PC=cpu.debugger.nextDisassemblyAddr;
		for(int i=0; i<DISASM_NUM_LINES; ++i)
		{
			auto inst=cpu.NonDestructiveFetchInstruction(mem,PC);
			for(auto str : cpu.WholeDisassembly(mem,PC))
			{
				std::cout << str << std::endl;
			}
			PC+=inst.length;
		}
		cpu.debugger.nextDisassemblyAddr=PC;
	}
}
void FM77AVCommandInterpreter::Execute_Disassemble_Main(FM77AVThread &thr,FM77AV &fm77av,class Outside_World *outside_world,Command &cmd)
{
	auto &cpu=fm77av.mainCPU;
	auto &mem=fm77av.mainMemAcc;
	uint16_t PC;
	fm77av.var.lastDisassemblyCPU=CPU_MAIN;
	if(cmd.argv.size()<2)
	{
		PC=cpu.debugger.nextDisassemblyAddr;
	}
	else
	{
		PC=MakeAddressForCPU(fm77av,cpu,cmd.argv[1]);
	}
	for(int i=0; i<DISASM_NUM_LINES; ++i)
	{
		auto inst=cpu.NonDestructiveFetchInstruction(mem,PC);
		for(auto str : cpu.WholeDisassembly(mem,PC))
		{
			std::cout << str << std::endl;
		}
		PC+=inst.length;
	}
	cpu.debugger.nextDisassemblyAddr=PC;
}
void FM77AVCommandInterpreter::Execute_Disassemble_Sub(FM77AVThread &thr,FM77AV &fm77av,class Outside_World *outside_world,Command &cmd)
{
	auto &cpu=fm77av.subCPU;
	auto &mem=fm77av.subMemAcc;
	uint16_t PC;
	fm77av.var.lastDisassemblyCPU=CPU_SUB;
	if(cmd.argv.size()<2)
	{
		PC=cpu.debugger.nextDisassemblyAddr;
	}
	else
	{
		PC=MakeAddressForCPU(fm77av,cpu,cmd.argv[1]);
	}
	for(int i=0; i<DISASM_NUM_LINES; ++i)
	{
		auto inst=cpu.NonDestructiveFetchInstruction(mem,PC);
		for(auto str : cpu.WholeDisassembly(mem,PC))
		{
			std::cout << str << std::endl;
		}
		PC+=inst.length;
	}
	cpu.debugger.nextDisassemblyAddr=PC;
}
void FM77AVCommandInterpreter::Execute_Dump(FM77AVThread &thr,FM77AV &fm77av,Command &cmd)
{
	if(cmd.argv.size()<2)
	{
		Error_TooFewArgs(cmd);
	}
	else
	{
		auto DUMPABLE=cmd.argv[1];
		cpputil::Capitalize(DUMPABLE);
		auto found=dumpableMap.find(DUMPABLE);
		if(found!=dumpableMap.end())
		{
			switch(found->second)
			{
			case DUMP_FDC:
				for(auto str : fm77av.fdc.GetStatusText())
				{
					std::cout << str << std::endl;
				}
				break;
			case DUMP_CRTC:
				for(auto str : fm77av.crtc.GetStatusText())
				{
					std::cout << str << std::endl;
				}
				break;
			case DUMP_TAPE:
				for(auto str : fm77av.dataRecorder.GetStatusText(fm77av.state.fm77avTime))
				{
					std::cout << str << std::endl;
				}
				break;
			case DUMP_PC_LOG:
				Execute_PrintHistory(thr,fm77av,cmd);
				break;
			case DUMP_SOUND:
				for(auto str : fm77av.sound.GetStatusText())
				{
					std::cout << str << std::endl;
				}
				break;
			case DUMP_MEMORY:
				for(auto str : fm77av.physMem.GetStatusText())
				{
					std::cout << str << std::endl;
				}
				for(auto str : fm77av.mainMemAcc.GetStatusText())
				{
					std::cout << str << std::endl;
				}
				break;
			case DUMP_CALLSTACK:
				Execute_PrintCallStack(thr,fm77av,cmd);
				break;
			case DUMP_IRQ:
				for(auto str : fm77av.GetIRQStatusText())
				{
					std::cout << str << std::endl;
				}
				break;
			case DUMP_PSG_LOG:
				for(auto str : fm77av.sound.state.ay38910.FormatRegisterLog())
				{
					std::cout << str << std::endl;
				}
				break;
			case DUMP_CAS0:
			case DUMP_CAS1:
				{
					auto *tapePtr=(found->second==DUMP_CAS0 ? &fm77av.dataRecorder.state.primary : &fm77av.dataRecorder.state.toSave);
					for(auto file : tapePtr->t77.Files())
					{
						std::cout << file.fName;
						for(int i=file.fName.size(); i<10; ++i)
						{
							std::cout << ' ';
						}
						switch(file.fType)
						{
						case FM7File::FTYPE_BASIC_BINARY:
							std::cout << "(F-BASIC Binary)";
							break;
						case FM7File::FTYPE_BASIC_ASCII:
							std::cout << "(F-BASIC ASCII) ";
							break;
						case FM7File::FTYPE_BINARY:
							std::cout << "(Machine-Go)    ";
							break;
						case FM7File::FTYPE_DATA_BINARY:
							std::cout << "(Binary Data)   ";
							break;
						case FM7File::FTYPE_DATA_ASCII:
							std::cout << "(ASCII Data)    ";
							break;
						case FM7File::FTYPE_UNKNOWN:
							std::cout << "(Unknown)       ";
							break;
						}
						std::cout << " at " << file.ptr << std::endl;
					}
				}
				break;
			}
		}
		else
		{
			// Dump memory version 1.
		}
	}
}
void FM77AVCommandInterpreter::Execute_PrintHistory(FM77AVThread &thr,FM77AV &av,Command &cmd)
{
	int mainOrSub=CPU_UNKNOWN;
	int n=32;
	int paramBase=3;

	if(3<=cmd.argv.size())
	{
		mainOrSub=StrToCPU(cmd.argv[2]);
		paramBase=3;
	}
	if(CPU_UNKNOWN==mainOrSub)
	{
		mainOrSub=thr.OnlyOneCPUIsUnmuted();
		paramBase=2;
	}
	if(CPU_UNKNOWN==mainOrSub)
	{
		// There is a possibility that mainOrSub is omitted, default to the one unmuted.
		if(CPU_UNKNOWN==mainOrSub)
		{
			Error_UnknownCPU(cmd);
			return;
		}
	}
	if(paramBase<cmd.argv.size())
	{
		n=cpputil::Atoi(cmd.argv[paramBase].c_str());
	}

	auto list=av.CPU(mainOrSub).debugger.GetPCLog(n);
	// auto &symTable=av.debugger.GetSymTable();
	for(auto iter=list.rbegin(); iter!=list.rend(); ++iter)
	{
		std::cout << cpputil::Ustox(iter->regs.PC);
		if(true==av.CPU(mainOrSub).debugger.logAllRegisters)
		{
			std::cout << " CC=" << cpputil::Ubtox(iter->regs.CC);
			std::cout << " A=" << cpputil::Ubtox(iter->regs.A());
			std::cout << " B=" << cpputil::Ubtox(iter->regs.B());
			std::cout << " DP=" << cpputil::Ubtox(iter->regs.DP);
			std::cout << " X=" << cpputil::Ustox(iter->regs.X);
			std::cout << " Y=" << cpputil::Ustox(iter->regs.Y);
			std::cout << " U=" << cpputil::Ustox(iter->regs.U);
			std::cout << " S=" << cpputil::Ustox(iter->regs.S);
		}
		else
		{
			std::cout << " CC=" << cpputil::Ubtox(iter->regs.CC);
			std::cout << " S=" << cpputil::Ustox(iter->regs.S);
		}

		if(true==av.CPU(mainOrSub).debugger.logDisassembly)
		{
			std::cout << "  " << iter->disasm;
			if(1<iter->count) // Space for showning count
			{
				for(int i=iter->disasm.size(); i<20; ++i)
				{
					std::cout << " ";
				}
			}
		}

		if(1<iter->count)
		{
			std::cout << " (" << cpputil::Itoa((unsigned int)iter->count) << ")";
		}
		// auto symbolPtr=symTable.Find(iter->SEG,iter->OFFSET);
		// if(nullptr!=symbolPtr)
		// {
		// 	std::cout << " " << symbolPtr->Format();
		// }
		std::cout << std::endl;
	}
}
void FM77AVCommandInterpreter::Execute_SaveHistory(FM77AVThread &thr,FM77AV &av,Command &cmd)
{
	if(2<=cmd.argv.size())
	{
		std::ofstream ofp(cmd.argv[1]);
		if(ofp.is_open())
		{
			for(int mainOrSub=0; mainOrSub<2; ++mainOrSub)
			{
				ofp << CPUToStr(mainOrSub) << " CPU" << std::endl;

				auto list=av.CPU(mainOrSub).debugger.GetPCLog(MC6809::PC_LOG_SIZE);
				// auto &symTable=av.debugger.GetSymTable();
				for(auto iter=list.rbegin(); iter!=list.rend(); ++iter)
				{
					ofp << cpputil::Ustox(iter->regs.PC);
					if(true==av.CPU(mainOrSub).debugger.logAllRegisters)
					{
						ofp << " CC=" << cpputil::Ubtox(iter->regs.CC);
						ofp << " A=" << cpputil::Ubtox(iter->regs.A());
						ofp << " B=" << cpputil::Ubtox(iter->regs.B());
						ofp << " DP=" << cpputil::Ubtox(iter->regs.DP);
						ofp << " X=" << cpputil::Ustox(iter->regs.X);
						ofp << " Y=" << cpputil::Ustox(iter->regs.Y);
						ofp << " U=" << cpputil::Ustox(iter->regs.U);
						ofp << " S=" << cpputil::Ustox(iter->regs.S);
					}
					else
					{
						ofp << " CC=" << cpputil::Ubtox(iter->regs.CC);
						ofp << " S=" << cpputil::Ustox(iter->regs.S);
					}

					if(true==av.CPU(mainOrSub).debugger.logDisassembly)
					{
						ofp << "  " << iter->disasm;
						if(1<iter->count) // Space for showning count
						{
							for(int i=iter->disasm.size(); i<20; ++i)
							{
								ofp << " ";
							}
						}
					}

					if(1<iter->count)
					{
						ofp << " (" << cpputil::Itoa((unsigned int)iter->count) << ")";
					}
					// auto symbolPtr=symTable.Find(iter->SEG,iter->OFFSET);
					// if(nullptr!=symbolPtr)
					// {
					// 	ofp << " " << symbolPtr->Format();
					// }
					ofp << std::endl;
				}
			}
			ofp.close();
		}
		else
		{
			Error_CannotSaveFile(cmd);
		}
	}
	else
	{
		Error_TooFewArgs(cmd);
	}
}
void FM77AVCommandInterpreter::Execute_MemoryDump(FM77AVThread &thr,FM77AV &fm77av,Command &cmd)
{
	if(cmd.argv.size()<2)
	{
		std::cout << "Need address." << std::endl;
		Error_TooFewArgs(cmd);
	}
	else
	{
		int wid=16,hei=16,skip=1;
		bool ascii=true;
		if(3<=cmd.argv.size())
		{
			wid=cpputil::Atoi(cmd.argv[2].c_str());
		}
		if(4<=cmd.argv.size())
		{
			hei=cpputil::Atoi(cmd.argv[3].c_str());
		}
		if(5<=cmd.argv.size())
		{
			skip=cpputil::Atoi(cmd.argv[4].c_str());
			if(skip<=0)
			{
				skip=1;
			}
		}
		if(6<=cmd.argv.size())
		{
			ascii=(0!=cpputil::Atoi(cmd.argv[5].c_str()));
		}


		auto addr=DecodeAddress(fm77av,cmd.argv[1],thr.OnlyOneCPUIsUnmuted(),thr.OnlyOneCPUIsUnmuted());
		if(FM77AV::ADDR_NONE==addr.type)
		{
			Error_Addressing(cmd);
		}
		else if(FM77AV::ADDR_PHYS==addr.type)
		{
			std::vector <unsigned char> data;
			for(uint32_t offset=0; offset<(wid*hei); ++offset)
			{
				data.push_back(fm77av.physMem.NonDestructiveFetchByte((addr.addr+offset)&(FM77AV_MAX_RAM_SIZE-1)));
			}
			for(auto str : miscutil::MakeDump(data.data(),addr.addr,wid,hei,skip,/*shiftJIS*/false,ascii))
			{
				std::cout << str << std::endl;
			}
		}
		else if(FM77AV::ADDR_MAIN==addr.type || FM77AV::ADDR_SUB==addr.type)
		{
			auto &cpu=fm77av.CPU(addr.type);
			auto &mem=fm77av.MemAccess(addr.type);
			for(auto str : miscutil::MakeMemDump2(cpu,mem,addr.addr,wid,hei,skip,/*shiftJIS*/false,ascii))
			{
				std::cout << str << std::endl;
			}
		}
		else if(FM77AV::ADDR_VRAM==addr.type || FM77AV::ADDR_VRAM_RAW==addr.type)
		{
			std::vector <unsigned char> data;
			for(uint32_t offset=0; offset<(wid*hei); ++offset)
			{
				uint32_t ptr=addr.addr+offset;
				uint8_t byte=0xFF;
				unsigned int bank=0xffffffff;
				if(ptr<fm77av.physMem.GetVRAMBankSize(0))
				{
					bank=0;
				}
				else if(ptr<fm77av.physMem.GetVRAMBankSize(0)+fm77av.physMem.GetVRAMBankSize(1))
				{
					bank=1;
					ptr-=fm77av.physMem.GetVRAMBankSize(0);
				}
				else if(ptr<fm77av.physMem.GetVRAMBankSize(0)+fm77av.physMem.GetVRAMBankSize(1)+fm77av.physMem.GetVRAMBankSize(2))
				{
					bank=2;
					ptr-=(fm77av.physMem.GetVRAMBankSize(0)+fm77av.physMem.GetVRAMBankSize(1));
				}
				auto VRAM=fm77av.physMem.GetVRAMBank(bank);
				if(nullptr!=VRAM)
				{
					if(FM77AV::ADDR_VRAM_RAW==addr.type)
					{
						ptr=fm77av.crtc.TransformVRAMAddress(ptr,fm77av.crtc.state.scrnMode,fm77av.crtc.state.VRAMOffset[bank]&fm77av.crtc.state.VRAMOffsetMask);
					}
					byte=VRAM[ptr&0xFFFF];
				}
				data.push_back(byte);
			}
			for(auto str : miscutil::MakeDump(data.data(),addr.addr,wid,hei,skip,/*shiftJIS*/false,ascii))
			{
				std::cout << str << std::endl;
			}
		}
		else
		{
			std::cout << "Address type not implemented yet." << std::endl;
		}
	}
}
void FM77AVCommandInterpreter::Execute_BreakOnOrMonitor(FM77AVThread &thr,FM77AV &av,Command &cmd,bool justMonitorDontBreak)
{
	if(2<=cmd.argv.size())
	{
		std::string verb=JustMonitorOrBreakString(justMonitorDontBreak);

		auto EVT=cmd.argv[1];
		cpputil::Capitalize(EVT);
		auto found=breakEventMap.find(EVT);
		if(breakEventMap.end()!=found)
		{
			switch(found->second)
			{
			case BREAK_ON_SUBCPU_UNHALT:
				av.var.monitorSubSysCmd=justMonitorDontBreak;
				av.var.breakOnUnhaltSubCPU=(true!=justMonitorDontBreak);
				std::cout << verb << " on Sub-CPU Unhalt." << std::endl;
				break;
			case BREAK_ON_SUBCMD:
				if(3<=cmd.argv.size())
				{
					auto num=cpputil::Xtoi(cmd.argv[2].data());
					if(num<FM7_MAX_SUB_CMD)
					{
						if(true!=justMonitorDontBreak)
						{
							av.var.breakOnSubCmd[num]=MC6809::Debugger::BRKPNT_FLAG_BREAK;
						}
						else
						{
							av.var.breakOnSubCmd[num]=MC6809::Debugger::BRKPNT_FLAG_MONITOR_ONLY;
						}
						std::cout << verb << " on Sub-CPU Command " << cpputil::Ubtox(num) << " " << SubCmdToStr(num) << std::endl;
					}
					else
					{
						Error_IllegalSubCommand(cmd);
					}
				}
				else
				{
					Error_TooFewArgs(cmd);
				}
				break;
			case BREAK_ON_MEM_READ:
				Execute_BreakOnMemoryRead(thr,av,cmd,justMonitorDontBreak);
				break;
			case BREAK_ON_MEM_WRITE:
				Execute_BreakOnMemoryWrite(thr,av,cmd,justMonitorDontBreak);
				break;
			case BREAK_ON_HARDWARE_VRAM_WRITE:
				Execute_BreakOnHardwareVRAMWrite(thr,av,cmd,justMonitorDontBreak);
				break;
			case BREAK_ON_HARDWARE_LINE_DRAWING:
				av.crtc.BreakOnHardwareLineDrawing(justMonitorDontBreak);
				break;
			}
		}
		else
		{
			Error_UnknownEvent(cmd);
		}
	}
	else
	{
		Error_TooFewArgs(cmd);
	}
}
void FM77AVCommandInterpreter::Execute_DontBreakOn(FM77AVThread &thr,FM77AV &av,Command &cmd)
{
	if(2<=cmd.argv.size())
	{
		auto EVT=cmd.argv[1];
		cpputil::Capitalize(EVT);
		auto found=breakEventMap.find(EVT);
		if(breakEventMap.end()!=found)
		{
			switch(found->second)
			{
			case BREAK_ON_SUBCPU_UNHALT:
				av.var.breakOnUnhaltSubCPU=false;
				std::cout << "Don't Break on Sub-CPU Unhalt." << std::endl;
				break;
			case BREAK_ON_SUBCMD:
				if(3<=cmd.argv.size())
				{
					auto num=cpputil::Xtoi(cmd.argv[2].data());
					if(num<FM7_MAX_SUB_CMD)
					{
						av.var.breakOnSubCmd[num]=0;
						std::cout << "Don't Break On Sub-CPU Command " << cpputil::Ubtox(num) << " " << SubCmdToStr(num) << std::endl;
					}
					else
					{
						Error_IllegalSubCommand(cmd);
					}
				}
				else
				{
					for(auto &b : av.var.breakOnSubCmd)
					{
						b=0;
					}
				}
				break;
			case BREAK_ON_MEM_READ:
				Execute_DontBreakOnMemoryRead(thr,av,cmd);
				break;
			case BREAK_ON_MEM_WRITE:
				Execute_DontBreakOnMemoryWrite(thr,av,cmd);
				break;
			case BREAK_ON_HARDWARE_VRAM_WRITE:
				Execute_DontBreakOnHardwareVRAMWrite(thr,av,cmd);
				break;
			case BREAK_ON_HARDWARE_LINE_DRAWING:
				av.crtc.ClearBreakOnHardwareLineDrawing();
				break;
			}
		}
		else
		{
			Error_UnknownEvent(cmd);
		}
	}
	else
	{
		Error_TooFewArgs(cmd);
	}
}
void FM77AVCommandInterpreter::Execute_BreakOnMemoryRead(FM77AVThread &thr,FM77AV &av,Command &cmd,bool justMonitorDontBreak)
{
	bool useValue=false,useMinMax=false;
	unsigned char value=0,minValue=0,maxValue=255;
	int nAddr=0;
	int mainOrSub=CPU_UNKNOWN;
	uint32_t addr[2];

	uint8_t debuggerFlag=(justMonitorDontBreak ? MC6809::Debugger::BRKPNT_FLAG_MONITOR_ONLY : MC6809::Debugger::BRKPNT_FLAG_BREAK);

	for(int i=2; i<cmd.argv.size(); ++i)
	{
		// DATA=, DATA:, D:, D=, VALUE:, VALUE=, V:, V=
		auto arg=cmd.argv[i];
		cpputil::Capitalize(arg);
		if(cpputil::StrStartsWith(arg,"DATA=") ||
		   cpputil::StrStartsWith(arg,"D=") ||
		   cpputil::StrStartsWith(arg,"VALUE=") ||
		   cpputil::StrStartsWith(arg,"V=") ||
		   cpputil::StrStartsWith(arg,"DATA:") ||
		   cpputil::StrStartsWith(arg,"D:") ||
		   cpputil::StrStartsWith(arg,"VALUE:") ||
		   cpputil::StrStartsWith(arg,"V:"))
		{
			auto pos=arg.find(':');
			if(std::string::npos==pos)
			{
				pos=arg.find('=');
			}
			if(std::string::npos!=pos)
			{
				useValue=true;
				value=cpputil::Xtoi(arg.c_str()+pos+1);
			}
		}
		else if(cpputil::StrStartsWith(arg,"MIN=") ||
		        cpputil::StrStartsWith(arg,"Min=") ||
		        cpputil::StrStartsWith(arg,"min=") ||
		        cpputil::StrStartsWith(arg,"MIN:") ||
		        cpputil::StrStartsWith(arg,"Min:") ||
		        cpputil::StrStartsWith(arg,"min:"))
		{
			auto pos=arg.find(':');
			if(std::string::npos==pos)
			{
				pos=arg.find('=');
			}
			if(std::string::npos!=pos)
			{
				useMinMax=true;
				minValue=cpputil::Xtoi(arg.c_str()+pos+1);
			}
		}
		else if(cpputil::StrStartsWith(arg,"MAX=") ||
		        cpputil::StrStartsWith(arg,"Max=") ||
		        cpputil::StrStartsWith(arg,"max=") ||
		        cpputil::StrStartsWith(arg,"MAX:") ||
		        cpputil::StrStartsWith(arg,"Max:") ||
		        cpputil::StrStartsWith(arg,"max:"))
		{
			auto pos=arg.find(':');
			if(std::string::npos==pos)
			{
				pos=arg.find('=');
			}
			if(std::string::npos!=pos)
			{
				useMinMax=true;
				maxValue=cpputil::Xtoi(arg.c_str()+pos+1);
			}
		}
		else if(0==nAddr)
		{
			auto ptr=DecodeAddress(av,cmd.argv[i],thr.OnlyOneCPUIsUnmuted(),thr.OnlyOneCPUIsUnmuted());
			if(CPU_UNKNOWN==ptr.type)
			{
				Error_UnknownCPU(cmd);
				return;
			}

			mainOrSub=ptr.type;
			addr[nAddr]=ptr.addr;
			++nAddr;
		}
		else if(nAddr<2)
		{
			addr[nAddr]=cpputil::Xtoi(arg.c_str());
			++nAddr;
		}
		else
		{
			Error_WrongParameter(cmd);
			return;
		}
	}

	if(useMinMax && useValue)
	{
		std::cout << "Min/Max and Data cannot be used simultaneously." << std::endl;
		Error_WrongParameter(cmd);
		return;
	}

	if(CPU_UNKNOWN==mainOrSub)
	{
		Error_UnknownCPU(cmd);
		return;
	}

	if(1==nAddr)
	{
		addr[1]=addr[0];
		nAddr=2;
	}

	if(2==nAddr)
	{
		std::string verb=JustMonitorOrBreakString(justMonitorDontBreak);

		if(addr[1]<addr[0])
		{
			std::swap(addr[0],addr[1]);
		}
		if(FM77AV::ADDR_PHYS==mainOrSub)
		{
			for(auto a=addr[0]; a<=addr[1]; ++a)
			{
				if(a<PhysicalMemory::PHYSMEM_SIZE)
				{
					av.physMem.var.memAttr[a].brkOnRead=true;
					av.physMem.var.memAttr[a].justMonitorDontBreakOnRead=justMonitorDontBreak;
					if(true!=useValue)
					{
						av.physMem.var.memAttr[a].brkOnReadMinMax[0]=minValue;
						av.physMem.var.memAttr[a].brkOnReadMinMax[1]=maxValue;
					}
					else
					{
						av.physMem.var.memAttr[a].brkOnReadMinMax[0]=value;
						av.physMem.var.memAttr[a].brkOnReadMinMax[1]=value;
					}
				}
			}
			std::cout << verb << " on Memory Read" << std::endl;
			std::cout << "  from " << "PHYS:" << cpputil::Uitox(addr[0]) << std::endl;
			std::cout << "  to   " << "PHYS:" << cpputil::Uitox(addr[1]) << std::endl;
		}
		else if(FM77AV::ADDR_MAIN==mainOrSub || FM77AV::ADDR_SUB==mainOrSub)
		{
			auto &cpu=av.CPU(mainOrSub);
			auto &mem=av.MemAccess(mainOrSub);
			for(auto a=addr[0]; a<=addr[1]; ++a)
			{
				if(true!=useValue)
				{
					cpu.debugger.SetBreakOnMemRead(a,minValue,maxValue,debuggerFlag);
				}
				else
				{
					cpu.debugger.SetBreakOnMemRead(a,value,value,debuggerFlag);
				}
			}
			std::cout << verb << " on Memory Read" << std::endl;
			std::cout << "  from " << CPUToStr(mainOrSub) << ":" << cpputil::Uitox(addr[0]) << std::endl;
			std::cout << "  to   " << CPUToStr(mainOrSub) << ":" << cpputil::Uitox(addr[1]) << std::endl;
			if(true==useValue)
			{
				std::cout << "  Value=    " << cpputil::Ubtox(value) << std::endl;
			}
			if(true==useMinMax)
			{
				std::cout << "  Min=    " << cpputil::Ubtox(minValue) << std::endl;
				std::cout << "  Max=    " << cpputil::Ubtox(maxValue) << std::endl;
			}
		}
	}
	else if(2<nAddr)
	{
		Error_WrongParameter(cmd);
	}
	else
	{
		Error_TooFewArgs(cmd);
	}
}
void FM77AVCommandInterpreter::Execute_BreakOnMemoryWrite(FM77AVThread &thr,FM77AV &av,Command &cmd,bool justMonitorDontBreak)
{
	bool useValue=false,useMinMax=false;
	unsigned char value=0,minValue=0,maxValue=255;
	int nAddr=0;
	int mainOrSub=CPU_UNKNOWN;
	uint32_t addr[2];

	uint8_t debuggerFlag=(justMonitorDontBreak ? MC6809::Debugger::BRKPNT_FLAG_MONITOR_ONLY : MC6809::Debugger::BRKPNT_FLAG_BREAK);

	for(int i=2; i<cmd.argv.size(); ++i)
	{
		// DATA=, DATA:, D:, D=, VALUE:, VALUE=, V:, V=
		auto arg=cmd.argv[i];
		cpputil::Capitalize(arg);
		if(cpputil::StrStartsWith(arg,"DATA=") ||
		   cpputil::StrStartsWith(arg,"D=") ||
		   cpputil::StrStartsWith(arg,"VALUE=") ||
		   cpputil::StrStartsWith(arg,"V=") ||
		   cpputil::StrStartsWith(arg,"DATA:") ||
		   cpputil::StrStartsWith(arg,"D:") ||
		   cpputil::StrStartsWith(arg,"VALUE:") ||
		   cpputil::StrStartsWith(arg,"V:"))
		{
			auto pos=arg.find(':');
			if(std::string::npos==pos)
			{
				pos=arg.find('=');
			}
			if(std::string::npos!=pos)
			{
				useValue=true;
				value=cpputil::Xtoi(arg.c_str()+pos+1);
			}
		}
		else if(cpputil::StrStartsWith(arg,"MIN=") ||
		        cpputil::StrStartsWith(arg,"Min=") ||
		        cpputil::StrStartsWith(arg,"min=") ||
		        cpputil::StrStartsWith(arg,"MIN:") ||
		        cpputil::StrStartsWith(arg,"Min:") ||
		        cpputil::StrStartsWith(arg,"min:"))
		{
			auto pos=arg.find(':');
			if(std::string::npos==pos)
			{
				pos=arg.find('=');
			}
			if(std::string::npos!=pos)
			{
				useMinMax=true;
				minValue=cpputil::Xtoi(arg.c_str()+pos+1);
			}
		}
		else if(cpputil::StrStartsWith(arg,"MAX=") ||
		        cpputil::StrStartsWith(arg,"Max=") ||
		        cpputil::StrStartsWith(arg,"max=") ||
		        cpputil::StrStartsWith(arg,"MAX:") ||
		        cpputil::StrStartsWith(arg,"Max:") ||
		        cpputil::StrStartsWith(arg,"max:"))
		{
			auto pos=arg.find(':');
			if(std::string::npos==pos)
			{
				pos=arg.find('=');
			}
			if(std::string::npos!=pos)
			{
				useMinMax=true;
				maxValue=cpputil::Xtoi(arg.c_str()+pos+1);
			}
		}
		else if(0==nAddr)
		{
			auto ptr=DecodeAddress(av,cmd.argv[i],thr.OnlyOneCPUIsUnmuted(),thr.OnlyOneCPUIsUnmuted());
			if(CPU_UNKNOWN==ptr.type)
			{
				Error_UnknownCPU(cmd);
				return;
			}

			mainOrSub=ptr.type;
			addr[nAddr]=ptr.addr;
			++nAddr;
		}
		else if(nAddr<2)
		{
			addr[nAddr]=cpputil::Xtoi(arg.c_str());
			++nAddr;
		}
		else
		{
			Error_WrongParameter(cmd);
			return;
		}
	}

	if(useMinMax && useValue)
	{
		std::cout << "Min/Max and Data cannot be used simultaneously." << std::endl;
		Error_WrongParameter(cmd);
		return;
	}

	if(CPU_UNKNOWN==mainOrSub)
	{
		Error_UnknownCPU(cmd);
		return;
	}

	if(1==nAddr)
	{
		addr[1]=addr[0];
		nAddr=2;
	}

	if(2==nAddr)
	{
		std::string verb=JustMonitorOrBreakString(justMonitorDontBreak);

		if(addr[1]<addr[0])
		{
			std::swap(addr[0],addr[1]);
		}
		if(FM77AV::ADDR_PHYS==mainOrSub)
		{
			for(auto a=addr[0]; a<=addr[1]; ++a)
			{
				if(a<PhysicalMemory::PHYSMEM_SIZE)
				{
					av.physMem.var.memAttr[a].brkOnWrite=true;
					av.physMem.var.memAttr[a].justMonitorDontBreakOnWrite=justMonitorDontBreak;
					if(true!=useValue)
					{
						av.physMem.var.memAttr[a].brkOnWriteMinMax[0]=minValue;
						av.physMem.var.memAttr[a].brkOnWriteMinMax[1]=maxValue;
					}
					else
					{
						av.physMem.var.memAttr[a].brkOnWriteMinMax[0]=value;
						av.physMem.var.memAttr[a].brkOnWriteMinMax[1]=value;
					}
				}
			}
			std::cout << verb << " on Memory Write" << std::endl;
			std::cout << "  from " << "PHYS:" << cpputil::Uitox(addr[0]) << std::endl;
			std::cout << "  to   " << "PHYS:" << cpputil::Uitox(addr[1]) << std::endl;
		}
		else if(FM77AV::ADDR_MAIN==mainOrSub || FM77AV::ADDR_SUB==mainOrSub)
		{
			auto &cpu=av.CPU(mainOrSub);
			auto &mem=av.MemAccess(mainOrSub);
			for(auto a=addr[0]; a<=addr[1]; ++a)
			{
				if(true!=useValue)
				{
					cpu.debugger.SetBreakOnMemWrite(a,minValue,maxValue,debuggerFlag);
				}
				else
				{
					cpu.debugger.SetBreakOnMemWrite(a,value,value,debuggerFlag);
				}
			}
			std::cout << verb << " on Memory Write" << std::endl;
			std::cout << "  from " << CPUToStr(mainOrSub) << ":" << cpputil::Uitox(addr[0]) << std::endl;
			std::cout << "  to   " << CPUToStr(mainOrSub) << ":" << cpputil::Uitox(addr[1]) << std::endl;
		}
		if(true==useValue)
		{
			std::cout << "  Value=    " << cpputil::Ubtox(value) << std::endl;
		}
		if(true==useMinMax)
		{
			std::cout << "  Min=    " << cpputil::Ubtox(minValue) << std::endl;
			std::cout << "  Max=    " << cpputil::Ubtox(maxValue) << std::endl;
		}
	}
	else if(2<nAddr)
	{
		Error_WrongParameter(cmd);
	}
	else
	{
		Error_TooFewArgs(cmd);
	}
}
void FM77AVCommandInterpreter::Execute_DontBreakOnMemoryRead(FM77AVThread &thr,FM77AV &av,Command &cmd)
{
	if(4<=cmd.argv.size())
	{
		auto ptr0=MakeCPUandAddress(thr,av,cmd.argv[2]);
		unsigned int addr0=ptr0.addr;
		unsigned int addr1=cpputil::Xtoi(cmd.argv[3].c_str());
		if(CPU_UNKNOWN==ptr0.cpu)
		{
			Error_UnknownCPU(cmd);
			return;
		}
		if(addr1<addr0)
		{
			std::swap(addr0,addr1);
		}
		auto &cpu=av.CPU(ptr0.cpu);
		for(auto addr=addr0; addr<=addr1; ++addr)
		{
			cpu.debugger.ClearBreakOnMemRead(addr);
		}
		std::cout << "Clear Break on Memory Read" << std::endl;
		std::cout << "  from " << CPUToStr(ptr0.cpu) << ":" << cpputil::Uitox(addr0) << std::endl;
		std::cout << "  to   " << CPUToStr(ptr0.cpu) << ":" << cpputil::Uitox(addr1) << std::endl;
	}
	else if(3<=cmd.argv.size())
	{
		auto ptr0=MakeCPUandAddress(thr,av,cmd.argv[2]);
		if(CPU_UNKNOWN==ptr0.cpu)
		{
			Error_UnknownCPU(cmd);
			return;
		}
		auto &cpu=av.CPU(ptr0.cpu);
		cpu.debugger.ClearBreakOnMemRead(ptr0.addr);
		std::cout << "Clear Break on Memory Read " << CPUToStr(ptr0.cpu) << ":" << cpputil::Uitox(ptr0.addr) << std::endl;
	}
	else
	{
		for(uint32_t addr=0; addr<=0xFFFF; ++addr)
		{
			av.mainCPU.debugger.ClearBreakOnMemRead(addr);
			av.subCPU.debugger.ClearBreakOnMemRead(addr);
		}
		std::cout << "Clear All Break on Memory Read on main- and sub-CPUs" << std::endl;
	}
}
void FM77AVCommandInterpreter::Execute_DontBreakOnMemoryWrite(FM77AVThread &thr,FM77AV &av,Command &cmd)
{
	if(4<=cmd.argv.size())
	{
		auto ptr0=DecodeAddress(av,cmd.argv[2],thr.OnlyOneCPUIsUnmuted(),thr.OnlyOneCPUIsUnmuted());
		unsigned int addr0=ptr0.addr;
		unsigned int addr1=cpputil::Xtoi(cmd.argv[3].c_str());
		if(CPU_UNKNOWN==ptr0.type)
		{
			Error_UnknownCPU(cmd);
			return;
		}
		if(addr1<addr0)
		{
			std::swap(addr0,addr1);
		}
		if(FM77AV::ADDR_PHYS==ptr0.type)
		{
			for(auto addr=addr0; addr<=addr1 && addr<PhysicalMemory::PHYSMEM_SIZE; ++addr)
			{
				av.physMem.var.memAttr[addr].brkOnWrite=false;
			}
			std::cout << "Clear Break on Memory Write" << std::endl;
			std::cout << "  from " << "PHYS:" << cpputil::Uitox(addr0) << std::endl;
			std::cout << "  to   " << "PHYS:" << cpputil::Uitox(addr1) << std::endl;
		}
		else
		{
			auto &cpu=av.CPU(ptr0.type);
			for(auto addr=addr0; addr<=addr1; ++addr)
			{
				cpu.debugger.ClearBreakOnMemWrite(addr);
			}
			std::cout << "Clear Break on Memory Write" << std::endl;
			std::cout << "  from " << CPUToStr(ptr0.type) << ":" << cpputil::Uitox(addr0) << std::endl;
			std::cout << "  to   " << CPUToStr(ptr0.type) << ":" << cpputil::Uitox(addr1) << std::endl;
		}
	}
	else if(3<=cmd.argv.size())
	{
		auto ptr0=DecodeAddress(av,cmd.argv[2],thr.OnlyOneCPUIsUnmuted(),thr.OnlyOneCPUIsUnmuted());
		if(CPU_UNKNOWN==ptr0.type)
		{
			Error_UnknownCPU(cmd);
			return;
		}
		if(FM77AV::ADDR_PHYS==ptr0.type)
		{
			if(ptr0.addr<PhysicalMemory::PHYSMEM_SIZE)
			{
				av.physMem.var.memAttr[ptr0.addr].brkOnWrite=false;
			}
			std::cout << "Clear Break on Memory Write " << "PHYS:" << cpputil::Uitox(ptr0.addr) << std::endl;
		}
		else
		{
			auto &cpu=av.CPU(ptr0.type);
			cpu.debugger.ClearBreakOnMemWrite(ptr0.addr);
			std::cout << "Clear Break on Memory Write " << CPUToStr(ptr0.type) << ":" << cpputil::Uitox(ptr0.addr) << std::endl;
		}
	}
	else
	{
		for(uint32_t addr=0; addr<=0xFFFF; ++addr)
		{
			av.mainCPU.debugger.ClearBreakOnMemWrite(addr);
			av.subCPU.debugger.ClearBreakOnMemWrite(addr);
		}
		for(uint32_t addr=0; addr<PhysicalMemory::PHYSMEM_SIZE; ++addr)
		{
			av.physMem.var.memAttr[addr].brkOnWrite=false;
		}
		std::cout << "Clear All Break on Memory Write on main- and sub-CPUs" << std::endl;
	}
}

void FM77AVCommandInterpreter::Execute_AddBreakPoint(FM77AVThread &thr,FM77AV &fm77av,Command &cmd)
{
	if(cmd.argv.size()<2)
	{
		Error_TooFewArgs(cmd);
		return;
	}

	auto ptr=MakeCPUandAddress(thr,fm77av,cmd.argv[1]);
	if(CPU_UNKNOWN==ptr.cpu)
	{
		Error_UnknownCPU(cmd);
		return;
	}

	auto &cpu=fm77av.CPU(ptr.cpu);
	cpu.debugger.SetBreakPoint(ptr.addr,ptr.addr);
}

void FM77AVCommandInterpreter::Execute_AddBreakPointWithPassCount(FM77AVThread &thr,FM77AV &fm77av,Command &cmd)
{
	if(cmd.argv.size()<3)
	{
		Error_TooFewArgs(cmd);
		return;
	}

	auto ptr=MakeCPUandAddress(thr,fm77av,cmd.argv[1]);
	if(CPU_UNKNOWN==ptr.cpu)
	{
		Error_UnknownCPU(cmd);
		return;
	}

	int passCountUntilBreak=cpputil::Atoi(cmd.argv[2].c_str());
	auto &cpu=fm77av.CPU(ptr.cpu);
	cpu.debugger.SetBreakPoint(ptr.addr,ptr.addr,passCountUntilBreak);
}

void FM77AVCommandInterpreter::Execute_AddMonitorPoint(FM77AVThread &thr,FM77AV &fm77av,Command &cmd)
{
	if(cmd.argv.size()<2)
	{
		Error_TooFewArgs(cmd);
		return;
	}

	if(cmd.argv.size()<2)
	{
		Error_TooFewArgs(cmd);
		return;
	}

	auto ptr=MakeCPUandAddress(thr,fm77av,cmd.argv[1]);
	if(CPU_UNKNOWN==ptr.cpu)
	{
		Error_UnknownCPU(cmd);
		return;
	}

	auto &cpu=fm77av.CPU(ptr.cpu);
	cpu.debugger.SetBreakPoint(ptr.addr,ptr.addr,1,MC6809::Debugger::BRKPNT_FLAG_MONITOR_ONLY);
}

void FM77AVCommandInterpreter::Execute_DeleteBreakPoint(FM77AVThread &thr,FM77AV &fm77av,Command &cmd)
{
	if(cmd.argv.size()<2)
	{
		Error_TooFewArgs(cmd);
		return;
	}

	auto ARGV1=cmd.argv[1];
	cpputil::Capitalize(ARGV1);

	if("*"==cmd.argv[1])
	{
		fm77av.mainCPU.debugger.ClearBreakPoint();
		fm77av.subCPU.debugger.ClearBreakPoint();
		std::cout << "Cleared all break points of main- and sub-CPUs." << std::endl;
	}
	else if("M:*"==ARGV1 || "MAIN:*"==ARGV1)
	{
		fm77av.mainCPU.debugger.ClearBreakPoint();
		std::cout << "Cleared all break points of main-CPU." << std::endl;
	}
	else if("S:*"==ARGV1 || "SUB:*"==ARGV1)
	{
		fm77av.subCPU.debugger.ClearBreakPoint();
		std::cout << "Cleared all break points of sub-CPU." << std::endl;
	}
	else
	{
		auto ptr=MakeCPUandAddress(thr,fm77av,cmd.argv[1]);
		if(CPU_UNKNOWN==ptr.cpu)
		{
			Error_UnknownCPU(cmd);
			return;
		}

		auto &cpu=fm77av.CPU(ptr.cpu);
		cpu.debugger.ClearBreakPoint(ptr.addr,ptr.addr);
		std::cout << "Cleared break point." << std::endl;
	}
}
void FM77AVCommandInterpreter::Execute_ListBreakPoints(FM77AVThread &thr,FM77AV &fm77av,Command &cmd)
{
	if(cmd.argv.size()<2)
	{
		Error_TooFewArgs(cmd);
		return;
	}

	auto mainOrSub=StrToCPU(cmd.argv[1]);
	if(CPU_UNKNOWN==mainOrSub)
	{
		mainOrSub=thr.OnlyOneCPUIsUnmuted();
	}
	if(CPU_UNKNOWN==mainOrSub)
	{
		Error_UnknownCPU(cmd);
		return;
	}

	auto &cpu=fm77av.CPU(mainOrSub);
	for(uint32_t addr=0; addr<0x10000; ++addr)
	{
		if(0!=(cpu.debugger.breakPoints[addr].flags))
		{
			std::cout << cpputil::Ustox(addr);
			if(0!=(MC6809::Debugger::BRKPNT_FLAG_MONITOR_ONLY&cpu.debugger.breakPoints[addr].flags))
			{
				std::cout << " M";
			}
			else
			{
				std::cout << "  ";
			}
			if(0!=cpu.debugger.breakPoints[addr].passCount)
			{
				std::cout << " " << cpu.debugger.breakPoints[addr].passed << "/" << cpu.debugger.breakPoints[addr].passCount;
			}
			std::cout << std::endl;
		}
	}
}

void FM77AVCommandInterpreter::Execute_BreakOnHardwareVRAMWrite(FM77AVThread &thr,FM77AV &av,Command &cmd,bool justMonitorDontBreak)
{
	std::string verb=JustMonitorOrBreakString(justMonitorDontBreak);

	if(cmd.argv.size()<=2)
	{
		av.crtc.AddBreakOnHardwareVRAMWriteType(0,justMonitorDontBreak);
		av.crtc.AddBreakOnHardwareVRAMWriteType(1,justMonitorDontBreak);
		av.crtc.AddBreakOnHardwareVRAMWriteType(2,justMonitorDontBreak);
		av.crtc.AddBreakOnHardwareVRAMWriteType(3,justMonitorDontBreak);
		av.crtc.AddBreakOnHardwareVRAMWriteType(4,justMonitorDontBreak);
		av.crtc.AddBreakOnHardwareVRAMWriteType(5,justMonitorDontBreak);
		av.crtc.AddBreakOnHardwareVRAMWriteType(6,justMonitorDontBreak);
		av.crtc.AddBreakOnHardwareVRAMWriteType(7,justMonitorDontBreak);
		std::cout << verb << " on all hardware VRAM write (logic op)" << std::endl;
	}
	else if(3<=cmd.argv.size())
	{
		for(int i=2; i<cmd.argv.size(); ++i)
		{
			auto t=cpputil::Xtoi(cmd.argv[i].c_str());
			av.crtc.AddBreakOnHardwareVRAMWriteType(t,justMonitorDontBreak);
			std::cout << "Break on all hardware VRAM write (logic op) Type=" << cpputil::Ubtox(t) << std::endl;
		}
	}
}
void FM77AVCommandInterpreter::Execute_DontBreakOnHardwareVRAMWrite(FM77AVThread &thr,FM77AV &av,Command &cmd)
{
	if(cmd.argv.size()<=2)
	{
		av.crtc.ClearBreakOnHardwareVRAMWriteType(0);
		av.crtc.ClearBreakOnHardwareVRAMWriteType(1);
		av.crtc.ClearBreakOnHardwareVRAMWriteType(2);
		av.crtc.ClearBreakOnHardwareVRAMWriteType(3);
		av.crtc.ClearBreakOnHardwareVRAMWriteType(4);
		av.crtc.ClearBreakOnHardwareVRAMWriteType(5);
		av.crtc.ClearBreakOnHardwareVRAMWriteType(6);
		av.crtc.ClearBreakOnHardwareVRAMWriteType(7);
		std::cout << "Do not Break on all hardware VRAM write (logic op)" << std::endl;
	}
	else if(3<=cmd.argv.size())
	{
		for(int i=2; i<cmd.argv.size(); ++i)
		{
			auto t=cpputil::Xtoi(cmd.argv[i].c_str());
			av.crtc.ClearBreakOnHardwareVRAMWriteType(t);
			std::cout << "Do Not Break on all hardware VRAM write (logic op) Type=" << cpputil::Ubtox(t) << std::endl;
		}
	}
}

void FM77AVCommandInterpreter::Execute_FDLoad(int drv,FM77AV &fm77av,Command &cmd)
{
	if(2<=cmd.argv.size())
	{
		auto fileName=fm77av.FileNameAlias(cmd.argv[1]);
		if(true==fm77av.fdc.LoadD77orRAW(drv,fileName.c_str(),false))
		{
			std::cout << "Loaded FD image." << std::endl;
		}
		else
		{
			std::cout << "Failed to load FD image." << std::endl;
		}
	}
	else
	{
		Error_TooFewArgs(cmd);
	}
}
void FM77AVCommandInterpreter::Execute_FDEject(int drv,FM77AV &fm77av,Command &cmd)
{
	fm77av.fdc.Eject(drv);
	std::cout << "Ejected Floppy Drive " << drv << std::endl;
}
void FM77AVCommandInterpreter::Execute_SaveScreenShot(FM77AV &fm77av,Command &cmd)
{
	if(2<=cmd.argv.size())
	{
		FM77AVRender render;
		fm77av.RenderQuiet(render);

		auto img=render.GetImage();

		YsRawPngEncoder encoder;
		if(YSOK==encoder.EncodeToFile(cmd.argv[1].c_str(),img.wid,img.hei,8,6,img.rgba))
		{
			std::cout << "Saved to " << cmd.argv[1] << std::endl;
		}
		else
		{
			std::cout << "Save error." << std::endl;
		}
	}
	else
	{
		Error_TooFewArgs(cmd);
	}
}
void FM77AVCommandInterpreter::Execute_QuickScreenShot(FM77AV &fm77av,Command &cmd)
{
	auto now=time(nullptr);
	auto tm=localtime(&now);

	auto year=tm->tm_year+1900;
	auto month=tm->tm_mon+1;
	auto date=tm->tm_mday;
	auto hour=tm->tm_hour;
	auto min=tm->tm_min;
	auto sec=tm->tm_sec;

	std::string ful;
	for(int count=0; count<100; ++count)
	{
		char fmt[256];
		sprintf(fmt,"%04d%02d%02d%02d%02d%02d%02d.png",year,month,date,hour,min,sec,count);

		if(""!=fm77av.var.quickScrnShotDir)
		{
			ful=cpputil::MakeFullPathName(fm77av.var.quickScrnShotDir,fmt);
		}
		else
		{
			ful=fmt;
		}

		if(true!=cpputil::FileExists(ful))
		{
			break;
		}
	}

	FM77AVRender render;
	fm77av.RenderQuiet(render);

	auto img=render.GetImage();

	YsRawPngEncoder encoder;
	if(YSOK==encoder.EncodeToFile(ful.c_str(),img.wid,img.hei,8,6,img.rgba))
	{
		std::cout << "Saved to " << ful << std::endl;
	}
	else
	{
		std::cout << "Save error." << std::endl;
	}
}
void FM77AVCommandInterpreter::Execute_QuickScreenShotDirectory(FM77AV &fm77av,Command &cmd)
{
	if(2<=cmd.argv.size())
	{
		fm77av.var.quickScrnShotDir=cmd.argv[1];
	}
	else
	{
		Error_TooFewArgs(cmd);
	}
}
void FM77AVCommandInterpreter::Execute_EnableCallStack(FM77AVThread &thr,FM77AV &fm77av,Command &cmd)
{
	if(cmd.argv.size()<=2) // Just ENA CST, no CPU specification.
	{
		auto mainOrSub=thr.OnlyOneCPUIsUnmuted();
		if(CPU_UNKNOWN==mainOrSub)
		{
			fm77av.mainCPU.debugger.enableCallStack=true;
			fm77av.subCPU.debugger.enableCallStack=true;
			std::cout << "Enabled Call Stack for Main and Sub CPUs" << std::endl;
		}
		else
		{
			fm77av.CPU(mainOrSub).debugger.enableCallStack=true;
			std::cout << "Enable Call Stack for " << CPUToStr(mainOrSub) << " CPU" << std::endl;
		}
	}
	else
	{
		auto mainOrSub=StrToCPU(cmd.argv[2]);
		if(CPU_UNKNOWN==mainOrSub)
		{
			Error_UnknownCPU(cmd);
		}
		else
		{
			fm77av.CPU(mainOrSub).debugger.enableCallStack=true;
			std::cout << "Enable Call Stack for " << CPUToStr(mainOrSub) << " CPU" << std::endl;
		}
	}
}
void FM77AVCommandInterpreter::Execute_DisableCallStack(FM77AVThread &thr,FM77AV &fm77av,Command &cmd)
{
	if(cmd.argv.size()<=2) // Just ENA CST, no CPU specification.
	{
		std::cout << "Disabled Call Stack for Main and Sub CPUs." << std::endl;
		fm77av.mainCPU.debugger.enableCallStack=false;
		fm77av.subCPU.debugger.enableCallStack=false;
		fm77av.mainCPU.debugger.ClearCallStack();
		fm77av.subCPU.debugger.ClearCallStack();
	}
	else
	{
		auto mainOrSub=StrToCPU(cmd.argv[2]);
		if(CPU_UNKNOWN==mainOrSub)
		{
			Error_UnknownCPU(cmd);
		}
		else
		{
			fm77av.CPU(mainOrSub).debugger.enableCallStack=false;
			fm77av.CPU(mainOrSub).debugger.ClearCallStack();
			std::cout << "Disabled Call Stack for " << CPUToStr(mainOrSub) << " CPU" << std::endl;
		}
	}
}
void FM77AVCommandInterpreter::Execute_PrintCallStack(FM77AVThread &thr,FM77AV &fm77av,Command &cmd)
{
	int mainOrSub=CPU_UNKNOWN;
	if(cmd.argv.size()<=2) // Just ENA CST, no CPU specification.
	{
		mainOrSub=thr.OnlyOneCPUIsUnmuted();
	}
	else
	{
		mainOrSub=StrToCPU(cmd.argv[2]);
	}

	if(CPU_UNKNOWN==mainOrSub)
	{
		Error_UnknownCPU(cmd);
		return;
	}
	if(true!=fm77av.CPU(mainOrSub).debugger.enableCallStack)
	{
		std::cout << "Call Stack Not Enabled for " << CPUToStr(mainOrSub) << " CPU" << std::endl;
		return;
	}

	std::cout << "Call Stack for " << CPUToStr(mainOrSub) << " CPU" << std::endl;
	for(auto s : fm77av.CPU(mainOrSub).debugger.GetCallStackText())
	{
		std::cout << s << std::endl;
	}
}
void FM77AVCommandInterpreter::Execute_TypeKeyboard(FM77AV &fm77av,Command &cmd)
{
	bool start=false;
	for(int i=0; i<cmd.cmdline.size(); ++i)
	{
		if(true==start)
		{
			fm77av.keyboard.Type(cmd.cmdline[i]);
		}
		else if(' '==cmd.cmdline[i] || '\t'==cmd.cmdline[i])
		{
			start=true;
		}
	}
	if(true==start)
	{
		fm77av.keyboard.Type(0x0D);
	}
}
void FM77AVCommandInterpreter::Execute_SaveCOM0Out(FM77AV &fm77av,Command &cmd)
{
	if(2<=cmd.argv.size())
	{
		if(true==cpputil::WriteBinaryFile(cmd.argv[1],fm77av.serialport.cli0.fromVM.size(),fm77av.serialport.cli0.fromVM.data()))
		{
			std::cout << "Saved COM0 log." << std::endl;
		}
		else
		{
			Error_CannotSaveFile(cmd);
		}
	}
	else
	{
		Error_TooFewArgs(cmd);
	}
}
void FM77AVCommandInterpreter::Execute_ClearCom0Out(FM77AV &fm77av,Command &cmd)
{
	std::cout << "Cleared COM0 log" << std::endl;
	fm77av.serialport.cli0.fromVM.clear();
}

void FM77AVCommandInterpreter::Execute_Let(FM77AVThread &thr,FM77AV &fm77av,Command &cmd)
{
	// Can be LET M:A=4F or LET M:A 4F
	std::string cpuStr,regStr,valueStr;
	if(2<=cmd.argv.size())
	{
		for(int i=0; i<cmd.argv[1].size(); ++i)
		{
			if(cmd.argv[1][i]==':')
			{
				cpuStr=cmd.argv[1].substr(0,i);
				regStr="";
			}
			else if(cmd.argv[1][i]=='=')
			{
				valueStr=cmd.argv[1].substr(i+1);
				break;
			}
			else
			{
				regStr.push_back(cmd.argv[1][i]);
			}
		}

		if(0==valueStr.size())
		{
			if(3<=cmd.argv.size())
			{
				valueStr=cmd.argv[2];
			}
			else
			{
				Error_TooFewArgs(cmd);
				return;
			}
		}

		unsigned int cpu=thr.OnlyOneCPUIsUnmuted();
		if(0!=cpuStr.size())
		{
			cpu=StrToCPU(cpuStr);
		}
		if(CPU_UNKNOWN==cpu)
		{
			Error_UnknownCPU(cmd);
			return;
		}

		auto reg=MC6809::StrToReg(regStr);
		if(MC6809::REG_NULL==reg)
		{
			Error_UnknownRegister(cmd);
			return;
		}

		fm77av.CPU(cpu).SetRegisterValue(reg,cpputil::Xtoi(valueStr.c_str()));
		std::cout << "Register " << regStr << "=" << valueStr << std::endl;
	}
	else
	{
		Error_TooFewArgs(cmd);
	}
}
void FM77AVCommandInterpreter::Execute_SetSpeed(FM77AVThread &thr,FM77AV &fm77av,Command &cmd)
{
	if(2<=cmd.argv.size())
	{
		auto STR=cmd.argv[1];
		cpputil::Capitalize(STR);
		if("FM8"==STR || "FM-8"==STR)
		{
			fm77av.SetFM8Speed();
		}
		else if("FM7"==STR || "FM-7"==STR)
		{
			fm77av.SetFM7Speed();
		}
		else if("FM77"==STR || "FM-77"==STR)
		{
			fm77av.SetFM77Speed();
		}
		else
		{
			std::cout << "Speed can be one of FM8, FM7, or FM77" << std::endl;
		}
	}
	else
	{
		Error_TooFewArgs(cmd);
	}
}
void FM77AVCommandInterpreter::Execute_LoadTape(FM77AVThread &thr,FM77AV &fm77av,Command &cmd)
{
	if(2<=cmd.argv.size())
	{
		auto fileName=fm77av.FileNameAlias(cmd.argv[1]);
		if(true==fm77av.dataRecorder.LoadT77(fileName))
		{
			std::cout << "Loaded Tape image." << std::endl;
		}
		else
		{
			std::cout << "Failed to load Tape image." << std::endl;
			Error_CannotOpenFile(cmd);
		}
	}
	else
	{
		Error_TooFewArgs(cmd);
	}
}
void FM77AVCommandInterpreter::Execute_TapeForSave(FM77AVThread &thr,FM77AV &fm77av,Command &cmd)
{
	if(2<=cmd.argv.size())
	{
		auto fileName=fm77av.FileNameAlias(cmd.argv[1]);
		if(true==fm77av.dataRecorder.LoadT77(fileName))
		{
			std::cout << "Loaded Tape Image for Save." << std::endl;
		}
		else
		{
			std::cout << "Failed to load Tape Image for Save." << std::endl;
			Error_CannotOpenFile(cmd);
		}
	}
	else
	{
		Error_TooFewArgs(cmd);
	}
}
void FM77AVCommandInterpreter::Execute_TapeEject(FM77AVThread &thr,FM77AV &fm77av,Command &cmd)
{
	fm77av.dataRecorder.Eject();
	std::cout << "Ejected Tape." << std::endl;
}
void FM77AVCommandInterpreter::Execute_Gameport(FM77AV &fm77av,Outside_World *outside_world,Command &cmd)
{
	if(3<=cmd.argv.size())
	{
		unsigned int port=cpputil::Atoi(cmd.argv[1].c_str());
		unsigned int devType=StrToGamePortEmu(cmd.argv[2]);
		if(port<FM77AV_NUM_GAMEPORTS)
		{
			fm77av.gameport.state.ports[port].device=FM77AVGamePort::EmulationTypeToDeviceType(devType);
			outside_world->gamePort[port]=devType;
			outside_world->CacheGamePadIndicesThatNeedUpdates();
		}
		else
		{
			std::cout << "Port needs to be 0 or 1." << std::endl;
		}
	}
	else
	{
		std::cout << "List of Device Options for Game Port:" << std::endl;
		for(int i=0; i<FM77AV_GAMEPORTEMU_NUM_DEVICES; ++i)
		{
			std::cout << GamePortEmuToStr(i) << std::endl;
		}
	}
}
void FM77AVCommandInterpreter::Execute_AddSymbol(FM77AVThread &thr,FM77AV &fm77av,Command &cmd)
{
	if(3<=cmd.argv.size() || 
	  (2<=cmd.argv.size() && (CMD_IMM_IS_IOPORT==cmd.primaryCmd || CMD_OFFSET_IS_LABEL==cmd.primaryCmd || CMD_IMM_IS_LABEL==cmd.primaryCmd)))
	{
		auto ptr=DecodeAddress(fm77av,cmd.argv[1],thr.OnlyOneCPUIsUnmuted(),thr.OnlyOneCPUIsUnmuted());
		if(FM77AV::ADDR_MAIN!=ptr.type && FM77AV::ADDR_SUB!=ptr.type)
		{
			Error_UnknownCPU(cmd);
			return;
		}

		auto &symTable=fm77av.CPU(ptr.type).debugger.symTable;

		switch(cmd.primaryCmd)
		{
		case CMD_ADD_SYMBOL:
		case CMD_ADD_LABEL:
		case CMD_ADD_DATALABEL:
			{
				auto *newSym=symTable.Update(ptr.addr,cmd.argv[2]);
				switch(cmd.primaryCmd)
				{
				case CMD_ADD_SYMBOL:
					newSym->symType=MC6809Symbol::SYM_PROCEDURE;
					break;
				case CMD_ADD_LABEL:
					newSym->symType=MC6809Symbol::SYM_JUMP_DESTINATION;
					break;
				case CMD_ADD_DATALABEL:
					newSym->symType=MC6809Symbol::SYM_DATA_LABEL;
					break;
				}
				std::cout << "Added symbol " << cmd.argv[2] << std::endl;
			}
			break;
		case CMD_DEF_RAW_BYTES:
			if(4<=cmd.argv.size())
			{
				auto numBytes=cpputil::Xtoi(cmd.argv[3].c_str());
				if(0<numBytes)
				{
					auto *newSym=symTable.Update(ptr.addr,cmd.argv[2]);
					newSym->symType=MC6809Symbol::SYM_RAW_DATA;
					newSym->rawDataBytes=numBytes;
					std::cout << "Added raw byte data " << cmd.argv[2] << std::endl;
				}
				else
				{
					std::cout << "Ignored 0-byte data " << cmd.argv[2] << std::endl;
				}
			}
			else
			{
				Error_TooFewArgs(cmd);
				return;
			}
			break;
		case CMD_ADD_COMMENT:
			{
				symTable.SetComment(ptr.addr,cmd.argv[2]);
				std::cout << "Added comment " << cmd.argv[2] << std::endl;
			}
			break;
		case CMD_IMM_IS_IOPORT:
			{
				symTable.SetImmIsIOPort(ptr.addr);
				std::cout << "Added imm-is-IO " << cmd.argv[2] << std::endl;
			}
			break;
		case CMD_IMM_IS_LABEL:
			{
				symTable.SetImmIsSymbol(ptr.addr);
				std::cout << "Added imm-is-label " << cmd.argv[2] << std::endl;
			}
			break;
		case CMD_OFFSET_IS_LABEL:
			{
				symTable.SetOffsetIsSymbol(ptr.addr);
				std::cout << "Added offset-is-label " << cmd.argv[2] << std::endl;
			}
			break;
		}

		if(true!=fm77av.AutoSaveSymbolTable())
		{
			std::cout << "Symbol Table Not Auto-Saved." << std::endl;
		}
	}
	else
	{
		Error_TooFewArgs(cmd);
	}
}

void FM77AVCommandInterpreter::Execute_ImportLSTSymbolTable(FM77AVThread &thr,FM77AV &fm77av,Command &cmd)
{
	if(3<=cmd.argv.size())
	{
		std::cout << "Not implemented yet." << std::endl;
	}
	else
	{
		Error_TooFewArgs(cmd);
	}
}
void FM77AVCommandInterpreter::Execute_DelSymbol(FM77AVThread &thr,FM77AV &fm77av,Command &cmd)
{
	if(2<=cmd.argv.size())
	{
		auto ptr=DecodeAddress(fm77av,cmd.argv[1],thr.OnlyOneCPUIsUnmuted(),thr.OnlyOneCPUIsUnmuted());
		if(FM77AV::ADDR_MAIN!=ptr.type && FM77AV::ADDR_SUB!=ptr.type)
		{
			Error_UnknownCPU(cmd);
			return;
		}

		auto &symTable=fm77av.CPU(ptr.type).debugger.symTable;

		if(true==symTable.Delete(ptr.addr))
		{
			std::cout << "Symbol&|Comment deleted." << std::endl;
		}
		else if(nullptr==symTable.Find(ptr.addr))
		{
			Error_SymbolNotFound(cmd);
		}
		else
		{
			Error_CouldNotDeleteSymbol(cmd);
		}
	}
}
void FM77AVCommandInterpreter::Execute_SymbolInquiry(FM77AVThread &thr,FM77AV &fm77av,Command &cmd)
{
	std::string ptn="*";
	bool showLabel=true,showProc=true,showComment=true;
	switch(cmd.primaryCmd)
	{
	case CMD_PRINT_SYMBOL:
		break;
	case CMD_PRINT_SYMBOL_LABEL_PROC:
		showComment=false;
		break;
	case CMD_PRINT_SYMBOL_PROC:
		showComment=false;
		showLabel=false;
		break;
	case CMD_PRINT_SYMBOL_FIND:
		if(3<=cmd.argv.size())
		{
			ptn="*"+cmd.argv[2]+"*";
		}
		else
		{
			ptn="*"+cmd.argv[1]+"*";
		}
		break;
	}
	for(int i=0; i<2; ++i)
	{
		auto &debugger=(0==i ? fm77av.mainCPU.debugger : fm77av.subCPU.debugger);
		for(auto &entry : debugger.symTable.GetTable())
		{
			auto &addr=entry.first;
			auto &sym=entry.second;

			if((showProc==true && sym.symType==MC6809Symbol::SYM_PROCEDURE) ||
			   (showLabel==true && sym.symType==MC6809Symbol::SYM_JUMP_DESTINATION) ||
			   (showComment==true && ""!=sym.inLineComment))
			{
				std::string addrStr,textStr;
				bool returnType=true,label=true,param=true;
				addrStr=cpputil::Ustox(addr);
				addrStr+=" ";

				textStr=sym.Format(returnType,label,param);
				if(0<sym.inLineComment.size())
				{
					textStr+=" ; ";
					textStr+=sym.inLineComment;
				}

				auto PTN=ptn;
				auto TEXTSTR=textStr;
				cpputil::Capitalize(PTN);
				cpputil::Capitalize(TEXTSTR);
				if("*"!=ptn && true!=cpputil::WildCardCompare(PTN,TEXTSTR))
				{
					continue;
				}

				std::cout << addrStr << " " << textStr << std::endl;
			}
		}
	}
}
