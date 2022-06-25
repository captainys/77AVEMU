#include <algorithm>
#include <iostream>
#include "fm77av.h"
#include "fm77avdef.h"



FM77AV::FM77AV() :
	Device(this),
	physMem(this),
	mainMemAcc(this,&physMem),
	mainCPU(this),
	subMemAcc(this,&physMem),
	subCPU(this)
{
}

bool FM77AV::SetUp(FM77AVParam &param)
{
	state.machineType=param.machineType;
	if(true!=LoadROMFiles(param.ROMPath))
	{
		std::cout << "Failed to load ROM files." << std::endl;
		return false;
	}

	for(int i=0; i<32; ++i)
	{
		//	MAINSYS_BOOT_ROM_END=       0x3FFE0,
		physMem.state.data[PhysicalMemory::MAINSYS_INT_VECTOR+i]=physMem.ROM_BOOT_BASIC[0x1E0+i];
	}

	physMem.state.data[PhysicalMemory::SUBSYS_RESET_VECTOR  ]=0xE0;
	physMem.state.data[PhysicalMemory::SUBSYS_RESET_VECTOR+1]=0x00;

	physMem.state.data[PhysicalMemory::MAINSYS_RESET_VECTOR  ]=0xFE;
	physMem.state.data[PhysicalMemory::MAINSYS_RESET_VECTOR+1]=0x00;

	Reset();
	return true;
}

bool FM77AV::LoadROMFiles(std::string ROMPath)
{
	return physMem.LoadROMFiles(ROMPath);
}

void FM77AV::PowerOn(void)
{
	Reset();
}
void FM77AV::Reset(void)
{
	// Sub-CPU reset vector is based on:
	//   F-BASIC Analysis Manual Phase III Sub-System, Fig. 2.2.1, pp. 31, Shuwa System Trading, 
	switch(state.machineType)
	{
	case MACHINETYPE_FM7:
		mainCPU.state.PC=0xFE00;
		subCPU.state.PC=0xE000;
		break;
	case MACHINETYPE_FM77AV:
	case MACHINETYPE_FM77AV40:
		mainCPU.state.PC=0x6000;
		subCPU.state.PC=0xE000;
		break;
	}
}
unsigned int FM77AV::RunOneInstruction(void)
{
	if(true==mainCPU.state.halt && true==subCPU.state.halt)
	{
		return 2; // 2 is the minimum clocks, which is the minimum time resolution.
	}
	else if(true==subCPU.state.halt)
	{
		return mainCPU.RunOneInstruction(mainMemAcc);
	}
	else if(true==mainCPU.state.halt)
	{
		return subCPU.RunOneInstruction(subMemAcc);
	}
	else
	{
		unsigned int clocksPassed=0;

		if(0==state.clockBalance)
		{
			int mainCPUClk=mainCPU.RunOneInstruction(mainMemAcc);
			int subCPUClk=subCPU.RunOneInstruction(subMemAcc);
			state.clockBalance=mainCPUClk-subCPUClk;
			return std::max(mainCPUClk,subCPUClk);
		}
		else if(0<state.clockBalance) // Main CPU ahead
		{
			state.clockBalance-=subCPU.RunOneInstruction(subMemAcc);
			return (0<state.clockBalance ? 0 : -state.clockBalance);
		}
		else // if(state.clockBalance<0) // Sub CPU ahead
		{
			state.clockBalance+=mainCPU.RunOneInstruction(mainMemAcc);
			return (state.clockBalance<0 ? 0 : state.clockBalance);
		}

		return clocksPassed;
	}
}

std::string FM77AV::MachineTypeStr(void) const
{
	return ::MachineTypeToStr(state.machineType);
}
