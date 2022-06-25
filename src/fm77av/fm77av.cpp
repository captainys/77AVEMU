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

MC6809 &FM77AV::CPU(unsigned int mainOrSub)
{
	if(CPU_SUB==mainOrSub)
	{
		return subCPU;
	}
	return mainCPU;
}
MemoryAccess &FM77AV::MemAccess(unsigned int mainOrSub)
{
	if(CPU_SUB==mainOrSub)
	{
		return subMemAcc;
	}
	return mainMemAcc;
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
	state.subSysBusy=true; // Busy on reset.
	state.subSysHalt=false;
	state.mainToSubIRQ=false;
}
bool FM77AV::SubCPUHalt(void) const
{
	return true==subCPU.state.halt ||
	       true==state.subSysHalt;
	       // || (machine type==FM-7, VRAM-access is set, VSYNC=false);
}
bool FM77AV::ExternalDevicePresent(void) const
{
	return false; // Tentatively no external devices.
}
unsigned int FM77AV::RunOneInstruction(void)
{
	unsigned int clocksPassed=0;
	uint64_t nanosec=0;
	if(true==mainCPU.state.halt && true==SubCPUHalt())
	{
		clocksPassed=2; // 2 is the minimum clocks, which is the minimum time resolution.
		nanosec=FM77AVTIME_MICROSEC;
		state.timeBalance=0;
	}
	else if(true==SubCPUHalt())
	{
		clocksPassed=mainCPU.RunOneInstruction(mainMemAcc);

		nanosec=clocksPassed;
		nanosec*=SCALE_NANO;
		nanosec/=mainCPU.state.freq;
	}
	else if(true==mainCPU.state.halt)
	{
		clocksPassed=subCPU.RunOneInstruction(subMemAcc);

		nanosec=clocksPassed;
		nanosec*=SCALE_NANO;
		nanosec/=mainCPU.state.freq;
	}
	else
	{
		unsigned int clocksPassed=0;

		if(0==state.timeBalance)
		{
			int64_t mainCPUClk=mainCPU.RunOneInstruction(mainMemAcc);
			int64_t subCPUClk=subCPU.RunOneInstruction(subMemAcc);

			int64_t mainCPUTime=(mainCPUClk*SCALE_NANO)/mainCPU.state.freq;
			int64_t subCPUTime=(subCPUClk*SCALE_NANO)/subCPU.state.freq;

			state.timeBalance=mainCPUTime-subCPUTime;
			nanosec=std::max(mainCPUTime,subCPUTime);
		}
		else if(0<state.timeBalance) // Main CPU ahead
		{
			int64_t subCPUClk=subCPU.RunOneInstruction(subMemAcc);
			int64_t subCPUTime=(subCPUClk*SCALE_NANO)/subCPU.state.freq;
			state.timeBalance-=subCPUTime;
			nanosec=(0<state.timeBalance ? 0 : -state.timeBalance);
		}
		else // if(state.timeBalance<0) // Sub CPU ahead
		{
			int64_t mainCPUClk=mainCPU.RunOneInstruction(mainMemAcc);
			int64_t mainCPUTime=(mainCPUClk*SCALE_NANO)/mainCPU.state.freq;
			state.timeBalance+=mainCPUTime;
			nanosec=(state.timeBalance<0 ? 0 : state.timeBalance);
		}
	}

	state.fm77avTime+=nanosec;

	return nanosec;
}

std::string FM77AV::MachineTypeStr(void) const
{
	return ::MachineTypeToStr(state.machineType);
}
