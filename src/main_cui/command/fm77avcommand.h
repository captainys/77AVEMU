#ifndef FM77AVCOMMAND_IS_INCLUDED
#define FM77AVCOMMAND_IS_INCLUDED
/* { */

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

		CMD_HELP,

		CMD_NOWAIT,
		CMD_YESWAIT,

		CMD_RUN,
		CMD_RUN_ONE_INSTRUCTION,
		CMD_PAUSE,

		CMD_MUTE,
		CMD_UNMUTE,

		CMD_ENABLE,
		CMD_DISABLE,

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

		CMD_TYPE_KEYBOARD,
		CMD_KEYBOARD,

		CMD_LOAD_EVENTLOG,
		CMD_PLAY_EVENTLOG,
		CMD_STOP_EVENTLOG,

		CMD_QUICK_SCREENSHOT,
		CMD_QUICK_SCREENSHOT_DIR,

		CMD_SAVE_COM0OUT,
		CMD_CLEAR_COM0OUT,
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
	void Error_CPUOrAddress(const Command &cmd);
	void Error_UnknownFeature(const Command &cmd);
	void Error_UnknownEvent(const Command &cmd);
	void Error_IllegalSubCommand(const Command &cmd);
	void Error_WrongParameter(const Command &cmd);
	void Error_CannotSaveFile(const Command &cmd);
	void Error_CannotOpenFile(const Command &cmd);
	void Error_Addressing(const Command &cmd);

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
	void Execute_BreakOn(FM77AVThread &thr,FM77AV &av,Command &cmd);
	void Execute_DontBreakOn(FM77AVThread &thr,FM77AV &av,Command &cmd);
	void Execute_BreakOnMemoryRead(FM77AVThread &thr,FM77AV &av,Command &cmd);
	void Execute_BreakOnMemoryWrite(FM77AVThread &thr,FM77AV &av,Command &cmd);
	void Execute_AddBreakPoint(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_AddBreakPointWithPassCount(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_AddMonitorPoint(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_DeleteBreakPoint(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_ListBreakPoints(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_DontBreakOnMemoryRead(FM77AVThread &thr,FM77AV &av,Command &cmd);
	void Execute_DontBreakOnMemoryWrite(FM77AVThread &thr,FM77AV &av,Command &cmd);
	void Execute_BreakOnHardwareVRAMWrite(FM77AVThread &thr,FM77AV &av,Command &cmd);
	void Execute_DontBreakOnHardwareVRAMWrite(FM77AVThread &thr,FM77AV &av,Command &cmd);
	void Execute_FDLoad(int drv,FM77AV &fm77av,Command &cmd);
	void Execute_FDEject(int drv,FM77AV &fm77av,Command &cmd);
	void Execute_QuickScreenShotDirectory(FM77AV &fm77av,Command &cmd);
	void Execute_QuickScreenShot(FM77AV &fm77av,Command &cmd);
	void Execute_EnableCallStack(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_DisableCallStack(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_PrintCallStack(FM77AVThread &thr,FM77AV &fm77av,Command &cmd);
	void Execute_TypeKeyboard(FM77AV &fm77av,Command &cmd);
	void Execute_SaveCOM0Out(FM77AV &fm77av,Command &cmd);
	void Execute_ClearCom0Out(FM77AV &fm77av,Command &cmd);
};

/* } */
#endif
