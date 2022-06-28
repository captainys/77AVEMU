#include <iostream>
#include "cpputil.h"
#include "miscutil.h"
#include "fm77avcommand.h"
#include "fm77avlineparser.h"


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
	primaryCmdMap["ENABLE"]=CMD_ENABLE;
	primaryCmdMap["ENA"]=CMD_ENABLE;
	primaryCmdMap["DISABLE"]=CMD_DISABLE;
	primaryCmdMap["DIS"]=CMD_DISABLE;
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
	primaryCmdMap["BRKON"]=CMD_BREAK_ON;
	primaryCmdMap["CBRKON"]=CMD_DONT_BREAK_ON;

	featureMap["IOMON"]=ENABLE_IOMONITOR;
	featureMap["SUBCMDMON"]=ENABLE_SUBSYSCMD_MONITOR;
	featureMap["BIOSMON"]=ENABLE_BIOSCMD_MONITOR;

	breakEventMap["SUBUNHALT"]=BREAK_ON_SUBCPU_UNHALT;
	breakEventMap["UNHALTSUB"]=BREAK_ON_SUBCPU_UNHALT;
	breakEventMap["SUBCMD"]=BREAK_ON_SUBCMD;
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
	std::cout << "ENA feature|ENABLE feature" << std::endl;
	std::cout << "  Enable a feature." << std::endl;
	std::cout << "DIS feature|DISABLE feature" << std::endl;
	std::cout << "  Disable a feature." << std::endl;
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
	std::cout << "BRKON event" << std::endl;
	std::cout << "  Break on event." << std::endl;
	std::cout << "CBRKON event" << std::endl;
	std::cout << "  Don't break on event." << std::endl;

	std::cout << "<< Features that can be enabled|disabled >>" << std::endl;
	std::cout << "IOMON iopotMin ioportMax" << std::endl;
	std::cout << "  IO Monitor." << std::endl;
	std::cout << "  ioportMin and ioportMax are optional." << std::endl;
	std::cout << "  Can specify multiple range by enabling IOMON multiple times." << std::endl;
	std::cout << "SUBCMDMON" << std::endl;
	std::cout << "  Monitor sub-system command when sub-CPU is unhalted." << std::endl;
	std::cout << "BIOSMON" << std::endl;
	std::cout << "  Monitor BIOS Call." << std::endl;

	std::cout << "<< Event that can break >>" << std::endl;
	std::cout << "SUBUNHALT | UNHALTSUB" << std::endl;
	std::cout << "  Sub-CPU Unhalted." << std::endl;
	std::cout << "SUBCMD ##" << std::endl;
	std::cout << "  Break on Sub-CPU command.  ## is a number in hexadecimal." << std::endl;
	std::cout << "  Break timing is same as SUBUNHALT/UNHALTSUB" << std::endl;
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
				FM77AVLineParserHexadecimal parser(&fm77av.CPU(ptr.cpu));
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
/* static */ uint16_t FM77AVCommandInterpreter::MakeAddressForCPU(const MC6809 &cpu,std::string arg)
{
	FM77AVLineParserHexadecimal parser(&cpu);
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
	case CMD_FORCE_QUIT:
		exit(0);
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
		Execute_MemoryDump(fm77av,cmd);
		break;
	case CMD_DUMP:
		Execute_Dump(fm77av,cmd);
		break;
	case CMD_BREAK_ON:
		Execute_BreakOn(fm77av,cmd);
		break;
	case CMD_DONT_BREAK_ON:
		Execute_DontBreakOn(fm77av,cmd);
		break;
	}
}

void FM77AVCommandInterpreter::Execute_Run(FM77AVThread &thr,FM77AV &fm77av,class Outside_World *outside_world,Command &cmd)
{
	if(3<=cmd.argv.size())
	{
		auto ptr=MakeCPUandAddress(fm77av,cmd.argv[1]);
		if(CPU_UNKNOWN!=ptr.cpu)
		{
			FM77AVLineParserHexadecimal parser(&fm77av.CPU(ptr.cpu));
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
		auto ptr=MakeCPUandAddress(fm77av,cmd.argv[1]);
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
			break;
		case ENABLE_SUBSYSCMD_MONITOR:
			fm77av.var.monitorSubSysCmd=true;
			std::cout << "Enabled Sub-System Command Monitor." << std::endl;
			break;
		case ENABLE_BIOSCMD_MONITOR:
			fm77av.var.monitorBIOSCall=true;
			std::cout << "Enabled BIOS Monitor." << std::endl;
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
		case ENABLE_SUBSYSCMD_MONITOR:
			fm77av.var.monitorSubSysCmd=false;
			std::cout << "Disabled Sub-System Command Monitor." << std::endl;
			break;
		case ENABLE_BIOSCMD_MONITOR:
			fm77av.var.monitorBIOSCall=false;
			std::cout << "Disabled BIOS Monitor." << std::endl;
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
		auto ptr=MakeCPUandAddress(fm77av,cmd.argv[1]);
		if(CPU_UNKNOWN!=ptr.cpu)
		{
			fm77av.var.lastDisassemblyCPU=ptr.cpu;
			auto &cpu=fm77av.CPU(ptr.cpu);
			auto &mem=fm77av.MemAccess(ptr.cpu);
			auto PC=ptr.addr;
			for(int i=0; i<DISASM_NUM_LINES; ++i)
			{
				auto inst=cpu.NonDestructiveFetchInstruction(mem,PC);
				std::cout << cpu.WholeDisassembly(mem,PC) << std::endl;
				PC+=inst.length;
			}
			cpu.debugger.nextDisassemblyAddr=PC;
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
			std::cout << cpu.WholeDisassembly(mem,PC) << std::endl;
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
	if(cmd.argv.size()<2)
	{
		PC=cpu.debugger.nextDisassemblyAddr;
	}
	else
	{
		PC=MakeAddressForCPU(cpu,cmd.argv[1]);
	}
	for(int i=0; i<DISASM_NUM_LINES; ++i)
	{
		auto inst=cpu.NonDestructiveFetchInstruction(mem,PC);
		std::cout << cpu.WholeDisassembly(mem,PC) << std::endl;
		PC+=inst.length;
	}
	cpu.debugger.nextDisassemblyAddr=PC;
}
void FM77AVCommandInterpreter::Execute_Disassemble_Sub(FM77AVThread &thr,FM77AV &fm77av,class Outside_World *outside_world,Command &cmd)
{
	auto &cpu=fm77av.subCPU;
	auto &mem=fm77av.subMemAcc;
	uint16_t PC;
	if(cmd.argv.size()<2)
	{
		PC=cpu.debugger.nextDisassemblyAddr;
	}
	else
	{
		PC=MakeAddressForCPU(cpu,cmd.argv[1]);
	}
	for(int i=0; i<DISASM_NUM_LINES; ++i)
	{
		auto inst=cpu.NonDestructiveFetchInstruction(mem,PC);
		std::cout << cpu.WholeDisassembly(mem,PC) << std::endl;
		PC+=inst.length;
	}
	cpu.debugger.nextDisassemblyAddr=PC;
}
void FM77AVCommandInterpreter::Execute_Dump(FM77AV &fm77av,Command &cmd)
{
}
void FM77AVCommandInterpreter::Execute_MemoryDump(FM77AV &fm77av,Command &cmd)
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

		auto ptr=MakeCPUandAddress(fm77av,cmd.argv[1]);
		if(CPU_UNKNOWN!=ptr.cpu)
		{
			auto &cpu=fm77av.CPU(ptr.cpu);
			auto &mem=fm77av.MemAccess(ptr.cpu);
			for(auto str : miscutil::MakeMemDump2(cpu,mem,ptr.addr,wid,hei,skip,/*shiftJIS*/false,ascii))
			{
				std::cout << str << std::endl;
			}
		}
		else
		{
			Error_CPUOrAddress(cmd);
			return;
		}
	}
}
void FM77AVCommandInterpreter::Execute_BreakOn(FM77AV &av,Command &cmd)
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
				av.var.breakOnUnhaltSubCPU=true;
				std::cout << "Break on Sub-CPU Unhalt." << std::endl;
				break;
			case BREAK_ON_SUBCMD:
				if(3<=cmd.argv.size())
				{
					auto num=cpputil::Xtoi(cmd.argv[2].data());
					if(num<FM7_MAX_SUB_CMD)
					{
						av.var.breakOnSubCmd[num]=MC6809::Debugger::BRKPNT_FLAG_BREAK;
						std::cout << "Break On Sub-CPU Command " << cpputil::Ubtox(num) << " " << SubCmdToStr(num) << std::endl;
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
void FM77AVCommandInterpreter::Execute_DontBreakOn(FM77AV &av,Command &cmd)
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
