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

		CMD_RUN,
		CMD_RUN_ONE_INSTRUCTION,
		CMD_PAUSE,

		CMD_MUTE,
		CMD_UNMUTE,

		CMD_ENABLE,
		CMD_DISABLE,

		CMD_DISASM,
		CMD_DISASM_MAIN,
		CMD_DISASM_SUB,
	};

	enum
	{
		ENABLE_IOMONITOR,
		ENABLE_SUBSYSCMD_MONITOR,
		ENABLE_BIOSCMD_MONITOR,
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

	Command Interpret(const std::string &cmdline) const;

	static FM77AV::CPUAddr MakeCPUandAddress(const FM77AV &fm77av,std::string arg);
	static uint16_t MakeAddressForCPU(const MC6809 &cpu,std::string arg);

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
};

/* } */
#endif
