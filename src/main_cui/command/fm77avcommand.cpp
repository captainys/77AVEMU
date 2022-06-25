#include <iostream>
#include "fm77avcommand.h"
#include "cpputil.h"


FM77AVCommandInterpreter::FM77AVCommandInterpreter()
{
	primaryCmdMap["HELP"]=CMD_HELP;
	primaryCmdMap["HLP"]=CMD_HELP;
	primaryCmdMap["H"]=CMD_HELP;
	primaryCmdMap["QUIT"]=CMD_QUIT;
	primaryCmdMap["Q"]=CMD_QUIT;
	primaryCmdMap["FORCEQUIT"]=CMD_FORCE_QUIT;
	primaryCmdMap["RUN"]=CMD_RUN;
	primaryCmdMap["PAUSE"]=CMD_PAUSE;
	primaryCmdMap["PAU"]=CMD_PAUSE;
	primaryCmdMap["T"]=CMD_RUN_ONE_INSTRUCTION;
	primaryCmdMap["MUTE"]=CMD_MUTE;
	primaryCmdMap["UNMUTE"]=CMD_UNMUTE;
}

void FM77AVCommandInterpreter::PrintHelp(void) const
{
	std::cout << "<< Primary Command >>" << std::endl;
	std::cout << "HELP|HLP|H|?" << std::endl;
	std::cout << "  Print help." << std::endl;
	std::cout << "QUIT|Q" << std::endl;
	std::cout << "  Quit." << std::endl;
	std::cout << "RUN" << std::endl;
	std::cout << "  Run." << std::endl;
	std::cout << "RUN M:addr/S:addr" << std::endl;
	std::cout << "  Run to specified address." << std::endl;
	std::cout << "RUN M:addr/S:addr addr" << std::endl;
	std::cout << "  Run to specified address range (RUN S:0000 DFFF)." << std::endl;
	std::cout << "T" << std::endl;
	std::cout << "  Trace.  Run one instruction." << std::endl;
	std::cout << "PAUSE|PAU" << std::endl;
	std::cout << "  Pause VM." << std::endl;
	std::cout << "MUTE M|S" << std::endl;
	std::cout << "UNMUTE M|S" << std::endl;
	std::cout << "  Mute/Unmute state output of main/sub CPU." << std::endl;
	std::cout << "  If no parameter is given, mute/unmutes both CPUs." << std::endl;
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
void FM77AVCommandInterpreter::Error_CPUOrAddress(const Command &cmd)
{
	std::cout << "Error in CPU or Address." << std::endl;
	Error_Common(cmd);
}

/* static */ bool FM77AVCommandInterpreter::DecomposeCPUandAddress(unsigned int &cpu,uint16_t &addr,std::string arg)
{
	std::string cpuStr;
	for(int i=0; i<arg.size(); ++i)
	{
		if(':'==arg[i])
		{
			std::string addrStr=arg.substr(i+1);
			cpu=StrToCPU(cpuStr);
			addr=cpputil::Xtoi(addrStr.data());
			return true;
		}
		else
		{
			cpuStr.push_back(arg[i]);
		}
	}
	return false;
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
	case CMD_FORCE_QUIT:
		exit(0);
		break;
	case CMD_RUN:
		Execute_Run(thr,fm77av,outside_world,cmd);
		break;
	case CMD_PAUSE:
		thr.SetRunMode(FM77AVThread::RUNMODE_PAUSE);
		thr.PrintStatus(fm77av);
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
	}
}

void FM77AVCommandInterpreter::Execute_Run(FM77AVThread &thr,FM77AV &fm77av,class Outside_World *outside_world,Command &cmd)
{
	if(3<=cmd.argv.size())
	{
		unsigned int cpu;
		uint16_t startAddr,endAddr;
		if(true==DecomposeCPUandAddress(cpu,startAddr,cmd.argv[1]))
		{
			endAddr=cpputil::Xtoi(cmd.argv[1].data());
			fm77av.CPU(cpu).debugger.SetOneTimeBreakPoint(startAddr,endAddr);
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
		unsigned int cpu;
		uint16_t addr;
		if(true==DecomposeCPUandAddress(cpu,addr,cmd.argv[1]))
		{
			fm77av.CPU(cpu).debugger.SetOneTimeBreakPoint(addr,addr);
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
