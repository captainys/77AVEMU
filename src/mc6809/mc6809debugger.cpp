#include "mc6809.h"

MC6809::Debugger::Debugger()
{
	CleanUp();
}

void MC6809::Debugger::CleanUp(void)
{
	for(auto &f : memRead)
	{
		f.flags=0;
	}
	for(auto &f : memWrite)
	{
		f.flags=0;
	}

	for(auto &log : usingPCLog->log)
	{
		log.regs.PC=0;
		log.regs.X=0;
		log.regs.Y=0;
		log.regs.U=0;
		log.regs.CC=0;
		log.regs.D=0;
		log.regs.DP=0;
		log.count=0;
	}
	usingPCLog->ptr=0;
}

void MC6809::Debugger::ClearStopFlag(void)
{
	stop=false;
}

void MC6809::Debugger::ExternalBreak(std::string reason)
{
	externalBreakReason=reason;
	stop=true;
}

void MC6809::Debugger::SetBreakPoint(uint16_t addrStart,uint16_t addrEnd,unsigned int passCount,unsigned char flags)
{
	for(uint32_t addr=addrStart; addr<=addrEnd; ++addr)
	{
		breakPoints[addr].flags=flags;
		breakPoints[addr].passed=0;
		breakPoints[addr].passCount=passCount;
	}
}
void MC6809::Debugger::ClearBreakPoint(uint16_t addrStart,uint16_t addrEnd)
{
	for(uint32_t addr=addrStart; addr<=addrEnd; ++addr)
	{
		breakPoints[addr].flags=0;
	}
}
void MC6809::Debugger::SetOneTimeBreakPoint(uint16_t addrStart,uint16_t addrEnd,unsigned int passCount,unsigned char flags)
{
	for(uint32_t addr=addrStart; addr<=addrEnd; ++addr)
	{
		oneTimeBreakPoints[addr].flags=flags;
		oneTimeBreakPoints[addr].passed=0;
		oneTimeBreakPoints[addr].passCount=passCount;
	}
}
void MC6809::Debugger::ClearOneTimeBreakPoint(uint16_t addrStart,uint16_t addrEnd)
{
	for(uint32_t addr=addrStart; addr<=addrEnd; ++addr)
	{
		oneTimeBreakPoints[addr].flags=0;
	}
}

void MC6809::Debugger::CheckBreakCondition(const MC6809 &cpu,const MemoryAccess &mem)
{
	if(0!=breakPoints[cpu.state.PC].flags)
	{
		++breakPoints[cpu.state.PC].passed;
		lastBreakPointInfo=breakPoints[cpu.state.PC];
		if(breakPoints[cpu.state.PC].passed<breakPoints[cpu.state.PC].passCount)
		{
			if(0==(breakPoints[cpu.state.PC].flags&BRKPNT_FLAG_SILENT_UNTIL_BREAK))
			{
				hitMonitorPoint=true;
				stop=true;
			}
		}
		else if(0!=(breakPoints[cpu.state.PC].flags&BRKPNT_FLAG_MONITOR_ONLY))
		{
			hitMonitorPoint=true;
			stop=true;
		}
		else
		{
			hitMonitorPoint=false;
			stop=true;
		}
	}
	if(0!=oneTimeBreakPoints[cpu.state.PC].flags)
	{
		++oneTimeBreakPoints[cpu.state.PC].passed;
		lastBreakPointInfo=oneTimeBreakPoints[cpu.state.PC];
		if(oneTimeBreakPoints[cpu.state.PC].passed<oneTimeBreakPoints[cpu.state.PC].passCount)
		{
			if(0==(oneTimeBreakPoints[cpu.state.PC].flags&BRKPNT_FLAG_SILENT_UNTIL_BREAK))
			{
				hitMonitorPoint=true;
			}
		}
		else
		{
			oneTimeBreakPoints[cpu.state.PC].flags=0;
			for(int addr=cpu.state.PC-1; 0<=addr; --addr)
			{
				if(0==oneTimeBreakPoints[cpu.state.PC].flags)
				{
					break;
				}
				oneTimeBreakPoints[cpu.state.PC].flags=0;
			}
			for(int addr=cpu.state.PC+1; 0<=addr; --addr)
			{
				if(0==oneTimeBreakPoints[cpu.state.PC].flags)
				{
					break;
				}
				oneTimeBreakPoints[cpu.state.PC].flags=0;
			}
			stop=true;
		}
	}
}



void MC6809::Debugger::SetBreakOnMemWrite(uint16_t addr,uint8_t min,uint8_t max,uint8_t flags)
{
	memWrite[addr].flags=flags;
	memWrite[addr].min=min;
	memWrite[addr].max=max;
}
void MC6809::Debugger::ClearBreakOnMemWrite(uint16_t addr)
{
	memWrite[addr].flags=0;
}
void MC6809::Debugger::SetBreakOnMemRead(uint16_t addr,uint8_t min,uint8_t max,uint8_t flags)
{
	memRead[addr].flags=flags;
	memRead[addr].min=min;
	memRead[addr].max=max;
}
void MC6809::Debugger::ClearBreakOnMemRead(uint16_t addr)
{
	memRead[addr].flags=0;
}
void MC6809::Debugger::FetchByte(uint16_t addr,uint8_t data)
{
}
void MC6809::Debugger::FetchWord(uint16_t addr,uint16_t data)
{
}
void MC6809::Debugger::StoreByte(uint16_t addr,uint8_t data)
{
}
void MC6809::Debugger::StoreWord(uint16_t addr,uint16_t data)
{
}

void MC6809::Debugger::BeforeRunOneInstructionMainBody(MC6809 &cpu,const MemoryAccess &mem)
{
	auto PCLog=usingPCLog->log;
	auto &PCLogPtr=usingPCLog->ptr;

	PCLog[PCLogPtr].mainOrSub=this->mainOrSub;

	if(true==logAllRegisters)
	{
		PCLog[PCLogPtr].regs=static_cast<RegisterSet>(cpu.state);
	}
	else
	{
		PCLog[PCLogPtr].regs.PC=cpu.state.PC;
		PCLog[PCLogPtr].regs.S=cpu.state.S;
		PCLog[PCLogPtr].regs.CC=cpu.state.CC;
	}

	if(true==logDisassembly)
	{
		PCLog[PCLogPtr].disasm=cpu.DecoratedDisassembly(mem,cpu.state.PC,false,false);
	}

	PCLog[PCLogPtr].count=1;
	auto &prevPCLog=PCLog[(PCLogPtr+PC_LOG_MASK)&PC_LOG_MASK];
	if(prevPCLog==PCLog[PCLogPtr])
	{
		++prevPCLog.count;
	}
	else if(PCLog[(PCLogPtr+PC_LOG_MASK-2)&PC_LOG_MASK]==PCLog[(PCLogPtr+PC_LOG_MASK)&PC_LOG_MASK] &&
	        PCLog[(PCLogPtr+PC_LOG_MASK-1)&PC_LOG_MASK]==PCLog[PCLogPtr])
	{
		// PC_LOG_MASK=length-1
		// Pattern like:
		//    BUSYWAIT:  LDA $FD05
		//               BMI BUSYWAIT
		// In that case execution should be:
		//    BUSYWAIT:  LDA $FD05          PCLog[currentPtr+PC_LOG_MASK-2]
		//               BMI BUSYWAIT       PCLog[currentPtr+PC_LOG_MASK-1]
		//    BUSYWAIT:  LDA $FD05          PCLog[currentPtr+PC_LOG_MASK]
		//               BMI BUSYWAIT       PCLog[currentPtr]
		++PCLog[(PCLogPtr+PC_LOG_MASK-2)&PC_LOG_MASK].count;
		++PCLog[(PCLogPtr+PC_LOG_MASK-1)&PC_LOG_MASK].count;
		PCLogPtr=(PCLogPtr+PC_LOG_MASK-0)&PC_LOG_MASK;
	}
	else if(PCLog[(PCLogPtr+PC_LOG_MASK-4)&PC_LOG_MASK]==PCLog[(PCLogPtr+PC_LOG_MASK-1)&PC_LOG_MASK] &&
	        PCLog[(PCLogPtr+PC_LOG_MASK-3)&PC_LOG_MASK]==PCLog[(PCLogPtr+PC_LOG_MASK)&PC_LOG_MASK] &&
	        PCLog[(PCLogPtr+PC_LOG_MASK-2)&PC_LOG_MASK]==PCLog[PCLogPtr])
	{
		// PC_LOG_MASK=length-1
		// Pattern like:
		//    BUSYWAIT:  LDA $FD02
		//               ANDA #2
		//               BNE BUSYWAIT
		// In that case execution should be:
		//    BUSYWAIT:  LDA $FD02           PCLog[currentPtr+PC_LOG_MASK-4]
		//               ANDA #2             PCLog[currentPtr+PC_LOG_MASK-3]
		//               BNE BUSYWAIT        PCLog[currentPtr+PC_LOG_MASK-2]
		//    BUSYWAIT:  LDA $FD02           PCLog[currentPtr+PC_LOG_MASK-1]
		//               ANDA #2             PCLog[currentPtr+PC_LOG_MASK]
		//               BNE BUSYWAIT        PCLog[currentPtr]
		++PCLog[(PCLogPtr+PC_LOG_MASK-4)&PC_LOG_MASK].count;
		++PCLog[(PCLogPtr+PC_LOG_MASK-3)&PC_LOG_MASK].count;
		++PCLog[(PCLogPtr+PC_LOG_MASK-2)&PC_LOG_MASK].count;
		PCLogPtr=(PCLogPtr+PC_LOG_MASK-1)&PC_LOG_MASK;
	}
	else if(PCLog[(PCLogPtr+PC_LOG_MASK-6)&PC_LOG_MASK]==PCLog[(PCLogPtr+PC_LOG_MASK-2)&PC_LOG_MASK] &&
	        PCLog[(PCLogPtr+PC_LOG_MASK-5)&PC_LOG_MASK]==PCLog[(PCLogPtr+PC_LOG_MASK-1)&PC_LOG_MASK] &&
	        PCLog[(PCLogPtr+PC_LOG_MASK-4)&PC_LOG_MASK]==PCLog[(PCLogPtr+PC_LOG_MASK)&PC_LOG_MASK] &&
	        PCLog[(PCLogPtr+PC_LOG_MASK-3)&PC_LOG_MASK]==PCLog[PCLogPtr])
	{
		++PCLog[(PCLogPtr+PC_LOG_MASK-6)&PC_LOG_MASK].count;
		++PCLog[(PCLogPtr+PC_LOG_MASK-5)&PC_LOG_MASK].count;
		++PCLog[(PCLogPtr+PC_LOG_MASK-4)&PC_LOG_MASK].count;
		++PCLog[(PCLogPtr+PC_LOG_MASK-3)&PC_LOG_MASK].count;
		PCLogPtr=(PCLogPtr+PC_LOG_MASK-2)&PC_LOG_MASK;
	}
	else
	{
		PCLogPtr=(PCLogPtr+1)&PC_LOG_MASK;
	}

	if(true==monitorBIOSTapeRead)
	{
		if(0xF39C==cpu.state.PC || 0xF4A0==cpu.state.PC)
		{
			uint8_t opCode=mem.NonDestructiveFetchByte(cpu.state.PC);
			uint8_t operand=mem.NonDestructiveFetchByte(cpu.state.PC+1);
			if(0xA7==opCode && 0x02==operand)
			{
				std::cout << cpputil::Ubtox(cpu.state.A()) << std::endl;
			}
		}
	}
}
std::vector <MC6809::Debugger::PCLogType> MC6809::Debugger::GetPCLog(void)
{
	return GetPCLog((unsigned int)PC_LOG_SIZE);
}

std::vector <MC6809::Debugger::PCLogType> MC6809::Debugger::GetPCLog(unsigned int steps)
{
	auto PCLog=usingPCLog->log;
	auto &PCLogPtr=usingPCLog->ptr;

	std::vector <PCLogType> list;
	unsigned int offset=PC_LOG_MASK;
	for(unsigned int i=0; i<steps && i<PC_LOG_SIZE; ++i)
	{
		auto idx=(PCLogPtr+offset)&PC_LOG_MASK;
		list.push_back(PCLog[idx]);
		--offset;
	}
	return list;
}
void MC6809::Debugger::PushCallStack(uint8_t callType,uint16_t S,uint16_t fromPC,uint16_t instLength,uint16_t toPC)
{
	CallStack cst;
	cst.type=callType;
	cst.instLength=instLength;
	cst.fromS=S;
	cst.fromPC=fromPC;
	cst.toPC=toPC;
	callStack.push_back(cst);
}
void MC6809::Debugger::PopCallStack(uint16_t S,uint16_t returnPC)
{
	if(true!=callStack.empty())
	{
		int nPop=1;
		bool match=false;
		for(auto iter=callStack.rbegin(); iter!=callStack.rend(); ++iter)
		{
			// I have seen jump by RTS, in which case, none of the above calls should match.
			if(iter->fromPC+iter->instLength==returnPC)
			{
				match=true;
				break;
			}
			// OS-9 uses one byte after SWI2 as function code.
			if(iter->type==CALLTYPE_SWI2 && iter->fromPC+iter->instLength+1==returnPC)
			{
				match=true;
				break;
			}
			++nPop;
		}

		if(true!=match) // Prob: Jump by RTS?
		{
			nPop=0;
		}
		while(0<nPop)
		{
			callStack.pop_back();
			--nPop;
		}
	}
}
std::vector <std::string> MC6809::Debugger::GetCallStackText(void) const
{
	// auto &symTable=GetSymTable();   I've got to do it.
	std::vector <std::string> text;
	for(auto &s : callStack)
	{
		std::string str;
		str+="FR="+cpputil::Ustox(s.fromPC)+"  ";
		str+="TO="+cpputil::Ustox(s.toPC)+"  ";
		str+="RET="+cpputil::Ustox(s.fromPC+s.instLength);
		// auto symbolPtr=symTable.Find(s.procCS,s.procEIP);
		//if(nullptr!=symbolPtr && (SYM_PROCEDURE==symbolPtr->symType || SYM_ANY==symbolPtr->symType))
		//{
		//	str+="{";
		//	str+=symbolPtr->Format();
		//	str+="}";
		//}

		switch(s.type)
		{
		case CALLTYPE_BSR_JSR:
			break;
		case CALLTYPE_NMI:
			str+=" NMI";
			break;
		case CALLTYPE_IRQ:
			str+=" IRQ";
			break;
		case CALLTYPE_FIRQ:
			str+=" FIRQ";
			break;
		case CALLTYPE_SWI:
			str+=" SWI";
			break;
		case CALLTYPE_SWI2:
			str+=" SWI2";
			break;
		case CALLTYPE_SWI3:
			str+=" SWI3";
			break;
		}
		text.push_back((std::string &&)str);
	}
	return text;
}
void MC6809::Debugger::ClearCallStack(void)
{
	callStack.clear();
}
unsigned int MC6809::Debugger::GetInstructionLength(const MC6809 &cpu,const MemoryAccess &mem,uint16_t PC) const
{
	auto found=symTable.Find(PC);
	if(nullptr!=found && MC6809Symbol::SYM_RAW_DATA==found->symType)
	{
		return found->rawDataBytes;
	}
	auto inst=cpu.NonDestructiveFetchInstruction(mem,PC);
	if(true==OS9Mode && INST_SWI2==inst.opCode)
	{
		return 3;
	}
	return inst.length;
}
