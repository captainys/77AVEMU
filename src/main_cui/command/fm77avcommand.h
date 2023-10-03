#ifndef FM77AVCOMMAND_IS_INCLUDED
#define FM77AVCOMMAND_IS_INCLUDED
/* { */

#include <string>
#include <unordered_map>
#include "fm77av.h"
#include "fm77avthread.h"

class FM77AVCommandInterpreter
{
private:
	std::unordered_map <std::string,unsigned int> primaryCmdMap;
	std::unordered_map <std::string,unsigned int> featureMap;
	std::unordered_map <std::string,unsigned int> dumpableMap;
	std::unordered_map <std::string,unsigned int> breakEventMap;

public:
	enum
	{
		DISASM_NUM_LINES=16,
	};

	enum
	{
		CMD_NONE,

		CMD_QUIT,
		CMD_FORCE_QUIT,

		CMD_RESET,

		CMD_HELP,

		CMD_NOWAIT,
		CMD_YESWAIT,

		CMD_SETSPEED,

		CMD_RUN,
		CMD_RUN_ONE_INSTRUCTION,
		CMD_PAUSE,

		CMD_MUTE,
		CMD_UNMUTE,

		CMD_ENABLE,
		CMD_DISABLE,

		CMD_TAPE_LOAD,
		CMD_TAPE_SAVETAPE,
		CMD_TAPE_EJECT,
		CMD_TAPE_WRITE_PROTECT,
		CMD_TAPE_WRITE_UNPROTECT,
		CMD_TAPE_REWIND,
		CMD_TAPE_REC_BUTTON_ON,
		CMD_TAPE_REC_BUTTON_RELEASE,
		CMD_TAPE_SAVE_AS,

		CMD_FD0LOAD,
		CMD_FD0EJECT,
		CMD_FD0WRITEPROTECT,
		CMD_FD0WRITEUNPROTECT,
		CMD_FD1LOAD,
		CMD_FD1EJECT,
		CMD_FD1WRITEPROTECT,
		CMD_FD1WRITEUNPROTECT,

		CMD_START_AUDIO_RECORDING,
		CMD_END_AUDIO_RECORDING,
		CMD_SAVE_AUDIO_RECORDING,

		CMD_START_VGM_RECORDING,
		CMD_END_VGM_RECORDING,
		CMD_SAVE_VGM_RECORDING,

		CMD_FMCH,
		CMD_PSGCH,

		CMD_PRINT_STATUS,
		CMD_PRINT_HISTORY,

		CMD_DISASM,
		CMD_DISASM_MAIN,
		CMD_DISASM_SUB,

		CMD_DUMP,
		CMD_MEMDUMP,

		CMD_SAVE_HISTORY,

		CMD_ADD_BREAKPOINT,
		CMD_ADD_BREAKPOINT_WITH_PASSCOUNT,
		CMD_ADD_MONITORPOINT,
		CMD_DELETE_BREAKPOINT,
		CMD_LIST_BREAKPOINTS,

		CMD_BREAK_ON,
		CMD_DONT_BREAK_ON,
		CMD_MONITOR,
		CMD_DONT_MONITOR,
		CMD_OS9MODE,

		CMD_ADD_SYMBOL,
		CMD_ADD_LABEL,
		CMD_ADD_DATALABEL,
		CMD_ADD_COMMENT,
		CMD_DEF_RAW_BYTES,
		CMD_IMM_IS_IOPORT,
		CMD_IMM_IS_LABEL,
		CMD_IMM_IS_ASCII,
		CMD_OFFSET_IS_LABEL,
		CMD_DEL_SYMBOL,
		CMD_IMPORT_LST_SYMTABLE,

		CMD_PRINT_SYMBOL,
		CMD_PRINT_SYMBOL_LABEL_PROC,
		CMD_PRINT_SYMBOL_PROC,
		CMD_PRINT_SYMBOL_FIND,

		CMD_TYPE_KEYBOARD,
		CMD_KEYBOARD,

		CMD_LOAD_EVENTLOG,
		CMD_PLAY_EVENTLOG,
		CMD_STOP_EVENTLOG,

		CMD_SAVE_SCREENSHOT,
		CMD_QUICK_SCREENSHOT,
		CMD_QUICK_SCREENSHOT_DIR,

		CMD_SAVE_COM0OUT,
		CMD_CLEAR_COM0OUT,

		CMD_LET,
		CMD_EDIT_MEMORY_BYTE,
		CMD_EDIT_MEMORY_WORD,
		CMD_EDIT_MEMORY_STRING,

		CMD_GAMEPORT,

		CMD_SAVE_STATE,
		CMD_LOAD_STATE,

		CMD_AUTOSTOPKEY,

		CMD_CALCULATE,
		CMD_STRING_TO_ASCII,
		CMD_ASCII_TO_STRING,

		CMD_MAKE_MEMORY_FILTER,
		CMD_UPDATE_MEMORY_FILTER,

		CMD_FIND,
		CMD_FIND_STRING,

		CMD_DISPLAY_PAGE,

		CMD_DOSMODE,
		CMD_BASMODE,
	};

	enum
	{
		ENABLE_IOMONITOR,
		ENABLE_FDCMONITOR,
		ENABLE_SUBSYSCMD_MONITOR,
		ENABLE_BIOSCMD_MONITOR,
		ENABLE_DEBUGGER,
		ENABLE_CALLSTACK,
		ENABLE_PRINT_COM0,
		ENABLE_PSG_LOG,
		ENABLE_AUTOSTOP,
		ENABLE_RKANA,
		ENABLE_AUDIOLEVELMETER,
		ENABLE_LOG_DISASM,
		ENABLE_LOG_ALLREGISTERS,
	};

	enum
	{
		BREAK_ON_NULL,
		BREAK_ON_SUBCPU_UNHALT,
		BREAK_ON_SUBCMD,
		BREAK_ON_MEM_READ,
		BREAK_ON_MEM_WRITE,
		BREAK_ON_HARDWARE_VRAM_WRITE,
		BREAK_ON_HARDWARE_LINE_DRAWING,
		BREAK_ON_AY38910_WRITE,
		BREAK_ON_YM2203C_WRITE,
		BREAK_ON_READ_SECTOR,
		BREAK_ON_FDC_IRQ,
	};

	enum
	{
		DUMP_NULL,
		DUMP_TAPE,
		DUMP_FDC,
		DUMP_CRTC,
		DUMP_PC_LOG,
		DUMP_SOUND,
		DUMP_MEMORY,
		DUMP_CALLSTACK,
		DUMP_IRQ,
		DUMP_PSG_LOG,
		DUMP_CAS0,
		DUMP_CAS1,
		DUMP_MEMORY_FILTER,
		DUMP_WHERE_I_AM,
	};

	class Command
	{
	public:
		std::string cmdline;
		std::vector <std::string> argv;
		int primaryCmd;
	};

	bool cuiQuit=false;

	FM77AVCommandInterpreter();

	void PrintHelp(void) const;
	void Error_Common(const Command &cmd);
	void Error_TooFewArgs(const Command &cmd);
	void Error_UnknownCPU(const Command &cmd);
	void Error_UnknownRegister(const Command &cmd);
	void Error_CPUOrAddress(const Command &cmd);
	void Error_UnknownFeature(const Command &cmd);
	void Error_UnknownEvent(const Command &cmd);
	void Error_IllegalSubCommand(const Command &cmd);
	void Error_WrongParameter(const Command &cmd);
	void Error_CannotSaveFile(const Command &cmd);
	void Error_CannotOpenFile(const Command &cmd);
	void Error_Addressing(const Command &cmd);
	void Error_SymbolNotFound(const Command &cmd);
	void Error_CouldNotDeleteSymbol(const Command &cmd);

	static std::string JustMonitorOrBreakString(bool justMonitor);

	Command Interpret(const std::string &cmdline) const;

	/*! If thread is given, and the CPU main or sub is not given, try the unmuted CPU if one is muted and one is unmuted.
	*/
	static FM77AV::Address DecodeAddress(const FM77AV &fm77av,std::string arg,uint8_t defaultCPU,uint8_t defaultAddrType=FM77AV::ADDR_NONE);
	static FM77AV::CPUAddr MakeCPUandAddress(const FM77AV &fm77av,std::string arg);
	static FM77AV::CPUAddr MakeCPUandAddress(const FM77AVThread &thr,const FM77AV &fm77av,std::string arg);
	static uint32_t DecodePhysicalAddress(const FM77AV &fm77av,std::string arg);
	static uint16_t MakeAddressForCPU(const FM77AV &fm77av,const MC6809 &cpu,std::string arg);

	/*! Executes a command.
	    VM must be locked before calling.
	*/
	void Execute(FM77AVThread &thr,FM77AV &fm77av,class Outside_World *outside_world,Command &cmd);
	void Execute_Run(FM77AVThread &thr,FM77AV &fm77av,class Outside_World *outside_world,Command &cmd);
	void Execute_Enable(FM77AVThread &thr,FM77AV &fm77av,class Outside_World *outside_world,Command &cmd);
	void Execute_Disable(FM77AVThread &thr,FM77AV &fm77av,class Outside_World *outside_world,Command &cmd);
	void Execute_Disassemble(FM77AVThread &thr,FM77AV &fm77av,class Outside_World *outside_world,Command &cmd);
	void Execute_Disassemble_Main(FM77AVThread &thr,FM77AV &fm77av,class Outside_World *outside_world,Command &cmd);
	void Execute_Disassemble_Sub(FM77AVThread &thr,FM77AV &fm77av,class Outside_World *outside_world,Command &cmd);
	void Execute_Dump(FM77AVThread &thr,FM77AV &av,Command &cmd);
	void Execute_PrintHistory(FM77AVThread &thr,FM77AV &av,Command &cmd);
	void Execute_SaveHistory(FM77AVThread &thr,FM77AV &av,Command &cmd);
	void Execute_MemoryDump(FM77AVThread &thr,FM77AV &av,Command &cmd);
	void Execute_BreakOnOrMonitor(FM77AVThread &thr,FM77AV &av,Command &cmd,bool justMonitorDontBreak);
	void Execute_DontBreakOn(FM77AVThread &thr,FM77AV &av,Command &cmd);
	void Execute_BreakOnMemoryRead(FM77AVThread &thr,FM77AV &av,Command &cmd,bool justMonitorDontBreak);
	void Execute_BreakOnMemoryWrite(FM77AVThread &thr,FM77AV &av,Command &cmd,bool justMonitorDontBreak);
	void Execute_AddBreakPoint(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_AddBreakPointWithPassCount(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_AddMonitorPoint(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_DeleteBreakPoint(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_ListBreakPoints(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_DontBreakOnMemoryRead(FM77AVThread &thr,FM77AV &av,Command &cmd);
	void Execute_DontBreakOnMemoryWrite(FM77AVThread &thr,FM77AV &av,Command &cmd);
	void Execute_BreakOnHardwareVRAMWrite(FM77AVThread &thr,FM77AV &av,Command &cmd,bool justMonitorDontBreak);
	void Execute_DontBreakOnHardwareVRAMWrite(FM77AVThread &thr,FM77AV &av,Command &cmd);
	void Execute_FDLoad(int drv,FM77AV &fm77av,Command &cmd);
	void Execute_FDEject(int drv,FM77AV &fm77av,Command &cmd);
	void Execute_SaveScreenShot(FM77AV &fm77av,Command &cmd);
	void Execute_QuickScreenShotDirectory(FM77AV &fm77av,Command &cmd);
	void Execute_QuickScreenShot(FM77AV &fm77av,Command &cmd);
	void SaveScreenShot(FM77AV &fm77av,std::string fName);
	void Execute_EnableCallStack(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_DisableCallStack(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_PrintCallStack(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_TypeKeyboard(FM77AV &fm77av,Command &cmd);
	void Execute_SaveCOM0Out(FM77AV &fm77av,Command &cmd);
	void Execute_ClearCom0Out(FM77AV &fm77av,Command &cmd);
	void Execute_Let(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_SetSpeed(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_LoadTape(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_TapeForSave(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_TapeEject(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_Gameport(FM77AV &fm77av,Outside_World *outside_world,Command &cmd);
	void Execute_AddSymbol(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_DelSymbol(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_ImportLSTSymbolTable(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_SymbolInquiry(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_EditMemory(FM77AVThread &thr,FM77AV &fm77av,Command &cmd,unsigned int numBytes);
	void Execute_Calculate(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_MakeMemoryFilter(FM77AV &fm77av,Command &cmd);
	void Execute_UpdateMemoryFilter(FM77AV &fm77av,Command &cmd);
	void Execute_Search_Bytes(FM77AV &fm77av,Command &cmd);
	void Execute_Search_String(FM77AV &fm77av,Command &cmd);
	void Execute_Search_ByteSequence(FM77AV &fm77av,const std::vector <unsigned char> &bytes);
	void FoundAt(FM77AV &fm77av,unsigned int physAddr);
};

/* } */
#endif
