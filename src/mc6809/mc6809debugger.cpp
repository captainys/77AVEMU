#include "mc6809.h"

void MC6809::Debugger::ClearStopFlag(void)
{
	stop=false;
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
