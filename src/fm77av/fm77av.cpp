#include "fm77av.h"



FM77AV::FM77AV() :
	Device(this),
	physMem(this),
	mainMemAcc(this,&physMem),
	mainCPU(this),
	subMemAcc(this,&physMem),
	subCPU(this)
{
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
	unsigned int clocksPassed=0;
	
	return clocksPassed;
}

std::string FM77AV::MachineTypeString(void) const
{
	switch(state.machineType)
	{
	case MACHINETYPE_FM7:
		return "FM-7";
	case MACHINETYPE_FM77AV:
		return "FM77AV";
	case MACHINETYPE_FM77AV40:
		return "FM77AV40";
	}
	return "Unknown";
}
