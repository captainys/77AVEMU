#include "mc6809.h"

MC6809::Debugger::Debugger()
{
	for(auto &f : memRead)
	{
		f.flags=0;
	}
	for(auto &f : memWrite)
	{
		f.flags=0;
	}
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
			}
		}
		else if(0!=(breakPoints[cpu.state.PC].flags&BRKPNT_FLAG_MONITOR_ONLY))
		{
			hitMonitorPoint=true;
		}
		else
		{
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
	memWrite[addr].flags=0;
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
